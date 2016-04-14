/** @file
  Static data of Boot information. Boot information is Misc for subclass type 26, SMBIOS type 32.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

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
// Static (possibly build generated) Bios Vendor data. SMBIOS TYPE 32
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE32, MiscBootInfoStatus) = {
  {                                           // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION,  ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE32),              ///< Hdr.Length
    0                                         ///< Hdr.Handle
  },
  {                                           // Reserved[6]
    0,
    0,
    0,
    0,
    0,
    0
  },
  BootInformationStatusNoError                ///< BootStatus
};
