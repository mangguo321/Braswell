/** @file
  This PEIM initialize platform for MRC, following action is performed,
    1. Initialize GMCH
    2. Detect boot mode
    3. Detect video adapter to determine whether we need pre-allocated memory
    4. Calls MRC to initialize memory and install a PPI notify to do post memory initialization.
  This file contains the main entry point of the PEIM.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "Platform.h"
#include "PlatformBaseAddresses.h"
#include "PchRegs.h"
#include <Guid/PlatformInfo.h>
#include "ChvAccess.h"
#include <Library/GpioLib.h>
#include <Guid/Chv2Variable.h>
#include <Ppi/ChvPlatformPolicyPpi.h>
#include <Ppi/BoardDetection.h>
#include <Guid/SetupVariable.h>
#include "SetupVariableDefault.h"

#ifndef EC_BASE
#define EC_BASE           ((UINTN)PcdGet64(PcdPciExpressBaseAddress))
#endif

#ifndef MmPciAddress
#define MmPciAddress(Segment, Bus, Device, Function, Register) \
  ((UINTN) EC_BASE + \
   (UINTN) (Bus << 20) + \
   (UINTN) (Device << 15) + \
   (UINTN) (Function << 12) + \
   (UINTN) (Register) \
  )
#endif

//
// The global indicator, the FvFileLoader callback will modify it to TRUE after loading PEIM into memory
//

EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN CONST EFI_PEI_STALL_PPI      *This,
  IN UINTN                        Microseconds
  );

static EFI_PEI_STALL_PPI  mStallPpi = {
  PEI_STALL_RESOLUTION,
  Stall
};

static EFI_PEI_PPI_DESCRIPTOR mInstallStallPpi[] = {
  {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiStallPpiGuid,
  &mStallPpi
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR mMemoryDiscoveredNotifyList[1] = {
  {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  MemoryDiscoveredPpiNotifyCallback
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR mEndOfPeiNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    EndOfPeiPpiNotifyCallback
  }
};

static EFI_PEI_PPI_DESCRIPTOR mBoardDetectionStartPpi[] = {
  {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gBoardDetectionStartPpiGuid,
  NULL
  }
};


STATIC
EFI_STATUS
CheckSetupVarItem (
  IN   SYSTEM_CONFIGURATION          *SystemConfiguration
  )
{

  DEBUG ((EFI_D_INFO, "CheckSetupVarItem(): entry \n"));    

  if (SystemConfiguration->TurboModeEnable > 1) 
    SystemConfiguration->TurboModeEnable = PcdGet8(PcdTurboMode);
  
  //
  // South Complex
  //
  if (SystemConfiguration->SccSdcardEnabled > 2)
    SystemConfiguration->SccSdcardEnabled = PcdGet8(PcdSdcardMode);
  
  if (SystemConfiguration->LpssHsuart0Enabled > 2)
  SystemConfiguration->LpssHsuart0Enabled = PcdGet8(PcdEnableHsuart0);

  if (SystemConfiguration->LpssHsuart1Enabled > 2)
  SystemConfiguration->LpssHsuart1Enabled = PcdGet8(PcdEnableHsuart1);

  if (SystemConfiguration->PchAzalia > 1)
  SystemConfiguration->PchAzalia = PcdGet8(PcdEnableAzalia);

  if (SystemConfiguration->PchSata > 1)
  SystemConfiguration->PchSata = PcdGet8(PcdEnableSata);

  if (SystemConfiguration->PchUsb30Mode > 1)
  SystemConfiguration->PchUsb30Mode = PcdGet8(PcdEnableXhci);

  if (SystemConfiguration->PchLpeEnabled > 2)
  SystemConfiguration->PchLpeEnabled = PcdGet8(PcdEnableLpe);

  if (SystemConfiguration->LpssDma0Enabled > 2)
  SystemConfiguration->LpssDma0Enabled = PcdGet8(PcdEnableDma0);

  if (SystemConfiguration->LpssDma1Enabled > 2)
  SystemConfiguration->LpssDma1Enabled = PcdGet8(PcdEnableDma1);

  if (SystemConfiguration->LpssI2C0Enabled > 2)
  SystemConfiguration->LpssI2C0Enabled = PcdGet8(PcdEnableI2C0);

  if (SystemConfiguration->LpssI2C1Enabled > 2)
  SystemConfiguration->LpssI2C1Enabled = PcdGet8(PcdEnableI2C1);

  if (SystemConfiguration->LpssI2C2Enabled > 2)
  SystemConfiguration->LpssI2C2Enabled = PcdGet8(PcdEnableI2C2);

  if (SystemConfiguration->LpssI2C3Enabled > 2)
  SystemConfiguration->LpssI2C3Enabled = PcdGet8(PcdEnableI2C3);

  if (SystemConfiguration->LpssI2C4Enabled > 2)
  SystemConfiguration->LpssI2C4Enabled = PcdGet8(PcdEnableI2C4);

  if (SystemConfiguration->LpssI2C5Enabled > 2)
  SystemConfiguration->LpssI2C5Enabled = PcdGet8(PcdEnableI2C5);

  if (SystemConfiguration->LpssI2C6Enabled > 2)
  SystemConfiguration->LpssI2C6Enabled = PcdGet8(PcdEnableI2C6);

  if (SystemConfiguration->ScceMMCEnabled > 2) 
  SystemConfiguration->ScceMMCEnabled = PcdGet8(PcdEmmcMode);

  if (SystemConfiguration->SataInterfaceSpeed > 3) 
  SystemConfiguration->SataInterfaceSpeed = PcdGet8(PcdSataInterfaceSpeed);

  if (SystemConfiguration->ISPEn > 1) 
  SystemConfiguration->ISPEn = PcdGet8(ISPEnable);

  if (SystemConfiguration->ISPDevSel > 2) 
  SystemConfiguration->ISPDevSel = PcdGet8(ISPPciDevConfig);

  if (SystemConfiguration->MrcDvfsEnable > 1)
  SystemConfiguration->MrcDvfsEnable = PcdGet8(PcdDvfsEnable);
  
  if (SystemConfiguration->PnpSettings > 5) 
  SystemConfiguration->PnpSettings = PcdGet8(PcdPnpSettings);
  
  
  //
  // North Complex
  //
  if (SystemConfiguration->GTTSize > 2)
  SystemConfiguration->GTTSize = PcdGet8(PcdGttSize);

  if (SystemConfiguration->IgdApertureSize > 3) 
  SystemConfiguration->IgdApertureSize = PcdGet8(PcdApertureSize);

  if (SystemConfiguration->IgdDvmt50PreAlloc > 16) 
  SystemConfiguration->IgdDvmt50PreAlloc = PcdGet8(PcdIgdDvmt50PreAlloc);


  DEBUG ((EFI_D_INFO, "CheckSetupVarItem(): exit \n"));

  return EFI_SUCCESS;
}


EFI_STATUS
GetSetupVariable (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN   SYSTEM_CONFIGURATION          *SystemConfiguration
  )
{
  UINTN                        VariableSize;
  EFI_STATUS                   Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *Variable;

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  ZeroMem (SystemConfiguration, sizeof (SYSTEM_CONFIGURATION));

  Status = (*PeiServices)->LocatePpi (PeiServices,
                                      &gEfiPeiReadOnlyVariable2PpiGuid,
                                      0,
                                      NULL,
                                      &Variable
                                      );
  ASSERT_EFI_ERROR (Status);

  //
  // Use normal setup default from NVRAM variable,
  // the Platform Mode (manufacturing/safe/normal) is handle in PeiGetVariable.
  //
  VariableSize = sizeof(SYSTEM_CONFIGURATION);
  Status = Variable->GetVariable (Variable,
                                   L"Setup",
                                   &gEfiSetupVariableGuid,
                                   NULL,
                                   &VariableSize,
                                   SystemConfiguration);
  if (VariableSize != sizeof(SYSTEM_CONFIGURATION)) {
    DEBUG ((EFI_D_ERROR, "Setup variable is currputed\n"));
    Status = EFI_BAD_BUFFER_SIZE;
  }

  DEBUG ((EFI_D_INFO, "GetSetupVariable() Exit \n"));
  return Status;
}

/**
  This is the entry point of PEIM

  @param[in]   FileHandle     Handle of the file being invoked.
  @param[in]   PeiServices    Describes the list of possible PEI Services.

  @retval   EFI_SUCCESS       If it is completed successfully.

**/
EFI_STATUS
EFIAPI
PeiInitPlatform (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  UINTN                            Size;
  SYSTEM_CONFIGURATION             SystemConfiguration;
  
  //
  // Get setup variable. This can only be done after BootMode is updated
  //
  Status = GetSetupVariable (PeiServices, &SystemConfiguration);
  Size = sizeof(SYSTEM_CONFIGURATION);

  if(EFI_ERROR(Status)){
    //
    // For OC solution, there is no setup variable, so set the SystemConfiguration with default value.
    //
    CopyMem(&SystemConfiguration, &gDefaultSystemConfiguration,Size);
    //
    // Update the values according to PCD settings
    //
	
    //
    // Platform
    //
    SystemConfiguration.TurboModeEnable = PcdGet8(PcdTurboMode);
	
    //
    // South Complex
    //
    SystemConfiguration.SccSdcardEnabled = PcdGet8(PcdSdcardMode);
    SystemConfiguration.LpssHsuart0Enabled = PcdGet8(PcdEnableHsuart0);
    SystemConfiguration.LpssHsuart1Enabled = PcdGet8(PcdEnableHsuart1);
    SystemConfiguration.PchAzalia = PcdGet8(PcdEnableAzalia);
    SystemConfiguration.PchSata = PcdGet8(PcdEnableSata);
    SystemConfiguration.PchUsb30Mode = PcdGet8(PcdEnableXhci);
    SystemConfiguration.PchLpeEnabled = PcdGet8(PcdEnableLpe);
    SystemConfiguration.LpssDma0Enabled = PcdGet8(PcdEnableDma0);
    SystemConfiguration.LpssDma1Enabled = PcdGet8(PcdEnableDma1);
    SystemConfiguration.LpssI2C0Enabled = PcdGet8(PcdEnableI2C0);
    SystemConfiguration.LpssI2C1Enabled = PcdGet8(PcdEnableI2C1);
    SystemConfiguration.LpssI2C2Enabled = PcdGet8(PcdEnableI2C2);
    SystemConfiguration.LpssI2C3Enabled = PcdGet8(PcdEnableI2C3);
    SystemConfiguration.LpssI2C4Enabled = PcdGet8(PcdEnableI2C4);
    SystemConfiguration.LpssI2C5Enabled = PcdGet8(PcdEnableI2C5);
    SystemConfiguration.LpssI2C6Enabled = PcdGet8(PcdEnableI2C6);
    SystemConfiguration.ScceMMCEnabled = PcdGet8(PcdEmmcMode);
    SystemConfiguration.SataInterfaceSpeed = PcdGet8(PcdSataInterfaceSpeed);
    SystemConfiguration.ISPEn = PcdGet8(ISPEnable);
    SystemConfiguration.ISPDevSel = PcdGet8(ISPPciDevConfig);
    SystemConfiguration.PchSata = PcdGet8(PcdEnableSata);	
    SystemConfiguration.MrcDvfsEnable = PcdGet8(PcdDvfsEnable);
    SystemConfiguration.PnpSettings = PcdGet8(PcdPnpSettings);
	
    //
    // North Complex
    //
    SystemConfiguration.GTTSize = PcdGet8(PcdGttSize);
    SystemConfiguration.IgdApertureSize = PcdGet8(PcdApertureSize);
    SystemConfiguration.IgdDvmt50PreAlloc = PcdGet8(PcdIgdDvmt50PreAlloc);
    SystemConfiguration.TSEGSizeSel = (UINT8)PcdGet16(PcdMrcInitTsegSize);
    DEBUG ((EFI_D_INFO, "PeiInitPlatform(): GetSetupVariable returns EFI_NOT_FOUND!! \n"));
  }else {
    // Chipsec: Go thrugh all setup items is corrupted one by one
    CheckSetupVarItem(&SystemConfiguration);

  }
  PcdSetPtr (PcdSystemConfiguration, &Size, &SystemConfiguration);
  DEBUG ((EFI_D_INFO, "PcdSystemConfiguration size - 0x%x\n", LibPcdGetExSize(&gEfiEdkIIPlatformTokenSpaceGuid, PcdTokenEx(&gEfiEdkIIPlatformTokenSpaceGuid, PcdSystemConfiguration)) ));

  //
  // Initialize Stall PPIs
  //
  Status = (*PeiServices)->InstallPpi (PeiServices, &mInstallStallPpi[0]);
  ASSERT_EFI_ERROR (Status);

  Status = (*PeiServices)->NotifyPpi (PeiServices, &mMemoryDiscoveredNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  Status = (*PeiServices)->NotifyPpi (PeiServices, &mEndOfPeiNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  //
  // Start board detection
  //
  Status = PeiServicesInstallPpi (mBoardDetectionStartPpi);
  ASSERT_EFI_ERROR (Status);
  
  //
  // Set the new boot mode for MRC
  //
  Status = UpdateBootMode (PeiServices);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
