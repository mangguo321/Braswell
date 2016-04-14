/** @file
  Do Early PCH platform initialization.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformEarlyInit.h"
#include <Ppi/PchPlatformPolicy.h>
#include "PchRegs.h"
#include "Ppi/PchInit.h"
#include <Library/PcdLib.h>


#define MC_PMSTS_OFFSET                 0xC

#define DEFAULT_BUS_INFO                0x2020

VOID
PchInitInterrupt (
  IN SYSTEM_CONFIGURATION  *SystemConfiguration
  );

#pragma warning (push)
#pragma warning (disable : 4245)
#pragma warning (pop)

UINT8
ReadCmosBank1Byte (
  IN UINT8                      Address
  )
{
  UINT8                           Data;

  IoWrite8 (R_PCH_RTC_EXT_INDEX, Address);
  Data = IoRead8 (R_PCH_RTC_EXT_TARGET);

  return Data;
}

VOID
WriteCmosBank1Byte (
  IN UINT8                     Address,
  IN UINT8                     Data
  )
{
  IoWrite8 (R_PCH_RTC_EXT_INDEX, Address);
  IoWrite8 (R_PCH_RTC_EXT_TARGET, Data);
}

/**
  Turn off system if needed.

  @param[in] 

  @retval

**/
VOID
CheckPowerOffNow (
  VOID
  )
{
  UINT16  Pm1Sts;

  //
  // Read and check the ACPI registers
  //
  Pm1Sts = IoRead16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS);
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_PWRBTN) == B_PCH_ACPI_PM1_STS_PWRBTN) {
    IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_PWRBTN);
    IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5);
    IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5 + B_PCH_ACPI_PM1_CNT_SLP_EN);
  }
}

VOID
ClearPowerState (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  UINT8   Data8;
  UINT16  Data16;
  UINT32  Data32;

  //
  // Check for PowerState option for AC power loss and program the chipset
  //

  //
  // Clear PWROK (Set to Clear)
  //
  MmioOr32 (PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1, B_PCH_PMC_GEN_PMCON_PWROK_FLR);

  //
  // Clear Power Failure Bit (Set to Clear)
  //
  // TODO: Check if it is OK to clear here
  MmioOr32 (PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1, B_PCH_PMC_GEN_PMCON_SUS_PWR_FLR);

  //
  // Clear the GPE and PM enable
  //
  IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_EN, (UINT16) 0x00);
  IoWrite32 (ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_EN, (UINT32) 0x00);

  //
  // Halt the TCO timer
  //
  Data16 = IoRead16 (ACPI_BASE_ADDRESS + R_PCH_TCO_CNT);
  Data16 |= B_PCH_TCO_CNT_TMR_HLT;
  IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_TCO_CNT, Data16);

  //
  // if NMI_NOW_STS is set
  // Write '1' to Invert NMI_NOW & NMI_NOW_STS
  //
  Data8 = MmioRead8(ILB_BASE_ADDRESS + R_PCH_ILB_GNMI);
  if ((Data8 & B_PCH_ILB_GNMI_NMINS) == B_PCH_ILB_GNMI_NMINS) {
    MmioOr8 (ILB_BASE_ADDRESS + R_PCH_ILB_GNMI, B_PCH_ILB_GNMI_NMIN);
  }
  //
  // Before we clear the TO status bit here we need to save the results in a CMOS bit for later use.
  //
  Data32 = IoRead32 (ACPI_BASE_ADDRESS + R_PCH_TCO_STS);
  if ((Data32 & B_PCH_TCO_STS_SECOND_TO) == B_PCH_TCO_STS_SECOND_TO) {
#if (defined(HW_WATCHDOG_TIMER_SUPPORT) && (HW_WATCHDOG_TIMER_SUPPORT != 0))
    WriteCmosBank1Byte (
      EFI_CMOS_PERFORMANCE_FLAGS,
      ReadCmosBank1Byte (EFI_CMOS_PERFORMANCE_FLAGS) | B_CMOS_TCO_WDT_RESET
      );
#endif
  }
  //
  // Now clear the TO status bit (Write '1' to clear)
  //
  IoWrite32 (ACPI_BASE_ADDRESS + R_PCH_TCO_STS, (UINT32) (Data32 | B_PCH_TCO_STS_SECOND_TO));
}

