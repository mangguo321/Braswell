/** @file
  ssa address map

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef TUNIT_BIOS_SETTINGS_HH
#define TUNIT_BIOS_SETTINGS_HH

#define TUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x02, 0x3,  10,  10, 0x1,  /* T_CTL.TG_DW_POST_PUSH_LOG */ \
    0x02, 0x3,  12,  12, 0x1,  /* T_CTL.TG_NDRAMSNP */ \
    0x02, 0x3,  16,  16, 0x1,  /* T_CTL.ENABLE_IN_ORDER_APIC */ \
    0x02, 0x3,  20,  20, 0x1,  /* T_CTL.SPLIT_GOIWP_MODE */ \
    0x02, 0x4,   4,   1, 0x6,  /* T_MISC_CTL.DPTE_CNT */ \
    0x02, 0x4,  18,  18, 0x1,  /* T_MISC_CTL.DISABLE_IOSF_OUTBOUND_THROTTLE */ \
    0x02, 0x5,   0,   0, 0x1,  /* T_CLKGATE_CTL.IOSF_SB_MSG_CLK_GATE_EN */ \
    0x02, 0x5,   1,   1, 0x1,  /* T_CLKGATE_CTL.IOSF_SB_CFG_REG_CLK_GATE_EN */ \
    0x02, 0x5,   2,   2, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_CLK_GATE_EN */ \
    0x02, 0x5,   3,   3, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_VALID_CLK_GATE_EN */ \
    0x02, 0x5,   4,   4, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_VIOL_CLK_GATE_EN */ \
    0x02, 0x5,   5,   5, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_REQ_CLK_GATE_EN */ \
    0x02, 0x5,   6,   6, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_SNP_STAT_CLK_GATE_EN */ \
    0x02, 0x5,   7,   7, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_WRSTAT_CLK_GATE_EN */ \
    0x02, 0x5,   8,   8, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_B2X_DATSTAT_CLK_GATE_EN */ \
    0x02, 0x5,   9,   9, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_T2A_REQSTAT_CLK_GATE_EN */ \
    0x02, 0x5,  10,  10, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_S2C_RESP_CLK_GATE_EN */ \
    0x02, 0x5,  11,  11, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_OLDST_CLK_GATE_EN */ \
    0x02, 0x5,  12,  12, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_LLST_CLK_GATE_EN */ \
    0x02, 0x5,  13,  13, 0x1,  /* T_CLKGATE_CTL.S2C_REQ_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  14,  14, 0x1,  /* T_CLKGATE_CTL.S2C_REQ_FIFO_CLK_GATE_EN */ \
    0x02, 0x5,  15,  15, 0x1,  /* T_CLKGATE_CTL.C2APIC_FIFO_CLK_GATE_EN */ \
    0x02, 0x5,  16,  16, 0x1,  /* T_CLKGATE_CTL.T2A_REQ_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  17,  17, 0x1,  /* T_CLKGATE_CTL.S2C_RESP_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  18,  18, 0x1,  /* T_CLKGATE_CTL.X2B_DATSEL_CLK_GATE_EN */ \
    0x02, 0x5,  19,  19, 0x1,  /* T_CLKGATE_CTL.B2X_DATSEL_CLK_GATE_EN */ \
    0x02, 0x5,  20,  20, 0x1,  /* T_CLKGATE_CTL.A2TAPIC_CLK_GATE_EN */ \
    0x02, 0x5,  21,  21, 0x1,  /* T_CLKGATE_CTL.T2A_Q_CLK_GATE_EN */ \
    0x02, 0x5,  22,  22, 0x1,  /* T_CLKGATE_CTL.A2T_Q_CLK_GATE_EN */ \
    0x02, 0x5,  23,  23, 0x1,  /* T_CLKGATE_CTL.MON_LOG_CLK_GATE_EN */ \
    0x02, 0x5,  24,  24, 0x1,  /* T_CLKGATE_CTL.XUNIT_1_CLK_GATE_EN */ \
    0x02, 0x5,  25,  25, 0x1,  /* T_CLKGATE_CTL.XUNIT_2_CLK_GATE_EN */ \
    0x02, 0x5,  26,  26, 0x1,  /* T_CLKGATE_CTL.XUNIT_3_CLK_GATE_EN */ \
    0x02, 0x5,  27,  27, 0x1,  /* T_CLKGATE_CTL.XUNIT_4_CLK_GATE_EN */ \
