/** @file
  Definitions for MP and HT driver.

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

#include <PiPei.h>
#include <Guid/AcpiVariableCompatibility.h>
#include <Ppi/Stall.h>
#include <Ppi/SmmAccess.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include "../Include/CpuRegs.h"    
#include "../Include/CpuDataStruct.h"


#define TRIGGER_MODE_EDGE             0x0
#define TRIGGER_MODE_LEVEL            0x1
#define DELIVERY_MODE_INIT            0x5
#define DELIVERY_MODE_SIPI            0x6

#define VacantFlag          0x00
#define NotVacantFlag       0xff

#define MICROSECOND         10

#define MAXIMUM_CPU_NUMBER  0x40
#define STACK_SIZE_PER_PROC 0x8000

#define MAXIMUM_CPU_S3_TABLE_SIZE 0x1000

#define IO_APIC_INDEX_REGISTER  0xFEC00000
#define IO_APIC_DATA_REGISTER   0xFEC00010

extern UINTN  FixedMtrrNumber;
extern UINTN  MtrrDefTypeNumber;
extern UINTN  VariableMtrrNumber;

/**

  @todo	add description

**/
typedef struct {
  UINT8                 APSerializeLock;

  UINT8                 Tm2Core2BusRatio;
  UINT8                 Tm2Vid;
  BOOLEAN               LimitCpuidMaximumValue;
  BOOLEAN               IsC1eSupported;
  BOOLEAN               IsXapicSupported;
  BOOLEAN               IsCCxSupported;

  UINT32                ApicID[MAXIMUM_CPU_NUMBER];

  UINT64                NumberOfCPUs;
  UINT64                NumberOfEnabledCPUs;
  UINT64                MaximumCPUsForThisSystem;

  UINTN                 mFinishedCount;
} HT_SYSTEM_DATA;

/**

  @todo	add description

**/
typedef struct {
  UINT16            Index;
  UINT64            Value;
} EFI_MTRR_VALUES;

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
typedef struct {
  UINT32                  Lock;
  UINT32                  StackStart;
  UINT32                  StackSize;
  UINT32                  ApFunction;
  IA32_DESCRIPTOR         GdtrProfile;
  IA32_DESCRIPTOR         IdtrProfile;
  UINT32                  BufferStart;
  UINT32                  PmodeOffset;
  UINT32                  AcpiCpuDataAddress;
  UINT32                  MtrrValuesAddress;
  UINT32                  FinishedCount;
  UINT32                  SerializeLock;
  UINT32                  MicrocodePointer;
  MP_CPU_S3_SCRIPT_DATA   *S3BootScriptTable;
  UINT32                  StartState;
  UINT32                  VirtualWireMode;
  BOOLEAN                 VerifyMicrocodeChecksum;
} MP_CPU_EXCHANGE_INFO;

VOID *
AsmGetPmodeOffset (
  );

VOID
ReadMtrrRegisters (
  UINT64  *MtrrValues
  );

VOID
MpMtrrSynchUp (
  UINT64  *MtrrValues
  );

VOID
SetBspMtrrRegisters (
  IN  EFI_MTRR_VALUES   *MtrrArray
  );

EFI_STATUS
InitializeMicrocode (
  IN      EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  OUT     UINT32                     *FailedRevision,
  IN      BOOLEAN                    IsBsp,
  IN OUT  BOOLEAN                    *VerifyMicrocodeChecksum
  );

//
// Functions shared in MP/HT drivers
//
EFI_STATUS
SendInterrupt (
  IN  UINT32                               BroadcastMode,
  IN  UINT32                               ApicID,
  IN  UINT32                               VectorNumber,
  IN  UINT32                               DeliveryMode,
  IN  UINT32                               TriggerMode,
  IN  BOOLEAN                              Assert,
  IN  CONST EFI_PEI_SERVICES               **PeiServices,
  IN  EFI_PEI_STALL_PPI                    *PeiStall
  );

UINT32
GetApicID (
  VOID
  );

VOID
ProgramVirtualWireMode (
  BOOLEAN                     BSP,
  UINT32                      VirtualWireMode
  );

VOID
AsmAcquireMPLock (
  IN   UINT8            *Lock
  );

VOID
AsmReleaseMPLock (
  IN   UINT8            *Lock
  );

VOID *
AsmGetAddressMap (
  );

VOID
AsmCliHltLoop (
  );

VOID
CpuPause (
  VOID
  );

#endif
