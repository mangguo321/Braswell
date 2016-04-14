/** @file
  Provide the misc functions to enable some CPU features.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CpuDxe.h"
#include "PlatformMpService.h"
#include "MiscFuncs.h"

extern MP_SYSTEM_DATA               *mMPSystemData;

/**
  @todo	add description

**/
VOID
EfiWriteToScript (
  IN UINT32     Index,
  IN UINT64     Value
  )
{
  UINTN   TableIndex;

  if (mMPSystemData == NULL) {
    ASSERT(mMPSystemData != NULL);
    return;
  }

  //
  // Save it into script
  //
  AsmAcquireMPLock  (&(mMPSystemData->S3BootScriptLock));
  TableIndex = mMPSystemData->S3BootScriptCount++;
  AsmReleaseMPLock  (&(mMPSystemData->S3BootScriptLock));

  ASSERT (TableIndex < MAX_CPU_S3_TABLE_SIZE - 1);
  mMPSystemData->S3BootScriptTable[TableIndex].ApicId   = GetApicID (NULL, NULL);
  mMPSystemData->S3BootScriptTable[TableIndex].MsrIndex = Index;
  mMPSystemData->S3BootScriptTable[TableIndex].MsrValue = Value;
}

/**
  @todo	add description

**/
VOID
EfiWriteMsrWithScript (
  IN UINT32     Index,
  IN UINT64     Value
  )
{
  AsmWriteMsr64 (Index, Value);
  EfiWriteToScript (Index, Value);
}

/**
  Provide access to the CPU misc enables MSR

  @param[in]  Enable  -Enable or Disable Misc Features

**/
VOID
CpuMiscEnable (
  BOOLEAN        Enable,
  UINT64         BitMask
)
{
  UINT64  MsrValue;

  MsrValue = AsmReadMsr64 (EFI_MSR_IA32_MISC_ENABLE);
  if (Enable) {
    MsrValue |=  BitMask;
  } else {
    MsrValue &= ~BitMask;
  }
  AsmWriteMsr64 (EFI_MSR_IA32_MISC_ENABLE, MsrValue);
}

/**
  @todo	add description

**/
VOID
ProgramProcessorFuncs (
  IN  MP_SYSTEM_DATA               *MPSystemData
  )
{
  //
  // Initialize some misc functions
  //
  CpuMiscEnable (MPSystemData->MonitorMwaitEnable,     B_EFI_MSR_IA32_MISC_ENABLE_MONITOR);

}
