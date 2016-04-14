/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef SB_BIOS_SETTINGS_HH
#define SB_BIOS_SETTINGS_HH

#define SB_VALUEFORPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x0a, 0x60,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_0.CURRENT_VID */ \
    0x0a, 0x61,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_1.CURRENT_VID */ \
    0x0a, 0x64,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_4.CURRENT_VID */ \
    0x0a, 0x65,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_5.CURRENT_VID */ \
/* end of SB_VALUEFORPERF_VALUES_PLATFORM_DEFAULT */

#define SB_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x0a, 0x60,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_0.CURRENT_VID */ \
    0x0a, 0x61,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_1.CURRENT_VID */ \
    0x0a, 0x64,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_4.CURRENT_VID */ \
    0x0a, 0x65,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_5.CURRENT_VID */ \
/* end of SB_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#define SB_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x0a, 0x60,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_0.CURRENT_VID */ \
    0x0a, 0x61,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_1.CURRENT_VID */ \
    0x0a, 0x64,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_4.CURRENT_VID */ \
    0x0a, 0x65,  31,  24, 0x7f,  /* RAIL_CONFIGURATION_AND_STATUS_5.CURRENT_VID */ \
/* end of SB_VALUEFORPWRPERF_VALUES_PLATFORM_DEFAULT */

#endif // SB_BIOS_SETTINGS_HH
