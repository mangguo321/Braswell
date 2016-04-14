/** @file
  UEFI Driver Entry and Binding support.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MediaDeviceDriver.h"
#include <Protocol/EmmcCardInfoProtocol.h>

//
// MMCSDIOController Driver Global Variables
//
EFI_DRIVER_BINDING_PROTOCOL gMediaDeviceDriverBinding = {
  MediaDeviceDriverBindingSupported,
  MediaDeviceDriverBindingStart,
  MediaDeviceDriverBindingStop,
  0x10,
  NULL,
  NULL
};

EFI_EMMC_CARD_INFO_PROTOCOL *gEfiEmmcCardInfo = NULL;

/**
  Entry point for Media Device EFI drivers.

  @param[in] ImageHandle          EFI_HANDLE
  @param[in] SystemTable          EFI_SYSTEM_TABLE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        The function failed to complete

**/
EFI_STATUS
EFIAPI
MediaDeviceDriverEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  return EfiLibInstallAllDriverProtocols (
           ImageHandle,
           SystemTable,
           &gMediaDeviceDriverBinding,
           ImageHandle,
           &gMediaDeviceComponentName,
           NULL,
           NULL
           );
}

/**
  Test to see if this driver supports ControllerHandle. Any ControllerHandle
  that has installed will be supported.

  @param[in]  This                  Protocol instance pointer.
  @param[in]  Controller            Handle of device to test.
  @param[in]  RemainingDevicePath   Not used.

  @retval  EFI_SUCCESS              This driver supports this device.
  @retval  EFI_UNSUPPORTED          This driver does not support this device.

**/
EFI_STATUS
EFIAPI
MediaDeviceDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_SD_HOST_IO_PROTOCOL   *SdHostIo;

  //
  // Test whether there is SDHostIO Protocol attached on the controller handle.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiSdHostIoProtocolGuid,
                  (VOID **) &SdHostIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiSdHostIoProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

Exit:
  return Status;
}

