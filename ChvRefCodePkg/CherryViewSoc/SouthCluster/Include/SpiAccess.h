/** @file:
  The header of Spi accessed driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SPI_ACCESS_H_
#define _SPI_ACCESS_H_

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DriverLib.h>
#include <Protocol/SpiBus.h>

#define SPI_ACCESS_PROTOCOL_GUID \
  {0xced4f30b, 0xdc71, 0x405f, 0x83, 0xf, 0x2b, 0x4b, 0xbb, 0x65, 0xba, 0xce}

typedef struct _SPI_ACCESS_PROTOCOL   SPI_ACCESS_PROTOCOL;

#define DID_SPI_ID_PREFIX   "SPI"
#define DID_SPI_ID_SUFFIX   "\\OEMK"

typedef
EFI_STATUS
(EFIAPI *SPI_DEVICE_READ) (
	IN UINT8      ControllerId,
	IN UINT8      ChipSelect,
	IN OUT UINT8  *ReadBuffer,
	IN UINT32      BytesCount
);

typedef
EFI_STATUS
(EFIAPI *SPI_DEVICE_WRITE) (
	IN UINT8      ControllerId,
	IN UINT8      ChipSelect,
	IN UINT8      *WriteBuffer,
	IN UINT32      BytesCount
);

typedef struct _SPI_ACCESS_PROTOCOL {
   SPI_DEVICE_READ  		ReadRequest;
   SPI_DEVICE_WRITE 		WriteRequest;
} SPI_ACCESS_PROTOCOL;

extern EFI_GUID gEfiSpiAccessProtocolGuid;
#endif
