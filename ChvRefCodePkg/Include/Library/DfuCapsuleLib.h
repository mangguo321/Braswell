/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _DFU_CAPSULE_LIB_H
#define _DFU_CAPSULE_LIB_H
#include <Uefi.h>
#include <Uefi/UefiSpec.h>

#define UPDATE_SUCCESS   0
#define UPDATE_FAIL_GENERIC 1
#define UPDATE_NO_RESOURCE 2
#define UPDATE_VERSION_INCORRECT 3
#define UPDATE_INVALID_IMAGE 4
#define UPDATE_AUTH_ERROR 5
#define UPDATE_AC_NOT_CONNECTED 6
#define UPDATE_LOW_BATTERY 7
   
typedef enum {
  NO_FAILURE      =  0,
  NOT_UPDATED     =  1,
  PARTIAL_UPDATED =  2
}DFU_FAILURE_REASON;

extern EFI_GUID gEfiDFUResultGuid;
extern EFI_GUID gEfiDFUVerGuid;

typedef struct {
  EFI_GUID DFU_DEVICE_GUID;
  UINT32 UpdateStatus;
  DFU_FAILURE_REASON Failure;
} DFU_UPDATE_STATUS;

/**
  @todo	add description

**/
extern EFI_STATUS
EFIAPI
LoadDFUImage (
  void **pFileBuffer,
  UINT64 *pFileSize
);

/**
  @todo	add description

**/
extern EFI_STATUS
EFIAPI  
ReportUpdateStatus(
  UINT32 UpdateStatus,
  DFU_FAILURE_REASON failure
);

#endif
