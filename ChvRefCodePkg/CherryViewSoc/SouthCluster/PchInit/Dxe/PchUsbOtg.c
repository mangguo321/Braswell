/** @file
  Initializes PCH USB On-The-Go Device.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchInit.h"

/**
  Hide PCI config space of OTG device and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureOtgAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  )
{
  UINTN                         OtgPciMmBase;
  UINT32                        OtgMmioBase0;
  UINT32                        OtgMmioBase1;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  EFI_STATUS                    Status;
#ifndef FSP_FLAG
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
#endif
  DEBUG ((EFI_D_INFO, "ConfigureOtgAtBoot() Start\n"));
  //
  // Initialize Variables
  //
  OtgPciMmBase     = 0;
  OtgMmioBase0     = 0;
  OtgMmioBase1     = 0;
  Data32And        = 0;
  Data32Or         = 0;
  Status           = EFI_SUCCESS;
  //
  // Update OTG device ACPI variables
  //
  if (PchPlatformPolicy->UsbConfig->UsbOtgSettings.Enable == PchAcpiMode) {
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

    OtgPciMmBase = MmPciAddress (
                     0,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_OTG,
                     PCI_FUNCTION_NUMBER_PCH_OTG,
                     0
                     );
    OtgMmioBase0 = MmioRead32 ((UINTN) (OtgPciMmBase + R_PCH_OTG_BAR0)) & B_PCH_OTG_BAR0_BA;
    ASSERT ((OtgMmioBase0 != 0) && (OtgMmioBase0 != B_PCH_OTG_BAR0_BA));
    OtgMmioBase1 = MmioRead32 ((UINTN) (OtgPciMmBase + R_PCH_OTG_BAR1)) & B_PCH_OTG_BAR1_BA;
    ASSERT ((OtgMmioBase1 != 0) && (OtgMmioBase1 != B_PCH_OTG_BAR1_BA));
    if (!IsSetS3BootScript) {

      MmioOr32 (
        (UINTN) (OtgPciMmBase + R_PCH_OTG_STSCMD),
        (UINT32) (B_PCH_OTG_STSCMD_INTR_DIS | B_PCH_OTG_STSCMD_BME | B_PCH_OTG_STSCMD_MSE)
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->UsbOtgAddr = OtgMmioBase0;
      GlobalNvsArea->Area->UsbOtgAddr1 = OtgMmioBase1;
#endif

      PchMsgBusAndThenOr32 (
        0x59,
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
        (UINTN) (OtgPciMmBase + R_PCH_OTG_BAR0),
        1,
        &OtgMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (OtgPciMmBase + R_PCH_OTG_BAR1),
        1,
        &OtgMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_OTG_STSCMD_INTR_DIS | B_PCH_OTG_STSCMD_BME | B_PCH_OTG_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (OtgPciMmBase + R_PCH_OTG_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0x59,
        0x500,
        0xFFFFFFFF,
        (BIT1 | BIT0),
        0x06,
        0x07
      );
    }
  }

  DEBUG ((EFI_D_INFO, "ConfigureOtgAtBoot() End\n"));

  return EFI_SUCCESS;
}
