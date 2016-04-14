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

#include "SdHostDriver.h"
#include <Library/PcdLib.h>

//
// MMCSDIOController Driver Global Variables
//
EFI_DRIVER_BINDING_PROTOCOL gSdHostDriverBinding = {
  SdHostDriverBindingSupported,
  SdHostDriverBindingStart,
  SdHostDriverBindingStop,
  0x10,
  NULL,
  NULL
};

EFI_SD_HOST_IO_PROTOCOL  mSdHostIo = {
  EFI_SD_HOST_IO_PROTOCOL_REVISION_01,
  {
    0, // HighSpeedSupport
    0, // V18Support
    0, // V30Support
    0, // V33Support
    0, // SDR50Support
    0, // SDR104Support
    0, // DDR50Support
    0, // Reserved0
    0, // BusWidth4
    0, // BusWidth8
    0, // Reserved1
    0, // SDMASupport
    0, // ADMA2Support
    0, // DmaMode
    0, // ReTune Timer
    0, // ReTune Mode
    0, // Reserved2
    (512 * 1024) // BoundarySize 512 KB
  },
  SendCommand,
  SetClockFrequency,
  SetBusWidth,
  SetHostVoltage,
  SetHostDdrMode,
  SetHostSdrMode,
  ResetSdHost,
  EnableAutoStopCmd,
  DetectCardAndInitHost,
  SetBlockLength,
  SetupDevice,
  SetHostSpeedMode
};

/**
  Entry point for SD Host EFI drivers.

  @param[in] ImageHandle          EFI_HANDLE
  @param[in] SystemTable          EFI_SYSTEM_TABLE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        The function failed to complete
**/
EFI_STATUS
EFIAPI
SdHostDriverEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  return EfiLibInstallAllDriverProtocols (
           ImageHandle,
           SystemTable,
           &gSdHostDriverBinding,
           ImageHandle,
           &gSdHostComponentName,
           NULL,
           NULL
           );
}

/**
  Test to see if this SD Host driver supports ControllerHandle.
  Any ControllerHandle that has installed will be supported.

  @param[in] This                 Protocol instance pointer
  @param[in] Controller           Handle of device to test
  @param[in] RemainingDevicePath  Not used

  @retval EFI_SUCCESS             This driver supports this device
  @retval EFI_UNSUPPORTED         This driver does not support this device
**/
EFI_STATUS
EFIAPI
SdHostDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  EFI_SD_HOST_IO_PROTOCOL   *SdHostIo;
  PCI_CLASSC                PciClass;
  UINT32                    VidDid;
  UINT32                    Bar0 = 0;
  UINT32                    Bar1 = 0;
  UINTN                     Seg, Bus, Dev, Func;

  //
  // Verify the SD IO Protocol
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiSdHostIoProtocolGuid,
                  (VOID **)&SdHostIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if ( !EFI_ERROR ( Status )) {
    DEBUG (( DEBUG_INFO, "SdHost controller already started, Controller:0x%016Lx\r\n",  (UINT64)(UINTN)Controller));
    Status = EFI_ALREADY_STARTED;
    return Status;
  }

  //
  // Test whether there is PCI IO Protocol attached on the controller handle.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID**) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        PCI_VENDOR_ID_OFFSET,
                        1,
                        &VidDid
                        );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        PCI_CLASSCODE_OFFSET,
                        sizeof (PCI_CLASSC) / sizeof (UINT8),
                        &PciClass
                        );
  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0x10,
                        1,
                        &Bar0
                        );
  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0x10,
                        1,
                        &Bar1
                        );
  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ( !(VidDid == 0x22948086 || VidDid == 0x0F508086 || VidDid == 0x22968086)) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((PciClass.BaseCode != PCI_CLASS_SYSTEM_PERIPHERAL) ||
     (PciClass.SubClassCode != PCI_SUBCLASS_SD_HOST_CONTROLLER) ||
     ((PciClass.PI != PCI_IF_STANDARD_HOST_NO_DMA) && (PciClass.PI != PCI_IF_STANDARD_HOST_SUPPORT_DMA))
     ) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = PciIo->GetLocation (
                    PciIo,
                    &Seg,
                    &Bus,
                    &Dev,
                    &Func
                    );
  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported0: Seg %d, bus:%d,Dev:%d,Func:%d\n", Seg, Bus, Dev,Func));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported1: VidDid %08x\n", VidDid));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported1: Base Code %x\n", PciClass.BaseCode));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported1: SubClassCode %x\n", PciClass.SubClassCode));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported1: PI %x\n", PciClass.PI));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported0: MEMIO Base0 %x\n", Bar0));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingSupported0: MEMIO Base1 %x\n", Bar1));