/**
  Starting the Media Device Driver

  @param[in]  This                  Protocol instance pointer.
  @param[in]  Controller            Handle of device to test
  @param[in]  RemainingDevicePath   Not used

  @retval  EFI_SUCCESS              Supports this device.
  @retval  EFI_UNSUPPORTED          Do not support this device.
  @retval  EFI_DEVICE_ERROR         Cannot be started due to device Error.
  @retval  EFI_OUT_OF_RESOURCES     Cannot allocate resources.

**/
EFI_STATUS
EFIAPI
MediaDeviceDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN EFI_HANDLE                      Controller,
  IN EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath
  )
{
  EFI_STATUS                   Status;
  EFI_SD_HOST_IO_PROTOCOL      *SdHostIo = NULL;
  CARD_DATA                    *CardData;
  UINTN                        Loop;
#if 1
  UINTN                        SdCtlrDev, SdCtlrFunc, SdCtlrBus, Seg;
  EFI_STATUS                   LocatePciStatus;
#endif
  UINTN                        Pages = 0;
  EFI_EMMC_CARD_INFO_PROTOCOL  *EfiEmmcCardInfoRegister;

  EfiEmmcCardInfoRegister = NULL;
  CardData = NULL;
  //
  //Alloc memory for EfiEmmcCardInfoRegister variable
  //
  EfiEmmcCardInfoRegister = (EFI_EMMC_CARD_INFO_PROTOCOL *)AllocateZeroPool(sizeof(EFI_EMMC_CARD_INFO_PROTOCOL));
  if(EfiEmmcCardInfoRegister == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  //
  // Open PCI I/O Protocol and save pointer to open protocol
  // in private data area.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiSdHostIoProtocolGuid,
                  (VOID **) &SdHostIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Fail to open gEfiSdHostIoProtocolGuid \n"));
    goto Exit;
  }

  Status = SdHostIo->DetectCardAndInitHost (SdHostIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Fail to DetectCardAndInitHost %r\n", Status));
    goto Exit;
  }

  CardData = (CARD_DATA*)AllocateZeroPool (sizeof (CARD_DATA));
  if (CardData == NULL) {
    Status =  EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Fail to AllocateZeroPool(CARD_DATA) \n"));
    goto Exit;
  }

  ASSERT (SdHostIo->HostCapability.BoundarySize >= 4 * 1024);
  CardData->RawBufferPointer = (UINT8*)(UINTN)0xffffffff;
  Pages = (2 * SdHostIo->HostCapability.BoundarySize);

  DEBUG ((EFI_D_ERROR, "CardData->RawBufferPointer = 0x%x \n",CardData->RawBufferPointer));
  DEBUG ((EFI_D_ERROR, "requesting 0x%x pages \n",EFI_SIZE_TO_PAGES(Pages)));
  //
  // Allocated the buffer under 4G
  //
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiBootServicesData,
                  EFI_SIZE_TO_PAGES(Pages),
                  (EFI_PHYSICAL_ADDRESS *)(&(CardData->RawBufferPointer))
                  );
  DEBUG ((EFI_D_ERROR, "CardData->RawBufferPointer = 0x%x \n",CardData->RawBufferPointer));
  if (!EFI_ERROR (Status)) {
    CardData->RawBufferPointer = ZeroMem (CardData->RawBufferPointer, EFI_SIZE_TO_PAGES(Pages));
  } else {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Fail to AllocateZeroPool(2*x) \n"));
    Status =  EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CardData->AlignedBuffer = CardData->RawBufferPointer - ((UINTN)(CardData->RawBufferPointer) & (SdHostIo->HostCapability.BoundarySize - 1)) + SdHostIo->HostCapability.BoundarySize;

  CardData->Signature = CARD_DATA_SIGNATURE;
  CardData->SdHostIo  = SdHostIo;
  CardData->Handle = Controller;
  for (Loop = 0; Loop < MAX_NUMBER_OF_PARTITIONS; Loop++) {
    CardData->Partitions[Loop].Signature = CARD_PARTITION_SIGNATURE;
  }
#if 1
  LocatePciStatus = gBS->HandleProtocol (
                          Controller,
                          &gEfiPciIoProtocolGuid,
                          (VOID**) &(CardData->PciIo)
                          );
  if (EFI_ERROR (LocatePciStatus)) {
    DEBUG ((EFI_D_ERROR, "Get PCI IO Failed\n"));
  }

  LocatePciStatus = CardData->PciIo->GetLocation (
                                      CardData->PciIo,
                                      &Seg,
                                      &SdCtlrBus,
                                      &SdCtlrDev,
                                      &SdCtlrFunc
                                      );
  if (EFI_ERROR (LocatePciStatus)) {
    DEBUG ((EFI_D_ERROR, "Get PCI Bus Dev Func Fail\n"));
  } else {
    DEBUG ((EFI_D_ERROR, "Bus: %02x  Dev: %02x  Func: %02x\n", SdCtlrBus, SdCtlrDev, SdCtlrFunc));
  }
#endif

  Status = MMCSDCardInit (CardData);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Fail to MMCSDCardInit \n"));
    goto Exit;
  }

  DEBUG ((EFI_D_INFO, "MediaDeviceDriverBindingStart: MMC SD card\n"));
  Status = MMCSDBlockIoInit (CardData);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Card BlockIo init failed\n"));
    goto Exit;
  }

  Status = MediaDeviceDriverInstallBlockIo (This, CardData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Fail to install gEfiBlockIoProtocolGuid \n"));
    goto Exit;
  }

  //
  // Component name protocol
  //
  Status = AddUnicodeString (
             "eng",
             gMediaDeviceComponentName.SupportedLanguages,
             &CardData->ControllerNameTable,
             L"MMC/SD Media Device"
             );
  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
    for (Loop = 0; Loop < MAX_NUMBER_OF_PARTITIONS; Loop++) {
      if (!CardData->Partitions[Loop].Present) {
        continue;
      }
      gBS->UninstallMultipleProtocolInterfaces (
             CardData->Partitions[Loop].Handle,
             &gEfiBlockIoProtocolGuid,
             &CardData->Partitions[Loop].BlockIo,
             &gEfiDevicePathProtocolGuid,
             CardData->Partitions[Loop].DevPath,
             NULL
             );
    }  
    goto Exit;
  }

  if (EfiEmmcCardInfoRegister != NULL) {

    //
    //  assign to protocol
    //
    EfiEmmcCardInfoRegister->CardData = CardData;

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Controller,
                    &gEfiEmmcCardInfoProtocolGuid,
                    EfiEmmcCardInfoRegister,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: Install eMMC Card info protocol failed\n"));
      for (Loop = 0; Loop < MAX_NUMBER_OF_PARTITIONS; Loop++) {
        if (!CardData->Partitions[Loop].Present) {
          continue;
        }
        gBS->UninstallMultipleProtocolInterfaces (
               CardData->Partitions[Loop].Handle,
               &gEfiBlockIoProtocolGuid,
               &CardData->Partitions[Loop].BlockIo,
               &gEfiDevicePathProtocolGuid,
               CardData->Partitions[Loop].DevPath,
               NULL
               );
      }      
      
      goto Exit;
    }

    gEfiEmmcCardInfo = EfiEmmcCardInfoRegister;
  }

  DEBUG ((EFI_D_INFO, "MediaDeviceDriverBindingStart: Started\n"));

