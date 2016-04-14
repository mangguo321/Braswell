/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef LPESHIM_BIOS_SETTINGS_HH
#define LPESHIM_BIOS_SETTINGS_HH

#define LPESHIM_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x58, 0x78,   3,   0, 0x7,  /* CLKCTL.CLK_DIV */ \
    0x58, 0x80,   3,   0, 0x7,  /* FR_LAT_REQ.REQUIRED_FREQ */ \
/* end of LPESHIM_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define LPESHIM_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x58, 0x78,   3,   0, 0x0,  /* CLKCTL.CLK_DIV */ \
    0x58, 0x80,   6,   4, 0x7,  /* FR_LAT_REQ.LATENCY_TOL */ \
/* end of LPESHIM_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define LPESHIM_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x58, 0x78,   3,   0, 0x4,  /* CLKCTL.CLK_DIV */ \
    0x58, 0x80,   3,   0, 0x4,  /* FR_LAT_REQ.REQUIRED_FREQ */ \
    0x58, 0x80,   6,   4, 0x3,  /* FR_LAT_REQ.LATENCY_TOL */ \
/* end of LPESHIM_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // LPESHIM_BIOS_SETTINGS_HH
