/** @file
  Header file for PCH RESET Runtime Driver

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_RESET_H
#define _PCH_RESET_H

#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Protocol/Reset.h>
#include <Protocol/PchExtendedReset.h>
#include <Protocol/PchReset.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/BaseLib.h>
#include "PchAccess.h"
//
// Driver private data
//
#define PCH_RESET_SIGNATURE SIGNATURE_32 ('I', 'E', 'R', 'S')

typedef struct {
  UINT32                          Signature;
  EFI_HANDLE                      Handle;
  EFI_PCH_EXTENDED_RESET_PROTOCOL PchExtendedResetProtocol;
#if ((TIANO_RELEASE_VERSION != 0) && (EFI_SPECIFICATION_VERSION < 0x00020000))
  CHAR16                          *CapsuleVariableName;
#endif
  UINTN                           PmcBase;
  UINTN                           AcpiBar;
} PCH_RESET_INSTANCE;

#define PCH_RESET_INSTANCE_FROM_THIS(a) \
  CR ( \
  a, \
  PCH_RESET_INSTANCE, \
  PchExtendedResetProtocol, \
  PCH_RESET_SIGNATURE \
  )

/**
  Initialize the state information for the Reset Architectural Protocol

  @param[in] ImageHandle          Image handle of the loaded driver
  @param[in] SystemTable          Pointer to the System Table

  @retval EFI_SUCCESS             Thread can be successfully created
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR        Cannot create the timer service
**/
EFI_STATUS
EFIAPI
InitializePchReset (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Reset the system.

  @param[in] ResetType            Warm or cold
  @param[in] ResetStatus          Possible cause of reset
  @param[in] DataSize             Size of ResetData in bytes
  @param[in] ResetData            Optional Unicode string

  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
VOID
EFIAPI
IntelPchResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN VOID           *ResetData OPTIONAL
  );

#if ((TIANO_RELEASE_VERSION != 0) && (EFI_SPECIFICATION_VERSION < 0x00020000))
/**
  If need be, do any special reset required for capsules. For this
  implementation where we're called from the ResetSystem() api,
  just set our capsule variable and return to let the caller
  do a soft reset.

  @param[in] CapsuleDataPtr       Pointer to the capsule block descriptors

  
**/
VOID
CapsuleReset (
  IN UINTN   CapsuleDataPtr
  );
#endif

/**
  Execute Pch Extended Reset from the host controller.

  @param[in] This                    Pointer to the EFI_PCH_EXTENDED_RESET_PROTOCOL instance.
  @param[in] PchExtendedResetTypes   Pch Extended Reset Types which includes PowerCycle, Globalreset.

  @retval EFI_SUCCESS                Successfully completed.
  @retval EFI_INVALID_PARAMETER      If ResetType is invalid.
**/
EFI_STATUS
EFIAPI
PchExtendedReset (
  IN     EFI_PCH_EXTENDED_RESET_PROTOCOL   *This,
  IN     PCH_EXTENDED_RESET_TYPES          PchExtendedResetTypes
  );

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                   The event registered.
  @param[in] Context                 Event context. Not used in this event handler.

**/
VOID
EFIAPI
PchResetVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

#endif