Exit:
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStart: End with failure\n"));
    if (CardData != NULL && SdHostIo != NULL) {
      if (CardData->RawBufferPointer != NULL) {
        gBS->FreePages ((EFI_PHYSICAL_ADDRESS)(UINTN) CardData->RawBufferPointer, EFI_SIZE_TO_PAGES(Pages));
      }
      FreePool (CardData);
    }
    gBS->CloseProtocol (
           Controller,
           &gEfiSdHostIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
  }

  return Status;
}

/**
  Stop this driver on ControllerHandle. Support stoping any child handles
  created by this driver.

  @param[in]  This                  Protocol instance pointer.
  @param[in]  Controller            Handle of device to stop driver on
  @param[in]  NumberOfChildren      Number of Children in the ChildHandleBuffer
  @param[in]  ChildHandleBuffer     List of handles for the children we need to stop.

  @retval  EFI_SUCCESS              Success
  @retval  EFI_DEVICE_ERROR         Fail

**/
EFI_STATUS
EFIAPI
MediaDeviceDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN  EFI_HANDLE                      Controller,
  IN  UINTN                           NumberOfChildren,
  IN  EFI_HANDLE                      *ChildHandleBuffer
  )

{
  EFI_STATUS                Status;
  CARD_DATA                 *CardData;
  BOOLEAN                   AllChildrenStopped;
  UINTN                     Index;
  UINTN                     Pages = 0;

  Status = EFI_SUCCESS;
  CardData = gEfiEmmcCardInfo->CardData;

  if (NumberOfChildren == 0) {
    Status = gBS->UninstallMultipleProtocolInterfaces (
                    Controller,
                    &gEfiEmmcCardInfoProtocolGuid,
                    gEfiEmmcCardInfo,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStop: UNINSTALL gEfiEmmcCardInfoProtocolGuid FAILURE\n"));
      return Status;
    }

    FreeUnicodeStringTable (CardData->ControllerNameTable);
    
    Pages = (2 * (CardData->SdHostIo->HostCapability.BoundarySize));
    if (CardData->RawBufferPointer != NULL) {
      FreePages (CardData->RawBufferPointer, EFI_SIZE_TO_PAGES(Pages));
    }

    Status = gBS->CloseProtocol (
                    Controller,
                    &gEfiSdHostIoProtocolGuid,
                    This->DriverBindingHandle,
                    Controller
                    );

    if (MediaDeviceDriverAllPartitionNotPresent(CardData)) {
      FreePool (CardData);
      FreePool (gEfiEmmcCardInfo);
      gEfiEmmcCardInfo = NULL;
    }
  
    return Status;
  }

  AllChildrenStopped = TRUE;
  for (Index = 0; Index < NumberOfChildren; Index++) {
    Status = MediaDeviceDriverUninstallBlockIo(This, CardData, ChildHandleBuffer[Index]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStop: UNINSTALL block io FAILURE\n"));
      AllChildrenStopped = FALSE;
      break;
    }
  }

  return Status;
}

BOOLEAN
MediaDeviceDriverAllPartitionNotPresent (
  IN  CARD_DATA    *CardData
  )
{
  BOOLEAN             AllPartitionNotPresent;
  UINTN               Loop;
  MMC_PARTITION_DATA  *Partition;

  Partition = CardData->Partitions;

  AllPartitionNotPresent = TRUE;

  for (Loop = 0; Loop < MAX_NUMBER_OF_PARTITIONS; Partition++, Loop++) {
    if (Partition->Present) {
      AllPartitionNotPresent = FALSE;
      break;
    }
  }

  return AllPartitionNotPresent;
}

