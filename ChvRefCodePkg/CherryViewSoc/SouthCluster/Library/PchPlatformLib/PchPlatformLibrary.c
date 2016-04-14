/** @file
  PCH Platform Lib implementation.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchPlatformLibrary.h"

/**
  Delay for at least the request number of microseconds.
  This function would be called by runtime driver, please do not use any MMIO marco here.

  @param[in] Microseconds         Number of microseconds to delay.

**/
VOID
EFIAPI
PchPmTimerStall (
  IN  UINTN   Microseconds
  )
{
  UINTN   Ticks;
  UINTN   Counts;
  UINTN   CurrentTick;
  UINTN   OriginalTick;
  UINTN   RemainingTick;
  UINT16  AcpiBase;

  if (Microseconds == 0) {
    return;
  }

  //
  // Please use PciRead here, it will link to MmioRead
  // if the caller is a Runtime driver, please use PchDxeRuntimePciLibPciExpress library, refer
  // PciExpressRead() on Library\DxeRuntimePciLibPciExpress\DxeRuntimePciLibPciExpress.c for the details.
  // For the rest please use EdkIIGlueBasePciLibPciExpress library
  //
  AcpiBase = PciRead16 (
               PCI_LIB_ADDRESS (
               DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_LPC,
               PCI_FUNCTION_NUMBER_PCH_LPC,
               R_PCH_LPC_ACPI_BASE)
               ) & B_PCH_LPC_ACPI_BASE_BAR;

  OriginalTick  = IoRead32 ((UINTN) (AcpiBase + R_PCH_ACPI_PM1_TMR)) & B_PCH_ACPI_PM1_TMR_VAL;
  CurrentTick   = OriginalTick;

  //
  // The timer frequency is 3.579545 MHz, so 1 ms corresponds 3.58 clocks
  //
  Ticks = Microseconds * 358 / 100 + OriginalTick + 1;

  //
  // The loops needed by timer overflow
  //
  Counts = Ticks / V_PCH_ACPI_PM1_TMR_MAX_VAL;

  //
  // Remaining clocks within one loop
  //
  RemainingTick = Ticks % V_PCH_ACPI_PM1_TMR_MAX_VAL;

  //
  // not intend to use TMROF_STS bit of register PM1_STS, because this adds extra
  // one I/O operation, and maybe generate SMI
  //
  while ((Counts != 0) || (RemainingTick > CurrentTick)) {
    CurrentTick = IoRead32 ((UINTN) (AcpiBase + R_PCH_ACPI_PM1_TMR)) & B_PCH_ACPI_PM1_TMR_VAL;
    //
    // Check if timer overflow
    //
    if ((CurrentTick < OriginalTick)) {
      if (Counts != 0) {
        Counts--;
      } else {
        //
        // If timer overflow and Counts equ to 0, that means we already stalled more than
        // RemainingTick, break the loop here
        //
        break;
      }
    }

    OriginalTick = CurrentTick;
  }
}

