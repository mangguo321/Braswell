/** @file
  ISP Platform Driver

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <ISPDxe.h>

EFI_HANDLE         mImageHandle;

///
/// ISP reserved memory length
///
#define ISP_MEMORY_LENGTH    0x400000
///
/// 4M alignment
///
#define ISP_MMIO_ALIGNMENT   22

EFI_GLOBAL_NVS_AREA_PROTOCOL      *GlobalNvsArea;

STATIC
VOID
EFIAPI
OnReadyToBootISP (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS ISPBase;

  //
  // Avoid the event will run twice
  //
  gBS->CloseEvent (Event);
  //
  // Allocated memeory for ISP
  //
  ISPBase = 0xFFFFFFFF;
  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchBottomUp,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  ISP_MMIO_ALIGNMENT,
                  ISP_MEMORY_LENGTH,
                  &ISPBase,
                  mImageHandle,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Update ISP Base in GlobalNvs
  //
  DEBUG ((EFI_D_ERROR, "=============ISPBase=%x==============\n\n", ISPBase));
  GlobalNvsArea->Area->ISPAddr = (UINT32) ISPBase;
  return;
}

/**
  Entry point for Acpi platform driver.

  @param[in]  ImageHandle  -  A handle for the image that is initializing this driver.
  @param[in]  SystemTable  -  A pointer to the EFI system table.

  @retval  EFI_SUCCESS           -  Driver initialized successfully.
  @retval  EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  @retval  EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
ISPDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_EVENT             Event;

  //
  // Locate the Global NVS Protocol.
  //
  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &GlobalNvsArea
                  );
  //
  // Return if ISP disabled.
  //
  if (GlobalNvsArea->Area->ISPDevSel == 0) {
    DEBUG ((EFI_D_ERROR, "ISP Feature Disable !!\n\n"));
    return EFI_SUCCESS;
  }

  mImageHandle = ImageHandle;
  //
  // Update ISP Base Address
  //
  if (GlobalNvsArea->Area->ISPDevSel == 1) {
    Status = EfiCreateEventReadyToBootEx (
               TPL_NOTIFY,
               OnReadyToBootISP,
               NULL,
               &Event
               );
  }

  return Status;
}