STATIC
struct {
  DEVICE_LOGICAL_UNIT_DEVICE_PATH  LogicalUnit;
  EFI_DEVICE_PATH_PROTOCOL  End;
} SdCardDevPathTemplate = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_DEVICE_LOGICAL_UNIT_DP,
      {
        sizeof (DEVICE_LOGICAL_UNIT_DEVICE_PATH),
        0
      },
    },
    0
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

EFI_STATUS
MediaDeviceDriverInstallBlockIo (
  IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN  CARD_DATA    *CardData
  )
{
  EFI_STATUS                Status;
  UINT8                     Loop;
  MMC_PARTITION_DATA        *Partition;
  EFI_DEVICE_PATH_PROTOCOL  *MainPath;
  EFI_SD_HOST_IO_PROTOCOL   *SdHostIo = NULL;

  Partition = CardData->Partitions;

  Status = gBS->HandleProtocol (
                  CardData->Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &MainPath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Loop = 0; Loop < MAX_NUMBER_OF_PARTITIONS; Partition++, Loop++) {
    if (!Partition->Present) {
      continue;
    }

    DEBUG ((EFI_D_INFO, "MediaDeviceDriverInstallBlockIo: Installing Block I/O for partition %d\n", Loop));

    Partition->Handle = NULL;
    Partition->CardData = CardData;

    SdCardDevPathTemplate.LogicalUnit.Lun = Loop;
    Partition->DevPath =
      AppendDevicePath (
        MainPath,
        (EFI_DEVICE_PATH_PROTOCOL *) &SdCardDevPathTemplate
        );
    if (Partition->DevPath == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    Status = gBS->InstallProtocolInterface (
                    &(Partition->Handle),
                    &gEfiDevicePathProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    Partition->DevPath
                    );
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = gBS->InstallProtocolInterface (
                    &(Partition->Handle),
                    &gEfiBlockIoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &Partition->BlockIo
                    );

    //
    // Open parent controller by child
    //
    Status = gBS->OpenProtocol (
                    CardData->Handle,
                    &gEfiSdHostIoProtocolGuid,
                    (VOID **) &SdHostIo,
                    This->DriverBindingHandle,
                    Partition->Handle,
                    EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
                    );
  }

  return Status;
}

EFI_STATUS
MediaDeviceDriverUninstallBlockIo (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  CARD_DATA      *CardData,
  IN  EFI_HANDLE     Handle
  )
{
  EFI_STATUS          Status;
  UINTN               Loop;
  MMC_PARTITION_DATA  *Partition;

  Partition = CardData->Partitions;

  Status = EFI_SUCCESS;

  for (Loop = 0; Loop < MAX_NUMBER_OF_PARTITIONS; Partition++, Loop++) {
    if (!Partition->Present || Partition->Handle != Handle) {
     continue;
    }
#if 1
    //
    // Close SdHostIoProtocol by child
    //
    Status = gBS->CloseProtocol (
                    CardData->Handle,
                    &gEfiSdHostIoProtocolGuid,
                    This->DriverBindingHandle,
                    Partition->Handle
                    );
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "CloseProtocol gEfiSdHostIoProtocolGuid FAILURE \n"));
      return Status;
    }

    Status = gBS->UninstallMultipleProtocolInterfaces (
                    Partition->Handle,
                    &gEfiBlockIoProtocolGuid,
                    &Partition->BlockIo,
                    &gEfiDevicePathProtocolGuid,
                    Partition->DevPath,
                    NULL
                    );
    Partition->Present = FALSE;
    DEBUG ((EFI_D_ERROR, "MediaDeviceDriverBindingStop gEfiBlockIoProtocolGuid removed.  %x\n", Status));
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "MediaDeviceDriverUninstallBlockIo UNISTALL FAILURE \n"));
    }
    
#else 
    Status = gBS->UninstallProtocolInterface (
                    Partition->Handle,
                    &gEfiBlockIoProtocolGuid,
                    &Partition->BlockIo
                    );
#endif
     return Status;
  }

  return EFI_INVALID_PARAMETER;
}