/**
  Clear any SMI status or wake status left over from boot.

  @param[in]
  
  @retval

**/
VOID
ClearSmiAndWake (
  VOID
  )
{
  UINT16  Pm1Sts;
  UINT32  Gpe0Sts;
  UINT32  SmiSts;

  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS);
  Gpe0Sts = IoRead32 (ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS);
  SmiSts  = IoRead32 (ACPI_BASE_ADDRESS + R_PCH_SMI_STS);

  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts = (B_PCH_ACPI_PM1_STS_PRBTNOR | B_PCH_ACPI_PM1_STS_PWRBTN);

  Gpe0Sts |=
    (
      B_PCH_ACPI_GPE0a_STS_CORE_GPIO |
      B_PCH_ACPI_GPE0a_STS_SUS_GPIO |
      B_PCH_ACPI_GPE0a_STS_TCO |
      B_PCH_ACPI_GPE0a_STS_PME_B0 |
      B_PCH_ACPI_GPE0a_STS_BATLOW |
      B_PCH_ACPI_GPE0a_STS_PCI_EXP |
      B_PCH_ACPI_GPE0a_STS_GUNIT_SCI |
      B_PCH_ACPI_GPE0a_STS_PUNIT_SCI |
      B_PCH_ACPI_GPE0a_STS_SWGPE |
      B_PCH_ACPI_GPE0a_STS_HOT_PLUG |
      B_PCH_ACPI_GPE0a_STS_PMU_WAKEB
    );

  SmiSts |=
    (
      B_PCH_SMI_STS_SMBUS |
      B_PCH_SMI_STS_PERIODIC |
      B_PCH_SMI_STS_TCO |
      B_PCH_SMI_STS_SWSMI_TMR |
      B_PCH_SMI_STS_APM |
      B_PCH_SMI_STS_ON_SLP_EN |
      B_PCH_SMI_STS_BIOS
    );

  //
  // Write them back
  //
  IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS, Pm1Sts);
  IoWrite32 (ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS, Gpe0Sts);
  IoWrite32 (ACPI_BASE_ADDRESS + R_PCH_SMI_STS, SmiSts);
}

/**
  Issue PCI-E Secondary Bus Reset
  
  @param[in]   PeiServices        General-purpose services that are available to every PEIM.
  @param[in]   Bus                Bus number of the bridge
  @param[in]   Dev                Devices number of the bridge
  @param[in]   Fun                Function number of the bridge

  @retval  EFI_SUCCESS

**/
EFI_STATUS
PcieSecondaryBusReset (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN UINT8                   Bus,
  IN UINT8                   Dev,
  IN UINT8                   Fun
  )
{
  EFI_PEI_STALL_PPI   *PeiStall;
  EFI_STATUS          Status;

  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gEfiPeiStallPpiGuid,
                             0,
                             NULL,
                             &PeiStall
                             );
  ASSERT_EFI_ERROR (Status);

  //
  // Issue secondary bus reset
  //
  MmPci16Or(0, Bus, Dev, Fun, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, EFI_PCI_BRIDGE_CONTROL_RESET_SECONDARY_BUS);

  //
  // Wait 1ms
  //
  PeiStall->Stall (PeiServices, PeiStall, 1000);

  //
  // Clear the reset bit
  // Note: The PCIe spec suggests 100ms delay between clearing this bit and accessing
  // the device's config space. Since we will not access the config space until we enter DXE
  // we don't put delay expressly here.
  //
  MmPci16And(0, Bus, Dev, Fun, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, ~(EFI_PCI_BRIDGE_CONTROL_RESET_SECONDARY_BUS));

  return EFI_SUCCESS;
}

