/** @file
  Ihis protocol is defined to pass the Boot Material and OS Loader RIP to VMM.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __BOOT_MATERIAL_H__
#define __BOOT_MATERIAL_H__

#include "MkhiMsgs.h"

#define EFI_BOOT_MATERIAL_PROTOCOL_GUID \
  {0x1d011a41, 0xfb19, 0x4a03, 0xa5, 0xca, 0xed, 0x33, 0x87, 0x4, 0xa1, 0xd2}

typedef struct _EFI_BOOT_MATERIAL_PROTOCOL EFI_BOOT_MATERIAL_PROTOCOL;

#define BOOT_MATERIAL SL_HI_GET_BOOT_MATERIAL_ACK_DATA

/**
  The EFI_BOOT_MATERIAL_PROTOCOL GetBootMaterial function call provides the Boot Material recieved from SEC FW

  @param[in]  This               Indicates the calling context
  @param[out] BootMaterial       Pointer to Boot material recieved from SEC FW

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
                                 The ProtocolCapability variable will not be populated. 
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.
                                 The ProtocolCapability variable will not be populated.
  @retval EFI_BUFFER_TOO_SMALL   The ProtocolCapability variable is too small to hold the full response.
                                 It will be partially populated (required Size field will be set).
								 
**/
typedef
EFI_STATUS
(EFIAPI *EFI_BOOT_MATERIAL_GET_BOOT_MATERIAL) (
  IN EFI_BOOT_MATERIAL_PROTOCOL     *This,
  IN OUT BOOT_MATERIAL              *BootMaterial,
  IN OUT UINT32                     *GetBootInfoResult
  );

/**
  Returns OS loader image handle.

  @param[in]   Indicates the calling context.
  @param[out]  OS loader image handle

  @retval      EFI_SUCCESS            The OS loader was successfully loaded and the image handle is valid.
  @retval      EFI_LOAD_ERROR         The OS loader failed to load and the image handle is not valid (set to NULL).
  @retval      EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BOOT_MATERIAL_GET_OSLOADER_IMAGE_HANDLE) (
  IN EFI_BOOT_MATERIAL_PROTOCOL     *This,
  OUT EFI_HANDLE                    *OSLoaderImageHandle
  );

/**
  The EFI_BOOT_MATERIAL_PROTOCOL Get FingerprintSensor IMR Address. 

  @param[in]  This               Indicates the calling context
  @param[out] FPSBase            FingerprintSensor IMR base address
  @param[out] FPSSize            FingerprintSensor IMR size

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect
                                 (e.g. asking for an event log whose format is not supported).

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BOOT_MATERIAL_GET_FPS_ADDR) (
  IN EFI_BOOT_MATERIAL_PROTOCOL     *This,
  OUT UINT32                        *FPSBase,
  OUT UINT32                        *FPSSize
  );

/**
  The EFI_BOOT_MATERIAL_PROTOCOL Get SilentLake IMR Address 

  @param[in]  This               Indicates the calling context
  @param[out] SLBase             SilentLake IMR base address
  @param[out] SLSize             SilentLake IMR size

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect
                                 (e.g. asking for an event log whose format is not supported).

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BOOT_MATERIAL_GET_SL_ADDR) (
  IN EFI_BOOT_MATERIAL_PROTOCOL     *This,
  OUT UINT32                        *SLBase,
  OUT UINT32                        *SLSize
  );

struct _EFI_BOOT_MATERIAL_PROTOCOL {
  EFI_BOOT_MATERIAL_GET_BOOT_MATERIAL          GetBootMaterial;
  EFI_BOOT_MATERIAL_GET_OSLOADER_IMAGE_HANDLE  GetOsLoaderHandle;
  EFI_BOOT_MATERIAL_GET_SL_ADDR                GetSilentLakeAddr;
  EFI_BOOT_MATERIAL_GET_FPS_ADDR               GetFingerprintSensorAddr;
};

extern EFI_GUID gEfiBootMaterialProtocolGuid;

typedef struct _EFI_SILENT_LAKE_PROTOCOL EFI_SILENT_LAKE_PROTOCOL;

/**
  The EFI_SILENT_LAKE_PROTOCOL Get VMM related Info.

  @param[in]  This               Indicates the calling context

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SILENT_LAKE_GET_VMM_INFO) (
  IN EFI_SILENT_LAKE_PROTOCOL        *This
  );

/**
  The EFI_SILENT_LAKE_PROTOCOL Verify VMM.

  @param[in]  This               Indicates the calling context

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SILENT_LAKE_VERIFY_VMM) (
  IN EFI_SILENT_LAKE_PROTOCOL        *This,
  IN UINT8                           *ImageBase,
  IN UINTN                           ImageSize
  );

struct _EFI_SILENT_LAKE_PROTOCOL {
  EFI_SILENT_LAKE_GET_VMM_INFO       GetVmmInfo;
  EFI_SILENT_LAKE_VERIFY_VMM         VerifyVmm;
  EFI_EVENT                          StartVmmEvent;
};

extern EFI_GUID gEfiSilentLakeProtocolGuid;
extern EFI_GUID gVmmDebugEnableDisableGuid;
#define VMM_DEBUG_VARIABLE_NAME L"VmmDebugVar"

#endif
