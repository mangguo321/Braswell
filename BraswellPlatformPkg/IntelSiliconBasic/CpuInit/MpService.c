/** @file
  Code which support multi-processor.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/

#include "CpuDxe.h"
#include "PlatformMpService.h"
#include "MiscFuncs.h"

#define MODULE_1_CPU0_APICID 4

/**
  @todo Add function description

  @param[in]  Event     @todo  Add argument description
  @param[in]  *Context  @todo  Add argument description

**/
VOID
EFIAPI
InitSmramDataContent (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  );

extern EFI_METRONOME_ARCH_PROTOCOL   *mMetronome;
extern MP_SYSTEM_DATA                *mMPSystemData;
extern UINTN                         mCommonFeatures;
extern volatile UINTN                mSwitchToLegacyRegionCount;
extern UINT32                        mCommonCStateValue;

static EFI_HANDLE                    mHandle         = NULL;
static volatile UINTN                mFinishedCount  = 0;

EFI_MP_SERVICES_PROTOCOL     mMpService = {
  GetNumberOfProcessors,
  GetProcessorInfo,
  StartupAllAPs,
  StartupThisAP,
  SwitchBSP,
  EnableDisableAP,
  WhoAmI
};

EFI_PHYSICAL_ADDRESS        mOriginalBuffer;
EFI_PHYSICAL_ADDRESS        mBackupBuffer;

/**
  Initialize the state information for the MP DXE Protocol.

**/
VOID
EFIAPI
MpServiceInitialize (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   LegacyBootEvent;
  EFI_EVENT   ExitBootServicesEvent;
  EFI_EVENT   ReAllocLegacyEvent;
  EFI_EVENT   ReAllocExitPmAuthEvent;
  VOID        *RegistrationLegacy;
  VOID        *RegistrationExitPmAuth;

  LegacyBootEvent = NULL;
  ExitBootServicesEvent = NULL;
  ReAllocLegacyEvent = NULL;
  ReAllocExitPmAuthEvent = NULL;
  RegistrationLegacy     = NULL;
  RegistrationExitPmAuth = NULL;

  //
  // Save Mtrr Registers in global data areas
  //
  ReadMtrrRegisters ();

  Status  = InitializeMpSystemData ();
  if (EFI_ERROR (Status)) {
    goto Done;
  }
#if defined EFI_S3_RESUME
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ReAllocateMemoryForAP,
                  NULL,
                  &ReAllocExitPmAuthEvent
                  );
  ASSERT_EFI_ERROR (Status);  
  Status = gBS->RegisterProtocolNotify (
                  &gExitPmAuthProtocolGuid,
                  ReAllocExitPmAuthEvent,
                  &RegistrationExitPmAuth
                  );
#else
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ReAllocateMemoryForAP,
                  NULL,
                  &ReAllocExitPmAuthEvent
                  );
  ASSERT_EFI_ERROR (Status);  
  Status = gBS->RegisterProtocolNotify (
                  &gExitPmAuthProtocolGuid,
                  ReAllocExitPmAuthEvent,
                  &RegistrationExitPmAuth
                  );
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ReAllocateMemoryForAP,
                  NULL,
                  &ReAllocLegacyEvent
                  );
  ASSERT_EFI_ERROR (Status);
#endif
  Status = EfiCreateEventLegacyBootEx (
             TPL_CALLBACK,
             (EFI_EVENT_NOTIFY)ResetAPs,
             NULL,
             &LegacyBootEvent
             );
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  (EFI_EVENT_NOTIFY)ResetAPs,
                  NULL,
                  &ExitBootServicesEvent
                  );
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  //
  // Now install the MP services protocol.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mHandle,
                  &gEfiMpServiceProtocolGuid,
                  &mMpService,
                  NULL
                  );

  if (EFI_ERROR (Status)) {

Done:
    if (LegacyBootEvent != NULL) {
      gBS->CloseEvent (LegacyBootEvent);
    }
    if (ExitBootServicesEvent != NULL) {
      gBS->CloseEvent (ExitBootServicesEvent);
    }
    gBS->FreePool (mMPSystemData);
  }
}

