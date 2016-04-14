/** @file
  This driver will determine the default string settting for SMBIOS tables.

  @par Revision Reference:
  SMBIOS Specification version 2.8.0 from DMTF: http://www.dmtf.org/standards/smbios
  Intel Framework Specifications, all available at: http://www.intel.com/technology/framework/spec.htm
   - Data Hub Specification
   - SMBUS Host Controller Protocol Specification
   - Human Interface Infrastructure Specification
  Unified Extensible Firmware Interface (UEFI) Specifications: http://www.uefi.org/specs/

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SMBIOS_MISC_STRINGS_H_
#define _SMBIOS_MISC_STRINGS_H_
///
/// String references in SMBIOS tables. This eliminates the need for pointers.
/// See the DMTF SMBIOS Specification v2.7.1, section 6.1.3.
///
#define SMBIOS_MISC_STRING_NULL                     0
#define SMBIOS_MISC_STRING_1                        1
#define SMBIOS_MISC_STRING_2                        2
#define SMBIOS_MISC_STRING_3                        3
#define SMBIOS_MISC_STRING_4                        4
#define SMBIOS_MISC_STRING_5                        5
#define SMBIOS_MISC_STRING_6                        6
#define SMBIOS_MISC_STRING_7                        7
#define SMBIOS_MISC_STRING_8                        8
#define SMBIOS_MISC_STRING_9                        9
#define SMBIOS_MISC_STRING_10                       10
#define SMBIOS_MISC_STRING_11                       11
#define SMBIOS_MISC_STRING_12                       12
#define SMBIOS_MISC_STRING_13                       13
#define SMBIOS_MISC_STRING_14                       14
#define SMBIOS_MISC_STRING_15                       15
#define SMBIOS_MISC_STRING_16                       16
#define SMBIOS_MISC_STRING_17                       17
#define SMBIOS_MISC_STRING_18                       18
#define SMBIOS_MISC_STRING_19                       19
#define SMBIOS_MISC_STRING_20                       20
#define SMBIOS_MISC_STRING_21                       21
#define SMBIOS_MISC_STRING_22                       22

///
/// Non-static SMBIOS table data to be filled later with a dynamically generated value
///
#define TO_BE_FILLED                                0
#define TO_BE_FILLED_STRING                         " "        ///< Initial value should not be NULL
#define TO_BE_FILLED_BY_OEM_STR                     L"To Be Filled by O.E.M."

///
/// SMBIOS_TABLE_TYPE00 Default Strings
/// 
///
#define SMBIOS_MISC_TYPE00_VENDOR                   L"Intel Corporation"
#define SMBIOS_MISC_TYPE00_BIOS_VERSION             L"Cherryview Platform BIOS"
#define SMBIOS_MISC_TYPE00_BIOS_RELEASE_DATE        L"12/01/2015"

typedef struct {
  CHAR16 *Vendor;
  CHAR16 *BiosVersion;
  CHAR16 *BiosReleaseDate;
} SMBIOS_TABLE_TYPE00_STRINGS;

///
/// SMBIOS_TABLE_TYPE01 Default Strings
///
#define SMBIOS_MISC_TYPE01_MANUFACTURER             L"Intel Corporation"
#define SMBIOS_MISC_TYPE01_PRODUCT_NAME             L"CHERRYVIEW Platform"
#define SMBIOS_MISC_TYPE01_VERSION                  L"0.1"
#define SMBIOS_MISC_TYPE01_SERIAL_NUMBER            L"112233445566"
#define SMBIOS_MISC_TYPE01_SKU_NUMBER               L"System SKUNumber"
#define SMBIOS_MISC_TYPE01_FAMILY                   L"Cherryview System"

typedef struct {
  CHAR16 *Manufacturer;
  CHAR16 *ProductName;
  CHAR16 *Version;
  CHAR16 *SerialNumber;
  CHAR16 *SkuNumber;
  CHAR16 *Family;
} SMBIOS_TABLE_TYPE01_STRINGS;

///
/// SMBIOS_TABLE_TYPE02 Default Strings
///
#define SMBIOS_MISC_TYPE02_BOARD_MANUFACTURER       L"Intel Corp."
#define SMBIOS_MISC_TYPE02_BOARD_PRODUCT_NAME       L"Braswell RVP"
#define SMBIOS_MISC_TYPE02_BOARD_VERSION            L"FAB"
#define SMBIOS_MISC_TYPE02_BOARD_SERIAL_NUMBER      L"1"
#define SMBIOS_MISC_TYPE02_BOARD_ASSET_TAG          L"Base Board Asset Tag"
#define SMBIOS_MISC_TYPE02_BOARD_CHASSIS_LOCATION   L"Part Component"

typedef struct {
  CHAR16 *Manufacturer;
  CHAR16 *ProductName;
  CHAR16 *Version;
  CHAR16 *SerialNumber;
  CHAR16 *AssetTag;
  CHAR16 *LocationInChassis;
} SMBIOS_TABLE_TYPE02_STRINGS;

///
/// SMBIOS_TABLE_TYPE03 Default Strings
///
#define SMBIOS_MISC_CHASSIS_MANUFACTURER            L"Intel Corporation"
#define SMBIOS_MISC_CHASSIS_VERSION                 L"0.1"
#define SMBIOS_MISC_CHASSIS_SERIAL_NUMBER           L"serial#"
#define SMBIOS_MISC_CHASSIS_ASSET_TAG               L"Asset Tag"

typedef struct {
  CHAR16 *Manufacturer;
  CHAR16 *Version;
  CHAR16 *SerialNumber;
  CHAR16 *AssetTag;
} SMBIOS_TABLE_TYPE03_STRINGS;

///
/// SMBIOS_TABLE_TYPE08 Default Strings
///
#define SMBIOS_MISC_PORT_TOKEN_KEYBOARD             0x01
#define SMBIOS_MISC_PORT_INTERNAL_KEYBOARD          L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_KEYBOARD          L"Keyboard"
#define SMBIOS_MISC_PORT_TOKEN_MOUSE                0x02
#define SMBIOS_MISC_PORT_INTERNAL_MOUSE             L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_MOUSE             L"Mouse"
#define SMBIOS_MISC_PORT_TOKEN_COM1                 0x03
#define SMBIOS_MISC_PORT_INTERNAL_COM1              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_COM1              L"COM 1"
#define SMBIOS_MISC_PORT_TOKEN_VIDEO                0x04
#define SMBIOS_MISC_PORT_INTERNAL_VIDEO             L"J1A2B"
#define SMBIOS_MISC_PORT_EXTERNAL_VIDEO             L"Video"
#define SMBIOS_MISC_PORT_TOKEN_HDMI                 0x05
#define SMBIOS_MISC_PORT_INTERNAL_HDMI              L"J3A2"
#define SMBIOS_MISC_PORT_EXTERNAL_HDMI              L"HDMI"
#define SMBIOS_MISC_PORT_TOKEN_USB1                 0x06
#define SMBIOS_MISC_PORT_INTERNAL_USB1              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB1              L"USB1.1 - 1#"
#define SMBIOS_MISC_PORT_TOKEN_USB2                 0x07
#define SMBIOS_MISC_PORT_INTERNAL_USB2              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB2              L"USB1.1 - 2#"
#define SMBIOS_MISC_PORT_TOKEN_USB3                 0x08
#define SMBIOS_MISC_PORT_INTERNAL_USB3              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB3              L"USB1.1 - 3#"
#define SMBIOS_MISC_PORT_TOKEN_USB4                 0x09
#define SMBIOS_MISC_PORT_INTERNAL_USB4              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB4              L"USB1.1 - 4#"
#define SMBIOS_MISC_PORT_TOKEN_USB5                 0x0A
#define SMBIOS_MISC_PORT_INTERNAL_USB5              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB5              L"USB1.1 - 5#"
#define SMBIOS_MISC_PORT_TOKEN_USB201               0x0B
#define SMBIOS_MISC_PORT_INTERNAL_USB201            L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB201            L"USB2.0 - 1#"
#define SMBIOS_MISC_PORT_TOKEN_USB202               0x0C
#define SMBIOS_MISC_PORT_INTERNAL_USB202            L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_USB202            L"USB2.0 - 2#"
#define SMBIOS_MISC_PORT_TOKEN_NETWORK              0x0D
#define SMBIOS_MISC_PORT_INTERNAL_NETWORK           L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_NETWORK           L"Ethernet"
#define SMBIOS_MISC_PORT_TOKEN_IDE1                 0x0E
#define SMBIOS_MISC_PORT_INTERNAL_IDE1              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_IDE1              L"PATA"
#define SMBIOS_MISC_PORT_TOKEN_SATA0                0x0F
#define SMBIOS_MISC_PORT_INTERNAL_SATA0             L"J8J1"
#define SMBIOS_MISC_PORT_EXTERNAL_SATA0             L"SATA Port 0 Direct Connect"
#define SMBIOS_MISC_PORT_TOKEN_ESATA4               0x10
#define SMBIOS_MISC_PORT_INTERNAL_ESATA4            L"J7J1"
#define SMBIOS_MISC_PORT_EXTERNAL_ESATA4            L"eSATA Port 4"
#define SMBIOS_MISC_PORT_TOKEN_ESATA3               0x11
#define SMBIOS_MISC_PORT_INTERNAL_ESATA3            L"J6J1"
#define SMBIOS_MISC_PORT_EXTERNAL_ESATA3            L"eSATA Port 3"
#define SMBIOS_MISC_PORT_TOKEN_ACIN                 0x12
#define SMBIOS_MISC_PORT_INTERNAL_ACIN              L"J1F2"
#define SMBIOS_MISC_PORT_EXTERNAL_ACIN              L"AC IN"
#define SMBIOS_MISC_PORT_TOKEN_PCHJTAG              0x13
#define SMBIOS_MISC_PORT_INTERNAL_PCHJTAG           L"J5B1 - PCH JTAG"
#define SMBIOS_MISC_PORT_EXTERNAL_PCHJTAG           L"None"
#define SMBIOS_MISC_PORT_TOKEN_PORT80               0x14
#define SMBIOS_MISC_PORT_INTERNAL_PORT80            L"J9A1 - TPM/PORT 80"
#define SMBIOS_MISC_PORT_EXTERNAL_PORT80            L"None"
#define SMBIOS_MISC_PORT_TOKEN_2X8HEADER            0x15
#define SMBIOS_MISC_PORT_INTERNAL_2X8HEADER         L"J9E4 - HDA 2X8 Header"
#define SMBIOS_MISC_PORT_EXTERNAL_2X8HEADER         L"None"
#define SMBIOS_MISC_PORT_TOKEN_8PINHEADER           0x16
#define SMBIOS_MISC_PORT_INTERNAL_8PINHEADER        L"J9E7 - HDA 8Pin Header"
#define SMBIOS_MISC_PORT_EXTERNAL_8PINHEADER        L"None"
#define SMBIOS_MISC_PORT_TOKEN_HDAHDMI              0x17
#define SMBIOS_MISC_PORT_INTERNAL_HDAHDMI           L"J8F1 - HDA HDMI"
#define SMBIOS_MISC_PORT_EXTERNAL_HDAHDMI           L"None"
#define SMBIOS_MISC_PORT_TOKEN_MKEYBOARD            0x18
#define SMBIOS_MISC_PORT_INTERNAL_MKEYBOARD         L"J9E3 - Scan Matrix Keyboard"
#define SMBIOS_MISC_PORT_EXTERNAL_MKEYBOARD         L"None"
#define SMBIOS_MISC_PORT_TOKEN_SPI                  0x19
#define SMBIOS_MISC_PORT_INTERNAL_SPI               L"J8E1 - SPI Program"
#define SMBIOS_MISC_PORT_EXTERNAL_SPI               L"None"
#define SMBIOS_MISC_PORT_TOKEN_LPCDOCKING           0x1A
#define SMBIOS_MISC_PORT_INTERNAL_LPCDOCKING        L"J9E5 - LPC Hot Docking"
#define SMBIOS_MISC_PORT_EXTERNAL_LPCDOCKING        L"None"
#define SMBIOS_MISC_PORT_TOKEN_SIDEBAND             0x1B
#define SMBIOS_MISC_PORT_INTERNAL_SIDEBAND          L"J9G2 - LPC SIDE BAND"
#define SMBIOS_MISC_PORT_EXTERNAL_SIDEBAND          L"None"
#define SMBIOS_MISC_PORT_TOKEN_LPCSLOT              0x1C
#define SMBIOS_MISC_PORT_INTERNAL_LPCSLOT           L"J8F2 - LPC Slot"
#define SMBIOS_MISC_PORT_EXTERNAL_LPCSLOT           L"None"
#define SMBIOS_MISC_PORT_TOKEN_PCHXDP               0x1D
#define SMBIOS_MISC_PORT_INTERNAL_PCHXDP            L"J8H3 - PCH XDP"
#define SMBIOS_MISC_PORT_EXTERNAL_PCHXDP            L"None"
#define SMBIOS_MISC_PORT_TOKEN_INFRARED             0x1E
#define SMBIOS_MISC_PORT_INTERNAL_INFRARED          L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_INFRARED          L"Infrared"
#define SMBIOS_MISC_PORT_TOKEN_SDVO                 0x1F
#define SMBIOS_MISC_PORT_INTERNAL_SDVO              L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_SDVO              L"Serial-Digital-Video-Out"
#define SMBIOS_MISC_PORT_TOKEN_LINEIN               0x20
#define SMBIOS_MISC_PORT_INTERNAL_LINEIN            L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_LINEIN            L"Audio Line In"
#define SMBIOS_MISC_PORT_TOKEN_LINEOUT              0x21
#define SMBIOS_MISC_PORT_INTERNAL_LINEOUT           L"None"
#define SMBIOS_MISC_PORT_EXTERNAL_LINEOUT           L"Audio Line Out"
#define SMBIOS_MISC_PORT_TOKEN_SATA2                0x22
#define SMBIOS_MISC_PORT_INTERNAL_SATA2             L"J7G1 - SATA Port 2"
#define SMBIOS_MISC_PORT_EXTERNAL_SATA2             L"None"
#define SMBIOS_MISC_PORT_TOKEN_SATA1                0x23
#define SMBIOS_MISC_PORT_INTERNAL_SATA1             L"J7G2 - SATA Port 1"
#define SMBIOS_MISC_PORT_EXTERNAL_SATA1             L"None"
#define SMBIOS_MISC_PORT_TOKEN_SATAPOWER            0x24
#define SMBIOS_MISC_PORT_INTERNAL_SATAPOWER         L"J6H1 - SATA Power"
#define SMBIOS_MISC_PORT_EXTERNAL_SATAPOWER         L"None"
#define SMBIOS_MISC_PORT_TOKEN_FPHEADER             0x25
#define SMBIOS_MISC_PORT_INTERNAL_FPHEADER          L"J5J1 - FP Header"
#define SMBIOS_MISC_PORT_EXTERNAL_FPHEADER          L"None"
#define SMBIOS_MISC_PORT_TOKEN_ATXPOWER             0x26
#define SMBIOS_MISC_PORT_INTERNAL_ATXPOWER          L"J4H1 - ATX Power"
#define SMBIOS_MISC_PORT_EXTERNAL_ATXPOWER          L"None"
#define SMBIOS_MISC_PORT_TOKEN_AVMC                 0x27
#define SMBIOS_MISC_PORT_INTERNAL_AVMC              L"J1J3 - AVMC"
#define SMBIOS_MISC_PORT_EXTERNAL_AVMC              L"None"
#define SMBIOS_MISC_PORT_TOKEN_BATTB                0x28
#define SMBIOS_MISC_PORT_INTERNAL_BATTB             L"J1H1 - BATT B"
#define SMBIOS_MISC_PORT_EXTERNAL_BATTB             L"None"
#define SMBIOS_MISC_PORT_TOKEN_BATTA                0x29
#define SMBIOS_MISC_PORT_INTERNAL_BATTA             L"J1H2 - BATT A"
#define SMBIOS_MISC_PORT_EXTERNAL_BATTA             L"None"
#define SMBIOS_MISC_PORT_TOKEN_CPUFAN               0x2A
#define SMBIOS_MISC_PORT_INTERNAL_CPUFAN            L"J2G1 - CPU Fan"
#define SMBIOS_MISC_PORT_EXTERNAL_CPUFAN            L"None"
#define SMBIOS_MISC_PORT_TOKEN_XDP                  0x2B
#define SMBIOS_MISC_PORT_INTERNAL_XDP               L"J1D3 - XDP"
#define SMBIOS_MISC_PORT_EXTERNAL_XDP               L"None"
#define SMBIOS_MISC_PORT_TOKEN_MEMORY1              0x2C
#define SMBIOS_MISC_PORT_INTERNAL_MEMORY1           L"J4V1 - Memory Slot 1"
#define SMBIOS_MISC_PORT_EXTERNAL_MEMORY1           L"None"
#define SMBIOS_MISC_PORT_TOKEN_MEMORY2              0x2D
#define SMBIOS_MISC_PORT_INTERNAL_MEMORY2           L"J4V2 - Memory Slot 2"
#define SMBIOS_MISC_PORT_EXTERNAL_MEMORY2           L"None"
#define SMBIOS_MISC_PORT_TOKEN_FANPWR               0x2E
#define SMBIOS_MISC_PORT_INTERNAL_FANPWR            L"J4C1 - FAN PWR"
#define SMBIOS_MISC_PORT_EXTERNAL_FANPWR            L"None"

typedef struct {
  CHAR16 *InternalReferenceDesignator;
  CHAR16 *ExternalReferenceDesignator;
} SMBIOS_TABLE_TYPE8_STRINGS;

///
/// SMBIOS_TABLE_TYPE9 Default Strings
///
#define SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE0         0x43
#define SMBIOS_MISC_SYSTEM_SLOT_PCIE0               L"PCI-Express 0"
#define SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE1         0x44
#define SMBIOS_MISC_SYSTEM_SLOT_PCIE1               L"PCI-Express 1"
#define SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE2         0x45
#define SMBIOS_MISC_SYSTEM_SLOT_PCIE2               L"PCI-Express 2"
#define SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE3         0x46
#define SMBIOS_MISC_SYSTEM_SLOT_PCIE3               L"PCI-Express 3"

typedef struct {
  CHAR16 *SlotDesignation;
} SMBIOS_TABLE_TYPE9_STRINGS;

///
/// SMBIOS_TABLE_TYPE10 Default Strings
///
#define SMBIOS_MISC_ONBOARD_DEVICE_VIDEO            L"To Be Filled by O.E.M."
#define SMBIOS_MISC_ONBOARD_DEVICE_ETHERNET         L"To Be Filled by O.E.M."
#define SMBIOS_MISC_ONBOARD_DEVICE_SOUND            L"To Be Filled by O.E.M."

typedef struct {
  CHAR16 *DescriptionString;
} SMBIOS_TABLE_TYPE10_STRINGS;

///
/// SMBIOS_TABLE_TYPE12 Default Strings
///
#define SMBIOS_MISC_SYSTEM_OPTION_STRING            L"J6H1:1-X CMOS CLEAR(default); J8H1:1-X BIOS RECOVERY"

typedef struct {
  CHAR16 *StringCount;
} SMBIOS_TABLE_TYPE12_STRINGS;

///
/// SMBIOS_TABLE_TYPE13 Default Strings
///
#define SMBIOS_MISC_INSTALLED_LANGUAGE_ENGLISH      L"English"

typedef struct {
  CHAR16 *CurrentLanguages;
} SMBIOS_TABLE_TYPE13_STRINGS;

///
/// SMBIOS_TABLE_TYPE22 Default Strings
///
#define SMBIOS_MISC_PROTABLE_BATTERY_TOKEN_VIRTUAL                    0x60
#define SMBIOS_MISC_PORTABLE_BATTERY_VIRTUAL_LOCATION                 L"Virtual"
#define SMBIOS_MISC_PORTABLE_BATTERY_VIRTUAL_MANUFACTURER             L"-Virtual Battery 0-" 
#define SMBIOS_MISC_PORTABLE_BATTERY_VIRTUAL_MANUFACTURER_DATE        L"01/01/2013"
#define SMBIOS_MISC_PORTABLE_BATTERY_VIRTUAL_SERIAL_NUMBER            L"Battery 0"
#define SMBIOS_MISC_PORTABLE_BATTERY_VIRTUAL_DEVICE_NAME              L"CRB Battery 0"
#define SMBIOS_MISC_PROTABLE_BATTERY_TOKEN_REAL1                      0x61
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL1_LOCATION                   L"Real 1"
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL1_MANUFACTURER               L"E-One Moli Energy" 
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL1_MANUFACTURER_DATE          L"01/01/2013"
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL1_SERIAL_NUMBER              L"FSPK50074"
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL1_DEVICE_NAME                L"MOLICEL"
#define SMBIOS_MISC_PROTABLE_BATTERY_TOKEN_REAL2                      0x62
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL2_LOCATION                   L"Real 2"
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL2_MANUFACTURER               L"E-One Moli Energy" 
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL2_MANUFACTURER_DATE          L"01/01/2013"
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL2_SERIAL_NUMBER              L"FSPK50074"
#define SMBIOS_MISC_PORTABLE_BATTERY_REAL2_DEVICE_NAME                L"MOLICEL"

typedef struct {
  CHAR16 *Location;
  CHAR16 *Manufacturer;
  CHAR16 *ManufacturerDate;
  CHAR16 *SerialNumber;
  CHAR16 *DeviceName;
} SMBIOS_TABLE_TYPE22_STRINGS;

///
/// SMBIOS_TABLE_TYPE27 Default Strings
///
#define SMBIOS_MISC_COOLING_DEVICE_DESCRIPTION                        L" "

typedef struct {
  CHAR16 *Description;
} SMBIOS_TABLE_TYPE27_STRINGS;

///
/// SMBIOS_TABLE_TYPE39 Default Strings
///
typedef struct {
  CHAR16 *Location;
  CHAR16 *DeviceName;
  CHAR16 *Manufacturer;
  CHAR16 *SerialNumber;
  CHAR16 *AssetTagNumber;
  CHAR16 *ModelPartNumber;
  CHAR16 *RevisionLevel;
} SMBIOS_TABLE_TYPE39_STRINGS;

///
/// SMBIOS_TABLE_TYPE90 Default Strings
///
typedef struct {
  CHAR16 *SECVersion;
  CHAR16 *uCodeVersion;
  CHAR16 *GOPVersion;
  CHAR16 *CpuStepping;
} SMBIOS_TABLE_TYPE90_STRINGS; 

///
/// SMBIOS_TABLE_TYPE94 Default Strings
///
typedef struct {
  CHAR16 *GopVersion;
  CHAR16 *uCodeVersion;
  CHAR16 *MRCVersion;
  CHAR16 *SECVersion;
  CHAR16 *ULPMCVersion;
  CHAR16 *PMCVersion;
  CHAR16 *PUnitVersion;
  CHAR16 *SoCVersion;
  CHAR16 *BoardVersion;
  CHAR16 *FabVersion;
  CHAR16 *CPUFlavor;
  CHAR16 *BiosVersion;
  CHAR16 *PmicVersion;
  CHAR16 *TouchVersion;
  CHAR16 *SecureBoot;
  CHAR16 *BootMode;
  CHAR16 *SpeedStepMode;
  CHAR16 *CPUTurboMode;
  CHAR16 *MaxCState;
  CHAR16 *GfxTurbo; 
  CHAR16 *S0ix;
  CHAR16 *RC6;
} SMBIOS_TABLE_TYPE94_STRINGS; 

#endif