Exit:
  gBS->CloseProtocol (
         Controller,
         &gEfiPciIoProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );
  return Status;
}

/**
  Starting the SD Host Driver

  @param[in] This                 Protocol instance pointer
  @param[in] Controller           Handle of device to start
  @param[in] RemainingDevicePath  Not used

  @retval EFI_SUCCESS             This driver start this device
  @retval EFI_UNSUPPORTED         This driver does not support this device
  @retval EFI_DEVICE_ERROR        This driver cannot be started due to device Error
  @retval EFI_OUT_OF_RESOURCES    This driver cannot allocate resources
**/
EFI_STATUS
EFIAPI
SdHostDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN EFI_HANDLE                      Controller,
  IN EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath
  )
{
  EFI_PCI_IO_PROTOCOL   *PciIo;
  EFI_STATUS            Status;
  SDHOST_DATA           *SdHostData;
  UINT32                Data;
  UINT16                Data16;
  UINT32                VidDid;
  UINT32                Bar0 = 0;
  UINT32                Bar1 = 0;
  UINTN                 Seg, Bus, Dev, Func;

  SdHostData = NULL;
  Data       = 0;
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Start\n"));

  //
  // Open PCI I/O Protocol and save pointer to open protocol
  // in private data area.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Enable the SD Host Controller MMIO space
  //
  Status = PciIo->Attributes (
                    PciIo,
                    EfiPciIoAttributeOperationEnable,
                    EFI_PCI_DEVICE_ENABLE,
                    NULL
                    );
  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  SdHostData = (SDHOST_DATA*)AllocateZeroPool (sizeof (SDHOST_DATA));
  if (SdHostData == NULL) {
    Status =  EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  SdHostData->Signature   = SDHOST_DATA_SIGNATURE;
  SdHostData->PciIo       = PciIo;

  CopyMem (&SdHostData->SdHostIo, &mSdHostIo, sizeof (EFI_SD_HOST_IO_PROTOCOL));

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        PCI_VENDOR_ID_OFFSET,
                        1,
                        &VidDid
                        );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: VidDid = 0x%08x\n", VidDid));

  Status = PciIo->GetLocation (
                    PciIo,
                    &Seg,
                    &Bus,
                    &Dev,
                    &Func
                    );
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Seg %d, bus:%d,Dev:%d,Func:%d\n", Seg, Bus, Dev,Func));

  SdHostData->PciVid = (UINT16)(VidDid & 0xffff);
  SdHostData->PciDid = (UINT16)(VidDid >> 16);

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0x10,
                        1,
                        &Bar0
                        );

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0x14,
                        1,
                        &Bar1
                        );

  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: MEMIO Base0 %x\n", Bar0));
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: MEMIO Base1 %x\n", Bar1));

  SdHostData->SdHostIo.ResetSdHost (&SdHostData->SdHostIo, Reset_All);
  SdHostData->EnableVerboseDebug = FALSE;

  PciIo->Mem.Read (
               PciIo,
               EfiPciIoWidthUint16,
               0,
               (UINT64)MMIO_CTRLRVER,
               1,
               &Data16
               );
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: MMIO_CTRLRVER = 0x%08x\n", Data16));

  SdHostData->ControllerVersion = Data16 & 0xFF;
  switch (SdHostData->ControllerVersion) {
    case 0: DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: SD Host Controller Version 1.0\n")); break;
    case 1: DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: SD Host Controller Version 2.0\n")); break;
    case 2: DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: SD Host Controller Version 3.0\n")); break;
    default:
      DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Unknown SD Host Controller Version, Stopping Driver!!\n"));
      goto Exit;
  }

  PciIo->Mem.Read (
               PciIo,
               EfiPciIoWidthUint32,
               0,
               (UINT32)MMIO_CAP,
               1,
               &Data
               );
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Low MMIO_CAP = 0x%08x\n", Data));

  if ((Data & BIT18) != 0) {
    SdHostData->SdHostIo.HostCapability.BusWidth8 = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: BusWidth8\n"));
  }

  SdHostData->SdHostIo.HostCapability.BusWidth4 = TRUE;
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: BusWidth4\n"));

  if ((Data & BIT19) != 0) {
    SdHostData->SdHostIo.HostCapability.ADMA2Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: ADMA2Support\n"));
  }

  if ((Data & BIT21) != 0) {
    SdHostData->SdHostIo.HostCapability.HighSpeedSupport = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: HighSpeedSupport\n"));
  }

  if ((Data & BIT22) != 0) {
    SdHostData->SdHostIo.HostCapability.SDMASupport = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: SDMASupport\n"));
  }

  if ((Data & BIT24) != 0) {
    SdHostData->SdHostIo.HostCapability.V33Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: V33Support\n"));
  }

  if ((Data & BIT25) != 0) {
    SdHostData->SdHostIo.HostCapability.V30Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: V30Support\n"));
  }

  if ((Data & BIT26) != 0) {
    SdHostData->SdHostIo.HostCapability.V18Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: V18Support\n"));
  }

  if (((Data) & (BIT30 | BIT31)) == 0) {
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Removable Card Slot\n"));
  } else if ((Data & BIT30) && (Data & (~BIT31))) {
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Embedded Slot for One Device\n"));
  }
  SdHostData->BaseClockInMHz = (Data >> 8) & 0xFF;

  PciIo->Mem.Read (
               PciIo,
               EfiPciIoWidthUint32,
               0,
               (UINT32)(MMIO_CAP + 4),
               1,
               &Data
               );

  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: High MMIO_CAP = 0x%08x\n", Data));

  if ((Data & 0x1<<(32-32)) != 0) {
    SdHostData->SdHostIo.HostCapability.SDR50Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: SDR50Support\n"));
  }

  if ((Data & 0x1<<(33-32)) != 0) {
    SdHostData->SdHostIo.HostCapability.SDR104Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: SDR104Support\n"));
  }

  if ((Data & 0x1<<(34-32)) != 0) {
    SdHostData->SdHostIo.HostCapability.DDR50Support = TRUE;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: DDR50Support\n"));
  }

  if (SdHostData->ControllerVersion >= 2) {
    SdHostData->SdHostIo.HostCapability.ReTuneMode = (Data >> (46-32)) & 0x3;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart:  ReTuneMode = 0x%08x\n", SdHostData->SdHostIo.HostCapability.ReTuneMode));

    SdHostData->SdHostIo.HostCapability.ReTuneTimer = (Data>>(40-32)) & 0xF;
    DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart:  ReTuneTimer = 0x%08x\n", SdHostData->SdHostIo.HostCapability.ReTuneTimer));
  }

  SdHostData->BlockLength    = BLOCK_SIZE;
  SdHostData->IsAutoStopCmd  = TRUE;

  Status = gBS->InstallProtocolInterface (
                  &Controller,
                  &gEfiSdHostIoProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &SdHostData->SdHostIo
                  );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Component name protocol
  //
  Status = AddUnicodeString (
             "eng",
             gSdHostComponentName.SupportedLanguages,
             &SdHostData->ControllerNameTable,
             L"SD Host Controller"
             );
  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
    gBS->UninstallProtocolInterface (
           Controller,
           &gEfiSdHostIoProtocolGuid,
           &SdHostData->SdHostIo
           );
  }

  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStart: Host Started\n"));

