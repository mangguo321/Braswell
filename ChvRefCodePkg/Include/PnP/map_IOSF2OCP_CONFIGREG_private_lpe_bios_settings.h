/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_BIOS_SETTINGS_HH
#define MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_BIOS_SETTINGS_HH

#define MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x58, 0x68,   7,   4, 0x0,  /* OCPCTL.CMDDEPTH */ \
/* end of MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x58, 0x10,   7,   4, 0xF,  /* OCPCTL.CMDDEPTH */ \
/* end of MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // MAP_IOSF2OCP_CONFIGREG_PRIVATE_LPE_BIOS_SETTINGS_HH
