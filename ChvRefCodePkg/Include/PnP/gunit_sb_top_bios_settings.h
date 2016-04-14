/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef GUNIT_SB_TOP_BIOS_SETTINGS_HH
#define GUNIT_SB_TOP_BIOS_SETTINGS_HH

#define GUNIT_SB_TOP_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x06, 0x9424,   1,   1, 0x0,  /* MISCCPCTL.L1UGTEN4RST */ \
/* end of GUNIT_SB_TOP_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define GUNIT_SB_TOP_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x06, 0x9424,   1,   1, 0x0,  /* MISCCPCTL.L1UGTEN4RST */ \
/* end of GUNIT_SB_TOP_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define GUNIT_SB_TOP_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x06, 0x9424,   1,   1, 0x0,  /* MISCCPCTL.L1UGTEN4RST */ \
/* end of GUNIT_SB_TOP_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // GUNIT_SB_TOP_BIOS_SETTINGS_HH
