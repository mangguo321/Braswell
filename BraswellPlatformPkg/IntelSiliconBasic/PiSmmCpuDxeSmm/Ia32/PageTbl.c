/** @file
  Page table manipulation functions for IA-32 processors

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED

**/

#include "PiSmmCpuDxeSmm.h"

SPIN_LOCK                           mPFLock;

/**
  Create PageTable for SMM use.

  @return     PageTable Address    

**/
UINT32
SmmInitPageTable (
  VOID
  )
{
  //
  // Initialize spin lock
  //
  InitializeSpinLock (&mPFLock);

  //
  // Register Smm Page Fault Handler
  //
  SmmRegisterExceptionHandler (&mSmmCpuService, EXCEPT_IA32_PAGE_FAULT, SmiPFHandler);

  return Gen4GPageTable (0);
}

/**
  Page Fault handler for SMM use.

**/
VOID
SmiDefaultPFHandler (
  VOID
  )
{
  CpuDeadLoop ();
}

/**
  ThePage Fault handler wrapper for SMM use.

  @param[in]  InterruptType    Defines the type of interrupt or exception that
                               occurred on the processor.This parameter is processor architecture specific.
  @param[in]  SystemContext    A pointer to the processor context when
                               the interrupt occurred on the processor.
**/
VOID
EFIAPI
SmiPFHandler (
    IN EFI_EXCEPTION_TYPE   InterruptType,
    IN EFI_SYSTEM_CONTEXT   SystemContext
  )
{
  UINTN             PFAddress;

  ASSERT (InterruptType == EXCEPT_IA32_PAGE_FAULT);

  AcquireSpinLock (&mPFLock);
  
  PFAddress = AsmReadCr2 ();

  if ((FeaturePcdGet (PcdCpuSmmStackGuard)) && 
      (PFAddress >= mCpuHotPlugData.SmrrBase) && 
      (PFAddress < (mCpuHotPlugData.SmrrBase + mCpuHotPlugData.SmrrSize))) {
    DEBUG ((EFI_D_ERROR, "SMM stack overflow!\n"));
    CpuDeadLoop ();
  }
  
  if (FeaturePcdGet (PcdCpuSmmProfileEnable)) {
    SmmProfilePFHandler (
      SystemContext.SystemContextIa32->Eip,
      SystemContext.SystemContextIa32->ExceptionData
      );
  } else {
    SmiDefaultPFHandler ();
  }  
  
  ReleaseSpinLock (&mPFLock);
}