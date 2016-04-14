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

UINTN   mSmmRelocationOriginalAddress;
BOOLEAN *mRebasedFlag;

/**
  Hook return address of SMM Save State so that semaphore code
  can be executed immediately after AP exits SMM to indicate to
  the BSP that an AP has exited SMM after SMBASE relocation.

  @param CpuIndex  The processor index.
**/
VOID
SemaphoreHook (
  IN UINTN  CpuIndex
  )
{
  SOCKET_LGA_775_SMM_CPU_STATE  *CpuState;

  mRebasedFlag = (BOOLEAN *) &mRebased[CpuIndex];

  {
    CpuState = (SOCKET_LGA_775_SMM_CPU_STATE *)(UINTN)(SMM_DEFAULT_SMBASE + SMM_CPU_STATE_OFFSET);

    //
    // The offset of EIP/RIP is different depending on the SMMRevId
    //
    if (CpuState->x86.SMMRevId < SOCKET_LGA_775_SMM_MIN_REV_ID_x64) {
      mSmmRelocationOriginalAddress = (UINTN) CpuState->x86._EIP;
      CpuState->x86._EIP            = (UINT32) (UINTN) &SmmRelocationSemaphoreComplete;
    } else {
      mSmmRelocationOriginalAddress = (UINTN) CpuState->x64._RIP;
      CpuState->x64._RIP            = (UINT64) (UINTN) &SmmRelocationSemaphoreComplete;
    }

    if (CpuState->x86.AutoHALTRestart & BIT0) {
      //
      // Clear the auto HALT restart flag so the RSM instruction returns 
      //   program control to the instruction following the HLT instruction,
      //   actually returns to SmmRelocationSemaphoreComplete
      //
      CpuState->x86.AutoHALTRestart &= ~BIT0;
    }
  }
}
          