Exit:
  if (EFI_ERROR (Status)) {
    if (SdHostData != NULL) {
      FreePool (SdHostData);
    }
    gBS->CloseProtocol (
           Controller,
           &gEfiPciIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
  }
  return Status;
}

/**
  Stop SD Host driver on ControllerHandle. Support stopping any child handles
  created by this driver.

  @param[in] This                 Protocol instance pointer
  @param[in] Controller           Handle of device to stop driver on
  @param[in] NumberOfChildren     Number of Children in the ChildHandleBuffer
  @param[in] ChildHandleBuffer    List of handles for the children we need to stop

  @retval EFI_SUCCESS             This driver stop this device
  @retval EFI_DEVICE_ERROR        This driver cannot be stop due to device Error
**/
EFI_STATUS
EFIAPI
SdHostDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN  EFI_HANDLE                      Controller,
  IN  UINTN                           NumberOfChildren,
  IN  EFI_HANDLE                      *ChildHandleBuffer
  )
{
  EFI_STATUS                Status;
  EFI_SD_HOST_IO_PROTOCOL  *SdHostIo;
  SDHOST_DATA              *SdHostData;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiSdHostIoProtocolGuid,
                  (VOID**) &SdHostIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    goto _exit_SdHostDriverBindingStop;
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiPciIoProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  SdHostData  = SDHOST_DATA_FROM_THIS(SdHostIo);

  //
  // Uninstall Block I/O protocol from the device handle
  //
  Status = gBS->UninstallProtocolInterface (
                  Controller,
                  &gEfiSdHostIoProtocolGuid,
                  &SdHostData->SdHostIo
                  );
  if (EFI_ERROR (Status)) {
    goto _exit_SdHostDriverBindingStop;
  }
  FreeUnicodeStringTable (SdHostData->ControllerNameTable);

  FreePool (SdHostData);
_exit_SdHostDriverBindingStop:
  DEBUG ((EFI_D_INFO, "SdHostDriverBindingStop exited with Status %r\n", Status));
  return Status;
}
