/** @file
  Pch Xhci PPI Init

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchXhci.h"

///
/// PPI interface function
///
STATIC
EFI_STATUS
EFIAPI
GetXhciController (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  PEI_USB_CONTROLLER_PPI          *This,
  IN  UINT8                           UsbControllerId,
  OUT UINTN                           *ControllerType,
  OUT UINTN                           *BaseAddress
  );

///
/// Globals
///
STATIC PEI_USB_CONTROLLER_PPI   mXhciControllerPpi = { GetXhciController };

STATIC EFI_PEI_PPI_DESCRIPTOR   mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiUsbControllerPpiGuid,
  NULL
};

///
/// Helper function
///
STATIC
EFI_STATUS
EnableXhciController (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN PCH_XHCI_DEVICE          *PeiPchXhciDev
  );


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
  )
{
  EFI_STATUS            Status;
  PCH_XHCI_DEVICE       *PeiPchXhciDev;
  EFI_BOOT_MODE         BootMode;

  DEBUG ((EFI_D_INFO, "InitForXHCI() Start\n"));

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);

  ///
  /// We do not export this in S3 boot path, because it is only for recovery.
  ///
  if (BootMode == BOOT_ON_S3_RESUME) {
    return EFI_SUCCESS;
  }

  PeiPchXhciDev = (PCH_XHCI_DEVICE *) AllocatePool (sizeof (PCH_XHCI_DEVICE));
  if (PeiPchXhciDev == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate memory for PeiPchXhciDev! \n"));
    return EFI_OUT_OF_RESOURCES;
  }

  PeiPchXhciDev->Signature            = PEI_PCH_XHCI_SIGNATURE;
  PeiPchXhciDev->XhciControllerPpi    = mXhciControllerPpi;
  PeiPchXhciDev->PpiList              = mPpiList;
  PeiPchXhciDev->PpiList.Ppi          = &PeiPchXhciDev->XhciControllerPpi;

  ///
  /// Assign resources and enable XHCI controllers
  ///
  PeiPchXhciDev->MemBase = XhciMemBaseAddr;
  Status                 = EnableXhciController (PeiServices, PeiPchXhciDev);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Install USB Controller PPI
  ///
  Status = PeiServicesInstallPpi (&PeiPchXhciDev->PpiList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "InitForXHCI() End\n"));

  return Status;

}
///
/// PPI interface implementation
///

/**

  Get XHCI controller information

  @param[in] PeiServices          General PEI services
  @param[in] This                 Pointer to the PEI_XHCI_CONTROLLER_PPI
  @param[in] UsbControllerId      The USB controller number
  @param[in] ControllerType       Output: USB controller type
  @param[in] BaseAddress          Output: XHCI controller memory base address

  @retval EFI_INVALID_PARAMETER   UsbControllerId is out of range
  @retval EFI_SUCCESS             Function completes successfully

**/
STATIC
EFI_STATUS
EFIAPI
GetXhciController (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  PEI_USB_CONTROLLER_PPI          *This,
  IN  UINT8                           UsbControllerId,
  OUT UINTN                           *ControllerType,
  OUT UINTN                           *BaseAddress
  )
{
  PCH_XHCI_DEVICE *PeiPchXhciDev;

  PeiPchXhciDev = PCH_XHCI_DEVICE_FROM_THIS (This);

  if (UsbControllerId >= PchXhciControllerMax) {
    return EFI_INVALID_PARAMETER;
  }

  *ControllerType = PEI_XHCI_CONTROLLER;

  *BaseAddress    = PeiPchXhciDev->MemBase;

  return EFI_SUCCESS;
}


/**

  Enable the XHCI controller

  @param[in] PeiServices          The general PEI services
  @param[in] PeiPchXhciDev        The XHCI device

  @retval EFI_INVALID_PARAMETER   UsbControllerId is out of range
  @retval EFI_SUCCESS             The function completes successfully

**/
STATIC
EFI_STATUS
EnableXhciController (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN PCH_XHCI_DEVICE          *PeiPchXhciDev
  )
{
  UINTN BaseAddress;

  BaseAddress = PeiPchXhciDev->MemBase;

  ///
  /// Assign base address register
  ///
  MmioWrite32 ((PCH_PCIE_XHCI_BUS_DEV_FUNC + R_PCH_XHCI_MEM_BASE), BaseAddress);

  ///
  /// Enable XHCI register
  ///
  MmioOr16 (
    (PCH_PCIE_XHCI_BUS_DEV_FUNC + R_PCH_XHCI_COMMAND_REGISTER),
    (UINT16) (B_PCH_XHCI_COMMAND_BME | B_PCH_XHCI_COMMAND_MSE)
    );

  return EFI_SUCCESS;
}
