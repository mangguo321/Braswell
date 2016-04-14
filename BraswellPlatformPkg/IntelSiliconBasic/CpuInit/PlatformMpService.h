/** @file
  some definitions for MP services Protocol.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PLATFORM_MP_SERVICE_H_
#define _PLATFORM_MP_SERVICE_H_

#include "CpuDxe.h"
#include "MpCommon.h"

/**
  @todo	add description

**/
typedef struct {
  UINT32  Package;
  UINT32  Die;
  UINT32  Core;
  UINT32  Thread;
} PHYSICAL_LOCATION;

//
// Constant definitions
//
#define FOURGB                                0x100000000
#define ONEPAGE                               0x1000

#define RENDEZVOUS_PROC_LENGTH                0x1000
#define STACK_SIZE_PER_PROC                   0x8000
#define MAX_CPU_S3_MTRR_ENTRY                 0x0020
#define MAX_CPU_S3_TABLE_SIZE                 0x0400

#define AP_HALT_CODE_SIZE                     10

#define CPU_CHECK_AP_INTERVAL                 0x10  // microseconds
//
//  The MP data structure follows.
//
#define CPU_SWITCH_STATE_IDLE   0
#define CPU_SWITCH_STATE_STORED 1
#define CPU_SWITCH_STATE_LOADED 2

#define MSR_L3_CACHE_DISABLE  0x40

/**
  @todo	add description

**/
typedef struct {
  UINT8             Lock;         // offset 0
  UINT8             State;        // offset 1
  UINTN             StackPointer; // offset 4 / 8
  IA32_DESCRIPTOR Gdtr;         // offset 8 / 16
  IA32_DESCRIPTOR Idtr;         // offset 14 / 26
} CPU_EXCHANGE_ROLE_INFO;

/**
  MTRR table definitions
**/
typedef struct {
  UINT16  Index;
  UINT64  Value;
} EFI_MTRR_VALUES;

typedef enum {
  CPU_STATE_IDLE,
  CPU_STATE_BLOCKED,
  CPU_STATE_READY,
  CPU_STATE_BUSY,
  CPU_STATE_FINISHED,
  CPU_STATE_DISABLED
} CPU_STATE;

//
// Define CPU feature information
//
#define MAX_FEATURE_NUM  6

/**
  @todo	add description

**/
typedef struct {
  UINTN                 Index;
  UINT32                ApicId;
  UINT32                Version;
  UINT32                FeatureDelta;
  UINT32                Features[MAX_FEATURE_NUM];
} LEAST_FEATURE_PROC;

/**
// Define Individual Processor Data block.
**/
typedef struct {
  UINT32                    ApicID;
  EFI_AP_PROCEDURE          Procedure;
  VOID                      *Parameter;
  UINT8                     StateLock;
  UINT8                     ProcedureLock;
  UINT32                    Health;
  BOOLEAN                   SecondaryCpu;
  UINTN                     NumberOfCores;
  UINTN                     NumberOfThreads;
  UINT64                    ActualFsbFrequency;
  EFI_STATUS                MicrocodeStatus;
  UINT32                    FailedRevision;
  EFI_CPU_PHYSICAL_LOCATION PhysicalLocation;
  CPU_STATE                 State;
  //
  // for PI MP Services Protocol
  //
  BOOLEAN   *Finished;
  UINT64    ExpectedTime;
  EFI_EVENT WaitEvent;
  EFI_EVENT             CheckThisAPEvent;
} CPU_DATA_BLOCK;

/**
  @todo	add description

**/
typedef struct {
  UINT32            ApicId;
  UINT32            MsrIndex;
  UINT64            MsrValue;
} MP_CPU_S3_SCRIPT_DATA;

/**
  @todo	add description

**/
typedef struct {  
  UINT32            S3BootScriptTable;
  UINT32            S3BspMtrrTable;
  UINT32            VirtualWireMode;
} MP_CPU_S3_DATA_POINTER;

/**
  @todo	add description

**/
#pragma pack (1)
typedef struct {
  UINT32 ApicId;
  UINT32 Health;
} BIST_HOB_DATA;
#pragma pack ()

