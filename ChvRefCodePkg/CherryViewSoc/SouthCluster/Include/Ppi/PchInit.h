/** @file
  This file defines the PCH Init PPI

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_INIT_H_
#define _PCH_INIT_H_

#include <Protocol/PchPlatformPolicy.h>

extern EFI_GUID               gPchInitPpiGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_INIT_PPI  PCH_INIT_PPI;

/**
  The function performing USB init in PEI phase. This could be used by USB recovery
  or debug features that need USB initialization during PEI phase.
  Note: Before executing this function, please be sure that PCH_INIT_PPI.Initialize
  has been done and PchUsbPolicyPpi has been installed.

  @param[in] PeiServices    General purpose services available to every PEIM

  @retval EFI_SUCCESS       The function completed successfully
  @retval Others            All other error conditions encountered result in an ASSERT.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_USB_INIT) (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

///
/// PCH_INIT_PPI Structure Definition
///
struct _PCH_INIT_PPI {
  ///
  /// The function performs USB init in PEI phase. This could be used by USB recovery
  /// or debug function that USB initialization needs to be done in PEI phase.
  /// Note: Before executing this function, please be sure that PCH_PLATFORM_POLICY_PPI
  /// and PCH_USB_POLICY_PPI have been installed.
  ///
  PCH_USB_INIT          UsbInit;
};

#endif
