/** @file
  Initializes PCH SCC Devices.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchInit.h"
#include <Protocol/GlobalNvsArea.h>
#include <Library/TimerLib.h>

/**
  Hide PCI config space of SCC devices and do any final initialization

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureSccAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  )
{
  UINTN                         SccPciMmBase;
  UINT32                        SccMmioBase0;
  UINT32                        SccMmioBase1;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  EFI_STATUS                    Status;
#ifndef FSP_FLAG
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
#endif
  UINTN                         PciD31F0RegBase;
  UINT32                        IoBase;
  UINT16                        Timeout;

  DEBUG ((EFI_D_INFO, "ConfigureSccAtBoot() Start\n"));

  //
  // Initialize Variables
  //
  SccPciMmBase     = 0;
  SccMmioBase0     = 0;
  SccMmioBase1     = 0;
  Data32And        = 0;
  Data32Or         = 0;
  Status           = EFI_SUCCESS;
#ifndef FSP_FLAG
  //
  // Update SCC devices ACPI variables
  //
  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &GlobalNvsArea
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
#endif
  //
  // SCC eMMC
  //
  if (PchPlatformPolicy->SccConfig->eMMCEnabled == PchAcpiMode) {

    SccPciMmBase = MmPciAddress (0,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SCC_SDIO_0,
                     PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,
                     0
                   );
    if (MmioRead32 (SccPciMmBase) != 0xFFFFFFFF) {
    SccMmioBase0 = MmioRead32 ((UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR)) & B_PCH_SCC_SDIO_BAR_BA;
    ASSERT ((SccMmioBase0 != 0) && (SccMmioBase0 != B_PCH_SCC_SDIO_BAR_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_INTRDIS | B_PCH_SCC_SDIO_STSCMD_BME | B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->eMMCAddr  = SccMmioBase0;
      GlobalNvsArea->Area->eMMCLen   = V_PCH_SCC_SDIO_BAR_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        PCH_SCC_EP_PORT_ID,
        R_PCH_SCC_EP_PCICFGCTR1,
        0xFFFFFFFF,
        (B_PCH_SCC_EP_PCICFGCTR1_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR1_PCI_CFG_DIS1),
        PCH_SCC_EP_PRIVATE_READ_OPCODE,
        PCH_SCC_EP_PRIVATE_WRITE_OPCODE
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR),
        1,
        &SccMmioBase0
        );
      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_INTRDIS | B_PCH_SCC_SDIO_STSCMD_BME | B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        PCH_SCC_EP_PORT_ID,
        R_PCH_SCC_EP_PCICFGCTR1,
        0xFFFFFFFF,
        (B_PCH_SCC_EP_PCICFGCTR1_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR1_PCI_CFG_DIS1),
        PCH_SCC_EP_PRIVATE_READ_OPCODE,
        PCH_SCC_EP_PRIVATE_WRITE_OPCODE
      );
    }
   }
  }

  //
  // SCC SDIO
  //
  if (PchPlatformPolicy->SccConfig->SdioEnabled == PchAcpiMode) {

    SccPciMmBase = MmPciAddress (0,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SCC_SDIO_1,
                     PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,
                     0
                   );
    if (MmioRead32 (SccPciMmBase) != 0xFFFFFFFF) {
    SccMmioBase0 = MmioRead32 ((UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR)) & B_PCH_SCC_SDIO_BAR_BA;
    ASSERT ((SccMmioBase0 != 0) && (SccMmioBase0 != B_PCH_SCC_SDIO_BAR_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_INTRDIS | B_PCH_SCC_SDIO_STSCMD_BME | B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->SDIOAddr  = SccMmioBase0;
      GlobalNvsArea->Area->SDIOLen   = V_PCH_SCC_SDIO_BAR_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        PCH_SCC_EP_PORT_ID,
        R_PCH_SCC_EP_PCICFGCTR3,
        0xFFFFFFFF,
        (B_PCH_SCC_EP_PCICFGCTR3_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR3_PCI_CFG_DIS1),
        PCH_SCC_EP_PRIVATE_READ_OPCODE,
        PCH_SCC_EP_PRIVATE_WRITE_OPCODE
      );
    } else {
      ///
      /// Set S3 Boot Script
      ///
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR),
        1,
        &SccMmioBase0
        );
      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_INTRDIS | B_PCH_SCC_SDIO_STSCMD_BME | B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        PCH_SCC_EP_PORT_ID,
        R_PCH_SCC_EP_PCICFGCTR3,
        0xFFFFFFFF,
        (B_PCH_SCC_EP_PCICFGCTR3_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR3_PCI_CFG_DIS1),
        PCH_SCC_EP_PRIVATE_READ_OPCODE,
        PCH_SCC_EP_PRIVATE_WRITE_OPCODE
      );
    }
   }
  }

  //
  // SCC SD Card
  //
  if (PchPlatformPolicy->SccConfig->SdcardEnabled != PchDisabled) {
    SccPciMmBase = MmPciAddress (0,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2,
                     PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,
                     0
                     );
    if (MmioRead32 (SccPciMmBase) != 0xFFFFFFFF) {
    SccMmioBase0 = MmioRead32 ((UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR)) & B_PCH_SCC_SDIO_BAR_BA;
    ASSERT ((SccMmioBase0 != 0) && (SccMmioBase0 != B_PCH_SCC_SDIO_BAR_BA));
    //
    // SDCard RCOMP - Stage 2
    // Stage 1 done in PchInitPeim.c PchInitialize ()
    // 1. Disable 1.8V buffer mode
    // 2. Memory space enable
    // 3. Wait for the card detect level, card state stable and card inserted.
    // 4. Turn on 3.3V power.
    // 5. Wait 5ms for power to stable.
    // 6. Enable SDCard RCOMP to start calibration
    // 7. Wait for calibration done.
    // 8. Turn off power
    // 9. Turn off card insertion simulation
    //
    PciD31F0RegBase = MmPciAddress (
                        0,
                        PchPlatformPolicy->BusNumber,
                        PCI_DEVICE_NUMBER_PCH_LPC,
                        PCI_FUNCTION_NUMBER_PCH_LPC,
                        0
                        );
    IoBase = MmioRead32 (PciD31F0RegBase + R_PCH_LPC_IO_BASE) & B_PCH_LPC_IO_BASE_BAR;
    Timeout = 3000;
    if (!IsSetS3BootScript) {
      if ((MmioRead32 ((UINTN) (IoBase + R_PCH_CFIO_SOUTHEAST + 0x1180)) & BIT31) != BIT31) {
        MmioAnd32 (((UINTN) (IoBase + R_PCH_CFIO_SOUTHEAST + 0x1194)), (UINT32)~BIT21);
        MmioOr32 ((UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD), (UINT32) (B_PCH_SCC_SDIO_STSCMD_MSE));
        while (((MmioRead32 ((UINTN) (SccMmioBase0 + R_PCH_SCC_SDIO_MEM_PRE_STATE)) & 0x70000) != 0x70000) && Timeout != 0) {
          MicroSecondDelay (1000);
          Timeout--;
        }
        MmioWrite8 (
          (UINTN) (SccMmioBase0 + R_PCH_SCC_SDIO_MEM_PWR_CTL),
          (UINT8) (V_PCH_SCC_SDIO_MEM_PWR_CTL_SD_VOLSEL_3P3 | B_PCH_SCC_SDIO_MEM_PWR_CTL_SD_PWR)
          );
        PchPmTimerStall (5000);
        MmioOr32 (((UINTN) (IoBase + R_PCH_CFIO_SOUTHEAST + 0x1180)), (UINT32) BIT31);
        PchPmTimerStall (10);
        MmioOr32 (((UINTN) (IoBase + R_PCH_CFIO_SOUTHEAST + 0x1180)), (UINT32) BIT30);
        PchPmTimerStall (10);
        MmioAnd32 (((UINTN) (IoBase + R_PCH_CFIO_SOUTHEAST + 0x1180)), (UINT32)~BIT30);
        MmioWrite8 ((UINTN) (SccMmioBase0 + R_PCH_SCC_SDIO_MEM_PWR_CTL), (UINT8) 0x00);
        MmioWrite8 ((UINTN) (SccMmioBase0 + R_PCH_SCC_SDIO_MEM_HOST_CTL), (UINT8) 0x00);
      }
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR),
        1,
        &SccMmioBase0
        );
      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      Data32Or  = (UINT32) (0x00);
      Data32And = 0xFFFFFF00;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (SccMmioBase0 + 0x28),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
    }
   }
  }

  if (PchPlatformPolicy->SccConfig->SdcardEnabled == PchAcpiMode) {
    SccPciMmBase = MmPciAddress (0,
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2,
                   PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,
                   0
                   );
    if (MmioRead32 (SccPciMmBase) != 0xFFFFFFFF) {
    SccMmioBase0 = MmioRead32 ((UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR)) & B_PCH_SCC_SDIO_BAR_BA;
    ASSERT ((SccMmioBase0 != 0) && (SccMmioBase0 != B_PCH_SCC_SDIO_BAR_BA));

    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_INTRDIS | B_PCH_SCC_SDIO_STSCMD_BME | B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->SDCardAddr  = SccMmioBase0;
      GlobalNvsArea->Area->SDCardLen   = V_PCH_SCC_SDIO_BAR_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        PCH_SCC_EP_PORT_ID,
        R_PCH_SCC_EP_PCICFGCTR2,
        0xFFFFFFFF,
        (B_PCH_SCC_EP_PCICFGCTR2_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR2_PCI_CFG_DIS1),
        PCH_SCC_EP_PRIVATE_READ_OPCODE,
        PCH_SCC_EP_PRIVATE_WRITE_OPCODE
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_BAR),
        1,
        &SccMmioBase0
        );
      Data32Or  = (UINT32) (B_PCH_SCC_SDIO_STSCMD_INTRDIS | B_PCH_SCC_SDIO_STSCMD_BME | B_PCH_SCC_SDIO_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (SccPciMmBase + R_PCH_SCC_SDIO_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        PCH_SCC_EP_PORT_ID,
        R_PCH_SCC_EP_PCICFGCTR2,
        0xFFFFFFFF,
        (B_PCH_SCC_EP_PCICFGCTR2_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR2_PCI_CFG_DIS1),
        PCH_SCC_EP_PRIVATE_READ_OPCODE,
        PCH_SCC_EP_PRIVATE_WRITE_OPCODE
      );
    }
   }
  }

  DEBUG ((EFI_D_INFO, "ConfigureSccAtBoot() End\n"));

  return EFI_SUCCESS;
}