/**
  Define MP data block which consumes individual processor block.
**/
typedef struct {
  UINT8                       APSerializeLock;

  UINT8                       Tm2Core2BusRatio;       // for thermal monitor 2 initialization
  UINT8                       Tm2Vid;                 // for thermal monitor 2 initialization
  BOOLEAN                     LimitCpuidMaximumValue; // make processor look like < F40
  BOOLEAN                     EnableL3Cache;
  BOOLEAN                     IsC1eSupported;
  BOOLEAN                     C1eEnable;
  BOOLEAN                     AesEnable;
  BOOLEAN                     PeciEnable;
  BOOLEAN                     ProcessorVmxEnable;
  BOOLEAN                     ProcessorBistEnable;
  BOOLEAN                     ProcessorMsrLockControl;
  BOOLEAN                     Processor3StrikeControl;
  BOOLEAN                     LtEnable;
  BOOLEAN                     EchoTprDisable;
  BOOLEAN                     MonitorMwaitEnable;
  BOOLEAN                     FastString;
  BOOLEAN                     TurboModeEnable;
  BOOLEAN                     ExtremeEnable;
  BOOLEAN                     XapicEnable;
  BOOLEAN                     MachineCheckEnable;
  BOOLEAN                     MLCSpatialPrefetcherEnable;
  BOOLEAN                     MLCStreamerPrefetcherEnable;
  BOOLEAN                     DCUStreamerPrefetcherEnable;
  BOOLEAN                     DCUIPPrefetcherEnable;
  BOOLEAN                     CcxEnable;
  BOOLEAN                     C1AutoDemotion;
  BOOLEAN                     C3AutoDemotion;
  BOOLEAN                     Vr11Enable;
  UINT8                       PackageCState;

  BOOLEAN                     Gv3Enable;
  BOOLEAN                     PsdState;
  BOOLEAN                     EnableSecondaryCpu;
  BOOLEAN                     DcaEnable;
  UINTN                       DcaPrefetchDelayValue;

  BOOLEAN                     DCUModeSelection;
  BOOLEAN                     BiDirectionalProchot;

  UINTN                       NumberOfCpus;
  UINTN                       MaximumCpusForThisSystem;

  CPU_EXCHANGE_ROLE_INFO      BSPInfo;
  CPU_EXCHANGE_ROLE_INFO      APInfo;

  EFI_CPU_ARCH_PROTOCOL       *CpuArch;
  EFI_EVENT                   CheckThisAPEvent;
  EFI_EVENT                   CheckAllAPsEvent;
  EFI_EVENT                   WaitEvent;
  UINTN                       BSP;
  BIST_HOB_DATA              *BistHobData;
  UINTN                       BistHobSize;

  UINTN                       FinishCount;
  UINTN                       StartCount;
  EFI_AP_PROCEDURE            Procedure;
  VOID                        *ProcArguments;
  BOOLEAN                     SingleThread;
  UINTN                       StartedCpuNumber;
  UINT8                       Pad;

  CPU_DATA_BLOCK              CpuData[MAXIMUM_CPU_NUMBER];
  EFI_CPU_STATE_CHANGE_CAUSE  DisableCause[MAXIMUM_CPU_NUMBER];

  UINT8                       S3BootScriptLock;
  UINT32                      S3BootScriptCount;
  MP_CPU_S3_DATA_POINTER      S3DataPointer;
  MP_CPU_S3_SCRIPT_DATA       S3BootScriptTable[MAX_CPU_S3_TABLE_SIZE];
  EFI_MTRR_VALUES             S3BspMtrrTable[MAX_CPU_S3_MTRR_ENTRY];
  UINT8                       ActiveProcessorCores;  
} MP_SYSTEM_DATA;

#pragma pack (1)

/**
  @todo	add description

**/
typedef struct {
  ACPI_CPU_DATA_COMPATIBILITY  AcpiCpuData;
  MP_SYSTEM_DATA               MPSystemData;
  IA32_DESCRIPTOR              GdtrProfile;
  IA32_DESCRIPTOR              IdtrProfile;
  EFI_CPU_MICROCODE_HEADER*    MicrocodePointerBuffer[NUMBER_OF_MICROCODE_UPDATE + 1];
} MP_CPU_RESERVED_DATA;

#define CPU_MP_SERVICE_PRIVATE_SIGNATURE  EFI_SIGNATURE_32 ('m', 'p', '3', '2')

/**
  @todo	add description

**/
typedef struct {
  UINTN                     Signature;
  EFI_HANDLE                Handle;
  EFI_MP_SERVICES_PROTOCOL  MpService;
  MP_SYSTEM_DATA            MPSystemData;
} CPU_MP_SERVICE_PROTOCOL_PRIVATE;

#define CPU_MP_SERVICE_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, \
      CPU_MP_SERVICE_PROTOCOL_PRIVATE, \
      MpService, \
      CPU_MP_SERVICE_PRIVATE_SIGNATURE \
      )
#pragma pack ()

extern CPU_MP_SERVICE_PROTOCOL_PRIVATE  *Private;
extern MP_SYSTEM_DATA                   *mMPSystemData;
extern ACPI_CPU_DATA_COMPATIBILITY      *mAcpiCpuData;

