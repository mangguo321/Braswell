/** @file
  This library contains power management configuration functions for 
  Cherryview processors.

  Acronyms:
    PPM   Processor Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology
    HT    Hyper-Threading Technology

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// Include files
//
#include "SilvermontPpmLib.h"
#include "PpmCommon.h"
#include "CpuPpmLib.h"
#include "CpuRegs.h"

//
// Global variables
//
//
// Start: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
//
STATIC UINT8                     mTurboModeNotAvailable = 0;
//
// End: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
//

//
// CPU stepping
//
STATIC UINT16                     mProcessorStepping;
//
// Only one instance of this protocol currently supported.
//
PPM_PROCESSOR_SUPPORT_PROTOCOL_2 mPpmProcessorSupportProtocolCherryview = {
  0,                                        // PpmFlags
  0,                                        // PpmFeatureFlags
  SetPpmFlagsCherryview,
  EnableCStatesCherryview,
  InitGv3Cherryview,
  ConfigureTurboPowerLimitsCherryview,
  InitThermalCherryview,
  EnableTmCherryview,
  EnableProcHotCherryview,
  EnableTscCherryview,
  NULL,                        // EMTTM Not supportted for SLT
  S3SaveMsrCherryview,
  S3RestoreMsrCherryview,
  SetBootPState,
  EnableCStateIoRedirectionCherryview,
  DisableCStateIoRedirectionCherryview,
  EnablePStateHardwareCoordinationCherryview,
  DisablePStateHardwareCoordinationCherryview,
  NULL,
  NULL,                        // PStateTransition
  NULL                         //PowerSourceChangeCherryview,                  
};

//
// CPUID 01 values
//
STATIC EFI_CPUID_REGISTER mCpuid01 = { 0, 0, 0, 0 };

//
// Values for FVID table calculate.
//
STATIC UINT16                     mTurboBusRatio   = 0;
STATIC UINT16                     mTurboBusRatio_3C = 0;
STATIC UINT16                     mMaxBusRatio     = 0;
STATIC UINT16                     mMinBusRatio     = 0;
STATIC UINT16                     mTurboVid        = 0;
STATIC UINT16                     mTurboVid_3C     = 0;
STATIC UINT16                     mMaxVid          = 0;
STATIC UINT16                     mMinVid          = 0;
STATIC UINT16                     mProcessorFlavor = 0;
//
// Fractional part of Processor Power Unit in Watts. (i.e. Unit is 1/mProcessorPowerUnit)
//
STATIC UINT8                      mProcessorPowerUnit = 0;
//
// Fractional part of Processor Time Unit in seconds. (i.e Unit is 1/mProcessorTimeUnit)
//
STATIC UINT8                      mProcessorTimeUnit = 0;
//
// Maximum allowed power limit value in TURBO_POWER_LIMIT_MSR and PRIMARY_PLANE_POWER_LIMIT_MSR
// in units specified by PACKAGE_POWER_SKU_UNIT_MSR
//
STATIC UINT16                     mPackageMaxPower = 0;
//
// Minimum allowed power limit value in TURBO_POWER_LIMIT_MSR and PRIMARY_PLANE_POWER_LIMIT_MSR
// in units specified by PACKAGE_POWER_SKU_UNIT_MSR
//
STATIC UINT16                     mPackageMinPower = 0;
//
// Processor TDP value in Watts
//
STATIC UINT16                     mPackageTdp = 0;
//
// Porgrammable TDP Limit
//
STATIC UINT8                      mTdpLimitProgrammble = 0;

//
// Table to convert Seconds into equivalent MSR values
// This table is used for TDP Time Window programming
//
STATIC UINT8                      mSecondsToMsrValueMapTable[][2] = {
        // Seconds       MSR Value 
       {   1        ,     0x0 },
       {   5        ,     0x1 },
       {  10        ,     0x2 },
       {  15        ,     0x3 },
       {  20        ,     0x4 },
       {  25        ,     0x5 },
       {  30        ,     0x6 }, 
       {  35        ,     0x7 }, 
       {  40        ,     0x8 },
       {  45        ,     0x9 }, 
       {  50        ,     0xA },
       {END_OF_TABLE,END_OF_TABLE}
};

//
// MSR table for S3 resume
//
STATIC EFI_MSR_VALUES mMsrValues[] = {
  { MSR_IA32_CLOCK_MODULATION,           0 },  // 0x19A
  { MSR_IA32_PERF_CTL,                   0 },  // 0x199
  { MSR_PMG_IO_CAPTURE_BASE,             0 },  // 0x0E4
  { MSR_PM_CFG_CTRL,                     0 },  // 0x0E2
  { MSR_IA32_MISC_ENABLES,               0 },  // 0x1A0
  { MSR_POWER_CTL,                       0 },  // 0x1FC
  { MSR_TURBO_POWER_LIMIT,               0 },  // 0x610
  { MSR_TEMPERATURE_TARGET,              0 },  // 0x1A2
  { MSR_FLEX_RATIO,                      0 },  // 0x194
  { MSR_PRIMARY_PLANE_CURRENT_CONFIG,    0 },  // 0x601
  { MSR_IA32_ENERGY_PERFORMANCE_BIAS,    0 },  // 0x1B0
  { MSR_BBL_CR_CTL3,                     0 },  // 0x11E
  { MSR_IA32_THERM_INTERRUPT,            0 },  // 0x19B
  { MSR_POWER_MISC,                      0 },  // 0x120
  { MSR_CPU_THERM_SENS_CFG,              0 },  // 0x675
  { MSR_PRIMARY_PLANE_TURBO_POWER_LIMIT, 0 },  // 0x638
  { MSR_CPU_THERM_CFG1,                  0 },  // 0x673
  { MSR_CPU_THERM_CFG2,                  0 },  // 0x674
  { MSR_PACKAGE_POWER_SKU_UNIT,          0 },  // 0x606
  { MSR_EBC_HARD_POWERON,                0 },  // 0x02A
  { MSR_PKG_TURBO_CFG1,                  0 },  // 0x670
  { MSR_CPU_TURBO_WKLD_CFG1,             0 },  // 0x671
  { MSR_CPU_TURBO_WKLD_CFG2,             0 }   // 0x672
};

//
// Function implemenations
//
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#else
#pragma optimize("", off)
#endif

/**
  Initializes the processor power management library.  This must be called
  prior to any of the library functions being used.

  @param[in]  This          Pointer to the PPM support protocol instance

  @retval     EFI_SUCCESS   Library was initialized successfully

**/
EFI_STATUS
InitializeCherryviewPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  )
{
  MSR_REGISTER  PackagePowerSKUUnitMsr;
  MSR_REGISTER  TempMsr; 
  UINTN         remainder;

  MSR_REGISTER  PlatformIdMsr;
  MSR_REGISTER  IaCoreRatios;
  MSR_REGISTER  IaCoreVids;

  ASSERT (gSmst != NULL);
  //
  // Read the CPUID information
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, &mCpuid01.RegEax, &mCpuid01.RegEbx, &mCpuid01.RegEcx, &mCpuid01.RegEdx);
  //
  // Check if this library supports the current processor
  //
  mProcessorStepping = (UINT16) (mCpuid01.RegEax & CPUID_STEPPING);
  
  //
  // Assign the protocol pointer.
  //
  *This = &mPpmProcessorSupportProtocolCherryview;

  //
  // Get Platform ID 
  //
  PlatformIdMsr.Qword = AsmReadMsr64 (MSR_IA32_PLATFORM_ID);
  mProcessorFlavor = (UINT8) RShiftU64((PlatformIdMsr.Dwords.High & PLATFORM_ID_BITS_MASK), 18);

  //
  // Get the Bus Ratio for the processor
  //
  //
  // Get Maximum Non-Turbo bus ratio (HFM) from IACORE_RATIOS MSR Bits[23:16]
  //
  IaCoreRatios.Qword = AsmReadMsr64 (MSR_IACORE_RATIOS);
  mMaxBusRatio  = IaCoreRatios.Bytes.ThirdByte;

  //
  // Get Maximum Efficiency bus ratio (LFM) from IACORE_RATIOS MSR Bits[15:8]
  //
  mMinBusRatio = IaCoreRatios.Bytes.SecondByte;

  //
  // Get Max Turbo Ratio from Turbo Ratio Limit MSR Bits [5:0]
  //
  TempMsr.Qword = AsmReadMsr64 (MSR_IACORE_TURBO_RATIOS);
  mTurboBusRatio = (UINT16)(TempMsr.Bytes.FirstByte & MAX_RATIO_1C_MASK); 
  mTurboBusRatio_3C  = (UINT16)(TempMsr.Bytes.ThirdByte & MAX_RATIO_1C_MASK);

  //
  // Get the Vid for the processor
  //
  //
  // Get Maximum Non-Turbo Vid (HFM) from IACORE_VIDS MSR Bits[23:16]
  //
  IaCoreVids.Qword = AsmReadMsr64 (MSR_IACORE_VIDS);
  mMaxVid  = IaCoreVids.Bytes.ThirdByte;

  //
  // Get Maximum Efficiency VID (LFM) from IACORE_VIDS MSR Bits[15:8]
  //
  mMinVid = IaCoreVids.Bytes.SecondByte;

  //
  // Get Max Turbo VID from Turbo Ratio Limit MSR Bits [5:0]
  //
  TempMsr.Qword = AsmReadMsr64 (MSR_IACORE_TURBO_VIDS);
  mTurboVid = (UINT16)(TempMsr.Bytes.FirstByte & MAX_VID_1C_MASK); 
  mTurboVid_3C = (UINT16)(TempMsr.Bytes.ThirdByte & MAX_VID_1C_MASK);

  // Get Processor TDP from Turbo Power Limit MSR Bits[14:0]
  // and convert it to units specified by Package Power SKU
  // Unit MSR [3:0]
  //
  TempMsr.Qword = AsmReadMsr64 (MSR_TURBO_POWER_LIMIT);

  // Get Maximum Power from Turbo Power Limit MSR Bits[14:0]
  // and convert it to units specified by Package Power SKU Unit MSR [3:0]
  //
  PackagePowerSKUUnitMsr.Qword = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);

  mProcessorPowerUnit = (PackagePowerSKUUnitMsr.Bytes.FirstByte & PACKAGE_POWER_UNIT_MASK);
  if (mProcessorPowerUnit == 0) {
    mProcessorPowerUnit = 1;
  } else {
    // The actual unit value is calculated by 1mW*Power(2,POWER_UNIT)..Reset value of 5 represents 32mW units.
    mProcessorPowerUnit = (UINT8) LShiftU64 (1, (mProcessorPowerUnit));
  }
  
  // There are two power limits in the same MSR [14:0] and [46:32]. Bit field [14:0] reflects the package TDP.
  mPackageTdp = (UINT16) DivU64x32Remainder((TempMsr.Dwords.Low & PACKAGE_TDP_POWER_MASK), mProcessorPowerUnit, (UINT32 *)&remainder);

  /* @NOTE: This may be used in Cherryview, but still keeping this code. */
  mProcessorTimeUnit  = (UINT8) RShiftU64((PackagePowerSKUUnitMsr.Dwords.Low & PACKAGE_TIME_UNIT_MASK), 16);
  if (mProcessorTimeUnit == 0) {
    mProcessorTimeUnit = 1;
  } else {
    mProcessorTimeUnit = (UINT8) LShiftU64 (1, (mProcessorTimeUnit));
  }
  
  return EFI_SUCCESS;
}

