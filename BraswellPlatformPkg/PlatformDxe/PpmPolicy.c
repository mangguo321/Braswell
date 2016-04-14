/** @file
  Wrapper for Intel PPM Platform Policy driver.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PpmPolicy.h"
#include <Protocol/MpService.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuIA32.h>

#include <PchRegs.h>
#include <Library/PchPlatformLib.h>

#define EFI_CPUID_FAMILY                      0x0F00
#define EFI_CPUID_MODEL                       0x00F0
#define EFI_CPUID_STEPPING                    0x000F

PPM_PLATFORM_POLICY_PROTOCOL    mDxePlatformPpmPolicy;

EFI_STATUS 
EFIAPI
PpmPolicyEntry(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_MP_SERVICES_PROTOCOL *MpService;
  EFI_CPUID_REGISTER        Cpuid01 = { 0, 0, 0, 0};
  EFI_HANDLE                Handle;
  EFI_STATUS                Status;
  UINTN                     CpuCount;
  UINTN                     EnabledCpuCount;
  UINT64                    MaxRatio;
  UINT8                     CPUMobileFeature;

  //
  // Set PPM policy structure to known value
  //
  SetMem (&mDxePlatformPpmPolicy, sizeof(PPM_PLATFORM_POLICY_PROTOCOL), 0);

  //
  // Find the MpService Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (void **) &MpService
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Get processor count from MP service.
  //
  Status = MpService->GetNumberOfProcessors (MpService, &CpuCount, &EnabledCpuCount);
  ASSERT_EFI_ERROR (Status);

  //
  // Store the CPUID for use by SETUP items.
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, &Cpuid01.RegEax, &Cpuid01.RegEbx, &Cpuid01.RegEcx, &Cpuid01.RegEdx);
  MaxRatio = ((RShiftU64 (AsmReadMsr64(EFI_MSR_IA32_PLATFORM_ID), 8)) & 0x1F);

  mDxePlatformPpmPolicy.Revision                       = PPM_PLATFORM_POLICY_PROTOCOL_REVISION_4;

  //Read CPU Mobile feature from PLATFORM_ID_MSR MSR(0x17) NOTFB_I_AM_NOT_MOBILE_FUSE_CLIAMC00H Bit 28
  //Bit Description: { Disables Mobile features 0 = I am NOT a mobile part 1 = I am a mobile part (default)"}
  CPUMobileFeature = ((RShiftU64 (AsmReadMsr64(EFI_MSR_IA32_PLATFORM_ID), 28)) & 0x1);

  if (CPUMobileFeature == 1){//CPU mobile feature
      mDxePlatformPpmPolicy.FunctionEnables.EnableGv       = ICH_DEVICE_ENABLE;
      mDxePlatformPpmPolicy.FunctionEnables.EnableCx       = ICH_DEVICE_ENABLE;
      mDxePlatformPpmPolicy.FunctionEnables.EnableCxe      = ICH_DEVICE_DISABLE;
      mDxePlatformPpmPolicy.FunctionEnables.EnableTm       = ICH_DEVICE_ENABLE;
      //MaxC7
      mDxePlatformPpmPolicy.FunctionEnables.EnableC7       = ICH_DEVICE_ENABLE;
      mDxePlatformPpmPolicy.FunctionEnables.EnableC6       = ICH_DEVICE_ENABLE;
      mDxePlatformPpmPolicy.FunctionEnables.EnableC4       = ICH_DEVICE_ENABLE;
       
      
  }else{//CPU desktop feature
       mDxePlatformPpmPolicy.FunctionEnables.EnableGv       = ICH_DEVICE_DISABLE;
       mDxePlatformPpmPolicy.FunctionEnables.EnableCx       = ICH_DEVICE_DISABLE;
       mDxePlatformPpmPolicy.FunctionEnables.EnableCxe      = ICH_DEVICE_DISABLE;
       mDxePlatformPpmPolicy.FunctionEnables.EnableTm       = ICH_DEVICE_DISABLE;
       mDxePlatformPpmPolicy.FunctionEnables.EnableC4       = ICH_DEVICE_DISABLE;
       mDxePlatformPpmPolicy.FunctionEnables.EnableC6       = ICH_DEVICE_DISABLE;
       mDxePlatformPpmPolicy.FunctionEnables.EnableC7       = ICH_DEVICE_DISABLE;
  }

  mDxePlatformPpmPolicy.FunctionEnables.EnableEmttm    = ICH_DEVICE_DISABLE;
  mDxePlatformPpmPolicy.FunctionEnables.EnableProcHot  = ICH_DEVICE_ENABLE;
  mDxePlatformPpmPolicy.FunctionEnables.TStatesEnable  = ICH_DEVICE_ENABLE;
  mDxePlatformPpmPolicy.FunctionEnables.EnableDynamicFsb=ICH_DEVICE_DISABLE;

  if (SocStepping() < SocA3) {
      mDxePlatformPpmPolicy.FunctionEnables.EnableTurboMode= ICH_DEVICE_DISABLE;
  } else {
      mDxePlatformPpmPolicy.FunctionEnables.EnableTurboMode= ICH_DEVICE_ENABLE;
  }

  mDxePlatformPpmPolicy.FunctionEnables.EnableTm      = ICH_DEVICE_ENABLE;
  mDxePlatformPpmPolicy.FunctionEnables.HTD           = ICH_DEVICE_ENABLE;

  mDxePlatformPpmPolicy.FunctionEnables.EnableCMP      = ICH_DEVICE_ENABLE;

  //
  // Set boot performance mode based on setup option. If the ratio is greater than
  // an arbitrary max, boot in LFM so user can set up custom VID table.
  //
  mDxePlatformPpmPolicy.BootInLfm = 0;
  mDxePlatformPpmPolicy.FlexRatioVid = 0;
  // mDxePlatformPpmPolicy.OsPolicy         =  SetupVariables.OsSelection;

  mDxePlatformPpmPolicy.PowerStateSwitchSmiNumber                     = POWER_STATE_SWITCH_SMI;
  mDxePlatformPpmPolicy.EnableCStateIoRedirectionSmiNumber            = ENABLE_C_STATE_IO_REDIRECTION_SMI;
  mDxePlatformPpmPolicy.DisableCStateIoRedirectionSmiNumber           = DISABLE_C_STATE_IO_REDIRECTION_SMI;
  mDxePlatformPpmPolicy.EnablePStateHardwareCoordinationSmiNumber     = ENABLE_P_STATE_HARDWARE_COORDINATION_SMI;
  mDxePlatformPpmPolicy.DisablePStateHardwareCoordinationSmiNumber    = DISABLE_P_STATE_HARDWARE_COORDINATION_SMI;
  mDxePlatformPpmPolicy.S3RestoreMsrSwSmiNumber                       = S3_RESTORE_MSR_SW_SMI;
  mDxePlatformPpmPolicy.EnableEnableC6ResidencySmiNumber              = ENABLE_C6_RESIDENCY_SMI;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gPpmPlatformPolicyProtocolGuid,
                  &mDxePlatformPpmPolicy,
                  NULL
                  );

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