//
// Prototypes.
//
EFI_STATUS
MpInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
;

/**
  Implementation of GetNumberOfProcessors() service of MP Services Protocol.

  This service retrieves the number of logical processor in the platform
  and the number of those logical processors that are enabled on this boot.
  This service may only be called from the BSP.

  @param[in] This                        A pointer to the EFI_MP_SERVICES_PROTOCOL instance.
  @param[in] NumberOfProcessors          Pointer to the total number of logical processors in the system,
                                         including the BSP and disabled APs.
  @param[in] NumberOfEnabledProcessors   Pointer to the number of enabled logical processors that exist
                                         in system, including the BSP.

  @retval EFI_SUCCESS                    Number of logical processors and enabled logical processors retrieved..
  @retval EFI_DEVICE_ERROR               Caller processor is AP.
  @retval EFI_INVALID_PARAMETER          NumberOfProcessors is NULL.
  @retval EFI_INVALID_PARAMETER          NumberOfEnabledProcessors is NULL.

**/
EFI_STATUS
EFIAPI
GetNumberOfProcessors (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  OUT UINTN                   *NumberOfProcessors,
  OUT UINTN                   *NumberOfEnabledProcessors
  );

/**
  Implementation of GetProcessorInfo() service of MP Services Protocol.

  Gets detailed MP-related information on the requested processor at the
  instant this call is made. This service may only be called from the BSP.

  @param[in] This                  A pointer to the EFI_MP_SERVICES_PROTOCOL instance.
  @param[in] ProcessorNumber       The handle number of processor.
  @param[in] ProcessorInfoBuffer   A pointer to the buffer where information for the requested processor is deposited.

  @retval EFI_SUCCESS              Processor information successfully returned.
  @retval EFI_DEVICE_ERROR         Caller processor is AP.
  @retval EFI_INVALID_PARAMETER    ProcessorInfoBuffer is NULL
  @retval EFI_NOT_FOUND            Processor with the handle specified by ProcessorNumber does not exist.

**/
EFI_STATUS
EFIAPI
GetProcessorInfo (
  IN EFI_MP_SERVICES_PROTOCOL    *This,
  IN UINTN                       ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION  *ProcessorInfoBuffer
  );

/**
  MP Service to get specified application processor (AP)
  to execute a caller-provided code stream.

  @param[in] This                  Pointer to MP Service Protocol
  @param[in] Procedure             The procedure to be assigned to AP.
  @param[in] ProcessorNumber       The handle number of processor.
  @param[in] WaitEvent             If timeout, the event to be triggered after this AP finishes.
  @param[in] TimeoutInMicroSecs    The timeout value in microsecond. Zero means infinity.
  @param[in] ProcArguments         Argument for Procedure.

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
  IN UINTN                    ProcessorNumber,
  IN EFI_EVENT WaitEvent      OPTIONAL,
  IN UINTN TimeoutInMicroSecs OPTIONAL,
  IN VOID *ProcArguments      OPTIONAL,
  OUT BOOLEAN *Finished       OPTIONAL
  );

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
  @param[in] FailedCPUList         If not NULL, all APs that fail to start will be recorded in the list.

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
  );

/**
  MP Service to makes the current BSP into an AP and then switches the
  designated AP into the AP. This procedure is usually called after a CPU
  test that has found that BSP is not healthy to continue it's responsbilities.

  @param[in] This                 Pointer to MP Service Protocol.
  @param[in] ProcessorNumber      The handle number of processor.
  @param[in] EnableOldBSPState    Whether to enable or disable the original BSP.

  @retval EFI_INVALID_PARAMETER   Number for Specified AP out of range.
  @retval EFI_INVALID_PARAMETER   Number of specified CPU belongs to BSP.
  @retval EFI_NOT_READY           Specified AP is not idle.
  @retval EFI_SUCCESS             BSP successfully switched.

**/
EFI_STATUS
EFIAPI
SwitchBSP (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN UINTN                    ProcessorNumber,
  IN BOOLEAN                  EnableOldBSPState
  );

/**
  Implementation of EnableDisableAP() service of MP Services Protocol.

  This service lets the caller enable or disable an AP.
  This service may only be called from the BSP.

  @param[in] This                    A pointer to the EFI_MP_SERVICES_PROTOCOL instance.
  @param[in] ProcessorNumber         The handle number of processor.
  @param[in] NewAPState              Indicates whether the newstate of the AP is enabled or disabled.
  @param[in] HealthState             Indicates new health state of the AP..

  @retval EFI_SUCCESS                AP successfully enabled or disabled.
  @retval EFI_DEVICE_ERROR           Caller processor is AP.
  @retval EFI_NOT_FOUND              Processor with the handle specified by ProcessorNumber does not exist.
  @retval EFI_INVALID_PARAMETERS     ProcessorNumber specifies the BSP.

**/
EFI_STATUS
EFIAPI
EnableDisableAP (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN UINTN                      ProcessorNumber,
  IN BOOLEAN                    NewAPState,
  IN UINT32 *HealthState OPTIONAL
  );

