/** @file
  This file contains routines that support PCH SMBUS FUNCTION

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSmbusLib.h"

/**
  This function provides a standard way to execute Smbus sequential
  I2C Read. This function allows the PCH to perform block reads to
  certain I2C devices, such as serial E2PROMs. Typically these data
  bytes correspond to an offset (address) within the serial memory
  chips.

  @param[in] SmBusAddress         Address that encodes the SMBUS Slave Address,
                                  SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Buffer              Pointer to the buffer to store the bytes read
                                  from the SMBUS
  @param[out] Status              eturn status for the executed command.

  @retval UINTN                   The number of bytes read
**/
UINTN
EFIAPI
SmBusSeqI2CRead (
  IN  UINTN          SmBusAddress,
  OUT VOID           *Buffer,
  OUT RETURN_STATUS  * Status OPTIONAL
  )
{
  UINTN  Length;

  ASSERT (Buffer != NULL);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress) >= 1);
  ASSERT (SMBUS_LIB_RESEARVED (SmBusAddress) == 0);

  Length = SMBUS_LIB_LENGTH (SmBusAddress);
  return InternalSmBusExec (EfiSmbusReadByte, SmBusAddress, Length, Buffer, Status);
}
