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

#ifndef _SPI_DEVICE_H_
#define _SPI_DEVICE_H_

#include <Protocol/Spi.h>
#include <Protocol/SpiFlashPart.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

//
// Defines the offset in the SPI device where the BIOS region starts.
//
#define BIOS_REGION_FLASH_OFFSET        (PcdGet32 (PcdBiosImageBase) - PcdGet32 (PcdFlashAreaBaseAddress))
#define VN_STORAGE_REGION_FLASH_OFFSET  (PcdGet32 (PcdFlashNvStorageVariableBase) - PcdGet32 (PcdFlashAreaBaseAddress))

extern EFI_SPI_PROTOCOL  *mSpiProtocol;
extern UINTN mNvStorageBase;

//
// Prefix Opcode Index on the host SPI controller
//
typedef enum {
  SPI_WREN,             // Prefix Opcode 0: Write Enable
  SPI_EWSR,             // Prefix Opcode 1: Enable Write Status Register
} PREFIX_OPCODE_INDEX;

//
// Opcode Menu Index on the host SPI controller
//
typedef enum {
  SPI_READ_ID,        // Opcode 0: READ ID, Read cycle with address
  SPI_READ,           // Opcode 1: READ, Read cycle with address
  SPI_RDSR,           // Opcode 2: Read Status Register, No address
  SPI_WRDI_SFDP,      // Opcode 3: Write Disable or Discovery Parameters, No address
  SPI_SERASE,         // Opcode 4: Sector Erase (4KB), Write cycle with address
  SPI_BERASE,         // Opcode 5: Block Erase (32KB), Write cycle with address
  SPI_PROG,           // Opcode 6: Byte Program, Write cycle with address
  SPI_WRSR,           // Opcode 7: Write Status Register, No address
} SPI_OPCODE_INDEX;

EFI_STATUS
EFIAPI
InitSpiDevice (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_STATUS
EFIAPI
SpiRead (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       *Size,
  OUT     UINT8       *Buffer
  );

EFI_STATUS
EFIAPI
SpiWrite (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       *Size,
  IN      UINT8       *Buffer
  );

EFI_STATUS
EFIAPI
SpiErase (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       Size
  );

EFI_STATUS
EFIAPI
SpiLock (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       Size,
  IN      BOOLEAN     Lock
  );

EFI_STATUS
EFIAPI
SpiSetRange (
  IN      UINTN       SpiOffset,
  IN      UINTN       Size,
  IN      BOOLEAN     ReadLock,
  IN      BOOLEAN     WriteLock
  );

EFI_STATUS
EFIAPI
SpiLockRanges (
  );

BOOLEAN
ReadUsingMmio (
  IN UINTN  SpiOffset
  );

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
  );

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

**/
typedef
EFI_STATUS
(EFIAPI  *PERFORM_JEDEC_ID_OPERATION) (
  IN       VOID                    *Context,
  IN CONST FLASH_PART_DESCRIPTION  *Description,
  IN       UINTN                   BufferLength,
  OUT      UINT8                   *JedecId
  );
  
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
  );
  
  
/**
  Load opcode into the SPI controller for specific flash device

  @param[in]  FlashDescription       Description of the flash device

  @retval  EFI_SUCCESS               The opcode was successfully loaded
  @retval  EFI_UNSUPPORTED           The opcode was not found

**/
EFI_STATUS
SpiFlashInit (
  IN CONST FLASH_PART_DESCRIPTION  *FlashDescription
  );
  
#endif
