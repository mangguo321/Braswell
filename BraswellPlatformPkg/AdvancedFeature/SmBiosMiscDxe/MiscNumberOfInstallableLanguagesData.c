/** @file
  Static data of the Number of installable languages information.
  Number of installable languages information is Misc for subclass type 11, SMBIOS type 13.

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
// Static (possibly build generated) Bios Vendor data.
//
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE13, NumberOfInstallableLanguages) = {
  {                                                     // Hdr
    EFI_SMBIOS_TYPE_BIOS_LANGUAGE_INFORMATION,          ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE13),                        ///< Hdr.Length
    0                                                   ///< Hdr.Handle
  },
  1,                                                    // NumberOfInstallableLanguages
  1,                                                    // LanguageFlags
  {                                                     // Reserve[15]
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  },
  SMBIOS_MISC_STRING_1,                                 // CurrentLanguage
};