#ifdef __GNUC__
#pragma GCC pop_options
#else
#pragma optimize("", on)
#endif
/**
  Set the PPM flags specific to mobile Cherryview processors

  @param[in]  This          Pointer to the protocol instance

  @retval     EFI_SUCCESS   PpmFlags updated with the features supported by the processor

**/
STATIC
EFI_STATUS
EFIAPI
SetPpmFlagsCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  MSR_REGISTER                PlatformId;
  MSR_REGISTER                Ia32MiscEnable;
  EFI_CPUID_REGISTER          Cpuid01 = {0};
  EFI_CPUID_REGISTER          Cpuid05 = {0};
  EFI_CPUID_REGISTER          Cpuid06 = {0};
  EFI_CPUID_REGISTER          Cpuid0B = {0};
  UINTN                       States;
  UINT8                       NumberOfLP = 0;
  UINT8                       ThreadPerCore = 0; 
  UINT32                      Ecx = 0;
  BOOLEAN                     CpuidLimitingEnabled;

  CpuidLimitingEnabled = FALSE;

  //
  // Check if the processor has multiple cores
  //
  //
  // Set the level number to 0 for SMT level of Processor Topology.
  //
  Ecx = 0;
  AsmCpuidEx (EFI_CPUID_XAPIC_PROC_TOPOLOGY, Ecx, &Cpuid0B.RegEax, &Cpuid0B.RegEbx, &Cpuid0B.RegEcx, &Cpuid0B.RegEdx);
  ThreadPerCore = (UINT8) Cpuid0B.RegEbx & 0xFF;

  //
  // Set the level number to 1 for Core level of Processor Topology.
  //
  Ecx = 1;
  AsmCpuidEx (EFI_CPUID_XAPIC_PROC_TOPOLOGY, Ecx, &Cpuid0B.RegEax, &Cpuid0B.RegEbx, &Cpuid0B.RegEcx, &Cpuid0B.RegEdx);
  NumberOfLP = (UINT8) Cpuid0B.RegEbx & 0xFF;

  if (NumberOfLP > 2 && ThreadPerCore == 1) {
    This->PpmFlags |= (PPM_QUAD | PPM_CMP);
  } else if (NumberOfLP > 1 && ThreadPerCore == 1) {
    This->PpmFlags |= PPM_CMP;
  }
  
  //
  // Cherryview support TSC updates
  //
  if (mCpuid01.RegEcx & (1 << 4)) {
      This->PpmFlags |= PPM_TSC;
  }  
  
  //
  // Set PROCHOT# always
  //
  This->PpmFlags |= PPM_PROC_HOT;
  
  //
  // Check TM capable
  //
  if (mCpuid01.RegEdx & CPUXFF_TM1) {
    This->PpmFlags |= PPM_TM;

  }

  //
  // Check GV3/ EIST capable, If EIST capable, also set the boot P-state to HFM flag.
  //

#if (!_PSSD_FIX_)
  //
  // mCpuid01.RegEcx = 0xD8E23F for PSSD and 0x43D8E3BF for ERB
  // Since BIT7 is not set in case of PSSD so skip this check to enable the PPM_GV3 flag
  // We are checking this flag to add Cpu0Ist ACPI table in SSDT
  //
  if (mCpuid01.RegEcx & CPUXFF_GV3) {
#endif
    This->PpmFlags |= (PPM_GV3 | PPM_BOOT_P_ST_HFM) ;
#if (!_PSSD_FIX_)
  }
#endif
  
  //
  // Check if anything has been disabled
  //
  PlatformId.Qword = AsmReadMsr64 (EFI_MSR_IA32_PLATFORM_ID);
  
  //
  // Check if EIST has been fuse disabled.
  //
  if (PlatformId.Qword & FB_GV3_DISABLED) {
    This->PpmFlags &= ~(PPM_GV3 | PPM_BOOT_P_ST_HFM );
  }

  //
  // Check if any type of automatic internal throttling has been fuse disabled
  //
  if (PlatformId.Qword & FB_THERMAL_THROT_DISABLED) {
    This->PpmFlags &= ~(PPM_TM);
  }

  //
  // Disable CPUID limiting (and save current setting) if enabled 
  // and enable MONITOR/MWAIT support
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (EFI_MSR_IA32_MISC_ENABLE);
  if (Ia32MiscEnable.Qword & LIMIT_CPUID) {
    Ia32MiscEnable.Qword &= (~(UINT64)LIMIT_CPUID);
    Ia32MiscEnable.Qword |= (UINT64)MONITOR_MWAIT_ENABLE;
    AsmWriteMsr64 (EFI_MSR_IA32_MISC_ENABLE, Ia32MiscEnable.Qword);

    CpuidLimitingEnabled = TRUE;
  }

  //
  // Read the CPUID values we care about.  We cannot use the stored 
  // values because they may have changes since we disabled limiting 
  // and enabled MONITOR/MWAIT
  //
  AsmCpuid (1, &Cpuid01.RegEax, &Cpuid01.RegEbx, &Cpuid01.RegEcx, &Cpuid01.RegEdx);
  AsmCpuid (5, &Cpuid05.RegEax, &Cpuid05.RegEbx, &Cpuid05.RegEcx, &Cpuid05.RegEdx);
  AsmCpuid (6, &Cpuid06.RegEax, &Cpuid06.RegEbx, &Cpuid06.RegEcx, &Cpuid06.RegEdx);

  //
  // Determine if the MONITOR/MWAIT instructions are supported.
  //
  if ((Cpuid01.RegEcx & CPUXFF_MONITOR_MWAIT && Cpuid05.RegEcx & MONITOR_MWAIT_EXTENSIONS)) {
    This->PpmFlags |= PPM_MWAIT_EXT;
  }

  //
  // Determine the C-State and Enhanced C-State support present.
  // Monitor/MWAIT parameters function describes the numbers supported.
  //
  States = (Cpuid05.RegEdx >> 4) & 0xF;
  if (States >= ENHANCED_CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C1 + PPM_C1E;
  } else if (States == CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C1;
  }

  States = (Cpuid05.RegEdx >> 8) & 0xF;
  if (States >= ENHANCED_CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C2 + PPM_C2E;
  } else if (States == CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C2;
  }

  States = (Cpuid05.RegEdx >> 12) & 0xF;
  if (States >= ENHANCED_CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C3 + PPM_C3E;
  } else if (States == CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C3;
  }

  States = (Cpuid05.RegEdx >> 16) & 0xF;
  if (States >= ENHANCED_CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C4 + PPM_C4E;
  } else if (States == CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C4;
  }

    States = (Cpuid05.RegEdx >> 24) & 0xF;
  if (States >= CSTATE_SUPPORTED) {
    This->PpmFlags |= PPM_C6 | PPM_C6S | PPM_C7;
}

  // Check if turbo mode is supported
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  if (((Cpuid06.RegEax & TURBO_MODE_SUPPORTED) == 0) && ((Ia32MiscEnable.Dwords.High & DISABLE_MASTER_TURBO_MODE) == 0)) {
    //
    // Turbo Mode is not available in this physical processor package.
    // BIOS should not attempt to enable Turbo Mode via IA32_MISC_ENABLE MSR.
    // BIOS should show Turbo Mode as Disabled and Not Configurable.
    //

    //
    // Start: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
    //
    mTurboModeNotAvailable = 1;
    //
    // End: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
    //

  } else if (((Cpuid06.RegEax & TURBO_MODE_SUPPORTED) == 0) && ((Ia32MiscEnable.Dwords.High & DISABLE_MASTER_TURBO_MODE) != 0)) {
    //
    // Turbo Mode is available but globally disabled for the all logical
    // processors in this processor package.
    // BIOS can enable Turbo Mode by IA32_MISC_ENABLE MSR 1A0h bit [38] = 0.
    //
    This->PpmFlags |= PPM_TURBO;
  } else if ((Cpuid06.RegEax & TURBO_MODE_SUPPORTED) == TURBO_MODE_SUPPORTED) {
    //
    // Turbo Mode is factory-configured as available and enabled for all logical processors in this processor package.
    // This case handles the cases where turbo mode is enabled before PPM gets chance to enable it
    //
    This->PpmFlags |= PPM_TURBO;
  }

  //
  // Restore the CPUID limit setting.
  //
  if (CpuidLimitingEnabled == TRUE) {
    Ia32MiscEnable.Qword = AsmReadMsr64 (EFI_MSR_IA32_MISC_ENABLE);
    Ia32MiscEnable.Qword |= (UINT64)LIMIT_CPUID;
    AsmWriteMsr64 (EFI_MSR_IA32_MISC_ENABLE, Ia32MiscEnable.Qword);
  }
  
  //
  // Set the T-states flag
  //
  This->PpmFlags |= PPM_TSTATES;

  //
  // Set the S0ix flag
  //
  This->PpmFlags |= PPM_S0ix;

  return EFI_SUCCESS;
}

/**
  This function will disable Power Control configuration.

  @param[in]  Buffer      Pointer to the function parameters passed in.
                         (Pointer to the PPM_PROCESSOR_SUPPORT_PROTOCOL_2 instance.)

  @retval   EFI_SUCCESS   Processor MSR setting is saved.

**/
VOID
EFIAPI
ApSafeDisablePowerControl (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER            PowerCtl;

  PowerCtl.Qword = AsmReadMsr64 (MSR_POWER_CTL);
  PowerCtl.Dwords.Low &= ~C1E_ENABLE;
  AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Qword);

  return;
}

/**
  This function will Enable Power Control configuration.

  @param[in]   Buffer        Pointer to the function parameters passed in.
                            (Pointer to the PPM_PROCESSOR_SUPPORT_PROTOCOL_2 instance.)

  @retval   EFI_SUCCESS      Processor MSR setting is saved.

**/
VOID
EFIAPI
ApSafeEnablePowerControl (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER            PowerCtl;

  PowerCtl.Qword = AsmReadMsr64 (MSR_POWER_CTL);
  PowerCtl.Dwords.Low |= C1E_ENABLE;
  AsmWriteMsr64 (MSR_POWER_CTL, PowerCtl.Qword);

  return;
}

