/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef CUNIT_BIOS_SETTINGS_HH
#define CUNIT_BIOS_SETTINGS_HH

#define CUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x08, 0x43,   7,   0, 0x30,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.IDLE_CNT_VAL */ \
    0x08, 0x43,  16,  16, 0x1,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.IDLE_CNT_DEC_CTL */ \
    0x08, 0x43,  18,  18, 0x1,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.SSA_CORE_CLK_GATE_EN */ \
/* end of CUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define CUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x08, 0x43,   7,   0, 0x30,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.IDLE_CNT_VAL */ \
    0x08, 0x43,  16,  16, 0x1,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.IDLE_CNT_DEC_CTL */ \
    0x08, 0x43,  18,  18, 0x1,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.SSA_CORE_CLK_GATE_EN */ \
/* end of CUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define CUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x08, 0x43,   7,   0, 0x30,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.IDLE_CNT_VAL */ \
    0x08, 0x43,  16,  16, 0x1,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.IDLE_CNT_DEC_CTL */ \
    0x08, 0x43,  18,  18, 0x1,  /* CUNIT_SSA_REGIONAL_TRUNKGATE_CTL.SSA_CORE_CLK_GATE_EN */ \
/* end of CUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // CUNIT_BIOS_SETTINGS_HH
