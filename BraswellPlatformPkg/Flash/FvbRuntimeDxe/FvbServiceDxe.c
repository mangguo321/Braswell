/** @file
  Firmware Volume Block Driver for Tunnel Mountain Platform.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Library/UefiRuntimeLib.h>
#include "FvbService.h"

/**
  Call back function on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.

  Fixup internal data so that the driver is callable in EFI  runtime
  in virtual mode. Convert the mFvbModuleGlobal date items to there
  virtual address.

  @param[in]  Event     Event whose notification function is being invoked.
  @param[in]  Context   The context of the Notification context. Not used in
                        this call back function.

**/
VOID
EFIAPI
FvbVirtualddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_FW_VOL_INSTANCE       *FwhInstance;
  UINTN                     Index;

  //
  // Convert the base address of all the instances
  //
  for (Index = 0; Index < mFvbModuleGlobal.NumFv; Index++) {
    FwhInstance = GetFvbInstance (Index);
    EfiConvertPointer (0, (VOID **) &FwhInstance->FvBase);
  }

  EfiConvertPointer (0, (VOID **) &mFvbModuleGlobal.FvInstance);

  EfiConvertPointer (0, (VOID **)&mSpiDeviceProtocol);
}

/**
  The function installs EFI_FIRMWARE_VOLUME_BLOCK protocol
  for each FV in the system.

  @param[in]  FwhInstance   The pointer to a FW volume instance structure,
                            which contains the information about one FV.
  @param[in]  InstanceNum   The instance number which can be used as a ID
                            to locate this FwhInstance in other functions.

  @retval     VOID

**/
VOID
InstallFvbProtocol (
  IN  EFI_FW_VOL_INSTANCE *FwhInstance,
  IN  UINTN               InstanceNum
  )
{
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;
  EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader;
  EFI_STATUS                            Status;
  EFI_HANDLE                            FwbHandle;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *OldFwbInterface;

  FvbDevice = (EFI_FW_VOL_BLOCK_DEVICE *) AllocateRuntimeCopyPool (
                                            sizeof (EFI_FW_VOL_BLOCK_DEVICE),
                                            &mFvbDeviceTemplate
                                            );
  ASSERT (FvbDevice != NULL);

  FvbDevice->Instance = InstanceNum;
  FwVolHeader = &FwhInstance->VolumeHeader;

  //
  // Set up the devicepath
  //
  DEBUG ((EFI_D_INFO, "FwBlockService.c: Setting up DevicePath for 0x%lx:\n", FwhInstance->FvBase));
  if (FwVolHeader->ExtHeaderOffset == 0) {
    //
    // FV does not contains extension header, then produce MEMMAP_DEVICE_PATH
    //
    FvbDevice->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateRuntimeCopyPool (sizeof (FV_MEMMAP_DEVICE_PATH), &mFvMemmapDevicePathTemplate);
    ((FV_MEMMAP_DEVICE_PATH *) FvbDevice->DevicePath)->MemMapDevPath.StartingAddress = FwhInstance->FvBase;
    ((FV_MEMMAP_DEVICE_PATH *) FvbDevice->DevicePath)->MemMapDevPath.EndingAddress   = FwhInstance->FvBase + FwVolHeader->FvLength - 1;
  } else {
    FvbDevice->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateRuntimeCopyPool (sizeof (FV_PIWG_DEVICE_PATH), &mFvPIWGDevicePathTemplate);
    CopyGuid (
      &((FV_PIWG_DEVICE_PATH *)FvbDevice->DevicePath)->FvDevPath.FvName,
      (GUID *)(UINTN)(FwhInstance->FvBase + FwVolHeader->ExtHeaderOffset)
      );
  }

  //
  // Find a handle with a matching device path that has supports FW Block protocol
  //
  Status = gBS->LocateDevicePath (&gEfiFirmwareVolumeBlockProtocolGuid, &FvbDevice->DevicePath, &FwbHandle);
  if (EFI_ERROR (Status) ) {
    //
    // LocateDevicePath fails so install a new interface and device path
    //
    DEBUG ((EFI_D_INFO, "FwBlockService.c: LocateDevicePath failed, install new interface 0x%lx:\n", FwhInstance->FvBase));
    FwbHandle = NULL;
    Status =  gBS->InstallMultipleProtocolInterfaces (
                     &FwbHandle,
                     &gEfiFirmwareVolumeBlockProtocolGuid,
                     &FvbDevice->FwVolBlockInstance,
                     &gEfiDevicePathProtocolGuid,
                     FvbDevice->DevicePath,
                     NULL
                     );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_INFO, "FwBlockService.c: IMPI FirmwareVolBlockProt, DevPath 0x%lx: %r\n", FwhInstance->FvBase, Status));

  } else if (IsDevicePathEnd (FvbDevice->DevicePath)) {
    //
    // Device allready exists, so reinstall the FVB protocol
    //
    DEBUG ((EFI_D_INFO, "FwBlockService.c: LocateDevicePath succeeded, reinstall interface 0x%lx:\n", FwhInstance->FvBase));
    Status = gBS->HandleProtocol (
                    FwbHandle,
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    (VOID **) &OldFwbInterface
                    );
    ASSERT_EFI_ERROR (Status);

    Status =  gBS->ReinstallProtocolInterface (
                     FwbHandle,
                     &gEfiFirmwareVolumeBlockProtocolGuid,
                     OldFwbInterface,
                     &FvbDevice->FwVolBlockInstance
                     );
    ASSERT_EFI_ERROR (Status);

  } else {
    //
    // There was a FVB protocol on an End Device Path node
    //
    ASSERT (FALSE);
  }

}

EFI_STATUS
EFIAPI
DxeFvbInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     Event;

  //
  // Register for a callback when virtual addressing is updated.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  FvbVirtualddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Locate required protocol for access to flash.
  //
  Status = gBS->LocateProtocol (
                  &gSpiDeviceProtocolGuid,
                  NULL,
                  (VOID **) &mSpiDeviceProtocol
                  );
  ASSERT_EFI_ERROR(Status);

  //
  // Initialize the rest of the module.
  //
  Status = FvbInitialize ();

  return Status;
}