/**
  Enables C-State support as specified by the input flags on all logical
  processors and sets associated timing requirements in the chipset.

  @param[in, out] This         Pointer to the protocol instance
  @param[in] C2IoAddress       IO address to generate C2 states (PM base + 014 usually)
  @param[in] CsmIoAddress      IO trap address for CSM generated Cx requests (see PMG_IO_BASE_ADDR and PMG_CST_CONFIG_CONTROL MSR)

  @retval   EFI_SUCCESS        Processor C state support configured successfully.

**/
STATIC
EFI_STATUS
EFIAPI
EnableCStatesCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN     UINT16                             C2IoAddress,
  IN     UINT16                             CsmIoAddress
  )
{
  ENABLE_CSTATE_PARAMS  EnableCStateParameters;
  //
  // @NOTE: This code segment may not be valid in VLV due to I/O legacy support 
  //        for C-states is no longer available. But the code is remained.
  //
  // Load the C-State parameters to pass to the core function.
  //
  EnableCStateParameters.This = This;
  EnableCStateParameters.C2IoAddress = C2IoAddress;
  EnableCStateParameters.CsmIoAddress = CsmIoAddress;
  //
  // Enable C-States on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeEnableCStates, &EnableCStateParameters);

  //
  // Configure supported enhanced C-states
  // C1E enable bit in Power Ctl MSR has package scope
  //
  //
  // Enable supported states
  //
  if (This->PpmFlags & PPM_C1E) {
    RunOnAllLogicalProcessors(ApSafeEnablePowerControl, This);
  } else {
    RunOnAllLogicalProcessors(ApSafeDisablePowerControl, This);

  }

  return EFI_SUCCESS;
}

/**
  Enable C-State support as specified by the input flags on a logical processor.
    Configure BIOS C1 Coordination (SMI coordination)
    Enable IO redirection coordination
    Choose proper coordination method
    Configure extended C-States

  This function must be MP safe.

  @param[in]   Buffer        Pointer to a ENABLE_CSTATE_PARAMS containing the necessary
                             information to enable C-States

  @retval     EFI_SUCCESS    Processor C-State support configured successfully.

**/
VOID
EFIAPI
ApSafeEnableCStates (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER                      PmCfgCtrl;
  MSR_REGISTER                      IoCaptAddr;
  MSR_REGISTER                      BblCrCtl3;
  PPM_PROCESSOR_SUPPORT_PROTOCOL_2  *This;
  UINT16                            C2IoAddress;
  UINT16                            CsmIoAddress;

  //
  // Extract parameters from the buffer
  //
  This = ((ENABLE_CSTATE_PARAMS*) Buffer)->This;
  C2IoAddress = ((ENABLE_CSTATE_PARAMS*) Buffer)->C2IoAddress;
  CsmIoAddress = ((ENABLE_CSTATE_PARAMS*) Buffer)->CsmIoAddress;

  //
  // If C-states are disabled in setup, configure PACK_LIM in CLOCK_CST_CONFIG_CONTROL
  //
  if (!(This->PpmFlags & PPM_C_STATES)) {
    PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
    PmCfgCtrl.Dwords.Low &= ~CSTATE_LIMIT_MASK;
    AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);
    return;
  }

  //
  // Set c-state package limit to the highest C-state enabled
  //
  PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
  PmCfgCtrl.Dwords.Low &= ~CSTATE_LIMIT_MASK;
  if (This->PpmFlags & PPM_C7){
    PmCfgCtrl.Dwords.Low |= CSTATE_LIMIT_C7;
    PmCfgCtrl.Dwords.Low |= DYNAMIC_L2_ENABLE;
    // 
    // When GV3 ratio is BELOW or EQUAL to this ratio, L2-reduction 
    // will be allowed in C6C/C6;  	  
    // When Ratio is ABOVE this point, Expand will be requested; 
    // If Ratio=0, then this feature is OFF (ratio will not be taken 
    // into account for L2-shrink decision making). 
    //
  	PmCfgCtrl.Dwords.Low &= ~L2_SHRINK_THRESHOLD_MASK;
    //
    //Program L2_SHRINK_THRESH to Max Ratio
    //
    if (This->PpmFlags & PPM_S0ix){
      PmCfgCtrl.Dwords.Low |= mMinBusRatio << L2_SHRINK_THRESHOLD_OFFSET;
    }
    else {
      PmCfgCtrl.Dwords.Low |= mMaxBusRatio << L2_SHRINK_THRESHOLD_OFFSET;
    }

  }
  //
  //If CPU support C6
  //
  else if (This->PpmFlags & PPM_C6) {
    PmCfgCtrl.Dwords.Low |= CSTATE_LIMIT_C6;
    PmCfgCtrl.Dwords.Low |= DYNAMIC_L2_ENABLE;
    // 
    // When GV3 ratio is BELOW or EQUAL to this ratio, L2-reduction 
    // will be allowed in C6C/C6;  	  
    // When Ratio is ABOVE this point, Expand will be requested; 
    // If Ratio=0, then this feature is OFF (ratio will not be taken 
    // into account for L2-shrink decision making). 
    // 
    PmCfgCtrl.Dwords.Low &= ~L2_SHRINK_THRESHOLD_MASK;
    //
    //Program L2_SHRINK_THRESH to Max Ratio
    //
    if (This->PpmFlags & PPM_S0ix){
      PmCfgCtrl.Dwords.Low |= mMinBusRatio << L2_SHRINK_THRESHOLD_OFFSET;
    }
    else {
      PmCfgCtrl.Dwords.Low |= mMaxBusRatio << L2_SHRINK_THRESHOLD_OFFSET;
    }
  }
  else if (This->PpmFlags & PPM_C4) {
    PmCfgCtrl.Dwords.Low |= CSTATE_LIMIT_C4;
  }
  else if (This->PpmFlags & PPM_C2) {
    PmCfgCtrl.Dwords.Low |= CSTATE_LIMIT_C2;
  }
  else if (This->PpmFlags & PPM_C1) {
    PmCfgCtrl.Dwords.Low |= CSTATE_LIMIT_C1;
  }
  AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);

  //
  // Cherryview specific configuration of I/O capture and I/O coordination SMI MSR
  // Configure the base port and range in the MSR to match LVL_X settings in ACPI tables
  //
  //
  // Set I/O capture base port and range
  //
  IoCaptAddr.Qword = AsmReadMsr64 (MSR_PMG_IO_CAPTURE_BASE);
  //
  // Mask off CST range and set the CST range
  //
  IoCaptAddr.Dwords.Low &= ~IO_CAPT_RANGE_MASK;
  if (This->PpmFlags & PPM_C7) {
    IoCaptAddr.Dwords.Low |= IO_CAPT_LVL4;
  } else if (This->PpmFlags & PPM_C6) {
    IoCaptAddr.Dwords.Low |= IO_CAPT_LVL3;
  } else if (This->PpmFlags & PPM_C4) {
    IoCaptAddr.Dwords.Low |= IO_CAPT_LVL2;
  }

  //
  // Set the base CST address
  //
  IoCaptAddr.Dwords.Low &= ~(IO_CAPT_LVL2_BASE_ADDR_MASK);
  IoCaptAddr.Dwords.Low |= (UINT32)C2IoAddress;

  AsmWriteMsr64 (MSR_PMG_IO_CAPTURE_BASE, IoCaptAddr.Qword);

  if (This->PpmFlags & PPM_C4) {
    //
    // Set the L2 Way Chunk Size to 1/4 before enabling Deep C4.
    // From Core 8 Release 57
    //
    BblCrCtl3.Qword = AsmReadMsr64 (MSR_BBL_CR_CTL3);
    BblCrCtl3.Dwords.Low &= ~L2_WAY_CHUNK_SZ_MASK;
    BblCrCtl3.Dwords.Low |= L2_WAY_CHUNK_SZ_4;

    AsmWriteMsr64 (MSR_BBL_CR_CTL3, BblCrCtl3.Qword);
  }
  return;
}

/**
  This will perform general thermal initialization other then TM1, TM2, or
  PROCHOT# on all logical processors.

  @param[in]   This            Pointer to the protocol instance

  @retval      EFI_SUCCESS     General thermal initialization completed successfully

**/
STATIC
EFI_STATUS
EFIAPI
InitThermalCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  )
{
  MSR_REGISTER MsrVal;

  //
  // Run thermal code on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeInitThermal, PPMPolicy);

  // core thermal initialization

  MsrVal.Qword = AsmReadMsr64 (MSR_CPU_THERM_CFG1);
  MsrVal.Dwords.Low = 0x00000305;
  AsmWriteMsr64(MSR_CPU_THERM_CFG1, MsrVal.Qword);

  MsrVal.Qword = AsmReadMsr64 (MSR_CPU_THERM_CFG2);
  MsrVal.Dwords.Low = 0x0405500D;
  AsmWriteMsr64(MSR_CPU_THERM_CFG2, MsrVal.Qword);

  MsrVal.Qword = AsmReadMsr64 (MSR_CPU_THERM_SENS_CFG);
  MsrVal.Dwords.Low = 0x00000027;
  AsmWriteMsr64(MSR_CPU_THERM_SENS_CFG, MsrVal.Qword);

  MsrVal.Qword = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
  MsrVal.Dwords.Low = 0x00000505;
  AsmWriteMsr64(MSR_PACKAGE_POWER_SKU_UNIT, MsrVal.Qword);

  return EFI_SUCCESS;
}

/**
  This will perform general thermal initialization other then TM1, TM2, or PROCHOT#.
  This currently disables TM2 if enabled by default.  EnableTm2Cherryview will be called
  if TM2 is desired.
  
  @param[in] This          Pointer to the protocol instance
  @param[in] PpmFlags      Processor power management feature flags

  @retval  EFI_SUCCESS     General thermal initialization completed successfully

**/
VOID
EFIAPI
ApSafeInitThermal (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER                      Ia32MiscEnable;
  MSR_REGISTER                      EbcHardPoweron;
  PPM_PLATFORM_POLICY_PROTOCOL      *This;

  //
  // Extract parameters from the buffer
  //
  This = (PPM_PLATFORM_POLICY_PROTOCOL *) Buffer;

  //
  // Configure Adaptive thermal monitor. IA32_MISC_ENABLE[3]
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  Ia32MiscEnable.Dwords.Low &= ~TM_ENABLE;
  if (This->FunctionEnables.EnableTm) {
    Ia32MiscEnable.Dwords.Low |= TM_ENABLE;
  }

  AsmWriteMsr64 (MSR_IA32_MISC_ENABLES, Ia32MiscEnable.Qword);

  //
  // @NOTE: Inherit similar setting from CDV to VLV.
  // Set the Stop-Enable (STEN) bit for Mermom so internal clocks stop during
  // AutoHalt or Stop Clock states.
  //
  EbcHardPoweron.Qword = AsmReadMsr64 (EFI_MSR_EBC_HARD_POWERON);
  EbcHardPoweron.Qword |= (UINT64)STOP_ENABLE;
  AsmWriteMsr64 (EFI_MSR_EBC_HARD_POWERON, EbcHardPoweron.Qword);
  
  return;
}

