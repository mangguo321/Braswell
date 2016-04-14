/** @file
  Platform Emmc Dxe driver to perform Emmc DLL Tuning.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformEmmcDxe.h"

/**
  After EFI_EMMC_CARD_INFO_PROTOCOL and PCH_EMMC_TUNING_PROTOCOL installed,
  configure platform EMMC for HS200/HS400 Tuning and set variable for HS200/HS400 Tuning Data.

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
ConfigurePlatformEmmc (
  IN EFI_EVENT                  Event,
  IN VOID                       *Context
  )
{
  EFI_EMMC_CARD_INFO_PROTOCOL       *EfiEmmcApplicationRegister;
  PCH_EMMC_TUNING_PROTOCOL          *PchEmmcTuningProtocol;
  EMMC_INFO                         EmmcInfo;
  EMMC_TUNING_DATA                  EmmcTuningData;
  UINT8                             PchEmmcTuningProtocolRevision;
  EFI_STATUS                        Status;

  PchEmmcTuningProtocolRevision = PCH_SCC_TUNING_PROTOCOL_REVISION;

  DEBUG ((DEBUG_INFO, "ConfigurePlatformEmmc Start()\n"));

  ///
  /// Closed the event to avoid call twice when launch shell
  ///
  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (
                  &gEfiEmmcCardInfoProtocolGuid,
                  NULL,
                  (VOID **) &EfiEmmcApplicationRegister
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
                  &gPchSccTuningProtocolGuid,
                  NULL,
                  (VOID **) &PchEmmcTuningProtocol
                  );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, " eMMC tuning aborted\n"));
    return;
  }
  //
  // Return if eMMC device does not support HS200
  //
  if (((EfiEmmcApplicationRegister->CardData->ExtCSDRegister.CARD_TYPE & B_EMMC_HS200_12V) == FALSE) &&
     ((EfiEmmcApplicationRegister->CardData->ExtCSDRegister.CARD_TYPE & B_EMMC_HS200_18V) == FALSE)) {
    DEBUG ((DEBUG_INFO, " eMMC device does not support HS200, exit!\n"));
    return;
  }

  //
  // Get EmmcInfo from EmmcCardInfoProtocol
  // EmmcCardInfoProtocol is produced in the driver
  //
  EmmcInfo.PartitionHandle = EfiEmmcApplicationRegister->CardData->Partitions->Handle;
  EmmcInfo.Lba = 0;
  EmmcInfo.RelativeDevAddress = (EfiEmmcApplicationRegister->CardData->Address << 16);

  if (EfiEmmcApplicationRegister->CardData->CurrentBusWidth == 4) {
    EmmcInfo.HS200BusWidth = V_EMMC_HS200_BUS_WIDTH_4;
  } else {
    EmmcInfo.HS200BusWidth = V_EMMC_HS200_BUS_WIDTH_8;
  }
  DEBUG ((DEBUG_INFO, " HS200BusWidth: %d\n", EmmcInfo.HS200BusWidth));

  Status = PchEmmcTuningProtocol->EmmcTune (PchEmmcTuningProtocol, PchEmmcTuningProtocolRevision, &EmmcInfo, &EmmcTuningData);

  DEBUG ((DEBUG_INFO, "ConfigurePlatformEmmc End()\n"));

}

/**
  Entry point for the driver.

  This routine gets info from the EFI_EMMC_CARD_INFO_PROTOCOL and perform
  Platform EMMC HS200/HS400 mode tuning.

  @param[in] ImageHandle              Image Handle.
  @param[in] SystemTable              EFI System Table.

**/
EFI_STATUS
EFIAPI
PlatformEmmcInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_EVENT                         EmmcApplicationRegisterEvent;
  VOID                              *EmmcApplicationRegisterEventRegistration;

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  (EFI_EVENT_NOTIFY) ConfigurePlatformEmmc,
                  NULL,
                  &EmmcApplicationRegisterEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->RegisterProtocolNotify (
                  &gEfiEmmcCardInfoProtocolGuid,
                  EmmcApplicationRegisterEvent,
                  &EmmcApplicationRegisterEventRegistration
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

