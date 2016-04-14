/** @file
  ACPI Platform Driver.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Protocol/TcgService.h>
#include <Protocol/FirmwareVolume2.h>
#include "AcpiPlatform.h"
#include "Platform.h"
#include <Guid/PlatformInfo.h>
#include <Hpet.h>
#include <Mcfg.h>
#include <Guid/GlobalVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <Protocol/CpuIo.h>
#include <Guid/EventGroup.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiS3Save.h>
#include <Guid/AcpiTableStorage.h>
#include <PchAccess.h>
#include <Guid/Chv2Variable.h>
#include "CpuRegs.h"
#include <Library/KscLib.h>

#define AML_OPREGION_OP   0x80

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

EFI_CPU_IO2_PROTOCOL                    *mCpuIo;

#pragma optimize("", off)

EFI_PLATFORM_INFO_HOB     *mPlatformInfo;
SYSTEM_CONFIGURATION      mSystemConfiguration;

VOID
SettingI2CTouchAddress (
  IN VOID
  );

VOID
LoadSsdtIsctTable (
  VOID
  );

/**
Routine Description:

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param[in]  Protocol              The protocol to find.
  @param[in]  Instance              Return pointer to the first instance of the protocol.
  @param[in]  Type                  The type of protocol to locate.

  @retval  EFI_SUCCESS              The function completed successfully.
  @retval  EFI_NOT_FOUND            The protocol could not be located.
  @retval  EFI_OUT_OF_RESOURCES     There are not enough resources to find the protocol.

**/
EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  Protocol,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }

  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    Protocol,
                    Instance
                    );
    ASSERT (!EFI_ERROR (Status));

    if (!Type) {
      //
      // Not looking for the FV protocol, so find the first instance of the
      // protocol.  There should not be any errors because our handle buffer
      // should always contain at least one or LocateHandleBuffer would have
      // returned not found.
      //
      break;
    }

    //
    // See if it has the ACPI storage file
    //
    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL *) (*Instance))->ReadFile (
                                                                *Instance,
                                                                &gEfiAcpiTableStorageGuid,
                                                                NULL,
                                                                &Size,
                                                                &FileType,
                                                                &Attributes,
                                                                &FvStatus
                                                                );

    //
    // If we found it, then we are done
    //
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}

/**
    Update the DSDT table.

    @param[in][out]  *TableHeader   The table to be set.

    @retval  EFI_SUCCESS            Returns EFI_SUCCESS.

**/
EFI_STATUS
PatchDsdtTable (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER  *TableHeader
  )
{
  UINT8                     *CurrPtr;
  UINT8                     *DsdtPointer;
  UINT32                    *Signature;
  UINT8                     *EndPtr;
  UINT8                     *Operation;
  UINT32                    *Address;
  UINT16                    *Size;

  CurrPtr = (UINT8*) &((EFI_ACPI_DESCRIPTION_HEADER*) TableHeader)[0];
  EndPtr = (UINT8*) TableHeader;
  EndPtr = EndPtr + TableHeader->Length;
  while (CurrPtr < (EndPtr-2)) {
    //
    // Removed the _S3 tag to indicate that we do not support S3. The 4th byte is blank space
    // since there are only 3 char "_S3".
    //
    if (mSystemConfiguration.AcpiSuspendState == 0) {
      //
      // For iasl compiler version 20061109.
      //
      if ((CurrPtr[0] == '_') && (CurrPtr[1] == 'S') && (CurrPtr[2] == '3') && (CurrPtr[3] == '_')) {
        break;
      }
      //
      // For iasl compiler version 20040527.
      //
      if ((CurrPtr[0] == '\\') && (CurrPtr[1] == '_') && (CurrPtr[2] == 'S') && (CurrPtr[3] == '3')) {
        break;
      }
    }
    CurrPtr++;
  }
  EndPtr = (UINT8*) TableHeader;
  EndPtr = EndPtr + TableHeader->Length;
  //
  // Loop through the ASL looking for values that we must fix up.
  //
  CurrPtr = (UINT8 *) TableHeader;
  for (DsdtPointer = CurrPtr; DsdtPointer <= (CurrPtr + ((EFI_ACPI_COMMON_HEADER *) CurrPtr)->Length); DsdtPointer++) {
    Signature = (UINT32 *) DsdtPointer;

    switch (*Signature) {
      //
      // GNVS operation region.
      //
      case (SIGNATURE_32 ('G', 'N', 'V', 'S')):
        //
        // Conditional match.  For Region Objects, the Operator will always be the
        // byte immediately before the specific name.  Therefore, subtract 1 to check
        // the Operator.
        //
        Operation = DsdtPointer - 1;
        if (*Operation == AML_OPREGION_OP) {
          Address   = (UINT32 *) (DsdtPointer + 6);
          *Address  = (UINT32) (UINTN) mGlobalNvsArea.Area;
          Size      = (UINT16 *) (DsdtPointer + 11);
          *Size     = sizeof (EFI_GLOBAL_NVS_AREA);
        }
        break;
      default:
        break;
    }
  }

  return EFI_SUCCESS;
}

