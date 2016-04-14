/** @file
  Dxe Platform Policy driver init.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/ChvPlatformPolicyProtocol.h>

#ifndef FSP_FLAG
#include <PiDxe.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#else
#include <ChvPlatformPolicyUpdateDxeLib.h>
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#endif

DXE_CHV_PLATFORM_POLICY_PROTOCOL mDxePlatformChvPolicy;

#ifdef FSP_FLAG
extern EFI_GUID gDxeChvPlatformPolicyGuid;

static EFI_PEI_PPI_DESCRIPTOR     mChvPolicyInitPpiList = {  
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gDxeChvPlatformPolicyGuid,	
    &mDxePlatformChvPolicy
};
#endif
/**
@todo  add description

**/
EFI_STATUS
ChvPlatformPolicyInit (
#ifndef FSP_FLAG
  IN EFI_HANDLE     ImageHandle
#else
  IN CONST EFI_PEI_SERVICES    **PeiServices
#endif
  )
{
  DXE_CHV_PLATFORM_POLICY_PROTOCOL  *DxePlatformChvPolicy;
  EFI_STATUS                        Status;

  ZeroMem (&mDxePlatformChvPolicy, sizeof (DXE_CHV_PLATFORM_POLICY_PROTOCOL));
  
  DxePlatformChvPolicy = &mDxePlatformChvPolicy;

  DxePlatformChvPolicy->MaxInverterPWM = 0;
  DxePlatformChvPolicy->MinInverterPWM = 0;

  DxePlatformChvPolicy->ForceWake = 1;
  DxePlatformChvPolicy->IgdPanelFeatures.PFITStatus = 0; //Auto
  DxePlatformChvPolicy->IgdPanelFeatures.LidStatus= 1; //On
  DxePlatformChvPolicy->S0ixSupported = 0;

  mDxePlatformChvPolicy.EnableIGDTurbo = 2; //Auto
  mDxePlatformChvPolicy.PAVP_PR3_MemLength = 0;
  mDxePlatformChvPolicy.RPMBLock = 0;
  mDxePlatformChvPolicy.PavpAsmf = 0;  // If PR3 enabled, then this policy item needs to be enabled.

  DxePlatformChvPolicy->AudioTypeSupport = HD_AUDIO;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeChvPlatformPolicyGuid,
                  DxePlatformChvPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  
  return Status;
}
