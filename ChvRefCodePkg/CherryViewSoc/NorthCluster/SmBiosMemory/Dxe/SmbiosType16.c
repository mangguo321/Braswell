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


/**
  This function installs SMBIOS Table Type 16 (Physical Memory Array).

  @retval EFI_SUCCESS            if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES   if not able to get resources.

**/
EFI_STATUS
InstallSmbiosType16 (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_SMBIOS_HANDLE               SmbiosHandle;
  UINT8                           NumSlots;
  SMBIOS_TABLE_TYPE16             *SmbiosRecord;

  NumSlots = (UINT8)(MAX_SOCKETS);

  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE16));
  ZeroMem(SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE16));
  
  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE16);
  
  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0; 

  ///
  /// Update ECC Support
  ///
  SmbiosRecord->MaximumCapacity = MAX_MEM_CAPACITY;
  SmbiosRecord->MemoryErrorCorrection = gFspSmbiosMemoryInfo.ErrorCorrectionType;
  SmbiosRecord->Location = MemoryArrayLocationSystemBoard;
  SmbiosRecord->NumberOfMemoryDevices = NumSlots;
  SmbiosRecord->Use = MemoryArrayUseSystemMemory;
  SmbiosRecord->MemoryErrorInformationHandle = 0xFFFE;

  ///
  /// Install SMBIOS Table Type 16
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
