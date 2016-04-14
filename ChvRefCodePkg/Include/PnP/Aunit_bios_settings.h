/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef AUNIT_BIOS_SETTINGS_HH
#define AUNIT_BIOS_SETTINGS_HH

#define AUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x00, 0x18,   0,   0, 0x1,  /* ACKGATE.CONFIG_REGS */ \
    0x00, 0x18,   1,   1, 0x1,  /* ACKGATE.TA_DATA_QUEUE */ \
    0x00, 0x18,   2,   2, 0x1,  /* ACKGATE.TA_REQ_QUEUE */ \
    0x00, 0x18,   3,   3, 0x1,  /* ACKGATE.AT_DATA_QUEUE */ \
    0x00, 0x18,   4,   4, 0x1,  /* ACKGATE.AT_REQ_QUEUE */ \
    0x00, 0x18,   5,   5, 0x1,  /* ACKGATE.AIOSFDOWN_CMD_DATA_BUFF */ \
    0x00, 0x18,   6,   6, 0x1,  /* ACKGATE.AIOSFDOWN_CMD_DRVR */ \
    0x00, 0x18,   7,   7, 0x1,  /* ACKGATE.AREQDOWN_DATA_QUEUE */ \
    0x00, 0x18,   8,   8, 0x1,  /* ACKGATE.AREQDOWN_RSP_QUEUE */ \
    0x00, 0x18,   9,   9, 0x1,  /* ACKGATE.AREQUP_DATA_QUEUE */ \
    0x00, 0x18,  10,  10, 0x1,  /* ACKGATE.AREQUP_CMD_QUEUE */ \
    0x00, 0x18,  11,  11, 0x1,  /* ACKGATE.ASCLUP_DATA_QUEUE */ \
    0x00, 0x18,  12,  12, 0x1,  /* ACKGATE.ASCLUP_CMD_QUEUE */ \
    0x00, 0x18,  14,  14, 0x1,  /* ACKGATE.ASCLUP_IOSF_ADAPTER */ \
    0x00, 0x18,  15,  15, 0x1,  /* ACKGATE.AIOSFDOWN_DATA */ \
    0x00, 0x18,  16,  16, 0x1,  /* ACKGATE.ASCLUP_FAIR_ARBITER */ \
    0x00, 0x18,  17,  17, 0x1,  /* ACKGATE.AREQDOWN_CREDIT */ \
    0x00, 0x18,  18,  18, 0x1,  /* ACKGATE.AREQDOWN_TAREQQ */ \
    0x00, 0x18,  19,  19, 0x1,  /* ACKGATE.AREQTAHACK */ \
    0x00, 0x18,  20,  20, 0x1,  /* ACKGATE.AREQUP_MIRROR */ \
    0x00, 0x18,  21,  21, 0x1,  /* ACKGATE.AREQDOWN_SCL0_ARB */ \
    0x00, 0x18,  22,  22, 0x1,  /* ACKGATE.AMESSAGE_MSGIF */ \
    0x00, 0x20,   8,   0, 0x1,  /* AISOCHCTL.VC1_ISOC_CH_DEFAULT_DDLINE_DLY */ \
    0x00, 0x21,  31,  31, 0x1,  /* AVCCTL.EFFICIENT_PERF_UP_EN */ \
    0x00, 0xc0,   3,   0, 0x4,  /* AARBCTL0.IOSF0VC0_WGT */ \
    0x00, 0xc0,   7,   4, 0x4,  /* AARBCTL0.IOSF0VC1_WGT */ \
    0x00, 0xc0,  11,   8, 0x4,  /* AARBCTL0.IOSF0VC2_WGT */ \
    0x00, 0xd0,   3,   0, 0x8,  /* ADNARBCTL.VC0_WGT */ \
    0x00, 0xd0,   7,   4, 0x4,  /* ADNARBCTL.VC1_WGT */ \
    0x00, 0xd0,  11,   8, 0x4,  /* ADNARBCTL.VC2_WGT */ \
    0x00, 0xd1,   3,   0, 0x4,  /* AVC0ARBCTL.BA_WGT */ \
    0x00, 0xd1,   7,   4, 0x4,  /* AVC0ARBCTL.TA_WGT */ \
