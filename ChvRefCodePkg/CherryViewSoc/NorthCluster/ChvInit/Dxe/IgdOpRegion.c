/** @file
  This is part of the implementation of an Intel Graphics drivers OpRegion /
  Software SCI interface between system BIOS, ASL code, and Graphics drivers.
  The code in this file will load the driver and initialize the interface

  Supporting Specifiction: OpRegion / Software SCI SPEC 0.70

  Acronyms:
    IGD:        Internal Graphics Device
    NVS:        ACPI Non Volatile Storage
    OpRegion:   ACPI Operational Region
    VBT:        Video BIOS Table (OEM customizable data)

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// Include files
//
#include "IgdOpRegion.h"
#include "ChvInit.h"
#include <Uefi.h>
#include <PchRegs.h>
#include <Protocol/IgdOpRegion.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/PlatformGopPolicy.h>
#include <Protocol/PciIo.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/GopComponentName2.h>
#include <Protocol/PlatformGopPolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/IoLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/DriverBinding.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

UINT8 gSVER[12] = "Intel";

GLOBAL_REMOVE_IF_UNREFERENCED GOP_COMPONENT_NAME2_PROTOCOL  *GopComponentName2Protocol = NULL;

DXE_CHV_PLATFORM_POLICY_PROTOCOL  *DxePlatformSaPolicy;

//
// Global variables
//
IGD_OPREGION_PROTOCOL mIgdOpRegion;
EFI_EVENT             mConOutEvent;
EFI_EVENT             mSetGOPverEvent;
VOID                  *mConOutReg;

/**
  Get VBT data.

  @param[in] VbtFileBuffer    Pointer to VBT data buffer.

  @retval EFI_SUCCESS      VBT data was returned.
  @retval EFI_NOT_FOUND    VBT data not found.
  @exception EFI_UNSUPPORTED  Invalid signature in VBT data.

**/
EFI_STATUS
GetIntegratedIntelVbtPtr (
  OUT VBIOS_VBT_STRUCTURE **VbtFileBuffer
  )
{
  EFI_STATUS Status;
  PLATFORM_GOP_POLICY_PROTOCOL  *PlatformGopPolicy;
  EFI_PHYSICAL_ADDRESS          VbtAddress;
  UINT32                        VbtSize;
  
  Status = gBS->LocateProtocol (
                  &gPlatformGOPPolicyGuid,
                  NULL,
                  (VOID **) &PlatformGopPolicy
                  );
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "ERROR: gPlatformGOPPolicyGuid not Found. \n"));
    DEBUG ((EFI_D_ERROR, "ERROR: VBT not Found. \n"));
    return Status;    
  }

  Status = PlatformGopPolicy->GetVbtData(&VbtAddress, &VbtSize);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "ERROR: VBT not Found. \n"));
    return Status;    
  }

  //
  // Check VBT signature
  //
  *VbtFileBuffer = (VBIOS_VBT_STRUCTURE *) (UINTN) VbtAddress;
  if (*VbtFileBuffer != NULL) {
    if ((*((UINT32 *) ((*VbtFileBuffer)->HeaderSignature))) != VBT_SIGNATURE) {
       if (*VbtFileBuffer != NULL) {
        *VbtFileBuffer = NULL;
       }
      return EFI_UNSUPPORTED;
    }
    
    //
    // Check VBT size
    //
    if ((*VbtFileBuffer)->HeaderVbtSize > VbtSize) {
      (*VbtFileBuffer)->HeaderVbtSize = (UINT16) VbtSize;
    }

    Status = EFI_SUCCESS;
  }

  return Status;
}

//
// Function implementations.
//

