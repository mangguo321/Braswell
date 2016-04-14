/** @file
  Chassis manufacturer information boot time changes for SMBIOS type 3.

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
#include <Guid/PlatformInfo.h>

/**
  Get the chassis type through the PlatformInfoProtocol.

  @retval UINT8                The chassis type.

**/
UINT8
GetChassisType (
  VOID
  )
{
  EFI_PLATFORM_INFO_HOB           *PlatformInfoHob;

  PlatformInfoHob = PcdGetPtr (PcdPlatformInfo);
  
  if ( NULL != PlatformInfoHob) {
    if (PlatformInfoHob->PlatformFlavor == FlavorMobile) {
      return MiscChassisTypeLapTop;
    } else if (PlatformInfoHob->PlatformFlavor == FlavorDesktop) {
      return MiscChassisTypeDeskTop;
    } else if (PlatformInfoHob->PlatformFlavor == FlavorTablet) {
      return MiscChassisTypeHandHeld;
    }
  }
  
  return MiscChassisTypeLapTop;
}

/**
  This function makes boot time changes to the contents of the
  MiscChassisManufacturer (Type 3).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscChassisManufacturer)
{
  CHAR8                           *OptionalStrStart;
  UINTN                           ManuStrLen;
  UINTN                           VerStrLen;
  UINTN                           AssertTagStrLen;
  UINTN                           SerialNumStrLen;
  EFI_STATUS                      Status;
  CHAR16                          Manufacturer[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          Version[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          SerialNumber[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          AssertTag[SMBIOS_STRING_MAX_LENGTH];  
  EFI_STRING                      ManufacturerPtr;
  EFI_STRING                      VersionPtr;
  EFI_STRING                      SerialNumberPtr;
  EFI_STRING                      AssertTagPtr;
  STRING_REF                      TokenToGet;
  STRING_REF                      TokenToUpdate;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  SMBIOS_TABLE_TYPE3              *SmbiosRecord;
  EFI_MISC_CHASSIS_MANUFACTURER   *ForType3InputData;

  ForType3InputData = (EFI_MISC_CHASSIS_MANUFACTURER *)RecordData;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Update strings from PCD
  //
  AsciiStrToUnicodeStr ((const CHAR8 *)PcdGetPtr(PcdSMBIOSChassisManufacturer), Manufacturer);
  if (StrLen (Manufacturer) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_CHASSIS_MANUFACTURER);
    HiiSetString (mHiiHandle, TokenToUpdate, Manufacturer, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_MANUFACTURER);
  ManufacturerPtr = SmbiosMiscGetString (TokenToGet);
  ManuStrLen = StrLen(ManufacturerPtr);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  AsciiStrToUnicodeStr ((const CHAR8 *)PcdGetPtr(PcdSMBIOSChassisVersion), Version);
  if (StrLen (Version) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_CHASSIS_VERSION);
    HiiSetString (mHiiHandle, TokenToUpdate, Version, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_VERSION);
  VersionPtr = SmbiosMiscGetString (TokenToGet);
  VerStrLen = StrLen(VersionPtr);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
  
  AsciiStrToUnicodeStr ((const CHAR8 *)PcdGetPtr(PcdSMBIOSChassisSerialNumber), SerialNumber);
  if (StrLen (SerialNumber) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_CHASSIS_SERIAL_NUMBER);
    HiiSetString (mHiiHandle, TokenToUpdate, SerialNumber, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_SERIAL_NUMBER);
  SerialNumberPtr = SmbiosMiscGetString (TokenToGet);
  SerialNumStrLen = StrLen(SerialNumberPtr);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  AsciiStrToUnicodeStr ((const CHAR8 *)PcdGetPtr(PcdSMBIOSChassisAssetTag), AssertTag);
  if (StrLen (AssertTag) > 0) {     
    TokenToUpdate = STRING_TOKEN (STR_MISC_CHASSIS_ASSET_TAG);
    HiiSetString (mHiiHandle, TokenToUpdate, AssertTag, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_CHASSIS_ASSET_TAG);
  AssertTagPtr = SmbiosMiscGetString (TokenToGet);
  AssertTagStrLen = StrLen(AssertTagPtr);
  if (AssertTagStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE3) + ManuStrLen + 1  + VerStrLen + 1 + SerialNumStrLen + 1 + AssertTagStrLen + 1 + 1);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE3);

  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;

  //
  // Manu will be the 1st optional string following the formatted structure.
  //
  SmbiosRecord->Manufacturer = 1;
  SmbiosRecord->Type = GetChassisType ();

  //
  // Version will be the 2nd optional string following the formatted structure.
  //
  SmbiosRecord->Version = 2;

  //
  // SerialNumber will be the 3rd optional string following the formatted structure.
  //
  SmbiosRecord->SerialNumber = 3;

  //
  // AssertTag will be the 4th optional string following the formatted structure.
  //
  SmbiosRecord->AssetTag = 4;
  SmbiosRecord->BootupState = PcdGet8 (PcdSMBIOSChassisBootupState);
  SmbiosRecord->PowerSupplyState = PcdGet8 (PcdSMBIOSChassisPowerSupplyState);
  SmbiosRecord->ThermalState = (UINT8)ForType3InputData->ChassisThermalState;
  SmbiosRecord->SecurityStatus = PcdGet8 (PcdSMBIOSChassisSecurityState);
  *(UINT32 *)&SmbiosRecord->OemDefined = PcdGet32 (PcdSMBIOSChassisOemDefined);
  SmbiosRecord->Height = PcdGet8 (PcdSMBIOSChassisHeight);                
  SmbiosRecord->NumberofPowerCords = PcdGet8 (PcdSMBIOSChassisNumberPowerCords);
  SmbiosRecord->ContainedElementCount = PcdGet8 (PcdSMBIOSChassisElementCount);
  SmbiosRecord->ContainedElementRecordLength = PcdGet8 (PcdSMBIOSChassisElementRecordLength);

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(ManufacturerPtr, OptionalStrStart);
  UnicodeStrToAsciiStr(VersionPtr, OptionalStrStart + ManuStrLen + 1);
  UnicodeStrToAsciiStr(SerialNumberPtr, OptionalStrStart + ManuStrLen + 1 + VerStrLen + 1);
  UnicodeStrToAsciiStr(AssertTagPtr, OptionalStrStart + ManuStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1);

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
  FreePool (VersionPtr);
  FreePool (SerialNumberPtr);
  FreePool (AssertTagPtr);

  return Status;
}
