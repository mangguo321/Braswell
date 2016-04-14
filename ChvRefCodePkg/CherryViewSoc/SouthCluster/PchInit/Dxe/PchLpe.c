/** @file
  Initializes PCH Low Power Audio Engine Device.

  @Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchInit.h"

UINT32  gIndex = 0;

typedef struct {
  EFI_PHYSICAL_ADDRESS  LpeBar2Base;
  UINT32                LpeBar2Length;
  } LPE_MEM_LIST;

LPE_MEM_LIST gLpeList[] = {
  {0x20000000,   0x00200000},   // 512M
  {0x40000000,   0x00200000},   // 1G
  {0x60000000,   0x00200000},   // 1.5G
  {0x00000000,   0x00000000}    // END
};

/**
  Hide PCI config space of LPE device and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureLpeAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  )
{
  UINTN                         LpePciMmBase;
  UINT32                        LpeMmioBase0;
  UINT32                        LpeMmioBase1;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  EFI_STATUS                    Status;
#ifndef FSP_FLAG
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
#endif
  DEBUG ((EFI_D_INFO, "ConfigureLpeAtBoot() Start\n"));
  //
  // Initialize Variables
  //
  LpePciMmBase     = 0;
  LpeMmioBase0     = 0;
  LpeMmioBase1     = 0;
  Data32And        = 0;
  Data32Or         = 0;
  Status           = EFI_SUCCESS;
  //
  // Update LPE device ACPI variables
  //
  if (PchPlatformPolicy->DeviceEnabling->LpeEnabled == PchAcpiMode) {
#ifndef FSP_FLAG
    //
    // Locate GlobalNVS protocol
    //
    Status = gBS->LocateProtocol (
                    &gEfiGlobalNvsAreaProtocolGuid,
                    NULL,
                    (VOID **) &GlobalNvsArea
                    );
    if (EFI_ERROR(Status)) {
      return Status;
    }
#endif

    LpePciMmBase = MmPciAddress (
                     0,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPE,
                     PCI_FUNCTION_NUMBER_PCH_LPE,
                     0
                     );
    LpeMmioBase0 = MmioRead32 ((UINTN) (LpePciMmBase + R_PCH_LPE_BAR0)) & B_PCH_LPE_BAR0_BA;
    ASSERT ((LpeMmioBase0 != 0) && (LpeMmioBase0 != B_PCH_LPE_BAR0_BA));
    LpeMmioBase1 = MmioRead32 ((UINTN) (LpePciMmBase + R_PCH_LPE_BAR1)) & B_PCH_LPE_BAR1_BA;
    ASSERT ((LpeMmioBase1 != 0) && (LpeMmioBase1 != B_PCH_LPE_BAR1_BA));
    if (!IsSetS3BootScript) {

      MmioOr32 (
        (UINTN) (LpePciMmBase + R_PCH_LPE_STSCMD),
        (UINT32) (B_PCH_LPE_STSCMD_INTR_DIS | B_PCH_LPE_STSCMD_BME | B_PCH_LPE_STSCMD_MSE)
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->LPEBar0 = LpeMmioBase0;
      GlobalNvsArea->Area->LPEBar1 = LpeMmioBase1;
      GlobalNvsArea->Area->LPEBar2 = (UINT32) gLpeList[gIndex].LpeBar2Base;
#endif
      DEBUG ((EFI_D_INFO, "Memory space allocated for LPE Audio is 0x%08llX with size 0x%X\n", gLpeList[gIndex].LpeBar2Base, gLpeList[gIndex].LpeBar2Length));

      PchMsgBusAndThenOr32 (
        0x58,
        0x500,
        0xFFFFFFFF,
        (BIT1 | BIT0),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpePciMmBase + R_PCH_LPE_BAR0),
        1,
        &LpeMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpePciMmBase + R_PCH_LPE_BAR1),
        1,
        &LpeMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPE_STSCMD_INTR_DIS | B_PCH_LPE_STSCMD_BME | B_PCH_LPE_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpePciMmBase + R_PCH_LPE_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0x58,
        0x500,
        0xFFFFFFFF,
        (BIT1 | BIT0),
        0x06,
        0x07
      );
    }
  }

  DEBUG ((EFI_D_INFO, "ConfigureLpeAtBoot() End\n"));

  return EFI_SUCCESS;
}
