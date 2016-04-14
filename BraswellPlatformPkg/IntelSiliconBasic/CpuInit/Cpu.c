/** @file
  Cpu driver, which initializes CPU and implements CPU Architecture
  Protocol as defined in Framework specification.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CpuDxe.h"
#include "Exception.h"
#include "MiscFuncs.h"
#include "MemoryAttribute.h"
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#define   SAMPLE_TICK_COUNT        100

extern UINT64                      mValidMtrrAddressMask;
extern UINT64                      mValidMtrrBitsMask;
extern EFI_CPU_MICROCODE_HEADER    **mMicrocodePointerBuffer;

VOID                               *mSmmBaseRegistration;
EFI_METRONOME_ARCH_PROTOCOL        *mMetronome;
BOOLEAN                            mIsFlushingGCD = TRUE;
UINT64                             mCpuFrequency = 0;


CONST UINT16 miFSBFrequencyTable[] = {
   83,          // 83.4MHz
  100,          // 100MHz
  133,          // 133.4MHz
  116,          // 116.5MHz
   83,          // 83.3MHz
  100,          // 100MHz
  133,          // 133.3MHz
  116,          // 116.7MHz

   80,          // 80MHz
   93,          // 93.3MHz
   90,          // 90MHz
   88,          // 88.9MHz
   87           // 87.5MHz
};

//
// Function declarations
//
EFI_STATUS
EFIAPI
InitializeCpu (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_STATUS
PrepareMemory (
  VOID
  );

EFI_STATUS
EFIAPI
FlushCpuDataCache (
  IN EFI_CPU_ARCH_PROTOCOL     *This,
  IN EFI_PHYSICAL_ADDRESS      Start,
  IN UINT64                    Length,
  IN EFI_CPU_FLUSH_TYPE        FlushType
  );

EFI_STATUS
EFIAPI
EnableInterrupt (
  IN EFI_CPU_ARCH_PROTOCOL     *This
  );

EFI_STATUS
EFIAPI
DisableInterrupt (
  IN EFI_CPU_ARCH_PROTOCOL     *This
  );

EFI_STATUS
EFIAPI
CpuGetInterruptState (
  IN  EFI_CPU_ARCH_PROTOCOL     *This,
  OUT BOOLEAN                   *State
  );

EFI_STATUS
EFIAPI
Init (
  IN EFI_CPU_ARCH_PROTOCOL     *This,
  IN EFI_CPU_INIT_TYPE         InitType
  );

EFI_STATUS
EFIAPI
RegisterInterruptHandler (
  IN EFI_CPU_ARCH_PROTOCOL         *This,
  IN EFI_EXCEPTION_TYPE            InterruptType,
  IN EFI_CPU_INTERRUPT_HANDLER     InterruptHandler
  );

EFI_STATUS
EFIAPI
GetTimerValue (
  IN  EFI_CPU_ARCH_PROTOCOL       *This,
  IN  UINT32                      TimerIndex,
  OUT UINT64                      *TimerValue,
  OUT UINT64                      *TimerPeriod OPTIONAL
  );

EFI_STATUS
EFIAPI
SetMemoryAttributes (
  IN EFI_CPU_ARCH_PROTOCOL      *This,
  IN EFI_PHYSICAL_ADDRESS       BaseAddress,
  IN UINT64                     Length,
  IN UINT64                     Attributes
  );

//
// Global Variables
//
EFI_CPU_INTERRUPT_HANDLER   mExternalVectorTable[0x100];

BOOLEAN                     mInterruptState = FALSE;

//
// The Cpu Architectural Protocol that this Driver produces
//
EFI_CPU_ARCH_PROTOCOL       gCpu = {
  FlushCpuDataCache,
  EnableInterrupt,
  DisableInterrupt,
  CpuGetInterruptState,
  Init,
  RegisterInterruptHandler,
  GetTimerValue,
  SetMemoryAttributes,
  1,  // NumberOfTimers
  4,  // DmaBufferAlignment
};

/**
  Flush CPU data cache. If the instruction cache is fully coherent
  with all DMA operations then function can just return EFI_SUCCESS.

  @param[in]        This                 Protocol instance structure
  @param[in]        Start                Physical address to start flushing from.
  @param[in]        Length               Number of bytes to flush. Round up to chipset granularity.
  @param[in]        FlushType            Specifies the type of flush operation to perform.

  @retval           EFI_SUCCESS          If cache was flushed
  @retval           EFI_UNSUPPORTED      If flush type is not supported.
  @retval           EFI_DEVICE_ERROR     If requested range could not be flushed.

**/
EFI_STATUS
EFIAPI
FlushCpuDataCache (
  IN EFI_CPU_ARCH_PROTOCOL     *This,
  IN EFI_PHYSICAL_ADDRESS      Start,
  IN UINT64                    Length,
  IN EFI_CPU_FLUSH_TYPE        FlushType
  )
{
  if (FlushType == EfiCpuFlushTypeWriteBackInvalidate) {
    AsmWbinvd ();
    return EFI_SUCCESS;
  } else if (FlushType == EfiCpuFlushTypeInvalidate) {
    AsmInvd ();
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

/**
  Enables CPU interrupts.

  @param[in]  This                   Protocol instance structure

  @retval     EFI_SUCCESS            If interrupts were enabled in the CPU
  @retval     EFI_DEVICE_ERROR       If interrupts could not be enabled on the CPU.

**/
EFI_STATUS
EFIAPI
EnableInterrupt (
  IN EFI_CPU_ARCH_PROTOCOL          *This
  )
{
  CpuEnableInterrupt ();
  mInterruptState = TRUE;

  return EFI_SUCCESS;
}

/**
  Disables CPU interrupts.

  @param[in]  This                   Protocol instance structure

  @retval     EFI_SUCCESS            If interrupts were disabled in the CPU.
  @retval     EFI_DEVICE_ERROR       If interrupts could not be disabled on the CPU.

**/
EFI_STATUS
EFIAPI
DisableInterrupt (
  IN EFI_CPU_ARCH_PROTOCOL     *This
  )
{
  CpuDisableInterrupt ();
  mInterruptState = FALSE;

  return EFI_SUCCESS;
}

/**
  Return the state of interrupts.

  @param[in]      This                     Protocol instance structure
  @param[out]     State                    Pointer to the CPU's current interrupt state

  @retval         EFI_SUCCESS              If interrupts were disabled in the CPU.
  @retval         EFI_INVALID_PARAMETER	   State is NULL.

**/
EFI_STATUS
EFIAPI
CpuGetInterruptState (
  IN  EFI_CPU_ARCH_PROTOCOL     *This,
  OUT BOOLEAN                   *State
  )
{
  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *State = mInterruptState;

  return EFI_SUCCESS;
}

/**
  Generates an INIT to the CPU

  @param[in]        This                 Protocol instance structure
  @param[in]        InitType             Type of CPU INIT to perform

  @retval  EFI_SUCCESS                   If CPU INIT occurred. This value should never be seen.
  @retval  EFI_DEVICE_ERROR              If CPU INIT failed.
  @retval  EFI_UNSUPPORTED               Requested type of CPU INIT not supported.

**/
EFI_STATUS
EFIAPI
Init (
  IN EFI_CPU_ARCH_PROTOCOL      *This,
  IN EFI_CPU_INIT_TYPE          InitType
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Registers a function to be called from the CPU interrupt handler.

  @param[in]  This                  Protocol instance structure
  @param[in]  InterruptType         Defines which interrupt to hook. IA-32 valid range
                                    is 0x00 through 0xFF
  @param[in]  InterruptHandler      A pointer to a function of type
                                    EFI_CPU_INTERRUPT_HANDLER that is called when a
                                    processor interrupt occurs. A null pointer
                                    is an error condition.

  @retval  EFI_SUCCESS              If handler installed or uninstalled.

  @retval  EFI_ALREADY_STARTED      InterruptHandler is not NULL, and a handler for
                                    InterruptType was previously installed

  @retval  EFI_INVALID_PARAMETER    InterruptHandler is NULL, and a handler for
                                    InterruptType was not previously installed.

  @retval  EFI_UNSUPPORTED          The interrupt specified by InterruptType is not
                                    supported.

**/
EFI_STATUS
EFIAPI
RegisterInterruptHandler (
  IN EFI_CPU_ARCH_PROTOCOL         *This,
  IN EFI_EXCEPTION_TYPE            InterruptType,
  IN EFI_CPU_INTERRUPT_HANDLER     InterruptHandler
  )
{
  if (InterruptType < 0 || InterruptType > 0xff) {
    return EFI_UNSUPPORTED;
  }

  if (InterruptHandler == NULL && mExternalVectorTable[InterruptType] == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (InterruptHandler != NULL && mExternalVectorTable[InterruptType] != NULL) {
    return EFI_ALREADY_STARTED;
  }

  mExternalVectorTable[InterruptType] = InterruptHandler;
  return EFI_SUCCESS;
}

/**
  Returns the CPU core to processor bus frequency ratio.

  @param[out]       Ratio                       Pointer to the CPU core to processor bus frequency ratio.

  @retval           EFI_SUCCESS                 If the ratio is returned successfully
  @retval           EFI_UNSUPPORTED             If the ratio cannot be measured
  @retval           EFI_INVALID_PARAMETER       If the input parameter is not valid

**/
EFI_STATUS
GetCpuBusRatio (
  OUT UINT32        *Ratio
  )
{
  UINT64              TempQword;

  if (Ratio == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TempQword = AsmReadMsr64 (EFI_MSR_IA32_PERF_STS);
  *Ratio    = (UINT32) (RShiftU64 (TempQword, 8) & 0x1F);

  return EFI_SUCCESS;
}

/**
  Returns a timer value from one of the CPU's internal timers. There is no
  inherent time interval between ticks but is a function of the CPU frequency.

  @param[in]  This                              Protocol instance structure.
  @param[in]  TimerIndex                        Specifies which CPU timer is requested.
  @param[out]  TimerValue                       Pointer to the returned timer value.
  @param[out]  TimerPeriod                      A pointer to the amount of time that passes in femtoseconds (10-15) for each
                                                increment of TimerValue. If TimerValue does not increment at a predictable
                                                rate, then 0 is returned. The amount of time that has passed between two calls to
                                                GetTimerValue() can be calculated with the formula
                                                (TimerValue2 - TimerValue1) * TimerPeriod. This parameter is optional and may be NULL.

  @retval           EFI_SUCCESS                 If the CPU timer count was returned.
  @retval           EFI_UNSUPPORTED             If the CPU does not have any readable timers.
  @retval           EFI_DEVICE_ERROR            If an error occurred while reading the timer.
  @retval           EFI_INVALID_PARAMETER       TimerIndex is not valid or TimerValue is NULL.

**/
EFI_STATUS
EFIAPI
GetTimerValue (
  IN  EFI_CPU_ARCH_PROTOCOL     *This,
  IN  UINT32                    TimerIndex,
  OUT UINT64                    *TimerValue,
  OUT UINT64                    *TimerPeriod OPTIONAL
  )
{
  UINT64          Actual;

  if (TimerValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (TimerIndex != 0) {
    return EFI_INVALID_PARAMETER;
  }

  *TimerValue = AsmReadTsc ();

  if (TimerPeriod != NULL) {
    GetActualFrequency (mMetronome, &Actual);
    *TimerPeriod = DivU64x32 (1000000000, (UINT32) Actual);
  }

  return EFI_SUCCESS;
}

/**
  Set memory cacheability attributes for given range of memeory

  @param[in]  This                  Protocol instance structure
  @param[in]  BaseAddress           Specifies the start address of the memory range
  @param[in]  Length                Specifies the length of the memory range
  @param[in]  Attributes            The memory cacheability for the memory range

  @retval

  EFI_SUCCESS                       If the cacheability of that memory range is set successfully
  EFI_UNSUPPORTED                   If the desired operation cannot be done
  EFI_INVALID_PARAMETER             The input parameter is not correct, such as Length = 0

**/
EFI_STATUS
EFIAPI
SetMemoryAttributes (
  IN EFI_CPU_ARCH_PROTOCOL     *This,
  IN EFI_PHYSICAL_ADDRESS      BaseAddress,
  IN UINT64                    Length,
  IN UINT64                    Attributes
  )
{
  EFI_STATUS                Status;
  UINT64                    TempQword;
  UINT32                    MsrNum, MsrNumEnd;
  UINTN                     MtrrNumber;
  BOOLEAN                   Positive;
  BOOLEAN                   OverLap;
  UINTN                     Remainder;
  EFI_MP_SERVICES_PROTOCOL  *MpService;
  EFI_STATUS                Status1;

  if (mIsFlushingGCD) {
    return EFI_SUCCESS;
  }

  TempQword = 0;

  //
  // Check for invalid parameter
  //
  if (Length == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if ((BaseAddress &~mValidMtrrAddressMask) != 0 || (Length &~mValidMtrrAddressMask) != 0) {
    return EFI_UNSUPPORTED;
  }

  switch (Attributes) {
    case EFI_MEMORY_UC:
      Attributes = EFI_CACHE_UNCACHEABLE;
      break;

    case EFI_MEMORY_WC:
      Attributes = EFI_CACHE_WRITECOMBINING;
      break;

    case EFI_MEMORY_WT:
      Attributes = EFI_CACHE_WRITETHROUGH;
      break;

    case EFI_MEMORY_WP:
      Attributes = EFI_CACHE_WRITEPROTECTED;
      break;

    case EFI_MEMORY_WB:
      Attributes = EFI_CACHE_WRITEBACK;
      break;

    default:
      return EFI_UNSUPPORTED;
  }
  //
  // Check if Fixed MTRR
  //
  Status = EFI_SUCCESS;
  while ((BaseAddress < (1 << 20)) && (Length > 0) && Status == EFI_SUCCESS) {
    PreMtrrChange ();
    Status = ProgramFixedMtrr (Attributes, &BaseAddress, &Length);
    PostMtrrChange ();
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (Length == 0) {
    //
    // Just Fixed MTRR. NO need to go through Variable MTRR
    //
    goto Done;
  }

  //
  // since mem below 1m will be override by fixed mtrr, we can set it to 0 to save mtrr.
  //
  if (BaseAddress == 0x100000) {
    BaseAddress = 0;
    Length += 0x100000;
  }

  //
  // Check memory base address alignment
  //
  Remainder = ModU64x32(BaseAddress, (UINT32) Power2MaxMemory (LShiftU64 (Length, 1)));
  if (Remainder != 0) {
    Remainder = ModU64x32 (BaseAddress, (UINT32) Power2MaxMemory (Length));
    if (Remainder != 0) {
      Status = EFI_UNSUPPORTED;
      goto Done;
    }
  }

  //
  // Check overlap
  //
  GetMemoryAttribute ();
  OverLap = CheckMemoryAttributeOverlap (BaseAddress, BaseAddress + Length - 1);
  if (OverLap) {
    Status = CombineMemoryAttribute (Attributes, &BaseAddress, &Length);
    if (EFI_ERROR (Status)) {
      goto Done;
    }

    if (Length == 0) {
      //
      // combine successfully
      //
      Status = EFI_SUCCESS;
      goto Done;
    }
  } else {
    if (Attributes == EFI_CACHE_UNCACHEABLE) {
      Status = EFI_SUCCESS;
      goto Done;
    }
  }

  //
  // Program Variable MTRRs
  //
  if (mUsedMtrr >= 6) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  //
  // Find first unused MTRR
  //
  MsrNumEnd = EFI_MSR_CACHE_VARIABLE_MTRR_BASE + (2 * (UINT32)(AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP) & B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT));
  for (MsrNum = EFI_MSR_CACHE_VARIABLE_MTRR_BASE; MsrNum < MsrNumEnd; MsrNum += 2) {
    if ((AsmReadMsr64 (MsrNum + 1) & B_EFI_MSR_CACHE_MTRR_VALID) == 0) {
      break;
    }
  }

  TempQword = Length;

  if (TempQword == Power2MaxMemory (TempQword)) {
    ProgramVariableMtrr (
      MsrNum,
      BaseAddress,
      Length,
      Attributes
      );
  } else {

    GetDirection (TempQword, &MtrrNumber, &Positive);

    if ((mUsedMtrr + MtrrNumber) > 6) {
      goto Done;
    }

    if (!Positive) {
      Length = Power2MaxMemory (LShiftU64 (TempQword, 1));
      ProgramVariableMtrr (
        MsrNum,
        BaseAddress,
        Length,
        Attributes
        );
      BaseAddress += TempQword;
      TempQword   = Length - TempQword;
      Attributes  = EFI_CACHE_UNCACHEABLE;
    }

    do {
      //
      // Find unused MTRR
      //
      for (; MsrNum < MsrNumEnd; MsrNum += 2) {
        if ((AsmReadMsr64 (MsrNum + 1) & B_EFI_MSR_CACHE_MTRR_VALID) == 0) {
          break;
        }
      }

      Length = Power2MaxMemory (TempQword);
      ProgramVariableMtrr (
        MsrNum,
        BaseAddress,
        Length,
        Attributes
        );
      BaseAddress += Length;
      TempQword -= Length;

    } while (TempQword);

  }

Done:
  Status1 = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpService
                  );
  if (!EFI_ERROR (Status1)) {
    ReadMtrrRegisters ();
    Status1 = MpService->StartupAllAPs (
                          MpService,
                          MpMtrrSynchUp,
                          TRUE,
                          NULL,
                          0,
                          NULL,
                          NULL
                          );
  }

  return Status;
}

/** 
  @todo	Add structure description

**/
typedef struct {
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
} PCI_CONTROLLER_BITMAP;

PCI_CONTROLLER_BITMAP  mHostBus[] =
{
  {0, 0, 0},
  {0, 27, 0},
  {0, 30, 0},
  {0, 30, 1},
  {0, 30, 2},
  {0, 30, 3},
  {0, 30, 4},
  {0, 30, 5},
  {0, 30, 6},
  {0, 30, 7},
  {0, 31, 3},
  {0, 16, 0},
  {0, 17, 0},
  {0, 18, 0},
  {0, 26, 0},
  {0, 26, 1},
  {0, 31, 0},
  {0, 21, 0} ,
  {0, 19, 0} ,
  {0, 2, 0},
  {0, 29, 0},
  {0, 20, 0} ,
  {0, 22, 0},
  {0, 3, 0},
  {0, 24, 0},
  {0, 24, 1} ,
  {0, 24, 2},
  {0, 24, 3} ,
  {0, 24, 4},
  {0, 24, 5},
  {0, 24, 6} ,
  {0, 24, 7} ,
  {0, 28, 0},
  {0, 28, 1} ,
  {0, 28, 2},
  {0, 28, 3} ,
  {0, 23, 0},
  {0, 25, 0} ,
};

/**
  Initialize the state information for the CPU Architectural Protocol

  @param[in]  ImageHandle     Image handle of the loaded driver
  @param[in]  SystemTable     Pointer to the System Table

  @retval

  EFI_SUCCESS                 Thread can be successfully created
  EFI_OUT_OF_RESOURCES        Cannot allocate protocol data structure
  EFI_DEVICE_ERROR            Cannot create the thread

**/
EFI_STATUS
EFIAPI
InitializeCpu (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS               Status;
  EFI_HANDLE               NewHandle;
  EFI_EVENT                LegacyBootEvent;
  EFI_EVENT                EndOfDxeEvent = NULL;

  //
  // DXE CPU Post Codes are defined in PostCode.c, variable mPort80Table[]
  //
  Status = REPORT_STATUS_CODE_EX (
             EFI_PROGRESS_CODE,
             EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_DXE_INIT,
             0,
             &gEfiCallerIdGuid,
             NULL,
             NULL,
             0
             );

  //
  // Initialize the Global Descriptor Table
  //
  InitializeSelectors ();

  Status = PrepareMemory ();
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize Exception Handlers
  //
  Status = InitializeException (&gCpu);

  //
  // Install CPU Architectural Protocol
  //
  NewHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &NewHandle,
                  &gEfiCpuArchProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &gCpu
                  );

  ASSERT_EFI_ERROR (Status);

  Status = REPORT_STATUS_CODE_EX (
             EFI_PROGRESS_CODE,
             EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_DXE_STEP1,
             0,
             &gEfiCallerIdGuid,
             NULL,
             NULL,
             0
             );

  //
  // Refresh memory space attributes according to MTRRs
  //
  Status = RefreshGcdMemoryAttributes ();
  mIsFlushingGCD = FALSE;
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = REPORT_STATUS_CODE_EX (
             EFI_PROGRESS_CODE,
             EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_DXE_STEP2,
             0,
             &gEfiCallerIdGuid,
             NULL,
             NULL,
             0
             );
    
  Status = gBS->LocateProtocol (&gEfiMetronomeArchProtocolGuid, NULL, (VOID **) &mMetronome);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = REPORT_STATUS_CODE_EX (
             EFI_PROGRESS_CODE,
             EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_DXE_STEP3,
             0,
             &gEfiCallerIdGuid,
             NULL,
             NULL,
             0
             );

  Status = REPORT_STATUS_CODE_EX (
             EFI_PROGRESS_CODE,
             EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_DXE_STEP4,
             0,
             &gEfiCallerIdGuid,
             NULL,
             NULL,
             0
             );

  //
  // Initialize MP Support if necessary
  //
  Status = InitializeMpSupport (ImageHandle, SystemTable);

  Status = REPORT_STATUS_CODE_EX (
             EFI_PROGRESS_CODE,
             EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_DXE_END,
             0,
             &gEfiCallerIdGuid,
             NULL,
             NULL,
             0
             );
  //
  // Create an EndOfDxe protocol callback event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  (EFI_EVENT_NOTIFY) CpuInitBeforeBoot,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventLegacyBootEx (
            TPL_CALLBACK,
            (EFI_EVENT_NOTIFY) CpuInitBeforeBoot,
            NULL,
            &LegacyBootEvent
            );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Determine the processor core frequency

  @retval		  Processor core frequency multiplied by 3

**/
UINT16
DetermineiFsbFromMsr (
  VOID
  )
{
  UINT64  FrequencyIndex;
  
  //
  // Determine the processor core frequency
  //
  FrequencyIndex = (AsmReadMsr64 (BSEL_CR_OVERCLOCK_CONTROL)) & FUSE_BSEL_MASK;
  if (AsmReadMsr64 (BSEL_CR_OVERCLOCK_CONTROL) & BIT5) {
    //
    //maybe change to (FrequencyIndex&0x3)+12 if Lookup table size increases on B-Step
    //
    return miFSBFrequencyTable[12];
  } else if (AsmReadMsr64 (BSEL_CR_OVERCLOCK_CONTROL) & BIT28) {
    //
    // Bit 28 combined with bits[0:1] gives us unique lookup value.
    // When BIT28 is set, Table offset will be 8 + bits[1:0] of MSR
    //
    return miFSBFrequencyTable[(FrequencyIndex&0x3)+8];
  } else {
    return miFSBFrequencyTable[FrequencyIndex];
  }
}

/**
  Returns the actual CPU core frequency in MHz.

  @param[in]  Metronome               Metronome protocol
  @param[out] Frequency               Pointer to the CPU core frequency

  @retval  EFI_SUCCESS                If the frequency is returned successfully
  @retval  EFI_INVALID_PARAMETER      If the input parameter is wrong

**/
EFI_STATUS
GetActualFrequency (
  IN  EFI_METRONOME_ARCH_PROTOCOL   *Metronome,
  OUT UINT64                        *Frequency
  )
{
  UINT64               BeginValue;
  UINT64               EndValue;
  UINTN                TickCount;
  BOOLEAN              mInterruptState;
  EFI_STATUS           Status;
  UINT64               TotalValue;

  if (Metronome == NULL || Frequency == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (mCpuFrequency == 0) {
    *Frequency = 0;

    //
    // In order to calculate the actual CPU frequency, we keep track of the CPU Tsc value (which
    // increases by 1 for every cycle) for a know period of time. The Metronome is not accurate
    // for the 1st tick, so I choose to wait for 100 ticks, thus the error can be control to be
    // lower than 1%.
    //
    CpuGetInterruptState (&gCpu, &mInterruptState);
    if (mInterruptState) {
      DisableInterrupt (&gCpu);
    }
    //
    // Wait for 3000us = 3ms for the calculation
    // It needs a precise timer to calculate the ticks
    //
    TickCount   = SAMPLE_TICK_COUNT *4;
    while (TRUE) {
      BeginValue  = AsmReadTsc ();
      Status      = Metronome->WaitForTick (Metronome, (UINT32)TickCount);
      EndValue    = AsmReadTsc ();
      if (!EFI_ERROR (Status)) {
        TotalValue = EndValue - BeginValue;
        break;
      }
    }
    if (mInterruptState) {
      EnableInterrupt (&gCpu);
    }

    mCpuFrequency = MultU64x32 (TotalValue, 10);
    mCpuFrequency = DivU64x32 (mCpuFrequency, Metronome->TickPeriod * (UINT32)TickCount);

  }
  *Frequency = mCpuFrequency;

  return EFI_SUCCESS;
}

/**
  Create Perform Final Init before boot to OS

  @param[in]    Event      A pointer to the Event that triggered the callback.
  @param[in]    Context    A pointer to private data registered with the callback function.

**/
VOID
CpuInitBeforeBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                Status;
  VOID                      *ProtocolPointer;
  EFI_MP_SERVICES_PROTOCOL  *MpService;

  Status = gBS->LocateProtocol (&gEfiDxeSmmReadyToLockProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return ;
  }

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpService
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Configure for BSP first
    //
    ApCpuInitBeforeBoot();
    //
    // Confiture the rest APs
    //
    Status = MpService->StartupAllAPs (
                          MpService,
                          (EFI_AP_PROCEDURE) ApCpuInitBeforeBoot,
                          TRUE,
                          NULL,
                          0,
                          NULL,
                          NULL
                          );
  }

  return ;
}

/**
  Perform final Inititialization before boot to OS

  @param[in] None

  @retval None
**/
VOID
ApCpuInitBeforeBoot (
  VOID
  )
{
}
