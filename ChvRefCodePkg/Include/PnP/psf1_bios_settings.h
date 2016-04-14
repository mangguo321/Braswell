/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef PSF1_BIOS_SETTINGS_HH
#define PSF1_BIOS_SETTINGS_HH

#define PSF1_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x46, 0xc000,   0,   0, 0x0,  /* POWER_OPTIONS.CLKGATE_DISABLE */ \
    0x46, 0xc000,   1,   1, 0x1,  /* POWER_OPTIONS.DISABLE_NP_TRACK */ \
/* end of PSF1_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define PSF1_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x46, 0xc000,   0,   0, 0x0,  /* POWER_OPTIONS.CLKGATE_DISABLE */ \
    0x46, 0xc000,   1,   1, 0x1,  /* POWER_OPTIONS.DISABLE_NP_TRACK */ \
/* end of PSF1_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // PSF1_BIOS_SETTINGS_HH
