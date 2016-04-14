/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef IUNIT_BIOS_SETTINGS_HH
#define IUNIT_BIOS_SETTINGS_HH

#define IUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x1c, 0x118,   6,   5, 0x2,  /* IUNIT_CONTROL.ISPCLK_GATING_DISABLE */ \
    0x1c, 0x118,  15,   8, 0xff,  /* IUNIT_CONTROL.MID */ \
/* end of IUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define IUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x1c, 0x118,  15,   8, 0x0,  /* IUNIT_CONTROL.MID */ \
/* end of IUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define IUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x1c, 0x118,   6,   5, 0x2,  /* IUNIT_CONTROL.ISPCLK_GATING_DISABLE */ \
/* end of IUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // IUNIT_BIOS_SETTINGS_HH