/**
Routine Description:

  This function will update any runtime platform specific information.
  This currently includes:Setting OEM table values, ID, table ID, creator ID and creator revision.
  Enabling the proper processor entries in the APIC tables.

  @param[in]   Table          The table to update

  @retval    EFI_SUCCESS      The function completed successfully.

**/
EFI_STATUS
PlatformUpdateTables (
  IN OUT EFI_ACPI_COMMON_HEADER  *Table
  )
{
  EFI_ACPI_DESCRIPTION_HEADER                                 *TableHeader;
  UINT8                                                       *CurrPtr;
  UINT8                                                       *EndPtr;
  ACPI_APIC_STRUCTURE_PTR                                     *ApicPtr;
  UINT8                                                       CurrProcessor;
  EFI_STATUS                                                  Status;
  EFI_MP_SERVICES_PROTOCOL                                    *MpService;
  UINTN                                                       MaximumNumberOfCPUs;
  UINTN                                                       NumberOfEnabledCPUs;
  UINTN                                                       BufferSize;
  ACPI_APIC_STRUCTURE_PTR                                     *ProcessorLocalApicEntry;
  UINTN                                                       BspIndex;
  EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER            *HpetTbl;
  UINT64                                                      OemIdValue;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE                   *pFACP;
  UINT16                                                      NumberOfHpets;
  UINT16                                                      HpetCapIdValue;
  UINT32                                                      HpetBlockID;
  UINTN                                                       LocalApicCounter;
  EFI_PROCESSOR_INFORMATION                                   ProcessorInfoBuffer;
  EFI_ACPI_3_0_IO_APIC_STRUCTURE                              *IOApicType;
  EFI_ACPI_3_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER         *APICTableHeader;

  CurrPtr                 = NULL;
  EndPtr                  = NULL;
  ApicPtr                 = NULL;
  LocalApicCounter        = 0;
  CurrProcessor           = 0;
  ProcessorLocalApicEntry = NULL;

  if (Table->Signature != EFI_ACPI_1_0_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE) {
    TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

    //
    // Update the OEMID
    //
    OemIdValue = 0;
    CopyMem (&OemIdValue, PcdGetPtr (PcdAcpiDefaultOemId), 6);

    *(UINT32 *)(TableHeader->OemId)     = (UINT32) OemIdValue;
    *(UINT16 *)(TableHeader->OemId + 4) = *(UINT16*)(((UINT8 *)&OemIdValue) + 4);

    if ((Table->Signature != EFI_ACPI_2_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE)) {
      //
      // Update the OEM Table ID
      //
      TableHeader->OemTableId = PcdGet64(PcdAcpiDefaultOemTableId);
    }

    //
    // Update the OEM Table ID
    //
    TableHeader->OemRevision = EFI_ACPI_OEM_REVISION;

    //
    // Update the creator ID
    //
    TableHeader->CreatorId = EFI_ACPI_CREATOR_ID;

    //
    // Update the creator revision
    //
    TableHeader->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  }
  //
  // Complete this function
  //
  //
  // Locate the MP services protocol
  //
  //
  // Find the MP Protocol. This is an MP platform, so MP protocol must be
  // there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &MpService
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
               MpService,
               &MaximumNumberOfCPUs,
               &NumberOfEnabledCPUs
               );

  ASSERT (MaximumNumberOfCPUs <= MAX_CPU_NUM && NumberOfEnabledCPUs >= 1);

  //
  // Assign a invalid intial value for update
  //
  //
  // Update the processors in the APIC table
  //
  switch (Table->Signature) {
    case EFI_ACPI_3_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:

      Status = MpService->WhoAmI (
                            MpService,
                            &BspIndex
                            );

      //
      // PCAT_COMPAT Set to 1 indicate 8259 vectors should be disabled
      //
      APICTableHeader = (EFI_ACPI_3_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)Table;
      APICTableHeader->Flags |= EFI_ACPI_3_0_PCAT_COMPAT;

      CurrPtr = (UINT8 *) &((EFI_ACPI_DESCRIPTION_HEADER *) Table)[1];
      CurrPtr = CurrPtr + 8;

      //
      // Size of Local APIC Address & Flag
      //
      EndPtr  = (UINT8 *) Table;
      EndPtr  = EndPtr + Table->Length;
      while (CurrPtr < EndPtr) {
        ApicPtr = (ACPI_APIC_STRUCTURE_PTR *) CurrPtr;
        switch (ApicPtr->AcpiApicCommon.Type) {
          case EFI_ACPI_3_0_PROCESSOR_LOCAL_APIC:
            //
            // ESS override
            // Fix for Ordering of MADT to be maintained as it is in MADT table.
            //
            // Update processor enabled or disabled and keep the local APIC
            // order in MADT intact
            //
            // Sanity check to make sure proc-id is not arbitrary
            //
            DEBUG ((EFI_D_ERROR, "ApicPtr->AcpiLocalApic.AcpiProcessorId = %x, MaximumNumberOfCPUs = %x\n", ApicPtr->AcpiLocalApic.AcpiProcessorId, MaximumNumberOfCPUs));
            if (ApicPtr->AcpiLocalApic.AcpiProcessorId > MaximumNumberOfCPUs) {
              ApicPtr->AcpiLocalApic.AcpiProcessorId = (UINT8)MaximumNumberOfCPUs;
            }

            BufferSize                    = 0;
            ApicPtr->AcpiLocalApic.Flags  = 0;

            for (CurrProcessor = 0; CurrProcessor < MaximumNumberOfCPUs; CurrProcessor++) {
              Status = MpService->GetProcessorInfo (
                                    MpService,
                                    CurrProcessor,
                                    &ProcessorInfoBuffer
                                    );

              if (Status == EFI_SUCCESS && ProcessorInfoBuffer.ProcessorId == ApicPtr->AcpiLocalApic.ApicId) {
                //
                // Check to see whether or not a processor (or thread) is enabled
                //
                if (BspIndex == CurrProcessor || ((ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0)) {
                  //
                  // Go on and check if Hyper Threading is enabled. If HT not enabled
                  // hide this thread from OS by not setting the flag to 1.  This is the
                  // software way to disable Hyper Threading.  Basically we just hide it
                  // from the OS.
                  //
                  ApicPtr->AcpiLocalApic.Flags = EFI_ACPI_1_0_LOCAL_APIC_ENABLED;

                  if (!mSystemConfiguration.ProcessorHtMode) {
                    if (ProcessorInfoBuffer.Location.Thread != 0) {
                      ApicPtr->AcpiLocalApic.Flags = 0;
                    }
                  }

                  if (mSystemConfiguration.ActiveProcessorCores) {
                    if (ProcessorInfoBuffer.Location.Core >= mSystemConfiguration.ActiveProcessorCores) {
                      ApicPtr->AcpiLocalApic.Flags = 0;
                    }
                  }
                }
                break;
              }
            }

            //
            // If no APIC-ID match, the cpu may not be populated
            //
            break;

          case EFI_ACPI_3_0_IO_APIC:

            IOApicType = (EFI_ACPI_3_0_IO_APIC_STRUCTURE *)CurrPtr;
            MmioWrite8 ((UINTN) R_PCH_IO_APIC_INDEX, R_PCH_IO_APIC_ID);
            IOApicType->IoApicId = (UINT8) (MmioRead32 ((UINTN) R_PCH_IO_APIC_WINDOW) >> 24);
            //
            // IO APIC entries can be patched here
            //
            break;
        }

        CurrPtr = CurrPtr + ApicPtr->AcpiApicCommon.Length;
      }
      break;

    case EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
       //
       // if Native ASPM is disabled, set FACP table to skip Native ASPM
       //
      if ((mSystemConfiguration.PciExpNative == 0)) {
        pFACP = (EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *) Table;
        pFACP->IaPcBootArch |= 0x10;
        }
      break;
    case EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
      //
      // Patch the memory resource
      //
      PatchDsdtTable ((EFI_ACPI_DESCRIPTION_HEADER *) Table);
      break;

    case EFI_ACPI_3_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:

      break;

    case EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE:
      //
      // Adjust HPET Table to correct the Base Address
      //
      // Enable HPET always as Hpet.asi always indicates that Hpet is enabled.
      MmioOr8 (R_PCH_PCH_HPET + R_PCH_PCH_HPET_GCFG, B_PCH_PCH_HPET_GCFG_EN);

      HpetTbl = (EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER *) Table;
      HpetTbl->BaseAddressLower32Bit.Address = HPET_BASE_ADDRESS;
      HpetTbl->EventTimerBlockId = *((UINT32*)(UINTN)HPET_BASE_ADDRESS);

      HpetCapIdValue = *(UINT16 *)(UINTN)(HPET_BASE_ADDRESS);
      NumberOfHpets = HpetCapIdValue & B_PCH_PCH_HPET_GCID_NT;  // Bits [8:12] contains the number of Hpets
      HpetBlockID = EFI_ACPI_EVENT_TIMER_BLOCK_ID;

      if ((NumberOfHpets) && (NumberOfHpets & B_PCH_PCH_HPET_GCID_NT)) {
        HpetBlockID |= (NumberOfHpets);
      }
      HpetTbl->EventTimerBlockId = HpetBlockID;

      break;

    case EFI_ACPI_3_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
      //
      // Update MCFG base and end bus number
      //
      ((EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE *) Table)->Segment[0].BaseAddress
        = mPlatformInfo->PciData.PciExpressBase;
      ((EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE *) Table)->Segment[0].EndBusNumber
        = (UINT8)RShiftU64 (mPlatformInfo->PciData.PciExpressSize, 20) - 1;
      break;

    default:
      break;
  }
  //
  //
  // Update the hardware signature in the FACS structure
  //
  //
  // Locate the SPCR table and update based on current settings.
  // The user may change CR settings via setup or other methods.
  // The SPCR table must match.
  //

  return EFI_SUCCESS;
}

