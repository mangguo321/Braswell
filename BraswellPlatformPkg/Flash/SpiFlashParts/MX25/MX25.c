/** @file
  MACRONIX MX25*** family SPI flash support

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SpiFlashPart.h>

//
// Initialization data that can be used to identify SPI flash part
//    DeviceId0       Device ID0 of the SPI device
//    DeviceId1       Device ID1 of the SPI device
//
typedef struct _SPI_CHIP_ID {
  UINT8  DeviceId0;
  UINT8  DeviceId1;
} SPI_CHIP_ID;

//
// Serial Flash VendorId and DeviceId
//
#define SF_VENDOR_ID_MACRONIX     0xC2
#define SF_DEVICE_ID0_MX25LXX     0x20
#define SF_DEVICE_ID0_MX25UXX     0x25
#define SF_DEVICE_ID1_MX25L16     0x15
#define SF_DEVICE_ID1_MX25L32     0x16
#define SF_DEVICE_ID1_MX25L64     0x17
#define SF_DEVICE_ID1_MX25L128    0x18
#define SF_DEVICE_ID1_MX25U32     0x36
#define SF_DEVICE_ID1_MX25U64     0x37

//
// Generic SPI flash part description
//
CONST FLASH_PART_DESCRIPTION mFlashDescription = {
  L"MACRONIX MX25*** family",  //  Part number
  1,                //  Number of status bytes
  0x06,             //  Write enable
  0x50,             //  Write status enable
  3,                //  Length of JEDEC ID response
  8,                //  Number of opcodes in the table

  //  Opcode table
  {
    {33000000,          3, FALSE, 0x9f, 0, SPI_FLASH_PART_OPCODE_JEDEC_ID},
    {50000000,          1, FALSE, 0x05, 0, SPI_FLASH_PART_OPCODE_READ_STATUS},
    {50000000,          1, TRUE,  0x01, 0, SPI_FLASH_PART_OPCODE_WRITE_STATUS},
    {33000000, 0xffffffff, FALSE, 0x03, 3, SPI_FLASH_PART_OPCODE_READ_BYTES},
    {50000000,          1, TRUE,  0x02, 3, SPI_FLASH_PART_OPCODE_WRITE_256_BYTE_PAGE},
    {50000000,          0, TRUE,  0x20, 3, SPI_FLASH_PART_OPCODE_ERASE_4K_BYTE_BLOCK},
    {50000000,          0, TRUE,  0xd8, 3, SPI_FLASH_PART_OPCODE_ERASE_64K_BYTE_BLOCK},
    {50000000,          0, TRUE,  0x5a, 0, SPI_FLASH_PART_OPCODE_WRITE_DISABLE_DISCOVERY}
  }
};

SPI_CHIP_ID  mSpiChipIdTable[] = {
  {
    SF_DEVICE_ID0_MX25LXX,      // DeviceId 0
    SF_DEVICE_ID1_MX25L16       // DeviceId 1
  },
  {
    SF_DEVICE_ID0_MX25LXX,      // DeviceId 0
    SF_DEVICE_ID1_MX25L32       // DeviceId 1
  },
  {
    SF_DEVICE_ID0_MX25LXX,      // DeviceId 0
    SF_DEVICE_ID1_MX25L64       // DeviceId 1
  },
  {
    SF_DEVICE_ID0_MX25LXX,      // DeviceId 0
    SF_DEVICE_ID1_MX25L128      // DeviceId 1
  },
  {
    SF_DEVICE_ID0_MX25UXX,      // DeviceId 0
    SF_DEVICE_ID1_MX25U32       // DeviceId 1
  },
  {
    SF_DEVICE_ID0_MX25UXX,      // DeviceId 0
    SF_DEVICE_ID1_MX25U64       // DeviceId 1
  }	
};

/**
  Get the flash part size and opcode table.

  Validate support for this flash part and determine
  the flash part size and opcode description table
  from the JEDEC ID information provided.

  @param[in]  This       Pointer to a SPI_FLASH_DATA_PROTOCOL
                         data structure.
  @param[in]  JedecId    Pointer to a three byte buffer containing
                         the JEDEC ID returned by the flash part.
                         If the input value is NULL then a
                         table containing the description for the
                         JEDEC ID opcode is returned by this
                         routine.
  @param[out] FlashSize  Pointer to receive the size of the flash
                         part in bytes.  Zero (0) is returned when
                         JedecId is NULL.
  @returns               When JedecId is not NULL, this routine returns a pointer
                         to a FLASH_PART_DESCRIPTION data structure which supports
                         this flash part.  The returned value is NULL if the flash
                         part is not supported.
                         When JedecId is NULL, this routine returns a pointer to
                         a FLASH_PART_DESCRIPTION structure which supports the
                         JEDEC ID command.  This opcode description may be used
                         to determine the manufacture and product data for the
                         SPI flash part.

**/
CONST FLASH_PART_DESCRIPTION *
EFIAPI
GetFlashDescription (
  IN  SPI_FLASH_PART_PROTOCOL  *This,
  IN  UINT8                    *JedecId    OPTIONAL,
  OUT UINT64                   *FlashSize  OPTIONAL
  )
{
  CONST FLASH_PART_DESCRIPTION  *FlashDescription;
  UINT8                         Shift;
  UINT64                        Size;
  UINTN                         Index;

  //
  //  Determine if the SPI flash device is supported
  //
  Size = 0;
  FlashDescription = &mFlashDescription;
  if (JedecId != NULL) {
    FlashDescription = NULL;
    if (JedecId [0] != SF_VENDOR_ID_MACRONIX)
      return NULL;

    for (Index = 0;Index < (sizeof (mSpiChipIdTable) / sizeof (SPI_CHIP_ID));Index++) {
      if ((JedecId [1] == mSpiChipIdTable[Index].DeviceId0) 
           && (JedecId [2] == mSpiChipIdTable[Index].DeviceId1))
        break;	
    }

    if (Index >= (sizeof (mSpiChipIdTable) / sizeof (SPI_CHIP_ID)))
      return NULL;

    Shift = JedecId[2];

    if (JedecId [1] == SF_DEVICE_ID0_MX25UXX)
      Shift-= 0x20;

    if (Shift < 64) {
      Size = LShiftU64 (1, Shift);
      FlashDescription = &mFlashDescription;
    }
  }

  //
  //  Return the flash size
  //
  if (FlashSize != NULL) {
    *FlashSize = Size;
  }

  return FlashDescription;
}

SPI_FLASH_PART_PROTOCOL mSpiFlashPartProtocol = {
  0xC0000000,     /// SPI flash part family priority
  GetFlashDescription
};

/**
  Indicate that all the SPI devices are available

  @retval  EFI_SUCCESS  SPI flash part protocol installed successfully

**/
EFI_STATUS
EFIAPI
SpiFlashPart (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS  Status;

  //
  //  Install the SPI flash part protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &gImageHandle,
                  &gSpiFlashPartProtocolGuid,
                  &mSpiFlashPartProtocol,
                  NULL
                  );

  return Status;
}
