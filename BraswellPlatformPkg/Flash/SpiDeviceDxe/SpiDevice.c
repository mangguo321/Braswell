/** @file
  SPI Device driver for Braswell Platform.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Protocol/Spi.h>
#include <Protocol/SpiDevice.h>
#include "SpiDevice.h"

EFI_SPI_PROTOCOL  *mSpiProtocol;

UINTN mNvStorageBase = 0;

EFI_STATUS
EFIAPI
SpiRead (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       *Size,
  OUT     UINT8       *Buffer
  )
{
  EFI_STATUS  Status;
  VOID        *BiosMmioAddress;
  UINTN       RegionOffset;
  UINTN       Length;
  UINTN       RemainingBytes;

  //
  // Validate parameters.
  //
  if (Size == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (SpiOffset + *Size > PcdGet32 (PcdFlashAreaSize)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check to see if the read is taking place in a memory mapped part of the flash.
  // Some flash regions may not be mapped for runtime access by the OS and must
  // be accessed through the controller and not MMIO.
  //
  if (ReadUsingMmio (SpiOffset)) {
    //
    // Convert BIOS region offset into an actual memory address.
    //
    BiosMmioAddress = (VOID*) (SpiOffset + PcdGet32 (PcdFlashAreaBaseAddress));

    //
    // Do memory copy instead of using SPI controller.
    //
    CopyMem ((VOID*) Buffer, BiosMmioAddress, *Size);
  } else if ((SpiOffset >= VN_STORAGE_REGION_FLASH_OFFSET) && (SpiOffset < (VN_STORAGE_REGION_FLASH_OFFSET + PcdGet32 (PcdFlashAreaSize)))) {
    //
    // Convert the offset into a memory address into the NV Storage region.  At
    // runtime this is the only region of the flash that is mapped for runtime
    // access.  Prior to runtime the preceding case will cover MMIO flash access.
    //
    BiosMmioAddress = (VOID*) ((SpiOffset - VN_STORAGE_REGION_FLASH_OFFSET) + mNvStorageBase);

    //
    // Do memory copy instead of using SPI controller.
    //
    CopyMem ((VOID*) Buffer, BiosMmioAddress, *Size);
  } else {
    Status = EFI_SUCCESS;
    RemainingBytes = *Size;
    RegionOffset = SpiOffset;
    while (RemainingBytes > 0) {
      if (RemainingBytes > SIZE_4KB) {
        Length = SIZE_4KB;
      } else {
        Length = RemainingBytes;
      }
      Status = mSpiProtocol->Execute (
                               mSpiProtocol,
                               SPI_READ,
                               0,
                               TRUE,
                               TRUE,
                               FALSE,
                               (UINT32) RegionOffset,
                               (UINT32) Length,
                               Buffer,
                               EnumSpiRegionAll
                               );
      if (EFI_ERROR (Status)) {
        DEBUG((EFI_D_ERROR, "Failed to read SPI region.\n"));
        break;
      }
      RemainingBytes -= Length;
      RegionOffset += Length;
      Buffer += Length;
    }  
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SpiWrite (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       *Size,
  IN      UINT8       *Buffer
  )
{
  EFI_STATUS  Status;
  UINTN       RegionOffset;
  UINTN       Length;
  UINTN       RemainingBytes;

  //
  // Validate the input parameters
  //
  if (Size == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (SpiOffset + *Size > PcdGet32 (PcdFlashAreaSize)) {
    return EFI_INVALID_PARAMETER;
  }
  
  Status = EFI_SUCCESS;
  RemainingBytes = *Size;
  RegionOffset = SpiOffset;
  
  while (RemainingBytes > 0) {
    if (RemainingBytes > SIZE_4KB) {
      Length = SIZE_4KB;
    } else {
      Length = RemainingBytes;
    }
    Status = mSpiProtocol->Execute (
                             mSpiProtocol,
                             SPI_PROG,
                             SPI_WREN,
                             TRUE,
                             TRUE,
                             TRUE,
                             (UINT32) RegionOffset,
                             (UINT32) Length,
                             Buffer,
                             EnumSpiRegionAll
                             );
    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR, "Failed to write SPI region.\n"));
      break;
    }
    RemainingBytes -= Length;
    RegionOffset += Length;
    Buffer += Length;
  }
  
  return Status;
}

EFI_STATUS
EFIAPI
SpiErase (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       Size
  )
{
  EFI_STATUS  Status;
  UINTN       RegionOffset;
  UINTN       BytesRemaining;

  //
  // Validate the input parameters
  //
  Status = EFI_INVALID_PARAMETER;
  if (SpiOffset + Size > PcdGet32 (PcdFlashAreaSize)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Force the minimal alignment of 4k.
  //
  BytesRemaining = Size;
  RegionOffset = SpiOffset;
  if (RegionOffset & (SIZE_4KB - 1)) {
    DEBUG((EFI_D_INFO, "Forcing SPI Device Erase alignment to a 4k base.\n"));
    BytesRemaining += (RegionOffset & (SIZE_4KB - 1));
    RegionOffset = RegionOffset & (SIZE_4KB - 1);
  }

  //
  // Perform as many erase operations as needed to erase requested region.
  //
  while (BytesRemaining > 0) {
    Status = mSpiProtocol->Execute (
                             mSpiProtocol,
                             SPI_SERASE,
                             SPI_WREN,
                             FALSE,
                             TRUE,
                             FALSE,
                             (UINT32) RegionOffset,
                             0,
                             NULL,
                             EnumSpiRegionAll
                             );
    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR, "Failed to erase SPI region.\n"));
      break;
    }    

    //
    // Update the number of bytes left to erase.
    //
    BytesRemaining -= SIZE_4KB;
    RegionOffset += SIZE_4KB;
  }

  return Status;
}

EFI_STATUS
EFIAPI
SpiLock (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       Size,
  IN      BOOLEAN     Lock
  )
{
  //
  // Block/Sector locking is not supported in this implementation.  Use SpiSetRange
  // and SpiLockRanges to protect areas of the flash.
  //
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
SpiSetRange (
  IN      UINTN       SpiOffset,
  IN      UINTN       Size,
  IN      BOOLEAN     ReadLock,
  IN      BOOLEAN     WriteLock
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
SpiLockRanges (
  )
{
  //
  // Call lock on the SPI interface.  This will lock down further configuration
  // changes in the SPI controller.
  //
  return mSpiProtocol->Lock (mSpiProtocol);
}

/**
  Get the JEDED ID from the SPI flash part.

  @param[in]  Context             Pointer to a context data structure
                                  needed by the SPI controller driver
  @param[in]  Description         Description of the flash device
  @param[in]  BufferLength        Length of the JedecId buffer
  @param[out] JedecId             Pointer to a buffer to fill with
                                  the JEDEC ID value

  @retval  EFI_SUCCESS            The JEDEC ID value is in the buffer
  @retval  EFI_INVALID_PARAMETER  JedecId is NULL
  @retval  EFI_INVALID_PARAMETER  Description is NULL
  @retval  EFI_INVALID_PARAMETER  Too few opcode entries
  @retval  EFI_INVALID_PARAMETER  JEDEC ID response buffer too small
  @retval  EFI_UNSUPPORTED        JEDEC ID opcode not found

**/
EFI_STATUS
EFIAPI
JedecIdRead (
  IN       VOID                    *Context,
  IN CONST FLASH_PART_DESCRIPTION  *Description,
  IN       UINTN                   BufferLength,
  OUT      UINT8                   *JedecId
  )
{
  //
  //  Validate parameters.
  //
  if ((JedecId == NULL)
    || (Description == NULL)
    || (BufferLength < Description->JededIdResponseLengthInBytes)) {
    return EFI_INVALID_PARAMETER;
  }
  
  return mSpiProtocol->ReadId (mSpiProtocol, 0, JedecId);
}

