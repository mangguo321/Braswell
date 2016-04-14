/** @file
  SMM Firmware Volume Block Driver for the Tunnel Creek Platform.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiSmm.h>
#include <Protocol/SmmSpiDevice.h>
#include <Library/SmmServicesTableLib.h>
#include "FvbSmmCommon.h"
#include "FvbService.h"

/**
  Communication service SMI Handler entry.

  This SMI handler provides communication services with the SMM FVB runtime driver.

  @param[in]     DispatchHandle    The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]     RegisterContext   Points to an optional handler context which was specified when the
                                   handler was registered.
  @param[in, out] CommBuffer       A pointer to a collection of data in memory that will
                                   be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize   The size of the CommBuffer.

  @retval EFI_SUCCESS              The interrupt was handled successfully.
  @retval EFI_UNSUPPORTED          The request was not supported by the handler.

**/
EFI_STATUS
EFIAPI
FvbSmmHandler (
  IN       EFI_HANDLE                 DispatchHandle,
  IN CONST VOID                       *RegisterContext,
  IN OUT   VOID                       *CommBuffer,
  IN OUT   UINTN                      *CommBufferSize
  )
{
  EFI_STATUS                           Status;
  SMM_FVB_COMMUNICATE_FUNCTION_HEADER  *SmmFvbFunctionHeader;
  SMM_FVB_ATTRIBUTES_HEADER            *SmmFvbAttributesHeader;
  SMM_FVB_PHYSICAL_ADDRESS_HEADER      *SmmFvbPhysicalAddressHeader;
  SMM_FVB_BLOCK_SIZE_HEADER            *SmmFvbBlockSizeHeader;
  SMM_FVB_READ_WRITE_HEADER            *SmmFvbReadWriteHeader;
  SMM_FVB_BLOCKS_HEADER                *SmmFvbBlocksHeader;

  ASSERT (CommBuffer != NULL);

  //
  // Determine the function that is being requested and execute it.
  //
  SmmFvbFunctionHeader = (SMM_FVB_COMMUNICATE_FUNCTION_HEADER *) CommBuffer;
  switch (SmmFvbFunctionHeader->Function) {
    case EFI_FUNCTION_GET_ATTRIBUTES:
      SmmFvbAttributesHeader = (SMM_FVB_ATTRIBUTES_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolGetAttributes (
                 SmmFvbAttributesHeader->SmmFvb,
                 &SmmFvbAttributesHeader->Attributes
                 );
      break;
    case EFI_FUNCTION_SET_ATTRIBUTES:
      SmmFvbAttributesHeader = (SMM_FVB_ATTRIBUTES_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolSetAttributes (
                 SmmFvbAttributesHeader->SmmFvb,
                 &SmmFvbAttributesHeader->Attributes
                 );
      break;
    case EFI_FUNCTION_GET_PHYSICAL_ADDRESS:
      SmmFvbPhysicalAddressHeader = (SMM_FVB_PHYSICAL_ADDRESS_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolGetPhysicalAddress (
                 SmmFvbPhysicalAddressHeader->SmmFvb,
                 &SmmFvbPhysicalAddressHeader->Address
                 );
      break;
    case EFI_FUNCTION_GET_BLOCK_SIZE:
      SmmFvbBlockSizeHeader = (SMM_FVB_BLOCK_SIZE_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolGetBlockSize (
                 SmmFvbBlockSizeHeader->SmmFvb,
                 SmmFvbBlockSizeHeader->Lba,
                 &SmmFvbBlockSizeHeader->BlockSize,
                 &SmmFvbBlockSizeHeader->NumOfBlocks
                 );
      break;
    case EFI_FUNCTION_READ:
      SmmFvbReadWriteHeader = (SMM_FVB_READ_WRITE_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolRead (
                 SmmFvbReadWriteHeader->SmmFvb,
                 SmmFvbReadWriteHeader->Lba,
                 SmmFvbReadWriteHeader->Offset,
                 &SmmFvbReadWriteHeader->NumBytes,
                 (UINT8 *) (SmmFvbReadWriteHeader + 1)
                 );
      break;
    case EFI_FUNCTION_WRITE:
      SmmFvbReadWriteHeader = (SMM_FVB_READ_WRITE_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolWrite (
                 SmmFvbReadWriteHeader->SmmFvb,
                 SmmFvbReadWriteHeader->Lba,
                 SmmFvbReadWriteHeader->Offset,
                 &SmmFvbReadWriteHeader->NumBytes,
                 (UINT8 *) (SmmFvbReadWriteHeader + 1)
                 );
      break;
    case EFI_FUNCTION_ERASE_BLOCKS:
      SmmFvbBlocksHeader = (SMM_FVB_BLOCKS_HEADER *) SmmFvbFunctionHeader->Data;
      Status = FvbProtocolEraseBlocks (
                 SmmFvbBlocksHeader->SmmFvb,
                 SmmFvbBlocksHeader->StartLba,
                 SmmFvbBlocksHeader->NumOfLba,
                 EFI_LBA_LIST_TERMINATOR,
                 0
                 );
      break;
    default:
      ASSERT (FALSE);
      Status = EFI_UNSUPPORTED;
  }

  //
  // Set the return value.
  //
  SmmFvbFunctionHeader->ReturnStatus = Status;

  return EFI_SUCCESS;
}

/**
  The function installs EFI_SMM_FIRMWARE_VOLUME_BLOCK protocol
  for each FV in the system.

  @param[in]  FwhInstance   The pointer to a FW volume instance structure,
                            which contains the information about one FV.
  @param[in]  InstanceNum   The instance number which can be used as a ID
                            to locate this FwhInstance in other functions.

  @retval     VOID

**/
VOID
InstallFvbProtocol (
  IN  EFI_FW_VOL_INSTANCE               *FwhInstance,
  IN  UINTN                             InstanceNum
  )
{
  EFI_FW_VOL_BLOCK_DEVICE               *FvbDevice;
  EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader;
  EFI_STATUS                            Status;
  EFI_HANDLE                            FvbHandle;

  FvbDevice = (EFI_FW_VOL_BLOCK_DEVICE *) AllocateRuntimeCopyPool (
                                            sizeof (EFI_FW_VOL_BLOCK_DEVICE),
                                            &mFvbDeviceTemplate
                                            );
  ASSERT (FvbDevice != NULL);

  FvbDevice->Instance = InstanceNum;
  FwVolHeader         = &FwhInstance->VolumeHeader;

  //
  // Set up the devicepath
  //
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
  // Install the SMM Firmware Volume Block Protocol and Device Path Protocol
  //
  FvbHandle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &FvbHandle,
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &FvbDevice->FwVolBlockInstance
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmInstallProtocolInterface (
                    &FvbHandle,
                    &gEfiDevicePathProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    FvbDevice->DevicePath
                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Notify the Fvb wrapper driver SMM fvb is ready
  //
  FvbHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &FvbHandle,
                  &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &FvbDevice->FwVolBlockInstance
                  );
}

/**
  The driver entry point for SMM Firmware Volume Block Driver.

  The function does the necessary initialization work
  Firmware Volume Block Driver.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       This funtion always return EFI_SUCCESS.
                                It will ASSERT on errors.

**/
EFI_STATUS
EFIAPI
FvbSmmInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HANDLE            FvbHandle;
  
  //
  // Locate required protocol for access to flash.
  //
  Status = gSmst->SmmLocateProtocol (
                    &gSmmSpiDeviceProtocolGuid,
                    NULL,
                    (VOID **) &mSpiDeviceProtocol
                    );
  ASSERT_EFI_ERROR(Status);

  //
  // Initialize the rest of the module.
  //
  FvbInitialize ();

  //
  // Register SMM variable SMI handler
  //
  Status = gSmst->SmiHandlerRegister (FvbSmmHandler, &gEfiSmmFirmwareVolumeBlockProtocolGuid, &FvbHandle);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

