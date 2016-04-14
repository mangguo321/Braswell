/** @file
  SMM driver for SPI Device initialization.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiSmm.h>
#include <Protocol/Spi.h>
#include <Protocol/SmmSpiDevice.h>
#include <Library/BaseLib.h>
#include <Library/SmmServicesTableLib.h>
#include "SpiDevice.h"
#include "SpiDeviceSmmComm.h"

SPI_DEVICE_PROTOCOL mSpiDevProtocol = {
  SpiRead,
  SpiWrite,
  SpiErase,
  SpiLock,
  SpiSetRange,
  SpiLockRanges
};

EFI_STATUS
EFIAPI
SpiDeviceSmmHandler (
  IN       EFI_HANDLE                 DispatchHandle,
  IN CONST VOID                       *RegisterContext,
  IN OUT   VOID                       *CommBuffer,
  IN OUT   UINTN                      *CommBufferSize
  )
{
  EFI_STATUS                                Status;
  SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER   *SpiDevCommHeader;
  SMM_SPI_DEV_READ_WRITE_ERASE_HEADER       *SpiDevDataOpHeader;
  SMM_SPI_DEV_LOCK_HEADER                   *SpiDevLockHeader;
  SMM_SPI_DEV_SET_RANGE_HEADER              *SpiDevSetRangeHeader;

  ASSERT (CommBuffer != NULL);

  SpiDevCommHeader = (SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER*) CommBuffer;
  switch (SpiDevCommHeader->Function) {
    case SPI_DEV_FUNCTION_READ:
      SpiDevDataOpHeader = (SMM_SPI_DEV_READ_WRITE_ERASE_HEADER*) SpiDevCommHeader->Data;
      Status = SpiRead (
                 SpiDevDataOpHeader->Offset,
                 &SpiDevDataOpHeader->Size,
                 (UINT8*) (SpiDevDataOpHeader + 1)
                 );
      break;
    case SPI_DEV_FUNCTION_WRITE:
      SpiDevDataOpHeader = (SMM_SPI_DEV_READ_WRITE_ERASE_HEADER*) SpiDevCommHeader->Data;
      Status = SpiWrite (
                 SpiDevDataOpHeader->Offset,
                 &SpiDevDataOpHeader->Size,
                 (UINT8*) (SpiDevDataOpHeader + 1)
                 );
      break;
    case SPI_DEV_FUNCTION_ERASE:
      SpiDevDataOpHeader = (SMM_SPI_DEV_READ_WRITE_ERASE_HEADER*) SpiDevCommHeader->Data;
      Status = SpiErase (
                 SpiDevDataOpHeader->Offset,
                 SpiDevDataOpHeader->Size
                 );
      break;
    case SPI_DEV_FUNCTION_LOCK:
      SpiDevLockHeader = (SMM_SPI_DEV_LOCK_HEADER*) SpiDevCommHeader->Data;
      Status = SpiLock (
                 SpiDevLockHeader->Offset,
                 SpiDevLockHeader->Size,
                 SpiDevLockHeader->Lock
                 );
      break;
    case SPI_DEV_FUNCTION_SET_RANGE:
      SpiDevSetRangeHeader = (SMM_SPI_DEV_SET_RANGE_HEADER*) SpiDevCommHeader->Data;
      Status = SpiSetRange (
                 SpiDevSetRangeHeader->Offset,
                 SpiDevSetRangeHeader->Size,
                 SpiDevSetRangeHeader->ReadLock,
                 SpiDevSetRangeHeader->WriteLock
                 );
      break;
    case SPI_DEV_FUNCTION_LOCK_RANGES:
      Status = SpiLockRanges ();
      break;
    default:
      ASSERT (FALSE);
      Status = EFI_UNSUPPORTED;
      break;
  }

  //
  // Set the return value.
  //
  SpiDevCommHeader->ReturnStatus = Status;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitSpiDevice (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  CONST FLASH_PART_DESCRIPTION  *FlashDescription;
  UINT64                        FlashSize;
  EFI_HANDLE                    Handle;
  EFI_STATUS                    Status;

  //--------------------------------------------------
  //
  //  Note only this routine is able to make calls
  //  into the DXE environment since it is called
  //  synchronously from that environment and DXE
  //  is still executing in physical mode.
  //
  //--------------------------------------------------

  mNvStorageBase = PcdGet32 (PcdFlashNvStorageVariableBase);

  //
  // Locate the SPI controller protocol and save it for later.
  //
  DEBUG((EFI_D_INFO, "Locating SPI Controller Protocol.\n"));
  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSpi2ProtocolGuid,
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
      // Publish the SMM SPI Device protocol for FVB service
      //
      DEBUG((EFI_D_INFO, "Installing SPI Device Protocol.\n"));
      Handle = NULL;
      Status = gSmst->SmmInstallProtocolInterface (
      &Handle,
      &gSmmSpiDeviceProtocolGuid,
      EFI_NATIVE_INTERFACE,
      &mSpiDevProtocol
      );
      if (EFI_ERROR (Status)) {
        return EFI_OUT_OF_RESOURCES;
      }

      //
      // Install protocol to inform other DXE drivers the SMM service is available.
      //
      Handle = NULL;
      Status = gBS->InstallProtocolInterface (
                      &Handle,
                      &gSmmSpiDeviceProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mSpiDevProtocol
                      );
      if (EFI_ERROR (Status)) {
        DEBUG((EFI_D_ERROR, "Unable to install SMM SPI device protocol, Status: %r\n", Status));
        return Status;
      }

      //
      // Install communication handler.
      //
      Handle = NULL;
      Status = gSmst->SmiHandlerRegister (SpiDeviceSmmHandler, &gSmmSpiDeviceProtocolGuid, &Handle);
      if (EFI_ERROR (Status)) {
        DEBUG((EFI_D_ERROR, "Unable to register SMM SPI handler, Status: %r\n", Status));
        return Status;
      }

      DEBUG((EFI_D_INFO, "SPI flash controller configured successfully\n", Status));
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
  return (BOOLEAN) ((SpiOffset >= BIOS_REGION_FLASH_OFFSET) && (SpiOffset < (BIOS_REGION_FLASH_OFFSET + PcdGet32 (PcdBiosImageSize))));
}