/* end of AUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define AUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x00, 0x18,   0,   0, 0x1,  /* ACKGATE.CONFIG_REGS */ \
    0x00, 0x18,   1,   1, 0x1,  /* ACKGATE.TA_DATA_QUEUE */ \
    0x00, 0x18,   2,   2, 0x1,  /* ACKGATE.TA_REQ_QUEUE */ \
    0x00, 0x18,   3,   3, 0x1,  /* ACKGATE.AT_DATA_QUEUE */ \
    0x00, 0x18,   4,   4, 0x1,  /* ACKGATE.AT_REQ_QUEUE */ \
    0x00, 0x18,   5,   5, 0x1,  /* ACKGATE.AIOSFDOWN_CMD_DATA_BUFF */ \
    0x00, 0x18,   6,   6, 0x1,  /* ACKGATE.AIOSFDOWN_CMD_DRVR */ \
    0x00, 0x18,   7,   7, 0x1,  /* ACKGATE.AREQDOWN_DATA_QUEUE */ \
    0x00, 0x18,   8,   8, 0x1,  /* ACKGATE.AREQDOWN_RSP_QUEUE */ \
    0x00, 0x18,   9,   9, 0x1,  /* ACKGATE.AREQUP_DATA_QUEUE */ \
    0x00, 0x18,  10,  10, 0x1,  /* ACKGATE.AREQUP_CMD_QUEUE */ \
    0x00, 0x18,  11,  11, 0x1,  /* ACKGATE.ASCLUP_DATA_QUEUE */ \
    0x00, 0x18,  12,  12, 0x1,  /* ACKGATE.ASCLUP_CMD_QUEUE */ \
    0x00, 0x18,  14,  14, 0x1,  /* ACKGATE.ASCLUP_IOSF_ADAPTER */ \
    0x00, 0x18,  15,  15, 0x1,  /* ACKGATE.AIOSFDOWN_DATA */ \
    0x00, 0x18,  16,  16, 0x1,  /* ACKGATE.ASCLUP_FAIR_ARBITER */ \
    0x00, 0x18,  17,  17, 0x1,  /* ACKGATE.AREQDOWN_CREDIT */ \
    0x00, 0x18,  18,  18, 0x1,  /* ACKGATE.AREQDOWN_TAREQQ */ \
    0x00, 0x18,  19,  19, 0x1,  /* ACKGATE.AREQTAHACK */ \
    0x00, 0x18,  20,  20, 0x1,  /* ACKGATE.AREQUP_MIRROR */ \
    0x00, 0x18,  21,  21, 0x1,  /* ACKGATE.AREQDOWN_SCL0_ARB */ \
    0x00, 0x18,  22,  22, 0x1,  /* ACKGATE.AMESSAGE_MSGIF */ \
    0x00, 0x20,   8,   0, 0x1,  /* AISOCHCTL.VC1_ISOC_CH_DEFAULT_DDLINE_DLY */ \
    0x00, 0x21,  31,  31, 0x1,  /* AVCCTL.EFFICIENT_PERF_UP_EN */ \
    0x00, 0xc0,   3,   0, 0x4,  /* AARBCTL0.IOSF0VC0_WGT */ \
    0x00, 0xc0,   7,   4, 0x4,  /* AARBCTL0.IOSF0VC1_WGT */ \
    0x00, 0xc0,  11,   8, 0x4,  /* AARBCTL0.IOSF0VC2_WGT */ \
    0x00, 0xd0,   3,   0, 0x8,  /* ADNARBCTL.VC0_WGT */ \
    0x00, 0xd0,   7,   4, 0x4,  /* ADNARBCTL.VC1_WGT */ \
    0x00, 0xd0,  11,   8, 0x4,  /* ADNARBCTL.VC2_WGT */ \
    0x00, 0xd1,   3,   0, 0x4,  /* AVC0ARBCTL.BA_WGT */ \
    0x00, 0xd1,   7,   4, 0x4,  /* AVC0ARBCTL.TA_WGT */ \
