/** @file
  USB PEI Init

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchUsb.h"

/**
  Initialize PCH USB PEIM

  @param[in] FfsHeader            Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The PCH USB PEIM is initialized successfully
  @retval Others                  All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
InitializePchUsb (
  IN EFI_PEI_FILE_HANDLE             FfsHeader,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  DEBUG ((EFI_D_INFO, "InitializePchUsb() Start\n"));
  
  InitForXHCI (PeiServices, PcdGet32(PcdPeiPchXhciControllerMemoryBaseAddress));

  DEBUG ((EFI_D_INFO, "InitializePchUsb() End\n"));

  return EFI_SUCCESS;
}
