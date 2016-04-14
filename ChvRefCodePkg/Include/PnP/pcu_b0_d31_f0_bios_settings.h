/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef PCU_B0_D31_F0_BIOS_SETTINGS_HH
#define PCU_B0_D31_F0_BIOS_SETTINGS_HH

#define PCU_B0_D31_F0_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x54, 0x8c,   8,   8, 0x0,  /* CGC.CGE */ \
    0x54, 0x8c,   9,   9, 0x0,  /* CGC.CGD */ \
    0x54, 0x90,  26,  24, 0x4,  /* SOFTWARE_SEQUENCING_FLASH_CONTROL_STATUS_BIOS.SCF */ \
    0x54, 0xfc,   3,   2, 0x2,  /* BIOS_CONTROL_REGISTER_BIOS.SRC */ \
    0x54, 0x100,   9,   9, 0x1,  /* TRUNK_CLOCK_GATING_CONTROL_BIOS.SBCGCDEF */ \
    0x54, 0x100,  11,  11, 0x1,  /* TRUNK_CLOCK_GATING_CONTROL_BIOS.FCREPDIS */ \
    0x54, 0x10e1,  26,  24, 0x4,  /* SOFTWARE_SEQUENCING_FLASH_CONTROL_STATUS_SEC.SCF */ \
    0x54, 0xe1,  26,  24, 0x4,  /* SOFTWARE_SEQUENCING_FLASH_CONTROL_STATUS_GBE.SCF */ \
/* end of PCU_B0_D31_F0_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define PCU_B0_D31_F0_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x54, 0x1c,   8,   8, 0x0,  /* BC.PFE */ \
    0x54, 0x84,   2,   2, 0x1,  /* LPCC.CLKRUN_EN */ \
    0x54, 0x8c,   9,   9, 0x0,  /* CGC.CGD */ \
    0x54, 0xc0,   0,   0, 0x1,  /* ADDITIONAL_FLASH_CONTROL_BIOS.FCDCGE */ \
    0x54, 0xc0,   1,   1, 0x1,  /* ADDITIONAL_FLASH_CONTROL_BIOS.FMDCGE */ \
    0x54, 0xc0,   2,   2, 0x1,  /* ADDITIONAL_FLASH_CONTROL_BIOS.FSDCGE */ \
    0x54, 0x100,  10,  10, 0x0,  /* TRUNK_CLOCK_GATING_CONTROL_BIOS.FCGDIS */ \
    0x54, 0x114d,   1,   1, 0x0,  /* SEC_FLASH_CONTROL_SEC.SECDRPE */ \
/* end of PCU_B0_D31_F0_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define PCU_B0_D31_F0_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x54, 0x1c,   8,   8, 0x0,  /* BC.PFE */ \
    0x54, 0x8c,   8,   8, 0x0,  /* CGC.CGE */ \
    0x54, 0x8c,   9,   9, 0x0,  /* CGC.CGD */ \
    0x54, 0x90,  26,  24, 0x4,  /* SOFTWARE_SEQUENCING_FLASH_CONTROL_STATUS_BIOS.SCF */ \
    0x54, 0xc0,   0,   0, 0x1,  /* ADDITIONAL_FLASH_CONTROL_BIOS.FCDCGE */ \
    0x54, 0xc0,   1,   1, 0x1,  /* ADDITIONAL_FLASH_CONTROL_BIOS.FMDCGE */ \
    0x54, 0xc0,   2,   2, 0x1,  /* ADDITIONAL_FLASH_CONTROL_BIOS.FSDCGE */ \
    0x54, 0x100,  10,  10, 0x0,  /* TRUNK_CLOCK_GATING_CONTROL_BIOS.FCGDIS */ \
    0x54, 0x10e1,  26,  24, 0x4,  /* SOFTWARE_SEQUENCING_FLASH_CONTROL_STATUS_SEC.SCF */ \
    0x54, 0xe1,  26,  24, 0x4,  /* SOFTWARE_SEQUENCING_FLASH_CONTROL_STATUS_GBE.SCF */ \
/* end of PCU_B0_D31_F0_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // PCU_B0_D31_F0_BIOS_SETTINGS_HH
