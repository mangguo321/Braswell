/** @file
  Wrapper for Platform Policy driver. Get Setup value to initialize Intel DXE Platform Policy.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PolicyInitDxe.h"

VOID
EFIAPI
PpmPolicyEntryCallback (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Protocol;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&Protocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  PpmPolicyEntry (gImageHandle, gST);
}

/**
  Initialize DXE Platform Policy.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
PolicyInitDxeEntryPoint (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS           Status;
  VOID                 *Registration;

  //
  // Platform CherryView Policy Initialization
  //
  Status = ChvPlatformPolicyInit (ImageHandle);
  DEBUG ((DEBUG_INFO, "CherryView Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // Platform PCH Policy Initialization
  //
  Status = PchPlatformPolicyInit (ImageHandle);
  DEBUG ((DEBUG_INFO, "PCH Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // GOP policy
  //
  Status = PlatformGOPPolicyEntryPoint (ImageHandle, SystemTable);

  //
  // PPM Policy
  //
  EfiCreateProtocolNotifyEvent (&gEfiMpServiceProtocolGuid, TPL_CALLBACK, PpmPolicyEntryCallback, NULL, &Registration);

  return Status;
}