/**
  Enable TM1

  @param[in]  This          Pointer to the protocol instance

  @retval   None

**/
STATIC
EFI_STATUS
EFIAPI
EnableTmCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{

  //
  // Enable TM1 on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeEnableTm, This);

  return EFI_SUCCESS;
}

/**
  This function will enable TM.
  This function must be MP safe.

  @param[in]   Buffer        Pointer to the function parameters passed in.
                (Pointer to the PPM_PROCESSOR_SUPPORT_PROTOCOL_2 instance.)

  @retval   EFI_SUCCESS      TM1 enabled

**/
VOID
EFIAPI
ApSafeEnableTm (
  IN OUT VOID        *Buffer
  )
{
  //
  // @IMPORTANT NOTES:
  // Enable Thermal Monitor features. Thermal throttling should take effect 
  // when operating thermal conditions are exceeded. TM2 style throttling is 
  // automatically engaged if GS3_EN is set. Otherwise, TM1 style throttling 
  // will be engaged. Thermal interrupts to the CPU should also be disabled 
  // if TM_EN is 0. Additionally, the Turbo range of operation should be 
  // disallowed when TM_EN=0.
  // TM_EN can only be set if FB_THERM_THROT_DIS_FUSE=0. 
  // 0 - Thermal throttling is disabled
  // 1 - Thermal throttling is enabled.
  //

  MSR_REGISTER                      Ia32MiscEnable;

  //
  // Enable TM1 in the CPU MSR
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (EFI_MSR_IA32_MISC_ENABLE);
  Ia32MiscEnable.Qword |= (UINT64)TM_ENABLE;
  AsmWriteMsr64 (EFI_MSR_IA32_MISC_ENABLE, Ia32MiscEnable.Qword);

  return;
}

/**
  Enables the bi-directional PROCHOT# signal on all logical processors.

  @param[in]  This          Pointer to the protocol instance

  @retval    EFI_SUCCESS    PROCHOT# configured successfully

**/
STATIC
EFI_STATUS
EFIAPI
EnableProcHotCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  MSR_REGISTER  CpuThermCfg2;
  //
  // Enable PROCHOT# in the CPU MSR if TM is enabled,
  // else disable it.
  //
  CpuThermCfg2.Qword = AsmReadMsr64 (MSR_CPU_THERM_CFG2);
  if (This->PpmFlags & (PPM_TM)) {
    CpuThermCfg2.Qword |= (UINT64)PHOT_ENABLE;
  } else {
    //
    // Clear the PPM_PROC_HOT flag so that the correct state is reflected.
    //
    This->PpmFlags &= ~PPM_PROC_HOT;
    CpuThermCfg2.Qword &= (~(UINT64)PHOT_ENABLE);
  }

  AsmWriteMsr64 (MSR_CPU_THERM_CFG2, CpuThermCfg2.Qword);

  return EFI_SUCCESS;
}

/**
  Enables GV3 support in a logical processor.

  This function must be MP safe.

  @param[in]  Buffer      Pointer to the function parameters passed in.
                   (Pointer to the PPM_PROCESSOR_SUPPORT_PROTOCOL_2 instance.)

  @retval   EFI_SUCCESS

**/
VOID
EFIAPI
ApSafeDisableGv3 (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER                        Ia32MiscEnable;
  
  //
  // Enable GV3 in the CPU MSR
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  Ia32MiscEnable.Qword &= (~(UINT64)GV3_ENABLE);
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLES, Ia32MiscEnable.Qword);

  return;
}

/**
  Set processor P state to HFM or LFM.

  @param[in]   Buffer       Unused

  @retval   EFI_SUCCESS     Processor MSR setting is saved.

**/
VOID
EFIAPI
ApSafeSetBootPState (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER	Ia32PerfCtl;

  Ia32PerfCtl.Qword = AsmReadMsr64 (MSR_IA32_PERF_CTL);
  Ia32PerfCtl.Qword &= (~(UINT64)P_STATE_TARGET_MASK);
  if ( mPpmProcessorSupportProtocolCherryview.PpmFlags & PPM_BOOT_P_ST_HFM ) {
    // Set to HFM
    Ia32PerfCtl.Qword |= LShiftU64 (mMaxBusRatio, P_STATE_TARGET_OFFSET);
    Ia32PerfCtl.Qword |= (UINT64)mMaxVid, 0xFF;
  } else {
    // Set to LFM
    Ia32PerfCtl.Qword |= LShiftU64 (mMinBusRatio, P_STATE_TARGET_OFFSET);
    Ia32PerfCtl.Qword |= (UINT64)mMinVid, 0xFF;
  }
  AsmWriteMsr64 (MSR_IA32_PERF_CTL, Ia32PerfCtl.Qword);
  return;
}

/**
  Set processor P state to HFM or LFM.

  @param[in]  This          Pointer to the protocol instance

  @retval  EFI_UNSUPPORTED  EIST not supported.
  @retval  EFI_SUCCESS      Processor P state has been set.

**/
STATIC
EFI_STATUS
EFIAPI
SetBootPState (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  MSR_REGISTER	Ia32MiscEnable;
  BOOLEAN       EistEnabled;

  //
  // This function will be executed when EIST is enabled and EIST is capable
  // So processor can be switched to HFM
  //
  if ((mCpuid01.RegEcx & CPUXFF_GV3) == 0) {
    return EFI_UNSUPPORTED;
  }

  //
  // Enable EIST
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  EistEnabled = (BOOLEAN)RShiftU64((Ia32MiscEnable.Qword & GV3_ENABLE),GV3_ENABLE_OFFSET);
  //
  // Check whether EIST is disabled in setup.
  //
  if ((This->PpmFlags & PPM_GV3) == 0)  {
    EistEnabled = 0;
  }
  //
  // If EIST is disabled, temporarily enable it
  //
  if (EistEnabled == 0) {
    RunOnAllLogicalProcessors (ApSafeEnableGv3, This);
  }

  //
  // Set P-state to HFM on all cores
  //
  RunOnAllLogicalProcessors (ApSafeSetBootPState, This);

  //
  // Disable EIST if we enabled it previously
  //
  if (EistEnabled == 0) {
    RunOnAllLogicalProcessors (ApSafeDisableGv3, This);
  }

  return EFI_SUCCESS;
}

/**
  Initialize the TSC support.

  @param[in, out] This                             Pointer to the protocol instance
  @param[in, out] FvidPointer                      Pointer to a table to be updated
  @param[in] iFSBFrequency            			   Processor core iFSB Frequency
  @param[in, out] PpmCstTmrFlags                   Flag of update TSC MSR if C3 or C4  
  @param[in, out] PpmTscCorrFactor                 TSC correction factor 
  @param[in, out] PpmTscCorrFactorRem              TSC correction factor remainder

  @retval     EFI_SUCCESS                          Processor TSC support configured successfully.

**/
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
  )
{
  UINT64              FrequencyId;

  //
  // Initialize the TSC update variables for Cherryview
  //
  *PpmCstTmrFlags = CST_UPDT_TSC;

  //
  // Initialize the FVID tables, so that the maximum ratio setting is identified.
  //
  InitFvidTableCherryview (This, FvidPointer, FVID_MAX_STATES, FVID_MIN_STEP_SIZE, FALSE);
  ASSERT (FvidPointer->FvidHeader.Gv3States != 0);

  //
  // Get the maximum frequency.
  //

  FrequencyId = mMaxBusRatio;
  //
  // Direct multiply Core IFSB with Max Ratio
  FrequencyId = MultU64x32 (FrequencyId, iFSBFrequency);

  //
  // Divide by timer base frequency
  // Save value and remainder
  //
  *PpmTscCorrFactor = (UINTN) DivU64x32Remainder (FrequencyId, CST_DATA_TBASE, (UINT32 *)PpmTscCorrFactorRem);

  return EFI_SUCCESS;
}

/**
  Save processor MSR runtime settings for S3.

  @param[in]  This          Pointer to the protocol instance

  @retval    EFI_SUCCESS   Processor MSR setting is saved.

**/
STATIC
EFI_STATUS
EFIAPI
S3SaveMsrCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  UINT32   Index;
  
  for (Index = 0; Index < sizeof (mMsrValues) / sizeof (EFI_MSR_VALUES); Index++) {
    mMsrValues[Index].Value = AsmReadMsr64 (mMsrValues[Index].Index);
  }

  return  EFI_SUCCESS;
}

/**
  Restore processor MSR runtime settings for S3.

  @param[in]  This          Pointer to the protocol instance

  @retval   EFI_SUCCESS     Processor MSR setting is restored.

**/
STATIC
EFI_STATUS
EFIAPI
S3RestoreMsrCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  //
  // Restore MSR's on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeRestoreMsr, This);

  return EFI_SUCCESS;
}

/**
  This function will restore MSR settings.

  This function must be MP safe.

  @param[in]  Buffer        Unused

  @retval    EFI_SUCCESS   MSR restored

**/
VOID
EFIAPI
ApSafeRestoreMsr (
  IN OUT VOID        *Buffer
  )
{
  UINT32  Index;

  for (Index = 0; Index < sizeof (mMsrValues) / sizeof (EFI_MSR_VALUES); Index++) {
    AsmWriteMsr64 (mMsrValues[Index].Index, mMsrValues[Index].Value);
  }

  AsmWriteMsr64 (MSR_POWER_MISC, AsmReadMsr64(MSR_POWER_MISC) | ENABLE_IA_UNTRUSTED_MODE);
  return;
}

/**
  Enables IO redirection C-State control on all logical processors.
  This should only be enabled if the OS and processor driver support
  independent C-State control for C2 and higher.

  This will cause the processor to capture LVL_X reads and perform the requested
  C-State transition.

  This cannot be enabled when SMI based C-State coordination is enabled, 
  so this function will disable SMI based C-State coordination.

  @param[in]  This          Pointer to the protocol instance

  @retval    EFI_SUCCESS   Processor IO redirection C-State control enabled.

**/
STATIC
EFI_STATUS
EFIAPI
EnableCStateIoRedirectionCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  //
  // Enable C-State I/O redirection on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeEnableCStateIoRedirection, This);

  return EFI_SUCCESS;
}

