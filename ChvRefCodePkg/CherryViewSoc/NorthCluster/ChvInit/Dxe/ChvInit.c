/** @file
  This is the driver that initializes the Intel CherryView.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "ChvInit.h"
#include <Protocol/ChvPlatformPolicyProtocol.h>

//
// Extern the GUID for protocol users.
//
#ifdef FSP_FLAG
extern EFI_GUID gDxeChvPlatformPolicyGuid;
#else
extern EFI_GUID gEndOfSaInitDxeProtocolGuid;
#endif
/**
  This is the standard EFI driver point that detects
  whether there is an ICH southbridge in the system
  and if so, initializes the chip.

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
ChvInitEntryPoint (
#ifndef FSP_FLAG
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
#else
  IN EFI_PEI_FILE_HANDLE       FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
#endif
  )
{
  EFI_STATUS                        Status;

#ifndef FSP_FLAG
  EFI_HANDLE                        Handle;
#endif

  //
  // IgdOpRegion Install Initialization
  //
  DEBUG ((EFI_D_ERROR, "Initializing IGD OpRegion\n"));
  IgdOpRegionInit ();
  
  //
  // Install gEndOfSaDxeInitProtocolGuid
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEndOfSaInitDxeProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