/* end of TUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define TUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x02, 0x3,  10,  10, 0x1,  /* T_CTL.TG_DW_POST_PUSH_LOG */ \
    0x02, 0x3,  12,  12, 0x1,  /* T_CTL.TG_NDRAMSNP */ \
    0x02, 0x3,  16,  16, 0x1,  /* T_CTL.ENABLE_IN_ORDER_APIC */ \
    0x02, 0x3,  20,  20, 0x1,  /* T_CTL.SPLIT_GOIWP_MODE */ \
    0x02, 0x4,   0,   0, 0x1,  /* T_MISC_CTL.DPTE_EN */ \
    0x02, 0x4,   4,   1, 0xa,  /* T_MISC_CTL.DPTE_CNT */ \
    0x02, 0x4,  18,  18, 0x1,  /* T_MISC_CTL.DISABLE_IOSF_OUTBOUND_THROTTLE */ \
    0x02, 0x5,   0,   0, 0x1,  /* T_CLKGATE_CTL.IOSF_SB_MSG_CLK_GATE_EN */ \
    0x02, 0x5,   1,   1, 0x1,  /* T_CLKGATE_CTL.IOSF_SB_CFG_REG_CLK_GATE_EN */ \
    0x02, 0x5,   2,   2, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_CLK_GATE_EN */ \
    0x02, 0x5,   3,   3, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_VALID_CLK_GATE_EN */ \
    0x02, 0x5,   4,   4, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_VIOL_CLK_GATE_EN */ \
    0x02, 0x5,   5,   5, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_REQ_CLK_GATE_EN */ \
    0x02, 0x5,   6,   6, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_SNP_STAT_CLK_GATE_EN */ \
    0x02, 0x5,   7,   7, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_WRSTAT_CLK_GATE_EN */ \
    0x02, 0x5,   8,   8, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_B2X_DATSTAT_CLK_GATE_EN */ \
    0x02, 0x5,   9,   9, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_T2A_REQSTAT_CLK_GATE_EN */ \
    0x02, 0x5,  10,  10, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_S2C_RESP_CLK_GATE_EN */ \
    0x02, 0x5,  11,  11, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_OLDST_CLK_GATE_EN */ \
    0x02, 0x5,  12,  12, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_LLST_CLK_GATE_EN */ \
    0x02, 0x5,  13,  13, 0x1,  /* T_CLKGATE_CTL.S2C_REQ_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  14,  14, 0x1,  /* T_CLKGATE_CTL.S2C_REQ_FIFO_CLK_GATE_EN */ \
    0x02, 0x5,  15,  15, 0x1,  /* T_CLKGATE_CTL.C2APIC_FIFO_CLK_GATE_EN */ \
    0x02, 0x5,  16,  16, 0x1,  /* T_CLKGATE_CTL.T2A_REQ_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  17,  17, 0x1,  /* T_CLKGATE_CTL.S2C_RESP_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  18,  18, 0x1,  /* T_CLKGATE_CTL.X2B_DATSEL_CLK_GATE_EN */ \
    0x02, 0x5,  19,  19, 0x1,  /* T_CLKGATE_CTL.B2X_DATSEL_CLK_GATE_EN */ \
    0x02, 0x5,  20,  20, 0x1,  /* T_CLKGATE_CTL.A2TAPIC_CLK_GATE_EN */ \
    0x02, 0x5,  21,  21, 0x1,  /* T_CLKGATE_CTL.T2A_Q_CLK_GATE_EN */ \
    0x02, 0x5,  22,  22, 0x1,  /* T_CLKGATE_CTL.A2T_Q_CLK_GATE_EN */ \
    0x02, 0x5,  23,  23, 0x1,  /* T_CLKGATE_CTL.MON_LOG_CLK_GATE_EN */ \
    0x02, 0x5,  24,  24, 0x1,  /* T_CLKGATE_CTL.XUNIT_1_CLK_GATE_EN */ \
    0x02, 0x5,  25,  25, 0x1,  /* T_CLKGATE_CTL.XUNIT_2_CLK_GATE_EN */ \
    0x02, 0x5,  26,  26, 0x1,  /* T_CLKGATE_CTL.XUNIT_3_CLK_GATE_EN */ \
    0x02, 0x5,  27,  27, 0x1,  /* T_CLKGATE_CTL.XUNIT_4_CLK_GATE_EN */ \
