/** @file
  Sample to provide FSP platform information related function.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FspApiLib.h>
#include <Library/PcdLib.h>
#include <VpdHeader.h>
#include <Library/GpioLib.h>
#include <Library/HobLib.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/ChvPlatformPolicyPpi.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/PlatformGopPolicy.h>
#include <Guid/AcpiVariableCompatibility.h>
#include <Guid/PlatformInfo.h>
#include <MemoryMappedVariable.h>
#include <Guid/Chv2Variable.h>


CHAR16    EfiMemoryConfigVariable[] = L"MemoryConfig";


EFI_STATUS
FindVbt (
  IN EFI_GUID               *FileName,
  OUT EFI_PHYSICAL_ADDRESS  *Vbt 
  )
{
  EFI_STATUS                 Status;
  UINTN                      Size;
  UINT32                     Pages;
  VOID*                      Memory;
  UINTN                      Instance;
  EFI_PEI_FV_HANDLE          VolumeHandle;
  EFI_PEI_FILE_HANDLE        FileHandle;
  UINT8                      *VbtData;
  BOOLEAN                    Found;
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  
  Size = PcdGet32 (PcdFlashFvVbtSize);
  Pages= EFI_SIZE_TO_PAGES (Size);

  Memory = AllocatePages (Pages);
  CopyMem(Memory , (VOID *) PcdGet32(PcdFlashFvVbtBase) , Size);

  PeiServicesInstallFvInfoPpi (
    NULL,
    (VOID *) ((UINTN)Memory),
    PcdGet32 (PcdFlashFvVbtSize),
    NULL,
    NULL
    );
    
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)Memory;
  BuildFvHob ((UINTN)FvHeader, FvHeader->FvLength);
  
  Found = FALSE;         
  Instance    = 0;
  while (TRUE) {
    //
    // Traverse all firmware volume instances
    //
    Status = PeiServicesFfsFindNextVolume (Instance, &VolumeHandle);
    
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return EFI_NOT_FOUND;
    }
    
    //
    // Find the PEIM file type from the beginning in this firmware volume.
    //
    FileHandle = NULL;
    Status = PeiServicesFfsFindFileByName (FileName, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (Status)) {
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FileHandle, &VbtData);
      if (!EFI_ERROR (Status)) {
         *Vbt = (EFI_PHYSICAL_ADDRESS)VbtData; 
         Found = TRUE;   
         break;
      }
    }
    //
    // We cannot find DxeCore in this firmware volume, then search the next volume.
    //
    Instance++;
  }
  
  if (Found == TRUE) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

STATIC
EFI_STATUS
UpdateSilicionInitUpd (
 IN EFI_PEI_SERVICES  **PeiServices,
 IN SILICON_INIT_UPD  *SiliconInitUpd
)
{
  EFI_STATUS                  Status;
  EFI_PHYSICAL_ADDRESS        VbtAddress;                      
  SYSTEM_CONFIGURATION        SystemConfiguration;
  EFI_PLATFORM_INFO_HOB       *PlatformInfo = NULL;
  EFI_BOOT_MODE               BootMode;
  
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  
  PlatformInfo = PcdGetPtr (PcdPlatformInfo);
  
  if (*((UINT32 *)PcdGetPtr (PcdCustomizedVbtFile)) == SIGNATURE_32 ('$', 'V', 'B', 'T')) {
  	// It is a valid VBT Table
  	VbtAddress = (EFI_PHYSICAL_ADDRESS) (UINTN)PcdGetPtr (PcdCustomizedVbtFile);
  } else {
  	Status = FindVbt (PcdGetPtr (PcdBmpImageGuid), &VbtAddress);
  	ASSERT_EFI_ERROR (Status);
  }
  
  //
  // Should skip FSP graphic initialization for S3 resume. 
  //
  SiliconInitUpd->GraphicsConfigPtr = 0;
  if (BootMode != BOOT_ON_S3_RESUME) {
    SiliconInitUpd->GraphicsConfigPtr = (UINT32)VbtAddress;
  }
  
  SiliconInitUpd->PcdLogoPtr = 0;
  SiliconInitUpd->PcdLogoSize = 0;

  SiliconInitUpd->PcdDispClkSsc = 0x00;
  SiliconInitUpd->GpioPadInitTablePtr = NULL;
  SiliconInitUpd->GpioFamilyInitTablePtr = NULL;
 
  //
  // Power Source Configuration for Cherry Hill. 
  //
  SiliconInitUpd->PunitPwrConfigDisable = 0; // Enable PunitPwrConfig
  SiliconInitUpd->ChvSvidConfig = PlatformInfo->BoardSvidConfig;
  SiliconInitUpd->PMIC_I2CBus = 0;

  
  DEBUG ((EFI_D_INFO, "FspSiliconInitUpd->PcdDispClkSsc = 0x%x\n", (UINT32)(UINTN)SiliconInitUpd->PcdDispClkSsc));
  DEBUG ((EFI_D_INFO, "PcdLogoPtr - 0x%x\n", SiliconInitUpd->PcdLogoPtr));
  DEBUG ((EFI_D_INFO, "PcdLogoSize - 0x%x\n", SiliconInitUpd->PcdLogoSize));
  DEBUG ((EFI_D_INFO, "GraphicsConfigPtr - 0x%x\n", SiliconInitUpd->GraphicsConfigPtr));
  DEBUG ((EFI_D_INFO, "VBT Sign - 0x%x\n", *(UINT32 *)(UINTN)SiliconInitUpd->GraphicsConfigPtr));


  //
  // Update UPD accroding to platform SetUp. 
  //
  CopyMem (&SystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  SiliconInitUpd->PcdEnableAzalia = SystemConfiguration.PchAzalia;
  SiliconInitUpd->PcdEnableSata = SystemConfiguration.PchSata;

  SiliconInitUpd->PcdEnableXhci = SystemConfiguration.PchUsb30Mode;
  SiliconInitUpd->PcdPchSsicEnable = SystemConfiguration.PchSsicEnable;
  SiliconInitUpd->PcdPchUsbSsicPort = SystemConfiguration.PchUsbSsicPort[0];
  SiliconInitUpd->PcdPchUsbHsicPort = SystemConfiguration.PchUsbHsicPort[0];
  
  SiliconInitUpd->Usb2Port0PerPortPeTxiSet = 0x7;
  SiliconInitUpd->Usb2Port0PerPortTxiSet   = 0x3;
  SiliconInitUpd->Usb2Port0IUsbTxEmphasisEn = 0x2;
  SiliconInitUpd->Usb2Port0PerPortTxPeHalf = 0x01;

  SiliconInitUpd->Usb2Port1PerPortPeTxiSet = 0x7;
  SiliconInitUpd->Usb2Port1PerPortTxiSet   = 0x3;
  SiliconInitUpd->Usb2Port1IUsbTxEmphasisEn = 0x2;
  SiliconInitUpd->Usb2Port1PerPortTxPeHalf = 0x01;

  SiliconInitUpd->Usb2Port2PerPortPeTxiSet = 0x7;
  SiliconInitUpd->Usb2Port2PerPortTxiSet   = 0x3;
  SiliconInitUpd->Usb2Port2IUsbTxEmphasisEn = 0x2;
  SiliconInitUpd->Usb2Port2PerPortTxPeHalf = 0x01;

  SiliconInitUpd->Usb2Port3PerPortPeTxiSet = 0x7;
  SiliconInitUpd->Usb2Port3PerPortTxiSet   = 0x3;
  SiliconInitUpd->Usb2Port3IUsbTxEmphasisEn = 0x2;
  SiliconInitUpd->Usb2Port3PerPortTxPeHalf = 0x01;

  SiliconInitUpd->Usb3Lane0Ow2tapgen2deemph3p5 = 0x3a;
  SiliconInitUpd->Usb3Lane1Ow2tapgen2deemph3p5 = 0x3a;
  SiliconInitUpd->Usb3Lane2Ow2tapgen2deemph3p5 = 0x3a;
  SiliconInitUpd->Usb3Lane3Ow2tapgen2deemph3p5 = 0x3a;

  //
  // LPSS devices.
  //
  SiliconInitUpd->PcdEnableHsuart0 = SystemConfiguration.LpssHsuart0Enabled;
  SiliconInitUpd->PcdEnableHsuart1 = SystemConfiguration.LpssHsuart1Enabled;
  SiliconInitUpd->PcdEnableLpe = SystemConfiguration.PchLpeEnabled;
  SiliconInitUpd->PcdEnableDma0 = SystemConfiguration.LpssDma0Enabled;
  SiliconInitUpd->PcdEnableDma1 = SystemConfiguration.LpssDma1Enabled;
  SiliconInitUpd->PcdEnableI2C0 = SystemConfiguration.LpssI2C0Enabled;
  SiliconInitUpd->PcdEnableI2C1 = SystemConfiguration.LpssI2C1Enabled;
  SiliconInitUpd->PcdEnableI2C2 = SystemConfiguration.LpssI2C2Enabled;
  SiliconInitUpd->PcdEnableI2C3 = SystemConfiguration.LpssI2C3Enabled;
  SiliconInitUpd->PcdEnableI2C4 = SystemConfiguration.LpssI2C4Enabled;
  SiliconInitUpd->PcdEnableI2C5 = SystemConfiguration.LpssI2C5Enabled;
  SiliconInitUpd->PcdEnableI2C6 = SystemConfiguration.LpssI2C6Enabled;

  //
  // SCC devices.
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // Config FSP not to initiliaze this device in its Notify function. PchDxe will do it in ExitBootService CallBack
    //
    if (SystemConfiguration.SccSdcardEnabled != PchDisabled) {
      SiliconInitUpd->PcdSdcardMode = PchPciMode;
    } else {
      SiliconInitUpd->PcdSdcardMode = PchDisabled;
    }

    if (SystemConfiguration.ScceMMCEnabled != PchDisabled) {
      SiliconInitUpd->PcdEmmcMode = PchPciMode;
    } else {
      SiliconInitUpd->PcdEmmcMode = PchDisabled;
    }

  } else {
    //
    // For S3 resume, let FSP swtich PCI mode/ACPI mode according to setup option.
    //
    SiliconInitUpd->PcdSdcardMode = SystemConfiguration.SccSdcardEnabled;
    SiliconInitUpd->PcdEmmcMode   = SystemConfiguration.ScceMMCEnabled;
  }


  
  SiliconInitUpd->DptfDisable = 1;
  if (SystemConfiguration.EnableDptf == 1) {
    SiliconInitUpd->DptfDisable = 0;
  }  
 
  SiliconInitUpd->PcdSataInterfaceSpeed = SystemConfiguration.SataInterfaceSpeed;
  SiliconInitUpd->ISPEnable = SystemConfiguration.ISPEn;
  SiliconInitUpd->ISPPciDevConfig = SystemConfiguration.ISPDevSel;
  
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateMemoryInitUpd (
  MEMORY_INIT_UPD        *FspUpdRgn
)
{  
  MEMORY_INIT_UPD *MemoryInitUpd;


  MemoryInitUpd = (MEMORY_INIT_UPD *)FspUpdRgn;
 
  DEBUG ((EFI_D_ERROR, "Memory Related UPD's can be overwritten here\n"));

  MemoryInitUpd->PcdMemChannel0Config = PcdGet8(PcdOemMemeoryDimmType);
  MemoryInitUpd->PcdMemChannel1Config = PcdGet8(PcdOemMemeoryDimmType);
  MemoryInitUpd->PcdMrcInitSpdAddr1 = PcdGet8(PcdMrcInitSpdAddr1);
  MemoryInitUpd->PcdMrcInitSpdAddr2 = PcdGet8(PcdMrcInitSpdAddr2);
  MemoryInitUpd->PcdMemorySpdPtr = (UINT32)(UINTN)PcdGet64 (PcdMemorySpdPtr);
  
  DEBUG ((DEBUG_INFO, "PcdMemorySpdPtr - 0x%x (0x%x)\n", MemoryInitUpd->PcdMemorySpdPtr, &(MemoryInitUpd->PcdMemorySpdPtr)));
  
  MemoryInitUpd->PcdGttSize = 2; // GTT_SIZE_4MB;

  //
  // MMIO region below 4GB(MMIO Base - 4GB).  Granuity is 1MB(0x10000)
  //
  MemoryInitUpd->PcdMrcInitMmioSize = (UINT16)(PcdGet32 (PcdMmioBase) >> 20); 
    
  return EFI_SUCCESS;
}

/**
  Get current boot mode.

  @note At this point, memory is ready, PeiServices are NOT available to use.
  Platform can get some data from chipset register.

  @return BootMode current boot mode.
**/
UINT32
EFIAPI
GetBootMode (
  VOID
  )
{
  EFI_PEI_SERVICES                **PeiServices;
  EFI_BOOT_MODE                   BootMode;
  EFI_STATUS                      Status;
  
  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer (); 
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  
  if (BootMode == BOOT_ON_S3_RESUME) {
    return BOOT_ON_S3_RESUME; 
  }
  
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  Get NVS buffer parameter.

  @note At this point, memory is NOT ready, PeiServices are available to use.

  @return NvsBuffer NVS buffer parameter.

**/
VOID *
EFIAPI
GetNvsBuffer (
  VOID
  )
{
  UINTN                           VariableSize;
  EFI_STATUS                      Status;
  EFI_PEI_SERVICES                **PeiServices;
  UINT8*                          MrcData;
  EFI_BOOT_MODE                   BootMode;
  
  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer (); 
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  VariableSize = 0;
  MrcData = NULL;
      
	//
	// TODO: Will remove this hardcode address. 
	//
	VariableSize = 0x6C58; 
	
	DEBUG((EFI_D_INFO, "VarSize=0x%x.\n", VariableSize));
	
	//
	// Get the MRC Parameters from SPI Flash. This function return the address of this variable in SPI Flash.
	//
	Status = PeiGetVariable (  
	           EfiMemoryConfigVariable,
	           &gEfiChvVariableGuid,
	           NULL,
	           &VariableSize,
	           &MrcData
	           );
	DEBUG((EFI_D_INFO, "Get MRC data from SPI Flash at 0x0x%=. \n", (UINT32)MrcData));
	
	if (EFI_ERROR(Status))
		MrcData = NULL;
 
  return MrcData;
}

/**
  Get UPD region size.

  @note At this point, memory is NOT ready, PeiServices are available to use.

  @return UPD region size.

**/
UINT32
EFIAPI
GetUpdRegionSize (
  VOID
  )
{
  return  sizeof(UPD_DATA_REGION);
}

/**
  This function overrides the default configurations in the UPD data region.

  @note At this point, memory is NOT ready, PeiServices are available to use.

  @param[in, out] FspUpdRgnPtr   A pointer to the UPD data region data strcture.

  @return  FspUpdRgnPtr          A pointer to the UPD data region data strcture.
**/
VOID *
EFIAPI
UpdateFspUpdConfigs (
  IN OUT VOID        *FspUpdRgnPtr
  )
{
  FSP_INFO_HEADER             *pFspHeader = NULL;
  VPD_DATA_REGION             *FspVpdRgn = NULL;
  UINT32                      UpdRegionSize = sizeof(UPD_DATA_REGION);
  MEMORY_INIT_UPD             *MemoryInitUpd = NULL;
  UPD_DATA_REGION             *pFspUpdRgnPtrTmp = NULL;

  if (NULL == FspUpdRgnPtr)
    return NULL;

  pFspUpdRgnPtrTmp = (UPD_DATA_REGION *)FspUpdRgnPtr;
  if (PcdGet32 (PcdFlashFvSecondFspBase) == 0) {
    pFspHeader = FspFindFspHeader (PcdGet32 (PcdFlashFvFspBase));
  } else {
    pFspHeader = FspFindFspHeader (PcdGet32 (PcdFlashFvSecondFspBase));
  }
  if (NULL == pFspHeader) {
    return NULL;
  }

  FspVpdRgn = (VPD_DATA_REGION *)(UINTN)(pFspHeader->ImageBase + pFspHeader->CfgRegionOffset);

  CopyMem (FspUpdRgnPtr, (void *)(pFspHeader->ImageBase + FspVpdRgn->PcdUpdRegionOffset), UpdRegionSize);

  MemoryInitUpd = (MEMORY_INIT_UPD *)((UINT8 *)FspUpdRgnPtr + pFspUpdRgnPtrTmp->MemoryInitUpdOffset);
  
  UpdateMemoryInitUpd((MEMORY_INIT_UPD *)MemoryInitUpd);

  return (VOID *)MemoryInitUpd;
}

/**
  Get BootLoader Tolum size.

  @note At this point, memory is NOT ready, PeiServices are available to use.

  @return BootLoader Tolum size.

**/
UINT32
EFIAPI
GetBootLoaderTolumSize (
  VOID
  )
{
  return 0;
}

/**
  Get TempRamExit parameter.

  @note At this point, memory is ready, PeiServices are available to use.

  @return TempRamExit parameter.

**/
VOID *
EFIAPI
GetTempRamExitParam (
  VOID
  )
{
  return NULL;
}

/**
  Get FspSiliconInit parameter.

  @note At this point, memory is ready, PeiServices are available to use.

  @return FspSiliconInit parameter.

**/
VOID *
EFIAPI
GetFspSiliconInitParam (
  VOID
  )
{
  FSP_INFO_HEADER           *FspHeader;
  VPD_DATA_REGION           *FspVpdRgn;
  UPD_DATA_REGION           *FspUpdRgnPtr;
  SILICON_INIT_UPD          *FspSiliconInitUpd;
  EFI_PEI_SERVICES          **PeiServices;
  
  if (PcdGet32 (PcdFlashFvSecondFspBase) == 0) {
    FspHeader = FspFindFspHeader (PcdGet32 (PcdFlashFvFspBase));
  } else {
    FspHeader = FspFindFspHeader (PcdGet32 (PcdFlashFvSecondFspBase));
  }
  if (FspHeader == NULL) {
    return NULL;
  }

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();
  if (PeiServices == NULL) {
    return NULL;
  }

  //
  //Get VPD region start
  //
  FspVpdRgn = (VPD_DATA_REGION *)(UINTN)(FspHeader->ImageBase + FspHeader->CfgRegionOffset);

  ASSERT (PcdGet32 (PcdMaxUpdRegionSize) >= sizeof(UPD_DATA_REGION));
  FspUpdRgnPtr = (UPD_DATA_REGION *)AllocatePool (PcdGet32 (PcdMaxUpdRegionSize));
  if (NULL == FspUpdRgnPtr) {
    DEBUG ((EFI_D_INFO, "Allocate failed.\n"));
    return NULL;
  }

  CopyMem ((UINT8 *)FspUpdRgnPtr, (UINT8 *)(FspHeader->ImageBase + FspVpdRgn->PcdUpdRegionOffset), sizeof(UPD_DATA_REGION));
  FspSiliconInitUpd = (SILICON_INIT_UPD *)((UINT8 *)FspUpdRgnPtr + FspUpdRgnPtr->SiliconInitUpdOffset);
  DEBUG ((EFI_D_INFO, "FspSiliconInitUpd->Signature = 0x%x\n", (UINT32)(UINTN)FspSiliconInitUpd->Signature));
  DEBUG ((EFI_D_INFO, "FspSiliconInitUpd->PcdDispClkSsc = 0x%x\n", (UINT32)(UINTN)FspSiliconInitUpd->PcdDispClkSsc));

  UpdateSilicionInitUpd (PeiServices, FspSiliconInitUpd);

  return FspSiliconInitUpd;
}

/**
  Get S3 PEI memory information.

  @note At this point, memory is ready, and PeiServices are available to use.
  Platform can get some data from SMRAM directly.

  @param[out] S3PeiMemSize  PEI memory size to be installed in S3 phase.
  @param[out] S3PeiMemBase  PEI memory base to be installed in S3 phase.

  @return                   If S3 PEI memory information is got successfully.

**/
EFI_STATUS
EFIAPI
GetS3MemoryInfo (
  OUT UINT64               *S3PeiMemSize,
  OUT EFI_PHYSICAL_ADDRESS *S3PeiMemBase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get stack information according to boot mode.

  @note If BootMode is BOOT_ON_S3_RESUME or BOOT_ON_FLASH_UPDATE,
  this stack should be in some reserved memory space.

  @note If FspInitDone is TRUE, memory is ready, but no PeiServices there.
  Platform can get some data from SMRAM directly.
  @note If FspInitDone is FALSE, memory is NOT ready, but PeiServices are available to use.
  Platform can get some data from variable via VariablePpi.

  @param[in]  BootMode     Current boot mode.
  @param[in]  FspInitDone  If FspInit is called.
  @param[out] StackSize    Stack size to be used in PEI phase.
  @param[out] StackBase    Stack base to be used in PEI phase.

  @return                  If Stack information is got successfully.

**/
EFI_STATUS
EFIAPI
GetStackInfo (
  IN  UINT32               BootMode,
  IN  BOOLEAN              FspInitDone,
  OUT UINT64               *StackSize,
  OUT EFI_PHYSICAL_ADDRESS *StackBase
  )
{
  *StackBase = PcdGet32 (PcdTemporaryRamBase);
  *StackSize = PcdGet32 (PcdTemporaryRamSize);

  if (BootMode == BOOT_ON_S3_RESUME) {
    if (!FspInitDone) {
    } else {
    }
  } else if (BootMode == BOOT_ON_FLASH_UPDATE) {
    if (!FspInitDone) {
    } else {
    }
  }

  return EFI_SUCCESS;
}
