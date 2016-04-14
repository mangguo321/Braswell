/** @file
  BIOS processor cache details is Misc for subclass type 7, SMBIOS type 7.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "MiscSubclassDriver.h"
#include <Guid/DataHubRecords.h>

extern  SMBIOS_TABLE_TYPE7            *SmbiosRecordL1;
extern  SMBIOS_TABLE_TYPE7            *SmbiosRecordL2;
extern  SMBIOS_TABLE_TYPE7            *SmbiosRecordL3;

UINT32
ConvertBase2ToRaw (
  IN  EFI_EXP_BASE2_DATA             *Data)
{
  UINTN         Index;
  UINT32        RawData;

  RawData = Data->Value;
  for (Index = 0; Index < (UINTN) Data->Exponent; Index++) {
     RawData <<= 1;
  }

  return  RawData;
}

MISC_SMBIOS_TABLE_FUNCTION(MiscProcessorCache)
{

  return EFI_SUCCESS;
}
