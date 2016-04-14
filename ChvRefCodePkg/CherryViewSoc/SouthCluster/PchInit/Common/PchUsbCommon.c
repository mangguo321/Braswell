/** @file
  Initializes PCH USB Controller.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchUsbCommon.h"

/**
  USB initialization before boot to OS

  @param[in] UsbConfig            The PCH Platform Policy for USB configuration

**/
VOID
UsbInitAtBoot (
  IN     PCH_USB_CONFIG              *UsbConfig
)
{
  UINTN                         XhciPciMmBase;
  UINT32                        XhciMmioBase;

  DEBUG ((EFI_D_INFO, "UsbInitAtBoot() - Start\n"));

  if ((UsbConfig->SsicPortSettings[0].Enable == PCH_DEVICE_DISABLE) &&
      (UsbConfig->SsicPortSettings[1].Enable == PCH_DEVICE_DISABLE)) {
    DEBUG ((EFI_D_INFO, "UsbInitAtBoot() - SsicPortSettings[0] & SsicPortSettings[1] - Disabled\n"));
    XhciPciMmBase = MmPciAddress (
                      0,
                      0,
                      PCI_DEVICE_NUMBER_PCH_XHCI,
                      PCI_FUNCTION_NUMBER_PCH_XHCI,
                      0
                      );

    XhciMmioBase = MmioRead32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE) & B_PCH_XHCI_MEM_BASE_BA;

    //
    // SSIC port 0: PORT_UNUSED=1 PROG_DONE=1
    // SSIC port 1: PORT_UNUSED=0 PROG_DONE=1
    //

    //
    // Set Port 0 un-used
    //
    MmioOr32 (
      (XhciMmioBase + 0x880C),
      (UINT32) (BIT31)
      );
    //
    // Clear Port 1 un-used
    //
    MmioAnd32 (
      (XhciMmioBase + 0x883C),
      (UINT32) ~(BIT31)
      );

    //
    // Set PROG_DONE bit,
    // xHCIBAR + 0x880C [30] = 1b and xHCIBAR + 0x883C [30] = 1b.
    //
    MmioOr32 (
      (XhciMmioBase + 0x880C),
      (UINT32) (BIT30)
      );
    MmioOr32 (
      (XhciMmioBase + 0x883C),
      (UINT32) (BIT30)
      );
  }
  DEBUG ((EFI_D_INFO, "UsbInitAtBoot() - End\n"));
}
