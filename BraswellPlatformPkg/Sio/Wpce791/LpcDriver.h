/** @file
  EFI Lpc Driver for a Generic PC Platform.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _LPC_DRIVER_H
#define _LPC_DRIVER_H

#include "LpcSio.h"
#include "LpcIsaAcpi.h"
#include "Protocol/IsaAcpi.h"
#include "Protocol/PciIo.h"
#include "Protocol/DriverBinding.h"
#include "Library/UefiBootServicesTableLib.h"
#include "IsaAcpiDxe/PcatIsaAcpi.h"
#include "IndustryStandard/Pci22.h"
#include "Protocol/LpcWpce791Policy.h"
#include <PchRegs.h>
#include <PlatformBaseAddresses.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>

#define ICH_LPC_BRIDGE_BUS_DEV_FUNC 0x1F0000

//
// LPC device private data structure
//
#define LPC_DEV_SIGNATURE SIGNATURE_32('X', '7', '8', 'W') //'W87X'
#define EFI_WPCE791_PS2_KEYBOARD_ENABLE       0x01
#define EFI_WPCE791_PS2_KEYBOARD_DISABLE      0x00

#define EFI_WPCE791_PS2_MOUSE_ENABLE       0x01
#define EFI_WPCE791_PS2_MOUSE_DISABLE      0x00

#define PCI_IDX             0xCF8
#define PCI_DAT             0xCFC

#define PCI_LPC_BASE    (0x8000F800)
#define PCI_LPC_REG(x)  (PCI_LPC_BASE + (x))

#define V_PCH_ILB_IRQE_UARTIRQEN_IRQ4             BIT4 // UART IRQ4 Enable

typedef struct {
  UINTN                 Signature;
  EFI_HANDLE            Handle;
  EFI_ISA_ACPI_PROTOCOL IsaAcpi;
  EFI_PCI_IO_PROTOCOL   *PciIo;

} LPC_DEV;

#define LPC_ISA_ACPI_FROM_THIS(a) BASE_CR (a, LPC_DEV, IsaAcpi)

//
// Driver entry point
//
EFI_STATUS
EFIAPI
LpcDriverEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

//
// Prototypes for Driver model protocol interface
//
EFI_STATUS
EFIAPI
LpcDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
LpcDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
LpcDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  );

VOID
LpcIoRead8 (
  IN  UINT16  Port,
  OUT UINT8   *Data
  );

VOID
LpcIoWrite8 (
  IN  UINT16  Port,
  IN  UINT8   Data
  );

VOID
EnableInternalUartDevice (
  VOID
  );  

#endif
