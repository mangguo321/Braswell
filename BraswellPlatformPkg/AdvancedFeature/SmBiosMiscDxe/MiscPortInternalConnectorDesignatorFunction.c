/** @file
  Create the device path for the Port internal connector designator.
  Port internal connector designator information is Misc for subclass type 6, SMBIOS type 8.

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

/**
  This function makes boot time changes to the contents of the
  MiscPortConnectorInformation (Type 8).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscPortInternalConnectorDesignator)
{
  CHAR8                       *OptionalStrStart;
  UINTN                       StringNumber;
  UINTN                       StringLength;
  EFI_STATUS                  Status;
  SMBIOS_TABLE_TYPE8          *SmbiosRecord;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  SMBIOS_TABLE_TYPE8          *ForType8InputData;
  SMBIOS_TABLE_TYPE8_STRINGS  SmbiosTableType8Strings;
  CHAR16                      *StrBufferStart;
  CHAR16                      *StrBufferPtr;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "(MiscPortInternalConnectorDesignator) Entry.\n"));
  ZeroMem (&SmbiosTableType8Strings, sizeof(SMBIOS_TABLE_TYPE8_STRINGS));
  StringNumber          = 0;
  StringLength          = 0;
  ForType8InputData     = (SMBIOS_TABLE_TYPE8 *)RecordData;
  StrBufferStart        = AllocateZeroPool (2 * SMBIOS_STRING_MAX_LENGTH * sizeof(CHAR16));
  StrBufferPtr          = StrBufferStart;

  //
  // Initialize SMBIOS Tables Type8 strings
  //
  switch (ForType8InputData->InternalReferenceDesignator) {
    case SMBIOS_MISC_PORT_TOKEN_KEYBOARD:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_KEYBOARD, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_KEYBOARD, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_MOUSE:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_MOUSE, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_MOUSE, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_COM1:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_COM1, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_COM1, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_VIDEO:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_VIDEO, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_VIDEO, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_HDMI:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_HDMI, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_HDMI, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB1:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB1, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB1, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB2:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB2, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB2, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB3:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB3, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB3, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB4:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB4, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB4, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB5:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB5, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB5, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB201:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB201, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB201, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_USB202:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_USB202, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_USB202, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_NETWORK:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_NETWORK, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_NETWORK, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_SATA0:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_SATA0, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_SATA0, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_ESATA4:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_ESATA4, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_ESATA4, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_ESATA3:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_ESATA3, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_ESATA3, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_SATA2:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_SATA2, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_SATA2, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_SATA1:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_SATA1, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_SATA1, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_ACIN:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_ACIN, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_ACIN, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_PCHJTAG:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_PCHJTAG, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_PCHJTAG, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_PORT80:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_PORT80, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_PORT80, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_2X8HEADER:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_2X8HEADER, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_2X8HEADER, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_8PINHEADER:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_8PINHEADER, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_8PINHEADER, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_HDAHDMI:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_HDAHDMI, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_HDAHDMI, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_MKEYBOARD:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_MKEYBOARD, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_MKEYBOARD, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_SPI:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_SPI, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_SPI, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_LPCDOCKING:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_LPCDOCKING, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_LPCDOCKING, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_SIDEBAND:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_SIDEBAND, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_SIDEBAND, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_LPCSLOT:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_LPCSLOT, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_LPCSLOT, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_PCHXDP:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_PCHXDP, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_PCHXDP, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_SATAPOWER:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_SATAPOWER, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_SATAPOWER, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_FPHEADER:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_FPHEADER, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_FPHEADER, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_ATXPOWER:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_ATXPOWER, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_ATXPOWER, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_AVMC:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_AVMC, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_AVMC, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_BATTB:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_BATTB, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_BATTB, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_BATTA:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_BATTA, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_BATTA, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_CPUFAN:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_CPUFAN, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_CPUFAN, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_XDP:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_XDP, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_XDP, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_MEMORY1:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_MEMORY1, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_MEMORY1, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_MEMORY2:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_MEMORY2, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_MEMORY2, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case SMBIOS_MISC_PORT_TOKEN_FANPWR:
      SmbiosStrInit (&SmbiosTableType8Strings.InternalReferenceDesignator, SMBIOS_MISC_PORT_INTERNAL_FANPWR, &StrBufferPtr, &StringNumber, &StringLength);
      SmbiosStrInit (&SmbiosTableType8Strings.ExternalReferenceDesignator, SMBIOS_MISC_PORT_EXTERNAL_FANPWR, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    default:
      break;
  }
  ForType8InputData->InternalReferenceDesignator = SMBIOS_MISC_STRING_1;

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE8) + StringLength + StringNumber + 1);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem(SmbiosRecord, sizeof(SMBIOS_TABLE_TYPE8) + StringLength + StringNumber + 1);
  CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE8));

  //
  // Update SMBIOS Tables Type8 strings
  //
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  SmbiosStringsUpdate ((CHAR16 **) &SmbiosTableType8Strings, OptionalStrStart, StringNumber);

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
