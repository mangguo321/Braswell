/** @file
  Null Platform Hook Library instance.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>
#include <Library/PlatformHookLib.h>

#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <PchRegs.h>
#include <PlatformBaseAddresses.h>

#define PCI_IDX             0xCF8
#define PCI_DAT             0xCFC

#define PCI_LPC_BASE    (0x8000F800)
#define PCI_LPC_REG(x)  (PCI_LPC_BASE + (x))

#define V_PCH_ILB_IRQE_UARTIRQEN_IRQ4             BIT4 // UART IRQ4 Enable
#ifndef PCIEX_BASE_ADDRESS
#define PCIEX_BASE_ADDRESS                        ((UINTN)PcdGet64(PcdPciExpressBaseAddress))
#endif
#define PciD31F0RegBase                           PCIEX_BASE_ADDRESS + (UINT32) (31 << 15)

VOID
EnableInternalUart (
  VOID
  )
{
  //
  // Program and enable PMC Base.
  //
  IoWrite32 (PCI_IDX, PCI_LPC_REG(R_PCH_LPC_PMC_BASE));
  IoWrite32 (PCI_DAT, (PMC_BASE_ADDRESS | B_PCH_LPC_PMC_BASE_EN));

  //
  // Program and enable iLB Base.
  //
  IoWrite32 (PCI_IDX, PCI_LPC_REG(R_PCH_LPC_ILB_BASE));
  IoWrite32 (PCI_DAT, (ILB_BASE_ADDRESS | B_PCH_LPC_ILB_BASE_EN));
  //
  // Enable internal UART interrupt.
  //
  MmioOr32 (ILB_BASE_ADDRESS + R_PCH_ILB_IRQE, (UINT32) (B_PCH_ILB_IRQE_UARTIRQEN_IRQ4));
  //
  // GPIO: N67(HV_DDI2_DDC_SCL) - Setting Mode 3
  // UART0_TXD
  //
  MmioWrite32 ((UINTN) (IO_BASE_ADDRESS + 0xD438), (UINT32) 0x00930300);
  //
  // GPIO: N62(HV_DDI2_DDC_SDA) - Setting Mode 3
  // UART0_RXD
  //
  MmioWrite32 ((UINTN) (IO_BASE_ADDRESS + 0xD410), (UINT32) 0x00930300);
  MmioOr32 (
    (UINTN) (PciD31F0RegBase + R_PCH_LPC_UART_CTRL),
    (UINT32) (B_PCH_LPC_UART_CTRL_COM1_EN)
    );
}

/**
  Performs platform specific initialization required for the CPU to access
  the hardware associated with a SerialPortLib instance.  This function does
  not intiailzie the serial port hardware itself.  Instead, it initializes 
  hardware devices that are required for the CPU to access the serial port 
  hardware.  This function may be called more than once.

  @retval RETURN_SUCCESS       The platform specific initialization succeeded.
  @retval RETURN_DEVICE_ERROR  The platform specific initialization could not be completed.
 
**/
RETURN_STATUS
EFIAPI
PlatformHookSerialPortInitialize (
  VOID
  )
{
  if (PcdGet64 (PcdSerialRegisterBase) == 0) {
    return RETURN_SUCCESS;
  }
  EnableInternalUart ();
  return  RETURN_SUCCESS;
}

