/** @file
  Firmware Volume Block Driver for Braswell Platform.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "FvbService.h"

//
// Global variable for this FVB driver  which contains
// the private data of all firmware volume block instances
//
FWB_GLOBAL   mFvbModuleGlobal;

FV_MEMMAP_DEVICE_PATH mFvMemmapDevicePathTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_MEMMAP_DP,
      {
        (UINT8)(sizeof (MEMMAP_DEVICE_PATH)),
        (UINT8)(sizeof (MEMMAP_DEVICE_PATH) >> 8)
      }
    },
    EfiMemoryMappedIO,
    (EFI_PHYSICAL_ADDRESS) 0,
    (EFI_PHYSICAL_ADDRESS) 0,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

FV_PIWG_DEVICE_PATH mFvPIWGDevicePathTemplate = {
  {
    {
      MEDIA_DEVICE_PATH,
      MEDIA_PIWG_FW_VOL_DP,
      {
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH)),
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH) >> 8)
      }
    },
    { 0 }
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

//
// Template structure used when installing FVB protocol
//
EFI_FW_VOL_BLOCK_DEVICE mFvbDeviceTemplate = {
  FVB_DEVICE_SIGNATURE,
  NULL,
  0, // Instance
  {
    FvbProtocolGetAttributes,
    FvbProtocolSetAttributes,
    FvbProtocolGetPhysicalAddress,
    FvbProtocolGetBlockSize,
    FvbProtocolRead,
    FvbProtocolWrite,
    FvbProtocolEraseBlocks,
    NULL
  } // FwVolBlockInstance
};

SPI_DEVICE_PROTOCOL *mSpiDeviceProtocol;

/**
  Get the pointer to EFI_FW_VOL_INSTANCE from the buffer pointed
  by mFvbModuleGlobal.FvInstance based on a index.
  Each EFI_FW_VOL_INSTANCE is  with variable length as
  we have a block map at the end of the EFI_FIRMWARE_VOLUME_HEADER.

  @param[in] Instance The index of the EFI_FW_VOL_INSTANCE.

  @return             A pointer to EFI_FW_VOL_INSTANCE.

**/
EFI_FW_VOL_INSTANCE *
GetFvbInstance (
  IN  UINTN             Instance
  )
{
  EFI_FW_VOL_INSTANCE   *FwhRecord;

  if ( Instance >= mFvbModuleGlobal.NumFv ) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return NULL;
  }

  //
  // Find the right instance of the FVB private data
  //
  FwhRecord = mFvbModuleGlobal.FvInstance;
  while ( Instance > 0 ) {
    FwhRecord = (EFI_FW_VOL_INSTANCE *) ((UINTN)((UINT8 *)FwhRecord) +
                FwhRecord->VolumeHeader.HeaderLength +
                (sizeof (EFI_FW_VOL_INSTANCE) - sizeof (EFI_FIRMWARE_VOLUME_HEADER)));
    Instance --;
  }

  return FwhRecord;
}

/**
  Get the EFI_FVB_ATTRIBUTES_2 of a FV.

  @param[in]  The index of the EFI_FW_VOL_INSTANCE.

  @return     EFI_FVB_ATTRIBUTES_2 of the FV identified by Instance.

**/
STATIC
EFI_FVB_ATTRIBUTES_2
FvbGetVolumeAttributes (
  IN UINTN                                Instance
  )
{
  return GetFvbInstance (Instance)->VolumeHeader.Attributes;
}

/**
  Retrieves the starting address of an LBA in an FV. It also
  return a few other attribut of the FV.

  @param[in]  Instance             The index of the EFI_FW_VOL_INSTANCE.
  @param[in]  Lba                  The logical block address
  @param[out] FlashLinearAddress   Provides the linear address into the flash device.
  @param[out] LbaAddress           On output, contains the physical starting address
                                   of the Lba
  @param[out] LbaLength            On output, contains the length of the block
  @param[out] NumOfBlocks          A pointer to a caller allocated UINTN in which the
                                   number of consecutive blocks starting with Lba is
                                   returned. All blocks in this range have a size of
                                   BlockSize

  @retval   EFI_SUCCESS            Successfully returns
  @retval   EFI_INVALID_PARAMETER  Instance not found

**/
STATIC
EFI_STATUS
FvbGetLbaAddress (
  IN  UINTN                               Instance,
  IN  EFI_LBA                             Lba,
  OUT UINTN                               *FlashLinearAddress,
  OUT UINTN                               *LbaAddress,
  OUT UINTN                               *LbaLength,
  OUT UINTN                               *NumOfBlocks
  )
{
  UINT32                                  NumBlocks;
  UINT32                                  BlockLength;
  UINTN                                   Offset;
  EFI_LBA                                 StartLba;
  EFI_LBA                                 NextLba;
  EFI_FW_VOL_INSTANCE                     *FwhInstance;
  EFI_FV_BLOCK_MAP_ENTRY                  *BlockMap;

  //
  // Find the right instance of the FVB private data
  //
  FwhInstance = GetFvbInstance (Instance);

  StartLba  = 0;
  Offset    = 0;
  BlockMap  = &(FwhInstance->VolumeHeader.BlockMap[0]);

  //
  // Parse the blockmap of the FV to find which map entry the Lba belongs to
  //
  while (TRUE) {
    NumBlocks   = BlockMap->NumBlocks;
    BlockLength = BlockMap->Length;

    if (NumBlocks == 0 || BlockLength == 0) {
      return EFI_INVALID_PARAMETER;
    }

    NextLba = StartLba + NumBlocks;

    //
    // The map entry found
    //
    if (Lba >= StartLba && Lba < NextLba) {
      Offset = Offset + (UINTN)MultU64x32((Lba - StartLba), BlockLength);
      if (FlashLinearAddress) {
        *FlashLinearAddress = FwhInstance->FvFlashLinearAddress + Offset;
      }

      if (LbaAddress) {
        *LbaAddress = FwhInstance->FvBase + Offset;
      }

      if (LbaLength) {
        *LbaLength = BlockLength;
      }

      if (NumOfBlocks) {
        *NumOfBlocks = (UINTN)(NextLba - Lba);
      }
      return EFI_SUCCESS;
    }

    StartLba  = NextLba;
    Offset    = Offset + NumBlocks * BlockLength;
    BlockMap++;
  }
}

