/** @file
  ICH9 ACPI Timer implements one instance of Timer Library.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __I2C_DELAY_PEI__

#define __I2C_DELAY_PEI__
#include "PiPei.h"

/**
  Stalls the CPU for the number of microseconds specified by MicroSeconds.

  @param[in]  MicroSeconds  The minimum number of microseconds to delay.

  @return MicroSeconds

**/
EFI_STATUS
EFIAPI
I2CMicroSecondDelay (
  IN      UINTN                     MicroSeconds
  );

#endif
