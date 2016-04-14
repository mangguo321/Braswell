/** @file
  SMM Communication formats for the SPI Device protocols.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SPI_DEVICE_SMM_COMM_H_
#define _SPI_DEVICE_SMM_COMM_H_

#include <Protocol/SmmSpiDevice.h>

//
// Define communication constants
//
#define SPI_DEV_FUNCTION_READ         1
#define SPI_DEV_FUNCTION_WRITE        2
#define SPI_DEV_FUNCTION_ERASE        3
#define SPI_DEV_FUNCTION_LOCK         4
#define SPI_DEV_FUNCTION_SET_RANGE    5
#define SPI_DEV_FUNCTION_LOCK_RANGES  6

//
// Generic SPI Device communication structure header.
//
typedef struct {
  UINTN       Function;
  EFI_STATUS  ReturnStatus;
  UINT8       Data[1];
} SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER;

//
// Macros used to determine size of the headers without data size.
//
#define SMM_COMMUNICATE_HEADER_SIZE  (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data))
#define SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER_SIZE  (OFFSET_OF (SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER, Data))

//
// SPI Read, Write and Erase Data.  Erase will not have any extra data.
//
typedef struct {
  UINTN     Offset;
  UINTN     Size;
} SMM_SPI_DEV_READ_WRITE_ERASE_HEADER;

//
// SPI Lock
//
typedef struct {
  UINTN   Offset;
  UINTN   Size;
  BOOLEAN Lock;
} SMM_SPI_DEV_LOCK_HEADER;

//
// SPI Set Range
//
typedef struct {
  UINTN   Offset;
  UINTN   Size;
  BOOLEAN ReadLock;
  BOOLEAN WriteLock;
} SMM_SPI_DEV_SET_RANGE_HEADER;

#endif
