/** @file
  Code which support multi-processor.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformMpService.h"
#include "MiscFuncs.h"
#include "CpuRegs.h"

extern MP_SYSTEM_DATA               *mMPSystemData;

EFI_MTRR_VALUES mFixedMtrrValues[] = {
  { EFI_MSR_IA32_MTRR_FIX64K_00000, 0 },
  { EFI_MSR_IA32_MTRR_FIX16K_80000, 0 },
  { EFI_MSR_IA32_MTRR_FIX16K_A0000, 0 },
  { EFI_MSR_IA32_MTRR_FIX4K_C0000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_C8000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_D0000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_D8000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_E0000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_E8000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_F0000,  0 },
  { EFI_MSR_IA32_MTRR_FIX4K_F8000,  0 }
};

EFI_MTRR_VALUES mMtrrDefType[] = { { EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE, 0 } };

EFI_MTRR_VALUES mVariableMtrrValues[] = {
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE, 0      },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 1,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 2,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 3,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 4,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 5,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 6,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 7,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 8,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 9,  0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 10, 0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 11, 0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 12, 0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 13, 0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 14, 0 },
  { EFI_MSR_CACHE_VARIABLE_MTRR_END, 0       }
};

/**
  Save the MTRR registers to global variables

**/
VOID
ReadMtrrRegisters (
  VOID
  )
{
  UINT32  Index, IndexEnd;

  //
  // Read Fixed Mtrrs
  //
  for (Index = 0; Index < sizeof (mFixedMtrrValues) / sizeof (EFI_MTRR_VALUES); Index++) {
    mFixedMtrrValues[Index].Value = AsmReadMsr64 (mFixedMtrrValues[Index].Index);
  }

  //
  // Read def type Fixed Mtrrs
  //
  mMtrrDefType[0].Value = AsmReadMsr64 (EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE);

  //
  // Read Variable Mtrr
  //
  IndexEnd = 2 * (UINT32)(AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP) & B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT);
  for (Index = 0; Index < IndexEnd; Index++) {
    if (Index < (sizeof (mVariableMtrrValues) / sizeof (EFI_MTRR_VALUES))) {
      mVariableMtrrValues[Index].Value = AsmReadMsr64 (mVariableMtrrValues[Index].Index);
    }
  }

  return ;
}

/**
  Synch up the MTRR values for all processors

  @param[in]  Buffer - Not used.

**/
VOID
EFIAPI
MpMtrrSynchUp (
  IN VOID               *Buffer
  )
{
  UINT32              Index, IndexEnd;
  UINTN               Cr4;
  UINT64              MsrValue;
  UINT64              ValidMtrrAddressMask;
  EFI_CPUID_REGISTER  FeatureInfo;
  EFI_CPUID_REGISTER  FunctionInfo;
  UINT8               PhysicalAddressBits;

  //
  // ASM code to setup processor register before synching up the MTRRs
  //
  Cr4 = MpMtrrSynchUpEntry ();

  //
  // Get physical CPU MTRR width in case of difference from BSP
  //
  AsmCpuid (
          EFI_CPUID_EXTENDED_FUNCTION,
          &FunctionInfo.RegEax,
          &FunctionInfo.RegEbx,
          &FunctionInfo.RegEcx,
          &FunctionInfo.RegEdx
          );
  PhysicalAddressBits = 36;
  if (FunctionInfo.RegEax >= EFI_CPUID_VIRT_PHYS_ADDRESS_SIZE) {
    AsmCpuid (
            EFI_CPUID_VIRT_PHYS_ADDRESS_SIZE,
            &FeatureInfo.RegEax,
            &FeatureInfo.RegEbx,
            &FeatureInfo.RegEcx,
            &FeatureInfo.RegEdx
            );
    PhysicalAddressBits = (UINT8) FeatureInfo.RegEax;
  }

  ValidMtrrAddressMask = (LShiftU64( 1, PhysicalAddressBits) - 1) & 0xfffffffffffff000;

  //
  // Disable Fixed Mtrrs
  //
  AsmWriteMsr64 (EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE, mMtrrDefType[0].Value & 0xFFFFF7FF);

  //
  // Update Fixed Mtrrs
  //
  for (Index = 0; Index < sizeof (mFixedMtrrValues) / sizeof (EFI_MTRR_VALUES); Index++) {
    AsmWriteMsr64 (mFixedMtrrValues[Index].Index, mFixedMtrrValues[Index].Value);
  }

  //
  // Synchup def type Fixed Mtrrs
  //
  AsmWriteMsr64 (EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE, mMtrrDefType[0].Value);

  //
  // Synchup Base Variable Mtrr
  //
  IndexEnd = 2 * (UINT32)(AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP) & B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT);
  for (Index = 0; Index < IndexEnd; Index += 2) {
    if (Index < (sizeof (mVariableMtrrValues) / sizeof (EFI_MTRR_VALUES))) {
      MsrValue = (mVariableMtrrValues[Index].Value & 0x0FFF) | (mVariableMtrrValues[Index].Value & ValidMtrrAddressMask);
      AsmWriteMsr64 (mVariableMtrrValues[Index].Index, MsrValue);
    }
  }

  //
  // Synchup Mask Variable Mtrr including valid bit
  //
  for (Index = 1; Index < IndexEnd; Index += 2) {
    if (Index < (sizeof (mVariableMtrrValues) / sizeof (EFI_MTRR_VALUES))) {
      MsrValue = (mVariableMtrrValues[Index].Value & 0x0FFF) | (mVariableMtrrValues[Index].Value & ValidMtrrAddressMask);
      AsmWriteMsr64 (mVariableMtrrValues[Index].Index, MsrValue);
    }
  }

  //
  // ASM code to setup processor register after synching up the MTRRs
  //
  MpMtrrSynchUpExit (Cr4);
}