/**
  Enables C-State I/O redirection on a logical processor.

  This function must be MP safe.

  @param[in]  Buffer        Unused

  @retval   EFI_SUCCESS   Processor IO redirection C-State control enabled.

**/
VOID
EFIAPI
ApSafeEnableCStateIoRedirection (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER    PmCfgCtrl;

  //
  // Enable I/O redirection control
  //
  PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
  PmCfgCtrl.Dwords.Low |= MWAIT_IO_REDIR;
  AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);

  return ;
}

/**
  Disables C-State IO redirection on all logical processors.

  @param[in]  This         Pointer to the protocol instance

  @retval    EFI_SUCCESS   Processor IO redirection C-State control disabled.

**/
STATIC
EFI_STATUS
EFIAPI
DisableCStateIoRedirectionCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  //
  // Disable C-State I/O redirection on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeDisableCStateIoRedirection, This);

  return EFI_SUCCESS;
}

/**
  Disables C-State IO redirection on a logical processor.

  This function must be MP safe.

  @param[in]  Buffer        Unused

  @retval  EFI_SUCCESS   Processor IO redirection C-State control disabled.

**/
VOID
EFIAPI
ApSafeDisableCStateIoRedirection (
  IN OUT VOID        *Buffer
  )
{
  MSR_REGISTER    PmCfgCtrl;

  //
  // Disable I/O redirection C-State control
  //
  PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
  PmCfgCtrl.Dwords.Low &= ~MWAIT_IO_REDIR;
  AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);

  return;
}

/**
  Disable Turbo Mode at package level

  @param[in]  This          Pointer to the protocol instance
  @param[in]  PPMPolicy     Pointer to policy protocol instance

  @retval  None

**/
VOID
EFIAPI
ApSafeDisableTurboMode (
  IN OUT VOID      *Buffer
  )
{
  MSR_REGISTER    Ia32MiscEnableMsr;

  //
  // Set Turbo Mode disable bit in IA32 Misc Enable MSR
  //
  Ia32MiscEnableMsr.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  Ia32MiscEnableMsr.Dwords.High |= DISABLE_MASTER_TURBO_MODE;
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLES, Ia32MiscEnableMsr.Qword);
}

/**
  Enables Turbo Mode at package level

  @param[in] This          Pointer to the protocol instance
  @param[in] PPMPolicy     Pointer to policy protocol instance

  @retval  None

**/
VOID
EFIAPI
ApSafeEnableTurboMode (
  IN OUT VOID      *Buffer
  )
{
  MSR_REGISTER    Ia32MiscEnableMsr;
  //
  // Clear Turbo Mode disable bit in IA32 Misc Enable MSR
  //
  Ia32MiscEnableMsr.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  Ia32MiscEnableMsr.Dwords.High &= ~DISABLE_MASTER_TURBO_MODE;
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLES, Ia32MiscEnableMsr.Qword);

}

/**
  Initializes required structures for P-State table creation and enables GV3
  support in the processor.
  
  @param[in, out]This          Pointer to the protocol instance
  @param[in, out]FvidPointer   Table to update, must be initialized.
  @param[in, out]PPMPolicy     Pointer to policy protocol instance

  @retval  EFI_SUCCESS

**/
STATIC 
EFI_STATUS 
EFIAPI
InitGv3Cherryview ( 
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,   
  IN OUT FVID_TABLE                         *FvidPointer,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL       *PPMPolicy
  )
{
  MSR_REGISTER    TempMsr;
  MSR_REGISTER    FlexRatioMsr;
  MSR_REGISTER    PlatformIdMsr;

  PlatformIdMsr.Qword = AsmReadMsr64 (EFI_MSR_IA32_PLATFORM_ID);
  //
  // If not fuse disabled, then the Max ratio is programmable.
  //
  if (!(PlatformIdMsr.Dwords.Low & RATIO_LOCKED) && (PPMPolicy->FlexRatioVid & 0x10000)) {
  	if ((PPMPolicy->FlexRatioVid & 0xFFFF) != 0) {
    FlexRatioMsr.Qword = PPMPolicy->FlexRatioVid;
    AsmWriteMsr64 (MSR_FLEX_RATIO, FlexRatioMsr.Qword);
    }
  }

  //
  // Test for Turbo Mode supported and initialize if true.
  //

  //
  // Start: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
  //
  if (mTurboModeNotAvailable==0) {
  //
  // End: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
  //
     if (This->PpmFlags & PPM_TURBO) {
       RunOnAllLogicalProcessors (ApSafeEnableTurboMode, This);
      } else {
       RunOnAllLogicalProcessors (ApSafeDisableTurboMode, This);
      }
  //
  // Start: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
  //
  }
  //
  // End: Workaround for sighting 4683480, 4683441 for Q8XA IVI SKU
  //

  //
  // Initialize the FVID tables.
  //
  InitFvidTableCherryview (This, FvidPointer, FVID_MAX_STATES, FVID_MIN_STEP_SIZE, FALSE);
  ASSERT (FvidPointer->FvidHeader.Gv3States != 0);

  //
  // Enable GV3 on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeEnableGv3, This);
  
  //
  // Program Primary Power Plane Current Limit's
  //
  if( PPMPolicy->PrimaryPlaneCurrentLimit != AUTO ) {
    TempMsr.Qword = AsmReadMsr64 (MSR_PRIMARY_PLANE_CURRENT_CONFIG);
    TempMsr.Dwords.Low &= ~CURRENT_LIMIT_MASK;
    TempMsr.Dwords.Low |= (UINT32)PPMPolicy->PrimaryPlaneCurrentLimit;
    AsmWriteMsr64 (MSR_PRIMARY_PLANE_CURRENT_CONFIG, TempMsr.Qword); 
  }
  return EFI_SUCCESS;
}

/**
  Enables GV3 support in a logical processor.
  This function must be MP safe.

  @param[in] Buffer      Pointer to the function parameters passed in.
              (Pointer to the PPM_PROCESSOR_SUPPORT_PROTOCOL_2 instance.)

  @retval  EFI_SUCCESS

**/
VOID
EFIAPI
ApSafeEnableGv3 (
  IN OUT VOID        *Buffer
  )
{
  PPM_PROCESSOR_SUPPORT_PROTOCOL_2    *This;
  MSR_REGISTER                        Ia32MiscEnable;
  MSR_REGISTER                        PmCfgCtrl;
  MSR_REGISTER                        PowerMisc;

  //
  // Extract parameters from the buffer.
  //
  This = (PPM_PROCESSOR_SUPPORT_PROTOCOL_2*) Buffer;
  
  //
  // Enable GV3 in the CPU MSR
  //
  Ia32MiscEnable.Qword = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  Ia32MiscEnable.Qword |= (UINT64)GV3_ENABLE;
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLES, Ia32MiscEnable.Qword);

  PowerMisc.Qword = AsmReadMsr64 (MSR_POWER_MISC);
  
  //
  // If CMP is disabled, disable hardware coordination.
  //
  if (!(This->PpmFlags & PPM_CMP)) {
    PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
    PmCfgCtrl.Qword |= (UINT64)HW_COORD_DIS;
    AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);
  }

   return;
}

/**
  Enables GV3 hardware coordination on all logical processors.

  @param[in]  This          Pointer to the protocol instance

  @retval   EFI_SUCCESS     Hardware coordination enabled.

**/
STATIC
EFI_STATUS
EFIAPI
EnablePStateHardwareCoordinationCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  //
  // Enable GV3 hardware coordination on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeEnablePStateHardwareCoordination, This);

  return EFI_SUCCESS;
}

/**
  Enable the hardware coordination of P-states.
  This function must be MP safe.

  @param[in]  Buffer        Unused

  @retval  EFI_SUCCESS   Hardware coordination enabled.

**/
VOID
EFIAPI
ApSafeEnablePStateHardwareCoordination (
  IN OUT VOID   *Buffer
  )
{
  MSR_REGISTER    PmCfgCtrl;
  MSR_REGISTER    PowerMisc;
  
  //
  // @NOTE: If Microcode SINGLE_PCTL BUG NOT FIXED
  //        EHalt should not be set if SINGLE_PCTL=1. 
  //        Each module is a domain. 
  //        OS Must do SW_ALL coordination between Modules to get V^2 power savings. 
  //        Otherwise will get  >=linear savings.
  //
  // @WORKAROUND: To disable hardware coordination by setting SINGLE_PCTL = 1.
  //              SINGLE_PCTL = 0 implies HW_ALL coordination. 
  //
  // @TODO: To remove this workaround when the bug is fixed.
  //

  PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
  PmCfgCtrl.Dwords.Low |= SINGLE_PCTL;  
  AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);
  
  PowerMisc.Qword = AsmReadMsr64 (MSR_POWER_MISC);
  PowerMisc.Qword &= (~(UINT64)(ENABLE_ULFM_AUTOCM_MASK | ENABLE_INDP_AUTOCM_MASK));
  AsmWriteMsr64 (MSR_POWER_MISC, PowerMisc.Qword);
  return;
}

/**
  Disable the hardware coordination of P-states.

  @param[in]  This        Pointer to the protocol instance

  @retval   EFI_SUCCESS   Hardware coordination disabled.

**/
STATIC
EFI_STATUS
EFIAPI
DisablePStateHardwareCoordinationCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This
  )
{
  //
  // Disable GV3 hardware coordination on all logical processors.
  //
  RunOnAllLogicalProcessors(ApSafeDisablePStateHardwareCoordination, This);

  return EFI_SUCCESS;
}

/**
  Disables GV3 hardware coordination on a logical processor.

  This function must be MP safe.

  @param[in]  Buffer        Unused

  @retval    EFI_SUCCESS   Hardware coordination disabled.

**/
VOID
EFIAPI
ApSafeDisablePStateHardwareCoordination (
  IN OUT VOID   *Buffer
  )
{
  MSR_REGISTER    PmCfgCtrl;

  //
  // Disable HW coordination
  //
  PmCfgCtrl.Qword = AsmReadMsr64 (MSR_PM_CFG_CTRL);
  PmCfgCtrl.Dwords.Low |= HW_COORD_DIS;
  AsmWriteMsr64 (MSR_PM_CFG_CTRL, PmCfgCtrl.Qword);

  return;
}

