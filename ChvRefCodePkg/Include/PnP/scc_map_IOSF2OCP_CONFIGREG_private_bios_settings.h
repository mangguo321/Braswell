/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_BIOS_SETTINGS_HH
#define SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_BIOS_SETTINGS_HH

#define SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x63, 0x243,   0,   0, 0x1,  /* PMCTL.IOSFPRIMCLK_GATE_EN */ \
    0x63, 0x243,   1,   1, 0x1,  /* PMCTL.OCPCLK_GATE_EN */ \
    0x63, 0x243,   2,   2, 0x1,  /* PMCTL.OCPCLK_TRUNK_GATE_EN */ \
    0x63, 0x243,   3,   3, 0x1,  /* PMCTL.IOSFSBCLK_GATE_EN */ \
    0x63, 0x243,   4,   4, 0x1,  /* PMCTL.IOSFPRIM_TRUNK_GATE_EN */ \
    0x63, 0x243,   5,   5, 0x1,  /* PMCTL.IOSFSB_TRUNK_GATE_EN */ \
    0x63, 0x663,   1,   0, 0x1,  /* GEN_REGRW1.CR_CLOCK_ENABLE_CLK_XIN */ \
    0x63, 0x663,   3,   2, 0x1,  /* GEN_REGRW1.CR_CLOCK_ENABLE_CLK_OCP */ \
/* end of SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x63, 0x1e0,   0,   0, 0x1,  /* PMCTL.IOSFPRIMCLK_GATE_EN */ \
    0x63, 0x1e0,   1,   1, 0x1,  /* PMCTL.OCPCLK_GATE_EN */ \
    0x63, 0x1e0,   2,   2, 0x1,  /* PMCTL.OCPCLK_TRUNK_GATE_EN */ \
    0x63, 0x1e0,   3,   3, 0x1,  /* PMCTL.IOSFSBCLK_GATE_EN */ \
    0x63, 0x1e0,   4,   4, 0x1,  /* PMCTL.IOSFPRIM_TRUNK_GATE_EN */ \
    0x63, 0x1e0,   5,   5, 0x1,  /* PMCTL.IOSFSB_TRUNK_GATE_EN */ \
    0x63, 0x600,   1,   0, 0x1,  /* GEN_REGRW1.CR_CLOCK_ENABLE_CLK_XIN */ \
    0x63, 0x600,   3,   2, 0x1,  /* GEN_REGRW1.CR_CLOCK_ENABLE_CLK_OCP */ \
/* end of SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // SCC_MAP_IOSF2OCP_CONFIGREG_PRIVATE_BIOS_SETTINGS_HH
