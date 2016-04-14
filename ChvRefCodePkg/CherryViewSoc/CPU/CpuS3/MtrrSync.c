/** @file
  Synchronization of MTRRs on S3 boot path.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MpCommon.h"

UINTN
MpMtrrSynchUpEntry (
  VOID
  )
;

VOID
MpMtrrSynchUpExit (
  UINTN Cr4
  )
;

UINT16 mFixedMtrrIndex[] = {
    EFI_MSR_IA32_MTRR_FIX64K_00000,
    EFI_MSR_IA32_MTRR_FIX16K_80000,
    EFI_MSR_IA32_MTRR_FIX16K_A0000,
    EFI_MSR_IA32_MTRR_FIX4K_C0000,
    EFI_MSR_IA32_MTRR_FIX4K_C8000,
    EFI_MSR_IA32_MTRR_FIX4K_D0000,
    EFI_MSR_IA32_MTRR_FIX4K_D8000,
    EFI_MSR_IA32_MTRR_FIX4K_E0000,
    EFI_MSR_IA32_MTRR_FIX4K_E8000,
    EFI_MSR_IA32_MTRR_FIX4K_F0000,
    EFI_MSR_IA32_MTRR_FIX4K_F8000,
};

UINT16 mMtrrDefType[] = {EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE};

UINT16 mVariableMtrrIndex[] = {
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 1,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 2,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 3,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 4,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 5,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 6,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 7,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 8,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 9,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 10,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 11,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 12,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 13,
    EFI_MSR_CACHE_VARIABLE_MTRR_BASE + 14,
    EFI_MSR_CACHE_VARIABLE_MTRR_END,
};

UINTN  FixedMtrrNumber    = sizeof (mFixedMtrrIndex) / sizeof (UINT16);
UINTN  MtrrDefTypeNumber  = sizeof (mMtrrDefType) / sizeof (UINT16);
UINTN  VariableMtrrNumber = sizeof (mVariableMtrrIndex) / sizeof (UINT16);

/**

    @todo Add function description

  @retval  EFI_SUCCESS -   @todo Add description for return value

**/
VOID
DisableCacheAsRam (
  )
{
  UINT64                    CacheAsRamMsr;

  CacheAsRamMsr = AsmReadMsr64 (0x000002E0);

  //
  // Per Pineview BWG section 3.21, BIOS must set the Disable FERR Assertion
  //
//  CacheAsRamMsr |= B_EFI_MSR_NO_EVICT_MODE_DISABLE_FERR;
//  AsmWriteMsr64 (0x000002E0, CacheAsRamMsr);

  //
  // Step 3: Disable No-Eviction Mode Run State by clearing
  //         NO_EVICT_MODE MSR 2E0h bit [1] = 0
  CacheAsRamMsr &= (~(UINT64)BIT1);
  AsmWriteMsr64 (0x000002E0, CacheAsRamMsr);

  // Step 4: Disable No-Eviction Mode Setup State by clearing
  //         NO_EVICT_MODE MSR 2E0h bit [0] = 0
  CacheAsRamMsr &= (~(UINT64)BIT0);
  AsmWriteMsr64 (0x000002E0, CacheAsRamMsr);
}

/**

  Save the MTRR registers to global variables

**/
VOID
ReadMtrrRegisters (
  UINT64  *MtrrValues
  )
{
  UINT32  Index, IndexEnd;
  //
  // Read all Mtrrs
  //
  for (Index = 0; Index < FixedMtrrNumber; Index++) {
    *MtrrValues = AsmReadMsr64 (mFixedMtrrIndex[Index]);
    MtrrValues++;
  }

  for (Index = 0; Index < MtrrDefTypeNumber; Index++) {
    *MtrrValues = AsmReadMsr64 (mMtrrDefType[Index]);
    MtrrValues++;
  }

  IndexEnd = 2 * (UINT32)(AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP) & B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT);
  for (Index = 0; Index < IndexEnd; Index++) {
    if (Index < (sizeof (mVariableMtrrIndex) / sizeof (UINT16))) {
      *MtrrValues = AsmReadMsr64 (mVariableMtrrIndex[Index]);
      MtrrValues++;
    }
  }
}

/**

  Synch up the MTRR values for all processors

**/
//   @todo    Buffer - add argument and description to function comment
VOID
MpMtrrSynchUp (
  UINT64  *MtrrValues
  )
{
  UINT32              Index, IndexEnd;
  UINTN               Cr4;
  UINT64              *FixedMtrr;
  UINT64              *MtrrDefType;
  UINT64              *VariableMtrr;
  UINT64              ValidMtrrAddressMask;
  UINT8               PhysicalAddressBits;
  UINT32              RegEax;

  //
  // Get physical CPU MTRR width in case of difference from BSP
  //
  AsmCpuid (EFI_CPUID_EXTENDED_FUNCTION, &RegEax, NULL, NULL, NULL);
  PhysicalAddressBits = 36;
  if (RegEax >= EFI_CPUID_VIRT_PHYS_ADDRESS_SIZE) {
    AsmCpuid (EFI_CPUID_VIRT_PHYS_ADDRESS_SIZE, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  }

  ValidMtrrAddressMask = (LShiftU64(1, PhysicalAddressBits) - 1) & 0xfffffffffffff000;

  FixedMtrr    = MtrrValues;
  MtrrDefType  = MtrrValues + FixedMtrrNumber;
  VariableMtrr = MtrrValues + FixedMtrrNumber + MtrrDefTypeNumber;

  //
  // ASM code to setup processor register before synching up the MTRRs
  //
  Cr4 = MpMtrrSynchUpEntry ();

  //
  // Make sure all threads has FERR disabled per Pineview BWG section 3.21
  //
  //DisableCacheAsRam ();

  //
  // Disable Fixed Mtrrs
  //
  AsmWriteMsr64 (EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE, MtrrDefType[0] & 0xFFFFF7FF);

  //
  // Update Fixed Mtrrs
  //
  for (Index = 0; Index < FixedMtrrNumber; Index++) {
    AsmWriteMsr64 (mFixedMtrrIndex[Index], FixedMtrr[Index]);
  }

  //
  // Synchup Base Variable Mtrr
  //
  IndexEnd = 2 * (UINT32)(AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP) & B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT);
  for (Index = 0; Index < IndexEnd; Index ++) {
    if ((Index < (sizeof (mVariableMtrrIndex) / sizeof (UINT16))) && (Index < VariableMtrrNumber ) ) {
      AsmWriteMsr64 (mVariableMtrrIndex[Index], (VariableMtrr[Index] & 0x0FFF) | (VariableMtrr[Index] & ValidMtrrAddressMask));
    }
  }

  //
  // Synchup def type Fixed Mtrrs
  //
  AsmWriteMsr64 (EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE, MtrrDefType[0]);

  //
  // ASM code to setup processor register after synching up the MTRRs
  //
  MpMtrrSynchUpExit (Cr4);

  return ;
}

VOID
SetBspMtrrRegisters (
  IN  EFI_MTRR_VALUES   *MtrrArray
  )
{
  UINT32              Index;
  UINTN               Cr4;

  //
  // ASM code to setup processor register before synching up the MTRRs
  //
  Cr4 = MpMtrrSynchUpEntry ();

  Index = 0;
  while (MtrrArray[Index].Index != 0) {
    AsmWriteMsr64 (MtrrArray[Index].Index, MtrrArray[Index].Value);
    Index++;
  }

  //
  // ASM code to setup processor register after synching up the MTRRs
  //
  MpMtrrSynchUpExit (Cr4);
}
