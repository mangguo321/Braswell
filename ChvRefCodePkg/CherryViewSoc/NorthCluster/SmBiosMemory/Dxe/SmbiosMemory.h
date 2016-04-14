/** @file
  Header file for the SMBIOS Memory Driver.
  This driver will determine memory configuration information from the chipset
  and memory and create SMBIOS Memory structures appropriately.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SMBIOS_MEMORY_H_
#define _SMBIOS_MEMORY_H_

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBus.h>
#include <ChvAccess.h>
#include <IndustryStandard/SmBios.h>
#include <VpdHeader.h>

//
// Maximum number of SDRAM channels supported by the memory controller
//
#define MAX_CHANNELS         2
//
// Maximum number of DIMM sockets supported by each channel
//
#define MAX_SLOTS            1
//
// Maximum number of sides supported per DIMM
//
#define MAX_SIDES            2
//
// Maximum number of DIMM sockets supported by the memory controller
//
#define MAX_SOCKETS          (MAX_CHANNELS * MAX_SLOTS)
//
// Maximum number of rows supported by the memory controller
//
#define MAX_RANKS            (MAX_SOCKETS * MAX_SIDES)
// This is the generated header file which includes whatever needs to be exported (strings + IFR)
//
#include "SmbiosMemoryStrDefs.h"

//
// Driver Consumed Protocol Prototypes
//
#include "Protocol/MemInfo.h"
// Driver private data
//
#include "Guid/MemoryConfigData.h"

FSP_SMBIOS_MEMORY_INFO gFspSmbiosMemoryInfo;

#define EFI_MEMORY_SUBCLASS_DRIVER_GUID \
  { 0x1767CEED, 0xDB82, 0x47cd, 0xBF, 0x2B, 0x68, 0x45, 0x8A, 0x8C, 0xCF, 0xFF }


enum {
  DDRType_DDR3 = 0,
  DDRType_DDR3L = 1,
  DDRType_DDR3U = 2,
  DDRType_DDR3All = 3,
  DDRType_LPDDR2 = 4,
  DDRType_LPDDR3 = 5,
  DDRType_DDR4 = 6
};

// Maximum memory supported by the memory controller
// 4 GB in terms of KB
//
#define MAX_MEM_CAPACITY (4 * 1024 * 1024)

//
// Memory Module Manufacture ID List Structure
//
typedef struct {
  UINT8 Index;
  UINT8 ManufactureId;
  CHAR16* ManufactureName;
} MEMORY_MODULE_MANUFACTURE_LIST;

//
// Row configuration data structure
//
typedef struct {
  EFI_PHYSICAL_ADDRESS          BaseAddress;
  UINT64                        RowLength;  // Size of Row in bytes
} DDR_ROW_CONFIG;

EFI_STRING
EFIAPI
SmbiosMiscGetString (
  IN EFI_STRING_ID   StringId
  );
//
// Prototypes
//
EFI_STATUS
SmbiosMemoryEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

typedef UINT16  STRING_REF;

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Guid/DataHubRecords.h>
#include "Protocol/DataHub.h"
#include "Protocol/SmbusHc.h"
#include "Protocol/FrameworkHii.h"
#include "Protocol/HiiDatabase.h"
#include "Protocol/HiiString.h"
#include "Library/HiiLib.h"

EFI_HII_HANDLE                         *mHiiHandle;
EFI_HII_STRING_PROTOCOL                *mHiiString;

extern MEM_INFO_PROTOCOL               *mMemInfoHob;
extern EFI_SMBIOS_PROTOCOL             *mSmbios;
EFI_HII_HANDLE                         StringPackHandle;


VOID
GetType16Hndl (
  IN  EFI_SMBIOS_PROTOCOL      *Smbios,
  OUT  EFI_SMBIOS_HANDLE       *Handle
  );


/**
  This function installs SMBIOS Table Type 16 (Physical Memory Array).

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
**/
EFI_STATUS
InstallSmbiosType16 (
  VOID
  );

/**
  This function installs SMBIOS Table Type 17 (Memory Device).
  This function installs one table per memory device slot, whether populated or not.

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_NOT_FOUND         - if the Hii PackageList could not be found.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
  @retval EFI_INVALID_PARAMETER - if a required parameter in a subfunction is NULL.
**/
EFI_STATUS
InstallSmbiosType17 (
  VOID
  );

/**
  This function installs SMBIOS Table Type 19 (Physical Memory Array).

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
**/
EFI_STATUS
InstallSmbiosType19 (
  VOID
  );


#endif
