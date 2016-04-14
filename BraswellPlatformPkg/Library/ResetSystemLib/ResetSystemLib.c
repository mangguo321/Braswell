/** @file
  System reset Library Services.  This library class provides a set of
  methods to reset whole system with manipulate ICH.

  Copyright (c) 2007 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>
#include <Library/ResetSystemLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <PchRegs.h>
#include <Rsci.h>

#define RESET_GENERATOR_PORT R_PCH_RST_CNT

VOID
EFIAPI
PlatformResetHook (
  UINT8 ResetType
  )
{
}

/**
  Calling this function causes a system-wide reset. This sets
  all circuitry within the system to its initial state. This type of reset
  is asynchronous to system operation and operates without regard to
  cycle boundaries.

  System reset should not return, if it returns, it means the system does
  not support cold reset.
**/
VOID
EFIAPI
ResetCold (
  VOID
  )
{
  PlatformResetHook(COLD_RESET);
  IoWrite8 (RESET_GENERATOR_PORT, 0x2);
  IoWrite8 (RESET_GENERATOR_PORT, 0x6);
}

/**
  Calling this function causes a system-wide initialization. The processors
  are set to their initial state, and pending cycles are not corrupted.

  System reset should not return, if it returns, it means the system does
  not support warm reset.
**/
VOID
EFIAPI
ResetWarm (
  VOID
  )
{
  PlatformResetHook(WARM_RESET);
  IoWrite8 (RESET_GENERATOR_PORT, 0x0);
  IoWrite8 (RESET_GENERATOR_PORT, 0x4);
}

/**
  Calling this function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  System shutdown should not return, if it returns, it means the system does
  not support shut down reset.
**/
VOID
EFIAPI
ResetShutdown (
  VOID
  )
{
  UINT16  IchPmioBase;
  UINT16  Data16;
  UINT32  Data32;

  //
  // Firstly, ACPI decode must be enabled
  //
  PciOr8 (
    PCI_LIB_ADDRESS(PCI_BUS_NUMBER_ICH, PCI_DEVICE_NUMBER_ICH_LPC, 0, R_ICH_LPC_ACPI_CNT),
    (UINT8) (B_ICH_LPC_ACPI_CNT_ACPI_EN)
    );

  IchPmioBase = (UINT16) (PciRead16 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_ICH, PCI_DEVICE_NUMBER_ICH_LPC, 0, R_ICH_LPC_ACPI_BASE)) & ~BIT0);

  //
  // Then, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
  //
  Data16 = 0;
  IoWrite16 (
    (UINTN)(IchPmioBase + R_ACPI_GPE0_EN),
    (UINT16)Data16
    );

  //
  // Secondly, PwrSts register must be cleared
  //
  // Write a "1" to bit[8] of power button status register at
  // (PM_BASE + PM1_STS_OFFSET) to clear this bit
  //
  Data16 = B_PWRBTN_STS;
  IoWrite16 (
    (UINTN)(IchPmioBase + R_ACPI_PM1_STS),
    (UINT16)Data16
    );

  //
  // Finally, transform system into S5 sleep state
  //
  Data32 = IoRead32 ((UINTN)(IchPmioBase + R_ACPI_PM1_CNT));

  Data32  = (UINT32) ((Data32 & ~(B_SLP_TYP + B_SLP_EN)) | V_S5);

  IoWrite32 (
    (UINTN) (IchPmioBase + R_ACPI_PM1_CNT),
    (UINT32)Data32
    );

  Data32 = Data32 | B_SLP_EN;

  IoWrite32 (
    (UINTN) (IchPmioBase + R_ACPI_PM1_CNT),
    (UINT32)Data32
    );

  return;
}

/**
  Calling this function causes the system to enter a power state for capsule
  update.

  Reset update should not return, if it returns, it means the system does
  not support capsule update.

**/
VOID
EFIAPI
EnterS3WithImmediateWake (
  VOID
  )
{
  ASSERT (FALSE);
}