/**
  This function updates the table provided with the FVID data for the processor.
  If CreateDefaultTable is TRUE, a minimam FVID table will be provided.
  The maximum number of states must be greater then or equal to two.
  The table should be initialized in such a way as for the caller to determine if the
  table was updated successfully.  This function should be deprecated in the future when
  Release 8 is integrated in favor of the EIST protocol calculating FVID information.
  
  @param[in] This                Pointer to the protocol instance
  @param[in] FvidPointer         Pointer to a table to be updated
  @param[in] MaxNumberOfStates   Number of entries in the table pointed to by FvidPointer
  @param[in] MinStepSize         Minimum step size for generating the FVID table
  @param[in] CreateDefaultTable  Create default FVID table rather then full state support

  @retval  EFI_SUCCESS     Table pointed to FvidPointer updated with desired FVID states

**/
STATIC
EFI_STATUS
InitFvidTableCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE                         *FvidPointer,
  IN     UINT16                             MaxNumberOfStates,
  IN     UINT16                             MinStepSize,
  IN     BOOLEAN                            CreateDefaultTable
  )
{
  EFI_STATUS    Status;
  MSR_REGISTER  PlatformIdMsr;
  MSR_REGISTER  IaCoreRatios;
  MSR_REGISTER  IaCoreVids;
  MSR_REGISTER  ClockFlexMax;

  //
  // If the FVID tables have already been created, return.
  //
  if (FvidPointer[0].FvidHeader.Gv3States != 0) {
    return EFI_SUCCESS;
  }
  PlatformIdMsr.Qword = AsmReadMsr64 (EFI_MSR_IA32_PLATFORM_ID);
  
  IaCoreRatios.Qword = AsmReadMsr64 (MSR_IACORE_RATIOS);
  mMaxBusRatio  = IaCoreRatios.Bytes.ThirdByte;

  //
  // Get Maximum Efficiency bus ratio (LFM) from IACORE_RATIOS MSR Bits[15:8]
  //
  mMinBusRatio = IaCoreRatios.Bytes.SecondByte; 
  
  //
  // Get the Vid for the processor
  //
  //
  // Get Maximum Non-Turbo Vid (HFM) from IACORE_VIDS MSR Bits[23:16]
  //
  IaCoreVids.Qword = AsmReadMsr64 (MSR_IACORE_VIDS);
  mMaxVid  = IaCoreVids.Bytes.ThirdByte;

  //
  // Get Maximum Efficiency VID (LFM) from IACORE_VIDS MSR Bits[15:8]
  //
  mMinVid = IaCoreVids.Bytes.SecondByte;

  //
  // If flex settings are enabled, they must be used in place of max. Ratio/VID
  // defined in the MSR_PLATFORM_ID MSR used above.  In addition, if flex settings are
  // enabled, the maximum frequency is limited an Turbo Mode is unavailable.
  //
  // NOTE: This code does not provide facilities to program the flex settings
  // as such code would best be suited for the PEI phase, as a processor
  // reset may be required after programming.
  //
  if (!(PlatformIdMsr.Dwords.Low & RATIO_LOCKED)) {
    ClockFlexMax.Qword = AsmReadMsr64 (MSR_FLEX_RATIO);
    if (ClockFlexMax.Dwords.Low & ENABLE_FLEX) {
      This->PpmFlags &= ~(PPM_TURBO); 
      mMaxVid = ClockFlexMax.Bytes.FirstByte & VID_FLEX_BYTE_MASK;
      mMaxBusRatio = ClockFlexMax.Bytes.SecondByte & RATIO_FLEX_BYTE_MASK;
    }
  }
  
  //
  // Create FVID table
  //
  if (CreateDefaultTable) {
    CreateDefaultFvidTable (FvidPointer);
    This->PpmFlags &= ~(PPM_TURBO | PPM_DYNAMIC_FSB);
  } else {
    Status = CreateFvidTable (This, FvidPointer, MaxNumberOfStates, MinStepSize);
    if (EFI_ERROR (Status)) {
      CreateDefaultFvidTable (FvidPointer);
      This->PpmFlags &= ~(PPM_TURBO | PPM_DYNAMIC_FSB);
    }
  }

  return EFI_SUCCESS;
}

/**
  Create default FVID table with max and min states only.

  @param[in]  FvidPointer         Pointer to a table to be updated

  @retval   None.

**/  
STATIC
VOID
CreateDefaultFvidTable (
  IN OUT FVID_TABLE     *FvidPointer
  )
{

  //
  // Fill in the header.
  //
  FvidPointer[0].FvidHeader.Stepping     = (mCpuid01.RegEax & CPUID_FULL_FAMILY_MODEL_STEPPING);
  FvidPointer[0].FvidHeader.MaxVid       = mMaxVid;
  FvidPointer[0].FvidHeader.MaxBusRatio  = mMaxBusRatio;
  FvidPointer[0].FvidHeader.Gv3States    = 2;
  
  //
  // First entry is state 0, highest state.
  //
  FvidPointer[1].FvidState.State     = 0;
  FvidPointer[1].FvidState.Vid       = mMaxVid;
  FvidPointer[1].FvidState.BusRatio  = mMaxBusRatio;
  FvidPointer[1].FvidState.Power     = FVID_MAX_POWER_MIDVIEW;
  //
  // power is calculated in milliwatts
  //
  
  //
  // Second entry is state 1, lowest state. 
  //
  FvidPointer[2].FvidState.State     = 1;
  FvidPointer[2].FvidState.Vid       = (UINT16) mMinVid;
  FvidPointer[2].FvidState.BusRatio  = (UINT16) mMinBusRatio;
  FvidPointer[2].FvidState.Power     = FVID_MIN_POWER_MIDVIEW;
}