/* end of TUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define TUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x02, 0x3,  12,  12, 0x1,  /* T_CTL.TG_NDRAMSNP */ \
    0x02, 0x5,   0,   0, 0x1,  /* T_CLKGATE_CTL.IOSF_SB_MSG_CLK_GATE_EN */ \
    0x02, 0x5,   1,   1, 0x1,  /* T_CLKGATE_CTL.IOSF_SB_CFG_REG_CLK_GATE_EN */ \
    0x02, 0x5,   2,   2, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_CLK_GATE_EN */ \
    0x02, 0x5,   3,   3, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_VALID_CLK_GATE_EN */ \
    0x02, 0x5,   4,   4, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_VIOL_CLK_GATE_EN */ \
    0x02, 0x5,   5,   5, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_REQ_CLK_GATE_EN */ \
    0x02, 0x5,   6,   6, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_SNP_STAT_CLK_GATE_EN */ \
    0x02, 0x5,   7,   7, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_WRSTAT_CLK_GATE_EN */ \
    0x02, 0x5,   8,   8, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_B2X_DATSTAT_CLK_GATE_EN */ \
    0x02, 0x5,   9,   9, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_T2A_REQSTAT_CLK_GATE_EN */ \
    0x02, 0x5,  10,  10, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_S2C_RESP_CLK_GATE_EN */ \
    0x02, 0x5,  11,  11, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_OLDST_CLK_GATE_EN */ \
    0x02, 0x5,  12,  12, 0x1,  /* T_CLKGATE_CTL.TRKR_SB_LLST_CLK_GATE_EN */ \
    0x02, 0x5,  13,  13, 0x1,  /* T_CLKGATE_CTL.S2C_REQ_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  14,  14, 0x1,  /* T_CLKGATE_CTL.S2C_REQ_FIFO_CLK_GATE_EN */ \
    0x02, 0x5,  15,  15, 0x1,  /* T_CLKGATE_CTL.C2APIC_FIFO_CLK_GATE_EN */ \
    0x02, 0x5,  16,  16, 0x1,  /* T_CLKGATE_CTL.T2A_REQ_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  17,  17, 0x1,  /* T_CLKGATE_CTL.S2C_RESP_SEL_CLK_GATE_EN */ \
    0x02, 0x5,  18,  18, 0x1,  /* T_CLKGATE_CTL.X2B_DATSEL_CLK_GATE_EN */ \
    0x02, 0x5,  19,  19, 0x1,  /* T_CLKGATE_CTL.B2X_DATSEL_CLK_GATE_EN */ \
    0x02, 0x5,  20,  20, 0x1,  /* T_CLKGATE_CTL.A2TAPIC_CLK_GATE_EN */ \
    0x02, 0x5,  21,  21, 0x1,  /* T_CLKGATE_CTL.T2A_Q_CLK_GATE_EN */ \
    0x02, 0x5,  22,  22, 0x1,  /* T_CLKGATE_CTL.A2T_Q_CLK_GATE_EN */ \
    0x02, 0x5,  23,  23, 0x1,  /* T_CLKGATE_CTL.MON_LOG_CLK_GATE_EN */ \
    0x02, 0x5,  24,  24, 0x1,  /* T_CLKGATE_CTL.XUNIT_1_CLK_GATE_EN */ \
    0x02, 0x5,  25,  25, 0x1,  /* T_CLKGATE_CTL.XUNIT_2_CLK_GATE_EN */ \
    0x02, 0x5,  26,  26, 0x1,  /* T_CLKGATE_CTL.XUNIT_3_CLK_GATE_EN */ \
    0x02, 0x5,  27,  27, 0x1,  /* T_CLKGATE_CTL.XUNIT_4_CLK_GATE_EN */ \
/* end of TUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // TUNIT_BIOS_SETTINGS_HH
