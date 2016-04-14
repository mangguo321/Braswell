/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef GMM_SBPR_BIOS_SETTINGS_HH
#define GMM_SBPR_BIOS_SETTINGS_HH

#define GMM_SBPR_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xb4, 0xb4,   3,   0, 0xf,  /* GMMSLVCTL.MAXOTC */ \
/* end of GMM_SBPR_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define GMM_SBPR_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0xb4, 0xb4,   3,   0, 0x0,  /* GMMSLVCTL.MAXOTC */ \
/* end of GMM_SBPR_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#endif // GMM_SBPR_BIOS_SETTINGS_HH
