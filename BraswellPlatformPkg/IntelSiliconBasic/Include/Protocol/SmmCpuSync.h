/** @file
  Smm CPU Sync protocol definition.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SMM_CPU_SYNC_PROTOCOL_H_
#define _SMM_CPU_SYNC_PROTOCOL_H_

#define SMM_CPU_SYNC_PROTOCOL_GUID \
  { \
    0xd5950985, 0x8be3, 0x4b1c, { 0xb6, 0x3f, 0x95, 0xd1, 0x5a, 0xb3, 0xb6, 0x5f } \
  }

typedef struct _SMM_CPU_SYNC_PROTOCOL SMM_CPU_SYNC_PROTOCOL;

//
// Data structures
//

typedef enum {
  SmmCpuSyncModeTradition,
  SmmCpuSyncModeRelaxedAp,
  SmmCpuSyncModeMax
} SMM_CPU_SYNC_MODE;

//
// Protocol functions
//

/**
  Given timeout constraint, wait for all APs to arrive, and insure if this function returns EFI_SUCCESS, then
  no AP will execute normal mode code before entering SMM, so it is safe to access shared hardware resource 
  between SMM and normal mode. Note if there are SMI disabled APs, this function will return EFI_NOT_READY.

  @param[in]  This                  A pointer to the SMM_CPU_SYNC_PROTOCOL instance.

  @retval  EFI_SUCCESS              No AP will execute normal mode code before entering SMM, so it is safe to access 
                                    shared hardware resource between SMM and normal mode
  @retval  EFI_NOT_READY            One or more CPUs may still execute normal mode code
  @retval  EFI_DEVICE_ERROR         Error occured in obtaining CPU status.

**/
typedef
EFI_STATUS
(EFIAPI *SMM_CPU_SYNC_CHECK_AP_ARRIVAL) (
  IN    SMM_CPU_SYNC_PROTOCOL            *This
  );

/**
  Set SMM synchronization mode starting from the next SMI run.

  @param[in]  This                  A pointer to the SMM_CPU_SYNC_PROTOCOL instance.
  @param[in]  SyncMode              The SMM synchronization mode to be set and effective from the next SMI run.

  @retval  EFI_SUCCESS              The function completes successfully.
  @retval  EFI_INVALID_PARAMETER    SynMode is not valid.

**/
typedef
EFI_STATUS
(EFIAPI *SMM_CPU_SYNC_SET_MODE) (
  IN    SMM_CPU_SYNC_PROTOCOL            *This,
  IN    SMM_CPU_SYNC_MODE                SyncMode
  );

/**
  Get current effective SMM synchronization mode.

  @param[in]  This                  A pointer to the SMM_CPU_SYNC_PROTOCOL instance.
  @param[in]  SyncMode              Output parameter. The current effective SMM synchronization mode.

  @retval  EFI_SUCCESS              The SMM synchronization mode has been retrieved successfully.
  @retval  EFI_INVALID_PARAMETER    SyncMode is NULL.

**/
typedef
EFI_STATUS
(EFIAPI *SMM_CPU_SYNC_GET_MODE) (
  IN    SMM_CPU_SYNC_PROTOCOL            *This,
  OUT   SMM_CPU_SYNC_MODE                *SyncMode
  );

/**
  This protocol provides services on SMM CPU syncrhonization related status and controls
**/
struct _SMM_CPU_SYNC_PROTOCOL {
  SMM_CPU_SYNC_CHECK_AP_ARRIVAL      CheckApArrival;
  SMM_CPU_SYNC_SET_MODE              SetMode;
  SMM_CPU_SYNC_GET_MODE              GetMode;
};

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gSmmCpuSyncProtocolGuid;

#endif
