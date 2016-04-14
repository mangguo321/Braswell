/** @file
  This driver will determine memory configuration information from the chipset
  and memory and create SMBIOS memory structures appropriately.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SmbiosMemory.h"

CHAR8 *DimmToDevLocator[] = {
  "ChannelA-DIMM0",
  "ChannelB-DIMM0"
};

CHAR8 *DimmToBankLocator[] = {
  "BANK 0",
  "BANK 1"
};

MEMORY_MODULE_MANUFACTURE_LIST MemoryModuleManufactureList[] = {
  {0,    0x2c, "Micron"},
  {0,    0xad, "Hynix/Hyundai"},
  {0,    0xce, "Samsung"},
  {1,    0x4f, "Transcend"},
  {1,    0x98, "Kingston"},
  {2,    0xfe, "Elpida"},
  {0xff, 0xff, 0}
};
