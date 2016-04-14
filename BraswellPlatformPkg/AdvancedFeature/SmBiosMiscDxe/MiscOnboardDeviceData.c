/** @file
  Static data of Onboard device information .
  The onboard device information is Misc for subclass type 8, SMBIOS type 10.

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
MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE10, MiscOnBoardDeviceVideo) = {
  {                                               // Hdr
    EFI_SMBIOS_TYPE_ONBOARD_DEVICE_INFORMATION,   ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE10),                  ///< Hdr.Length
    0                                             ///< Hdr.Handle
  },
  {                                               // Device
    OnBoardDeviceTypeVideo,                       ///< DeviceType
    SMBIOS_MISC_STRING_1                          ///< DescriptionString
  }
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE10, MiscOnBoardDeviceEthernet) = {
  {                                               // Hdr
    EFI_SMBIOS_TYPE_ONBOARD_DEVICE_INFORMATION,   ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE10),                  ///< Hdr.Length
    0                                             ///< Hdr.Handle
  },
  {                                               // Device
    OnBoardDeviceTypeEthernet,                    ///< DeviceType
    SMBIOS_MISC_STRING_1                          ///< DescriptionString
  }
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE10, MiscOnBoardDeviceSound) = {
  {                                               // Hdr
    EFI_SMBIOS_TYPE_ONBOARD_DEVICE_INFORMATION,   ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE10),                  ///< Hdr.Length
    0                                             ///< Hdr.Handle
  },
  {                                               // Device
    OnBoardDeviceTypeSound,                       ///< DeviceType
    SMBIOS_MISC_STRING_1                          ///< DescriptionString
  }
};
