/** @file
  This protocol provides Platform Power Management support functionality and definitions.

  Acronyms:
    PPM   Platform Power Management
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

#ifndef _PPM_PROCESSOR_SUPPORT_PROTOCOL_2_H_
#define _PPM_PROCESSOR_SUPPORT_PROTOCOL_2_H_

#include <Protocol/PpmPlatformPolicy.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gPpmProcessorSupportProtocol2Guid;

//
// Forward reference for pure ANSI compatability
//
typedef struct _PPM_PROCESSOR_SUPPORT_PROTOCOL_2 PPM_PROCESSOR_SUPPORT_PROTOCOL_2;

//
// Protocol data definitions
//

//
// Protocol member function prototypes
//

/**
  Set the PPM flags supported by this processor.
  PpmFlags format is defined in the GlobalNvsArea protocol.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   PpmFlags updated with the features supported by the processor

**/
typedef
EFI_STATUS
(EFIAPI *SET_PPM_FLAGS) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Enable C state support as specified by the input flags

  @param[in]
  
  This          Pointer to the protocol instance
  C2IoAddress   IO address to generate C2 states (PM base + 014 usually)
  CsmIoAddress  IO trap address for CSM generated Cx requests (see PMG_IO_BASE_ADDR and PMG_CST_CONFIG_CONTROL MSR)

  @retval

  EFI_SUCCESS   Processor C state support configured successfully.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_C_STATES) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN     UINT16                             C2IoAddress,
  IN     UINT16                             CsmIoAddress

  );

/**
  Initialize general thermal management for the processor

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor thermal support configured successfully.

**/
typedef
EFI_STATUS
(EFIAPI *INIT_THERMAL) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  );

/**
  Enable TM1 for the processor

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor TM1 support configured successfully.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_TM) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Enable TM2 for the processor

  @param[in]
  
  This          Pointer to the protocol instance
  FvidPointer   Pointer to a table to be updated

  @retval

  EFI_SUCCESS   Processor TM2 support configured successfully.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_TM2) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer
  );

/**
  Enable the bi-directional PROCHOT# signal

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor PROCHOT# support configured successfully.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_PROC_HOT) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Initialize the TSC support.

  @param[in]
  
  This                                 Pointer to the protocol instance
  FvidPointer                          Pointer to a table to be updated
  FrequencyCorrectionFactor            Applicable frequency correction factor, the processor core frequency.
  PpmCstTmrFlags                       Flag of update TSC MSR if C3 or C4  
  PpmTscCorrFactor                     TSC correction factor 
  PpmTscCorrFactorRem                  TSC correction factor remainder

  @retval

  EFI_SUCCESS   Processor TSC support configured successfully.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_TSC) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN     UINT16                             FrequencyCorrectionFactor,
  IN OUT UINT8                              *PpmCstTmrFlags,
  IN OUT UINTN                              *PpmTscCorrFactor,
  IN OUT UINTN                              *PpmTscCorrFactorRem
  );

/**
  Save processor MSR runtime settings for S3.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor MSR setting is saved.

**/
typedef
EFI_STATUS
(EFIAPI *S3_SAVE_MSR) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Restore processor MSR runtime settings for S3.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor MSR setting is restored.

**/
typedef
EFI_STATUS
(EFIAPI *S3_RESTORE_MSR) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Enable IO redirection C state control.
  This should only be enabled if the OS and processor driver support
  independent C state control for C2 and higher.

  This will cause the processor to capture LVL_X reads and perform the requested
  C state transition.

  This cannot be enabled when SMI based C state coordination is enabled, 
  so this function will disable SMI based C state coordination.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor IO redirection C state control enabled.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_C_STATE_IO_REDIRECTION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Disable IO redirection C state control.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor IO redirection C state control disabled.

**/
typedef
EFI_STATUS
(EFIAPI *DISABLE_C_STATE_IO_REDIRECTION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Enable SMI based C state coordination.
  This should only be enabled if the OS and processor driver do not support
  independent C state control for C2 and higher.
  This will cause an SMI to be generated when both processors enter autohalt state.

  This cannot be enabled when IO redirection control is enabled, so this function
  will disable IO redirection control.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor SMI based C state coordination enabled.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_C_STATE_SMI_COORDINATION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Disable SMI based C state coordination.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor SMI based C state coordination disabled.

**/
typedef
EFI_STATUS
(EFIAPI *DISABLE_C_STATE_SMI_COORDINATION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Enable the Geyserville hardware coordination of P-states.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Hardware coordination enabled.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_P_STATE_HARDWARE_COORDINATION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Disable the Geyserville hardware coordination of P-states.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Hardware coordination disabled.

**/
typedef
EFI_STATUS
(EFIAPI *DISABLE_P_STATE_HARDWARE_COORDINATION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  This function enables EMTTM support in the processor.

  @param[in]
  
  This                Pointer to the protocol instance
  FvidPointer         Pointer to a table to be updated

  @retval

  EFI_SUCCESS         EMTTM enabled in the processor.

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_EMTTM) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer
  );

/**
  This function causes the specified processor to transition to a new P state.

  The function may be provided by either the protocol producer or consumer.
  If the protocol producer provides this function, it should take precedence over
  a consumer version.

  E.G. If the protocol producer does not provide the function, it will be NULL in
  the protocol structure.  The consumer would detect this and update the protocol
  with a function to provide this service.
  If the protocol producer does provide this service, the consumer should not 
  update the protocol.

  @param[in]
  
  This          Pointer to the protocol instance
  FvidPointer   Pointer to a table containing P state information
  CpuNumber     The processor thread to perform the transition on
  NextState     The desired state (zero based) from the provided FVID table

  @retval

  EFI_SUCCESS     The specified processor was transitioned to the requested state.

**/
typedef
EFI_STATUS
(EFIAPI *P_STATE_TRANSITION) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN     FVID_TABLE                         *FvidPointer,
  IN     UINTN                              CpuNumber,
  IN     UINTN                              NextState
  );

