/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef MAP_IOSF2OCP_CONFIGREG_PCI_BIOS_SETTINGS_HH
#define MAP_IOSF2OCP_CONFIGREG_PCI_BIOS_SETTINGS_HH

#define MAP_IOSF2OCP_CONFIGREG_PCI_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT \
/*  MsgPort,    MsgRegAddr, MSB, LSB, Value */  \
    0x58, 0x84,   1,   0, 0x3,  /* PMECTRLSTATUS.POWERSTATE */ \
/* end of MAP_IOSF2OCP_CONFIGREG_PCI_VALUEFORPOWER_VALUES_PLATFORM_DEFAULT */

#endif // MAP_IOSF2OCP_CONFIGREG_PCI_BIOS_SETTINGS_HH
