/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef DRNG_BIOS_SETTINGS_HH
#define DRNG_BIOS_SETTINGS_HH

#define DRNG_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x0f, 0x3,  17,  17, 0x1,  /* CONFIG.DRBG_DISABLE_CLK_GATING */ \
/* end of DRNG_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#endif // DRNG_BIOS_SETTINGS_HH