/**
  Provide hard reset PPI service.
  To generate full hard reset, write 0x0E to ICH RESET_GENERATOR_PORT (0xCF9).

  @param[in]  PeiServices        General purpose services available to every PEIM.

  @retval  Not return            System reset occured.
  @retval  EFI_DEVICE_ERROR      Device error, could not reset the system.

**/
EFI_STATUS
EFIAPI
IchReset (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  IoWrite8 (
    R_PCH_RST_CNT,
    V_PCH_RST_CNT_HARDSTARTSTATE
    );

  IoWrite8 (
    R_PCH_RST_CNT,
    V_PCH_RST_CNT_HARDRESET
    );

  //
  // System reset occured, should never reach at this line.
  //
  ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);

  return EFI_DEVICE_ERROR;
}

VOID
PchPlatformLpcInit (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN SYSTEM_CONFIGURATION       *SystemConfiguration
  )
{
  EFI_BOOT_MODE BootMode;
  UINT8         Data8;
  UINT16        Data16;

  (*PeiServices)->GetBootMode(PeiServices, &BootMode);

  if ((BootMode != BOOT_ON_S3_RESUME)) {
    //
    // Clear all pending SMI. On S3 clear power button enable so it wll not generate an SMI
    //
    ClearSmiAndWake ();
  }

  ClearPowerState (SystemConfiguration);

  //
  // Disable SERR NMI and IOCHK# NMI in port 61
  //
  Data8 = IoRead8 (R_PCH_NMI_SC);
  IoWrite8(R_PCH_NMI_SC, (UINT8) (Data8 | B_PCH_NMI_SC_PCI_SERR_EN | B_PCH_NMI_SC_IOCHK_NMI_EN));

  //
  // Enable Bus Master, I/O, Mem, and SERR on LPC bridge
  //
  Data16 = PchLpcPciCfg16 (R_PCH_LPC_COMMAND);
  MmioWrite16 (
    MmPciAddress (0,
      DEFAULT_PCI_BUS_NUMBER_PCH,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_COMMAND
    ),
    (Data16 |
     B_PCH_LPC_COMMAND_IOSE |
     B_PCH_LPC_COMMAND_MSE |
     B_PCH_LPC_COMMAND_BME |
     B_PCH_LPC_COMMAND_SERR_EN)
  );
}

VOID
IchRcrbInit (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  EFI_BOOT_MODE                   BootMode;

  (*PeiServices)->GetBootMode(PeiServices, &BootMode);

  //
  // If not recovery or flash update boot path. set the BIOS interface lock down bit.
  // It locks the top swap bit and BIOS boot strap bits from being changed.
  //
  if ((BootMode != BOOT_IN_RECOVERY_MODE) && (BootMode != BOOT_ON_FLASH_UPDATE)) {
    MmioOr8 (RCBA_BASE_ADDRESS + R_PCH_RCRB_GCS, B_PCH_RCRB_GCS_BILD);
  }

  //
  // Disable the Watchdog timer expiration from causing a system reset
  //
  MmioOr8 (PMC_BASE_ADDRESS + R_PCH_PMC_PM_CFG, B_PCH_PMC_PM_CFG_NO_REBOOT);

  if ((BootMode == BOOT_ON_S3_RESUME)) {
    //
    // We are resuming from S3
    // Program HPET based on Setup
    //
    if (SystemConfiguration->Hpet == 1) {
      MmioOr8 (R_PCH_PCH_HPET + R_PCH_PCH_HPET_GCFG, B_PCH_PCH_HPET_GCFG_EN);
    } else {
      MmioAnd8 (R_PCH_PCH_HPET + R_PCH_PCH_HPET_GCFG, (UINT8) ~(B_PCH_PCH_HPET_GCFG_EN));
    }
  }
}