EFI_STATUS
EFIAPI
WhoAmI (
  IN  EFI_MP_SERVICES_PROTOCOL            *This,
  OUT UINTN                               *CpuNumber
  );

EFI_STATUS
GetMPDataBlocks (
  IN     MP_SYSTEM_DATA  *MPSystemData
  );

EFI_STATUS
InitializeMpSystemData (
  VOID
  );

//
// Assembly stub definitions.
//
VOID
AsmGetProcsParams1 (
  OUT  UINTN               MyGlobalID,
  OUT  UINTN               MyNumberOfProcessorCores,
  OUT  UINTN               MyNumberOfProcessorThreads,
  OUT  UINTN               RendezIntNumber
  );

VOID
AsmGetProcsParams2 (
  OUT  UINTN               MyHealthStatus,
  OUT  UINTN               MyNodeNumber,
  OUT  UINTN               MyNodeMemSize,
  OUT  UINTN               MyProcessorCompatibility
  );

VOID
AsmGetProcsParams3 (
  OUT  UINTN               MyProcessorTestMask,
  OUT  UINTN               MyProcessorSlotNumber,
  OUT  UINTN               MyProcessorPackageNumber
  );

VOID
AsmFlushProgData (
  IN   UINTN               MemAddress,
  IN   UINTN               Count
  );

VOID
AsmWakeUpAPs (
  IN   UINTN               *WakeUpBuffer,
  IN   UINTN               MemAddress,
  IN   UINTN               *StackAddressStart,
  IN   UINTN               StackSize,
  IN   UINTN               *APDoneSemaphore,
  IN   UINTN               *GDTPageAddress
  );

EFI_STATUS
WakeUpAPs (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  );

VOID
AsmExchangeRole (
  IN   CPU_EXCHANGE_ROLE_INFO    *MyInfo,
  IN   CPU_EXCHANGE_ROLE_INFO    *OthersInfo
  );

VOID
FutureBSPProc (
  IN     MP_SYSTEM_DATA  *MPSystemData
  );

EFI_STATUS
GetMpBistStatus (
  IN     MP_SYSTEM_DATA  *MPSystemData
  );

//
// Function declarations
//
VOID
InitializeMpData (
  IN UINTN         ProcessorInstance
  );

VOID
EFIAPI
MpServiceInitialize (
  VOID
  );

UINTN
MpMtrrSynchUpEntry (
  VOID
  );

VOID
MpMtrrSynchUpExit (
  UINTN Cr4
  );

VOID
SaveBspMtrrForS3 (
  );

EFI_STATUS
FillInProcessorInformation (
  IN     MP_SYSTEM_DATA       *MPSystemData,
  IN     BOOLEAN              BSP,
  IN     UINT32               BistParam
  );

VOID
APFinishTask (
  IN  EFI_EVENT                        Event,
  IN  VOID                             *Context
  );

EFI_STATUS
SetApicBSPBit (
  IN  BOOLEAN   Enable
  );

EFI_STATUS
ChangeCpuState (
  IN     UINTN                      CpuNumber,
  IN     BOOLEAN                    NewState,
  IN     EFI_CPU_STATE_CHANGE_CAUSE Cause
  );

BOOLEAN
IsSecondaryThread (
  VOID
  );

VOID
CheckAllAPsStatus (
  IN  EFI_EVENT                           Event,
  IN  VOID                                *Context
  );

VOID
CheckThisAPStatus (
  IN  EFI_EVENT                           Event,
  IN  VOID                                *Context
  );

UINT64
CalculateTimeout (
  IN  UINTN                               TimeoutInMicroSecs
  );

BOOLEAN
CheckTimeout (
  IN  UINT64                              ExpectedTsc
  );

EFI_STATUS
GetNextBlockedCpuNumber (
  OUT UINTN                               *NextCpuNumber
  );

EFI_STATUS
FillInCpuLocation (
  IN EFI_CPU_PHYSICAL_LOCATION   *Location
  );

VOID
DetailedInitialization (
  VOID
  );

VOID
WakeUpAp (
  IN   CPU_DATA_BLOCK        *CpuData,
  IN   EFI_AP_PROCEDURE      Procedure,
  IN   VOID                  *Parameter
  );

UINT8
GetCoreNumber (
  VOID
  );

#endif
