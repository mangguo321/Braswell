/** @file
  Do platform specific PEI stage initializations.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformEarlyInit.h"
#include <Library/BiosIdLib.h>
#include <CpuRegs.h>
#include <CpuPpmLib.h>

#pragma optimize ("", off)

static EFI_PEI_FIND_FV_PPI mEfiFindFvPpi = {
  FindFv
};

static EFI_PEI_PPI_DESCRIPTOR       mPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiFindFvPpiGuid,
    &mEfiFindFvPpi
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR    mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiEndOfPeiSignalPpiGuid,
    EndOfPeiPpiNotifyCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK| EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    MemoryDiscoveredPpiNotifyCallback
  }
};


/**
  Bugbug: temp workaround - Initialize performance to HFM.

  @param[in] None

**/
VOID
ProcessorsPerfPowerInit (
  )
{
  EFI_CPUID_REGISTER    Cpuid = { 0, 0, 0, 0 };
  UINT16                MaxBusRatio;
  UINT16                MaxVid;
  MSR_REGISTER          Ia32MiscEnable;
  MSR_REGISTER          TempMsr;
  MSR_REGISTER          IaCoreRatios;
  MSR_REGISTER          IaCoreVids;

  
  //
  // Get Maximum Non-Turbo bus ratio (HFM) from IACORE_RATIOS MSR Bits[23:16]
  //
  IaCoreRatios.Qword = AsmReadMsr64 (MSR_IACORE_RATIOS);
  MaxBusRatio  = IaCoreRatios.Bytes.ThirdByte;

  //
  // Get Maximum Non-Turbo Vid (HFM) from IACORE_VIDS MSR Bits[23:16]
  //
  IaCoreVids.Qword = AsmReadMsr64 (MSR_IACORE_VIDS);
  MaxVid  = IaCoreVids.Bytes.ThirdByte;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &Cpuid.RegEax, &Cpuid.RegEbx, &Cpuid.RegEcx, &Cpuid.RegEdx);

  //
  // This function will be executed when EIST is enabled and EIST is capable
  // So processor can be switched to HFM
  //
  if ((Cpuid.RegEcx & B_EFI_CPUID_VERSION_INFO_ECX_EIST) == B_EFI_CPUID_VERSION_INFO_ECX_EIST) {
        
    //
    // Enable EIST
    //
    Ia32MiscEnable.Qword = AsmReadMsr64 (EFI_MSR_IA32_MISC_ENABLE);
    Ia32MiscEnable.Qword |= B_EFI_MSR_IA32_MISC_ENABLE_EIST;
    AsmWriteMsr64 (MSR_IA32_MISC_ENABLE, Ia32MiscEnable.Qword);

    TempMsr.Qword = AsmReadMsr64 (EFI_MSR_IA32_PERF_CTRL);
    TempMsr.Qword &= (~(UINT64)P_STATE_TARGET_MASK);

    TempMsr.Qword |= LShiftU64 (MaxBusRatio, P_STATE_TARGET_OFFSET);
    TempMsr.Qword |= (UINT64)MaxVid;

    AsmWriteMsr64 (EFI_MSR_IA32_PERF_CTRL, TempMsr.Qword);
  }

  return;
}

/**
  Platform specific initializations in stage1.

  @param[in]  FileHandle        Pointer to the PEIM FFS file header.
  @param[in]  PeiServices       General purpose services available to every PEIM.

  @retval   EFI_SUCCESS         Operation completed successfully.
  @retval   Otherwise           Platform initialization failed.

**/
EFI_STATUS
EFIAPI
PlatformEarlyInitEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS                  Status;
  SYSTEM_CONFIGURATION        SystemConfiguration;
  EFI_PLATFORM_INFO_HOB       *PlatformInfo;

  //
  // Set the some PCI and chipset range as UC
  // And align to 1M at least
  //
  PlatformInfo = PcdGetPtr (PcdPlatformInfo);

  //
  // Initialize PlatformInfo HOB
  //
  MultiPlatformInfoInit (PeiServices, PlatformInfo);

  //
  // Get setup variable. This can only be done after BootMode is updated
  //
  CopyMem (&SystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  //
  // Do basic PCH init
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, (EFI_COMPUTING_UNIT_CHIPSET | EFI_CHIPSET_PC_PEI_CAR_SB_INIT));
  Status = PlatformPchInit (&SystemConfiguration, PeiServices, PlatformInfo->PlatformType);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize platform PPIs
  //
  Status = (*PeiServices)->NotifyPpi(PeiServices, &mNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  //
  // Bugbug: temp workaround to increase performance.
  //
  ProcessorsPerfPowerInit();

  return Status;
}

/**
  Return the mainblockcompact Fv.

  @param[in]   This          EFI PEI FIND FV PPI instance
  @param[in]   PeiServices   General purpose services available to every PEIM.
  @param[in]   FvNumber      enumeration of the firmware volumes we care about.
  @param[in]   FvAddress     Base Address of the memory containing the firmware volume

  @retval      EFI_SUCCESS
  @retval      EFI_NOT_FOUND

**/
EFI_STATUS
EFIAPI
FindFv (
  IN EFI_PEI_FIND_FV_PPI          *This,
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT UINT8                    *FvNumber,
  OUT EFI_FIRMWARE_VOLUME_HEADER  **FVAddress
  )
{
  //
  // At present, we only have one Fv to search
  //
  if (*FvNumber == 0) {
    *FvNumber = 1;
    *FVAddress = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) MAINBLOCK_BASE_ADDR;
    return EFI_SUCCESS;
  } else if (*FvNumber == 1) {
    *FvNumber = 2;
    if (IsA16Inverted ()) {
      *FVAddress = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BOOTBLOCK2_BACKUP_BASE_ADDR;
    } else {
      *FVAddress = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BOOTBLOCK2_BASE_ADDR;
    }
    return EFI_SUCCESS;
  } else { // Not the one Fv we care about
    return EFI_NOT_FOUND;
  }
}

#pragma optimize ("", on)

