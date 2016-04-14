/** @file
  SPI flash part protocol declaration
  
  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SPI_FLASH_PART_H__
#define __SPI_FLASH_PART_H__

//
//  Index values into the flash part opcode table
//
#define SPI_FLASH_PART_OPCODE_JEDEC_ID              0
#define SPI_FLASH_PART_OPCODE_READ_STATUS           1
#define SPI_FLASH_PART_OPCODE_WRITE_STATUS          2
#define SPI_FLASH_PART_OPCODE_READ_BYTES            3
#define SPI_FLASH_PART_OPCODE_WRITE_256_BYTE_PAGE   4
#define SPI_FLASH_PART_OPCODE_ERASE_4K_BYTE_BLOCK   5
#define SPI_FLASH_PART_OPCODE_ERASE_64K_BYTE_BLOCK  6
#define SPI_FLASH_PART_OPCODE_WRITE_DISABLE_DISCOVERY  7

///
/// SPI flash part opcode description
///
typedef struct {
  ///
  /// The maximum frequency in Hz to use for this opcode
  /// 
  UINT32   MaxFrequency;

  ///
  /// Maximum transfer length
  ///
  UINT32   MaxTransferLength;

  ///
  /// Set to TRUE if sending the one of the WriteEnable bytes
  /// before the opcode
  ///
  BOOLEAN  EnableWrite;

  ///
  /// Opcode for the flash part
  ///
  UINT8    Opcode;

  ///
  /// Number of address bytes to send for this opcode
  ///
  UINT8    AddressBytes;

  ///
  /// Index for the opcode (SPI_FLASH_PART_OPCODE_*),
  /// enables sparse opcode tables
  ///
  UINT8   OpcodeIndex;
} SPI_FLASH_PART_OPCODE_ENTRY;

typedef struct {
  ///
  /// Part number of the flash device
  ///
  CONST CHAR16                 *PartNumber;

  ///
  /// Number of status bytes
  ///
  UINT8                        StatusBytes;

  ///
  /// Write enable - Byte to issue before a write or erase command
  ///
  UINT8                        WriteEnable;

  ///
  /// Write status enable - Byte to issue before a status write
  ///
  UINT8                        WriteStatusEnable;

  ///
  /// Length of the JEDEC ID repsonse.
  ///
  UINT8                        JededIdResponseLengthInBytes;

  ///
  /// Number of entries in the opcode table
  ///
  UINT8                        OpcodeTableEntries;

  ///
  /// Opcodes supported by the SPI flash part
  ///
  SPI_FLASH_PART_OPCODE_ENTRY  OpcodeTable [];
} FLASH_PART_DESCRIPTION;

///
/// Forward reference to protocol
///
typedef struct _SPI_FLASH_PART_PROTOCOL  SPI_FLASH_PART_PROTOCOL;

/**
  Get the flash part size and opcode table.

  Validate support for this flash part and determine
  the flash part size and opcode description table
  from the JEDEC ID information provided.

  @param  This       Pointer to a SPI_FLASH_DATA_PROTOCOL
                     data structure.
  @param  JedecId    Pointer to a three byte buffer containing
                     the JEDEC ID returned by the flash part.
                     If the input value is NULL then a
                     table containing the description for the
                     JEDEC ID opcode is returned by this
                     routine.
  @param  FlashSize  Pointer to receive the size of the flash
                     part in bytes.  Zero (0) is returned when
                     JedecId is NULL.
  @returns  When JedecId is not NULL, this routine returns a pointer
            to a FLASH_PART_DESCRIPTION data structure which supports
            this flash part.  The returned value is NULL if the flash
            part is not supported.
            When JedecId is NULL, this routine returns a pointer to
            a FLASH_PART_DESCRIPTION structure which supports the
            JEDEC ID command.  This opcode description may be used
            to determine the manufacture and product data for the
            SPI flash part.

**/
typedef
CONST FLASH_PART_DESCRIPTION *
(EFIAPI  *SPI_FLASH_PART_PROTOCOL_GET_FLASH_DESCRIPTION) (
  IN  SPI_FLASH_PART_PROTOCOL  *This,
  IN  UINT8                    *JedecId    OPTIONAL,
  OUT UINT64                   *FlashSize  OPTIONAL
  );

///
/// SPI Flash Part Protocol
///
/// The SPI flash part protocol provides manufacture
/// specific SPI flash opcode descriptions to the
/// generic code which manipulates the SPI flash part.
/// Descriptions may range from part specific through
/// family specific to generic.  The flash search
/// algorithm use the Priority value to order which
/// JEDEC ID opcodes are provided first to the SPI
/// flash part.
///
/// Two Priority values will typically be defined by the
/// IVH to support:
///    * A single chip - higher value
///    * A family of chips - lower value
/// The GetOpcodeTable routine for these values matches
/// all or part of the JEDEC ID value to make its determination.
///
/// The OEM may also provide a driver to provide a
/// default opcode set.  The GetOpcodeTable routine for the
/// Priority value selected most likely will ignore the
/// JEDEC ID value.
///
typedef struct _SPI_FLASH_PART_PROTOCOL {
  ///
  /// The search code uses the priority value to order
  /// the processing of the SPI_FLASH_PART_PROTOCOL
  /// entries to determine the support for the SPI flash
  /// part.  The Priority values of zero (0) is reserved.
  /// Priority values of 0x00000001-0x0fffffff are reserved
  /// for the platform/OEM specific drivers.  The Priority
  /// values of 0x10000000-0xffffffff are reserved for IHV
  /// developed drivers.  Note that Priority values do not
  /// need to be unique.  When two or more Priority values
  /// are equal the order in which the JEDEC commands are
  /// passed to the SPI flash part is random.
  /// 
  UINT32  Priority;

  ///
  /// Get the flash part size and opcode table
  ///
  SPI_FLASH_PART_PROTOCOL_GET_FLASH_DESCRIPTION  GetFlashDescription;
} __SPI_FLASH_PART_PROTOCOL;

///
/// Reference to variable defined in the .DEC file
///
extern EFI_GUID gSpiFlashPartProtocolGuid;

///
/// Tag GUID to indicate that all SPI flash parts are available
///
extern EFI_GUID gSpiFlashPartSyncGuid;

#endif  //  __SPI_FLASH_PART_H__
