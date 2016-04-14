/** @file
  Semaphore mechanism to indicate to the BSP that an AP has exited SMM
  after SMBASE relocation.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PiSmmCpuDxeSmm.h"

#define LMA BIT10

UINTN   mSmmRelocationOriginalAddress;
BOOLEAN *mRebasedFlag;
extern  UINT32 mSmmRelocationOriginalAddressPtr32;
extern  UINT32 mRebasedFlagAddr32;

/**
  AP Seamphore operation in 32-bit mode while BSP runs in 64-bit mode.
**/
VOID
SmmRelocationSemaphoreComplete32 (
  VOID
  );

/**
  Hook return address of SMM Save State so that semaphore code
  can be executed immediately after AP exits SMM to indicate to
  the BSP that an AP has exited SMM after SMBASE relocation.

  @param[in]  CpuIndex       The processor index.

**/
VOID
SemaphoreHook (
  IN UINTN  CpuIndex
  )
{
  UINTN                         FunctionPointer;
  UINT64                        Efer;
  UINT16                        AutoHaltRestart;
  SOCKET_LGA_775_SMM_CPU_STATE  *CpuState;
  UINTN                         TempValue;

  mRebasedFlag = (BOOLEAN *) &mRebased[CpuIndex];

  //
  // If BSP runs in 64-bit mode executing boot script while APs rendezvous
  // in 32-bit mode. In this case semaphore code should be 32-bit code instead of x64 code.
  //
  // Note that this module is loaded below 4G, so truncation of 64-bit address to 32-bit for 32-bit semaphore
  // code is safe.
  //
  CpuState = NULL;
  
  {
    CpuState = (SOCKET_LGA_775_SMM_CPU_STATE *)(UINTN)(SMM_DEFAULT_SMBASE + SMM_CPU_STATE_OFFSET);

    //
    // We are now in 64-bit mode, so SMM Save State Map must be x64 format.
    //

    mSmmRelocationOriginalAddress = CpuState->x64._RIP;
    Efer = CpuState->x64.IA32_EFER;
    AutoHaltRestart = CpuState->x86.AutoHALTRestart;
  }

  if ((Efer & LMA) == 0) {
    //
    // Use temp value to fix ICC complier warning
    // 
    TempValue                          = (UINTN)&mSmmRelocationOriginalAddress;
    mSmmRelocationOriginalAddressPtr32 = (UINT32)TempValue;
    mRebasedFlagAddr32                 = (UINT32)(UINTN)mRebasedFlag;

    FunctionPointer                    = (UINTN)&SmmRelocationSemaphoreComplete32;
  } else {
    FunctionPointer                    = (UINTN)&SmmRelocationSemaphoreComplete;
  }

  {
    CpuState->x64._RIP = FunctionPointer;
  }

  if ((AutoHaltRestart & BIT0) != 0) {
    //
    // Clear the auto HALT restart flag so the RSM instruction returns 
    //   program control to the instruction following the HLT instruction,
    //   actually returns to SmmRelocationSemaphoreComplete
    //
    {
      CpuState->x86.AutoHALTRestart &= ~BIT0;
    }
  } 
}
          
