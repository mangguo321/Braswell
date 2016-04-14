/** @file
  This is the driver that initializes the Intel PCH.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchInit.h"
#ifndef FSP_FLAG
#include <Library/UefiDriverEntryPoint.h>
#endif
#include <IndustryStandard/Pci22.h>

//
// Global Variables
//
EFI_HANDLE  mImageHandle;

//
// Local function prototypes
//
EFI_STATUS
InitializePchDevice (
#ifndef FSP_FLAG
  IN OUT PCH_INSTANCE_PRIVATE_DATA           *PchInstance,
#else
  IN CONST EFI_PEI_SERVICES                  **PeiServices,
#endif
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
);

EFI_STATUS
EFIAPI
PchExitBootServicesEvent (
#ifndef FSP_FLAG
  IN EFI_EVENT    Event,
  IN VOID         *Context
#else
  IN  CONST EFI_PEI_SERVICES      **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
#endif
  );

EFI_STATUS
EFIAPI
PchInitBeforeBoot (
#ifndef FSP_FLAG
  IN EFI_EVENT    Event,
  IN VOID         *Context
#else
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
#endif
  );

/**
  Set an IOSF-SB extended programming S3 dispatch item, this function may assert if any error happened

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SetMsgBusAndThenOrEx32S3Item (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          AndData,
  IN     UINT32                          OrData,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           WriteOpCode,
  IN     UINT8                           Bar,
  IN     UINT8                           Device,
  IN     UINT8                           Function

  )
{
#ifdef EFI_S3_RESUME
  UINTN   PciExBaseAddress;
  UINT32  Buffer32;

  Buffer32         = 0;
  PciExBaseAddress = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);

  Buffer32 = (UINT32) (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF));
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress + MC_MCRXX),
    1,
    &Buffer32
    );
  Buffer32 = (UINT32) (Register & MSGBUS_MASKHI);
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress + MC_MCRX),
    1,
    &Buffer32
    );
  Buffer32 = (UINT32) ((ReadOpCode << 24) | (PortId << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN);
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress + MC_MCR),
    1,
    &Buffer32
    );
  S3BootScriptSaveMemReadWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress+ MC_MDR),
    &OrData,  // Data to be ORed
    &AndData  // Data to be ANDed
    );
  Buffer32 = (UINT32) (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF));
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress + MC_MCRXX),
    1,
    &Buffer32
    );
  Buffer32 = (UINT32) (Register & MSGBUS_MASKHI);
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress + MC_MCRX),
    1,
    &Buffer32
    );
  Buffer32 = (UINT32) ((WriteOpCode << 24) | (PortId << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN);
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (PciExBaseAddress + MC_MCR),
    1,
    &Buffer32
    );
#endif
  return EFI_SUCCESS;
}

/**
  Set an IOSF-SB programming S3 dispatch item, this function may assert if any error happened

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SetMsgBusAndThenOr32S3Item (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          AndData,
  IN     UINT32                          OrData,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           WriteOpCode
  )
{
  return (SetMsgBusAndThenOrEx32S3Item (PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode, 0x00, 0x00, 0x00));
}

/**
  Configures extended IOSF-SB and stores this configuration in S3 boot script

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

**/
VOID
PchMsgBusAndThenOrEx32WithScript (
    IN     UINT8                           PortId,
    IN     UINT32                          Register,
    IN     UINT32                          AndData,
    IN     UINT32                          OrData,
    IN     UINT8                           ReadOpCode,
    IN     UINT8                           WriteOpCode,
    IN     UINT8                           Bar,
    IN     UINT8                           Device,
    IN     UINT8                           Function
  )
{
  EFI_STATUS Status;

  Status = PchMsgBusAndThenOrEx32 (PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode, Bar, Device, Function);
  ASSERT_EFI_ERROR (Status);

  Status = PCH_INIT_COMMON_SCRIPT_MSG_BUS_AND_THEN_OR_EX_32_S3_ITEM (
             PortId,
             Register,
             AndData,
             OrData,
             ReadOpCode,
             WriteOpCode,
             Bar,
             Device,
             Function
             );
  ASSERT_EFI_ERROR (Status);
}

/**
  Configures IOSF-SB and stores this configuration in S3 boot script

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code

**/
VOID
PchMsgBusAndThenOr32WithScript (
    IN     UINT8                           PortId,
    IN     UINT32                          Register,
    IN     UINT32                          AndData,
    IN     UINT32                          OrData,
    IN     UINT8                           ReadOpCode,
    IN     UINT8                           WriteOpCode
  )
{
  PchMsgBusAndThenOrEx32WithScript (PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode, 0x00, 0x00, 0x00);
  return;
}