/**
Routine Description:

  Entry point for Acpi platform driver.

  @param[in]   ImageHandle         A handle for the image that is initializing this driver.
  @param[in]   SystemTable         A pointer to the EFI system table.

  @retval  EFI_SUCCESS             Driver initialized successfully.
  @retval  EFI_LOAD_ERROR          Failed to Initialize or has been loaded.
  @retval  EFI_OUT_OF_RESOURCES    Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINTN                         TableHandle;
  UINT32                        FvStatus;
  UINT32                        Size;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  EFI_HANDLE                    Handle;
  UINT8                         PortData;
  EFI_MP_SERVICES_PROTOCOL      *MpService;
  UINTN                         MaximumNumberOfCPUs;
  UINTN                         NumberOfEnabledCPUs;
  UINT32                        Data32;
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_EVENT                     EfiExitBootServicesEvent;
  
  Address = 0xffffffff;

  TableVersion      = EFI_ACPI_TABLE_VERSION_2_0;
  Instance          = 0;
  CurrentTable      = NULL;
  TableHandle       = 0;
  Data32            = 0;

  mPlatformInfo = PcdGetPtr (PcdPlatformInfo);

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  CopyMem (&mSystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));
  
  //
  // Locate the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (&gEfiFirmwareVolume2ProtocolGuid, &FwVol, 1);
  ASSERT_EFI_ERROR (Status);

  Instance  = 0;

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &MpService
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
               MpService,
               &MaximumNumberOfCPUs,
               &NumberOfEnabledCPUs
               );

  //
  // Allocate and initialize the NVS area for SMM and ASL communication.
  //
  Pages = EFI_SIZE_TO_PAGES ( sizeof (EFI_GLOBAL_NVS_AREA) );
  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address 
                   );
  ASSERT_EFI_ERROR (Status);
  mGlobalNvsArea.Area = (EFI_GLOBAL_NVS_AREA*) (UINTN)Address;

  gBS->SetMem (mGlobalNvsArea.Area, sizeof (EFI_GLOBAL_NVS_AREA), 0);
  DEBUG ((EFI_D_ERROR, "mGlobalNvsArea.Area is at 0x%X\n", mGlobalNvsArea.Area));

  //
  // Update global NVS area for ASL and SMM init code to use
  //
  mGlobalNvsArea.Area->ApicEnable                 = 1;
  mGlobalNvsArea.Area->NumberOfBatteries          = 1;
  mGlobalNvsArea.Area->BatteryCapacity0           = 100;
  mGlobalNvsArea.Area->BatteryStatus0             = 84;
  mGlobalNvsArea.Area->OnboardCom                 = 1;
  mGlobalNvsArea.Area->PowerState                 = 0;

  mGlobalNvsArea.Area->LogicalProcessorCount    = (UINT8)NumberOfEnabledCPUs;

  mGlobalNvsArea.Area->PassiveThermalTripPoint  = 85;  
  mGlobalNvsArea.Area->PassiveTc1Value          = 1;
  mGlobalNvsArea.Area->PassiveTc2Value          = 5;
  mGlobalNvsArea.Area->PassiveTspValue          = 50;
  mGlobalNvsArea.Area->CriticalThermalTripPoint = 110;

  mGlobalNvsArea.Area->IgdPanelType             = mSystemConfiguration.IgdFlatPanel;
  mGlobalNvsArea.Area->IgdPanelScaling          = mSystemConfiguration.PanelScaling;
  mGlobalNvsArea.Area->IgdSciSmiMode            = 0;
  mGlobalNvsArea.Area->IgdTvFormat              = 0;
  mGlobalNvsArea.Area->IgdTvMinor               = 0;
  mGlobalNvsArea.Area->IgdSscConfig             = 1;
  mGlobalNvsArea.Area->IgdBiaConfig             = mSystemConfiguration.IgdLcdIBia;
  mGlobalNvsArea.Area->IgdBlcConfig             = mSystemConfiguration.IgdLcdIGmchBlc;
  mGlobalNvsArea.Area->IgdDvmtMemSize           =  mSystemConfiguration.IgdDvmt50TotalAlloc;

  mGlobalNvsArea.Area->AlsEnable                = mSystemConfiguration.AlsEnable;
  mGlobalNvsArea.Area->BacklightControlSupport  = 2;
  mGlobalNvsArea.Area->BrightnessPercentage    = 100;
  mGlobalNvsArea.Area->IgdState = 1;
  mGlobalNvsArea.Area->LidState = 1;

  mGlobalNvsArea.Area->DeviceId1 = 0x80000100 ;
  mGlobalNvsArea.Area->DeviceId2 = 0x80000400 ;
  mGlobalNvsArea.Area->DeviceId3 = 0x80000200 ;
  mGlobalNvsArea.Area->DeviceId4 = 0x04;
  mGlobalNvsArea.Area->DeviceId5 = 0x05;
  mGlobalNvsArea.Area->NumberOfValidDeviceId = 4 ;
  mGlobalNvsArea.Area->CurrentDeviceList = 0x0F ;
  mGlobalNvsArea.Area->PreviousDeviceList = 0x0F ;

  mGlobalNvsArea.Area->UartSelection = mSystemConfiguration.UartInterface;

  //
  // DPTF related
  //
  mGlobalNvsArea.Area->DptfEnable                               = mSystemConfiguration.EnableDptf;
  mGlobalNvsArea.Area->DptfSysThermal0                          = mSystemConfiguration.DptfSysThermal0;
  mGlobalNvsArea.Area->DptfSysThermal1                          = mSystemConfiguration.DptfSysThermal1;
  mGlobalNvsArea.Area->DptfSysThermal2                          = mSystemConfiguration.DptfSysThermal2;
  mGlobalNvsArea.Area->DptfSysThermal3                          = mSystemConfiguration.DptfSysThermal3;
  mGlobalNvsArea.Area->DptfSysThermal4                          = mSystemConfiguration.DptfSysThermal4;
  mGlobalNvsArea.Area->DptfCharger                              = mSystemConfiguration.DptfChargerDevice;
  mGlobalNvsArea.Area->DptfDisplayDevice                        = mSystemConfiguration.DptfDisplayDevice;
  mGlobalNvsArea.Area->DptfSocDevice                            = mSystemConfiguration.DptfSocDevice;
  mGlobalNvsArea.Area->DptfProcessor                            = mSystemConfiguration.DptfProcessor;
  mGlobalNvsArea.Area->DptfProcCriticalTemperature              = mSystemConfiguration.CriticalThermalTripPoint;
  mGlobalNvsArea.Area->DptfProcPassiveTemperature               = mSystemConfiguration.PassiveThermalTripPoint;

  mGlobalNvsArea.Area->EnablePassivePolicy                      = mSystemConfiguration.EnablePassivePolicy;
  mGlobalNvsArea.Area->EnableCriticalPolicy                     = mSystemConfiguration.EnableCriticalPolicy;
  mGlobalNvsArea.Area->EnableActivePolicy                       = mSystemConfiguration.EnableActivePolicy;

  mGlobalNvsArea.Area->DptfGenericActiveTemperature0            = mSystemConfiguration.GenericActiveTemp0;
  mGlobalNvsArea.Area->DptfGenericCR3Temperature0               = mSystemConfiguration.GenericCR3Temp0;
  mGlobalNvsArea.Area->DptfGenericHotTemperature0               = mSystemConfiguration.GenericHotTemp0;
  mGlobalNvsArea.Area->DptfGenericActiveTemperature1            = mSystemConfiguration.GenericActiveTemp1;
  mGlobalNvsArea.Area->DptfGenericCR3Temperature1               = mSystemConfiguration.GenericCR3Temp1;
  mGlobalNvsArea.Area->DptfGenericHotTemperature1               = mSystemConfiguration.GenericHotTemp1;
  mGlobalNvsArea.Area->DptfGenericActiveTemperature2            = mSystemConfiguration.GenericActiveTemp2;
  mGlobalNvsArea.Area->DptfGenericCR3Temperature2               = mSystemConfiguration.GenericCR3Temp2;
  mGlobalNvsArea.Area->DptfGenericHotTemperature2               = mSystemConfiguration.GenericHotTemp2;

  mGlobalNvsArea.Area->DptfProcActiveTemperatureSA              = mSystemConfiguration.ActiveThermalTripPointSA;
  mGlobalNvsArea.Area->DptfProcCriticalTemperatureSA            = mSystemConfiguration.CriticalThermalTripPointSA;
  mGlobalNvsArea.Area->DptfProcCR3TemperatureSA                 = mSystemConfiguration.CR3ThermalTripPointSA;
  mGlobalNvsArea.Area->DptfProcHotTemperatureSA                 = mSystemConfiguration.HotThermalTripPointSA;
  mGlobalNvsArea.Area->DptfProcPassiveTemperatureSA             = mSystemConfiguration.PassiveThermalTripPointSA;

  mGlobalNvsArea.Area->DptfGenericCriticalTemperature0          = mSystemConfiguration.GenericCriticalTemp0;
  mGlobalNvsArea.Area->DptfGenericPassiveTemperature0           = mSystemConfiguration.GenericPassiveTemp0;
  mGlobalNvsArea.Area->DptfGenericCriticalTemperature1          = mSystemConfiguration.GenericCriticalTemp1;
  mGlobalNvsArea.Area->DptfGenericPassiveTemperature1           = mSystemConfiguration.GenericPassiveTemp1;
  mGlobalNvsArea.Area->DptfGenericCriticalTemperature2          = mSystemConfiguration.GenericCriticalTemp2;
  mGlobalNvsArea.Area->DptfGenericPassiveTemperature2           = mSystemConfiguration.GenericPassiveTemp2;
  mGlobalNvsArea.Area->DptfGenericCriticalTemperature3          = mSystemConfiguration.GenericCriticalTemp3;
  mGlobalNvsArea.Area->DptfGenericPassiveTemperature3           = mSystemConfiguration.GenericPassiveTemp3;
  mGlobalNvsArea.Area->DptfGenericCriticalTemperature4          = mSystemConfiguration.GenericCriticalTemp4;
  mGlobalNvsArea.Area->DptfGenericPassiveTemperature4           = mSystemConfiguration.GenericPassiveTemp4;
  mGlobalNvsArea.Area->CLpmSetting                              = mSystemConfiguration.Clpm;
  mGlobalNvsArea.Area->DptfSuperDbg                             = mSystemConfiguration.SuperDebug;
  mGlobalNvsArea.Area->LPOEnable                                = mSystemConfiguration.LPOEnable;
  mGlobalNvsArea.Area->LPOStartPState                           = mSystemConfiguration.LPOStartPState;
  mGlobalNvsArea.Area->LPOStepSize                              = mSystemConfiguration.LPOStepSize;
  mGlobalNvsArea.Area->LPOPowerControlSetting                   = mSystemConfiguration.LPOPowerControlSetting;
  mGlobalNvsArea.Area->LPOPerformanceControlSetting             = mSystemConfiguration.LPOPerformanceControlSetting;
  mGlobalNvsArea.Area->DppmEnabled                              = mSystemConfiguration.EnableDppm;
  mGlobalNvsArea.Area->AmbientTripPointChange                   = mSystemConfiguration.AmbientTripPointChange;
  mGlobalNvsArea.Area->DptfWwanDevice                           = mSystemConfiguration.DptfWwanDevice;
  mGlobalNvsArea.Area->DptfWwanCrt                              = mSystemConfiguration.DptfWwanCrt;
  mGlobalNvsArea.Area->DptfWwanPsv                              = mSystemConfiguration.DptfWwanPsv;
  mGlobalNvsArea.Area->DisplayHighLimit                         = mSystemConfiguration.DisplayHighLimit;
  mGlobalNvsArea.Area->DisplayLowLimit                          = mSystemConfiguration.DisplayLowLimit;
  
  //
  // Ambient temperature related data
  //
  mGlobalNvsArea.Area->AmbientConstants[0]                      = mSystemConfiguration.AmbientConstants[0];
  mGlobalNvsArea.Area->AmbientConstants[1]                      = mSystemConfiguration.AmbientConstants[1];
  mGlobalNvsArea.Area->AmbientConstants[2]                      = mSystemConfiguration.AmbientConstants[2];
  mGlobalNvsArea.Area->AmbientConstants[3]                      = mSystemConfiguration.AmbientConstants[3];
  mGlobalNvsArea.Area->AmbientConstants[4]                      = mSystemConfiguration.AmbientConstants[4];
  mGlobalNvsArea.Area->AmbientConstants[5]                      = mSystemConfiguration.AmbientConstants[5];
  mGlobalNvsArea.Area->AmbientConstantSign[0]                   = mSystemConfiguration.AmbientConstantSign[0];
  mGlobalNvsArea.Area->AmbientConstantSign[1]                   = mSystemConfiguration.AmbientConstantSign[1];
  mGlobalNvsArea.Area->AmbientConstantSign[2]                   = mSystemConfiguration.AmbientConstantSign[2];
  mGlobalNvsArea.Area->AmbientConstantSign[3]                   = mSystemConfiguration.AmbientConstantSign[3];
  mGlobalNvsArea.Area->AmbientConstantSign[4]                   = mSystemConfiguration.AmbientConstantSign[4];
  mGlobalNvsArea.Area->AmbientConstantSign[5]                   = mSystemConfiguration.AmbientConstantSign[5];
  mGlobalNvsArea.Area->Str2TspValue                             = mSystemConfiguration.Str2TspValue;

  DEBUG((EFI_D_INFO, "Dumping DPTF settings in global nvs init...\n"));
  DEBUG((EFI_D_INFO, "DPTFEnabled = %d\n", mGlobalNvsArea.Area->DptfEnable));
  DEBUG((EFI_D_INFO, "CpuParticipantCriticalTemperature = %d\n", mGlobalNvsArea.Area->DptfProcCriticalTemperature));
  DEBUG((EFI_D_INFO, "CpuParticipantPassiveTemperature = %d\n", mGlobalNvsArea.Area->DptfProcPassiveTemperature));
  DEBUG((EFI_D_INFO, "GenParticipant0CriticalTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericCriticalTemperature0));
  DEBUG((EFI_D_INFO, "GenParticipant0PassiveTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericPassiveTemperature0));
  DEBUG((EFI_D_INFO, "GenParticipant1CriticalTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericCriticalTemperature1));
  DEBUG((EFI_D_INFO, "GenParticipant1PassiveTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericPassiveTemperature1));
  DEBUG((EFI_D_INFO, "GenParticipant2CriticalTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericCriticalTemperature2));
  DEBUG((EFI_D_INFO, "GenParticipant2PassiveTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericPassiveTemperature2));
  DEBUG((EFI_D_INFO, "GenParticipant3CriticalTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericCriticalTemperature3));
  DEBUG((EFI_D_INFO, "GenParticipant3PassiveTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericPassiveTemperature3));
  DEBUG((EFI_D_INFO, "GenParticipant4CriticalTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericCriticalTemperature4));
  DEBUG((EFI_D_INFO, "GenParticipant4PassiveTemperature = %d\n", mGlobalNvsArea.Area->DptfGenericPassiveTemperature4));
  DEBUG((EFI_D_INFO,"WwanDevice = %d Wwan PSv = %d WwanCrt = %d\n",  mGlobalNvsArea.Area->DptfWwanDevice,mGlobalNvsArea.Area->DptfWwanPsv, mGlobalNvsArea.Area->DptfWwanCrt));
  DEBUG((EFI_D_INFO, "ClpmSetting = %d\n", mGlobalNvsArea.Area->CLpmSetting));
  DEBUG((EFI_D_INFO, "SuperDebug = %d\n", mGlobalNvsArea.Area->DptfSuperDbg));
  DEBUG((EFI_D_INFO, "LPOEnable = %d\n", mGlobalNvsArea.Area->LPOEnable));
  DEBUG((EFI_D_INFO, "LPOStartPState = %d\n", mGlobalNvsArea.Area->LPOStartPState));
  DEBUG((EFI_D_INFO, "LPOStepSize = %d\n", mGlobalNvsArea.Area->LPOStepSize));
  DEBUG((EFI_D_INFO, "LPOPowerControlSetting = %d\n", mGlobalNvsArea.Area->LPOPowerControlSetting));
  DEBUG((EFI_D_INFO, "LPOPerformanceControlSetting = %d\n", mGlobalNvsArea.Area->LPOPerformanceControlSetting));
  DEBUG((EFI_D_INFO, "bDppmEnabled = %d\n", mGlobalNvsArea.Area->DppmEnabled));

  DEBUG((EFI_D_INFO, "DptfEnable = %d\n", mGlobalNvsArea.Area->DptfEnable));
  DEBUG((EFI_D_INFO, "DptfSysThermal0 = %d\n", mGlobalNvsArea.Area->DptfSysThermal0));
  DEBUG((EFI_D_INFO, "DptfSysThermal1 = %d\n", mGlobalNvsArea.Area->DptfSysThermal1));
  DEBUG((EFI_D_INFO, "DptfSysThermal2 = %d\n", mGlobalNvsArea.Area->DptfSysThermal2));
  DEBUG((EFI_D_INFO, "DptfSysThermal3 = %d\n", mGlobalNvsArea.Area->DptfSysThermal3));
  DEBUG((EFI_D_INFO, "DptfCharger = %d\n", mGlobalNvsArea.Area->DptfCharger));
  DEBUG((EFI_D_INFO, "DptfDisplayDevice = %d\n", mGlobalNvsArea.Area->DptfDisplayDevice));
  DEBUG((EFI_D_INFO, "DptfSocDevice = %d\n", mGlobalNvsArea.Area->DptfSocDevice));
  DEBUG((EFI_D_INFO, "DptfProcessor = %d\n", mGlobalNvsArea.Area->DptfProcessor));
  DEBUG((EFI_D_INFO, "Dptf Ambient Temperature Constants values = %d \t %d \t %d \t %d \t %d \t %d\n", mGlobalNvsArea.Area->AmbientConstants[0], \
    mGlobalNvsArea.Area->AmbientConstants[1], mGlobalNvsArea.Area->AmbientConstants[2], mGlobalNvsArea.Area->AmbientConstants[3], \
    mGlobalNvsArea.Area->AmbientConstants[4], mGlobalNvsArea.Area->AmbientConstants[5] ));

  //
  //
  // Platform Flavor
  //
  mGlobalNvsArea.Area->PlatformFlavor = mPlatformInfo->PlatformFlavor;

  //
  // Update the Platform ID
  //
  mGlobalNvsArea.Area->BoardID        = mPlatformInfo->BoardId;

  //
  // Update the Board Revision
  //
  mGlobalNvsArea.Area->FabID          = (UINT8)(mPlatformInfo->FABID + 1);

  mGlobalNvsArea.Area->ECSupport      = mPlatformInfo->ECSupport;
  mGlobalNvsArea.Area->FanSupport     = mPlatformInfo->FanSupport;
  mGlobalNvsArea.Area->BatterySupport = mPlatformInfo->BatterySupport;
  
  mGlobalNvsArea.Area->PmicType       = mPlatformInfo->PmicId;

  DEBUG((EFI_D_INFO, "GlobalNvs BoardId     = %d\n", mGlobalNvsArea.Area->BoardID));
  DEBUG((EFI_D_INFO, "GlobalNvs PmicType    = %d\n", mGlobalNvsArea.Area->PmicType));

  mGlobalNvsArea.Area->OtgMode        = mSystemConfiguration.PchUsbOtg;
  mGlobalNvsArea.Area->XhciMode       = mSystemConfiguration.PchUsb30Mode;

  mGlobalNvsArea.Area->PmicEnable                       = GLOBAL_NVS_DEVICE_DISABLE;

  mGlobalNvsArea.Area->ISPDevSel                        = mSystemConfiguration.ISPDevSel;

  mGlobalNvsArea.Area->ISPEn                            = mSystemConfiguration.ISPEn;
  mGlobalNvsArea.Area->GpioAcpiEnabled                  = mSystemConfiguration.GpioAcpiEnabled;
  mGlobalNvsArea.Area->Sdcard1p8vSwitchingDelay         = mSystemConfiguration.Sdcard1p8vSwitchingDelay;
  mGlobalNvsArea.Area->Sdcard3p3vDischargeDelay         = mSystemConfiguration.Sdcard3p3vDischargeDelay;
 
  //
  // BSW Virtual Button Support
  //
  if (mSystemConfiguration.VirtualButtonEnable == 0) {
    mGlobalNvsArea.Area->VirtualButtonSupport = 0x00;
  } else if (mSystemConfiguration.VirtualButtonEnable == 1) {
    mGlobalNvsArea.Area->VirtualButtonSupport = 0x01;

    if (mSystemConfiguration.ConvertibleState == 1) {
      mGlobalNvsArea.Area->VirtualButtonSupport = mGlobalNvsArea.Area->VirtualButtonSupport | 0x08;
    }

    if (mSystemConfiguration.DockIndicator == 1) {
      mGlobalNvsArea.Area->VirtualButtonSupport = mGlobalNvsArea.Area->VirtualButtonSupport | 0x10;
    }
  }

  //
  // LPE Audio Device Selection & NFC connection
  //
  mGlobalNvsArea.Area->PchLpeEnabled                = mSystemConfiguration.PchLpeEnabled;
  mGlobalNvsArea.Area->NfcConnection                = PcdGet8(PcdNfcConnection); // NFC1 to I2C7

  //
  // BT Module Selection - BRCM(Default), 1 - STP(Stone Peak)
  //
  mGlobalNvsArea.Area->BTModuleSelect               = mSystemConfiguration.BTModule;

  //
  // WIFI Device Selection
  //
  mGlobalNvsArea.Area->WIFIModuleSelect               = mSystemConfiguration.WIFIModule;
  mGlobalNvsArea.Area->NativePCIESupport              = mSystemConfiguration.PciExpNative;

  //
  // Modem selection
  //
  mGlobalNvsArea.Area->AcpiModemSel = mSystemConfiguration.AcpiModemSel;

  //
  // Camera selection
  //
  mGlobalNvsArea.Area->RvpCameraDevSel = mSystemConfiguration.RvpCameraDevSel;
  mGlobalNvsArea.Area->EbCameraDevSel = mSystemConfiguration.EbCameraDevSel;
  SettingI2CTouchAddress(); 
  mGlobalNvsArea.Area->S0ix= mSystemConfiguration.S0ix;

  //
  // Read BMBOUND and store it in GlobalNVS to pass into ASL
  //
  mGlobalNvsArea.Area->BmBound = PcdGet32 (PcdMmioBase);

  if (mSystemConfiguration.ScceMMCEnabled != 0) {
    DEBUG ((EFI_D_ERROR, "mGlobalNvsArea.Area->emmcVersion = 1\n"));
    mGlobalNvsArea.Area->emmcVersion = 1;
  }
  mGlobalNvsArea.Area->ToggleSelfClkDisabling = mSystemConfiguration.ToggleSelfClkDisabling;
  
  //
  // Support EllensBurg
  //
  mGlobalNvsArea.Area->Ellensburg = mSystemConfiguration.Ellensburg;
  DEBUG ((EFI_D_ERROR, "mGlobalNvsArea.Area->Ellensburg = %d\n", mGlobalNvsArea.Area->Ellensburg));

  #if (_PSSD_FIX_==1)
    mGlobalNvsArea.Area->PssdFix                   = 1;
  #else
    mGlobalNvsArea.Area->PssdFix                   = 0;
  #endif

  mGlobalNvsArea.Area->DisableBattery = 0;

  //
  // SIO related option
  //
  Status = gBS->LocateProtocol (&gEfiCpuIo2ProtocolGuid, NULL, &mCpuIo);
  ASSERT_EFI_ERROR (Status);

  mGlobalNvsArea.Area->WPCN381U = GLOBAL_NVS_DEVICE_DISABLE;

  mGlobalNvsArea.Area->DockedSioPresent = GLOBAL_NVS_DEVICE_DISABLE;
  
  if (mGlobalNvsArea.Area->DockedSioPresent != GLOBAL_NVS_DEVICE_ENABLE) {
    //
    // Check ID for SIO WPCN381U
    //
    Status = mCpuIo->Io.Read (
                          mCpuIo,
                          EfiCpuIoWidthUint8,
                          WPCN381U_CONFIG_INDEX,
                          1,
                          &PortData
                          );
    ASSERT_EFI_ERROR (Status);
    if (PortData != 0xFF) {
      PortData = 0x20;
      Status = mCpuIo->Io.Write (
                            mCpuIo,
                            EfiCpuIoWidthUint8,
                            WPCN381U_CONFIG_INDEX,
                            1,
                            &PortData
                            );
      ASSERT_EFI_ERROR (Status);
      Status = mCpuIo->Io.Read (
                            mCpuIo,
                            EfiCpuIoWidthUint8,
                            WPCN381U_CONFIG_DATA,
                            1,
                            &PortData
                            );
      ASSERT_EFI_ERROR (Status);
      if ((PortData == WPCN381U_CHIP_ID) || (PortData == WDCP376_CHIP_ID)) {
        mGlobalNvsArea.Area->WPCN381U = GLOBAL_NVS_DEVICE_ENABLE;
        mGlobalNvsArea.Area->OnboardCom = GLOBAL_NVS_DEVICE_ENABLE;
        mGlobalNvsArea.Area->OnboardComCir = GLOBAL_NVS_DEVICE_DISABLE;
      }
    }
  }

  mGlobalNvsArea.Area->SDIOMode = mSystemConfiguration.LpssSdioMode;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiGlobalNvsAreaProtocolGuid,
                  &mGlobalNvsArea,
                  NULL
                  );

  //
  // Read tables from the storage file.
  //
  while (!EFI_ERROR (Status)) {
    CurrentTable = NULL;

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      (UINTN *) &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      //
      // Perform any table specific updates.
      //
      AcpiStatus = PlatformUpdateTables (CurrentTable);
      if (!EFI_ERROR (AcpiStatus)) {
        //
        // Install the table
        //
        TableHandle = 0;
        AcpiStatus = AcpiTable->InstallAcpiTable (AcpiTable, CurrentTable, CurrentTable->Length, &TableHandle);
        ASSERT_EFI_ERROR (AcpiStatus);
      }
      //
      // Increment the instance
      //
      Instance++;
    }
  }

  //
  //Audio Codec Support for EB and RVP
  //
  mGlobalNvsArea.Area->AudioCodecSuppport = mSystemConfiguration.AudioCodecSuppport;
  DEBUG ((EFI_D_ERROR, "mGlobalNvsArea.Area->AudioCodecSuppport = %d\n", mGlobalNvsArea.Area->AudioCodecSuppport));
  
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  EnableAcpiCallback,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &EfiExitBootServicesEvent
                  );

  //
  // Finished
  //

  return EFI_SUCCESS;
}

VOID
SettingI2CTouchAddress (
  IN VOID
  )
{
  if (mSystemConfiguration.I2CTouchAd == 0) {   // if setup menu select auto set I2C Touch Address base on board id
  } else {
    mGlobalNvsArea.Area->I2CTouchAddress = mSystemConfiguration.I2CTouchAd;
  }
  DEBUG((EFI_D_ERROR, "GlobalNvsArea.Area->I2CTouchAddress: [%02x]\n", mGlobalNvsArea.Area->I2CTouchAddress));
}

/**
  Event Notification during exit boot service to enabel ACPI mode

  @param[in]     Event      EFI Event Handle
  @param[in]     Context    Pointer to Notify Context

  @return

**/
VOID
EnableAcpiCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINT32                        RegData32;
  UINT16                        Pm1Cnt;
  UINT16                        AcpiBase;
  UINT32                        Gpe0aEn;
  EFI_GLOBAL_NVS_AREA           *mGlobalNvsAreaPtr;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsAreaProtocol;
  EFI_STATUS                    Status;

  GlobalNvsAreaProtocol = NULL;

  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, &GlobalNvsAreaProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Unable to locate GlobalNvsAreaProtocol\n"));
    return;
  }

  mGlobalNvsAreaPtr = GlobalNvsAreaProtocol->Area;

  AcpiBase = MmioRead16 (
               MmPciAddress (
                 0,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_LPC,
                 PCI_FUNCTION_NUMBER_PCH_LPC,
                 R_PCH_LPC_ACPI_BASE
                 )
               ) & B_PCH_LPC_ACPI_BASE_BAR;

  DEBUG ((EFI_D_INFO, "EnableAcpiCallback: AcpiBase = %x\n", AcpiBase));

  //
  // Disable PM sources except power button
  // power button is enabled only for PCAT. Disabled it on Tablet platform
  //
  IoWrite16 (AcpiBase + R_PCH_ACPI_PM1_EN, B_PCH_ACPI_PM1_EN_PWRBTN);
  IoWrite16 (AcpiBase + R_PCH_ACPI_PM1_STS, 0xffff);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 5.0 Section 4.8.2.4 "Real Time Clock Alarm")
  // Clear Status D reg VM bit, Date of month Alarm to make Data in CMOS RAM is no longer Valid
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_D);
  IoWrite8 (PCAT_RTC_DATA_REGISTER, 0x0);

  {
    SendKscCommand (KSC_C_SMI_NOTIFY_DISABLE);

    RegData32 = IoRead32(AcpiBase + R_PCH_ALT_GP_SMI_EN);
    RegData32 &= ~(BIT7);
    IoWrite32 ((AcpiBase + R_PCH_ALT_GP_SMI_EN), RegData32);

    //
    // Disable APM/SMI mode
    //
    SendKscCommand (KSC_C_SMI_DISABLE);

    //
    // Disable ACPI/SCI mode
    //
    SendKscCommand (KSC_C_ACPI_ENABLE);
  }

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead16(AcpiBase + R_PCH_ACPI_PM1_CNT);
  Pm1Cnt |= B_PCH_ACPI_PM1_CNT_SCI_EN;
  IoWrite16 (AcpiBase + R_PCH_ACPI_PM1_CNT, Pm1Cnt);

  IoWrite8 (0x80, 0xA0);  //SW_SMI_ACPI_ENABLE

  //
  // Enable PME_B0_EN in GPE0a_EN
  // Caution: Enable PME_B0_EN must be placed after enabling SCI.
  // Otherwise, USB PME could not be handled as SMI event since no handler is there.
  //
  if (mGlobalNvsAreaPtr->OSImageId == 1) {
    Gpe0aEn = IoRead32 (AcpiBase + R_PCH_ACPI_GPE0a_EN);
    Gpe0aEn |= B_PCH_ACPI_GPE0a_EN_PME_B0;
    IoWrite32 (AcpiBase + R_PCH_ACPI_GPE0a_EN, Gpe0aEn);
  }
}
