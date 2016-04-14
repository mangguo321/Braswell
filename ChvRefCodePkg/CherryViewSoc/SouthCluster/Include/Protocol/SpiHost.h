/** @file
  SPI host interface declaration

  A driver or application uses the SPI host protocol to
  perform operations on the SPI bus.  With the support
  of the SPI platform driver, access is possible to any
  device on the SPI bus, even devices behind multiplexers
  and switches.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SPI_HOST_H__
#define __SPI_HOST_H__

//#include <Protocol/SpiMaster.h>

#include <Protocol/SpiBus.h>
/**
  Declare the forward references

**/
typedef struct _EFI_SPI_HOST_PROTOCOL EFI_SPI_HOST_PROTOCOL;

/**
  Queue an SPI operation for execution on the SPI controller.

  @param[in] This             Address of an EFI_SPI_HOST_PROTOCOL instance.
  @param[in] Event            Event to set for asynchronous operations,
                              NULL for synchronous operations
  @param[in] RequestPacket    Address of an EFI_SPI_REQUEST_PACKET
  

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER 
  @retval EFI_ABORTED           The request did not complete because the driver
                                was shutdown.
  @retval EFI_BAD_BUFFER_SIZE   The WriteBytes or ReadBytes buffer size is too large.
  @retval EFI_DEVICE_ERROR      There was an SPI error during the operation.
                                This could indicate the slave device is not present.
  @retval EFI_NO_RESPONSE       The SPI device is not responding to the
                                slave address.  EFI_DEVICE_ERROR may also be
                                returned if the controller can not distinguish
                                when the NACK occurred.
  @retval EFI_NOT_FOUND         SPI slave address exceeds maximum address
  @retval EFI_NOT_READY         SPI bus is busy or operation pending, wait for
                                the event and then read status pointed to by
                                the request packet.
  @retval EFI_OUT_OF_RESOURCES  Insufficient memory for SPI operation
  @retval EFI_TIMEOUT           The transaction did not complete within an internally
                                specified timeout period.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPI_HOST_START_REQUEST) (
  IN CONST EFI_SPI_HOST_PROTOCOL *This,
  IN CONST EFI_SPI_DEVICE * SpiDevice,
  IN EFI_EVENT Event OPTIONAL,
  IN UINT8 ChipSelect,
  IN EFI_SPI_REQUEST_PACKET *RequestPacket
  );

///
/// Host access to the SPI bus.
///
struct _EFI_SPI_HOST_PROTOCOL {
  ///
  /// Queue an operation for execution on the SPI bus
  ///
  EFI_SPI_HOST_START_REQUEST StartRequest;

  ///
  /// The maximum number of bytes the SPI host controller
  /// is able to receive from the SPI bus.
  ///
  UINT32 MaximumReceiveBytes;

  ///
  /// The maximum number of bytes the SPI host controller
  /// is able to send on the SPI bus.
  ///
  UINT32 MaximumTransmitBytes;

  ///
  /// The maximum number of bytes in the SPI bus transaction.
  ///
  UINT32 MaximumTotalBytes;
};

///
/// GUID for the EFI_SPI_HOST_PROTOCOL
///
extern EFI_GUID gEfiSpiHostProtocolGuid;

#endif  //  __SPI_HOST_H__