/**
  Create an FVID table based on the algorithm provided by the BIOS writer's guide.

  @param[in, out]This                Pointer to the protocol instance
  @param[in, out]FvidPointer         Pointer to a table to be updated
  @param[in, out]MaxNumberOfStates   Number of entries in the table pointed to by FvidPointer
  @param[in, out]MinStepSize         Minimum step size for generating the FVID table

  @retval  EFI_SUCCESS               FVID table created successfully.
  @retval  EFI_INVALID_PARAMETER     The VID and/or bus ratio ranges don't permit FVID table calculation;
                                     a default FVID table should be constructed.

**/
STATIC
EFI_STATUS
CreateFvidTable (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT FVID_TABLE     *FvidPointer,
  IN     UINT16         MaxNumberOfStates,
  IN     UINT16         MinStepSize
  )
{
  UINT16                BusRatioRange;
  UINT16                PowerRange;
  UINT16                StepSize;
  UINT16                NumberOfStates;
  UINT16                CurrentBusRatio;
  UINT16                i;
  UINT16                EntryCount;
  UINT16                Turbo;
  UINT16                BusRatioRangeX2;
  UINT16                VidRange;

  DEBUG((EFI_D_ERROR,"\n\n   ==  ==  CreateFvidTable  ==  ==\n\n"));

  //
  // Determine the bus ratio range
  //
  Turbo = 0;
  BusRatioRange =  mMaxBusRatio - mMinBusRatio;

  if (This->PpmFlags & PPM_TURBO) {
    Turbo = 1;
    BusRatioRange = mTurboBusRatio - mMinBusRatio;
  } 
  if ( ((INT16) BusRatioRange < 0) || ( MaxNumberOfStates == 0 )) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Determine step size desired
  // Step size is BusRatioRange / max states, rounded down to an even state
  //
  StepSize = 1;

  //
  // Determine the number of states
  // No Of States =  (BusRatioRange / step size) + 1; Add one to BusRatioRange to account for LFM
  // eg. LFM = 8 HFM = 10, BusRatioRange = 2, StepSize = 1 No Of States = (2 / 1) + 1 = 3 (8, 9, 10)
  // For No of States above 16...
  // StepSize = (No of States / Max No of States) + 1
  // eg. LFM = 8 HFM = 28, BusRatioRange = 20
  // StepSize = (21 / 16) + 1 = 2
  // No Of States = (20 / 2) + 1 = 11 (8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28)
  //
  NumberOfStates = ( BusRatioRange / StepSize ) + 1;
  DEBUG((EFI_D_ERROR, "NumberOfStates:[%04x]\n", NumberOfStates));

  //
  // Create a bus ratio range multiplied by 2 to take N/2 into account in calculations.
  //
  BusRatioRangeX2 = BusRatioRange * 2;

  //
  // Determine the Power range
  //
  PowerRange = FVID_MAX_POWER_MIDVIEW - FVID_MIN_POWER_MIDVIEW;

  //
  // Fill in the table header
  //

  FvidPointer[0].FvidHeader.Stepping     = (Turbo ? (mTurboBusRatio - mMaxBusRatio):1);
  FvidPointer[0].FvidHeader.MaxVid       = (Turbo ? mTurboVid : mMaxVid);
  FvidPointer[0].FvidHeader.MaxBusRatio  = (Turbo ? mTurboBusRatio : mMaxBusRatio);
  FvidPointer[0].FvidHeader.Gv3States    = (UINT16) (NumberOfStates);

  DEBUG ((EFI_D_ERROR, "FVID[00].FvidHeader.Stepping = %x\n", FvidPointer[0].FvidHeader.Stepping));
  DEBUG ((EFI_D_ERROR, "FVID[00].FvidHeader.MaxVid = %x\n", FvidPointer[0].FvidHeader.MaxVid));
  DEBUG ((EFI_D_ERROR, "FVID[00].FvidHeader.MaxBusRatio = %x\n", FvidPointer[0].FvidHeader.MaxBusRatio));
  DEBUG ((EFI_D_ERROR, "FVID[00].FvidHeader.Gv3States = %x\n", FvidPointer[0].FvidHeader.Gv3States));

  //
  // Fill in the first state
  //
  FvidPointer[1].FvidState.State		 = 0;
  FvidPointer[1].FvidState.Vid			 = mMaxVid;
  FvidPointer[1].FvidState.BusRatio  = mMaxBusRatio;
  //
  // power is calculated in milliwatts
  //
  FvidPointer[1].FvidState.Power		 = FVID_MAX_POWER_MIDVIEW;
  EntryCount = 1;
  if (This->PpmFlags & PPM_TURBO) {
    //
    // Fill in the first state
    //
    FvidPointer[1].FvidState.State		 = 0;
    FvidPointer[1].FvidState.Vid		 = mTurboVid;
    FvidPointer[1].FvidState.BusRatio  = mTurboBusRatio;

    DEBUG ((EFI_D_ERROR, "FVID[01].State = %x\n", FvidPointer[1].FvidState.State));
    DEBUG ((EFI_D_ERROR, "FVID[01].BusRatio = %x\n", FvidPointer[1].FvidState.BusRatio));
    DEBUG ((EFI_D_ERROR, "FVID[01].vid = %x\n", FvidPointer[1].FvidState.Vid));
    DEBUG ((EFI_D_ERROR, "FVID[01].Power = %x\n\n", FvidPointer[1].FvidState.Power));

    DEBUG((EFI_D_ERROR,"==	==	CreateTurbo 1C to 3C	==	==\n"));

    BusRatioRange = mTurboBusRatio - mTurboBusRatio_3C;
    DEBUG((EFI_D_ERROR, "BusRatioRange = %x\n", BusRatioRange));

    if (BusRatioRange != 0) {
      VidRange = mTurboVid - mTurboVid_3C;
      BusRatioRangeX2 = BusRatioRange * 2;

      CurrentBusRatio = mTurboBusRatio;
      for (i = 1; i < BusRatioRange + 1; i++) {
        FvidPointer[i + 1].FvidState.State = i;
        FvidPointer[i + 1].FvidState.BusRatio = CurrentBusRatio - StepSize;

        CurrentBusRatio = CurrentBusRatio - StepSize;

        if (BusRatioRange != 0) {
          FvidPointer[i + 1].FvidState.Vid = ((CurrentBusRatio - mTurboBusRatio_3C) * VidRange * 2) / BusRatioRangeX2 + mTurboVid_3C;
        } else {
          FvidPointer[i + 1].FvidState.Vid = mTurboVid_3C;
        }
        if (((CurrentBusRatio - mTurboBusRatio_3C) * VidRange * 2) % BusRatioRangeX2) {
          //
          // Round up if there is a remainder to remain above the minimum voltage
          //
          FvidPointer[i + 1].FvidState.Vid++;
        }

        FvidPointer[i + 1].FvidState.Power 		= FVID_MAX_POWER_MIDVIEW;

        DEBUG ((EFI_D_ERROR, "FVID[%02d].State = %x\n", (i + 1), FvidPointer[i + 1].FvidState.State));
        DEBUG ((EFI_D_ERROR, "FVID[%02d].BusRatio = %x\n", (i + 1), FvidPointer[i + 1].FvidState.BusRatio ));
        DEBUG ((EFI_D_ERROR, "FVID[%02d].Vid = %x\n", (i + 1), FvidPointer[i + 1].FvidState.Vid ));
        DEBUG( (EFI_D_ERROR, "FVID[%02d].Power = %x\n\n", (i + 1), FvidPointer[i + 1].FvidState.Power));
      }
      EntryCount = i;
    }
    //
    // Determine the VID range
    // Do not check for VID range to be zero because some of the ULV and LV processors
    // will have the same HFM and LFM VID values
    //

    VidRange = mTurboVid_3C - mMaxVid;
    BusRatioRange = mTurboBusRatio_3C - mMaxBusRatio;
    DEBUG((EFI_D_ERROR, "BusRatioRange = %x\n", BusRatioRange));

    BusRatioRangeX2 = BusRatioRange * 2;

    //
    // Fill in the table starting at the last entry
    // The algorithm is available in the processor BIOS writer's guide.
    //
    CurrentBusRatio = mTurboBusRatio_3C;
    DEBUG((EFI_D_ERROR,"==	==	CreateTurbo 3C to MaxFID	==	==\n"));

    for (i = EntryCount; i <= (mTurboBusRatio - mMaxBusRatio); i++) {
      FvidPointer[i + 1].FvidState.State     = i;
      FvidPointer[i + 1].FvidState.BusRatio  = CurrentBusRatio - StepSize;

      CurrentBusRatio = CurrentBusRatio - StepSize;

      if (BusRatioRange != 0) {
        FvidPointer[i + 1].FvidState.Vid       = ((CurrentBusRatio - mMaxBusRatio) * VidRange * 2) / BusRatioRangeX2 + mMaxVid;
      } else {
        FvidPointer[i + 1].FvidState.Vid       = mMaxVid;
      }

      if (((CurrentBusRatio - mMaxBusRatio) * VidRange * 2) % BusRatioRangeX2) {
      //
      // Round up if there is a remainder to remain above the minimum voltage
      //
        FvidPointer[i + 1].FvidState.Vid++;
      }

      FvidPointer[i + 1].FvidState.Power     = FVID_MAX_POWER_MIDVIEW;

      DEBUG ((EFI_D_ERROR, "FVID[%02d].State = %x\n", (i + 1), FvidPointer[i + 1].FvidState.State));
      DEBUG ((EFI_D_ERROR, "FVID[%02d].BusRatio = %x\n", (i + 1), FvidPointer[i + 1].FvidState.BusRatio ));
      DEBUG ((EFI_D_ERROR, "FVID[%02d].Vid = %x\n", (i + 1), FvidPointer[i + 1].FvidState.Vid ));
      DEBUG( (EFI_D_ERROR, "FVID[%02d].Power = %x\n\n", (i + 1), FvidPointer[i + 1].FvidState.Power));
    }
    EntryCount = i;
  }

  DEBUG((EFI_D_ERROR,"==	==	CreateTurbo MaxFID to MinFID	==	==\n"));
  VidRange = mMaxVid - mMinVid;
  BusRatioRange = mMaxBusRatio - mMinBusRatio;
  BusRatioRangeX2 = BusRatioRange * 2;

  //
  // Determine the Power range
  //
  PowerRange = FVID_MAX_POWER_MIDVIEW - FVID_MIN_POWER_MIDVIEW;
  CurrentBusRatio = mMaxBusRatio;

  for (i = EntryCount; i < NumberOfStates; i++) {

    FvidPointer[i + 1].FvidState.State     = i;
    FvidPointer[i + 1].FvidState.BusRatio  = CurrentBusRatio - StepSize;

    CurrentBusRatio = CurrentBusRatio - StepSize;

    if (BusRatioRange != 0) {
      FvidPointer[i + 1].FvidState.Vid       = ((CurrentBusRatio - mMinBusRatio) * VidRange * 2) / BusRatioRangeX2 + mMinVid;
    } else {
      FvidPointer[i + 1].FvidState.Vid       = mMinVid;
    }

    if (((CurrentBusRatio - mMinBusRatio) * VidRange * 2) % BusRatioRangeX2) {
      //
      // Round up if there is a remainder to remain above the minimum voltage
      //
      FvidPointer[i + 1].FvidState.Vid++;
    }

    if (BusRatioRange != 0) {
      FvidPointer[i + 1].FvidState.Power     = ((CurrentBusRatio - mMinBusRatio) * PowerRange * 2) / BusRatioRangeX2 + FVID_MIN_POWER_MIDVIEW;
    } else {
      FvidPointer[i + 1].FvidState.Power     = FVID_MIN_POWER_MIDVIEW;
    }

    DEBUG ((EFI_D_ERROR, "FVID[%02d].State = %x\n", (i + 1), FvidPointer[i + 1].FvidState.State));
    DEBUG ((EFI_D_ERROR, "FVID[%02d].BusRatio = %x\n", (i + 1), FvidPointer[i + 1].FvidState.BusRatio ));
    DEBUG ((EFI_D_ERROR, "FVID[%02d].Vid = %x\n", (i + 1), FvidPointer[i + 1].FvidState.Vid ));
    DEBUG( (EFI_D_ERROR, "FVID[%02d].Power = %x\n\n", (i + 1), FvidPointer[i + 1].FvidState.Power));
  }

  return EFI_SUCCESS;
}

/**
  Private helper function to convert various Turbo Power Limit Time from Seconds to CPU units

  @param[in]   TimeInSeconds     Time in seconds

  @retval   Converted time in CPU units

**/
UINT8 GetConvertedTime(
  IN UINT32 TimeInSeconds
  )
{
  UINT8         ConvertedPowerLimitTime;
  UINT8         Index;

  //
  // Convert seconds to MSR value. Since not all values are programmable, we'll select
  // the entry from mapping table which is either equal to the user selected value. OR to a value in the mapping table
  // which is closest (but less than) to the user-selected value.
  //
  ConvertedPowerLimitTime = mSecondsToMsrValueMapTable[0][1];

  for (Index = 0; mSecondsToMsrValueMapTable[Index][0] != END_OF_TABLE; Index++) {
    if (TimeInSeconds == mSecondsToMsrValueMapTable[Index][0]) {
      ConvertedPowerLimitTime = mSecondsToMsrValueMapTable[Index][1];
      break;
    }

    if (TimeInSeconds > mSecondsToMsrValueMapTable[Index][0]) {
      ConvertedPowerLimitTime = mSecondsToMsrValueMapTable[Index][1];
    } else {
      break;
      break;
    }
  }
  return ConvertedPowerLimitTime;  
}

/**
  Get the updated power configuration register values

  @param[in]   None

  @retval  None

**/
VOID
UpdateCurrentPowerInfo (
  )
{

  MSR_REGISTER        TempMsr;

  TempMsr.Qword        = AsmReadMsr64 (MSR_PLATFORM_INFO);
  //
  // Check if TDP Limit is programmable
  // Platform Info MSR (0xCE) [29]
  //
  mTdpLimitProgrammble = 1;

  TempMsr.Qword        = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
  //
  // Get Power Unit MSR [3:0]
  // The actual unit value is calculated by 1mW*Power(2,POWER_UNIT)..Reset value of 5 represents 32mW units.
  //
  mProcessorPowerUnit  = (TempMsr.Bytes.FirstByte & PACKAGE_POWER_UNIT_MASK);

  if (mProcessorPowerUnit == 0) {
    mProcessorPowerUnit = 1;
  } else {
    mProcessorPowerUnit = (UINT8) LShiftU64 (2, (mProcessorPowerUnit - 1));
  }
  // @Note: This field should be initialized by PUnit to IACORE_GUAR_TDP_FUSE + SOC_GUAR_TDP_FUSE
  //            Get the pre-si estimated settings for the Turbo from Punit. 
  mPackageTdp      = 0x1F;
  mPackageMaxPower = 0xFF;
  mPackageMinPower = 0x3F;

  return;

}

