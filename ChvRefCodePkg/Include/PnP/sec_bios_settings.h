/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef SEC_BIOS_SETTINGS_HH
#define SEC_BIOS_SETTINGS_HH

#define SEC_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x44, 0x88,   0,   0, 0x0,  /* CLK_GATE_DIS.SB_CG_DIS */ \
    0x44, 0x88,   1,   1, 0x0,  /* CLK_GATE_DIS.PRIM_CG_DIS */ \
    0x44, 0x88,   2,   2, 0x0,  /* CLK_GATE_DIS.PRIM_CLKREQ_DIS */ \
    0x44, 0x88,   3,   3, 0x0,  /* CLK_GATE_DIS.PRIM_XSM_CLKREQ_DIS */ \
    0x44, 0x88,   4,   4, 0x0,  /* CLK_GATE_DIS.SAP_CG_DIS */ \
    0x44, 0x88,   5,   5, 0x0,  /* CLK_GATE_DIS.SAP_IP_CG_DIS */ \
    0x44, 0x88,   6,   6, 0x0,  /* CLK_GATE_DIS.SAP_CLKIDLE_DIS */ \
    0x44, 0x88,   7,   7, 0x0,  /* CLK_GATE_DIS.NFC_CG_DIS */ \
/* end of SEC_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define SEC_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x44, 0x88,   0,   0, 0x0,  /* CLK_GATE_DIS.SB_CG_DIS */ \
    0x44, 0x88,   1,   1, 0x0,  /* CLK_GATE_DIS.PRIM_CG_DIS */ \
    0x44, 0x88,   2,   2, 0x0,  /* CLK_GATE_DIS.PRIM_CLKREQ_DIS */ \
    0x44, 0x88,   3,   3, 0x0,  /* CLK_GATE_DIS.PRIM_XSM_CLKREQ_DIS */ \
    0x44, 0x88,   4,   4, 0x0,  /* CLK_GATE_DIS.SAP_CG_DIS */ \
    0x44, 0x88,   5,   5, 0x0,  /* CLK_GATE_DIS.SAP_IP_CG_DIS */ \
    0x44, 0x88,   6,   6, 0x0,  /* CLK_GATE_DIS.SAP_CLKIDLE_DIS */ \
    0x44, 0x88,   7,   7, 0x0,  /* CLK_GATE_DIS.NFC_CG_DIS */ \
/* end of SEC_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define SEC_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x44, 0x88,   0,   0, 0x0,  /* CLK_GATE_DIS.SB_CG_DIS */ \
    0x44, 0x88,   1,   1, 0x0,  /* CLK_GATE_DIS.PRIM_CG_DIS */ \
    0x44, 0x88,   2,   2, 0x0,  /* CLK_GATE_DIS.PRIM_CLKREQ_DIS */ \
    0x44, 0x88,   3,   3, 0x0,  /* CLK_GATE_DIS.PRIM_XSM_CLKREQ_DIS */ \
    0x44, 0x88,   4,   4, 0x0,  /* CLK_GATE_DIS.SAP_CG_DIS */ \
    0x44, 0x88,   5,   5, 0x0,  /* CLK_GATE_DIS.SAP_IP_CG_DIS */ \
    0x44, 0x88,   6,   6, 0x0,  /* CLK_GATE_DIS.SAP_CLKIDLE_DIS */ \
    0x44, 0x88,   7,   7, 0x0,  /* CLK_GATE_DIS.NFC_CG_DIS */ \
/* end of SEC_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // SEC_BIOS_SETTINGS_HH