/**
  Implementation of GetNumberOfProcessors() service of MP Services Protocol.

  This service retrieves the number of logical processor in the platform
  and the number of those logical processors that are enabled on this boot.
  This service may only be called from the BSP.

  @param[in] This                         A pointer to the EFI_MP_SERVICES_PROTOCOL instance.
  @param[out] NumberOfProcessors          Pointer to the total number of logical processors in the system,
                                          including the BSP and disabled APs.
  @param[out] NumberOfEnabledProcessors   Pointer to the number of enabled logical processors that exist
                                          in system, including the BSP.

  @retval EFI_SUCCESS                     Number of logical processors and enabled logical processors retrieved..
  @retval EFI_DEVICE_ERROR                Caller processor is AP.
  @retval EFI_INVALID_PARAMETER           NumberOfProcessors is NULL.
  @retval EFI_INVALID_PARAMETER           NumberOfEnabledProcessors is NULL.

**/
EFI_STATUS
EFIAPI
GetNumberOfProcessors (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  OUT UINTN                   *NumberOfProcessors,
  OUT UINTN                   *NumberOfEnabledProcessors
  )
{
  UINTN          CallerNumber;
  UINTN          Index;
  CPU_DATA_BLOCK *CpuData;

  //
  // Check whether caller processor is BSP
  //
  WhoAmI (&mMpService, &CallerNumber);
  if (CallerNumber != mMPSystemData->BSP) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Check parameter NumberOfProcessors and NumberOfEnabledProcessors
  //
  if (NumberOfProcessors == NULL || NumberOfEnabledProcessors == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberOfProcessors        = mMPSystemData->NumberOfCpus;
  *NumberOfEnabledProcessors = 0;  
  for (Index = 0; Index < mMPSystemData->NumberOfCpus; Index++) {
    CpuData = &mMPSystemData->CpuData[Index];
    if (mMPSystemData->EnableSecondaryCpu) {
      if (CpuData->State != CPU_STATE_DISABLED) {
        (*NumberOfEnabledProcessors)++;
      }
    } else {
      if (CpuData->State != CPU_STATE_DISABLED && !mMPSystemData->CpuData[Index].SecondaryCpu) {
        (*NumberOfEnabledProcessors)++;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Implementation of GetProcessorInfo() service of MP Services Protocol.

  Gets detailed MP-related information on the requested processor at the
  instant this call is made. This service may only be called from the BSP.

  @param[in] This                   A pointer to the EFI_MP_SERVICES_PROTOCOL instance.
  @param[in] ProcessorNumber        The handle number of processor.
  @param[out] ProcessorInfoBuffer   A pointer to the buffer where information for the requested processor is deposited.

  @retval EFI_SUCCESS               Processor information successfully returned.
  @retval EFI_DEVICE_ERROR          Caller processor is AP.
  @retval EFI_INVALID_PARAMETER     ProcessorInfoBuffer is NULL
  @retval EFI_NOT_FOUND             Processor with the handle specified by ProcessorNumber does not exist.

**/
EFI_STATUS
EFIAPI
GetProcessorInfo (
  IN EFI_MP_SERVICES_PROTOCOL    *This,
  IN UINTN                       ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION  *ProcessorInfoBuffer
  )
{
  UINTN               CallerNumber;
  CPU_DATA_BLOCK      *CpuData;

  //
  // Check whether caller processor is BSP
  //
  WhoAmI (&mMpService, &CallerNumber);
  if (CallerNumber != mMPSystemData->BSP) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Check parameter ProcessorInfoBuffer
  //
  if (ProcessorInfoBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check whether processor with the handle specified by ProcessorNumber exists
  //
  if (ProcessorNumber >= mMPSystemData->NumberOfCpus) {
    return EFI_NOT_FOUND;
  }

  CpuData = &mMPSystemData->CpuData[ProcessorNumber];

  ProcessorInfoBuffer->ProcessorId = (UINT64) CpuData->ApicID;

  //
  // Get Status Flag of specified processor
  //
  ProcessorInfoBuffer->StatusFlag = 0;

  ProcessorInfoBuffer->StatusFlag |= PROCESSOR_ENABLED_BIT;
  if (!mMPSystemData->EnableSecondaryCpu) {
    if (CpuData->SecondaryCpu) {
      ProcessorInfoBuffer->StatusFlag &= ~PROCESSOR_ENABLED_BIT;
    }
  }

  if (ProcessorNumber == mMPSystemData->BSP) {
    ProcessorInfoBuffer->StatusFlag |= PROCESSOR_AS_BSP_BIT;
  }

  if (CpuData->Health == 0) {
    ProcessorInfoBuffer->StatusFlag |= PROCESSOR_HEALTH_STATUS_BIT;
  }

  ProcessorInfoBuffer->Location.Package = (UINT32) CpuData->PhysicalLocation.Package;
  ProcessorInfoBuffer->Location.Core    = (UINT32) CpuData->PhysicalLocation.Core;
  ProcessorInfoBuffer->Location.Thread  = (UINT32) CpuData->PhysicalLocation.Thread;

  return EFI_SUCCESS;
}

/**
  MP Service to get specified application processor (AP)
  to execute a caller-provided code stream.

  @param[in] This                  Pointer to MP Service Protocol
  @param[in] Procedure             The procedure to be assigned to AP.
  @param[in] CpuNumber             Number of the specified processor.
  @param[in] WaitEvent             If timeout, the event to be triggered after this AP finishes.
  @param[in] TimeoutInMicroSecs    The timeout value in microsecond. Zero means infinity.
  @param[in] ProcArguments         Argument for Procedure.
  @param[out] Finished             @todo Add argument description

  @retval EFI_INVALID_PARAMETER    Procudure is NULL.
  @retval EFI_INVALID_PARAMETER    Number of CPU out of range, or it belongs to BSP.
  @retval EFI_INVALID_PARAMETER    Specified CPU is not idle.
  @retval EFI_SUCCESS              The AP has finished.
  @retval EFI_TIMEOUT              Time goes out before the AP has finished.

**/
EFI_STATUS
EFIAPI
StartupThisAP (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN EFI_AP_PROCEDURE         Procedure,
  IN UINTN                    CpuNumber,
  IN EFI_EVENT WaitEvent      OPTIONAL,
  IN UINTN TimeoutInMicroSecs OPTIONAL,
  IN VOID *ProcArguments      OPTIONAL,
  OUT BOOLEAN *Finished       OPTIONAL
  )
{
  EFI_STATUS          Status;
  CPU_DATA_BLOCK      *CpuData;
  UINT64              ExpectedTime;

  //
  // Check for invalid CPU number
  //
  if ((CpuNumber >= mMPSystemData->NumberOfCpus) || (CpuNumber == mMPSystemData->BSP)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Procedure == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Finished != NULL) {
    *Finished = TRUE;
  }

  CpuData = &mMPSystemData->CpuData[CpuNumber];

  //
  // As a first step, check if processor is OK to start up code stream.
  //
  if (CpuData->State != CPU_STATE_IDLE) {
    return EFI_INVALID_PARAMETER;
  }

  ExpectedTime                = CalculateTimeout (TimeoutInMicroSecs);

  mMPSystemData->StartCount   = 1;
  mMPSystemData->FinishCount  = 0;

  WakeUpAp (
    CpuData,
    Procedure,
    ProcArguments
    );

  while (TRUE) {
    AsmAcquireMPLock (&CpuData->StateLock);
    if (CpuData->State == CPU_STATE_FINISHED) {
      CpuData->State = CPU_STATE_IDLE;
      AsmReleaseMPLock (&CpuData->StateLock);
      break;
    }

    AsmReleaseMPLock (&CpuData->StateLock);

    if (CheckTimeout (ExpectedTime)) {
      //
      // Save data into private data structure, and create timer to poll AP state before exiting
      //
      mMPSystemData->StartedCpuNumber = CpuNumber;
      mMPSystemData->WaitEvent        = WaitEvent;
      Status = gBS->SetTimer (
                      mMPSystemData->CheckThisAPEvent,
                      TimerPeriodic,
                      CPU_CHECK_AP_INTERVAL * MICROSECOND
                      );
      return EFI_TIMEOUT;
    }

    gBS->Stall (CPU_CHECK_AP_INTERVAL * MICROSECOND);
  }

  CpuData->Finished = Finished;

  return EFI_SUCCESS;
}

/**
  MP Service to get all the available application processors (APs)
  to execute a caller-provided code stream.

  @param[in] This                  Pointer to MP Service Protocol
  @param[in] Procedure             The procedure to be assigned to APs.
  @param[in] SingleThread          If true, all APs execute in block mode.
                                   Otherwise, all APs exceute in non-block mode.
  @param[in] WaitEvent             If timeout, the event to be triggered after all APs finish.
  @param[in] TimeoutInMicroSecs    The timeout value in microsecond. Zero means infinity.
  @param[in] ProcArguments         Argument for Procedure.
  @param[out] FailedCPUList        If not NULL, all APs that fail to start will be recorded in the list.

  @retval EFI_INVALID_PARAMETER    Procudure is NULL.
  @retval EFI_SUCCESS              Only 1 logical processor exists.
  @retval EFI_SUCCESS              All APs have finished.
  @retval EFI_TIMEOUT              Time goes out before all APs have finished.

**/
EFI_STATUS
EFIAPI
StartupAllAPs (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN EFI_AP_PROCEDURE         Procedure,
  IN BOOLEAN                  SingleThread,
  IN EFI_EVENT WaitEvent      OPTIONAL,
  IN UINTN                    TimeoutInMicroSecs,
  IN VOID *ProcArguments      OPTIONAL,
  OUT UINTN **FailedCPUList   OPTIONAL
  )
{
  EFI_STATUS     Status;
  CPU_DATA_BLOCK *CpuData;
  CPU_DATA_BLOCK *NextCpuData;
  UINTN          ListIndex;
  UINTN          CpuNumber;
  UINTN          NextCpuNumber;
  UINT64         ExpectedTime;
  CPU_STATE      APInitialState;
  CPU_STATE      CpuState;

  //
  // Check for valid procedure for APs
  //
  if (Procedure == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mMPSystemData->NumberOfCpus == 1) {
    return EFI_SUCCESS;
  }

  ExpectedTime                = CalculateTimeout (TimeoutInMicroSecs);

  ListIndex                   = 0;
  CpuData                     = NULL;

  mMPSystemData->FinishCount  = 0;
  mMPSystemData->StartCount   = 0;
  APInitialState              = CPU_STATE_READY;

  for (CpuNumber = 0; CpuNumber < mMPSystemData->NumberOfCpus; CpuNumber++) {
    CpuData = &mMPSystemData->CpuData[CpuNumber];

    //
    // Get APs prepared, and put failing APs into FailedCPUList
    // If "SingleThread", one AP will be put to ready state.
    // Once this AP finishes its task, the next AP is put to Ready state.
    // This process continues until all APs are put into Ready State
    // if not "SingleThread", all APs are put to ready state at the same time
    //
    if (CpuNumber != mMPSystemData->BSP) {
      if (CpuData->State == CPU_STATE_IDLE) {
        mMPSystemData->StartCount++;

        AsmAcquireMPLock (&CpuData->StateLock);
        CpuData->State = APInitialState;
        AsmReleaseMPLock (&CpuData->StateLock);

        if (SingleThread) {
          APInitialState = CPU_STATE_BLOCKED;
        }

      } else if (FailedCPUList != NULL) {
        *FailedCPUList[ListIndex] = CpuNumber;
        ListIndex++;
      }
    }
  }

  while (TRUE) {
    for (CpuNumber = 0; CpuNumber < mMPSystemData->NumberOfCpus; CpuNumber++) {
      CpuData = &mMPSystemData->CpuData[CpuNumber];
      if (CpuNumber == mMPSystemData->BSP) {
        continue;
      }
      CpuState = CpuData->State;
      switch (CpuState) {
        case CPU_STATE_READY:
          WakeUpAp (
            CpuData,
            Procedure,
            ProcArguments
            );
          break;

        case CPU_STATE_FINISHED:
          mMPSystemData->FinishCount++;
          if (SingleThread) {
            Status = GetNextBlockedCpuNumber (&NextCpuNumber);
            if (!EFI_ERROR (Status)) {
              NextCpuData = &mMPSystemData->CpuData[NextCpuNumber];
              AsmAcquireMPLock (&NextCpuData->StateLock);
              NextCpuData->State = CPU_STATE_READY;
              AsmReleaseMPLock (&NextCpuData->StateLock);
            }
          }
          AsmAcquireMPLock (&CpuData->StateLock);
          CpuData->State = CPU_STATE_IDLE;
          AsmReleaseMPLock (&CpuData->StateLock);
          break;

        default:
          break;
      }
    }

    if (mMPSystemData->FinishCount == mMPSystemData->StartCount) {
      return EFI_SUCCESS;
    }

    if (CheckTimeout (ExpectedTime)) {
      //
      // Save data into private data structure, and create timer to poll AP state before exiting
      //
      mMPSystemData->Procedure      = Procedure;
      mMPSystemData->ProcArguments  = ProcArguments;
      mMPSystemData->SingleThread   = SingleThread;
      mMPSystemData->WaitEvent      = WaitEvent;

      Status = gBS->SetTimer (
                      mMPSystemData->CheckAllAPsEvent,
                      TimerPeriodic,
                      CPU_CHECK_AP_INTERVAL * MICROSECOND
                      );
      return EFI_TIMEOUT;
    }

    gBS->Stall (CPU_CHECK_AP_INTERVAL * MICROSECOND);
  }

  return EFI_SUCCESS;
}

/**
  MP Service to makes the current BSP into an AP and then switches the
  designated AP into the AP. This procedure is usually called after a CPU
  test that has found that BSP is not healthy to continue it's responsbilities.

  @param[in] This                 Pointer to MP Service Protocol.
  @param[in] CpuNumber            The number of the specified AP.
  @param[in] EnableOldBSP         Whether to enable or disable the original BSP.

  @retval EFI_INVALID_PARAMETER   Number for Specified AP out of range.
  @retval EFI_INVALID_PARAMETER   Number of specified CPU belongs to BSP.
  @retval EFI_NOT_READY           Specified AP is not idle.
  @retval EFI_SUCCESS             BSP successfully switched.

**/
EFI_STATUS
EFIAPI
SwitchBSP (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN UINTN                    CpuNumber,
  IN BOOLEAN                  EnableOldBSP
  )
{
  EFI_STATUS            Status;
  EFI_CPU_ARCH_PROTOCOL *CpuArch;
  BOOLEAN               OldInterruptState;
  CPU_DATA_BLOCK        *CpuData;
  CPU_STATE             CpuState;

  //
  // Check if the specified CPU number is valid
  //
  if (CpuNumber >= mMPSystemData->NumberOfCpus) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if the specified CPU is already BSP
  //
  if (CpuNumber == mMPSystemData->BSP) {
    return EFI_INVALID_PARAMETER;
  }

  CpuData = &mMPSystemData->CpuData[CpuNumber];
  if (CpuData->State != CPU_STATE_IDLE) {
    return EFI_NOT_READY;
  }
  //
  // Before send both BSP and AP to a procedure to exchange their roles,
  // interrupt must be disabled. This is because during the exchange role
  // process, 2 CPU may use 1 stack. If interrupt happens, the stack will
  // be corrputed, since interrupt return address will be pushed to stack
  // by hardware.
  //
  CpuArch = mMPSystemData->CpuArch;
  (CpuArch->GetInterruptState)(CpuArch, &OldInterruptState);
  if (OldInterruptState) {
    Status = CpuArch->DisableInterrupt (CpuArch);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Unprogram virtual wire mode for the old BSP
  //
  ProgramVirtualWireMode (FALSE);
  SetApicBSPBit (FALSE);

  mMPSystemData->BSPInfo.State  = CPU_SWITCH_STATE_IDLE;
  mMPSystemData->BSPInfo.Lock   = VacantFlag;
  mMPSystemData->APInfo.State   = CPU_SWITCH_STATE_IDLE;
  mMPSystemData->APInfo.Lock    = VacantFlag;

  //
  // Need to wakeUp AP (future BSP)
  //
  WakeUpAp (
    CpuData,
    (EFI_AP_PROCEDURE)FutureBSPProc,
    mMPSystemData
    );

  AsmExchangeRole (&mMPSystemData->BSPInfo, &mMPSystemData->APInfo);

  //
  // The new BSP has come out. Since it carries the register value of the AP, need
  // to pay attention to variable which are stored in registers (due to optimization)
  //
  SetApicBSPBit (TRUE);
  ProgramVirtualWireMode (TRUE);

  if (OldInterruptState) {
    Status = CpuArch->EnableInterrupt (CpuArch);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  CpuData = &mMPSystemData->CpuData[mMPSystemData->BSP];
  while (TRUE) {
    AsmAcquireMPLock (&CpuData->StateLock);
    CpuState = CpuData->State;
    AsmReleaseMPLock (&CpuData->StateLock);

    if (CpuState == CPU_STATE_FINISHED) {
      break;
    }
  }

  Status              = ChangeCpuState (mMPSystemData->BSP, EnableOldBSP, EFI_CPU_CAUSE_NOT_DISABLED);
  mMPSystemData->BSP  = CpuNumber;

  return EFI_SUCCESS;
}

/**
  This procedure enables Or disables APs.

  @param[in] This                 Pointer to MP Service Protocol.
  @param[in] CpuNumber            The number of the specified AP.
  @param[in] NewAPState           Indicate new desired AP state
  @param[in] HealthState          If not NULL, it points to the value that specifies
                                  the new health status of the AP.  If it is NULL,
                                  this parameter is ignored.

  @retval EFI_INVALID_PARAMETER   Input paramters were not correct.
  @retval EFI_SUCCESS             Function completed successfully.

**/
EFI_STATUS
EFIAPI
EnableDisableAP (
  IN EFI_MP_SERVICES_PROTOCOL   *This,
  IN UINTN                      CpuNumber,
  IN BOOLEAN                    NewAPState,
  IN UINT32                     *HealthState OPTIONAL
  )
{
  EFI_STATUS     Status;
  CPU_DATA_BLOCK *CpuData;

  //
  // Check for valid input parameters.
  //
  if (CpuNumber >= mMPSystemData->NumberOfCpus || CpuNumber == mMPSystemData->BSP) {
    return EFI_INVALID_PARAMETER;
  }

  CpuData = &mMPSystemData->CpuData[CpuNumber];
  Status  = ChangeCpuState (CpuNumber, NewAPState, EFI_CPU_CAUSE_USER_SELECTION);

  if (HealthState != NULL) {
    CopyMem (&CpuData->Health, HealthState, sizeof (UINT32));
  }

  return EFI_SUCCESS;
}

/**
  This procedure returns the calling CPU handle.

  @param[in] This                 Pointer to MP Service Protocol.
  @param[out] CpuNumber           The number of the specified AP.

  @retval EFI_SUCCESS             Function completed successfully.

**/
EFI_STATUS
EFIAPI
WhoAmI (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  OUT UINTN                   *CpuNumber
  )
{
  UINTN ApicID;
  UINTN NumOfCpus;
  UINTN Index;

  ApicID    = GetApicID (NULL, NULL);

  NumOfCpus = mMPSystemData->NumberOfCpus;

  for (Index = 0; Index < NumOfCpus; Index++) {
    if (ApicID == mMPSystemData->CpuData[Index].ApicID) {
      break;
    }
  }

  *CpuNumber = Index;

  return EFI_SUCCESS;
}

/**
  Searches the HOB list provided by the core to find
  if a MP guided HOB list exists or not. If it does, it copies it to the driver
  data area, else returns 0

  @param[in]  MPSystemData       Pointer to an MP_SYSTEM_DATA structure

  @retval  EFI_SUCCESS           Success
  @retval  Others                HOB not found or else

**/
EFI_STATUS
GetMpBistStatus (
  IN     MP_SYSTEM_DATA  *MPSystemData
  )
{
  EFI_STATUS              Status;
  VOID                    *DataInHob;
  UINTN                   DataSize;
  EFI_PEI_HOB_POINTERS    GuidHob;

  //
  // Get Hob list
  //
  DataInHob = NULL;
  DataSize = 0;
  GuidHob.Raw = GetHobList ();

  if (GuidHob.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "No HOBs found\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Check for MP Data Hob.
  //
  GuidHob.Raw = GetNextGuidHob (&gEfiHtBistHobGuid, GuidHob.Raw);
  if (GuidHob.Raw != NULL) {
    DataInHob = GET_GUID_HOB_DATA (GuidHob.Guid);
    DataSize = GET_GUID_HOB_DATA_SIZE(GuidHob.Guid);
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_NOT_FOUND;
  }

  //
  // This is the MP HOB. So, copy all the data.
  //
  if (!(EFI_ERROR (Status))) {
    if(NULL == MPSystemData->BistHobData){
      gBS->AllocatePool(
             EfiACPIMemoryNVS,
             DataSize,
             (VOID **)&MPSystemData->BistHobData
             );
    }
    gBS->CopyMem (MPSystemData->BistHobData, DataInHob, DataSize);
    MPSystemData->BistHobSize = DataSize;
  }

  return Status;
}

/**
  Allocate data pool for MP information and fill data in it.

  @param[out]  WakeUpBuffer               The address of wakeup buffer.
  @param[out]  StackAddressStart          The start address of APs's stacks.
  @param[in]   MaximumCPUsForThisSystem   Maximum CPUs in this system.

  @retval      EFI_SUCCESS                Function successfully executed.
  @retval      Other                      Error occurred while allocating memory.

**/
EFI_STATUS
AllocateMemoryAndFillData (
  OUT EFI_PHYSICAL_ADDRESS           WakeUpBuffer,
  OUT VOID                           *StackAddressStart,
  IN  UINTN                          MaximumCPUsForThisSystem
  )
{
  EFI_STATUS                         Status;

  //
  // First check if the MP data structures and AP rendezvous routine have been
  // supplied by the PEIMs that executed in early boot stage.
  //

  //
  // Clear the data structure area first.
  //
  gBS->SetMem (mMPSystemData, sizeof (MP_SYSTEM_DATA), 0);

  Status  = GetMpBistStatus (mMPSystemData);

  mAcpiCpuData->CpuPrivateData = (EFI_PHYSICAL_ADDRESS)(UINTN)(&(mMPSystemData->S3DataPointer));
  mAcpiCpuData->APState       = 1;
  mAcpiCpuData->WakeUpBuffer  = WakeUpBuffer;
  mAcpiCpuData->StackAddress  = (EFI_PHYSICAL_ADDRESS)(UINTN)(StackAddressStart);

  Status = PrepareGdtIdtForAP ((IA32_DESCRIPTOR *) (UINTN) mAcpiCpuData->GdtrProfile, (IA32_DESCRIPTOR *) (UINTN) mAcpiCpuData->IdtrProfile);

  //
  // First BSP fills and inits all known values, including it's own records.
  //
  mMPSystemData->APSerializeLock     = VacantFlag;
  mMPSystemData->NumberOfCpus        = 1;
  mMPSystemData->EnableSecondaryCpu  = 1;

  //
  // Record these CPU configuration data (both for normal boot and for S3 use)
  //
  mMPSystemData->CpuArch = NULL;
  gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **) &mMPSystemData->CpuArch);

  mMPSystemData->MaximumCpusForThisSystem = MaximumCPUsForThisSystem;

  mMPSystemData->BSP = 0;

  FillInProcessorInformation (mMPSystemData, TRUE, 0);

  return EFI_SUCCESS;
}

/**
  Wake up APs for the first time to count their number and collect BIST data.

  @param[in]  WakeUpBuffer        Address of the wakeup buffer.

  @retval  EFI_SUCCESS            Function successfully finishes.

**/
EFI_STATUS
CountApNumberAndCollectBist (
  IN EFI_PHYSICAL_ADDRESS     WakeUpBuffer
  )
{
  MP_CPU_EXCHANGE_INFO    *ExchangeInfo;
  UINTN                   Index;

  //
  // Send INIT IPI - SIPI to all APs
  //
  SendInterrupt (BROADCAST_MODE_ALL_EXCLUDING_SELF, 0, 0, DELIVERY_MODE_INIT, TRIGGER_MODE_EDGE, TRUE);
  SendInterrupt (
    BROADCAST_MODE_ALL_EXCLUDING_SELF,
    0,
    (UINT32) RShiftU64 (WakeUpBuffer, 12),
    DELIVERY_MODE_SIPI,
    TRIGGER_MODE_EDGE,
    TRUE
    );

  //
  // Wait for task to complete and then exit.
  //
  gBS->Stall (200 * MICROSECOND);
  
  ExchangeInfo = (MP_CPU_EXCHANGE_INFO *) (UINTN) (WakeUpBuffer + MP_CPU_EXCHANGE_INFO_OFFSET);

  for (Index = 0; Index < MAXIMUM_CPU_NUMBER; Index++) {
    if (ExchangeInfo->BistBuffer[Index].Number == 1) {
      ExchangeInfo->BistBuffer[Index].Number = (UINT32) mMPSystemData->NumberOfCpus++;
    }
  }
  mAcpiCpuData->NumberOfCpus  = (UINT32) mMPSystemData->NumberOfCpus;

  ExchangeInfo->InitFlag = 0;

  return EFI_SUCCESS;
}

/**
  Wake up APs for the second time to collect detailed information.

  @param[in]  WakeUpBuffer     Address of the wakeup buffer.

  @retval  EFI_SUCCESS         Function successfully finishes.

**/
EFI_STATUS
PollForInitialization (
  IN EFI_PHYSICAL_ADDRESS     WakeUpBuffer
  )
{
  MP_CPU_EXCHANGE_INFO    *ExchangeInfo;

  ExchangeInfo             = (MP_CPU_EXCHANGE_INFO *) (UINTN) (WakeUpBuffer + MP_CPU_EXCHANGE_INFO_OFFSET);
  ExchangeInfo->ApFunction = (VOID *) (UINTN) DetailedInitialization;

  SendInterrupt (BROADCAST_MODE_ALL_EXCLUDING_SELF, 0, 0, DELIVERY_MODE_INIT, TRIGGER_MODE_EDGE, TRUE);
  SendInterrupt (
    BROADCAST_MODE_ALL_EXCLUDING_SELF,
    0,
    (UINT32) RShiftU64 (WakeUpBuffer, 12),
    DELIVERY_MODE_SIPI,
    TRIGGER_MODE_EDGE,
    TRUE
    );

  //
  // Wait until all APs finish
  //
  while (mFinishedCount < mAcpiCpuData->NumberOfCpus - 1) {
    CpuPause ();
  }

  return EFI_SUCCESS;
}

/**
  @todo	Add function description

  @param[in]    Location          @todo	Add argument description

  @retval       EFI_SUCCESS       @todo	Add argument description

**/
EFI_STATUS
FillInCpuLocation (
  IN EFI_CPU_PHYSICAL_LOCATION   *Location
  )
{
  UINT32                ApicId;
  UINT32                LevelType;
  UINT32                LevelBits;
  UINT32                RegEax;
  UINT32                RegEbx;
  UINT32                RegEcx;
  UINT8                 Shift;
  UINT8                 Bits;
  UINT32                Mask;
  BOOLEAN               HyperThreadingEnabled;

  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
  ApicId = (RegEbx >> 24);

  AsmCpuid (EFI_CPUID_SIGNATURE, &RegEax, NULL, NULL, NULL);
  if (RegEax >= EFI_CPUID_CORE_TOPOLOGY) {
    LevelBits = 0;
    LevelType = 0;
    do {
      AsmCpuidEx (EFI_CPUID_CORE_TOPOLOGY, LevelType, &RegEax, &RegEbx, &RegEcx, NULL);
      LevelType = ((RegEcx >> 8) & 0xFF);
      switch (LevelType) {
        case 1:  //Thread
          Location->Thread    = ApicId  & ((1 << (RegEax & 0x0F)) - 1);
          Location->Thread  >>= LevelBits;
          LevelBits           = RegEax & 0x0F;
          break;
        case 2:  //Core
          Location->Core      = ApicId  & ((1 << (RegEax & 0x0F)) - 1);
          Location->Core    >>= LevelBits;
          LevelBits           = RegEax & 0x0F;
          break;
        default: //End of Level
          Location->Package   = ApicId >> LevelBits;
          break;
      }
    } while (!(RegEax == 0 && RegEbx == 0));
  } else {

    AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
    Bits  = 0;
    Shift = (UINT8)((RegEbx >> 16) & 0xFF);

    Mask  =  Shift - 1;
    while (Shift > 1) {
      Shift >>= 1;
      Bits++;
    }

    HyperThreadingEnabled = FALSE;
    AsmCpuidEx (EFI_CPUID_CACHE_PARAMS, 0, &RegEax, NULL, NULL, NULL);
    if (Mask > (RegEax >> 26)) {
      HyperThreadingEnabled = TRUE;
    }

    Location->Package = (ApicId >> Bits);
    if (HyperThreadingEnabled) {
      Location->Core    = (ApicId & Mask) >> 1;
      Location->Thread  = (ApicId & Mask) & 1;
    } else {
      Location->Core    = (ApicId & Mask);
      Location->Thread  = 0;
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialize multiple processors and collect MP related data

  @retval  EFI_SUCCESS             Multiple processors get initialized and data collected successfully

  @retval  Other                   The operation failed due to some reason

**/
EFI_STATUS
InitializeMpSystemData (
  VOID
  )
{
  EFI_STATUS              Status;
  UINT32                  MaxThreadsPerCore;
  UINT32                  MaxCoresPerDie;
  UINT32                  MaxDiesPerPackage;
  UINT32                  MaxPackages;
  VOID                    *StackAddressStart;
  EFI_PHYSICAL_ADDRESS    WakeUpBuffer;
  MP_CPU_EXCHANGE_INFO    *ExchangeInfo;
  UINTN                   Index;
  EFI_CPU_ARCH_PROTOCOL   *CpuArch;
  BOOLEAN                 mInterruptState;
  CPU_DATA_BLOCK          *CpuData;
  UINTN                   MaximumCPUsForThisSystem;

  ProgramVirtualWireMode (TRUE);
  MaxThreadsPerCore = PcdGet32 (CpuNumberOfThreadsPerCore);
  MaxCoresPerDie = PcdGet32(CpuNumberOfCoresPerDie);
  MaxDiesPerPackage = PcdGet32(CpuNumberOfDiesPerPackage);
  MaxPackages = PcdGet32(CpuNumberOfPackages);

  MaximumCPUsForThisSystem = MaxThreadsPerCore * MaxCoresPerDie * MaxDiesPerPackage * MaxPackages;

  Status = PrepareMemoryForAPs (
             &WakeUpBuffer,
             &StackAddressStart,
             MaximumCPUsForThisSystem
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mOriginalBuffer             = WakeUpBuffer;

  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiBootServicesData,
                  1,
                  &mBackupBuffer
                  );

  Status = AllocateMemoryAndFillData (
             WakeUpBuffer,
             StackAddressStart,
             MaximumCPUsForThisSystem
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ExchangeInfo = (MP_CPU_EXCHANGE_INFO *) (UINTN) (WakeUpBuffer + MP_CPU_EXCHANGE_INFO_OFFSET);
  Status = PrepareExchangeInfo (
             ExchangeInfo,
             StackAddressStart,
             NULL,
             WakeUpBuffer
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpuArch = mMPSystemData->CpuArch;
  CpuArch->GetInterruptState (CpuArch, &mInterruptState);
  CpuArch->DisableInterrupt (CpuArch);

  //
  // For B stepping and above use broadcast
  //
  CountApNumberAndCollectBist (WakeUpBuffer);
  ExchangeInfo->WakeUpApManner = WakeUpApCounterInit;
  PollForInitialization (WakeUpBuffer);

  ExchangeInfo->WakeUpApManner = (WAKEUP_AP_MANNER) WakeUpApPerHltLoop;
  ExchangeInfo->ApFunction = (VOID *) (UINTN) ApProcWrapper;
  if (mInterruptState) {
    CpuArch->EnableInterrupt (CpuArch);
  }

  for (Index = 1; Index < mMPSystemData->NumberOfCpus; Index++) {
    CpuData = &mMPSystemData->CpuData[Index];
    if (CpuData->Health != 0) {
      REPORT_STATUS_CODE_EX (
        EFI_ERROR_MAJOR | EFI_ERROR_CODE,
        EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_SELF_TEST,
        (UINT32) Index,
        &gEfiCallerIdGuid,
        NULL,
        NULL,
        0
        );
    }

  }

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  (EFI_EVENT_NOTIFY)CheckAllAPsStatus,
                  NULL,
                  &mMPSystemData->CheckAllAPsEvent
                  );
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  (EFI_EVENT_NOTIFY)CheckThisAPStatus,
                  NULL,
                  &mMPSystemData->CheckThisAPEvent
                  );

  SaveBspMtrrForS3 ();
  CopyMem ((VOID *) (UINTN) mBackupBuffer, (VOID *) (UINTN) mOriginalBuffer, EFI_PAGE_SIZE);

  return EFI_SUCCESS;
}

/**
  Wrapper function for all procedures assigned to AP via MP service protocol.
  It controls states of AP and invokes assigned precedure.

**/
VOID
ApProcWrapper (
  VOID
  )
{
  EFI_AP_PROCEDURE      Procedure;
  VOID                  *Parameter;
  UINTN                 CpuNumber;
  CPU_DATA_BLOCK        *CpuData;

  WhoAmI (&mMpService, &CpuNumber);
  CpuData = &mMPSystemData->CpuData[CpuNumber];

  //
  // Now let us check it out.
  //
  AsmAcquireMPLock (&CpuData->ProcedureLock);
  Procedure = CpuData->Procedure;
  Parameter = CpuData->Parameter;
  AsmReleaseMPLock (&CpuData->ProcedureLock);

  if (Procedure != NULL) {
    AsmAcquireMPLock (&CpuData->StateLock);
    CpuData->State = CPU_STATE_BUSY;
    AsmReleaseMPLock (&CpuData->StateLock);

    Procedure (Parameter);

    //
    // if BSP is switched to AP, it continue execute from here, but it carries register state
    // of the old AP, so need to reload CpuData (might be stored in a register after compiler
    // optimization) to make sure it points to the right data
    //
    WhoAmI (&mMpService, &CpuNumber);
    CpuData = &mMPSystemData->CpuData[CpuNumber];

    AsmAcquireMPLock (&CpuData->ProcedureLock);
    CpuData->Procedure = NULL;
    AsmReleaseMPLock (&CpuData->ProcedureLock);

    AsmAcquireMPLock (&CpuData->StateLock);
    CpuData->State = CPU_STATE_FINISHED;
    AsmReleaseMPLock (&CpuData->StateLock);
  }
}

/**
  Procedure for detailed initialization of APs. It will be assigned to all APs while
  they are waken up for the second time.

**/
VOID
DetailedInitialization (
  VOID
  )
{
  EFI_STATUS            Status;
  UINT32                FailedRevision;

  CpuInitFloatPointUnit ();

  AsmAcquireMPLock (&mMPSystemData->APSerializeLock);

  Status = InitializeMicrocode (
             (EFI_CPU_MICROCODE_HEADER **) (UINTN) mAcpiCpuData->MicrocodePointerBuffer,
             &FailedRevision,
             FALSE
             );

  //
  // Save Mtrr Registers in global data areas
  //
  MpMtrrSynchUp (NULL);

  ProgramVirtualWireMode (FALSE);

  FillInProcessorInformation (mMPSystemData, FALSE, 0);

  mFinishedCount++;

  AsmReleaseMPLock (&mMPSystemData->APSerializeLock);
}

/**
  @todo Add function description

  @param[in]  MPSystemData  -   @todo add argument description

  @retval                   -   @todo add return values

**/
VOID
FutureBSPProc (
  IN     MP_SYSTEM_DATA  *MPSystemData
  )
{
  AsmExchangeRole (&MPSystemData->APInfo, &MPSystemData->BSPInfo);
  return ;
}

/**
  This function is called by all processors (both BSP and AP) once and collects MP related data

  @param[in]  MPSystemData    Pointer to the data structure containing MP related data
  @param[in]  BSP             TRUE if the CPU is BSP
  @param[in]  BistParam       BIST (build-in self test) data for the processor. This data
                              is only valid for processors that are waked up for the 1st
                              time in this CPU DXE driver.

  @retval     EFI_SUCCESS     Data for the processor collected and filled in

**/
EFI_STATUS
FillInProcessorInformation (
  IN     MP_SYSTEM_DATA       *MPSystemData,
  IN     BOOLEAN              BSP,
  IN     UINT32               BistParam
  )
{
  UINT32               Health;
  UINT32               ApicID;
  CPU_DATA_BLOCK       *CpuData;
  UINT32               BIST;
  UINTN                CpuNumber;
  UINTN                Index;
  UINTN                Count;
  MP_CPU_EXCHANGE_INFO *ExchangeInfo;

  ApicID = GetApicID (NULL, NULL);
  BIST   = 0;

  if (BSP) {
    CpuNumber = 0;
    BIST      = BistParam;
  } else {
    ExchangeInfo = (MP_CPU_EXCHANGE_INFO *) (UINTN) (mAcpiCpuData->WakeUpBuffer + MP_CPU_EXCHANGE_INFO_OFFSET);
    CpuNumber    = ExchangeInfo->BistBuffer[ApicID].Number;
    BIST         = ExchangeInfo->BistBuffer[ApicID].BIST;
  }

  CpuData                 = &MPSystemData->CpuData[CpuNumber];
  CpuData->SecondaryCpu   = IsSecondaryThread ();
  CpuData->ApicID         = ApicID;
  CpuData->Procedure      = NULL;
  CpuData->Parameter      = NULL;
  CpuData->StateLock      = VacantFlag;
  CpuData->ProcedureLock  = VacantFlag;
  CpuData->State          = CPU_STATE_IDLE;

  Health = BIST;
  Count  = MPSystemData->BistHobSize / sizeof(BIST_HOB_DATA);
  for (Index = 0; Index < Count; Index++) {
    if (ApicID ==  MPSystemData->BistHobData[Index].ApicId) {
      Health = MPSystemData->BistHobData[Index].Health;
    }
  }

  if (Health > 0) {
    CpuData->State                        = CPU_STATE_DISABLED;
    MPSystemData->DisableCause[CpuNumber] = EFI_CPU_CAUSE_SELFTEST_FAILURE;
  } else {
    MPSystemData->DisableCause[CpuNumber] = EFI_CPU_CAUSE_NOT_DISABLED;
  }

  //
  // Get Core and Thread number
  //
  CpuData->NumberOfCores = PcdGet32(CpuNumberOfCoresPerDie);

  CpuData->NumberOfThreads = PcdGet32(CpuNumberOfThreadsPerCore);

  FillInCpuLocation (&CpuData->PhysicalLocation);

  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @param[in]  Enable   -   @todo add argument description

  @retval  EFI_SUCCESS -   @todo Add description for return value

**/
EFI_STATUS
SetApicBSPBit (
  IN  BOOLEAN   Enable
  )
{
  UINT64  ApicBaseReg;

  ApicBaseReg = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);

  if (Enable) {
    ApicBaseReg |= B_EFI_MSR_IA32_APIC_BASE_BSP;
  } else {
    ApicBaseReg &= (~(UINT64)(B_EFI_MSR_IA32_APIC_BASE_BSP | 0xFF));
  }

  AsmWriteMsr64 (EFI_MSR_IA32_APIC_BASE, ApicBaseReg);

  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @param[in]  CpuNumber -   @todo add argument description
  @param[in]  NewState  -   @todo add argument description
  @param[in]  Cause     -   @todo add argument description

  @retval  EFI_SUCCESS  -   @todo Add description for return value

**/
EFI_STATUS
ChangeCpuState (
  IN     UINTN                      CpuNumber,
  IN     BOOLEAN                    NewState,
  IN     EFI_CPU_STATE_CHANGE_CAUSE Cause
  )
{
  CPU_DATA_BLOCK                              *CpuData;
  EFI_COMPUTING_UNIT_CPU_DISABLED_ERROR_DATA  ErrorData;

  CpuData = &mMPSystemData->CpuData[CpuNumber];

  mMPSystemData->DisableCause[CpuNumber] = Cause;

  if (!NewState) {
    AsmAcquireMPLock (&CpuData->StateLock);
    CpuData->State = CPU_STATE_DISABLED;
    AsmReleaseMPLock (&CpuData->StateLock);

    ErrorData.DataHeader.HeaderSize = sizeof (EFI_STATUS_CODE_DATA);
    ErrorData.DataHeader.Size       = sizeof (EFI_COMPUTING_UNIT_CPU_DISABLED_ERROR_DATA) - sizeof (EFI_STATUS_CODE_DATA);
    CopyMem (
      &ErrorData.DataHeader.Type,
      &gEfiStatusCodeSpecificDataGuid,
      sizeof (EFI_GUID)
      );
    ErrorData.Cause             = Cause;
    ErrorData.SoftwareDisabled  = TRUE;
    REPORT_STATUS_CODE_EX (
      EFI_ERROR_MINOR | EFI_ERROR_CODE,
      EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_EC_DISABLED,
      (UINT32) CpuNumber,
      &gEfiCallerIdGuid,
      NULL,
      (EFI_STATUS_CODE_DATA *) &ErrorData,
      sizeof(EFI_COMPUTING_UNIT_CPU_DISABLED_ERROR_DATA)
      );
  } else {
    AsmAcquireMPLock (&CpuData->StateLock);
    CpuData->State = CPU_STATE_IDLE;
    AsmReleaseMPLock (&CpuData->StateLock);
  }

  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @retval    @todo add return values

**/
BOOLEAN
IsSecondaryThread (
  VOID
  )
{
  UINT32              ApicID;
  UINT8               CpuPerCore;
  UINT32              Mask;

  ApicID = GetApicID (NULL, NULL);

  CpuPerCore = (UINT8)PcdGet32(CpuNumberOfThreadsPerCore);
  if (CpuPerCore == 1) {
    return FALSE;
  }

  //
  // Assume 1 Core has no more than 8 threads
  //
  if (CpuPerCore == 2) {
    Mask = 0x1;
  } else if (CpuPerCore <= 4) {
    Mask = 0x3;
  } else {
    Mask = 0x7;
  }

  if (ApicID & Mask) {
    return TRUE;
  }

  return FALSE;
}

/**
  If timeout occurs in StartupAllAps(), a timer is set, which invokes this
  procedure periodically to check whether all APs have finished.

  @param[in]  Event     Event triggered.
  @param[in]  Context   Parameter passed with the event.

**/
VOID
CheckAllAPsStatus (
  IN  EFI_EVENT                           Event,
  IN  VOID                                *Context
  )
{
  UINTN           CpuNumber;
  UINTN           NextCpuNumber;
  CPU_DATA_BLOCK  *CpuData;
  CPU_DATA_BLOCK  *NextCpuData;
  EFI_STATUS      Status;
  CPU_STATE       CpuState;

  for (CpuNumber = 0; CpuNumber < mMPSystemData->NumberOfCpus; CpuNumber++) {
    CpuData = &mMPSystemData->CpuData[CpuNumber];
    if (CpuNumber == mMPSystemData->BSP) {
      continue;
    }

    AsmAcquireMPLock (&CpuData->StateLock);
    CpuState = CpuData->State;
    AsmReleaseMPLock (&CpuData->StateLock);

    switch (CpuState) {
      case CPU_STATE_READY:
        WakeUpAp (
          CpuData,
          mMPSystemData->Procedure,
          mMPSystemData->ProcArguments
          );
        break;

      case CPU_STATE_FINISHED:
        if (mMPSystemData->SingleThread) {
          Status = GetNextBlockedCpuNumber (&NextCpuNumber);
          if (!EFI_ERROR (Status)) {
            NextCpuData = &mMPSystemData->CpuData[NextCpuNumber];

            AsmAcquireMPLock (&NextCpuData->ProcedureLock);
            NextCpuData->State = CPU_STATE_READY;
            AsmReleaseMPLock (&NextCpuData->ProcedureLock);

            WakeUpAp (
              NextCpuData,
              mMPSystemData->Procedure,
              mMPSystemData->ProcArguments
              );
          }
        }

        CpuData->State = CPU_STATE_IDLE;
        mMPSystemData->FinishCount++;
        break;

      default:
        break;
    }
  }

  if (mMPSystemData->FinishCount == mMPSystemData->StartCount) {
    gBS->SetTimer (
           mMPSystemData->CheckAllAPsEvent,
           TimerCancel,
           0
           );
    Status = gBS->SignalEvent (mMPSystemData->WaitEvent);
  }

  return ;
}

/**
  @todo Add function description

  @param[in]  Event    -   @todo add argument description
  @param[in]  *Context -   @todo add argument description

**/
VOID
CheckThisAPStatus (
  IN  EFI_EVENT            Event,
  IN  VOID                 *Context
  )
{
  CPU_DATA_BLOCK  *CpuData;
  EFI_STATUS      Status;
  CPU_STATE       CpuState;

  CpuData = &mMPSystemData->CpuData[mMPSystemData->StartedCpuNumber];

  AsmAcquireMPLock (&CpuData->StateLock);
  CpuState = CpuData->State;
  AsmReleaseMPLock (&CpuData->StateLock);

  if (CpuState == CPU_STATE_FINISHED) {
    gBS->SetTimer (
          mMPSystemData->CheckThisAPEvent,
          TimerCancel,
          0
          );
    Status = gBS->SignalEvent (mMPSystemData->WaitEvent);
    AsmAcquireMPLock (&CpuData->StateLock);
    CpuData->State = CPU_STATE_IDLE;
    AsmReleaseMPLock (&CpuData->StateLock);
  }

  return ;
}

/**
  @todo Add function description

  @param[in]        TimeoutInMicroSecs  -   @todo add argument description

  @retval                               -   @todo add return values

**/
UINT64
CalculateTimeout (
  IN  UINTN                               TimeoutInMicroSecs
  )
{
  UINT64      CurrentTsc;
  UINT64      ExpectedTsc;
  UINT64      Frequency;
  EFI_STATUS  Status;

  if (TimeoutInMicroSecs == 0) {
    return 0xffffffffffff;
  }

  CurrentTsc  = AsmReadTsc ();

  Status      = GetActualFrequency (mMetronome, &Frequency);

  ExpectedTsc = CurrentTsc + MultU64x32 (Frequency, (UINT32)TimeoutInMicroSecs);

  return ExpectedTsc;
}

/**
  @todo Add function description

  @param[in]  ExpectedTsc -   @todo add argument description

  @retval                 -   @todo add return values

**/
BOOLEAN
CheckTimeout (
  IN  UINT64                              ExpectedTsc
  )
{
  UINT64  CurrentTsc;

  CurrentTsc = AsmReadTsc ();
  if (CurrentTsc >= ExpectedTsc) {
    return TRUE;
  }

  return FALSE;
}

/**
  @todo Add function description

  @param[out]  NextCpuNumber -   @todo add argument description

  @retval  EFI_SUCCESS       -   @todo Add description for return value
  @retval  EFI_NOT_FOUND     -   @todo Add description for return value

**/
EFI_STATUS
GetNextBlockedCpuNumber (
  OUT UINTN                               *NextCpuNumber
  )
{
  UINTN           CpuNumber;
  CPU_STATE       CpuState;
  CPU_DATA_BLOCK  *CpuData;

  for (CpuNumber = 0; CpuNumber < mMPSystemData->NumberOfCpus; CpuNumber++) {
    if (CpuNumber == mMPSystemData->BSP) {
      continue;
    }

    CpuData = &mMPSystemData->CpuData[CpuNumber];

    AsmAcquireMPLock (&CpuData->StateLock);
    CpuState = CpuData->State;
    AsmReleaseMPLock (&CpuData->StateLock);

    if (CpuState == CPU_STATE_BLOCKED) {
      *NextCpuNumber = CpuNumber;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Function to wake up a specified AP and assign procedure to it.

  @param[in]  CpuData         CPU data block for the specified AP.
  @param[in]  Procedure       Procedure to assign.
  @param[in]  ProcArguments   Argument for Procedure.

**/
VOID
WakeUpAp (
  IN   CPU_DATA_BLOCK        *CpuData,
  IN   EFI_AP_PROCEDURE      Procedure,
  IN   VOID                  *ProcArguments
  )
{
  AsmAcquireMPLock (&CpuData->ProcedureLock);
  CpuData->Parameter  = ProcArguments;
  CpuData->Procedure  = Procedure;
  AsmReleaseMPLock (&CpuData->ProcedureLock);

  SendInterrupt (
    BROADCAST_MODE_SPECIFY_CPU,
    CpuData->ApicID,
    0,
    DELIVERY_MODE_INIT,
    TRIGGER_MODE_EDGE,
    TRUE
    );
  SendInterrupt (
    BROADCAST_MODE_SPECIFY_CPU,
    CpuData->ApicID,
    (UINT32) RShiftU64 (mAcpiCpuData->WakeUpBuffer, 12),
    DELIVERY_MODE_SIPI,
    TRIGGER_MODE_EDGE,
    TRUE
    );
  CpuData->StateLock = 0;
}

/**
  Check whether any AP is running for assigned task.

  @retval  TRUE  - Some APs are running.
  @retval  FALSE - No AP is running.

**/
BOOLEAN
ApRunning (
  VOID
  )
{
  CPU_DATA_BLOCK  *CpuData;
  UINTN           CpuNumber;

  for (CpuNumber = 0; CpuNumber < mMPSystemData->NumberOfCpus; CpuNumber++) {
    CpuData = &mMPSystemData->CpuData[CpuNumber];

    if (CpuNumber != mMPSystemData->BSP) {
      if (CpuData->State == CPU_STATE_READY || CpuData->State == CPU_STATE_BUSY) {
        return TRUE;
      }
    }
  }
 
  return FALSE;
}

/**
  Count the number of APs that have been switched
  to E0000 or F0000 segments by ReAllocateMemoryForAP().

**/
VOID
LegacyRegionAPCount (
  VOID
  )
{
  AsmAcquireMPLock (&mMPSystemData->APSerializeLock);

  mSwitchToLegacyRegionCount++;

  AsmReleaseMPLock (&mMPSystemData->APSerializeLock);
}
