/** @file
  The CPU specific programming for PiSmmCpuDxeSmm module, such as SMRR, EMRR, IED.
  Currently below CPUs are supported.

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>

#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/LocalApicLib.h>

#include "PiSmmCpuDxeSmm.h"
#include "SmmFeatures.h"

//
// The CPUID mapping for CherryView
//

CPUID_MAPPING mCherryViewMap[] = {
  {CPUID_SIGNATURE_CHERRYVIEW, CPUID_MASK_NO_STEPPING},
};

//
// The CLASS for CherryView
//
CPU_SMM_CLASS mCherryViewClass = {
  CpuCherryView,
  sizeof(mCherryViewMap)/sizeof(mCherryViewMap[0]),
  mCherryViewMap,
  };

//
// This table defines supported CPU class
//
CPU_SMM_CLASS *mCpuClasstable[] = {
  &mCherryViewClass
  };

////////
// Below section is common definition
////////

//
// Assumes UP, or MP with identical feature set
//
CPU_SMM_FEATURE_CONTEXT  mFeatureContext;

CPU_SMM_CLASS            *mThisCpu;
BOOLEAN                  mSmmCodeAccessCheckEnable = FALSE;
BOOLEAN                  mSmmUseDelayIndication;
BOOLEAN                  mSmmUseBlockIndication;

/**
  Return if SMRR is supported

  @retval TRUE  SMRR is supported.
  @retval FALSE SMRR is not supported.

**/
BOOLEAN
IsSmrrSupported (
  VOID
  )
{
  UINT64                            MtrrCap;

  MtrrCap = AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP);
  if ((MtrrCap & IA32_MTRR_SMRR_SUPPORT_BIT) == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Initialize SMRR in SMM relocate.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
**/
VOID
InitSmrr (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize
  )
{
  AsmWriteMsr64 (EFI_MSR_SMRR_PHYS_BASE, SmrrBase| CACHE_WRITE_BACK);
  AsmWriteMsr64 (EFI_MSR_SMRR_PHYS_MASK, (~(SmrrSize - 1) & EFI_MSR_SMRR_MASK)); // Valid bit will be set in ConfigSmrr() at first SMI
}

/**
  Configure SMRR register at each SMM entry.
**/
VOID
ConfigSmrr (
  VOID
  )
{
  UINT64 SmrrMask;

  SmrrMask = AsmReadMsr64 (EFI_MSR_SMRR_PHYS_MASK);
  if ((SmrrMask & EFI_MSR_SMRR_PHYS_MASK_VALID) == 0) {
    AsmWriteMsr64(EFI_MSR_SMRR_PHYS_MASK, SmrrMask | EFI_MSR_SMRR_PHYS_MASK_VALID);
  }
}

////////
// Below section is definition for the supported class
////////

/**
  This function will return current CPU_SMM_CLASS accroding to CPUID mapping.

  @return The point to current CPU_SMM_CLASS

**/
CPU_SMM_CLASS *
GetCpuFamily (
  VOID
  )
{
  UINT32         ClassIndex;
  UINT32         Index;
  UINT32         Count;
  CPUID_MAPPING  *CpuMapping;
  UINT32         RegEax;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  for (ClassIndex = 0; ClassIndex < sizeof(mCpuClasstable)/sizeof(mCpuClasstable[0]); ClassIndex++) {
    CpuMapping = mCpuClasstable[ClassIndex]->MappingTable;
    Count = mCpuClasstable[ClassIndex]->MappingCount;
    for (Index = 0; Index < Count; Index++) {
      if ((CpuMapping[Index].Signature & CpuMapping[Index].Mask) == (RegEax & CpuMapping[Index].Mask)) {
        return mCpuClasstable[ClassIndex];
      }
    }
  }

  // Not found!!! Should not happen
  ASSERT (FALSE);
  return NULL;
}

////////
// Below section is external function
////////
/**
  Read MSR or CSR based on the CPU type Register to read.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access 
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  
  @return 64-bit value read from register.
    
**/
UINT64
SmmReadReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName
  )
{
  UINT64      RetVal;

  RetVal = 0;
    switch (RegName) {
    //
    // Client uses MSR
    //
    case  SmmRegFeatureControl:
      RetVal = AsmReadMsr64 (EFI_MSR_HASWELL_SMM_FEATURE_CONTROL);
      break;
    case  SmmRegSmmDelayed:
      RetVal = AsmReadMsr64 (EFI_MSR_HASWELL_SMM_DELAYED);
      break;
    case  SmmRegSmmBlocked:
      RetVal = AsmReadMsr64 (EFI_MSR_HASWELL_SMM_BLOCKED);
      break;
    default:
      ASSERT (FALSE);
    }
  return  RetVal;
}

