/** @file
  SPI bus interface

  This layer provides I/O access to an SPI device.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SPI_BUS_H__
#define __SPI_BUS_H__

///
/// SPI bus protocol
///
typedef struct _EFI_SPI_BUS_PROTOCOL  EFI_SPI_BUS_PROTOCOL;

///
/// SPI device operation
///
/// This structure provides the information necessary for an operation
/// on an SPI device
///
typedef struct {
  // Number of bytes to read, set to zero for write only operations
  UINT32 ReadBytes;        
  
  // Number of bytes read after read operation.
  UINT32 BytesRead;        
  
  // Address of the buffer to receive data from the SPI device. Use NULL
  // for write only operations.  The ReadBuffer must be at least ReadBytes
  // in length.
  //
  UINT8  *ReadBuffer;      

  // Number of bytes to send to the SPI device
  UINT32 WriteBytes;
  
  // Number of bytes written after write operation.
  UINT32 BytesWritten;
  
  //
  // Address of the buffer containing the data to send to the SPI device.
  // The WriteBuffer must be at least WriteBytes in length.
  UINT8  *WriteBuffer;

  ///
  /// Timeout for the SPI operation in 100 ns units
  ///
  UINT32 Timeout;
  }EFI_SPI_REQUEST_PACKET; 

/**
  Perform an SPI operation on the device

  This routine must be called at or below TPL_NOTIFY.  For synchronous
  requests this routine must be called at or below TPL_CALLBACK.

  N.B. The typical consumers of this API are the third party SPI
  drivers.  Extreme care must be taken by other consumers of this
  API to prevent confusing the third party SPI drivers due to a
  state change at the SPI device which the third party SPI drivers
  did not initiate.  SPI platform drivers may use this API within
  these guidelines.

  This routine queues an operation to the SPI controller for execution
  on the SPI bus.

  As an upper layer driver writer, the following need to be provided
  to the platform vendor:
  
  1.  ACPI CID value or string - this is used to connect the upper layer
      driver to the device.
  2.  ChipSelect value to identify the slave devices

  @param[in] This               Address of an EFI_I2C_BUS_PROTOCOL
                                Structure
  @param[in] AcpiId             AcpiId to let API know the SPI controller selection 
  @param[in] ChipSelect         ChipSelect line for one of the slave devices  
                                          
  @param[in] Event              Event to set for asynchronous operations,
                                NULL for synchronous operations
  @param[in] RequestPacket      Address of an EFI_SPI_REQUEST_PACKET
                                structure describing the SPI operation

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_ABORTED           The request did not complete because the driver
                                was shutdown.
  @retval EFI_BAD_BUFFER_SIZE   The WriteBytes or ReadBytes buffer size is too large.
  @retval EFI_DEVICE_ERROR      There was an SPI error (NACK) during the operation.
                                This could indicate the slave device is not present.
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL
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
(EFIAPI *EFI_SPI_BUS_START_REQUEST) (
  IN CONST EFI_SPI_BUS_PROTOCOL *This,
  IN EFI_EVENT Event OPTIONAL,
  IN UINT8 ChipSelect,
  IN EFI_SPI_REQUEST_PACKET *RequestPacket
  );

///
/// The SPI bus protocol enables access to a specific device on the SPI bus.
///
/// Each SPI device is described as an ACPI node (HID, UID and CID) within the
/// platform layer.  The SPI bus protocol enumerates the SPI devices in the
/// platform and creates a unique handle and device path for each SPI device.
///
/// SPI Chip Select is abstracted to limit operation to the specified SPI device. 
/// The third party providing the SPI device support
/// provides an ordered list of Chip Select line for the SPI device to the team
/// building the platform layer.  The platform team must preserve the order of the
/// supplied list.  ChipSelect is the number of entries in this list 

struct _EFI_SPI_BUS_PROTOCOL {
  
  EFI_SPI_BUS_START_REQUEST StartRequest;

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
/// GUID for the SPI bus protocol
///
extern EFI_GUID gEfiSpiBusProtocolGuid;

#endif  //  __SPI_BUS_H__