/**
  Determine the flash size and description

  @param[in]  PerformJedecIdOperation  Callback routine to initiate
                                       the JEDEC ID operation using
                                       the SPI controller to identify
                                       the flash part.
  @param[in]  Context                  Pointer to a context structure to pass
                                       to PerformJedecIdOperation
  @param[out] FlashDescription         Pointer to a buffer to receive a
                                       pointer to a FLASH_PART_DESCRIPTION
                                       data structure containing the flash
                                       part information.

  @return                              This routine returns the size of the flash part if it is
                                       supported.  Zero is returned if the flash part is not
                                       supported.

**/
UINT64
EFIAPI
FindFlashSupport (
  IN        PERFORM_JEDEC_ID_OPERATION  PerformJedecIdOperation,
  IN        VOID                        *Context,
  OUT CONST FLASH_PART_DESCRIPTION      **FlashDescription
  )
{
  UINTN                         BufferLength;
  CONST FLASH_PART_DESCRIPTION  *Description;
  UINT64                        FlashSize;
  EFI_HANDLE                    *HandleArray;
  UINTN                         HandleCount;
  UINTN                         HandleIndex;
  UINT8                         *JedecId;
  UINT32                        MaxPriority;
  UINT32                        Priority;
  SPI_FLASH_PART_PROTOCOL       *Protocol;
  SPI_FLASH_PART_PROTOCOL       **SpiFlashPartProtocol;
  EFI_STATUS                    Status;

  //
  //  Assume failure
  //
  FlashSize = 0;
  HandleArray = NULL;
  JedecId = NULL;
  SpiFlashPartProtocol = NULL;

  //
  //  Locate handles containing SPI_FLASH_PART_PROTOCOLS
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gSpiFlashPartProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleArray
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - Failed to locate SPI_FLASH_PART_PROTOCOL, Status: %r\r\n", Status));
  } else {
    //
    // Allocate and fill in the protocol array
    //
    DEBUG ((DEBUG_INFO, "%d SPI flash part descriptions found\r\n", HandleCount));
    SpiFlashPartProtocol = AllocatePool (HandleCount * sizeof (*SpiFlashPartProtocol));
    if (SpiFlashPartProtocol == NULL) {
      DEBUG ((DEBUG_ERROR, "ERROR - Failed to allocate SpiFlashDataProtocol buffer\r\n"));
    } else {
      for (HandleIndex = 0; HandleCount > HandleIndex; HandleIndex++) {
        Status = gBS->OpenProtocol (
                        HandleArray [HandleIndex],
                        &gSpiFlashPartProtocolGuid,
                        (VOID **) &SpiFlashPartProtocol [HandleIndex],
                        NULL,
                        NULL,
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL
                        );
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "ERROR - Failed to open SPI_FLASH_DATA_PROTOCOL, Status: %r\r\n", Status));
          break;
        }
      }
      if (!EFI_ERROR (Status)) {
        //
        //  Allocate the JEDEC ID buffer
        //
        BufferLength = 0;
        for (HandleIndex = 0; HandleCount > HandleIndex; HandleIndex++) {
          //
          //  Get the JEDEC ID opcode description
          //
          Protocol = SpiFlashPartProtocol [HandleIndex];
          Description = Protocol->GetFlashDescription (
                                    Protocol,
                                    NULL,
                                    NULL
                                    );
          if (BufferLength < Description->JededIdResponseLengthInBytes) {
            BufferLength = Description->JededIdResponseLengthInBytes;
          }
        }
        JedecId = AllocatePool (BufferLength);
        if (JedecId == NULL) {
          DEBUG ((DEBUG_ERROR, "ERROR - Failed to allocate JedecId buffer\r\n"));
        } else {
          //
          //  Start with the first flash type description;
          //
          MaxPriority = 0xffffffff;
          do {
            //
            //  Determine the highest priority protocol
            //
            Priority = 0;
            for (HandleIndex = 0; HandleCount > HandleIndex; HandleIndex++) {
              Protocol = SpiFlashPartProtocol [HandleIndex];
              if ((MaxPriority >= Protocol->Priority)
                && (Priority < Protocol->Priority))
                Priority = Protocol->Priority;
            }
            if (Priority == 0) {
              //
              //  The flash is not supported
              //
              break;
            }

            //
            //  Walk the handles containing the SPI flash part protocol
            //
            HandleIndex = 0;
            do {
              //
              //  Verify the description type matches and the opcode table
              //  supports the minimum number of entries required for the code
              //
              Protocol = SpiFlashPartProtocol [HandleIndex];
              if (Priority == Protocol->Priority) {
                //
                //  Get the JEDEC ID opcode description
                //
                Description = Protocol->GetFlashDescription (
                                          Protocol,
                                          NULL,
                                          NULL
                                          );
                if ((Description == NULL)
                  || (SPI_FLASH_PART_OPCODE_JEDEC_ID == Description->OpcodeTableEntries)) {
                  DEBUG ((DEBUG_ERROR, "ERROR - JEDEC ID opcode not available\r\n"));
                } else {
                  //
                  //  Display the flash part
                  //
                  DEBUG ((DEBUG_INFO, "Priority: 0x%08x, SPI Flash Part: %s\r\n", Priority, Description->PartNumber ));

                  //
                  //  Attempt to read the JEDEC ID
                  //
                  Status = PerformJedecIdOperation (
                             Context,
                             Description,
                             Description->JededIdResponseLengthInBytes,
                             JedecId
                             );
                  if (!EFI_ERROR (Status)) {
                    //
                    //  Display the JEDEC ID
                    //
                    DEBUG_CODE_BEGIN ();
                    {
                      UINTN Index;

                      DEBUG ((DEBUG_INFO, "JEDEC ID:"));
                      for (Index = 0; Description->JededIdResponseLengthInBytes > Index; Index++) {
                        DEBUG ((DEBUG_INFO, " 0x%02x", JedecId [Index]));
                      }
                      DEBUG ((DEBUG_INFO, "\r\n"));
                    }
                    DEBUG_CODE_END ();

                    //
                    //  Verify support and determine flash size
                    //
                    Description = Protocol->GetFlashDescription (
                                              Protocol,
                                              JedecId,
                                              &FlashSize
                                              );
                    if (Description != NULL) {
                      //
                      //  The flash device is supported
                      //  Return the table for this flash device
                      //
                      DEBUG ((DEBUG_INFO, "SPI flash device found: %s\r\n", Description->PartNumber));
                      *FlashDescription = Description;
                      goto PartFound;
                    }
                  }
                }
              }

              //
              //  Set next handle
              //
              HandleIndex += 1;
            } while (HandleCount > HandleIndex);

            //
            //  Set the next priority
            //
            MaxPriority = Priority - 1;
          } while (Priority != 0);

          //
          //  No flash device found
          //
          DEBUG ((DEBUG_ERROR, "Matching SPI flash description not found\r\n"));
        }
      }
    }
  }

