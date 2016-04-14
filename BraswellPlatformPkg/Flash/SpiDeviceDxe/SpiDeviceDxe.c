/** @file
  This driver for SPI Device initialization

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Protocol/Spi.h>
#include <Protocol/SpiDevice.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/EventGroup.h>
#include "SpiDevice.h"

SPI_DEVICE_PROTOCOL    mSpiDevProtocol = {
  SpiRead,
  SpiWrite,
  SpiErase,
  SpiLock,
  SpiSetRange,
  SpiLockRanges
};

VOID
EFIAPI
SpiDeviceVirtualAddressChangeEvent (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  //
  // Update protocol pointer to the SPI Controller interface.
  //
  EfiConvertPointer (0x00, (VOID**) &(mSpiProtocol));

  //
  // Update the NV Storage location for runtime access.
  //
  EfiConvertPointer (0x00, (VOID**) &(mNvStorageBase));

  //
  // Take care of pointers in protocol.
  //
  EfiConvertPointer (0x00, (VOID**) &(mSpiDevProtocol.SpiRead));
  EfiConvertPointer (0x00, (VOID**) &(mSpiDevProtocol.SpiWrite));
  EfiConvertPointer (0x00, (VOID**) &(mSpiDevProtocol.SpiErase));
  EfiConvertPointer (0x00, (VOID**) &(mSpiDevProtocol.SpiLock));
}

EFI_STATUS
EFIAPI
InitSpiDevice (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_EVENT                     Event;
  CONST FLASH_PART_DESCRIPTION  *FlashDescription;
  UINT64                        FlashSize;
  EFI_HANDLE                    Handle;
  EFI_STATUS                    Status;

  mNvStorageBase = PcdGet32 (PcdFlashNvStorageVariableBase);

  //
  // Locate the SPI controller protocol and save it for later.
  //
  DEBUG((EFI_D_INFO, "Locating SPI Controller Protocol.\n"));
  Status = gBS->LocateProtocol (
                  &gEfiSpiProtocolGuid,
                  NULL,
                  (VOID **) &mSpiProtocol
                  );
  ASSERT_EFI_ERROR(Status);
  
  //
  // Loop through all the flash devices that are supported and see if one will
  // initialize the SPI Controller interface.
  //
  FlashSize = FindFlashSupport (
                &JedecIdRead,
                NULL,
                &FlashDescription
                );
  if (FlashSize == 0) {
    DEBUG((EFI_D_ERROR, "No SPI flash part description found!\r\n"));
  } else {
    //
    // Attempt to configure the SPI controller for this device.
    //
    DEBUG((EFI_D_INFO, "SPI flash size: %d MBytes\n", DivU64x32(FlashSize, 1024 * 1024 )));
    DEBUG((EFI_D_INFO, "Configuring SPI Controller.\n"));
    
    Status = SpiFlashInit (FlashDescription);    
    if (!EFI_ERROR (Status)) {
      //
      // Publish the SPI Device protocol.
      //
      DEBUG((EFI_D_INFO, "Installing SPI Device Protocol.\n"));
      Handle = NULL;
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &Handle,
                      &gSpiDeviceProtocolGuid,
                      &mSpiDevProtocol,
                      NULL
                      );
      if (EFI_ERROR (Status)) {
        return EFI_OUT_OF_RESOURCES;
      }

      //
      // Make sure we can handle virtual address changes.
      //
      Event = NULL;
      Status = gBS->CreateEventEx (
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      SpiDeviceVirtualAddressChangeEvent,
                      NULL,
                      &gEfiEventVirtualAddressChangeGuid,
                      &Event
                      );      

      return EFI_SUCCESS;
    }
  }

  //
  // Unable to find a supported SPI device
  //
  DEBUG((EFI_D_ERROR, "Unable to configure SPI Controller for SPI device present.\n"));

  return EFI_UNSUPPORTED;
}

BOOLEAN
ReadUsingMmio (
  IN UINTN  SpiOffset
  )
{
  return (BOOLEAN) ((SpiOffset >= BIOS_REGION_FLASH_OFFSET) && (SpiOffset < (BIOS_REGION_FLASH_OFFSET + PcdGet32 (PcdBiosImageSize))) && (!EfiAtRuntime ()));
}