/**
  @todo	add description

**/
VOID
SaveBspMtrrForS3 (
  )
{
  UINTN  Index, IndexEnd;
  UINTN  TableIndex;

  TableIndex = 0;

  for (Index = 0; Index < sizeof (mFixedMtrrValues) / sizeof (mFixedMtrrValues[0]); Index++) {
    mMPSystemData->S3BspMtrrTable[TableIndex].Index = mFixedMtrrValues[Index].Index;
    mMPSystemData->S3BspMtrrTable[TableIndex].Value = mFixedMtrrValues[Index].Value; 
    TableIndex++;
  }

  IndexEnd = 2 * (UINT32)(AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP) & B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT);
  for (Index = 0; Index < IndexEnd; Index ++) {
    if (TableIndex < (sizeof (mMPSystemData->S3BspMtrrTable) / sizeof (mMPSystemData->S3BspMtrrTable[0]))) {
      if (Index < (sizeof (mVariableMtrrValues) / sizeof (mVariableMtrrValues[0]))) {
        mMPSystemData->S3BspMtrrTable[TableIndex].Index = mVariableMtrrValues[Index].Index;
        mMPSystemData->S3BspMtrrTable[TableIndex].Value = mVariableMtrrValues[Index].Value; 
        TableIndex++;
      }
    }
  }

  if( TableIndex < ( (sizeof(mMPSystemData->S3BspMtrrTable) / sizeof(mMPSystemData->S3BspMtrrTable[0]) - 2) )) {
    mMPSystemData->S3BspMtrrTable[TableIndex].Index = mMtrrDefType[0].Index;
    mMPSystemData->S3BspMtrrTable[TableIndex].Value = mMtrrDefType[0].Value; 
    TableIndex++;

    //
    // To terminate the table during S3 resume for MTRR synch up
    //
    mMPSystemData->S3BspMtrrTable[TableIndex].Index = 0;
  } else if( TableIndex < ( (sizeof(mMPSystemData->S3BspMtrrTable) / sizeof(mMPSystemData->S3BspMtrrTable[0]) - 1) )) {
    //
    // To terminate the table during S3 resume for MTRR synch up
    //
    mMPSystemData->S3BspMtrrTable[TableIndex].Index = 0;
  }

  ASSERT (TableIndex < MAX_CPU_S3_MTRR_ENTRY);
}