/**
  Write MSR or CSR based on the CPU type Register to write.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access 
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  @param[in]  RegValue  64-bit Register value.
    
**/
VOID
SmmWriteReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName,
  IN  UINT64          RegValue
  )
{
    switch (RegName) {
    //
    // Client uses MSR
    //
    case  SmmRegFeatureControl:
      AsmWriteMsr64 (EFI_MSR_HASWELL_SMM_FEATURE_CONTROL, RegValue);
      break;
    default:
      ASSERT (FALSE);
    }
}

/**
  This function will return logical processor index in package.
    
  @param[in]  ProcessorNumber        The processor number.
  @param[out] LogProcIndexPackage    The logical processor index.

  @retval EFI_NOT_FOUND Cannot find the specified processor by ProcessorNumber.
  @retval EFI_SUCCESS   Logical processor index return in LogProcIndexPackage.

**/
EFI_STATUS
GetLogProcIndexInPackage (
  IN  UINTN   ProcessorNumber,
  OUT UINT16  *LogProcIndexPackage 
  )
{
  UINT64      ProcessorId;
  UINT32      PackageId;
  UINTN       Index;
  UINT16      LogProcIndex;

  ProcessorId  = gSmmCpuPrivate->ProcessorInfo[ProcessorNumber].ProcessorId;
  if (ProcessorId == INVALID_APIC_ID) {
    return EFI_NOT_FOUND;
  }

  PackageId    = gSmmCpuPrivate->ProcessorInfo[ProcessorNumber].Location.Package;
  LogProcIndex = 0;   
  for (Index = 0; Index < mMaxNumberOfCpus; Index++) {
    if (gSmmCpuPrivate->ProcessorInfo[Index].Location.Package == PackageId) {
      if (gSmmCpuPrivate->ProcessorInfo[Index].ProcessorId < ProcessorId) { 
        //
        // The logical processor number in the same packet
        //
        LogProcIndex++;
      }
    }
  }

  *LogProcIndexPackage = LogProcIndex;
  return  EFI_SUCCESS;
}

/**
  Return if it is needed to configure MTRR to set TSEG cacheability.

  @retval  TRUE  - we need configure MTRR
  @retval  FALSE - we do not need configure MTRR
**/
BOOLEAN
NeedConfigureMtrrs (
  VOID
  )
{
  ASSERT (mThisCpu != NULL);

  switch (mThisCpu->Family) {
  case CpuCherryView:
    return FALSE;
  default:
    return TRUE;
  }
}

/**
  Processor specific hook point at each SMM entry.

  @param  CpuIndex    The index of the cpu which need to check.

**/
VOID
SmmRendezvousEntry (
  IN UINTN  CpuIndex
  )
{

  ASSERT (mThisCpu != NULL);

  switch (mThisCpu->Family) {
  case CpuCherryView:
    if (mFeatureContext.SmrrEnabled) {
      ConfigSmrr ();
    }
    return;
  default:
    return ;
  }
}

/**
  Processor specific hook point at each SMM exit.

  @param  CpuIndex    The index of the cpu which need to check.
**/
VOID
SmmRendezvousExit (
  IN UINTN  CpuIndex
  )
{

  ASSERT (mThisCpu != NULL);

  switch (mThisCpu->Family) {
  case CpuCherryView:
  default:
    return ;
  }
}

/**
  Initialize SMRR context in SMM Init.
**/
VOID
InitializeSmmMtrrManager (
  VOID
  )
{
  mThisCpu = GetCpuFamily ();
  ASSERT (mThisCpu != NULL);

  switch (mThisCpu->Family) {  
  case CpuCherryView:
    if (!IsSmrrSupported ()) {
      return ;
    }
    mFeatureContext.SmrrEnabled = TRUE;
    return ;
  default:
    return ;
  }
}