VOID
PchInitInterrupt (
  IN SYSTEM_CONFIGURATION  *SystemConfiguration
  )
{
  DEBUG ((EFI_D_ERROR, "PchInitInterrupt () - Start\n"));

  //
  // Configure LPSS Interrupts (Done by FSP) 
  //

  //
  // Program Interrupt routing registers
  //
  //
  // Device 31 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D31IR),
    V_PCH_ILB_DXXIR_IBR_PIRQC    // For SMBUS
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D31IR); // Read Posted Writes Register

  //
  // Device 30 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D30IR),
    V_PCH_ILB_DXXIR_IAR_PIRQD +  // For LPSS1
    V_PCH_ILB_DXXIR_IBR_PIRQB +
    V_PCH_ILB_DXXIR_ICR_PIRQC +
    V_PCH_ILB_DXXIR_IDR_PIRQA
  );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D30IR); // Read Posted Writes Register

  //
  // Device 28 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D28IR),
    V_PCH_ILB_DXXIR_IAR_PIRQA +  // For PCIe #1
    V_PCH_ILB_DXXIR_IBR_PIRQB +  // For PCIe #2
    V_PCH_ILB_DXXIR_ICR_PIRQC +  // For PCIe #3
    V_PCH_ILB_DXXIR_IDR_PIRQD    // For PCIe #4
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D28IR); // Read Posted Writes Register
  //
  // Device 27 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D27IR),
    V_PCH_ILB_DXXIR_IAR_PIRQG    // For Azalia
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D27IR); // Read Posted Writes Register

  //
  // Device 26 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D26IR),
    V_PCH_ILB_DXXIR_IAR_PIRQF    // For SEC
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D26IR); // Read Posted Writes Register

  //
  // Device 24 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D24IR),
    V_PCH_ILB_DXXIR_IAR_PIRQB |  // For LPSS2
    V_PCH_ILB_DXXIR_IBR_PIRQA |
    V_PCH_ILB_DXXIR_ICR_PIRQD |
    V_PCH_ILB_DXXIR_IDR_PIRQC
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D24IR); // Read Posted Writes Register

  //
  // Device 22 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D22IR),
    V_PCH_ILB_DXXIR_IAR_PIRQH   // For OTG
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D22IR); // Read Posted Writes Register

  //
  // Device 21 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D21IR),
    V_PCH_ILB_DXXIR_IAR_PIRQF   // For LPE
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D21IR); // Read Posted Writes Register

  //
  // Device 20 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D20IR),
    V_PCH_ILB_DXXIR_IAR_PIRQE   // For xHCI
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D20IR); // Read Posted Writes Register
  //
  // Device 19 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D19IR),
    V_PCH_ILB_DXXIR_IAR_PIRQD   // For SATA
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D19IR); // Read Posted Writes Register

  //
  // Device 18 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D18IR),
    V_PCH_ILB_DXXIR_IAR_PIRQC   // For SDIO #2
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D18IR); // Read Posted Writes Register

  //
  // Device 17 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D17IR),
    V_PCH_ILB_DXXIR_IAR_PIRQB   // For SDIO #1
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D17IR); // Read Posted Writes Register

  //
  // Device 16 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D16IR),
    V_PCH_ILB_DXXIR_IAR_PIRQA   // For SDIO #0
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D16IR); // Read Posted Writes Register

  //
  // Device 10 Interrupt Route
  //
  MmioWrite16 (
    (ILB_BASE_ADDRESS + R_PCH_ILB_D10IR),
    V_PCH_ILB_DXXIR_IAR_PIRQE   // For ISH
    );
  MmioRead16 (ILB_BASE_ADDRESS + R_PCH_ILB_D10IR); // Read Posted Writes Register

  DEBUG ((EFI_D_ERROR, "PchInitInterrupt () - End\n"));
}

EFI_STATUS
PlatformPchInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN UINT16                      PlatformType
  )
{
  //
  // Setup Interrupt Routing
  //
  PchInitInterrupt (SystemConfiguration);

  PchPlatformLpcInit (PeiServices, SystemConfiguration);

  return EFI_SUCCESS;
}

/**
  Returns the state of A16 inversion

  @param  

  @retval  TRUE        A16 is inverted
  @retval  FALSE       A16 is not inverted

**/
BOOLEAN
IsA16Inverted (
  )
{
  UINT8  Data;

  Data = MmioRead8 (RCBA_BASE_ADDRESS + R_PCH_RCRB_GCS);
  return (Data & B_PCH_RCRB_GCS_TS) ? TRUE : FALSE;
}
