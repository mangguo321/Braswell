/** @file
  Create the mMiscSubclassDataTable structure, and it is used to report any generate data to the DataHub.

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
// External definitions referenced by Data Table entries.
//
MISC_SMBIOS_TABLE_EXTERNS(EFI_MISC_BIOS_VENDOR_DATA, MiscBiosVendor, MiscBiosVendor);
MISC_SMBIOS_TABLE_EXTERNS(EFI_MISC_SYSTEM_MANUFACTURER_DATA, MiscSystemManufacturer, MiscSystemManufacturer);
MISC_SMBIOS_TABLE_EXTERNS(EFI_MISC_BASE_BOARD_MANUFACTURER_DATA, MiscBaseBoardManufacturer, MiscBaseBoardManufacturer);
MISC_SMBIOS_TABLE_EXTERNS(EFI_MISC_CHASSIS_MANUFACTURER_DATA, MiscChassisManufacturer, MiscChassisManufacturer);
MISC_SMBIOS_TABLE_EXTERNS(EFI_CACHE_VARIABLE_RECORD, MiscProcessorCache, MiscProcessorCache); //type 7
MISC_SMBIOS_TABLE_EXTERNS(EFI_CPU_DATA_RECORD, MiscProcessorInformation, MiscProcessorInformation); //type 4
MISC_SMBIOS_TABLE_EXTERNS(EFI_MEMORY_ARRAY_LOCATION_DATA, MiscPhysicalMemoryArray,MiscPhysicalMemoryArray);
MISC_SMBIOS_TABLE_EXTERNS(EFI_MEMORY_ARRAY_LINK_DATA, MiscMemoryDevice, MiscMemoryDevice);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortKeyboard, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortMouse, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortCom1, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortVideo, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortHDMI, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortUsb201, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortUsb202, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortNetwork, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortSata0, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortSata1, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortACIN, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortPORT80, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPort2X8HEADER, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPort8PINHEADER, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortHDAHDMI, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortMKEYBOARD, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortSPI, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortLPCDOCKING, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortSIDEBAND, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortLPCSLOT, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortSATAPOWER, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortFPHEADER, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortATXPOWER, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortBATTB, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortBATTA, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortCPUFAN, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortXDP, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortMEMORY1, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortMEMORY2, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE8, MiscPortFANPWR, MiscPortInternalConnectorDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie0, MiscSystemSlotDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie1, MiscSystemSlotDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie2, MiscSystemSlotDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE9, MiscSystemSlotPcie3, MiscSystemSlotDesignator);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE10, MiscOnBoardDeviceVideo, MiscOnBoardDevice);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE10, MiscOnBoardDeviceEthernet, MiscOnBoardDevice);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE10, MiscOnBoardDeviceSound, MiscOnBoardDevice);
MISC_SMBIOS_TABLE_EXTERNS(EFI_MISC_NUMBER_OF_INSTALLABLE_LANGUAGES_DATA, NumberOfInstallableLanguages, NumberOfInstallableLanguages);
MISC_SMBIOS_TABLE_EXTERNS(EFI_MISC_SYSTEM_LANGUAGE_STRING_DATA, SystemLanguageString, SystemLanguageString);
MISC_SMBIOS_TABLE_EXTERNS(SMBIOS_TABLE_TYPE32, MiscBootInfoStatus, MiscBootInfoStatus);

//
// Data Table.
//
EFI_MISC_SMBIOS_DATA_TABLE  mMiscSubclassDataTable[] = {
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscBiosVendor, MiscBiosVendor),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscSystemManufacturer, MiscSystemManufacturer),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscBaseBoardManufacturer, MiscBaseBoardManufacturer),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscChassisManufacturer, MiscChassisManufacturer),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortKeyboard, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortMouse, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortCom1, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortVideo, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortHDMI, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortUsb201, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortUsb202, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortNetwork, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortSata0, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortSata1, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortACIN, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortPORT80, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPort2X8HEADER, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPort8PINHEADER, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortHDAHDMI, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortMKEYBOARD, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortSPI, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortLPCDOCKING, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortSIDEBAND, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortLPCSLOT, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortSATAPOWER, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortFPHEADER, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortATXPOWER, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortBATTB, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortBATTA, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortCPUFAN, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortXDP, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortMEMORY1, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortMEMORY2, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscPortFANPWR, MiscPortInternalConnectorDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscSystemSlotPcie0, MiscSystemSlotDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscSystemSlotPcie1, MiscSystemSlotDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscSystemSlotPcie2, MiscSystemSlotDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscSystemSlotPcie3, MiscSystemSlotDesignator),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscOnBoardDeviceEthernet, MiscOnBoardDevice),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscOnBoardDeviceSound, MiscOnBoardDevice),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscOnBoardDeviceVideo, MiscOnBoardDevice),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(SystemLanguageString, SystemLanguageString),
  MISC_SMBIOS_TABLE_ENTRY_DATA_AND_FUNCTION(MiscBootInfoStatus, MiscBootInfoStatus),
};

//
// Number of Data Table entries.
//
UINTN mMiscSubclassDataTableEntries =
        (sizeof mMiscSubclassDataTable) / sizeof(EFI_MISC_SMBIOS_DATA_TABLE);
