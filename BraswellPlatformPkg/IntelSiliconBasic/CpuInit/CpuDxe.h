/** @file
  Private data structures.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CPU_DXE_H
#define _CPU_DXE_H

#include <PiDxe.h>

#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/AcpiVariableCompatibility.h>
#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>

#include <Protocol/Cpu.h>
#include <Protocol/MpService.h>
#include <Protocol/Metronome.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmAccess2.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/ExitPmAuth.h>
#include <Protocol/DxeSmmReadyToLock.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>     
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/DxeServicesTableLib.h>

#include <IndustryStandard/SmBios.h>

#include "CpuRegs.h"    
#include "CpuDataStruct.h"

#define BSEL_CR_OVERCLOCK_CONTROL   0xCD
#define	FUSE_BSEL_MASK              0x07

#define INTERRUPT_VECTOR_NUMBER   256
#define INTERRUPT_GATE_ATTRIBUTE  0x8e00
#define NUMBER_OF_MICROCODE_UPDATE  10

extern EFI_GUID gEfiHtBistHobGuid;

#define TRIGGER_MODE_EDGE             0x0
#define TRIGGER_MODE_LEVEL            0x1
#define SINGLE_THREAD_BOOT_FLAG 0

#define   EfiProcessorFamilyIntelAtom    0x2B

#define SMM_FROM_SMBASE_DRIVER      0x55

#define SMM_FROM_SMBASE_DRIVER_BOOTTIME          0x0
#define SMM_FROM_SMBASE_DRIVER_RUNTIME           0x1
#define SMM_FROM_SMBASE_DRIVER_LOCK              0x2
//
// This value should be same as the one in CPU driver.
//
#define SMM_FROM_CPU_DRIVER_SAVE_INFO            0x81

#define EFI_CU_HP_PC_DXE_INIT                     (EFI_SUBCLASS_SPECIFIC | 0x00000020)
#define EFI_CU_HP_PC_DXE_STEP1                    (EFI_SUBCLASS_SPECIFIC | 0x00000021)
#define EFI_CU_HP_PC_DXE_STEP2                    (EFI_SUBCLASS_SPECIFIC | 0x00000022)
#define EFI_CU_HP_PC_DXE_STEP3                    (EFI_SUBCLASS_SPECIFIC | 0x00000023)
#define EFI_CU_HP_PC_DXE_STEP4                    (EFI_SUBCLASS_SPECIFIC | 0x00000024)
#define EFI_CU_HP_PC_DXE_END                      (EFI_SUBCLASS_SPECIFIC | 0x0000002F)
#define EfiMakeCpuVersion(f, m, s)         \
  (((UINT32) (f) << 16) | ((UINT32) (m) << 8) | ((UINT32) (s)))

extern
VOID
InitializeSelectors (
  VOID
  );

//
// This is the string tool generated data representing our strings.
//
extern UINT8                      STRING_ARRAY_NAME[];

typedef struct {
  VOID  *Start;
  UINTN Size;
  UINTN FixOffset;
} INTERRUPT_HANDLER_TEMPLATE_MAP;

//
// Function declarations
//
EFI_STATUS
InitializeMicrocode (
  IN      EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  OUT     UINT32                     *FailedRevision,
  IN      BOOLEAN                    IsBsp
  );

EFI_STATUS
InitializeMpSupport (
  IN EFI_HANDLE                       ImageHandle,
  IN EFI_SYSTEM_TABLE                 *SystemTable
  );

VOID
ReadMtrrRegisters (
  VOID
  );

VOID
EFIAPI
MpMtrrSynchUp (
  IN VOID                             *Buffer
  );

EFI_STATUS
LoadAllMicrocodeUpdates (
  VOID
  );

VOID
FreeAllMicrocodeUpdates (
  VOID
  );

EFI_STATUS
CheckIncompatibleFsb (
  IN  UINTN                  CpuNumber,
  IN  UINT64                 ActualFsb,
  IN  UINT64                 IntendFsb
  );

EFI_STATUS
CheckBspBusSpeed (
  IN  EFI_METRONOME_ARCH_PROTOCOL *Metronome
  );

EFI_STATUS
GetActualFrequency (
  IN  EFI_METRONOME_ARCH_PROTOCOL   *Metronome,
  OUT UINT64                        *Frequency
  );
UINT16
DetermineiFsbFromMsr (
  VOID
  );

EFI_STATUS
Actual2StandardFrequency (
  IN  UINT64                        Actual,
  IN  UINT32                        Ratio,
  OUT UINT64                        *Standard
  );

EFI_STATUS
EnableCpuIdMaximumValueLimit (
  BOOLEAN                     LimitCpuidMaximumValue
  );

EFI_STATUS
CheckMicrocodeUpdate (
  IN  UINTN              CpuNumber,
  IN  EFI_STATUS         Status,
  IN  UINT32             FailedRevision
  );

VOID
GetTemplateAddressMap (
  OUT INTERRUPT_HANDLER_TEMPLATE_MAP *AddressMap
  );

VOID
CpuEnableInterrupt (
  VOID
  );

VOID
CpuDisableInterrupt (
  VOID
  );

UINT16
GetCodeSegment (
  VOID
  );

VOID
CpuInitFloatPointUnit (
  VOID
  );

//
// Structures
//
typedef struct {
  EFI_HANDLE            Handle;

  EFI_CPU_ARCH_PROTOCOL Cpu;

  //
  // Local Data for CPU interface goes here
  //
} CPU_ARCH_PROTOCOL_PRIVATE;

VOID
CpuInitBeforeBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

VOID
ApCpuInitBeforeBoot ();

VOID
PCIConfigWA  (
  EFI_EVENT  Event,
  VOID       *Context
  );

#define CPU_ARCH_PROTOCOL_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, \
      CPU_ARCH_PROTOCOL_PRIVATE, \
      Cpu, \
      CPU_ARCH_PROT_PRIVATE_SIGNATURE \
      )

#endif
