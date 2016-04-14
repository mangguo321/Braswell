/** @file
  SPI Device ACPI Protocol

  This protocol supports the enumerations of device on the SPI bus.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SPI_ACPI_H__
#define __SPI_ACPI_H__

#include <Protocol/DevicePath.h>

///
/// SPI ACPI protocol
///
typedef struct _EFI_SPI_ACPI_PROTOCOL   EFI_SPI_ACPI_PROTOCOL;

typedef struct _SPI_TARGET_SETTINGS {
UINT32 WireMode;
UINT32 ChipSelectLine;
UINT32 ChipSelectPolarity;
UINT32 SerialClockPolarity;
UINT32 SerialClockPhase;
UINT32 DataFrameSize; //supported are 8, 16, 32
UINT32 ClockSpeed;
UINT32 LoopbackMode;
UINT8  BytesPerEntry;
UINT32 NotDefined; // to be asked to SSG reference in Spi0BusConfig.c mE2PROM_target
}SPI_TARGET_SETTINGS;

///
/// SPI device description
///
/// This structure provides the platform specific information which
/// describes an SPI device.
///
typedef struct {
  ///
  /// Hardware revision - SPI _HRV value
  ///
  UINT32 HardwareRevision;

  ///
  /// Device path node for the SPI device.
  ///
  CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath;

  SPI_TARGET_SETTINGS            *targetSettings;
}EFI_SPI_DEVICE;

/**
  Enumerate the SPI devices

  This routine must be called at or below TPL_NOTIFY.

  This function walks the platform specific data to enumerates the
  SPI devices on an SPI bus.

  @param[in]  This              Address of an EFI_SPI_ACPI_PROTOCOL
                                structure.
  @param[in, out] Device        Buffer containing the address of an
                                EFI_SPI_DEVICE structure.  Enumeration
                                is started by setting the initial
                                EFI_SPI_DEVICE structure address to NULL.
                                The buffer receives an EFI_SPI_DEVICE
                                structure address for the next SPI device.

  @retval EFI_SUCCESS           The platform data for the next device
                                on the SPI bus was returned successfully.
  @retval EFI_INVALID_PARAMETER NextDevice was NULL
  @retval EFI_NO_MAPPING        PreviousDevice does not point to a valid
                                EFI_SPI_DEVICE structure.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPI_ACPI_ENUMERATE) (
  IN CONST EFI_SPI_ACPI_PROTOCOL *This,
  IN OUT CONST EFI_SPI_DEVICE **Device
  );

///
/// SPI device description
///
/// This structure provides the platform specific information which
/// describes an SPI device.
///
struct _EFI_SPI_ACPI_PROTOCOL {
  ///
  /// Walk the platform's list of SPI devices on the bus.  This
  /// routine returns the next SPI device in the platform's list
  /// for this SPI bus.
  ///
  EFI_SPI_ACPI_ENUMERATE Enumerate;
};

///
/// Variable containing the GUID for the SPI device enumeration protocol
///
extern EFI_GUID gEfiSpiAcpiProtocolGuid;

#endif  //  __SPI_ACPI_H__