/**
  Get a pointer to an uncompressed image of the Intel video BIOS.

  Note: This function would only be called if the video BIOS at 0xC000 is
        missing or not an Intel video BIOS.  It may not be an Intel video BIOS
        if the Intel graphic contoller is considered a secondary adapter.

  @param[in]

  VBiosROMImage Pointer to an uncompressed Intel video BIOS.  This pointer must
                be set to NULL if an uncompressed image of the Intel Video BIOS
                is not obtainable.

  @retval

  EFI_SUCCESS   VBiosPtr is updated.

**/
EFI_STATUS
GetIntegratedIntelVBiosPtr (
  INTEL_VBIOS_OPTION_ROM_HEADER **VBiosImage
  )
{
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       HandleCount;
  UINTN                       Index;
  INTEL_VBIOS_PCIR_STRUCTURE  *PcirBlockPtr;
  EFI_STATUS                  Status;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  INTEL_VBIOS_OPTION_ROM_HEADER *VBiosRomImage;
  //
  // Set as if an umcompressed Intel video BIOS image was not obtainable.
  //
  VBiosRomImage = NULL;
  *VBiosImage = NULL;
  //
  // Get all PCI IO protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Find the video BIOS by checking each PCI IO handle for an Intel video
  // BIOS OPROM.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    ASSERT_EFI_ERROR (Status);

    VBiosRomImage = PciIo->RomImage;

    //
    // If this PCI device doesn't have a ROM image, skip to the next device.
    //
    if (!VBiosRomImage) {
      continue;
    }

    //
    // Get pointer to PCIR structure
    //
    PcirBlockPtr = (INTEL_VBIOS_PCIR_STRUCTURE *)((UINT8 *) VBiosRomImage + VBiosRomImage->PcirOffset);

    //
    // Check if we have an Intel video BIOS OPROM.
    //
    if ((VBiosRomImage->Signature == OPTION_ROM_SIGNATURE) &&
        (PcirBlockPtr->VendorId == MC_VID) &&
        (PcirBlockPtr->ClassCode[0] == 0x00) &&
        (PcirBlockPtr->ClassCode[1] == 0x00) &&
        (PcirBlockPtr->ClassCode[2] == 0x03)
        ) {
      //
      // Found Intel video BIOS.
      //
      *VBiosImage = VBiosRomImage;
      return EFI_SUCCESS;
    }
  }

  //
  // No Intel video BIOS found.
  //

  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);
  return EFI_UNSUPPORTED;
}

/**
  Get Intel GOP driver version and copy it into IGD OpRegion GVER. This version
  is picked up by IGD driver and displayed in CUI.

  @retval  EFI_SUCCESS       Video BIOS VBT information returned.
  @retval  EFI_UNSUPPORTED   Could not find VBT information (*VBiosVbtPtr = NULL).

**/
EFI_STATUS
EFIAPI
GetGOPVersion (
  VOID
  )
{
  CHAR16                    *DriverVersion;
  UINTN                     Index;
  EFI_STATUS                Status;

  ///
  /// Locate Gop Component Name2 Protocol.
  ///
  Status = gBS->LocateProtocol (&gGopComponentName2ProtocolGuid, NULL, (VOID **)&GopComponentName2Protocol);
  if (!EFI_ERROR (Status)) {
    Status = GopComponentName2Protocol->GetDriverVersion (
                                  GopComponentName2Protocol,
                                  "en-US",
                                  &DriverVersion
                                  );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; (DriverVersion[Index] != '\0'); Index++) {
      }
        Index = (Index+1)*2;
        CopyMem((CHAR16*)&(mIgdOpRegion.OpRegion->Header.GOPV[0]), DriverVersion, Index);
        return Status;
    }
  }

  return EFI_UNSUPPORTED;
}

