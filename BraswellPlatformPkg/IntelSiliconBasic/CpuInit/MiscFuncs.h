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

#ifndef _MISC_FUNCS_H_
#define _MISC_FUNCS_H_

#include "CpuDxe.h"
#include "PlatformMpService.h"

VOID
EfiWriteToScript (
  IN UINT32     Index,
  IN UINT64     Value
  );

VOID
EfiWriteMsrWithScript (
  IN UINT32     Index,
  IN UINT64     Value
  );

VOID
ProgramProcessorFuncs (
  IN  MP_SYSTEM_DATA               *MPSystemData
  );

VOID
CpuMiscEnable (
  BOOLEAN        Enable,
  UINT64         BitMask
);

#endif
