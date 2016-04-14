/** @file
  Some definitions for MP and HT driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MP_COMMON_
#define _MP_COMMON_

#include "CpuDxe.h"
#include "Exception.h"
#include "ProcessorDef.h"

#define EFI_CPU_CAUSE_NOT_DISABLED      0x0000
#define EFI_CPU_CAUSE_INTERNAL_ERROR    0x0001
#define EFI_CPU_CAUSE_THERMAL_ERROR     0x0002
#define EFI_CPU_CAUSE_SELFTEST_FAILURE  0x0004
#define EFI_CPU_CAUSE_PREBOOT_TIMEOUT   0x0008
#define EFI_CPU_CAUSE_FAILED_TO_START   0x0010
#define EFI_CPU_CAUSE_CONFIG_ERROR      0x0020
#define EFI_CPU_CAUSE_USER_SELECTION    0x0080
#define EFI_CPU_CAUSE_BY_ASSOCIATION    0x0100
#define EFI_CPU_CAUSE_UNSPECIFIED       0x8000

#define VacantFlag            0x00
#define NotVacantFlag         0xff

#define MICROSECOND           10

#define MAXIMUM_CPU_NUMBER    0x40
#define STACK_SIZE_PER_PROC   0x8000

#define IO_APIC_INDEX_REGISTER  0xFEC00000
#define IO_APIC_DATA_REGISTER   0xFEC00010
#define VIRT_WIRE_A             0

//
// Data structure used in MP/HT driver
//
#define MP_CPU_EXCHANGE_INFO_OFFSET     (0x1000 - 0x400)
#define MP_CPU_LEGACY_RESET_INFO_OFFSET (0x100 - 0x20)

#pragma pack(1)
#define SIZE_OF_MCE_HANDLER 16

/**
  @todo	add description

**/
typedef struct {
  UINT16  LimitLow;
  UINT16  BaseLow;
  UINT8   BaseMiddle;
  UINT16  Attributes;
  UINT8   BaseHigh;
} SEGMENT_DESCRIPTOR;

#pragma pack()

/**
  @todo	add description

**/
typedef struct {
  UINT32                    Number;
  UINT32                    BIST;
} BIST_INFO;

typedef enum {
  WakeUpApCounterInit = 0,
  WakeUpApPerHltLoop  = 1,
  WakeUpApPerMwaitLoop= 2,
  WakeUpApPerRunLoop  = 3
} WAKEUP_AP_MANNER;

/**
  @todo	add description

**/
typedef struct {
  UINTN             Lock;
  VOID              *StackStart;
  UINTN             StackSize;
  VOID              *ApFunction;
  IA32_DESCRIPTOR   GdtrProfile;
  IA32_DESCRIPTOR   IdtrProfile;
  UINT32            BufferStart;
  UINT32            Cr3;
  UINT32            InitFlag;
  WAKEUP_AP_MANNER  WakeUpApManner;
  BIST_INFO         BistBuffer[MAXIMUM_CPU_NUMBER];
} MP_CPU_EXCHANGE_INFO;

extern ACPI_CPU_DATA_COMPATIBILITY *mAcpiCpuData;

//
// Protocol interface functions
//
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
  @param[in] ProcessorNumber       Cpu number
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
  @param[in] OldBSPState          Whether to enable or disable the original BSP.

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
  IN BOOLEAN                  OldBSPState
  );

/**
  This procedure enables or disables APs.

  @param[in] This                 Pointer to MP Service Protocol.
  @param[in] ProcessorNumber      The handle number of processor.
  @param[in] NewAPState           Indicate new desired AP state
  @param[in] HealthState          If not NULL, it points to the value that specifies the new health status of the AP.
                                  If it is NULL, this parameter is ignored.

  @retval EFI_INVALID_PARAMETER   Input paramters were not correct.
  @retval EFI_SUCCESS             Function completed successfully.

**/
EFI_STATUS
EFIAPI
EnableDisableAP (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN UINTN                      ProcessorNumber,
  IN BOOLEAN                    NewAPState,
  IN UINT32 *HealthState OPTIONAL
  );