/**
  Initializes required structures for P-State table creation and enables GV3
  support in the processor.

  @param[in]
  
  This          Pointer to the protocol instance
  FvidPointer   Pointer to a table to be updated

  @retval

  EFI_SUCCESS

**/
typedef
EFI_STATUS
(EFIAPI *INIT_GV3) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       * PPMPolicy
  );

/**
  Enable C6 residency under Windows XP SP1 and SP2.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS

**/
typedef
EFI_STATUS
(EFIAPI *ENABLE_C6_RESIDENCY) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  Locks various PPM Configurations.

  @param[in]
  
  PpmPolicy      Pointer to PPM Policy structure

  @retval

  EFI_SUCCESS

--
typedef
EFI_STATUS
(EFIAPI *PPM_LOCK_SETTINGS) (
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL_2     * PPMPolicy
  );

*/
typedef
EFI_STATUS
(EFIAPI *POWER_SOURCE_CHANGE) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN BOOLEAN                                PowerSource
  );

/**
  Set processor P state when Eist is disabled.

  @param[in]
  
  This          Pointer to the protocol instance

  @retval

  EFI_SUCCESS   Processor MSR setting is saved.

**/
typedef
EFI_STATUS
(EFIAPI *PPM_SET_BOOT_P_STATE) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  );

/**
  - Configures following fields of MSR 0x610 based on user configuration:
     - Configures Long duration Turbo Mode (power limit 1) power level and time window 
     - Configures Short duration turbo mode (power limit 2) 

  @param[in]

  This          Pointer to the protocol instance
  PPMPolicy     Pointer to policy protocol instance

  @retval

  None

**/
typedef
VOID
(EFIAPI *CONFIGURE_TURBO_POWER_LIMIT) (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  );

/**
  Perform any processor configuration required on power source change event.

  @param[in]
  
  This          Pointer to the protocol instance
  PowerSource   Current power source, TRUE = AC power, FALSE = Battery power

  @retval

  EFI_SUCCESS

**/

//
// Protocol data structure definition
//
typedef struct _PPM_PROCESSOR_SUPPORT_PROTOCOL_2 {
  UINT32                                        PpmFlags;
  UINT32                                        PpmFeatureFlags;

  SET_PPM_FLAGS                                 SetPpmFlags;
  ENABLE_C_STATES                               EnableCStates;
  INIT_GV3                                      InitGv3;
  CONFIGURE_TURBO_POWER_LIMIT                   ConfigureTurboPowerLimit;
  INIT_THERMAL                                  InitThermal;
  ENABLE_TM                                     EnableTm;
  ENABLE_PROC_HOT                               EnableProcHot;
  ENABLE_TSC                                    EnableTsc;
  ENABLE_EMTTM                                  EnableEmttm;                        OPTIONAL
  
  S3_SAVE_MSR                                   S3SaveMsr;
  S3_RESTORE_MSR                                S3RestoreMsr;
  PPM_SET_BOOT_P_STATE                          SetBootPState;
  ENABLE_C_STATE_IO_REDIRECTION                 EnableCStateIoRedirection;          OPTIONAL
  DISABLE_C_STATE_IO_REDIRECTION                DisableCStateIoRedirection;         OPTIONAL
  
  ENABLE_P_STATE_HARDWARE_COORDINATION          EnablePStateHardwareCoordination;   OPTIONAL
  DISABLE_P_STATE_HARDWARE_COORDINATION         DisablePStateHardwareCoordination;  OPTIONAL
  ENABLE_C6_RESIDENCY                           EnableC6Residency;                  OPTIONAL

  //
  // If this function is not-null, consumers should use the processor specific transition code
  // Otherwise, consumer code may update this with a P state transition function
  //
  P_STATE_TRANSITION                            PStateTransition;                   OPTIONAL

  POWER_SOURCE_CHANGE                           PowerSourceChange;                  OPTIONAL

} PPM_PROCESSOR_SUPPORT_PROTOCOL_2;

#endif
