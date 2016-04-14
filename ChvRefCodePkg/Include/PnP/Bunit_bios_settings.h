/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef BUNIT_BIOS_SETTINGS_HH
#define BUNIT_BIOS_SETTINGS_HH

#define BUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
    0x03, 0xb,   5,   0, 0x2f,  /* BALIMIT0.AGENT0_LIMIT */ \
    0x03, 0xb,  13,   8, 0x2f,  /* BALIMIT0.AGENT1_LIMIT */ \
    0x03, 0xb,  21,  16, 0x2f,  /* BALIMIT0.AGENT2_LIMIT */ \
    0x03, 0xb,  29,  24, 0x2f,  /* BALIMIT0.AGENT3_LIMIT */ \
    0x03, 0xc,   5,   0, 0x2f,  /* BALIMIT1.AGENT4_LIMIT */ \
    0x03, 0xc,  13,   8, 0x2f,  /* BALIMIT1.AGENT5_LIMIT */ \
    0x03, 0xc,  21,  16, 0x2f,  /* BALIMIT1.AGENT6_LIMIT */ \
    0x03, 0xc,  29,  24, 0x2f,  /* BALIMIT1.AGENT7_LIMIT */ \
    0x03, 0xd,   5,   0, 0x2f,  /* BALIMIT2.AGENT8_LIMIT */ \
    0x03, 0xd,  13,   8, 0x2f,  /* BALIMIT2.AGENT9_LIMIT */ \
    0x03, 0xd,  21,  16, 0x2f,  /* BALIMIT2.AGENT10_LIMIT */ \
    0x03, 0xd,  29,  24, 0x2f,  /* BALIMIT2.AGENT11_LIMIT */ \
    0x03, 0xe,   5,   0, 0x2f,  /* BALIMIT3.AGENT12_LIMIT */ \
    0x03, 0xe,  13,   8, 0x2f,  /* BALIMIT3.AGENT13_LIMIT */ \
    0x03, 0xe,  21,  16, 0x2f,  /* BALIMIT3.AGENT14_LIMIT */ \
    0x03, 0xe,  29,  24, 0x2f,  /* BALIMIT3.AGENT15_LIMIT */ \
    0x03, 0x12,  7,   6, 0x1,   /* BCOSCAT.COS_CAT_AGENT3 */ \
    0x03, 0x12,  9,   8, 0x1,   /* BCOSCAT.COS_CAT_AGENT4 */ \
    0x03, 0x12, 13,  12, 0x1,   /* BCOSCAT.COS_CAT_AGENT6 */ \
    0x03, 0x14,  5,   0, 0x10,  /* BFLWT.READ_WEIGHTS */ \
    0x03, 0x16,  5,   0, 0x10,  /* BISOCWT.NON_ISOC_REQUEST_WEIGHTS */ \
    0x03, 0x16, 13,   8, 0x3f,  /* BISOCWT.ISOC_REQUEST_WEIGHTS */ \
    0x03, 0x18,  6,   0, 0x20,  /* BSCHCTRL0.BE_OOO_SCH_LIMIT */ \
    0x03, 0x18, 23,  21, 0x6,   /* BSCHCTRL0.PAGE_HIT_DELAY */ \
    0x03, 0x18, 31,  24, 0x7f,  /* BSCHCTRL0.BEST_EFFORT_MAX_LATENCY */ \
    0x03, 0x3b, 23,  16, 0x4,   /* BDEBUG0.CASUAL_TIMER */ \
    0x03, 0x3c,  2,   2, 0x0,   /* BDEBUG1.EXIT_SR_FOR_CASUAL_FLUSH */ \
    0x03, 0x3c, 31,  16, 0x0,   /* BDEBUG1.AGENT_WEIGHT_ENABLE  */ \
    0x03, 0x3d,  0,   0, 0x1,   /* BCTRL.REQUESTCLK_GATE_EN */ \
    0x03, 0x3d,  1,   1, 0x1,   /* BCTRL.MASTERCLK_GATE_EN */ \
    0x03, 0x3d,  2,   2, 0x1,   /* BCTRL.BECLK_GATE_EN */ \
    0x03, 0x3d, 14,  14, 0x1,   /* BCTRL.BANK_STATUS_ENABLE */ \
    0x03, 0x7,  31,  24, 0x30,  /* BWFLUSH.FLUSH_THRSHOLD */ \
    0x03, 0x7,  15,   8, 0x10,  /* BWFLUSH.DIRTY_LWM */ \
    0x03, 0x7,   7,   0, 0x18,  /* BWFLUSH.DIRTY_HWM */ \

