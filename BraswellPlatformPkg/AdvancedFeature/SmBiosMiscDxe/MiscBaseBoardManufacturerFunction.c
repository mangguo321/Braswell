/** @file
  BaseBoard manufacturer information boot time changes for SMBIOS type 2.

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
#include "Library/DebugLib.h"
#include <Uefi/UefiBaseType.h>
#include <Guid/PlatformInfo.h>

/**
  This function makes boot time changes to the contents of the
  MiscBaseBoardManufacturer (Type 2).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscBaseBoardManufacturer)
{
  CHAR8                           *OptionalStrStart;
  UINTN                           ManuStrLen;
  UINTN                           VerStrLen;
  UINTN                           PdNameStrLen;
  UINTN                           SerialNumStrLen;
  UINTN                           AssetTagLen;
  EFI_STATUS                      Status;
  CHAR16                          Manufacturer[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          ProductName[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          Version[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          SerialNumber[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          AssetTag[SMBIOS_STRING_MAX_LENGTH];
  EFI_STRING                      ManufacturerPtr;
  EFI_STRING                      ProductNamePtr;
  EFI_STRING                      VersionPtr;
  EFI_STRING                      SerialNumberPtr;
  EFI_STRING                      AssetTagPtr;
  STRING_REF                      TokenToGet;
  STRING_REF                      TokenToUpdate;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  SMBIOS_TABLE_TYPE2              *SmbiosRecord;
  EFI_MISC_BASE_BOARD_MANUFACTURER   *ForType2InputData;
  UINTN                           RecordLengthInBytes;
  EFI_PLATFORM_INFO_HOB           *PlatformInfoHob;

  PlatformInfoHob = PcdGetPtr (PcdPlatformInfo);

  ForType2InputData = (EFI_MISC_BASE_BOARD_MANUFACTURER *)RecordData;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Update strings from PCD for STR_MISC_BASE_BOARD_MANUFACTURER
  //
  AsciiStrToUnicodeStr ((const CHAR8*)PcdGetPtr(PcdSMBIOSBoardManufacturer), Manufacturer);
  if (StrLen (Manufacturer) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_BASE_BOARD_MANUFACTURER);
    HiiSetString (mHiiHandle, TokenToUpdate, Manufacturer, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_MANUFACTURER);
  ManufacturerPtr = SmbiosMiscGetString (TokenToGet);
  ManuStrLen = StrLen(ManufacturerPtr);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    DEBUG (( DEBUG_ERROR, "ManuStrLen (0x%08x) > 0x%08x\r\n", ManuStrLen, SMBIOS_STRING_MAX_LENGTH ));
    return EFI_UNSUPPORTED;
  }

  //
  // Get Product name from BoardId
  //
  switch (PlatformInfoHob->BoardId) {
    case 0x00:
      StrCpy (ProductName, L"Alpine Valley Board");
      break;
    case 0x01:
      StrCpy (ProductName, L"Cherry Trail CR");
      break;
    case 0x02:
      StrCpy (ProductName, L"Cherry Trail COPOP");
      break;
    case 0x03:
      StrCpy (ProductName, L"Cherry Trail Tablet");
      break;
    case 0x04:
      StrCpy (ProductName, L"Cherry Trail Sony");
      break;
    case 0x05:
      StrCpy (ProductName, L"BayLake Board (RVP DDR3L)");
      break;
    case 0x07:
      StrCpy (ProductName, L"PPV- STHI Board");
      break;
    case 0x08:
      StrCpy (ProductName, L"Cherry Trail FFD");
      break;
    case 0x09:
      StrCpy (ProductName, L"Cherry Trail Tablet");
      break;
    case 0x0A:
      StrCpy (ProductName, L"Cherry Trail FFD");
      break;
    case 0x20:
      StrCpy (ProductName, L"Bayley Bay Board");
      break;
    case 0x30:
      StrCpy (ProductName, L"Bakersport Board");
      break;
    case 0x44:
      StrCpy (ProductName, L"Braswell CRB");
      break;
    case 0x55:
      StrCpy (ProductName, L"Braswell LC");
      break;
    case 0x60:
      StrCpy (ProductName, L"Braswell RVP ECC");
      break;
    case 0x80:
      StrCpy (ProductName, L"Braswell Cherry Hill");
      break;
    case 0x82:
      StrCpy (ProductName, L"Braswell Cherry Island");
      break;
    case 0xEE:
      StrCpy (ProductName, L"Bakersport Board");
      break;
    case 0xF1:
      StrCpy (ProductName, L"BayLake FFRD");
      break;
    case 0xF2:
      StrCpy (ProductName, L"BayLake RVP");
      break;
    case 0xF3:
      StrCpy (ProductName, L"CherryTrail ERB");
      break;
    case 0xF4:
      StrCpy (ProductName, L"CherryTrail CRB");
      break;
    case 0xF5:
      StrCpy (ProductName, L"Baytrail FFRD8");
      break;
    default:
      StrCpy (ProductName, L"CherryTrail Platform");
      break;
  }

  if (StrLen (ProductName) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_BASE_BOARD_PRODUCT_NAME);
    HiiSetString (mHiiHandle, TokenToUpdate, ProductName, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_PRODUCT_NAME);
  ProductNamePtr = SmbiosMiscGetString (TokenToGet);
  PdNameStrLen = StrLen(ProductNamePtr);
  if (PdNameStrLen > SMBIOS_STRING_MAX_LENGTH) {
    DEBUG (( DEBUG_ERROR, "PdNameStrLen (0x%08x) > 0x%08x\r\n", PdNameStrLen, SMBIOS_STRING_MAX_LENGTH ));
    return EFI_UNSUPPORTED;
  }

  //
  // Update strings from PCD for STR_MISC_BASE_BOARD_VERSION
  //
  AsciiStrToUnicodeStr ((const CHAR8*)PcdGetPtr(PcdSMBIOSBoardVersion), Version);
  if (StrLen (Version) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_BASE_BOARD_VERSION);
    HiiSetString (mHiiHandle, TokenToUpdate, Version, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_VERSION);
  VersionPtr = SmbiosMiscGetString (TokenToGet);
  VerStrLen = StrLen(VersionPtr);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    DEBUG (( DEBUG_ERROR, "VerStrLen (0x%08x) > 0x%08x\r\n", VerStrLen, SMBIOS_STRING_MAX_LENGTH ));
    return EFI_UNSUPPORTED;
  }
  
  //
  // Update strings from PCD for STR_MISC_BASE_BOARD_SERIAL_NUMBER 
  //
  AsciiStrToUnicodeStr ((const CHAR8*)PcdGetPtr(PcdSMBIOSBoardSerialNumber), SerialNumber);
  if (StrLen (SerialNumber) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_BASE_BOARD_SERIAL_NUMBER);
    HiiSetString (mHiiHandle, TokenToUpdate, SerialNumber, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_SERIAL_NUMBER);
  SerialNumberPtr = SmbiosMiscGetString (TokenToGet);
  SerialNumStrLen = StrLen(SerialNumberPtr);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    DEBUG (( DEBUG_ERROR, "SerialNumStrLen (0x%08x) > 0x%08x\r\n", SerialNumStrLen, SMBIOS_STRING_MAX_LENGTH ));
    return EFI_UNSUPPORTED;
  }

  //
  // Update strings from PCD for STR_MISC_BASE_BOARD_ASSET_TAG 
  //
  AsciiStrToUnicodeStr ((const CHAR8*)PcdGetPtr(PcdSMBIOSBoardAssetTag), AssetTag);
  if (StrLen (AssetTag) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_BASE_BOARD_ASSET_TAG);
    HiiSetString (mHiiHandle, TokenToUpdate, AssetTag, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_BASE_BOARD_ASSET_TAG);
  AssetTagPtr = SmbiosMiscGetString (TokenToGet);
  AssetTagLen = StrLen(AssetTagPtr);
  if (AssetTagLen > SMBIOS_STRING_MAX_LENGTH) {
    DEBUG (( DEBUG_ERROR, "AssetTagLen (0x%08x) > 0x%08x\r\n", AssetTagLen, SMBIOS_STRING_MAX_LENGTH ));
    return EFI_UNSUPPORTED;
  }
  
  //
  // Two zeros following the last string.
  //
  RecordLengthInBytes = sizeof (SMBIOS_TABLE_TYPE2)
                        + ManuStrLen + 1
                        + PdNameStrLen + 1
                        + VerStrLen + 1
                        + SerialNumStrLen + 1
                        + AssetTagLen + 1
                        + 1;
  SmbiosRecord = AllocateZeroPool(RecordLengthInBytes);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE2);

  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;

  //
  // Manu will be the 1st optional string following the formatted structure.
  //
  SmbiosRecord->Manufacturer = 1;

  //
  // ProductName will be the 2st optional string following the formatted structure.
  //
  SmbiosRecord->ProductName  = 2;

  //
  // Version will be the 3rd optional string following the formatted structure.
  //
  SmbiosRecord->Version = 3;

  //
  // SerialNumber will be the 4th optional string following the formatted structure.
  //
  SmbiosRecord->SerialNumber = 4;

  //
  // AssertTag will be the 5th optional string following the formatted structure.
  //
  SmbiosRecord->AssetTag = 5;

  //
  // LocationInChassis will be the 6th optional string following the formatted structure.
  //
  SmbiosRecord->LocationInChassis = 6;
  SmbiosRecord->FeatureFlag = (*(BASE_BOARD_FEATURE_FLAGS*)&(ForType2InputData->BaseBoardFeatureFlags));
  SmbiosRecord->ChassisHandle  = 0;
  SmbiosRecord->BoardType      = (UINT8)ForType2InputData->BaseBoardType;
  SmbiosRecord->NumberOfContainedObjectHandles = 0;

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);

  //
  // Since we fill NumberOfContainedObjectHandles = 0 for simple, just after this filed to fill string
  //
  UnicodeStrToAsciiStr(ManufacturerPtr, OptionalStrStart);
  UnicodeStrToAsciiStr(ProductNamePtr, OptionalStrStart + ManuStrLen + 1);
  UnicodeStrToAsciiStr(VersionPtr, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1);
  UnicodeStrToAsciiStr(SerialNumberPtr, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1);
  UnicodeStrToAsciiStr(AssetTagPtr, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen+ 1);

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

  //
  // Free the memory allocated before 
  //
  FreePool (ManufacturerPtr);
  FreePool (ProductNamePtr);
  FreePool (VersionPtr);
  FreePool (SerialNumberPtr);
  FreePool (AssetTagPtr);

  return Status;
}