/**
  Check whether SPI is in descriptor mode

  @param[in] SpiBase              The PCH SPI Base Address

  @retval TRUE                    SPI is in descriptor mode
  @retval FALSE                   SPI is not in descriptor mode

**/
BOOLEAN
EFIAPI
PchIsSpiDescriptorMode (
  IN  UINTN   SpiBase
  )
{
  if ((MmioRead16 (SpiBase + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FDV) == B_PCH_SPI_HSFS_FDV) {
    MmioAndThenOr32 (
      SpiBase + R_PCH_SPI_FDOC,
      (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
      (UINT32) (V_PCH_SPI_FDOC_FDSS_FSDM | R_PCH_SPI_FDBAR_FLVALSIG)
      );
    if ((MmioRead32 (SpiBase + R_PCH_SPI_FDOD)) == V_PCH_SPI_FDBAR_FLVALSIG) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

/**
  Return SoC stepping type

  @retval SOC_STEPPING            SoC stepping type

**/
SOC_STEPPING
EFIAPI
SocStepping (
  VOID
  )
{
  UINT8 RevId;

  RevId = MmioRead8 (
            MmPciAddress (0,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_LPC,
            PCI_FUNCTION_NUMBER_PCH_LPC,
            R_PCH_LPC_RID_CC)
            );

  switch (RevId & B_PCH_LPC_RID_STEPPING_MASK) {
    case V_PCH_LPC_RID_A0:
      return SocA0;
      break;
    case V_PCH_LPC_RID_A1:
      return SocA1;
      break;
    case V_PCH_LPC_RID_A2:
      return SocA2;
      break;
    case V_PCH_LPC_RID_A3:
      return SocA3;
      break;
    case V_PCH_LPC_RID_A4:
      return SocA4;
      break;
    case V_PCH_LPC_RID_A5:
      return SocA5;
      break;
    case V_PCH_LPC_RID_A6:
      return SocA6;
      break;
    case V_PCH_LPC_RID_A7:
      return SocA7;
      break;
    case V_PCH_LPC_RID_B0:
      return SocB0;
      break;
    case V_PCH_LPC_RID_B1:
      return SocB1;
      break;
    case V_PCH_LPC_RID_B2:
      return SocB2;
      break;
    case V_PCH_LPC_RID_B3:
      return SocB3;
      break;
    case V_PCH_LPC_RID_B4:
      return SocB4;
      break;
    case V_PCH_LPC_RID_B5:
      return SocB5;
      break;
    case V_PCH_LPC_RID_B6:
      return SocB6;
      break;
    case V_PCH_LPC_RID_B7:
      return SocB7;
      break;
    case V_PCH_LPC_RID_C0:
      return SocC0;
      break;
    case V_PCH_LPC_RID_C1:
      return SocC1;
      break;
    case V_PCH_LPC_RID_C2:
      return SocC2;
      break;
    case V_PCH_LPC_RID_C3:
      return SocC3;
      break;
    case V_PCH_LPC_RID_C4:
      return SocC4;
      break;
    case V_PCH_LPC_RID_C5:
      return SocC5;
      break;
    case V_PCH_LPC_RID_C6:
      return SocC6;
      break;
    case V_PCH_LPC_RID_C7:
      return SocC7;
      break;
    default:
      return SocSteppingMax;
      break;
  }
}

/**
  Return SoC package type

  @retval SOC_PACKAGE            SoC package type

**/
SOC_PACKAGE
EFIAPI
GetSocPackageType (
VOID
)
{
  UINT8 RevId;

  RevId = MmioRead8(
    MmPciAddress(0,
    DEFAULT_PCI_BUS_NUMBER_PCH,
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_RID_CC)
    );

  switch (RevId & B_PCH_LPC_RID_PACKAGE_TYPE_MASK) {
    case V_PCH_LPC_RID_PACKAGE_TYPE_17x17_T4:
      return SocPackage17x17T4;
      break;
    case V_PCH_LPC_RID_PACKAGE_TYPE_25x27:
      return SocPackage25x27;
      break;
    case V_PCH_LPC_RID_PACKAGE_TYPE_17x17_T3:
      return SocPackage17x17T3;
      break;
    case V_PCH_LPC_RID_PACKAGE_TYPE_COPOP:
      return SocPackageCoPop;
      break;
    default:
      return SocPackageMax;
      break;
  }
}

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  )
{
  UINT32  Identifiers;
  UINT16  PcuVendorId;
  UINT16  PcuDeviceId;

  Identifiers = MmioRead32 (
                  MmPciAddress (0,
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  R_PCH_LPC_REG_ID)
                );

  PcuDeviceId = (UINT16) ((Identifiers & B_PCH_LPC_DEVICE_ID) >> 16);
  PcuVendorId = (UINT16) (Identifiers & B_PCH_LPC_VENDOR_ID);

  //
  // Verify that this is a supported chipset
  //
  if (PcuVendorId != (UINT16) V_PCH_LPC_VENDOR_ID || !IS_PCH_LPC_DEVICE_ID (PcuDeviceId)) {
    DEBUG ((EFI_D_ERROR, "This code doesn't support the SoC with PcuDeviceId: 0x%04x!\n", PcuDeviceId));
    return FALSE;
  }
  return TRUE;
}

/**
  This function can be called to enable/disable Alternate Access Mode

  @param[in] IlbBase              The PCH ILB Base Address
  @param[in] AmeCtrl              If TRUE, enable Alternate Access Mode.
                                  If FALSE, disable Alternate Access Mode.
**/
VOID
EFIAPI
PchAlternateAccessMode (
  IN  UINTN         IlbBase,
  IN  BOOLEAN       AmeCtrl
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  Data32Or  = 0;
  Data32And = 0xFFFFFFFF;

  if (AmeCtrl == TRUE) {
    //
    // Enable Alternate Access Mode
    // Note: The RTC Index field (including the NMI mask at bit7) is write-only
    // for normal operation and can only be read in Alt Access Mode.
    //
    Data32Or  = (UINT32) (B_PCH_ILB_MC_AME);
  }

  if (AmeCtrl == FALSE) {
    //
    // Disable Alternate Access Mode
    //
    Data32And = (UINT32) ~(B_PCH_ILB_MC_AME);
  }

  //
  // Program Alternate Access Mode Enable bit
  //
  MmioAndThenOr32 (
    IlbBase + R_PCH_ILB_MC,
    Data32And,
    Data32Or
    );

  //
  // Reads back for posted write to take effect
  //
  MmioRead32(IlbBase + R_PCH_ILB_MC);
}

/**
  This function can be called to poll for certain value within a time given.

  @param[in] MmioAddress          The Mmio Address.
  @param[in] BitMask              Bits to be masked.
  @param[in] BitValue             Value to be polled.
  #param[in] DelayTime            Delay time in terms of 100 micro seconds.

  @retval EFI_SUCCESS             Successfully polled the value.
  @retval EFI_TIMEOUT             Timeout while polling the value.
**/
EFI_STATUS
EFIAPI
PchMmioPoll32 (
  IN  UINTN         MmioAddress,
  IN  UINT32        BitMask,
  IN  UINT32        BitValue,
  IN  UINT16        DelayTime
  )
{
  UINT32  LoopTime;
  UINT8   PollSuccess;

  LoopTime    = 0;
  PollSuccess = 0;

  for (LoopTime = 0; LoopTime < DelayTime; LoopTime++) {
    if ((MmioRead32 (MmioAddress) & BitMask) == BitValue) {
      PollSuccess = 1;
      break;
    } else {
      PchPmTimerStall (100);
    }
  }

  if (PollSuccess) {
    return EFI_SUCCESS;
  } else {
    return EFI_TIMEOUT;
  }
}
