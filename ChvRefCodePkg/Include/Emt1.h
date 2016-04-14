/** @file
  Energe Management Table extra.

  Copyright (c) 1996 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _EMT1_H
#define _EMT1_H

//
// Statements that include other files
//
#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi20.h>
#include <IndustryStandard/Acpi30.h>

#define EFI_ACPI_EMT1_REVISION                          0x00000000
#define EFI_ACPI_OEM1_REVISION                          0x00000001
#define EFI_ACPI_EMT1_SIGNATURE                         SIGNATURE_32('E', 'M', '_', '1')
#define EFI_ACPI_OEM1_SIGNATURE                         SIGNATURE_32('O', 'E', 'M', '1')

#define EFI_ACPI_EMT1_TABLE_ID     SIGNATURE_64('O','E','M','1',' ',' ',' ',' ') // OEM table id 8 bytes long
#define EFI_ACPI_OEM1_TABLE_ID     SIGNATURE_64('E','N','R','G','Y','M','G','T') // OEM table id 8 bytes long

#pragma pack(1)

typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER Header;
    UINT16   IaAppsRun;
    UINT8    IaAppsCap;
    UINT8    CapOrVoltFlag;    
    UINT8    BootOnInvalidBatt;
} EFI_ACPI_ENERGY_MANAGEMENT_1_TABLE;

typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER Header;
	UINT8                       FixedOption0;
	UINT8                       FixedOption1;
	UINT8                       DBIInGpioNumber;
	UINT8                       DBIOutGpioNumber;
	UINT8                       BatChpType;
	UINT16                      IaAppsRun;
	UINT8                       BatIdDBIBase;
	UINT8                       BatIdAnlgBase;
	UINT8                       IaAppsCap;
	UINT16                      VBattFreqLmt;
	UINT8                       CapFreqIdx;
	UINT8                       Rsvd1;
	UINT8                       BatIdx;
	UINT8                       IaAppsToUse;
	UINT8                       TurboChrg;
	UINT8                       Rsvd2[11];
}EFI_ACPI_EM_OEM_1_TABLE;

#pragma pack()

#endif