/**
  Implementation of WhoAmI() service of MP Services Protocol.

  This service lets the caller processor get its handle number.
  This service may be called from the BSP and APs.

  @param[in] This                 A pointer to the EFI_MP_SERVICES_PROTOCOL instance.
  @param[in] ProcessorNumber      Pointer to the handle number of AP.

  @retval EFI_SUCCESS             Processor number successfully returned.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber is NULL

**/
EFI_STATUS
EFIAPI
WhoAmI (
  IN  EFI_MP_SERVICES_PROTOCOL *This,
  OUT UINTN                    *CpuNumber
  );

//
// Functions shared in MP/HT drivers
//
EFI_STATUS
SendInterrupt (
  IN  UINT32                              BroadcastMode,
  IN  UINT32                              ApicID,
  IN  UINT32                              VectorNumber,
  IN  UINT32                              DeliveryMode,
  IN  UINT32                              TriggerMode,
  IN  BOOLEAN                             Assert
  );

UINT32
GetApicID (
  OUT EFI_PHYSICAL_ADDRESS      *ApicBase OPTIONAL,
  OUT UINT32                    *ApicVersionNumber OPTIONAL
  );

VOID
ProgramVirtualWireMode (
  IN   BOOLEAN                  BSP
  );

EFI_STATUS
AllocateWakeUpBuffer (
  OUT EFI_PHYSICAL_ADDRESS          *WakeUpBuffer
  );

//
// Assembly functions implemented in MP/HT drivers
//
VOID
AsmAcquireMPLock (
  IN   UINT8            *Lock
  );

VOID
AsmReleaseMPLock (
  IN   UINT8            *Lock
  );

VOID
AsmGetGdtrIdtr (
  OUT IA32_DESCRIPTOR         **Gdt,
  OUT IA32_DESCRIPTOR         **Idt
  );

EFI_STATUS
PrepareGdtIdtForAP (
  OUT IA32_DESCRIPTOR          *GDTR,
  OUT IA32_DESCRIPTOR          *IDTR
  );

EFI_STATUS
AllocateAlignedReservedMemory (
  IN  UINTN         Size,
  IN  UINTN         Alignment,
  OUT VOID          **Pointer
  );

UINT64
AsmGetCr3 (
  VOID
  );

VOID
EFIAPI
ReAllocateMemoryForAP (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

VOID
ResetAPs (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

EFI_STATUS
PrepareMemoryForAPs (
  OUT EFI_PHYSICAL_ADDRESS       *WakeUpBuffer,
  OUT VOID                       **StackAddressStart,
  IN UINTN                       MaximumCPUsForThisSystem
  );

EFI_STATUS
PrepareExchangeInfo (
  OUT MP_CPU_EXCHANGE_INFO           *ExchangeInfo,
  IN  VOID                           *StackAddressStart,
  IN  VOID                           *ApFunction,
  IN  EFI_PHYSICAL_ADDRESS           WakeUpBuffer
  );

EFI_STATUS
S3PrepareMemoryForAPs (
  OUT EFI_PHYSICAL_ADDRESS       *WakeUpBuffer,
  OUT VOID                       **StackAddressStart
  );

EFI_STATUS
S3PrepareExchangeInfo (
  OUT MP_CPU_EXCHANGE_INFO           *ExchangeInfo,
  IN  VOID                           *StackAddressStart,
  IN  VOID                           *ApFunction,
  IN  EFI_PHYSICAL_ADDRESS           WakeUpBuffer
  );

BOOLEAN
ApRunning (
  VOID
  );

VOID
ApProcWrapper (
  VOID
);

UINTN
SetIdtEntry (
  IN  UINTN                       FunctionPointer,
  OUT INTERRUPT_GATE_DESCRIPTOR   *IdtEntry
);

EFI_STATUS
AllocateReservedMemoryBelow4G (
  IN   UINTN   Size,
  OUT  VOID    **Buffer
  );

VOID
RedirectFarJump (
  VOID
  );

VOID
LegacyRegionAPCount (
  VOID
  );

#endif
