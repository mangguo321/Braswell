/** @file
  Header file for SMM Control Driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SMM_CONTROL_DRIVER_H_
#define _SMM_CONTROL_DRIVER_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/SmmControl2.h>
#include <IndustryStandard/Pci30.h>
#include <PchAccess.h>
#include <Library/PchPlatformLib.h>
#define EFI_INTERNAL_POINTER  0x00000004

#define SMM_CONTROL_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('i', '4', 's', 'c')

typedef struct {
  UINTN                           Signature;
  EFI_HANDLE                      Handle;
  EFI_SMM_CONTROL2_PROTOCOL       SmmControl;
} SMM_CONTROL_PRIVATE_DATA;

#define SMM_CONTROL_PRIVATE_DATA_FROM_THIS(a) CR (a, SMM_CONTROL_PRIVATE_DATA, SmmControl, SMM_CONTROL_DEV_SIGNATURE)

//
// Prototypes
//

/**
  This is the constructor for the SMM Control protocol.

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_STATUS              Results of the installation of the SMM Control Protocol

**/
EFI_STATUS
EFIAPI
SmmControlDriverEntryInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  Trigger the software SMI.

  @param[in] Data                 The value to be set on the software SMI data port

  @retval EFI_SUCCESS             Function completes successfully

**/
EFI_STATUS
EFIAPI
SmmTrigger (
  UINT8   Data
  );

/**
  Clear the SMI status.

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_DEVICE_ERROR        Something error occurred

**/
EFI_STATUS
EFIAPI
SmmClear (
  VOID
  );

/**
  This routine generates an SMI.

  @param[in] This                       The EFI SMM Control protocol instance
  @param[in, out] CommandPort           The buffer contains data to the command port
  @param[in, out] DataPort              The buffer contains data to the data port
  @param[in] Periodic                   Periodic or not
  @param[in] ActivationInterval         Interval of periodic SMI

  @retval EFI Status                    Describing the result of the operation
  @retval EFI_INVALID_PARAMETER         Some parameter value passed is not supported

**/
EFI_STATUS
EFIAPI
Activate (
  IN CONST EFI_SMM_CONTROL2_PROTOCOL    *This,
  IN OUT   UINT8                        *CommandPort       OPTIONAL,
  IN OUT   UINT8                        *DataPort          OPTIONAL,
  IN       BOOLEAN                      Periodic           OPTIONAL,
  IN       UINTN                        ActivationInterval OPTIONAL
  );

/**
  This routine clears an SMI.

  @param[in] This                 The EFI SMM Control protocol instance
  @param[in] Periodic             Periodic or not

  @retval EFI Status              Describing the result of the operation
  @retval EFI_INVALID_PARAMETER   Some parameter value passed is not supported

**/
EFI_STATUS
EFIAPI
Deactivate (
  IN CONST EFI_SMM_CONTROL2_PROTOCOL    *This,
  IN       BOOLEAN                      Periodic OPTIONAL
  );

/**
  Disable all pending SMIs

**/
VOID
EFIAPI
DisablePendingSmis (
  VOID
  );

#endif
