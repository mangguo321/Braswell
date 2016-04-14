/** @file
  This library contains power management configuration functions specific to 
  Cherryview processors.

  Acronyms:
    PPM   Processor Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology
    HT    Hyper-Threading Technology
    CMP   Core Multi-Processing

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SILVERMONT_PPM_LIB_H_
#define _SILVERMONT_PPM_LIB_H_

//
// Include files
//
#include <SilvermontPpmDefines.h>
#include <PiDxe.h>
#include <PpmCommon.h>
#include <Protocol/PpmPlatformPolicy.h>
#include <Protocol/PpmProcessorSupport2.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;
//
// Stall period in microseconds
//
#define PPM_WAIT_PERIOD  15

//
// Structure Declarations
//
typedef struct _ENABLE_CSTATE_PARAMS {
  PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This;
  UINT16                             C2IoAddress;
  UINT16                             CsmIoAddress;
} ENABLE_CSTATE_PARAMS;

typedef struct _ENABLE_EMTTM_PARAMS {
  PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This;
  FVID_TABLE                         *FvidPointer;
} ENABLE_EMTTM_PARAMS; 
//
// Function prototypes
//
/**
  Initializes the platform power management library.  This must be called
  prior to any of the library functions being used.

  At this time, we don't properly publish the PPM processor support protocol,
  we simply return it if this library implements the protocol.

  If the processor is not supported, the input will not be modified.

  @param[in]   This            Pointer to the PPM support protocol instance

  @retval  EFI_SUCCESS         Library was initialized successfully
  @retval  EFI_NOT_SUPPORTED   The library does not support the current processor

**/
EFI_STATUS
InitializeCherryviewPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  );

/**
  Configures following fields of MSR 0x610 based on user configuration:
  Configures Long duration Turbo Mode (power limit 1) power level and time window 
  Configures Short duration turbo mode (power limit 2) 

  @param[in, out]  This          Pointer to the protocol instance
  @param[in, out]  PPMPolicy     Pointer to policy protocol instance

  @retval   None

**/
STATIC VOID EFIAPI ConfigureTurboPowerLimitsCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  );

STATIC
EFI_STATUS 
EFIAPI
SetPpmFlagsCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

STATIC
EFI_STATUS
EFIAPI
EnableCStatesCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN     UINT16                             C2IoAddress,
  IN     UINT16                             CsmIoAddress
  );

VOID
EFIAPI
ApSafeEnableCStates (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
InitThermalCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  );

VOID
EFIAPI
ApSafeInitThermal (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
EnableTmCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

VOID
EFIAPI
ApSafeEnableTm (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
EnableProcHotCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

STATIC
EFI_STATUS
EFIAPI
EnableTscCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN     UINT16                             iFSBFrequency,
  IN OUT UINT8                              *PpmCstTmrFlags,
  IN OUT UINTN                              *PpmTscCorrFactor,
  IN OUT UINTN                              *PpmTscCorrFactorRem
  );

STATIC
EFI_STATUS
EFIAPI
S3SaveMsrCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

STATIC
EFI_STATUS
EFIAPI
S3RestoreMsrCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

VOID
EFIAPI
ApSafeRestoreMsr (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
EnableCStateIoRedirectionCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

VOID
EFIAPI
ApSafeEnableCStateIoRedirection (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
DisableCStateIoRedirectionCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

VOID
EFIAPI
ApSafeDisableCStateIoRedirection (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
EnablePStateHardwareCoordinationCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

VOID
EFIAPI
ApSafeEnablePStateHardwareCoordination (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
EFIAPI
DisablePStateHardwareCoordinationCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

VOID
EFIAPI
ApSafeDisablePStateHardwareCoordination (
  IN OUT VOID                               *Buffer
  );

STATIC
EFI_STATUS
InitFvidTableCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN     UINT16                             MaxNumberOfStates,
  IN     UINT16                             MinStepSize,
  IN     BOOLEAN                            CreateDefaultTable
  );

VOID
EFIAPI
ApSafeEnableTurboMode (
  IN OUT VOID                               *Buffer 
  );

STATIC
EFI_STATUS
CreateFvidTable (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN     UINT16                             MaxNumberOfStates,
  IN     UINT16                             MinStepSize
  );

STATIC
VOID
CreateDefaultFvidTable (
  IN OUT FVID_TABLE                         *FvidPointer
  );

STATIC
EFI_STATUS
EFIAPI
InitGv3Cherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  );

VOID
EFIAPI
ApSafeEnableGv3 (
  IN OUT VOID                               *Buffer
  );

/**
  Lock MSR_PM_CFG_CTRL
  This function must be MP safe.

  @param[in]  Buffer     Not used (needed for API compatibility)

  @retval  EFI_SUCCESS   Processor C-State locked successfully.

**/
VOID
EFIAPI
ApSafeLockDown (
  IN OUT VOID                               *Buffer
  );

/**
  Runs the specified procedure on all logical processors, passing in the
  parameter buffer to the procedure.

  @param[in] Procedure     The function to be run.
  @param[in] Buffer        Pointer to a parameter buffer.

  @retval   EFI_SUCCESS

**/
STATIC
EFI_STATUS
RunOnAllLogicalProcessors (
  IN OUT EFI_AP_PROCEDURE   Procedure,
  IN OUT VOID               *Buffer
  );

/**
  Set processor P state to HFM or LFM

  @param[in]   This          Pointer to the protocol instance

  @retval   EFI_UNSUPPORTED  EIST not supported.
  @retval   EFI_SUCCESS      Processor P state has been set.

**/
STATIC
EFI_STATUS
EFIAPI
SetBootPState (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Set processor P state to HFM or LFM.

  @param[in]   Buffer     Unused

  @retval  EFI_SUCCESS   Processor MSR setting is saved.

**/
VOID
EFIAPI
ApSafeSetBootPState (
  IN OUT VOID        *Buffer
  );
#endif
