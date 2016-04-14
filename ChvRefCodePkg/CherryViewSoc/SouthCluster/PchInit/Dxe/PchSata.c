/** @file
  Configures PCH SATA Controller.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifdef SATA_SUPPORT

#include "PchInit.h"

/**
  Do any final initialization on SATA controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureSataAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  )
{
  UINT8                             Index;
  UINT32                            AhciBar;
  UINTN                             PciD19F0RegBase;
  UINT16                            SataModeSelect;
  UINT32                            PxSctlDet;
  UINT32                            PxCmdSud;
  UINT32                            OrgCmdWord;
  UINT32                            Data32And;
  UINT32                            Data32Or;

  DEBUG ((EFI_D_INFO, "ConfigureSataAtBoot() Start\n"));
  //
  // eSATA port support only up to Gen2
  //
  PciD19F0RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA, 0);
  //
  // Make sure SATA device exists.
  //
  if (MmioRead16 (PciD19F0RegBase + R_PCH_SATA_ID) != 0xFFFF) {
    SataModeSelect  = MmioRead16 (PciD19F0RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
    if ((SataModeSelect == V_PCH_SATA_MAP_SMS_AHCI) ||
        (SataModeSelect == V_PCH_SATA_MAP_SMS_RAID)) {
      AhciBar = MmioRead32 (PciD19F0RegBase + R_PCH_SATA_ABAR) & B_PCH_SATA_ABAR_BA;
      //
      // Make sure the AhciBar is valid.
      //
      if ((AhciBar != 0x00000000) && (AhciBar != B_PCH_SATA_ABAR_BA)) {
        //
        // Keep original CMD word, and enable MSE
        //
        OrgCmdWord = MmioRead32 (PciD19F0RegBase + R_PCH_SATA_COMMAND);
        if ((OrgCmdWord & B_PCH_SATA_COMMAND_MSE) == 0) {
          Data32And = (UINT32) 0xFFFFFFFF;
          Data32Or  = (UINT32) B_PCH_SATA_COMMAND_MSE;
          if (!IsSetS3BootScript) {
            MmioOr32 ((PciD19F0RegBase + R_PCH_SATA_COMMAND), Data32Or);
          } else {
            //
            // Set S3 Boot Script
            //
            S3BootScriptSaveMemReadWrite (
              EfiBootScriptWidthUint32,
              (UINTN) (PciD19F0RegBase + R_PCH_SATA_COMMAND),
              &Data32Or,  /// Data to be ORed
              &Data32And  /// Data to be ANDed
              );
          }
        }
        for (Index = 0; Index < PCH_AHCI_MAX_PORTS; Index++) {
          if (PchPlatformPolicy->SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
            PxSctlDet = MmioRead32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))) & B_PCH_SATA_AHCI_PXSCTL_DET;
            PxCmdSud = MmioRead32(AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index))) & B_PCH_SATA_AHCI_PxCMD_SUD;
            //
            // Limit speed to Gen2
            //
            Data32And = (UINT32)~(B_PCH_SATA_AHCI_PXSCTL_SPD);
            Data32Or  = (UINT32) V_PCH_SATA_AHCI_PXSCTL_SPD_2;
            if (!IsSetS3BootScript) {
              MmioAndThenOr32 (
                (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
                Data32And,
                Data32Or
                );
            } else {
              //
              // Set S3 Boot Script
              //
              S3BootScriptSaveMemReadWrite (
                EfiBootScriptWidthUint32,
                (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
                &Data32Or,  /// Data to be ORed
                &Data32And  /// Data to be ANDed
                );
            }
            //
            // If port is not offline, and it's spin up, need to port reset.
            // After port reset, clear the SERR.
            // - Set DET=1, and then set DET=0.
            // - Finally, set FRE=1.
            //
            if ((PxSctlDet == V_PCH_SATA_AHCI_PXSCTL_DET_0) &&
                (PxCmdSud == B_PCH_SATA_AHCI_PxCMD_SUD))
            {
              if (!IsSetS3BootScript) {
                MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), V_PCH_SATA_AHCI_PXSCTL_DET_1);
                PchPmTimerStall (1000);
                MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET));
                MmioWrite32 (AhciBar + (R_PCH_SATA_AHCI_P0SERR + (0x80 * Index)), (UINT32)~0u);
                MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index)), B_PCH_SATA_AHCI_PxCMD_FRE);
              } else {
                //
                // Set S3 Boot Script
                //
                Data32And = (UINT32) 0xFFFFFFFF;
                Data32Or  = (UINT32) V_PCH_SATA_AHCI_PXSCTL_DET_1;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
                S3BootScriptSaveStall (1000);
                Data32And = (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET);
                Data32Or  = (UINT32) 0x00000000;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
                Data32And = (UINT32) 0xFFFFFFFF;
                Data32Or  = (UINT32) 0xFFFFFFFF;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SERR + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
                Data32And = (UINT32) 0xFFFFFFFF;
                Data32Or  = (UINT32) B_PCH_SATA_AHCI_PxCMD_FRE;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
              }
            }
            //
            // If port is offline, and it's not spin up, meets the power bug.
            // Need to do the W/A to spin up the port and then spin down.
            // Then entering back to offline and listen.
            // - Set DET=0, SUD=1, and then set SUD=0, DET=4.
            //
            if ((PxSctlDet == V_PCH_SATA_AHCI_PXSCTL_DET_4) &&
                (PxCmdSud == 0))
            {
              if (!IsSetS3BootScript) {
                MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET));
                MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index)), B_PCH_SATA_AHCI_PxCMD_SUD);
                PchPmTimerStall (1000);
                MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PxCMD_SUD));
                MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), V_PCH_SATA_AHCI_PXSCTL_DET_4);
              } else {
                //
                // Set S3 Boot Script
                //
                Data32And = (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET);
                Data32Or  = (UINT32) 0x00000000;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
                Data32And = (UINT32) 0xFFFFFFFF;
                Data32Or  = (UINT32) B_PCH_SATA_AHCI_PxCMD_SUD;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
                S3BootScriptSaveStall (1000);
                Data32And = (UINT32) ~(B_PCH_SATA_AHCI_PxCMD_SUD);
                Data32Or  = (UINT32) 0x00000000;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
                Data32And = (UINT32) 0xFFFFFFFF;
                Data32Or  = (UINT32) V_PCH_SATA_AHCI_PXSCTL_DET_4;
                S3BootScriptSaveMemReadWrite (
                  EfiBootScriptWidthUint32,
                  (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
                  &Data32Or,  /// Data to be ORed
                  &Data32And  /// Data to be ANDed
                  );
              }
            }
          }
        }
        //
        // Restore original CMD word.
        //
        if ((OrgCmdWord & B_PCH_SATA_COMMAND_MSE) == 0) {
          Data32And = (UINT32) 0xFFFFFFFF;
          Data32Or  = (UINT32) OrgCmdWord;
          if (!IsSetS3BootScript) {
            MmioWrite32 ((PciD19F0RegBase + R_PCH_SATA_COMMAND), Data32Or);
          } else {
            S3BootScriptSaveMemReadWrite (
              EfiBootScriptWidthUint32,
              (UINTN) (PciD19F0RegBase + R_PCH_SATA_COMMAND),
              &Data32Or,  /// Data to be ORed
              &Data32And  /// Data to be ANDed
              );
          }
        }
      } // AhciBar is vaild
    } // SATA mode is AHCI or RAID
  } // if D19F0 is existed

  DEBUG ((EFI_D_INFO, "ConfigureSataAtBoot() End\n"));
  return EFI_SUCCESS;
}

#endif
