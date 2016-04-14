/** @file
  BIOS system slot designator information boot time changes for SMBIOS type 9.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MiscSubclassDriver.h"

/**
  This function makes boot time changes to the contents of the
  MiscSystemSlotDesignator structure (Type 9).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscSystemSlotDesignator)
{
  CHAR8                       *OptionalStrStart;
  UINTN                       StringNumber;
  UINTN                       StringLength;
  EFI_STATUS                  Status;
  SMBIOS_TABLE_TYPE9          *SmbiosRecord;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  SMBIOS_TABLE_TYPE9          *ForType9InputData;
  SMBIOS_TABLE_TYPE9_STRINGS  SmbiosTableType9Strings;
  CHAR16                      *StrBufferStart;
  CHAR16                      *StrBufferPtr;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "(MiscSystemSlotDesignator) Entry.\n"));
  ZeroMem (&SmbiosTableType9Strings, sizeof(SMBIOS_TABLE_TYPE9_STRINGS));
  StringNumber       = 0;
  StringLength       = 0;
  ForType9InputData  = (SMBIOS_TABLE_TYPE9 *) RecordData;
  StrBufferStart     = AllocateZeroPool (1 * SMBIOS_STRING_MAX_LENGTH * sizeof(CHAR16));
  StrBufferPtr       = StrBufferStart;

  //
  // Initialize SMBIOS Tables Type9 strings
  //
  switch (ForType9InputData->SlotDesignation) {
    case SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE0:
      SmbiosStrInit (&SmbiosTableType9Strings.SlotDesignation, SMBIOS_MISC_SYSTEM_SLOT_PCIE0, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE1:
      SmbiosStrInit (&SmbiosTableType9Strings.SlotDesignation, SMBIOS_MISC_SYSTEM_SLOT_PCIE1, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE2:
      SmbiosStrInit (&SmbiosTableType9Strings.SlotDesignation, SMBIOS_MISC_SYSTEM_SLOT_PCIE2, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_SYSTEM_SLOT_TOKEN_PCIE3:
      SmbiosStrInit (&SmbiosTableType9Strings.SlotDesignation, SMBIOS_MISC_SYSTEM_SLOT_PCIE3, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    default:
      break;
  }
  ForType9InputData->SlotDesignation = SMBIOS_MISC_STRING_1;

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE9) + StringLength + StringNumber + 1);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem(SmbiosRecord, sizeof(SMBIOS_TABLE_TYPE9) + StringLength + StringNumber + 1);
  CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE9));

  //
  // Update SMBIOS Tables Type9 strings
  //
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  SmbiosStringsUpdate ((CHAR16 **) &SmbiosTableType9Strings, OptionalStrStart, StringNumber);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios-> Add(
                      Smbios,
                      NULL,
                      &SmbiosHandle,
                      (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                      );

  FreePool(SmbiosRecord);
  FreePool(StrBufferStart);
  return Status;
}