/**
  Get Intel video BIOS VBT information (i.e. Pointer to VBT and VBT size).
  The VBT (Video BIOS Table) is a block of customizable data that is built
  within the video BIOS and edited by customers.

  @param[in]    Event             A pointer to the Event that triggered the callback.
  @param[in]	Context           A pointer to private data registered with the callback function.

  @retval  EFI_SUCCESS       Video BIOS VBT information returned.
  @retval  EFI_UNSUPPORTED   Could not find VBT information (*VBiosVbtPtr = NULL).

**/
EFI_STATUS
EFIAPI
GetVBiosVbtCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  INTEL_VBIOS_PCIR_STRUCTURE    *PcirBlockPtr;
  UINT16                        PciVenderId;
  UINT16                        PciDeviceId;
  INTEL_VBIOS_OPTION_ROM_HEADER *VBiosPtr;
  VBIOS_VBT_STRUCTURE           *VBiosVbtPtr;
  VBIOS_VBT_STRUCTURE           *VbtFileBuffer = NULL;

  VBiosPtr = (INTEL_VBIOS_OPTION_ROM_HEADER *)(UINTN)(VBIOS_LOCATION_PRIMARY);
  PcirBlockPtr = (INTEL_VBIOS_PCIR_STRUCTURE *)((UINT8 *)VBiosPtr + VBiosPtr->PcirOffset);
  PciVenderId = PcirBlockPtr->VendorId;
  PciDeviceId = PcirBlockPtr->DeviceId;

  //
  // If the video BIOS is not at 0xC0000 or it is not an Intel video BIOS get
  // the integrated Intel video BIOS (must be uncompressed).
  //
  if ((VBiosPtr->Signature != OPTION_ROM_SIGNATURE) || (PciVenderId != MC_VID) || ((PciDeviceId != IGD_DID) && (PciDeviceId != IGD_DID_C0))) {
    GetIntegratedIntelVBiosPtr (&VBiosPtr);

    if (VBiosPtr) {
      //
      // Video BIOS found.
      //
      PcirBlockPtr = (INTEL_VBIOS_PCIR_STRUCTURE *)((UINT8 *)VBiosPtr + VBiosPtr->PcirOffset);
      PciVenderId = PcirBlockPtr->VendorId;
      if ( (VBiosPtr->Signature != OPTION_ROM_SIGNATURE) || (PciVenderId != MC_VID)) {
        //
        // Intel video BIOS not found.
        //
        VBiosVbtPtr = NULL;
        return EFI_UNSUPPORTED;
      }
    } else {
      //
      // No Video BIOS found, try to get VBT from FV.
      //
      GetIntegratedIntelVbtPtr (&VbtFileBuffer);

      if (VbtFileBuffer != NULL) {
        //
        // Video BIOS not found, use VBT from FV
        //
        DEBUG ((EFI_D_ERROR, "VBT data found\n"));
        (gBS->CopyMem) (mIgdOpRegion.OpRegion->VBT.GVD1, VbtFileBuffer, VbtFileBuffer->HeaderVbtSize);
        FreePool (VbtFileBuffer);

        //
        // Get GOP Version details
        //
        GetGOPVersion ();
        return EFI_SUCCESS;
      }
    }

    if ((VBiosPtr == NULL) ) {
      //
      // Intel video BIOS not found.
      //
      VBiosVbtPtr = NULL;
      return EFI_UNSUPPORTED;
    }
  }

  DEBUG ((EFI_D_ERROR, "VBIOS found at 0x%X\n", VBiosPtr));
  VBiosVbtPtr = (VBIOS_VBT_STRUCTURE *) ((UINT8 *) VBiosPtr + VBiosPtr->VbtOffset);

  if ((*((UINT32 *) (VBiosVbtPtr->HeaderSignature))) != VBT_SIGNATURE) {
    return EFI_UNSUPPORTED;
  }

  //
  // Initialize Video BIOS version with its build number.
  //
  mIgdOpRegion.OpRegion->Header.VVER[0] = VBiosVbtPtr->CoreBlockBiosBuild[0];
  mIgdOpRegion.OpRegion->Header.VVER[1] = VBiosVbtPtr->CoreBlockBiosBuild[1];
  mIgdOpRegion.OpRegion->Header.VVER[2] = VBiosVbtPtr->CoreBlockBiosBuild[2];
  mIgdOpRegion.OpRegion->Header.VVER[3] = VBiosVbtPtr->CoreBlockBiosBuild[3];
  (gBS->CopyMem) (mIgdOpRegion.OpRegion->VBT.GVD1, VBiosVbtPtr, VBiosVbtPtr->HeaderVbtSize);

  //
  // Return final status
  //
  return EFI_SUCCESS;
}