/**
  This is the standard EFI driver point that detects
  whether there is a south cluster in the system
  and if so, initializes the chip.

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchInitEntryPoint (
#ifndef FSP_FLAG
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
#else
  IN EFI_PEI_FILE_HANDLE       FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
#endif
  )
{
  EFI_STATUS                        Status;
  UINT8                             BusNumber;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;
  UINTN                             NumHandles;
  EFI_HANDLE                        *HandleBuffer;
  UINT32                            Index;
  PCH_INSTANCE_PRIVATE_DATA         *PchInstance;

  DEBUG ((EFI_D_INFO, "PchInitEntryPoint() Start\n"));

  PchInstance       = NULL;
  PchPlatformPolicy = NULL;

#ifndef FSP_FLAG
  mImageHandle = ImageHandle;

  //
  // Retrieve all instances of PCH Platform Policy protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDxePchPlatformPolicyProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
#else
  NumHandles = 1;
#endif

  for (Index = 0; Index < NumHandles; Index++) {
#ifndef FSP_FLAG
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gDxePchPlatformPolicyProtocolGuid,
                    (VOID **) &PchPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);

#else
    //
    // PCH Policy
    //
    Status = (**PeiServices).LocatePpi (
                               PeiServices,
                               &gDxePchPlatformPolicyProtocolGuid,
                               0,
                               NULL,
                               (VOID **) &PchPlatformPolicy
                               );
    ASSERT_EFI_ERROR (Status);
#endif
    BusNumber = PchPlatformPolicy->BusNumber;

    //
    // Dump whole DXE_PCH_PLATFORM_POLICY_PROTOCOL and serial out.
    //
    PchDumpPlatformProtocol (PchPlatformPolicy);

    //
    // Initialize the PCH device
    //
    InitializePchDevice (PchInstance, PchPlatformPolicy);
    PchInstance = AllocateZeroPool (sizeof (PCH_INSTANCE_PRIVATE_DATA));
    if (PchInstance == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    PchInstance->PchInfo.Revision   = PCH_INFO_PROTOCOL_REVISION_1;
    PchInstance->PchInfo.BusNumber  = BusNumber;
    PchInstance->PchInfo.RCVersion  = PCH_RC_VERSION;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &(HandleBuffer[Index]),
                    &gEfiPchInfoProtocolGuid,
                    &(PchInstance->PchInfo),
                    NULL
                    );
  }
  
  DEBUG ((EFI_D_INFO, "PchInitEntryPoint() End\n"));

  return EFI_SUCCESS;
}

/**
  Initialize the PCH device according to the PCH Platform Policy protocol

  @param[in] PchInstance          PCH instance private data. May get updated by this function
  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] PmcBase              PMC base address of this PCH device
  @param[in] IlbBase              iLB base address of this PCH device
  @param[in] SpiBase              SPI base address of this PCH device
  @param[in] MphyBase             MPHY base address of this PCH device
  @param[in] AcpiBase             ACPI IO base address of this PCH device
  @param[in] IoBase               CFIO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
InitializePchDevice (
#ifndef FSP_FLAG
  IN OUT PCH_INSTANCE_PRIVATE_DATA           *PchInstance,
#else
  IN CONST EFI_PEI_SERVICES      **PeiServices,
#endif
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  )
{
  EFI_STATUS  Status;
  
#ifndef FSP_FLAG
  EFI_EVENT   LegacyBootEvent;
  EFI_EVENT   ExitBootServicesEvent;
#endif

  //
  // Create events for PCH to do the task before ExitBootServices/LegacyBoot.
  // It is guaranteed that only one of two events below will be signalled
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  (EFI_EVENT_NOTIFY) PchExitBootServicesEvent,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventLegacyBootEx (
             TPL_CALLBACK,
             (EFI_EVENT_NOTIFY) PchExitBootServicesEvent,
             NULL,
             &LegacyBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  if (PchPlatformPolicy->SccConfig->eMMCEnabled != PchDisabled) {
    PchMsgBusAndThenOr32 (
      0x63,
      0x500,
      (UINT32)(~BIT7),
      0x00000000,
      0x06,
      0x07
    );
  }

  if (PchPlatformPolicy->SccConfig->SdioEnabled != PchDisabled) {
    PchMsgBusAndThenOr32 (
      0x63,
      0x508,
      (UINT32)(~BIT7),
      0x00000000,
      0x06,
      0x07
    );
  }

  if (PchPlatformPolicy->SccConfig->SdcardEnabled != PchDisabled) {
    PchMsgBusAndThenOr32 (
      0x63,
      0x504,
      (UINT32)(~BIT7),
      0x00000000,
      0x06,
      0x07
    );
  }

  DEBUG ((EFI_D_INFO, "InitializePchDevice() End\n"));

  return Status;
}

/**
  PCH initialization before ExitBootServices / LegacyBoot events
  Useful for operations which must happen later than at EndOfPost event

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

**/
EFI_STATUS
EFIAPI
PchExitBootServicesEvent (
#ifndef FSP_FLAG
  IN EFI_EVENT    Event,
  IN VOID         *Context
#else
  IN  CONST EFI_PEI_SERVICES      **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
#endif
  )
{
  EFI_STATUS                        Status;
  UINTN                             NumHandles;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;
  UINTN                             Index;
#ifndef FSP_FLAG
  EFI_HANDLE                        *HandleBuffer;

  //
  // Closed the event to avoid call twice
  //
  gBS->CloseEvent (Event);

  //
  // Retrieve all instances of PCH Platform Policy protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDxePchPlatformPolicyProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR | EFI_D_INFO, "Failed to locate handle buffer for PCH Policy protocol.\n"));
    return Status;
  }
#else
  NumHandles = 1;
#endif

  for (Index = 0; Index < NumHandles; Index++) {
#ifndef FSP_FLAG
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gDxePchPlatformPolicyProtocolGuid,
                    (VOID **) &PchPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR | EFI_D_INFO, "Failed to find PCH Policy protocol.\n"));
      return Status;
    }
#else
    //
    // PCH Policy
    //
    Status = (**PeiServices).LocatePpi (
                               PeiServices,
                               &gDxePchPlatformPolicyProtocolGuid,
                               0,
                               NULL,
                               (VOID **) &PchPlatformPolicy
                               );
    ASSERT_EFI_ERROR (Status);
#endif

    ConfigureLpeAtBoot (PchPlatformPolicy, FALSE);
    ConfigureOtgAtBoot (PchPlatformPolicy, FALSE);
    ConfigureLpssAtBoot (PchPlatformPolicy, FALSE);
    ConfigureSccAtBoot (PchPlatformPolicy, FALSE);
  }
  return EFI_SUCCESS;
}

