/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef PCIE3_CFG_PRI_SB_PAIR_BIOS_SETTINGS_HH
#define PCIE3_CFG_PRI_SB_PAIR_BIOS_SETTINGS_HH

#define PCIE3_CFG_PRI_SB_PAIR_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa4, 0xd4,   4,   4, 0x1,  /* MPC2.ASPMCOEN */ \
/* end of PCIE3_CFG_PRI_SB_PAIR_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define PCIE3_CFG_PRI_SB_PAIR_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa4, 0x50,   1,   0, 0x3,  /* LCTL_LSTS.ASPM */ \
    0xa4, 0xd4,   3,   2, 0x3,  /* MPC2.ASPMCO */ \
    0xa4, 0xe0,   8,   8, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPDBCGEN */ \
    0xa4, 0xe0,   9,   9, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPDLCGEN */ \
    0xa4, 0xe0,  10,  10, 0x1,  /* RWC_RPDCGEN_RPPGEN.SRDBCGEN */ \
    0xa4, 0xe0,  11,  11, 0x1,  /* RWC_RPDCGEN_RPPGEN.SRDLCGEN */ \
    0xa4, 0xe0,  13,  13, 0x1,  /* RWC_RPDCGEN_RPPGEN.PCIELCLKREQEN */ \
    0xa4, 0xe0,  15,  15, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPSCGEN */ \
    0xa4, 0xe0,  16,  16, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPPGEN */ \
    0xa4, 0xe8,   0,   0, 0x1,  /* PWRCTL.RPDTSQPOL */ \
    0xa4, 0xe8,   1,   1, 0x1,  /* PWRCTL.RPL1SQPOL */ \
    0xa4, 0xf4,   8,   8, 0x1,  /* PHYCTL_PHYCTL2_PHYCTL3_IOSFSBCS.PXPPLLOFFEN */ \
    0xa4, 0x208,   0,   0, 0x1,  /* L1SCTL1.L1OE */ \
    0xa4, 0x208,   1,   1, 0x1,  /* L1SCTL1.L1SE */ \
/* end of PCIE3_CFG_PRI_SB_PAIR_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define PCIE3_CFG_PRI_SB_PAIR_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa4, 0x50,   1,   0, 0x3,  /* LCTL_LSTS.ASPM */ \
    0xa4, 0xd4,   3,   2, 0x3,  /* MPC2.ASPMCO */ \
    0xa4, 0xe0,   8,   8, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPDBCGEN */ \
    0xa4, 0xe0,   9,   9, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPDLCGEN */ \
    0xa4, 0xe0,  10,  10, 0x1,  /* RWC_RPDCGEN_RPPGEN.SRDBCGEN */ \
    0xa4, 0xe0,  11,  11, 0x1,  /* RWC_RPDCGEN_RPPGEN.SRDLCGEN */ \
    0xa4, 0xe0,  13,  13, 0x1,  /* RWC_RPDCGEN_RPPGEN.PCIELCLKREQEN */ \
    0xa4, 0xe0,  15,  15, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPSCGEN */ \
    0xa4, 0xe0,  16,  16, 0x1,  /* RWC_RPDCGEN_RPPGEN.RPPGEN */ \
    0xa4, 0xe8,   0,   0, 0x1,  /* PWRCTL.RPDTSQPOL */ \
    0xa4, 0xe8,   1,   1, 0x1,  /* PWRCTL.RPL1SQPOL */ \
    0xa4, 0xf4,   8,   8, 0x1,  /* PHYCTL_PHYCTL2_PHYCTL3_IOSFSBCS.PXPPLLOFFEN */ \
    0xa4, 0x208,   0,   0, 0x1,  /* L1SCTL1.L1OE */ \
    0xa4, 0x208,   1,   1, 0x1,  /* L1SCTL1.L1SE */ \
/* end of PCIE3_CFG_PRI_SB_PAIR_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // PCIE3_CFG_PRI_SB_PAIR_BIOS_SETTINGS_HH
