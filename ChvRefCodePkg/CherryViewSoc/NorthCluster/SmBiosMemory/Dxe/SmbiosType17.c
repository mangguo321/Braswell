/** @file
  This driver will determine memory configuration information from the chipset
  and memory and create SMBIOS memory structures appropriately.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SmbiosMemory.h"



#define EfiMemoryTypeDdr3  0x18
#define SMBIOS_TYPE17_USE_EXTENDED_SIZE          0x7FFF

typedef struct {
  EFI_PHYSICAL_ADDRESS        MemoryArrayStartAddress;
  EFI_PHYSICAL_ADDRESS        MemoryArrayEndAddress;
  EFI_INTER_LINK_DATA         PhysicalMemoryArrayLink;
  UINT16                      MemoryArrayPartitionWidth;
} EFI_MEMORY_ARRAY_START_ADDRESS;

EFI_MEMORY_ARRAY_LINK_DATA ForType17InputData = {
	STRING_TOKEN (STR_MEMORY_SUBCLASS_DEVICE_LOCATOR_0),   // Memory Device locator
	STRING_TOKEN (STR_MEMORY_SUBCLASS_BANK_LOCATOR_0),     // Memory Bank Locator
	STRING_TOKEN (STR_MEMORY_SUBCLASS_MANUFACTURER),       // Memory manufacturer
	STRING_TOKEN (STR_MEMORY_SUBCLASS_SERIAL_NUMBER),      // Memory serial Number
	STRING_TOKEN (STR_MEMORY_SUBCLASS_ASSET_TAG),          // Memory Asset Tag
	STRING_TOKEN (STR_MEMORY_SUBCLASS_PART_NUMBER),        // Memory Part Number
	0,											           // Memory Array Link
	0, 					                                   // Memory SubArray link
	0, 					                                   // UINT16 MemoryTotalWidth
	0, 					                                   // UINT16 MemoryDatawidth
	0,										               // Memory Device Size
	EfiMemoryFormFactorDip, 	                           // Memory Form Factor
    2,                                                     // UINT8 Memory Device type
	EfiMemoryTypeDram, 			                           // Memory Type
    0,                                                     // Memory Type Detail
    0,										               // Memory Speed
	0 					                                   // Memory State

};

MEMORY_MODULE_MANUFACTURE_LIST MemoryModuleManufactureList[] = {
  {0, 0x2c, L"Micron"},
  {0, 0xce, L"Samsung"},
  {1, 0x4f, L"Transcend"},
  {1, 0x98, L"Kingston"},
  {0xff, 0xff, 0}
};


/**
  This function makes boot time changes to the contents of the
  MiscBiosVendor (Type 0).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
VOID
GetType16Hndl (
  IN  EFI_SMBIOS_PROTOCOL      *Smbios,
  OUT  EFI_SMBIOS_HANDLE       *Handle
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  EFI_SMBIOS_TYPE            RecordType;
  EFI_SMBIOS_TABLE_HEADER    *Buffer;

  *Handle = 0;
  RecordType = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;

  Status = Smbios->GetNext (
                     Smbios,
                     Handle,
                     &RecordType,
                     &Buffer,
                     NULL
                     );
  if (!EFI_ERROR(Status)) {
    return;
  }
  *Handle = 0xFFFF;
}

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
  )
{
  CHAR8                           *OptionalStrStart;
  UINTN                           MemDeviceStrLen;
  UINTN                           MemBankLocatorStrLen;
  UINTN                           MemManufacturerStrLen;
  UINTN                           MemSerialNumberStrLen;
  UINTN                           MemAssetTagStrLen;
  UINTN                           MemPartNumberStrLen;
  CHAR16                          MemAssetTagTemp[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          MemManufacturerTemp[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          MemSerialNumberTemp[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          MemPartNumberTemp[SMBIOS_STRING_MAX_LENGTH];
  CHAR16                          *MemDevice = NULL;
  CHAR16                          *MemBankLocator = NULL;
  CHAR16                          *MemManufacturer = NULL;
  CHAR16                          *MemSerialNumber = NULL;
  CHAR16                          *MemAssetTag = NULL;
  CHAR16                          *MemPartNumber = NULL;
  EFI_STATUS                      Status = EFI_SUCCESS;
  STRING_REF                      TokenToGet;
  STRING_REF                      TokenToUpdate;
  SMBIOS_TABLE_TYPE17             *SmbiosRecord;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  UINT16                          DdrFreq=0;
  UINT16                          Type16Handle=0;
  UINT8                           MemoryType;
  UINT64                          DimmMemorySize;
  UINT64                          DimmMemorySizeInMB;
  UINT8                           NumSlots;
#if 1
  CHAR16                          StringBuffer2[64];
#endif
  EFI_STRING                      StringBuffer;
  UINTN                           StringBufferSize;
  DDR_ROW_CONFIG                  RowConfArray[MAX_SOCKETS];
  UINT8                           DimmIndex;
  EFI_PHYSICAL_ADDRESS            BaseAddress;
#if 1
  UINTN                           IdListIndex;
  UINTN                           Index;
#endif
  UINT8                           SerialNumStart;
  UINT8                           PartNumber;
  UINT16                          PrimaryBusBandwidth;
  UINT8                           ChannelIndex = 0;
  UINT64                          ChannelTotalSizeInMb = 0;
#if 1
  UINT16                          ManufactureId = 0;
#endif
  BOOLEAN                         ManufactureIdFound = FALSE;

  STRING_REF                      DevLocator[] = {
    STRING_TOKEN(STR_MEMORY_SUBCLASS_DEVICE_LOCATOR_0), STRING_TOKEN(STR_MEMORY_SUBCLASS_DEVICE_LOCATOR_1)
  };
  STRING_REF                      BankLocator[] = {
    STRING_TOKEN(STR_MEMORY_SUBCLASS_BANK_LOCATOR_0), STRING_TOKEN(STR_MEMORY_SUBCLASS_BANK_LOCATOR_1)
  };

  NumSlots = (UINT8)(MAX_SOCKETS);
  
  StringBufferSize = (sizeof (CHAR16)) * 100;
  StringBuffer = AllocateZeroPool(StringBufferSize);
  if (StringBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BaseAddress = 0;

  for ( ChannelIndex=0; ChannelIndex < gFspSmbiosMemoryInfo.ChannelCount; ChannelIndex++ ) {
    ChannelTotalSizeInMb = 0;
    for( DimmIndex=0; DimmIndex<gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmCount;DimmIndex++){
	  ChannelTotalSizeInMb += gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SizeInMb;
    }
    RowConfArray[ChannelIndex].BaseAddress = BaseAddress;
    RowConfArray[ChannelIndex].RowLength = LShiftU64(ChannelTotalSizeInMb, 20);
    BaseAddress += RowConfArray[ChannelIndex].RowLength;
  }

  //
  // Data Width
  //
  PrimaryBusBandwidth = gFspSmbiosMemoryInfo.DataWidth;
  //
  // Memory Freq
  //
  DdrFreq = gFspSmbiosMemoryInfo.MemoryFrequencyInMHz;

  DimmMemorySizeInMB = 0;
  //
  // Memory Type
  //
  MemoryType = gFspSmbiosMemoryInfo.MemoryType;
  
  SerialNumStart  = 122;
  PartNumber    = 128;


  for (ChannelIndex = 0; ChannelIndex < gFspSmbiosMemoryInfo.ChannelCount; ChannelIndex++) {
    ChannelTotalSizeInMb = 0;
    for( DimmIndex=0; DimmIndex<gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmCount;DimmIndex++){
	  ChannelTotalSizeInMb += gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SizeInMb;
    }

    //
    // Memory Device Locator
    //
    TokenToGet = DevLocator[ChannelIndex];
    MemDevice = SmbiosMiscGetString (TokenToGet);
    MemDeviceStrLen = StrLen(MemDevice);
    //
    // Memory Bank Locator
    //
    TokenToGet = BankLocator[ChannelIndex];
    MemBankLocator = SmbiosMiscGetString (TokenToGet);
    MemBankLocatorStrLen = StrLen(MemBankLocator);
    //
    // Update strings from PCD: Memory Asset Tag Number
    //
    AsciiStrToUnicodeStr ((const CHAR8*)PcdGetPtr(PcdSMBIOSMemoryAssetTag), MemAssetTagTemp);
    if (StrLen (MemAssetTagTemp) > 0) {     
      TokenToUpdate = STRING_TOKEN (STR_MEMORY_SUBCLASS_ASSET_TAG);
      HiiSetString (StringPackHandle, TokenToUpdate, MemAssetTagTemp, NULL);
    }
    TokenToGet = STRING_TOKEN (STR_MEMORY_SUBCLASS_ASSET_TAG);
    MemAssetTag = SmbiosMiscGetString (TokenToGet);
    MemAssetTagStrLen = StrLen(MemAssetTag);

    if (ChannelTotalSizeInMb){
      //
      // show known manufature name,show ID if the RAM module is unknow
      //
      StrCpy (StringBuffer, L"");
      ManufactureIdFound = FALSE;
		
#if 1
      //When FSP binary updated to higher than VpdHeaderV1.1.4.0.h, the MfgId can be get:
      ManufactureId = gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].MfgId;
        
      //
      // converter memory manufacture ID to string
      //
      for (IdListIndex=0; MemoryModuleManufactureList[IdListIndex].Index!=0xff; IdListIndex++) {
        if(  MemoryModuleManufactureList[IdListIndex].ManufactureId == ManufactureId) {
          StrCpy (StringBuffer, MemoryModuleManufactureList[IdListIndex].ManufactureName);
          ManufactureIdFound = TRUE;
          if (!(*StringBuffer))  {
            StrCpy (StringBuffer2, L"");
            StrCat (StringBuffer, StringBuffer2);
          }
          break;
        }
      }
#endif
      if(!ManufactureIdFound){
        StrCpy (StringBuffer, L"");
      }

      StrCpy(MemManufacturerTemp, StringBuffer);
      MemManufacturer = MemManufacturerTemp;
      MemManufacturerStrLen = StrLen(StringBuffer);		  
		  
      StrCpy(StringBuffer, L"");
      StrCpy(MemSerialNumberTemp, StringBuffer);
      MemSerialNumber = MemSerialNumberTemp;
      MemSerialNumberStrLen = StrLen(StringBuffer);

      StrCpy(StringBuffer, L"");
#if 1
      //When FSP binary updated to higher than VpdHeaderV1.1.4.0.h, the ModulePartNum can be get:
      for (Index = 0; Index < 20; Index++) {
        UnicodeSPrint(
          StringBuffer2,
          4,
          L"%c",
          gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].ModulePartNum[Index]);

        DEBUG ((EFI_D_ERROR, "MemPartNumber %s \n", StringBuffer2));
        StrCat (StringBuffer, StringBuffer2);
      }
#endif
      StrCpy(MemPartNumberTemp, StringBuffer);
      MemPartNumber = MemPartNumberTemp;
      MemPartNumberStrLen = StrLen(StringBuffer);

      //
      // Two zeros following the last string.
      //
      SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE17) +  MemDeviceStrLen + 1 + MemBankLocatorStrLen + 1 + MemManufacturerStrLen + 1 + MemSerialNumberStrLen + 1 + MemAssetTagStrLen+1 + MemPartNumberStrLen + 1 + 1);
	  
      SmbiosRecord->TotalWidth = PrimaryBusBandwidth;
      SmbiosRecord->DataWidth = PrimaryBusBandwidth;
		  
      if (ChannelTotalSizeInMb < SMBIOS_TYPE17_USE_EXTENDED_SIZE) {
        SmbiosRecord->Size = (UINT16) ChannelTotalSizeInMb;
        SmbiosRecord->ExtendedSize = 0;
      } else {
        SmbiosRecord->Size = SMBIOS_TYPE17_USE_EXTENDED_SIZE;
        SmbiosRecord->ExtendedSize = (UINT16) ChannelTotalSizeInMb;
      }

      SmbiosRecord->Speed = DdrFreq;
      SmbiosRecord->ConfiguredMemoryClockSpeed = DdrFreq;
      SmbiosRecord->FormFactor = EfiMemoryFormFactorDimm;

      SmbiosRecord->DeviceSet =(UINT8) ForType17InputData.MemoryDeviceSet;
      SmbiosRecord->DeviceLocator= 1;
      SmbiosRecord->BankLocator = 2;
  
      SmbiosRecord->Manufacturer = 3;
      SmbiosRecord->SerialNumber= 4;
      SmbiosRecord->AssetTag= 5;
      SmbiosRecord->PartNumber= 6;
      SmbiosRecord->Attributes = (UINT8) ForType17InputData.MemoryState;
      SmbiosRecord->MemoryType = MemoryType;

      OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
      UnicodeStrToAsciiStr(MemDevice, OptionalStrStart);
      UnicodeStrToAsciiStr(MemBankLocator, OptionalStrStart + MemDeviceStrLen + 1);
      UnicodeStrToAsciiStr(MemManufacturer, OptionalStrStart + MemDeviceStrLen + 1 + MemBankLocatorStrLen + 1);
      UnicodeStrToAsciiStr(MemSerialNumber, OptionalStrStart + MemDeviceStrLen + 1 + MemBankLocatorStrLen + 1 + MemManufacturerStrLen + 1);
      UnicodeStrToAsciiStr(MemAssetTag, OptionalStrStart + MemDeviceStrLen + 1 + MemBankLocatorStrLen + 1 + MemManufacturerStrLen + 1 + MemSerialNumberStrLen + 1);
      UnicodeStrToAsciiStr(MemPartNumber, OptionalStrStart + MemDeviceStrLen + 1 + MemBankLocatorStrLen + 1 + MemManufacturerStrLen + 1 + MemSerialNumberStrLen + 1+ MemAssetTagStrLen+1 );
    } else {		
      SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE17));
      DimmMemorySize = 0;
      SmbiosRecord->Speed		= 0;
      SmbiosRecord->Size	= 0;
      SmbiosRecord->MemoryType = EfiMemoryTypeUnknown;		
    }
    
    SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
    SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE17);

    //
    // Make handle chosen by smbios protocol.add automatically.
    //
    SmbiosRecord->Hdr.Handle = 0;

    //
    // Memory Array Handle will be the 3rd optional string following the formatted structure.
    //
    GetType16Hndl( mSmbios, &Type16Handle);
    SmbiosRecord->MemoryArrayHandle = Type16Handle; 

    //
    // Now we have got the full smbios record, call smbios protocol to add this record.
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (
                        mSmbios,
                        NULL,
                        &SmbiosHandle,
                        (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                        );
    FreePool(SmbiosRecord);

    //
    // Free the memory allocated before 
    //
    FreePool (MemDevice);
    FreePool (MemBankLocator);
    FreePool (MemAssetTag);
  }
  FreePool(StringBuffer);
  return Status;
}
