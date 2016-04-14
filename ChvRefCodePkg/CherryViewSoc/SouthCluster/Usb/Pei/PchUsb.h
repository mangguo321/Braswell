/** @file
  Header file for the USB PEIM

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_USB_H_
#define _PCH_USB_H_

#include "PchAccess.h"
#include <Ppi/PchUsbPolicy.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

/**
  Initialize PCH XHCI PEIM

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] UsbPolicyPpi         PCH Usb Policy PPI

  @retval EFI_SUCCESS             The PCH XHCI PEIM is initialized successfully
  @retval EFI_INVALID_PARAMETER   UsbControllerId is out of range
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
  @retval Others                  All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
InitForXHCI (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN UINTN                      XhciMemBaseAddr
  );

#endif