/**
  Reads specified number of bytes into a buffer from the specified block

  @param[in]      Instance               The FV instance to be read from
  @param[in]      Lba                    The logical block address to be read from
  @param[in]      BlockOffset            Offset into the block at which to begin reading
  @param[in]      NumBytes               Pointer that on input contains the total size of
                                         the buffer. On output, it contains the total number
                                         of bytes read
  @param[in]      Buffer                 Pointer to a caller allocated buffer that will be
                                         used to hold the data read

  @retval         EFI_SUCCESS            The firmware volume was read successfully and
                                         contents are in Buffer
  @retval         EFI_BAD_BUFFER_SIZE    Read attempted across a LBA boundary. On output,
                                         NumBytes contains the total number of bytes returned
                                         in Buffer
  @retval         EFI_ACCESS_DENIED      The firmware volume is in the ReadDisabled state
  @retval         EFI_DEVICE_ERROR       The block device is not functioning correctly and
                                         could not be read
  @retval         EFI_INVALID_PARAMETER  Instance not found, or NumBytes, Buffer are NULL

**/
STATIC
EFI_STATUS
FvbReadBlock (
  IN UINTN                                Instance,
  IN EFI_LBA                              Lba,
  IN UINTN                                BlockOffset,
  IN OUT UINTN                            *NumBytes,
  IN UINT8                                *Buffer
  )
{
  EFI_FVB_ATTRIBUTES_2                    Attributes;
  UINTN                                   LbaLength;
  EFI_STATUS                              Status;
  EFI_STATUS                              Status1;
  UINTN                                   FlashAddress;

  //
  // Validate input parameters.
  //
  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return (EFI_INVALID_PARAMETER);
  }
  if (*NumBytes == 0) {
    return (EFI_INVALID_PARAMETER);
  }

  //
  // Get information for the specific LBA.
  //
  Status = FvbGetLbaAddress (Instance, Lba, &FlashAddress, NULL, &LbaLength, NULL);
  if (EFI_ERROR (Status)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if operation can happen in the current state.
  //
  Attributes = FvbGetVolumeAttributes (Instance);
  if ((Attributes & EFI_FVB2_READ_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Check to make sure that block information is valid for the current FV and
  // correct it if needed.
  //
  if (BlockOffset > LbaLength) {
   return EFI_INVALID_PARAMETER;
  }
  if (LbaLength < (*NumBytes + BlockOffset)) {
    *NumBytes = (UINT32) (LbaLength - BlockOffset);
    Status = EFI_BAD_BUFFER_SIZE;
  }

  //
  // Perform read.
  //
  Status1 = mSpiDeviceProtocol->SpiRead (FlashAddress + BlockOffset, NumBytes, Buffer);
  if (Status1 == EFI_DEVICE_ERROR) {
    return Status1;
  }

  return Status;
}

/**
  Writes specified number of bytes from the input buffer to the block

  @param[in]  Instance               The FV instance to be written to
  @param[in]  Lba                    The starting logical block index to write to
  @param[in]  BlockOffset            Offset into the block at which to begin writing
  @param[in]  NumBytes               Pointer that on input contains the total size of
                                     the buffer. On output, it contains the total number
                                     of bytes actually written
  @param[in]  Buffer                 Pointer to a caller allocated buffer that contains
                                     the source for the write
  @retval     EFI_SUCCESS            The firmware volume was written successfully
  @retval     EFI_BAD_BUFFER_SIZE    Write attempted across a LBA boundary. On output,
                                     NumBytes contains the total number of bytes
                                     actually written
  @retval     EFI_ACCESS_DENIED      The firmware volume is in the WriteDisabled state
  @retval     EFI_DEVICE_ERROR       The block device is not functioning correctly and
                                     could not be written
  @retval     EFI_INVALID_PARAMETER  Instance not found, or NumBytes, Buffer are NULL

**/
EFI_STATUS
FvbWriteBlock (
  IN UINTN                                Instance,
  IN EFI_LBA                              Lba,
  IN UINTN                                BlockOffset,
  IN OUT UINTN                            *NumBytes,
  IN UINT8                                *Buffer
  )
{
  EFI_FVB_ATTRIBUTES_2                    Attributes;
  UINTN                                   LbaAddress;
  UINTN                                   LbaLength;
  EFI_FW_VOL_INSTANCE                     *FwhInstance;
  EFI_STATUS                              Status;
  EFI_STATUS                              Status1;
  UINTN                                   FlashAddress;

  //
  // Validate input parameters.
  //
  if ((NumBytes == NULL) || (Buffer == NULL)) {
    return (EFI_INVALID_PARAMETER);
  }
  if (*NumBytes == 0) {
    return (EFI_INVALID_PARAMETER);
  }

  //
  // Get the information for the FV specified.
  //
  FwhInstance = GetFvbInstance (Instance);
  Status = FvbGetLbaAddress (Instance, Lba, &FlashAddress, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR (Status)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if the FV is write enabled
  //
  Attributes = FvbGetVolumeAttributes (Instance);
  if ((Attributes & EFI_FVB2_WRITE_STATUS) == 0)  {
    return EFI_ACCESS_DENIED;
  }

  //
  // Perform boundary checks and adjust NumBytes if needed.
  //
  if (BlockOffset > LbaLength) {
    return EFI_INVALID_PARAMETER;
  }
  if ( LbaLength < ( *NumBytes + BlockOffset ) ) {
    *NumBytes = (UINT32) (LbaLength - BlockOffset);
    Status = EFI_BAD_BUFFER_SIZE;
  }

  //
  // Perform the write and flush the cache.
  //
  mSpiDeviceProtocol->SpiLock (FlashAddress, LbaLength, FALSE);
  Status1 = mSpiDeviceProtocol->SpiWrite (FlashAddress + BlockOffset, NumBytes, Buffer);
  mSpiDeviceProtocol->SpiLock (FlashAddress, LbaLength, TRUE);
  WriteBackInvalidateDataCacheRange ((VOID *) (LbaAddress + BlockOffset), *NumBytes);

  //
  // Determine the error to return based on PI spec.
  //
  if (Status1 == EFI_DEVICE_ERROR) {
    return Status1;
  }

  return Status;
}

/**
  Erases and initializes a firmware volume block

  @param[in]    Instance           The FV instance to be erased
  @param[in]    Lba                The logical block index to be erased

  @retval   EFI_SUCCESS            The erase request was successfully completed
  @retval   EFI_ACCESS_DENIED      The firmware volume is in the WriteDisabled state
  @retval   EFI_DEVICE_ERROR       The block device is not functioning correctly and
                                   could not be written. Firmware device may have been
                                   partially erased
  @retval   EFI_INVALID_PARAMETER  Instance not found

**/
EFI_STATUS
FvbEraseBlock (
  IN UINTN                                Instance,
  IN EFI_LBA                              Lba
  )
{
  EFI_FVB_ATTRIBUTES_2                    Attributes;
  UINTN                                   LbaAddress;
  EFI_FW_VOL_INSTANCE                     *FwhInstance;
  UINTN                                   LbaLength;
  EFI_STATUS                              Status;
  EFI_STATUS                              Status1;
  UINTN                                   FlashAddress;

  //
  // Find the right instance of the FVB private data
  //
  FwhInstance = GetFvbInstance (Instance);

  //
  // Check if the FV is write enabled
  //
  Attributes = FvbGetVolumeAttributes (Instance);
  if ((Attributes & EFI_FVB2_WRITE_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Get the starting address of the block for erase.
  //
  Status = FvbGetLbaAddress (Instance, Lba, &FlashAddress, &LbaAddress, &LbaLength, NULL);
  if (EFI_ERROR (Status)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Perform erase.
  //
  mSpiDeviceProtocol->SpiLock (FlashAddress, LbaLength, FALSE);
  Status1 = mSpiDeviceProtocol->SpiErase (FlashAddress, LbaLength);
  mSpiDeviceProtocol->SpiLock (FlashAddress, LbaLength, TRUE);
  WriteBackInvalidateDataCacheRange ((VOID *) LbaAddress, LbaLength);

  //
  // Check to see if the erase was successful.  If not return a device error to
  // meet PI required return values.
  //
  if (Status1 == EFI_DEVICE_ERROR) {
    return Status1;
  }

  return EFI_SUCCESS;
}

/**
  Modifies the current settings of the firmware volume according to the
  input parameter, and returns the new setting of the volume

  @param[in]  Instance              The FV instance whose attributes is going to be
                                    modified
  @param[in]  Attributes            On input, it is a pointer to EFI_FVB_ATTRIBUTES_2
                                    containing the desired firmware volume settings.
                                    On successful return, it contains the new settings
                                    of the firmware volume

  @retval     EFI_SUCCESS           Successfully returns
  @retval     EFI_ACCESS_DENIED     The volume setting is locked and cannot be modified
  @retval     EFI_INVALID_PARAMETER Instance not found, or The attributes requested are
                                    in conflict with the capabilities as declared in the
                                    firmware volume header

**/
STATIC
EFI_STATUS
FvbSetVolumeAttributes (
  IN UINTN                                Instance,
  IN OUT EFI_FVB_ATTRIBUTES_2             *Attributes
  )
{
  EFI_FW_VOL_INSTANCE                       *FwhInstance;
  EFI_FVB_ATTRIBUTES_2                      OldAttributes;
  EFI_FVB_ATTRIBUTES_2                      *AttribPtr;
  EFI_FVB_ATTRIBUTES_2                      UnchangedAttributes;
  UINT32                                    Capabilities;
  UINT32                                    OldStatus, NewStatus;

  //
  // Find the right instance of the FVB private data
  //
  FwhInstance = GetFvbInstance (Instance);

  AttribPtr     = (EFI_FVB_ATTRIBUTES_2 *) &(FwhInstance->VolumeHeader.Attributes);
  OldAttributes = *AttribPtr;
  Capabilities  = OldAttributes & EFI_FVB2_CAPABILITIES;
  OldStatus     = OldAttributes & EFI_FVB2_STATUS;
  NewStatus     = *Attributes & EFI_FVB2_STATUS;

  UnchangedAttributes = EFI_FVB2_READ_DISABLED_CAP  | \
                        EFI_FVB2_READ_ENABLED_CAP   | \
                        EFI_FVB2_WRITE_DISABLED_CAP | \
                        EFI_FVB2_WRITE_ENABLED_CAP  | \
                        EFI_FVB2_LOCK_CAP           | \
                        EFI_FVB2_STICKY_WRITE       | \
                        EFI_FVB2_MEMORY_MAPPED      | \
                        EFI_FVB2_ERASE_POLARITY     | \
                        EFI_FVB2_READ_LOCK_CAP      | \
                        EFI_FVB2_WRITE_LOCK_CAP     | \
                        EFI_FVB2_ALIGNMENT;

  //
  // Some attributes of FV is read only can *not* be set
  //
  if ((OldAttributes & UnchangedAttributes) ^ (*Attributes & UnchangedAttributes)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // If firmware volume is locked, no status bit can be updated
  //
  if ( OldAttributes & EFI_FVB2_LOCK_STATUS ) {
    if ( OldStatus ^ NewStatus ) {
      return EFI_ACCESS_DENIED;
    }
  }

  //
  // Test read disable
  //
  if ((Capabilities & EFI_FVB2_READ_DISABLED_CAP) == 0) {
    if ((NewStatus & EFI_FVB2_READ_STATUS) == 0) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test read enable
  //
  if ((Capabilities & EFI_FVB2_READ_ENABLED_CAP) == 0) {
    if (NewStatus & EFI_FVB2_READ_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test write disable
  //
  if ((Capabilities & EFI_FVB2_WRITE_DISABLED_CAP) == 0) {
    if ((NewStatus & EFI_FVB2_WRITE_STATUS) == 0) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test write enable
  //
  if ((Capabilities & EFI_FVB2_WRITE_ENABLED_CAP) == 0) {
    if (NewStatus & EFI_FVB2_WRITE_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Test lock
  //
  if ((Capabilities & EFI_FVB2_LOCK_CAP) == 0) {
    if (NewStatus & EFI_FVB2_LOCK_STATUS) {
      return EFI_INVALID_PARAMETER;
    }
  }

  *AttribPtr  = (*AttribPtr) & (0xFFFFFFFF & (~EFI_FVB2_STATUS));
  *AttribPtr  = (*AttribPtr) | NewStatus;
  *Attributes = *AttribPtr;

  return EFI_SUCCESS;
}

//
// FVB protocol APIs
//

/**
  Retrieves the physical address of the device.

  @param[in]  This          A pointer to EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL.
  @param[out] Address       Output buffer containing the address.

  retval      EFI_SUCCESS   The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolGetPhysicalAddress (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *This,
  OUT EFI_PHYSICAL_ADDRESS               *Address
  )
{
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;

  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  *Address = GetFvbInstance (FvbDevice->Instance)->FvBase;

  return EFI_SUCCESS;
}

/**
  Retrieve the size of a logical block

  @param[in]  This        Calling context
  @param[in]  Lba         Indicates which block to return the size for.
  @param[out] BlockSize   A pointer to a caller allocated UINTN in which
                          the size of the block is returned
  @param[out] NumOfBlocks A pointer to a caller allocated UINTN in which the
                          number of consecutive blocks starting with Lba is
                          returned. All blocks in this range have a size of
                          BlockSize

  @retval     EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolGetBlockSize (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *This,
  IN  EFI_LBA                            Lba,
  OUT UINTN                              *BlockSize,
  OUT UINTN                              *NumOfBlocks
  )
{
  EFI_FW_VOL_BLOCK_DEVICE                 *FvbDevice;

  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  return FvbGetLbaAddress (
           FvbDevice->Instance,
           Lba,
           NULL,
           NULL,
           BlockSize,
           NumOfBlocks
           );
}

/**
  Retrieves Volume attributes.  No polarity translations are done.

  @param[in]    This        Calling context
  @param[out]   Attributes  Output buffer which contains attributes

  @retval       EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolGetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  OUT EFI_FVB_ATTRIBUTES_2                      *Attributes
  )
{
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;

  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  *Attributes = FvbGetVolumeAttributes (FvbDevice->Instance);

  return EFI_SUCCESS;
}

/**
  Sets Volume attributes. No polarity translations are done.

  @param[in]  This        Calling context
  @param[out] Attributes  Output buffer which contains attributes

  @retval     EFI_SUCCESS The function always return successfully.

**/
EFI_STATUS
EFIAPI
FvbProtocolSetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN OUT EFI_FVB_ATTRIBUTES_2             *Attributes
  )
{
  EFI_STATUS                            Status;
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;

  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  Status = FvbSetVolumeAttributes (FvbDevice->Instance, Attributes);

  return Status;
}

/**
  The EraseBlock() function erases one or more blocks as denoted by the
  variable argument list. The entire parameter list of blocks must be verified
  prior to erasing any blocks.  If a block is requested that does not exist
  within the associated firmware volume (it has a larger index than the last
  block of the firmware volume), the EraseBlock() function must return
  EFI_INVALID_PARAMETER without modifying the contents of the firmware volume.

  @param[in] This           Calling context
  @param[in] ...            Starting LBA followed by Number of Lba to erase.
                            a -1 to terminate the list.

  @retval EFI_SUCCESS       The erase request was successfully completed
  @retval EFI_ACCESS_DENIED The firmware volume is in the WriteDisabled state
  @retval EFI_DEVICE_ERROR  The block device is not functioning correctly and
                            could not be written. Firmware device may have been
                            partially erased

**/
EFI_STATUS
EFIAPI
FvbProtocolEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *This,
  ...
  )
{
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;
  EFI_FW_VOL_INSTANCE                   *FwhInstance;
  UINTN                                 NumOfBlocks;
  VA_LIST                               args;
  EFI_LBA                               StartingLba;
  UINTN                                 NumOfLba;
  EFI_STATUS                            Status;
  EFI_FVB_ATTRIBUTES_2                  Attributes;

  //
  // Initialize data.
  //
  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  FwhInstance = GetFvbInstance (FvbDevice->Instance);
  NumOfBlocks = FwhInstance->NumOfBlocks;

  //
  // Check if this FV can be written to.
  //
  Attributes = FvbGetVolumeAttributes (FvbDevice->Instance);
  if ((Attributes & EFI_FVB2_WRITE_STATUS) == 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Validate LBA information passed in by caller.
  //
  VA_START (args, This);
  do {
    //
    // Check for last entry in variable argument list.
    //
    StartingLba = VA_ARG (args, EFI_LBA);
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      break;
    }

    //
    // Get parameter from stack.
    //
    NumOfLba = VA_ARG (args, UINT32);

    //
    // Check input parameters
    //
    if (NumOfLba == 0) {
      VA_END (args);
      return EFI_INVALID_PARAMETER;
    }
    if ((StartingLba + NumOfLba) > NumOfBlocks) {
      VA_END (args);
      return EFI_INVALID_PARAMETER;
    }
  } while (1);
  VA_END (args);

  //
  // Perform erase operation on all selected LBA.
  //
  VA_START (args, This);
  do {
    //
    // Check for last entry in variable argument list.
    //
    StartingLba = VA_ARG (args, EFI_LBA);
    if (StartingLba == EFI_LBA_LIST_TERMINATOR) {
      break;
    }

    //
    // Get parameter from stack.
    //
    NumOfLba = VA_ARG (args, UINT32);

    //
    // Perform the erase operation for the specific LBA.
    //
    while (NumOfLba > 0) {
      Status = FvbEraseBlock (FvbDevice->Instance, StartingLba);
      if (EFI_ERROR (Status)) {
        VA_END (args);
        return Status;
      }
      StartingLba ++;
      NumOfLba --;
    }
  } while (1);
  VA_END (args);

  return EFI_SUCCESS;
}

/**
  Writes data beginning at Lba:Offset from FV. The write terminates either
  when *NumBytes of data have been written, or when a block boundary is
  reached.  *NumBytes is updated to reflect the actual number of bytes
  written. The write opertion does not include erase. This routine will
  attempt to write only the specified bytes. If the writes do not stick,
  it will return an error.

  @param[in]      This      Calling context
  @param[in]      Lba       Block in which to begin write
  @param[in]      Offset    Offset in the block at which to begin write
  @param[in, out] NumBytes  On input, indicates the requested write size. On
                            output, indicates the actual number of bytes written
  @param[in]      Buffer    Buffer containing source data for the write.

  @retval EFI_SUCCESS           The firmware volume was written successfully
  @retval EFI_BAD_BUFFER_SIZE   Write attempted across a LBA boundary. On output,
                                NumBytes contains the total number of bytes
                                actually written
  @retval EFI_ACCESS_DENIED     The firmware volume is in the WriteDisabled state
  @retval EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                could not be written
  @retval EFI_INVALID_PARAMETER NumBytes or Buffer are NULL

**/
EFI_STATUS
EFIAPI
FvbProtocolWrite (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN EFI_LBA                              Lba,
  IN UINTN                                Offset,
  IN OUT UINTN                            *NumBytes,
  IN UINT8                                *Buffer
  )
{
  EFI_STATUS                            Status;
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;

  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  Status = FvbWriteBlock (FvbDevice->Instance, Lba, Offset, NumBytes, Buffer);

  return Status;
}

/**
  Reads data beginning at Lba:Offset from FV. The Read terminates either
  when *NumBytes of data have been read, or when a block boundary is
  reached.  *NumBytes is updated to reflect the actual number of bytes
  written. The write opertion does not include erase. This routine will
  attempt to write only the specified bytes. If the writes do not stick,
  it will return an error.

  @param[in]      This      Calling context
  @param[in]      Lba       Block in which to begin write
  @param[in]      Offset    Offset in the block at which to begin write
  @param[in, out] NumBytes  On input, indicates the requested write size. On
                            output, indicates the actual number of bytes written
  @param[in]      Buffer    Buffer containing source data for the write.

Returns:
  @retval EFI_SUCCESS           The firmware volume was read successfully and
                                contents are in Buffer
  @retval EFI_BAD_BUFFER_SIZE   Read attempted across a LBA boundary. On output,
                                NumBytes contains the total number of bytes returned
                                in Buffer
  @retval EFI_ACCESS_DENIED     The firmware volume is in the ReadDisabled state
  @retval EFI_DEVICE_ERROR      The block device is not functioning correctly and
                                could not be read
  @retval EFI_INVALID_PARAMETER NumBytes or Buffer are NULL

**/
EFI_STATUS
EFIAPI
FvbProtocolRead (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN EFI_LBA                              Lba,
  IN UINTN                                Offset,
  IN OUT UINTN                            *NumBytes,
  OUT UINT8                               *Buffer
  )
{

  EFI_FW_VOL_BLOCK_DEVICE   *FvbDevice;
  EFI_STATUS                Status;

  FvbDevice = FVB_DEVICE_FROM_THIS (This);
  Status = FvbReadBlock (FvbDevice->Instance, Lba, Offset, NumBytes, Buffer);

  return Status;
}

/**
  Check the integrity of firmware volume header

  @param[in]  FwVolHeader   A pointer to a firmware volume header

  @retval     TRUE          The firmware volume is consistent
  @retval     FALSE         The firmware volume has corrupted.

**/
STATIC
BOOLEAN
IsFvHeaderValid (
  IN       EFI_PHYSICAL_ADDRESS          FvBase,
  IN CONST EFI_FIRMWARE_VOLUME_HEADER    *FwVolHeader
  )
{
  UINT16 Checksum;

  if (FvBase == PcdGet32(PcdFlashNvStorageVariableBase)) {
    if (CompareMem (&FwVolHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid, sizeof(EFI_GUID)) != 0 ) {
      return FALSE;
    }
  } else {
    if (CompareMem (&FwVolHeader->FileSystemGuid, &gEfiFirmwareFileSystem2Guid, sizeof(EFI_GUID)) != 0 ) {
      return FALSE;
    }
  }
  if ((FwVolHeader->Revision != EFI_FVH_REVISION)   ||
       (FwVolHeader->Signature != EFI_FVH_SIGNATURE) ||
       (FwVolHeader->FvLength == ((UINTN) -1))       ||
       ((FwVolHeader->HeaderLength & 0x01 ) !=0)) {
    return FALSE;
  }

  Checksum = CalculateCheckSum16 ((UINT16 *) FwVolHeader, FwVolHeader->HeaderLength);
  if (Checksum != 0) {
    DEBUG (( DEBUG_ERROR,
              "ERROR - Invalid Firmware Volume Header Checksum, change 0x%04x to 0x%04x\r\n",
              FwVolHeader->Checksum,
              (UINT16)( Checksum + FwVolHeader->Checksum )));
    return FALSE;
  }

  return TRUE;
}

/**
  The driver entry point for Firmware Volume Block Driver.

  The function does the necessary initialization work
  Firmware Volume Block Driver.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       This funtion always return EFI_SUCCESS.
                                It will ASSERT on errors.

**/
EFI_STATUS
FvbInitialize (
  )
{
  EFI_FW_VOL_INSTANCE                   *FwhInstance;
  EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  EFI_FV_BLOCK_MAP_ENTRY                *PtrBlockMapEntry;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  UINTN                                 TmpHeaderLength;
  UINTN                                 Idx;
  UINT32                                MaxLbaSize;
  BOOLEAN                               FvHeaderValid;
  UINTN                                 FvFlashLinearAddress;
  EFI_BOOT_MODE                         BootMode;
  UINT32                                Index;
  UINT32                                PlatformFvBaseAddress[5];
  UINT32                                PlatformFvBaseAddressCount;
  UINT32                                PlatformFvLockList[4];
  UINT32                                PlatformFvLockListCount;
  
  //
  // This platform driver knows there are 3 FVs on
  // FD, which are FvRecovery, FvMain and FvNvStorage.
  //
  BootMode = GetBootModeHob ();
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    //
    // On recovery boot, don't report any firmware FV images except payload, because their data can't be trusted.
    //
    PlatformFvBaseAddressCount = 2;
    PlatformFvBaseAddress[0]   = PcdGet32 (PcdFlashNvStorageVariableBase);
    PlatformFvBaseAddress[1]   = PcdGet32 (PcdFlashPayloadBase);
  } else {
    PlatformFvBaseAddressCount = 5;
    PlatformFvBaseAddress[0]   = PcdGet32 (PcdFlashFvMainBase);
    PlatformFvBaseAddress[1]   = PcdGet32 (PcdFlashNvStorageVariableBase);
    PlatformFvBaseAddress[2]   = PcdGet32 (PcdFlashFvRecoveryBase);
    PlatformFvBaseAddress[3]   = PcdGet32 (PcdFlashFvRecovery2Base);
    PlatformFvBaseAddress[4]   = PcdGet32 (PcdFlashPayloadBase);
  }

  //
  // List of FVs that should be write protected on normal boots.
  //
  PlatformFvLockListCount = 4;
  PlatformFvLockList[0]   = PcdGet32 (PcdFlashFvMainBase);
  PlatformFvLockList[1]   = PcdGet32 (PcdFlashFvRecoveryBase);
  PlatformFvLockList[2]   = PcdGet32 (PcdFlashFvRecovery2Base);
  PlatformFvLockList[3]   = PcdGet32 (PcdFlashPayloadBase);

  //
  // Calculate the total size for all firmware volume block instances and
  // allocate a buffer to store them in.
  //
  BufferSize = 0;
  for (Idx = 0; Idx < PlatformFvBaseAddressCount; Idx++) {
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PlatformFvBaseAddress[Idx];
    if (FvHeader == NULL) {
      continue;
    }
    BufferSize +=  (FvHeader->HeaderLength +
                    sizeof (EFI_FW_VOL_INSTANCE) -
                    sizeof (EFI_FIRMWARE_VOLUME_HEADER)
                   );
  }
  mFvbModuleGlobal.FvInstance =  (EFI_FW_VOL_INSTANCE *) AllocateRuntimeZeroPool (BufferSize);
  ASSERT (NULL != mFvbModuleGlobal.FvInstance);

  //
  // Perform other variable initialization.
  //
  MaxLbaSize = 0;
  FwhInstance = mFvbModuleGlobal.FvInstance;
  mFvbModuleGlobal.NumFv   = 0;

  for (Idx = 0; Idx < PlatformFvBaseAddressCount; Idx++) {
  
    if ((BootMode == BOOT_ASSUMING_NO_CONFIGURATION_CHANGES) && PlatformFvBaseAddress[Idx]!= PcdGet32 (PcdFlashNvStorageVariableBase) && PlatformFvBaseAddress[Idx]!= PcdGet32 (PcdFlashPayloadBase)) {
      continue;
    }
    //
    // Get base address information.
    //
    BaseAddress = PlatformFvBaseAddress[Idx];
    FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;
    if (FwVolHeader == NULL) {
      continue;
    }
    //
    // Find the flash linear address of the current FV.
    //
    FvFlashLinearAddress = (UINTN) FLASH_LINEAR_ADDRESS(BaseAddress);

    if (!IsFvHeaderValid (BaseAddress, FwVolHeader)) {
      FvHeaderValid = FALSE;

      //
      // If not valid, get FvbInfo from the information carried in
      // FVB driver.
      //
      DEBUG ((EFI_D_ERROR, "Fvb: FV header @ 0x%lx invalid\n", BaseAddress));
      Status = GetFvbInfo (BaseAddress, &FwVolHeader);
      ASSERT_EFI_ERROR(Status);

      //
      //  Write back a healthy FV header.
      //
      DEBUG ((EFI_D_INFO, "FwBlockService.c: Writing back healthy FV header\n"));
      mSpiDeviceProtocol->SpiLock (FvFlashLinearAddress, FwVolHeader->BlockMap->Length, FALSE);

      Status = mSpiDeviceProtocol->SpiErase (FvFlashLinearAddress, FwVolHeader->BlockMap->Length);

      TmpHeaderLength = (UINTN) FwVolHeader->HeaderLength;
      Status = mSpiDeviceProtocol->SpiWrite (
                                     FvFlashLinearAddress,
                                     &TmpHeaderLength,
                                     (UINT8 *) FwVolHeader
                                     );

      mSpiDeviceProtocol->SpiLock (FvFlashLinearAddress, FwVolHeader->BlockMap->Length, TRUE);

      WriteBackInvalidateDataCacheRange (
        (VOID *) (UINTN) BaseAddress,
        FwVolHeader->BlockMap->Length
        );

    }

    //
    // Copy FV header into local storage and assign base address.
    //
    CopyMem (&(FwhInstance->VolumeHeader), FwVolHeader, FwVolHeader->HeaderLength);
    FwVolHeader = &(FwhInstance->VolumeHeader);
    FwhInstance->FvBase = (UINTN)BaseAddress;
    FwhInstance->FvFlashLinearAddress = FvFlashLinearAddress;

    //
    // In some cases the Recovery and Main FVs should be considered locked from
    // write access by this protocol.  Only in the case of flash updates and
    // configuration mode should they be left unlocked.
    //
    if (BootMode != BOOT_IN_RECOVERY_MODE &&
        BootMode != BOOT_ON_FLASH_UPDATE) {
      for (Index = 0; Index < PlatformFvLockListCount; Index++) {
        if (FwhInstance->FvBase == PlatformFvLockList[Index]) {
          //
          // For all FVs in the lock list we need to clear the write status bit
          // and lock write status updates.  This will make sure this protocol
          // will not attempt to write to the FV.
          //
          FwhInstance->VolumeHeader.Attributes &= (UINT64) ~EFI_FVB2_WRITE_STATUS;
          FwhInstance->VolumeHeader.Attributes |= (EFI_FVB2_LOCK_STATUS | EFI_FVB2_WRITE_LOCK_STATUS);
        }
      }
    }

    //
    // Process the block map for each FV
    //
    FwhInstance->NumOfBlocks   = 0;
    for (PtrBlockMapEntry = FwVolHeader->BlockMap;
         PtrBlockMapEntry->NumBlocks != 0;
         PtrBlockMapEntry++) {
      //
      // Get the maximum size of a block.
      //
      if (MaxLbaSize < PtrBlockMapEntry->Length) {
        MaxLbaSize  = PtrBlockMapEntry->Length;
      }
      FwhInstance->NumOfBlocks += PtrBlockMapEntry->NumBlocks;
    }

    //
    // Add a FVB Protocol Instance
    //
    mFvbModuleGlobal.NumFv++;
    InstallFvbProtocol (FwhInstance, mFvbModuleGlobal.NumFv - 1);

    //
    // Move on to the next FwhInstance
    //
    FwhInstance = (EFI_FW_VOL_INSTANCE *) ((UINTN)((UINT8 *)FwhInstance) +
                                          FwVolHeader->HeaderLength +
                                          (sizeof (EFI_FW_VOL_INSTANCE) - sizeof (EFI_FIRMWARE_VOLUME_HEADER)));
  }

  if ((PcdGet32 (PcdFlashNvStorageFtwWorkingBase) == 0) || (PcdGet32 (PcdFlashNvStorageFtwSpareBase) == 0)) {
    return EFI_SUCCESS;
  }
  
  //
  // Install FVB protocols for FTW spare space and FTW working space.
  // These is no FV header for these 2 spaces.
  //
  mFvbModuleGlobal.FvInstance = (EFI_FW_VOL_INSTANCE *) ReallocateRuntimePool (
                                  BufferSize, 
                                  BufferSize + (sizeof (EFI_FW_VOL_INSTANCE) + sizeof (EFI_FV_BLOCK_MAP_ENTRY)) * 2, 
                                  mFvbModuleGlobal.FvInstance
                                  );
  ASSERT (NULL != mFvbModuleGlobal.FvInstance);
  PlatformFvBaseAddress[0] = PcdGet32 (PcdFlashNvStorageFtwWorkingBase);
  PlatformFvBaseAddress[1] = PcdGet32 (PcdFlashNvStorageFtwSpareBase);
  
  for (Idx = 0; Idx < 2; Idx++) {
    BaseAddress = PlatformFvBaseAddress[Idx];
    Status = GetFtwFvbInfo (BaseAddress, &FwVolHeader);
    ASSERT_EFI_ERROR(Status);
  
    //
    // Copy FV header into local storage and assign base address.
    //
    mFvbModuleGlobal.NumFv++;
    FwhInstance = GetFvbInstance (mFvbModuleGlobal.NumFv - 1);
    CopyMem (&(FwhInstance->VolumeHeader), FwVolHeader, FwVolHeader->HeaderLength);
    FwVolHeader = &(FwhInstance->VolumeHeader);

    //
    // Process the block map for each FV
    //
    FwhInstance->NumOfBlocks   = 0;
    for (PtrBlockMapEntry = FwVolHeader->BlockMap;
         PtrBlockMapEntry->NumBlocks != 0;
         PtrBlockMapEntry++) {
      FwhInstance->NumOfBlocks += PtrBlockMapEntry->NumBlocks;
    }

    FwhInstance->FvBase = (UINTN)BaseAddress;
    FwhInstance->FvFlashLinearAddress = (UINTN) FLASH_LINEAR_ADDRESS(BaseAddress);

    InstallFvbProtocol (FwhInstance, mFvbModuleGlobal.NumFv - 1);
  }    
  
  return EFI_SUCCESS;
}

