/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef HDA_REGS_BIOS_SETTINGS_HH
#define HDA_REGS_BIOS_SETTINGS_HH

#define HDA_REGS_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa7, 0x43,   0,   0, 0x0,  /* TM1.ODCGEN */ \
    0xa7, 0x43,   1,   1, 0x0,  /* TM1.IDCGEN */ \
    0xa7, 0x43,   2,   2, 0x0,  /* TM1.MDCGEN */ \
    0xa7, 0x43,   4,   4, 0x0,  /* TM1.BCSS */ \
    0xa7, 0x43,   6,   6, 0x0,  /* TM1.ACCD */ \
    0xa7, 0x120,  31,  31, 0x1,  /* VCICTL.VCIEN */ \
/* end of HDA_REGS_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define HDA_REGS_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa7, 0x78,  11,  11, 0x0,  /* DEVC.NSNPEN */ \
/* end of HDA_REGS_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define HDA_REGS_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xa7, 0x120,  31,  31, 0x1,  /* VCICTL.VCIEN */ \
/* end of HDA_REGS_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // HDA_REGS_BIOS_SETTINGS_HH