/* end of BUNIT_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define BUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    Offset, MSB, LSB, Value */  \
    0x03, 0xb,   5,   0, 0x2f,  /* BALIMIT0.AGENT0_LIMIT */ \
    0x03, 0xb,  13,   8, 0x2f,  /* BALIMIT0.AGENT1_LIMIT */ \
    0x03, 0xb,  21,  16, 0x2f,  /* BALIMIT0.AGENT2_LIMIT */ \
    0x03, 0xb,  29,  24, 0x2b,  /* BALIMIT0.AGENT3_LIMIT */ \
    0x03, 0xc,   5,   0, 0x2f,  /* BALIMIT1.AGENT4_LIMIT */ \
    0x03, 0xc,  13,   8, 0x2f,  /* BALIMIT1.AGENT5_LIMIT */ \
    0x03, 0xc,  21,  16, 0x2f,  /* BALIMIT1.AGENT6_LIMIT */ \
    0x03, 0xc,  29,  24, 0x2f,  /* BALIMIT1.AGENT7_LIMIT */ \
    0x03, 0xd,   5,   0, 0x2f,  /* BALIMIT2.AGENT8_LIMIT */ \
    0x03, 0xd,  13,   8, 0x2f,  /* BALIMIT2.AGENT9_LIMIT */ \
    0x03, 0xd,  21,  16, 0x2f,  /* BALIMIT2.AGENT10_LIMIT */ \
    0x03, 0xd,  29,  24, 0x2f,  /* BALIMIT2.AGENT11_LIMIT */ \
    0x03, 0xe,   5,   0, 0x2f,  /* BALIMIT3.AGENT12_LIMIT */ \
    0x03, 0xe,  13,   8, 0x2f,  /* BALIMIT3.AGENT13_LIMIT */ \
    0x03, 0xe,  21,  16, 0x2f,  /* BALIMIT3.AGENT14_LIMIT */ \
    0x03, 0xe,  29,  24, 0x2f,  /* BALIMIT3.AGENT15_LIMIT */ \
    0x03, 0x12,   7,   6, 0x1,  /* BCOSCAT.COS_CAT_AGENT3 */ \
    0x03, 0x12,   9,   8, 0x1,  /* BCOSCAT.COS_CAT_AGENT4 */ \
    0x03, 0x12,  13,  12, 0x1,  /* BCOSCAT.COS_CAT_AGENT6 */ \
    0x03, 0x14,   5,   0, 0x10,  /* BFLWT.READ_WEIGHTS */ \
    0x03, 0x16,  13,   8, 0x3f,  /* BISOCWT.ISOC_REQUEST_WEIGHTS */ \
    0x03, 0x18,   6,   0, 0x20,  /* BSCHCTRL0.BE_OOO_SCH_LIMIT */ \
    0x03, 0x18,  23,  21, 0x6,  /* BSCHCTRL0.PAGE_HIT_DELAY */ \
    0x03, 0x18,  31,  24, 0xff,  /* BSCHCTRL0.BEST_EFFORT_MAX_LATENCY */ \
    0x03, 0x3b,  23,  16, 0x4,  /* BDEBUG0.CASUAL_TIMER */ \
    0x03, 0x3c,   2,   2, 0x0,  /* BDEBUG1.EXIT_SR_FOR_CASUAL_FLUSH */ \
    0x03, 0x3d,   0,   0, 0x1,  /* BCTRL.REQUESTCLK_GATE_EN */ \
    0x03, 0x3d,   2,   2, 0x1,  /* BCTRL.BECLK_GATE_EN */ \
    0x03, 0x3d,  14,  14, 0x1,  /* BCTRL.BANK_STATUS_ENABLE */ \
    0x03, 0x3e,  31,  16, 0xffff,  /* BTHCTRL.AGENT_THROTTLING_ENABLE */ \
    0x03, 0x3f,   7,   0, 0xaa,  /* BTHMASK.READ_MASK */ \
    0x03, 0x3f,  15,   8, 0xaa,  /* BTHMASK.WRITE_MASK */ \
