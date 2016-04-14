/** @file
  The header file of Platform PEIM.

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __PEI_PLATFORM_H__
#define __PEI_PLATFORM_H__

#define PEI_STALL_RESOLUTION            1
#define STALL_PEIM_SIGNATURE   SIGNATURE_32('p','p','u','s')

typedef struct {
  UINT32                      Signature;
  EFI_FFS_FILE_HEADER         *FfsHeader;
  EFI_PEI_NOTIFY_DESCRIPTOR   StallNotify;
} STALL_CALLBACK_STATE_INFORMATION;

#define STALL_PEIM_FROM_THIS(a) CR (a, STALL_CALLBACK_STATE_INFORMATION, StallNotify, STALL_PEIM_SIGNATURE)

/**
  Peform the boot mode determination logic
  If the box is closed, then
    1. If it's first time to boot, it's boot with full config .
    2. If the ChassisIntrution is selected, force to be a boot with full config
    3. Otherwise it's boot with no change.

  @param[in]  PeiServices      General purpose services available to every PEIM.

  @retval   EFI_SUCCESS        If the boot mode could be set

**/
EFI_STATUS
UpdateBootMode (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

/**
  This function reset the entire platform, including all processor and devices, and
  reboots the system.

  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS              If it completed successfully.

**/
EFI_STATUS
EFIAPI
ResetSystem (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

/**
  This function will be called when MRC is done.

  @param[in]  PeiServices          General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor     Information about the notify event..
  @param[in]  Ppi                  The notify context.

  @retval EFI_SUCCESS              If the function completed successfully.

**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES                     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR            *NotifyDescriptor,
  IN VOID                                 *Ppi
  );

/**
  PEI termination callback.

  @param[in]  PeiServices          General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor     Not uesed.
  @param[in]  Ppi                  Not uesed.

  @retval  EFI_SUCCESS             If the interface could be successfully
                                   installed.

**/
EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  );

/**
  This is the callback function notified by FvFileLoader PPI, it depends on FvFileLoader PPI to load
  the PEIM into memory.

  @param[in]   PeiServices         General purpose services available to every PEIM.
  @param[in]   NotifyDescriptor    The context of notification.
  @param[in]   Ppi                 The notify PPI.

  @retval  EFI_SUCCESS             If it completed successfully.

**/
EFI_STATUS
EFIAPI
FvFileLoaderPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  This function provides a blocking stall for reset at least the given number of microseconds
  stipulated in the final argument.

  @param[in]  PeiServices          General purpose services available to every PEIM.
  @param[in]  This                 Pointer to the local data for the interface.
  @param[in]  Microseconds         Number of microseconds for which to stall.

  @retval EFI_SUCCESS              The function provided at least the required stall.

**/
EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  );

/**
  This function initialize recovery functionality by installing the recovery PPI.

  @param[in]   PeiServices         General purpose services available to every PEIM.

  @retval  EFI_SUCCESS             If the interface could be successfully installed.

**/
EFI_STATUS
EFIAPI
InitializeRecovery (
  IN EFI_PEI_SERVICES     **PeiServices
  );

/**
  This function provides the implementation to properly setup both LM & PDM functionality.

  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS              Procedure returned successfully.

**/
BOOLEAN
EFIAPI
IsFastBootEnabled (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

EFI_STATUS
PrioritizeBootMode (
  IN OUT EFI_BOOT_MODE    *CurrentBootMode,
  IN EFI_BOOT_MODE        NewBootMode
  );

EFI_STATUS
EFIAPI
CapsulePpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );
#endif