PartFound:
  //
  //  Free the buffers
  //
  if (JedecId != NULL) {
    FreePool (JedecId);
  }
  if (SpiFlashPartProtocol != NULL) {
    FreePool (SpiFlashPartProtocol);
  }
  if (HandleArray != NULL) {
    FreePool (HandleArray);
  }

  //
  //  Return the flash size
  //  Zero (0) indicates flash not found or not supported
  //
  return FlashSize;
}

/**
  Load opcode into the SPI controller for specific flash device

  @param[in]  FlashDescription   Description of the flash device

  @retval  EFI_SUCCESS           The opcode was successfully loaded
  @retval  EFI_UNSUPPORTED       The opcode was not found

**/
EFI_STATUS
SpiFlashInit (
  IN CONST FLASH_PART_DESCRIPTION  *FlashDescription
  )
{
  EFI_STATUS       Status;
  SPI_INIT_DATA    SpiInitTable;
  UINTN            Index;
  UINT8            CmdCfgIndex;
  CONST UINT8                   OpcodeMap [] = {
  SPI_READ_ID,                        //  SPI_FLASH_PART_OPCODE_JEDEC_ID
  SPI_RDSR,                           //  SPI_FLASH_PART_OPCODE_READ_STATUS
  SPI_WRSR,                           //  SPI_FLASH_PART_OPCODE_WRITE_STATUS
  SPI_READ,                           //  SPI_FLASH_PART_OPCODE_READ_BYTES
  SPI_PROG,                           //  SPI_FLASH_PART_OPCODE_WRITE_256_BYTE_PAGE
  SPI_SERASE,                         //  SPI_FLASH_PART_OPCODE_ERASE_4K_BYTE_BLOCK
  SPI_BERASE,                         //  SPI_FLASH_PART_OPCODE_ERASE_64K_BYTE_BLOCK
  SPI_WRDI_SFDP                       //  SPI_FLASH_PART_OPCODE_WRITE_DISABLE
  };

  Status = EFI_SUCCESS;

  ZeroMem (&SpiInitTable, sizeof (SPI_INIT_DATA));

  SpiInitTable.PrefixOpcode[SPI_WREN] = FlashDescription->WriteEnable;
  SpiInitTable.PrefixOpcode[SPI_EWSR] = FlashDescription->WriteStatusEnable;

  SpiInitTable.BiosStartOffset = (UINTN)BIOS_REGION_FLASH_OFFSET;
  SpiInitTable.BiosSize = (UINTN)PcdGet32 (PcdBiosImageSize);
  SpiInitTable.SpecialOpcodeEntry = NULL;

  ASSERT (FlashDescription->OpcodeTableEntries <= SPI_NUM_OPCODE);

  for (Index = 0;Index < FlashDescription->OpcodeTableEntries;Index++) {
    CmdCfgIndex = OpcodeMap[Index];
    switch (FlashDescription->OpcodeTable[Index].MaxFrequency) {
      case 20000000:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Frequency = EnumSpiCycle20MHz;
        break;

      case 33000000:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Frequency = EnumSpiCycle33MHz;
        break;

      case 50000000:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Frequency = EnumSpiCycle50MHz;
        break;

      case 66000000:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Frequency = EnumSpiCycle66MHz;
        break;

      default:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Frequency = EnumSpiCycle33MHz;			
        break;
    }

    switch (FlashDescription->OpcodeTable[Index].OpcodeIndex) {
      case SPI_FLASH_PART_OPCODE_JEDEC_ID:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationJedecId;
        break;

      case SPI_FLASH_PART_OPCODE_READ_STATUS:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationReadStatus;
        break;

      case SPI_FLASH_PART_OPCODE_WRITE_STATUS:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationWriteStatus;
        break;

      case SPI_FLASH_PART_OPCODE_READ_BYTES:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationReadData;
        break;

      case SPI_FLASH_PART_OPCODE_WRITE_256_BYTE_PAGE:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationProgramData_1_Byte;
        break;

      case SPI_FLASH_PART_OPCODE_ERASE_4K_BYTE_BLOCK:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationErase_4K_Byte;
        break;

      case SPI_FLASH_PART_OPCODE_ERASE_64K_BYTE_BLOCK:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationErase_64K_Byte;
        break;

      case SPI_FLASH_PART_OPCODE_WRITE_DISABLE_DISCOVERY:
        SpiInitTable.SpiCmdConfig[CmdCfgIndex].Operation = EnumSpiOperationWriteDisable;
        break;

      default:
        DEBUG ((DEBUG_ERROR, "Unrecognized opcode index\r\n"));				
        ASSERT(FALSE);
        break;
    }
  }

  Status = mSpiProtocol->Init (mSpiProtocol, &SpiInitTable);	

  return Status;
}
