/** @file
  Defines and prototypes for the DigitalThermalSensor SMM driver

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _DTS_LIB_H_
#define _DTS_LIB_H_
//
// Include files
//
#include <Library/KscLib.h>

EFI_STATUS
InitializeDtsHookLib (
  VOID
);

VOID
PlatformHookBeforeGenerateSCI (
  VOID
);

UINT8
ReadPlatformThermalDiode (
   VOID
);

VOID
PlatformEventOutOfSpec (
  VOID
);

#endif