/* end of AUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define AUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x00, 0x18,   0,   0, 0x1,  /* ACKGATE.CONFIG_REGS */ \
    0x00, 0x18,   1,   1, 0x1,  /* ACKGATE.TA_DATA_QUEUE */ \
    0x00, 0x18,   2,   2, 0x1,  /* ACKGATE.TA_REQ_QUEUE */ \
    0x00, 0x18,   3,   3, 0x1,  /* ACKGATE.AT_DATA_QUEUE */ \
    0x00, 0x18,   4,   4, 0x1,  /* ACKGATE.AT_REQ_QUEUE */ \
    0x00, 0x18,   5,   5, 0x1,  /* ACKGATE.AIOSFDOWN_CMD_DATA_BUFF */ \
    0x00, 0x18,   6,   6, 0x1,  /* ACKGATE.AIOSFDOWN_CMD_DRVR */ \
    0x00, 0x18,   7,   7, 0x1,  /* ACKGATE.AREQDOWN_DATA_QUEUE */ \
    0x00, 0x18,   8,   8, 0x1,  /* ACKGATE.AREQDOWN_RSP_QUEUE */ \
    0x00, 0x18,   9,   9, 0x1,  /* ACKGATE.AREQUP_DATA_QUEUE */ \
    0x00, 0x18,  10,  10, 0x1,  /* ACKGATE.AREQUP_CMD_QUEUE */ \
    0x00, 0x18,  11,  11, 0x1,  /* ACKGATE.ASCLUP_DATA_QUEUE */ \
    0x00, 0x18,  12,  12, 0x1,  /* ACKGATE.ASCLUP_CMD_QUEUE */ \
    0x00, 0x18,  14,  14, 0x1,  /* ACKGATE.ASCLUP_IOSF_ADAPTER */ \
    0x00, 0x18,  15,  15, 0x1,  /* ACKGATE.AIOSFDOWN_DATA */ \
    0x00, 0x18,  16,  16, 0x1,  /* ACKGATE.ASCLUP_FAIR_ARBITER */ \
    0x00, 0x18,  17,  17, 0x1,  /* ACKGATE.AREQDOWN_CREDIT */ \
    0x00, 0x18,  18,  18, 0x1,  /* ACKGATE.AREQDOWN_TAREQQ */ \
    0x00, 0x18,  19,  19, 0x1,  /* ACKGATE.AREQTAHACK */ \
    0x00, 0x18,  20,  20, 0x1,  /* ACKGATE.AREQUP_MIRROR */ \
    0x00, 0x18,  21,  21, 0x1,  /* ACKGATE.AREQDOWN_SCL0_ARB */ \
    0x00, 0x18,  22,  22, 0x1,  /* ACKGATE.AMESSAGE_MSGIF */ \
    0x00, 0x20,   8,   0, 0x1,  /* AISOCHCTL.VC1_ISOC_CH_DEFAULT_DDLINE_DLY */ \
    0x00, 0xc0,   3,   0, 0x4,  /* AARBCTL0.IOSF0VC0_WGT */ \
    0x00, 0xc0,   7,   4, 0x4,  /* AARBCTL0.IOSF0VC1_WGT */ \
    0x00, 0xc0,  11,   8, 0x4,  /* AARBCTL0.IOSF0VC2_WGT */ \
    0x00, 0xd0,   3,   0, 0x8,  /* ADNARBCTL.VC0_WGT */ \
    0x00, 0xd0,   7,   4, 0x4,  /* ADNARBCTL.VC1_WGT */ \
    0x00, 0xd0,  11,   8, 0x4,  /* ADNARBCTL.VC2_WGT */ \
    0x00, 0xd1,   3,   0, 0x4,  /* AVC0ARBCTL.BA_WGT */ \
    0x00, 0xd1,   7,   4, 0x4,  /* AVC0ARBCTL.TA_WGT */ \
/* end of AUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // AUNIT_BIOS_SETTINGS_HH