/**
  Initialize SMRR/SMBASE/SMM Sync features in SMM Relocate.

  @param  ProcessorNumber    The processor number
  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
  @param  SmBase             The SMBASE value.
  @param  IsBsp              If this processor treated as BSP.
**/
VOID
SmmInitiFeatures (
  IN UINTN   ProcessorNumber,
  IN UINT32  SmrrBase,
  IN UINT32  SmrrSize,
  IN UINT32  SmBase,
  IN BOOLEAN IsBsp
  )
{
  SOCKET_LGA_775_SMM_CPU_STATE      *CpuState;
  SMM_CPU_SYNC_FEATURE              *SyncFeature;

  SyncFeature = &(gSmmCpuPrivate->SmmSyncFeature[ProcessorNumber]);
  SyncFeature->DelayIndicationSupported = FALSE;
  SyncFeature->BlockIndicationSupported = FALSE;
  SyncFeature->HaswellLogProcEnBit = (UINT64)(INT64)(-1);

  mThisCpu = GetCpuFamily ();
  ASSERT (mThisCpu != NULL);

  //
  // Configure SMBASE.
  //
  switch (mThisCpu->Family) {
  case CpuCherryView:
    //
    // Fall back to legacy SMBASE setup.
    //
    CpuState = (SOCKET_LGA_775_SMM_CPU_STATE *)(UINTN)(SMM_DEFAULT_SMBASE + SMM_CPU_STATE_OFFSET);
    CpuState->x86.SMBASE = SmBase;
    break ;
  default:
    return ;
  }

  switch (mThisCpu->Family) {
  case CpuCherryView:
    if (IsSmrrSupported ()) {
      InitSmrr (SmrrBase, SmrrSize);
    }
    return ;
  default:
    ASSERT (FALSE);
    return ;
  }
}

/**
  Configure SMM Code Access Check feature on an AP.
  SMM Feature Control MSR will be locked after configuration.

  @param[in,out] Buffer  Pointer to private data buffer.
**/
VOID
EFIAPI
ConfigSmmCodeAccessCheckOnCurrentProcessor (
  IN OUT VOID  *Buffer
  )
{
  UINT64 SmmFeatureControlMsr;
  UINTN  CpuIndex;

  CpuIndex = *(UINTN *)Buffer;

  SmmFeatureControlMsr = SmmReadReg64 (CpuIndex, SmmRegFeatureControl);
  //
  // The SMM Feature Control MSR is package scope. If lock bit is set, don't set it again.
  //
  if ((SmmFeatureControlMsr & SMM_FEATURE_CONTROL_LOCK_BIT) == 0) {
    if (mSmmCodeAccessCheckEnable) {
      SmmFeatureControlMsr |= SMM_CODE_CHK_EN_BIT;
    }
    if (FeaturePcdGet (PcdCpuSmmFeatureControlMsrLock)) {
      SmmFeatureControlMsr |=SMM_FEATURE_CONTROL_LOCK_BIT;
    }
    SmmWriteReg64 (CpuIndex, SmmRegFeatureControl, SmmFeatureControlMsr);
  }
}

/**
  Configure SMM Code Access Check feature for all processors.
  SMM Feature Control MSR will be locked after configuration.
**/
VOID
ConfigSmmCodeAccessCheck (
  VOID
  )
{
  UINTN      Index;
  EFI_STATUS Status;

  //
  // SMM Code Access Check feature is supported since Haswell.
  //
  if (FALSE /*mThisCpu->Family == CpuHaswell*/) {
    if ((AsmReadMsr64 (EFI_MSR_HASWELL_SMM_MCA_CAP) & SMM_CODE_ACCESS_CHK_BIT) == 0) {
      mSmmCodeAccessCheckEnable = FALSE;
      if (!FeaturePcdGet (PcdCpuSmmFeatureControlMsrLock)) {
        return;
      }
    }
    //
    // Enable SMM Code Access Check feature for the BSP.
    //
    ConfigSmmCodeAccessCheckOnCurrentProcessor (NULL);
    //
    // Enable SMM Code Access Check feature for the APs.
    //
    for (Index = 0; Index < mNumberOfCpus; Index++) {
      if (Index != gSmmCpuPrivate->SmmCoreEntryContext.CurrentlyExecutingCpu) {
        //
        // Don't call gSmst->SmmStartupThisAp() because it may be implemented in a blocking or non-blocking fashion.
        //
        Status = SmmBlockingStartupThisAp (ConfigSmmCodeAccessCheckOnCurrentProcessor, Index, NULL);
        ASSERT_EFI_ERROR (Status);
      }
    }
  }
}

