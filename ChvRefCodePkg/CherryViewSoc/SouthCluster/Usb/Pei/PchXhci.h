/** @file
  Header file for the PCH XHCI PPI

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PEI_PCH_XHCI_H
#define _PEI_PCH_XHCI_H

///
/// External include files do NOT need to be explicitly specified in real EDKII
/// environment
///

///
/// Driver Produced PPI Prototypes
///
#include <Ppi/UsbController.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include "PchAccess.h"
#include "PchUsb.h"

///
/// The following define is not in framework code yet
/// Add XHCI define as 0x04
///
#define PEI_XHCI_CONTROLLER 0x04

#define PCH_PCIE_XHCI_BUS_DEV_FUNC MmPciAddress ( \
                                     0, \
                                     DEFAULT_PCI_BUS_NUMBER_PCH, \
                                     PCI_DEVICE_NUMBER_PCH_XHCI, \
                                     PCI_FUNCTION_NUMBER_PCH_XHCI, \
                                     0 \
                                     )

#define PEI_PCH_XHCI_SIGNATURE  SIGNATURE_32 ('X', 'H', 'C', 'I')

typedef struct {
  UINTN                   Signature;
  PEI_USB_CONTROLLER_PPI  XhciControllerPpi;
  EFI_PEI_PPI_DESCRIPTOR  PpiList;
  UINTN                   MemBase;
} PCH_XHCI_DEVICE;

#define PCH_XHCI_DEVICE_FROM_THIS(a)  CR (a, PCH_XHCI_DEVICE, XhciControllerPpi, PEI_PCH_XHCI_SIGNATURE)

#endif
