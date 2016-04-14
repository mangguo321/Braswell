/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef MAP_CONFIG_REG_1_0_AHB_PRIVATE_BIOS_SETTINGS_HH
#define MAP_CONFIG_REG_1_0_AHB_PRIVATE_BIOS_SETTINGS_HH

#define MAP_CONFIG_REG_1_0_AHB_PRIVATE_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa0, 0x1f4,   0,   0, 0x1,  /* GPIODF0.CR_CLOCK_ENABLE_BASE_CLK_BIT_1 */ \
    0xa0, 0x1f4,   1,   1, 0x1,  /* GPIODF0.CR_CLOCK_ENABLE_AHB_CLK_BIT_1 */ \
    0xa0, 0x220,   0,   0, 0x1,  /* PMCTL.IOSF_CLK_GATE_ENABLE */ \
    0xa0, 0x220,   1,   1, 0x1,  /* PMCTL.AHB_CLK_GATE_EN */ \
    0xa0, 0x220,   2,   2, 0x1,  /* PMCTL.SIDE_CLK_GATE_ENABLE */ \
    0xa0, 0x220,   3,   3, 0x1,  /* PMCTL.IOSFPRIM_TRUNK_GATE_ENABLE */ \
    0xa0, 0x220,   4,   4, 0x1,  /* PMCTL.AHB_TRUNK_GATE_ENABLE */ \
/* end of MAP_CONFIG_REG_1_0_AHB_PRIVATE_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#endif // MAP_CONFIG_REG_1_0_AHB_PRIVATE_BIOS_SETTINGS_HH
