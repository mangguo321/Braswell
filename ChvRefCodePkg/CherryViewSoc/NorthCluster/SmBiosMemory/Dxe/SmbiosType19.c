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


#define SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES     0xFFFFFFFF


/**
  This function installs SMBIOS Table Type 19 (Physical Memory Array).

  @retval EFI_SUCCESS            if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES   if not able to get resources.

**/
EFI_STATUS
InstallSmbiosType19 (
  VOID
  )
{
  SMBIOS_TABLE_TYPE19       *SmbiosRecord;
  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  UINT64                    TotalMemorySizeInKB = 0;
  UINT64                    TotalMemorySizeInMB = 0;
  UINT16                    Type16Handle = 0;
  UINT8                     ChannelIndex = 0;
  UINT8                     DimmIndex = 0;

  for ( ChannelIndex=0; ChannelIndex < gFspSmbiosMemoryInfo.ChannelCount; ChannelIndex++ ) {
    for( DimmIndex=0; DimmIndex<gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmCount;DimmIndex++){
	  TotalMemorySizeInMB += gFspSmbiosMemoryInfo.ChannelInfo[ChannelIndex].DimmInfo[DimmIndex].SizeInMb;
    }
  }

  SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE19));
  
  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE19);
  
  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;

  TotalMemorySizeInKB = LShiftU64(TotalMemorySizeInMB, 10);

  GetType16Hndl( mSmbios, &Type16Handle);
  SmbiosRecord->MemoryArrayHandle = Type16Handle;

  if (TotalMemorySizeInKB > SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES) {
    SmbiosRecord->StartingAddress = SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES;
    SmbiosRecord->EndingAddress = SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES;
    SmbiosRecord->ExtendedEndingAddress = TotalMemorySizeInKB - 1;
  } else {
    SmbiosRecord->EndingAddress = (UINT32) (TotalMemorySizeInKB - 1);
  }
  SmbiosRecord->PartitionWidth = MAX_SOCKETS;  
  ///
  /// Install SMBIOS Table Type 19
  ///
  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  
  Status = mSmbios->Add(
                      mSmbios,
                      NULL,
                      &SmbiosHandle,
                      (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                      );

  return Status;
}
