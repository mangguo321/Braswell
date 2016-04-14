/** @file
  PCH Usb policy PPI produced by a platform driver specifying
  various expected PCH Usb settings. This PPI is consumed by the
  PCH PEI drivers.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_USB_POLICY_H_
#define _PCH_USB_POLICY_H_

///
/// PCH Usb policy provided by platform for PEI phase
///

#include <PiPei.h>

#include "PchRegs.h"
#include <Protocol/PchPlatformPolicy.h>

extern EFI_GUID                     gPchUsbPolicyPpiGuid;

typedef struct _PCH_USB_POLICY_PPI  PCH_USB_POLICY_PPI;

///
/// PPI revision number
/// Any backwards compatible changes to this PPI will result in an update in the revision number
/// Major changes will require publication of a new PPI
///
/// Revision 1: Original version
///
#define PCH_USB_POLICY_PPI_REVISION_1 1

//
// Generic definitions for device enabling/disabling used by PCH code.
//
#define PCH_DEVICE_ENABLE   1
#define PCH_DEVICE_DISABLE  0

#define XHCI_MODE           1

///
/// PCH Usb policy PPI produced by a platform driver specifying various expected
/// PCH Usb settings that would be used by PCH_INIT_PPI.UsbInit () and USB PEI module.
/// This PPI needs to be installed before calling PCH_INIT_PPI.UsbInit ().
///
struct _PCH_USB_POLICY_PPI {
  ///
  /// This member specifies the revision of the PEI PCH USB Policy PPI.
  /// This field is used to indicate backwards compatible changes to the protocol.
  /// Platform code that produces this PPI must fill with the correct revision value
  /// for the PCH reference code to correctly interpret the content of the PPI fields.
  ///
  UINT8           Revision;
  ///
  /// This member describes USB controller's related configuration.
  ///
  PCH_USB_CONFIG  *UsbConfig;
  ///
  /// This member decides which USB controller needs to be initialed and allocated
  /// resource in Pei Phase. It will be referred by USB PEI module.
  ///
  UINT8           Mode;
  ///
  /// This member describes XHCI memory base address. USB PEI module will refer to
  /// this field to program memory base address of the XHCI controller.
  ///
  UINTN           XhciMemBaseAddr;
};

#endif