/* end of BUNIT_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define BUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    Offset, MSB, LSB, Value */  \
    0x03, 0x7,   7,   0, 0x18, \
    0x03, 0x7,  15,   8, 0x10, \
    0x03, 0x7,  31,  24, 0x30, \
    0x03, 0xb,   5,   0, 0x2f,  /* BALIMIT0.AGENT0_LIMIT */ \
    0x03, 0xb,  13,   8, 0x2f,  /* BALIMIT0.AGENT1_LIMIT */ \
    0x03, 0xb,  21,  16, 0x2f,  /* BALIMIT0.AGENT2_LIMIT */ \
    0x03, 0xb,  29,  24, 0x2f,  /* BALIMIT0.AGENT3_LIMIT */ \
    0x03, 0xc,   5,   0, 0x2f,  /* BALIMIT1.AGENT4_LIMIT */ \
    0x03, 0xc,  13,   8, 0x2f,  /* BALIMIT1.AGENT5_LIMIT */ \
    0x03, 0xc,  21,  16, 0x2f,  /* BALIMIT1.AGENT6_LIMIT */ \
    0x03, 0xc,  29,  24, 0x2f,  /* BALIMIT1.AGENT7_LIMIT */ \
    0x03, 0xd,   5,   0, 0x2f,  /* BALIMIT2.AGENT8_LIMIT */ \
    0x03, 0xd,  13,   8, 0x2f,  /* BALIMIT2.AGENT9_LIMIT */ \
    0x03, 0xd,  21,  16, 0x2f,  /* BALIMIT2.AGENT10_LIMIT */ \
    0x03, 0xd,  29,  24, 0x2f,  /* BALIMIT2.AGENT11_LIMIT */ \
    0x03, 0xe,   5,   0, 0x2f,  /* BALIMIT3.AGENT12_LIMIT */ \
    0x03, 0xe,  13,   8, 0x2f,  /* BALIMIT3.AGENT13_LIMIT */ \
    0x03, 0xe,  21,  16, 0x2f,  /* BALIMIT3.AGENT14_LIMIT */ \
    0x03, 0xe,  29,  24, 0x2f,  /* BALIMIT3.AGENT15_LIMIT */ \
    0x03, 0x12,  7,   6, 0x1,   /* BCOSCAT.COS_CAT_AGENT3 */ \
    0x03, 0x12,  9,   8, 0x1,   /* BCOSCAT.COS_CAT_AGENT4 */ \
    0x03, 0x12, 13,  12, 0x1,   /* BCOSCAT.COS_CAT_AGENT6 */ \
    0x03, 0x14,  5,   0, 0x10,  /* BFLWT.READ_WEIGHTS */ \
    0x03, 0x16,  5,   0, 0x10,  /* BISOCWT.NON_ISOC_REQUEST_WEIGHTS */ \
    0x03, 0x16, 13,   8, 0x3f,  /* BISOCWT.ISOC_REQUEST_WEIGHTS */ \
    0x03, 0x18,  6,   0, 0x20,  /* BSCHCTRL0.BE_OOO_SCH_LIMIT */ \
    0x03, 0x18, 23,  21, 0x6,   /* BSCHCTRL0.PAGE_HIT_DELAY */ \
    0x03, 0x18, 31,  24, 0x7f,  /* BSCHCTRL0.BEST_EFFORT_MAX_LATENCY */ \
    0x03, 0x3b,  7,   0, 0x1, \
    0x03, 0x3b, 23,  16, 0x4,   /* BDEBUG0.CASUAL_TIMER */ \
    0x03, 0x3b, 27,  25, 0x1, \
    0x03, 0x3c,  2,   2, 0x0,   /* BDEBUG1.EXIT_SR_FOR_CASUAL_FLUSH */ \
    0x03, 0x3c, 31,  16, 0x0,   /* BDEBUG1.AGENT_WEIGHT_ENABLE  */ \
    0x03, 0x3d,  0,   0, 0x1,   /* BCTRL.REQUESTCLK_GATE_EN */ \
    0x03, 0x3d,  1,   1, 0x1,   /* BCTRL.MASTERCLK_GATE_EN */ \
    0x03, 0x3d,  2,   2, 0x1,   /* BCTRL.BECLK_GATE_EN */ \
    0x03, 0x3d,  8,   8, 0x0, \
    0x03, 0x3d, 14,  14, 0x1,   /* BCTRL.BANK_STATUS_ENABLE */ \
/* end of BUNIT_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // BUNIT_BIOS_SETTINGS_HH
