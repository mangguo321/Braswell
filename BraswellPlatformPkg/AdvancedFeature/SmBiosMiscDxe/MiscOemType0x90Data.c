/** @file
  This file contains the Misc Oem Data (SMBIOS data type 0x90)

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "MiscSubclassDriver.h"

//
// Static (possibly build generated) Oem data.
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE90, MiscOemType0x90) = {
	{                                             // Hdr
    EFI_SMBIOS_TYPE_FIRMWARE_VERSION_INFO,      ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE90),                ///< Hdr.Length
    0                                           ///< Hdr.Handle
  },
  SMBIOS_MISC_STRING_1,                         // SECVersion
  SMBIOS_MISC_STRING_2,                         // uCodeVersion
  SMBIOS_MISC_STRING_3,                         // GOPVersion
  SMBIOS_MISC_STRING_4                          // CpuStepping
};
