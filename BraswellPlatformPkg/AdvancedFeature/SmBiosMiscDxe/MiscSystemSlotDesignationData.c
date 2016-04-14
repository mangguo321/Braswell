/** @file
  Static data of System Slot Designation.
  System Slot Designation is Misc for subclass type 7, SMBIOS type 9.

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

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie0) = {
  {                                             // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,               ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE9),                 ///< Hdr.Length
    0                                           ///< Hdr.Handle
  },
  SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE0,          ///< SlotDesignation
  SlotTypePciExpressX16,                        ///< SlotType
  SlotDataBusWidth16X,                          ///< SlotDataBusWidth
  SlotUsageAvailable,                           ///< SlotUsage
  SlotLengthLong,                               ///< SlotLength
  0,                                            ///< SlotId
  {                                             // SlotCharacteristics1
    0,                                          ///< Bit0 CharacteristicsUnknown   :1
    0,                                          ///< Bit1 Provides50Volts          :1
    1,                                          ///< Bit2 Provides33Volts          :1
    0,                                          ///< Bit3 SharedSlot               :1
    0,                                          ///< Bit4 PcCard16Supported        :1
    0,                                          ///< Bit5 CardBusSupported         :1
    0,                                          ///< Bit6 ZoomVideoSupported       :1
    0,                                          ///< Bit7 ModemRingResumeSupported :1
  },
  {                                             // SlotCharacteristics2
    1,                                          ///< Bit0 PmeSignalSupported       :1
    0,                                          ///< Bit1 HotPlugDevicesSupported  :1
    1,                                          ///< Bit2 SmbusSignalSupported     :1
    0                                           ///< Bit3-7 Reserved               :5
  },
  0,                                            ///< SegmentGroupNum
  PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,        ///< BusNum
  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1      ///< DevFuncNum
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie1) = {
  {                                             // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,               ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE9),                 ///< Hdr.Length
    0                                           ///< Hdr.Handle
  },
  SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE1,          ///< SlotDesignation
  SlotTypePciExpress,                           ///< SlotType
  SlotDataBusWidth1X,                           ///< SlotDataBusWidth
  SlotUsageAvailable,                           ///< SlotUsage
  SlotLengthLong,                               ///< SlotLength
  1,                                            ///< SlotId
  {                                             // SlotCharacteristics1
    0,                                          ///< Bit0 CharacteristicsUnknown   :1
    0,                                          ///< Bit1 Provides50Volts          :1
    1,                                          ///< Bit2 Provides33Volts          :1
    0,                                          ///< Bit3 SharedSlot               :1
    0,                                          ///< Bit4 PcCard16Supported        :1
    0,                                          ///< Bit5 CardBusSupported         :1
    0,                                          ///< Bit6 ZoomVideoSupported       :1
    0,                                          ///< Bit7 ModemRingResumeSupported :1
  },
  {                                             // SlotCharacteristics2
    1,                                          ///< Bit0 PmeSignalSupported       :1
    0,                                          ///< Bit1 HotPlugDevicesSupported  :1
    1,                                          ///< Bit2 SmbusSignalSupported     :1
    0                                           ///< Bit3-7 Reserved               :5
  },
  0,                                            ///< SegmentGroupNum
  PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,        ///< BusNum
  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2      ///< DevFuncNum
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie2) = {
  {                                             // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,               ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE9),                 ///< Hdr.Length
    0                                           ///< Hdr.Handle
  },
  SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE2,          ///< SlotDesignation
  SlotTypePciExpress,                           ///< SlotType
  SlotDataBusWidth1X,                           ///< SlotDataBusWidth
  SlotUsageAvailable,                           ///< SlotUsage
  SlotLengthLong,                               ///< SlotLength
  2,                                            ///< SlotId
  {                                             // SlotCharacteristics1
    0,                                          ///< Bit0 CharacteristicsUnknown   :1
    0,                                          ///< Bit1 Provides50Volts          :1
    1,                                          ///< Bit2 Provides33Volts          :1
    0,                                          ///< Bit3 SharedSlot               :1
    0,                                          ///< Bit4 PcCard16Supported        :1
    0,                                          ///< Bit5 CardBusSupported         :1
    0,                                          ///< Bit6 ZoomVideoSupported       :1
    0,                                          ///< Bit7 ModemRingResumeSupported :1
  },
  {                                             // SlotCharacteristics2
    1,                                          ///< Bit0 PmeSignalSupported       :1
    0,                                          ///< Bit1 HotPlugDevicesSupported  :1
    1,                                          ///< Bit2 SmbusSignalSupported     :1
    0                                           ///< Bit3-7 Reserved               :5
  },
  0,                                            ///< SegmentGroupNum
  PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,        ///< BusNum
  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3      ///< DevFuncNum
};

MISC_SMBIOS_TABLE_DATA(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie3) = {
  {                                             // Hdr
    EFI_SMBIOS_TYPE_SYSTEM_SLOTS,               ///< Hdr.Type
    sizeof(SMBIOS_TABLE_TYPE9),                 ///< Hdr.Length
    0                                           ///< Hdr.Handle
  },
  SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE3,          ///< SlotDesignation
  SlotTypePciExpress,                           ///< SlotType
  SlotDataBusWidth1X,                           ///< SlotDataBusWidth
  SlotUsageAvailable,                           ///< SlotUsage
  SlotLengthLong,                               ///< SlotLength
  3,                                            ///< SlotId
  {                                             // SlotCharacteristics1
    0,                                          ///< Bit0 CharacteristicsUnknown   :1
    0,                                          ///< Bit1 Provides50Volts          :1
    1,                                          ///< Bit2 Provides33Volts          :1
    0,                                          ///< Bit3 SharedSlot               :1
    0,                                          ///< Bit4 PcCard16Supported        :1
    0,                                          ///< Bit5 CardBusSupported         :1
    0,                                          ///< Bit6 ZoomVideoSupported       :1
    0,                                          ///< Bit7 ModemRingResumeSupported :1
  },
  {                                             // SlotCharacteristics2
    1,                                          ///< Bit0 PmeSignalSupported       :1
    0,                                          ///< Bit1 HotPlugDevicesSupported  :1
    1,                                          ///< Bit2 SmbusSignalSupported     :1
    0                                           ///< Bit3-7 Reserved               :5
  },
  0,                                            ///< SegmentGroupNum
  PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,        ///< BusNum
  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4      ///< DevFuncNum
};

//
// PCIESC:RestrictedEnd
//
