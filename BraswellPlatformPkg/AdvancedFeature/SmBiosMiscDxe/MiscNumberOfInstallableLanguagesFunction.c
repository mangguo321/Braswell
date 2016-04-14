/** @file
  This driver parses the mSmbiosMiscDataTable structure and reports
  any generated data.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "MiscSubclassDriver.h"

/**
  This function makes boot time changes to the contents of the
  MiscNumberOfInstallableLanguages (Type 13).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(NumberOfInstallableLanguages)
{
  CHAR8                        *OptionalStrStart;
  UINTN                        StringNumber;
  UINTN                        StringLength;
  EFI_STATUS                   Status;
  EFI_SMBIOS_HANDLE            SmbiosHandle;
  SMBIOS_TABLE_TYPE13          *SmbiosRecord;
  SMBIOS_TABLE_TYPE13_STRINGS  SmbiosTableType13Strings;
  CHAR16                       *StrBufferStart;
  CHAR16                       *StrBufferPtr;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "(NumberOfInstallableLanguages) Entry.\n"));
  ZeroMem (&SmbiosTableType13Strings, sizeof(SMBIOS_TABLE_TYPE13_STRINGS));
  StringNumber    = 0;
  StringLength    = 0;
  StrBufferStart  = AllocateZeroPool (1 * SMBIOS_STRING_MAX_LENGTH * sizeof(CHAR16));
  StrBufferPtr    = StrBufferStart;

  //
  // Initialize SMBIOS Tables Type13 strings
  //
  SmbiosStrInit ((CHAR16 **) &SmbiosTableType13Strings.CurrentLanguages, SMBIOS_MISC_INSTALLED_LANGUAGE_ENGLISH, &StrBufferPtr, &StringNumber, &StringLength);
  
  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool (sizeof(SMBIOS_TABLE_TYPE13) + StringLength + StringNumber + 1);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (SmbiosRecord, sizeof(SMBIOS_TABLE_TYPE13) + StringLength + StringNumber + 1);
  CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE13));

  //
  // Update SMBIOS Tables Type13 strings
  //
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  SmbiosStringsUpdate ((CHAR16 **) &SmbiosTableType13Strings, OptionalStrStart, StringNumber);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                     );
  FreePool(SmbiosRecord);
  FreePool(StrBufferStart);
  return Status;
}