/**
  Graphics OpRegion / Software SCI driver installation function.

  @param[in]   DxePlatformSaPolicy - SA DxePlatformPolicy protocol
   
  @retval  EFI_SUCCESS     The driver installed without error.
  @retval  EFI_ABORTED     The driver encountered an error and could not complete
                  installation of the ACPI tables.

**/
EFI_STATUS
IgdOpRegionInit (
  void
  )
{
  EFI_HANDLE                    Handle;
  EFI_STATUS                    Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
  UINT32                        DwordData;
  EFI_CPU_IO2_PROTOCOL          *CpuIo;
  UINT16                        Data16;
  UINT16                        AcpiBase;
  VOID                          *gConOutNotifyReg;
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  Address = 0xffffffff;

  //
  //  Locate the Global NVS Protocol.
  //
  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &GlobalNvsArea
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Locate SA DXE Platform Policy
  //
  Status = gBS->LocateProtocol (&gDxeChvPlatformPolicyGuid, NULL, (VOID **) &DxePlatformSaPolicy);
  ASSERT_EFI_ERROR (Status);
  //
  // Allocate an ACPI NVS memory buffer as the IGD OpRegion, zero initialize
  // the first 1K, and set the IGD OpRegion pointer in the Global NVS
  // area structure.
  //

  Pages = EFI_SIZE_TO_PAGES ( sizeof (IGD_OPREGION_STRUC) );
  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address 
                   );
  ASSERT_EFI_ERROR (Status);

  mIgdOpRegion.OpRegion = (IGD_OPREGION_STRUC *)(UINTN)Address;

  (gBS->SetMem) (mIgdOpRegion.OpRegion, sizeof (IGD_OPREGION_STRUC), 0);
  GlobalNvsArea->Area->IgdOpRegionAddress = (UINT32)(UINTN)(mIgdOpRegion.OpRegion);

  //
  // If IGD is disabled return
  //
  if (IgdMmPci32 (0) == 0xFFFFFFFF) {
  	return EFI_SUCCESS;
  }

  //
  // Initialize OpRegion Header
  //

  (gBS->CopyMem) (mIgdOpRegion.OpRegion->Header.SIGN, HEADER_SIGNATURE, sizeof(HEADER_SIGNATURE));

  //
  // Set OpRegion Size in KBs
  //
  mIgdOpRegion.OpRegion->Header.SIZE = HEADER_SIZE/1024;

  //
  // FIXME: Need to check Header OVER Field and the supported version.
  //
  mIgdOpRegion.OpRegion->Header.OVER = (UINT32) (LShiftU64 (HEADER_OPREGION_VER, 16) + LShiftU64 (HEADER_OPREGION_REV, 8));
  gBS->CopyMem(mIgdOpRegion.OpRegion->Header.SVER, gSVER, sizeof(gSVER));
  DEBUG ((EFI_D_ERROR, "System BIOS ID is %a\n", mIgdOpRegion.OpRegion->Header.SVER));

  mIgdOpRegion.OpRegion->Header.MBOX = HEADER_MBOX_SUPPORT;

  if( 1 == DxePlatformSaPolicy->S0ixSupported) {
    //
    // S0ix supported
    //
    mIgdOpRegion.OpRegion->Header.PCON = (mIgdOpRegion.OpRegion->Header.PCON & 0xFFFC) | BIT1;
    DEBUG ((EFI_D_ERROR, "S0ix Supported!\n"));
  } else {
    //
    //S0ix not supported
    //
    mIgdOpRegion.OpRegion->Header.PCON = (mIgdOpRegion.OpRegion->Header.PCON & 0xFFFC) | (BIT1 | BIT0);
    DEBUG ((EFI_D_ERROR, "S0ix not Supported!\n"));
  }

  //
  //For graphics driver to identify if LPE Audio/HD Audio is enabled on the platform
  //
  mIgdOpRegion.OpRegion->Header.PCON &= AUDIO_TYPE_SUPPORT_MASK;
  mIgdOpRegion.OpRegion->Header.PCON &= AUDIO_TYPE_FIELD_MASK;
  if ( NO_AUDIO == DxePlatformSaPolicy->AudioTypeSupport ) {
     mIgdOpRegion.OpRegion->Header.PCON |=NO_AUDIO_SUPPORT;
     DEBUG ((EFI_D_ERROR, "No Audio Support!\n"));
     mIgdOpRegion.OpRegion->Header.PCON |= AUDIO_TYPE_FIELD_VALID;
  } else if ( HD_AUDIO== DxePlatformSaPolicy->AudioTypeSupport ) {
     mIgdOpRegion.OpRegion->Header.PCON = HD_AUDIO_SUPPORT;
     DEBUG ((EFI_D_ERROR, "HD Audio Support!\n"));
     mIgdOpRegion.OpRegion->Header.PCON |= AUDIO_TYPE_FIELD_VALID;
  } else if ( LPE_AUDIO == DxePlatformSaPolicy->AudioTypeSupport ) {
     mIgdOpRegion.OpRegion->Header.PCON |= LPE_AUDIO_SUPPORT;
     DEBUG ((EFI_D_ERROR, "LPE Audio Support!\n"));
     mIgdOpRegion.OpRegion->Header.PCON |= AUDIO_TYPE_FIELD_VALID;
  } else {
    // Audio type support field is not valid
    mIgdOpRegion.OpRegion->Header.PCON |= AUDIO_TYPE_FIELD_INVALID;
    DEBUG ((EFI_D_ERROR, "Audio type support field is not valid!\n"));
  }

  //
  // Initialize OpRegion Mailbox 1 (Public ACPI Methods).
  //
  //<TODO> The initial setting of mailbox 1 fields is implementation specific.
  // Adjust them as needed many even coming from user setting in setup.
  //
  //Workaround to solve LVDS is off after entering OS in desktop platform
  mIgdOpRegion.OpRegion->MBox1.CLID = DxePlatformSaPolicy->IgdPanelFeatures.LidStatus;
  //
  // Initialize OpRegion Mailbox 3 (ASLE Interrupt and Power Conservation).
  //
  //<TODO> The initial setting of mailbox 3 fields is implementation specific.
  // Adjust them as needed many even coming from user setting in setup.
  //

  //
  // Do not initialize TCHE. This field is written by the graphics driver only.
  //

  //
  // The ALSI field is generally initialized by ASL code by reading the embedded controller.
  //

  mIgdOpRegion.OpRegion->MBox3.BCLP = BACKLIGHT_BRIGHTNESS;

  mIgdOpRegion.OpRegion->MBox3.PFIT = (FIELD_VALID_BIT | PFIT_STRETCH);

  //
  // Set Initial current Brightness
  //
  mIgdOpRegion.OpRegion->MBox3.CBLV = (INIT_BRIGHT_LEVEL | FIELD_VALID_BIT);

  //
  // <EXAMPLE> Create a static Backlight Brightness Level Duty cycle Mapping Table
  // Possible 20 entries (example used 10), each 16 bits as follows:
  // [15] = Field Valid bit, [14:08] = Level in Percentage (0-64h), [07:00] = Desired duty cycle (0 - FFh).
  //
  mIgdOpRegion.OpRegion->MBox3.BCLM[0]  = (0x0000 + WORD_FIELD_VALID_BIT);  // 0%
  mIgdOpRegion.OpRegion->MBox3.BCLM[1]  = (0x0A19 + WORD_FIELD_VALID_BIT);  // 10%
  mIgdOpRegion.OpRegion->MBox3.BCLM[2]  = (0x1433 + WORD_FIELD_VALID_BIT);  // 20%
  mIgdOpRegion.OpRegion->MBox3.BCLM[3]  = (0x1E4C + WORD_FIELD_VALID_BIT);  // 30%
  mIgdOpRegion.OpRegion->MBox3.BCLM[4]  = (0x2866 + WORD_FIELD_VALID_BIT);  // 40%
  mIgdOpRegion.OpRegion->MBox3.BCLM[5]  = (0x327F + WORD_FIELD_VALID_BIT);  // 50%
  mIgdOpRegion.OpRegion->MBox3.BCLM[6]  = (0x3C99 + WORD_FIELD_VALID_BIT);  // 60%
  mIgdOpRegion.OpRegion->MBox3.BCLM[7]  = (0x46B2 + WORD_FIELD_VALID_BIT);  // 70%
  mIgdOpRegion.OpRegion->MBox3.BCLM[8]  = (0x50CC + WORD_FIELD_VALID_BIT);  // 80%
  mIgdOpRegion.OpRegion->MBox3.BCLM[9]  = (0x5AE5 + WORD_FIELD_VALID_BIT);  // 90%
  mIgdOpRegion.OpRegion->MBox3.BCLM[10] = (0x64FF + WORD_FIELD_VALID_BIT);  // 100%

  mIgdOpRegion.OpRegion->MBox3.PCFT = ((UINT32) GlobalNvsArea->Area->IgdPowerConservation) | BIT31;
  //
  // Create the notification and register callback function on the PciIo installation,
  //
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  (EFI_EVENT_NOTIFY)GetVBiosVbtCallback,
                  NULL,
                  &mConOutEvent
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;

  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  mConOutEvent,
                  &gConOutNotifyReg
                  );

  //
  // Initialize hardware state:
  //   Set ASLS Register to the OpRegion physical memory address.
  //   Set SWSCI register bit 15 to a "1" to activate SCI interrupts.
  //

  IgdMmPci32 (IGD_ASLS_OFFSET) = (UINT32)(UINTN)(mIgdOpRegion.OpRegion);
  IgdMmPci16AndThenOr (IGD_SWSCI_OFFSET, ~(BIT0), BIT15);

  DwordData = IgdMmPci32 (IGD_ASLS_OFFSET);
  S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      (UINTN) (EFI_PCI_ADDRESS  (IGD_BUS, IGD_DEV, IGD_FUN_0, IGD_ASLS_OFFSET)),
      1,
     &DwordData
    );

  DwordData = IgdMmPci32 (IGD_SWSCI_OFFSET);
  S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      (UINTN) (EFI_PCI_ADDRESS  (IGD_BUS, IGD_DEV, IGD_FUN_0, IGD_SWSCI_OFFSET)),
      1,
     &DwordData
    );

  AcpiBase =  MmPci16 (
                0,
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_LPC,
                PCI_FUNCTION_NUMBER_PCH_LPC,
                R_PCH_LPC_ACPI_BASE
                ) & B_PCH_LPC_ACPI_BASE_BAR;

  //
  // Find the CPU I/O Protocol.  ASSERT if not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiCpuIo2ProtocolGuid,
                  NULL,
                  (VOID **) &CpuIo
                  );
  ASSERT_EFI_ERROR (Status);

  CpuIo->Io.Read (
              CpuIo,
              EfiCpuIoWidthUint16,
              AcpiBase + R_PCH_ACPI_GPE0a_STS,
              1,
              &Data16
              );
  //
  // Clear the B_PCH_ACPI_GPE0a_STS_GUNIT_SCI bit in R_PCH_ACPI_GPE0a_STS by writing a '1'.
  //
  Data16 |= B_PCH_ACPI_GPE0a_STS_GUNIT_SCI;

  CpuIo->Io.Write (
              CpuIo,
              EfiCpuIoWidthUint16,
              AcpiBase + R_PCH_ACPI_GPE0a_STS,
              1,
              &Data16
              );

  //
  // Install OpRegion / Software SCI protocol
  //

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gIgdOpRegionProtocolGuid,
                  &mIgdOpRegion,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Return final status
  //
  return EFI_SUCCESS;
}