/**
  Runs the specified procedure on all logical processors, passing in the
  parameter buffer to the procedure.
  
  @param[in]Procedure      The function to be run.
  @param[in] Buffer        Pointer to a parameter buffer.

  @retval   None

**/
STATIC
EFI_STATUS
RunOnAllLogicalProcessors (
  IN OUT EFI_AP_PROCEDURE   Procedure,
  IN OUT VOID               *Buffer
  )
{
  UINTN      Index;
  EFI_STATUS Status;
  //
  // Run the procedure on all logical processors.
  //
  (*Procedure) (Buffer);
  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    //
    // Skip BSP itself. 
    //
    if (Index == gSmst->CurrentlyExecutingCpu) {
      continue;
    }
    
    Status = EFI_NOT_READY;
    while (Status != EFI_SUCCESS ) {
      Status = gSmst->SmmStartupThisAp (Procedure, Index, Buffer);
      if ( Status != EFI_SUCCESS ) {
        //
        // SmmStartupThisAp might return failure if AP is busy executing some other code. Let's wait for sometime and try again.
        //
        //PchPmTimerStall (PPM_WAIT_PERIOD);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Configures following fields of MSR 0x610 based on user configuration:
  Configures Long duration Turbo Mode (power limit 1) power level and time window 
  Configures Short duration turbo mode (power limit 2) 

  @param[in]  This          Pointer to the protocol instance
  @param[in]  PPMPolicy     Pointer to policy protocol instance

  @retval  None

**/
STATIC
VOID
EFIAPI
ConfigureTurboPowerLimitsCherryview (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   *This,
  IN OUT PPM_PLATFORM_POLICY_PROTOCOL     *PPMPolicy
  )
{
// @NOTE: This may be redundant in BIOS boot flow as P-unit will intialize this MSR with Package TDP.
  MSR_REGISTER        PKGTurboCfg1Msr;
  MSR_REGISTER        CPUTurboWkldCfg1Msr;
  MSR_REGISTER        CPUTurboWkldCfg2Msr;
  MSR_REGISTER        PP1TurboPowerLimitMsr;
  MSR_REGISTER        TurboPowerLimitMsr;
  UINT8               ConvertedPowerLimit1Time;
  UINT16              ProgramedPowerLimit1;
  UINT16              ConvertedPowerLimit2;
  UINT16              ProgramedPowerLimit2;
  UINT64              val;
  UINT32              Data32;
  UINT32              TdpOffset;

  //
  // Get the updated power congigurations as these values may be reconfigured by the Punit
  //
  UpdateCurrentPowerInfo ();
  if (mTdpLimitProgrammble) {
    TurboPowerLimitMsr.Qword  = AsmReadMsr64 (MSR_TURBO_POWER_LIMIT);
    //
    // Initialize the Power Limit 1 and Power Limit 1 enable bit
    // Power Limit 1: Turbo Power Limit MSR  [14:0]
    // Power Limit 1 Enable: Turbo Power Limit MSR  [15]
    //
    //
    // By default, program the value of IACORE_GUAR_TDP_FUSE + SOC_GUAR_TDP_FUSE to Package TDP limit
    //
    ProgramedPowerLimit1 = mPackageMinPower;
    if (PPMPolicy->TurboSettings.PowerLimit1 != AUTO) {
      //
      // mPackageMinPower, mPackageMaxPower and mPackageTdp are in watts.
      // they can directly compare to PPMPolicy->pTurboSettings->PowerLimit1
      //
      ProgramedPowerLimit1 = PPMPolicy->TurboSettings.PowerLimit1;
      if (PPMPolicy->TurboSettings.PowerLimit1 < mPackageMinPower) {
        //
        // If new Power Limit 1 is < mPackageMinPower, program Power Limit 1 to mPackageMinPower
        //
        ProgramedPowerLimit1 = mPackageMinPower;
      } else if ( PPMPolicy->TurboSettings.PowerLimit1 > mPackageMaxPower ) { 
        //
        // If new Power Limit 1 is not within bounds, program Power Limit 1 to Package TDP limit
        //
        ProgramedPowerLimit1 = mPackageMaxPower;
      }
      //
      // PPMPolicy->pTurboSettings->PowerLimit1 is in watts. We need to convert it to
      // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
      // Since we are converting from Watts to CPU power units, multiply by
      // PACKAGE_POWER_SKU_UNIT_MSR[3:0].
      //
    }

    TurboPowerLimitMsr.Dwords.Low &= ~POWER_LIMIT_MASK;
    TurboPowerLimitMsr.Dwords.Low |= (UINT32) (ProgramedPowerLimit1);

    //
    // Force Power Limit 1 override to be enabled
    //
    TurboPowerLimitMsr.Dwords.Low |= POWER_LIMIT_ENABLE;

    //
    // Program Power Limit 1 (Long Duration Turbo) Time Window 
    // If PowerLimit1Time is AUTO OR If PowerLimit1Time is > MAX_POWER_LIMIT_1_TIME_IN_SECONDS program default values
    //
    if ( ( PPMPolicy->TurboSettings.PowerLimit1Time == AUTO ) || ( PPMPolicy->TurboSettings.PowerLimit1Time > MAX_POWER_LIMIT_1_TIME_IN_SECONDS ) ) {
        //
        // For Mobile, default value is 5 seconds
        //
        PPMPolicy->TurboSettings.PowerLimit1Time = 5;
      
    } 
    ConvertedPowerLimit1Time = GetConvertedTime (PPMPolicy->TurboSettings.PowerLimit1Time);

    //
    //  Configure Power Limit 1 (Long Duration Turbo) time windows: Turbo Power Limit MSR [23:17]
    //
    TurboPowerLimitMsr.Dwords.Low &= ~POWER_LIMIT_1_TIME_MASK;
    TurboPowerLimitMsr.Dwords.Low |= (UINT32) LShiftU64(ConvertedPowerLimit1Time, 17);

    //
    // Initialize Short Duration Power limit and enable bit
    // Short duration Power Limit: Turbo Power Limit MSR (0x450h) [46:32]
    // Short duration Power Limit Enable:Turbo Power Limit MSR (0x450h) [47]
    //
    // PPMPolicy->pTurboSettings->PowerLimit2 value is in watts. We need to convert it to
    // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
    // Since we are converting from Watts to CPU power units, multiply by
    // PACKAGE_POWER_SKU_UNIT_MSR[3:0]
    //
    ProgramedPowerLimit2 = mPackageMaxPower;
    ConvertedPowerLimit2 = mPackageMaxPower;
    ConvertedPowerLimit2 = ConvertedPowerLimit2 * mProcessorPowerUnit;
    if (PPMPolicy->TurboSettings.PowerLimit2 != AUTO) {
      //
      // mPackageMinPower, mPackageMaxPower and mPackageTdp are in watts.
      // they can directly compare to PPMPolicy->pTurboSettings->PowerLimit1
      //
      ProgramedPowerLimit2 = PPMPolicy->TurboSettings.PowerLimit2;
      if (PPMPolicy->TurboSettings.PowerLimit2 < mPackageMinPower) {
        //
        // If new Power Limit 2 is < mPackageMinPower, program Power Limit 2 to mPackageMinPower
        //
        ProgramedPowerLimit2 = mPackageMinPower;
      } else if ( PPMPolicy->TurboSettings.PowerLimit2 > mPackageMaxPower ) { 
        //
        // If new Power Limit 2 is not within bounds, program Power Limit 2 to Package TDP limit
        //
        ProgramedPowerLimit2 = mPackageMaxPower;
      }
      //
      // PPMPolicy->pTurboSettings->PowerLimit2 is in watts. We need to convert it to
      // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
      // Since we are converting from Watts to CPU power units, multiply by
      // PACKAGE_POWER_SKU_UNIT_MSR[3:0].
      //
      ConvertedPowerLimit2 = (ProgramedPowerLimit2 * mProcessorPowerUnit);
    }
    TurboPowerLimitMsr.Dwords.High &= ~POWER_LIMIT_MASK;
    TurboPowerLimitMsr.Dwords.High |= (UINT32) (ProgramedPowerLimit2);
	// set PKG_PWR_LIM2_TIME = 0x0, 1s
	TurboPowerLimitMsr.Dwords.High &= ~POWER_LIMIT_1_TIME_MASK;
	// force Power Limit 2 override to be enabled
    TurboPowerLimitMsr.Dwords.High |= POWER_LIMIT_ENABLE;
  
    TurboPowerLimitMsr.Dwords.Low &= 0x0;
    val = AsmReadMsr64 (EFI_MSR_TURBO_POWER_LIMIT);
    Data32 = val & 0xFFF;
    if (Data32 == 0) {
      val = 0x003880BC;
    } else {
      TdpOffset = 0;
      val = 0x00388000 + Data32 + TdpOffset;
    }
    TurboPowerLimitMsr.Dwords.Low |= val;
    TurboPowerLimitMsr.Dwords.High &= 0x0;
    TurboPowerLimitMsr.Dwords.High |= 0x001481D5;	// BSW  : PL2 = 15W

    AsmWriteMsr64 (MSR_TURBO_POWER_LIMIT, TurboPowerLimitMsr.Qword);
    val =  AsmReadMsr64 (MSR_TURBO_POWER_LIMIT);
    DEBUG ((EFI_D_ERROR, "MSR_TURBO_POWER_LIMIT = %x\n", val));
  }

  // Configure PP1_TURBO_POWER_LIMIT [23:17]  = 0x0, 1 second time duration
  PP1TurboPowerLimitMsr.Qword = AsmReadMsr64 (MSR_PRIMARY_PLANE_TURBO_POWER_LIMIT);
  PP1TurboPowerLimitMsr.Dwords.Low &= ~PP_PWR_LIM_TIME_MASK;
  PP1TurboPowerLimitMsr.Dwords.Low |= PP_PWR_LIM_TIME;
  AsmWriteMsr64 (MSR_PRIMARY_PLANE_TURBO_POWER_LIMIT, PP1TurboPowerLimitMsr.Qword);

  // Configure PKG_TURBO_CFG1
  PKGTurboCfg1Msr.Qword = AsmReadMsr64 (MSR_PKG_TURBO_CFG1);
  // set Msr 0x670 = 0x00000002 (Dynamic Turbo Mode)
  PKGTurboCfg1Msr.Dwords.Low &= 0x0;
  PKGTurboCfg1Msr.Dwords.Low |= TURBOMODE | BIT1;
  AsmWriteMsr64 (MSR_PKG_TURBO_CFG1, PKGTurboCfg1Msr.Qword);

  // Configure CPU_TURBO_WKLD_CFG1
  CPUTurboWkldCfg1Msr.Qword = AsmReadMsr64 (MSR_CPU_TURBO_WKLD_CFG1);
  // set Msr 0x671 = 0x00001B14 (power meter weights)
  CPUTurboWkldCfg1Msr.Dwords.Low &= 0x0;
  CPUTurboWkldCfg1Msr.Dwords.Low |= 0x200B;
  AsmWriteMsr64 (MSR_CPU_TURBO_WKLD_CFG1, CPUTurboWkldCfg1Msr.Qword);

  // Configure CPU_TURBO_WKLD_CFG2
  CPUTurboWkldCfg2Msr.Qword = AsmReadMsr64 (MSR_CPU_TURBO_WKLD_CFG2);
  // set Msr 0x672 = 0x00000000
  CPUTurboWkldCfg2Msr.Dwords.Low &= 0x0;
  AsmWriteMsr64 (MSR_CPU_TURBO_WKLD_CFG2, CPUTurboWkldCfg2Msr.Qword);
}

