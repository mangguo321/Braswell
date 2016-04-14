/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef PCU_B0_D31_F3_BIOS_SETTINGS_HH
#define PCU_B0_D31_F3_BIOS_SETTINGS_HH

#define PCU_B0_D31_F3_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x54, 0x80,   8,   8, 0x1,  /* SMB_CONFIG_SMBSM.PHDCGDIS */ \
    0x54, 0x80,  10,  10, 0x1,  /* SMB_CONFIG_SMBSM.LNDCGDIS */ \
    0x54, 0x80,  12,  12, 0x1,  /* SMB_CONFIG_SMBSM.TRDCGDIS */ \
    0x54, 0x80,  14,  14, 0x1,  /* SMB_CONFIG_SMBSM.BBDCGDIS */ \
    0x54, 0xfc,  16,  16, 0x1,  /* SMB_CONFIG_CGC.TR_CLK_FORCE_REQ */ \
/* end of PCU_B0_D31_F3_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define PCU_B0_D31_F3_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x54, 0x54,   1,   0, 0x3,  /* SMB_CONFIG_PMCSR.PS */ \
    0x54, 0xfc,   9,   9, 0x0,  /* SMB_CONFIG_CGC.SB_LOCAL_CGD */ \
    0x54, 0xfc,  17,  17, 0x0,  /* SMB_CONFIG_CGC.FUNC_CLK_CGD */ \
/* end of PCU_B0_D31_F3_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define PCU_B0_D31_F3_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x54, 0xfc,   9,   9, 0x0,  /* SMB_CONFIG_CGC.SB_LOCAL_CGD */ \
    0x54, 0xfc,  17,  17, 0x0,  /* SMB_CONFIG_CGC.FUNC_CLK_CGD */ \
/* end of PCU_B0_D31_F3_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // PCU_B0_D31_F3_BIOS_SETTINGS_HH
