/** @file
  SampleCode for Intel PCH DXE Platform Policy initialization.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_PLATFORM_POLICY_DXE_H_
#define _PCH_PLATFORM_POLICY_DXE_H_

#include <Protocol/PchPlatformPolicy.h>
#include "PchAccess.h"
#include <Library/PchPlatformLib.h>

//
// Functions
//
/**
  Initilize Intel PCH DXE Platform Policy

  @param[in] ImageHandle           Image handle of this driver.
  @param[in] PeiServices           PeiServices.

  @retval  EFI_SUCCESS             Initialization complete.
  @retval  EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval  EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval  EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
PchPlatformPolicyInit (
#ifndef FSP_FLAG
  IN EFI_HANDLE       ImageHandle
#else
  IN CONST EFI_PEI_SERVICES **PeiServices
#endif
  );

#endif
