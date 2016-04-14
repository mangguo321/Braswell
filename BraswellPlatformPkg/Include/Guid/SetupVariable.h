/** @file
  Driver configuration include file.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SETUP_VARIABLE_H
#define _SETUP_VARIABLE_H

//
// ---------------------------------------------------------------------------
//
// Driver Configuration
//
// ---------------------------------------------------------------------------
//

//
// {EC87D643-EBA4-4bb5-A1E5-3F3E36B20DA9}
//
#define SYSTEM_CONFIGURATION_GUID\
  { \
    0xec87d643, 0xeba4, 0x4bb5, 0xa1, 0xe5, 0x3f, 0x3e, 0x36, 0xb2, 0xd, 0xa9 \
  }

#define ROOT_SECURITY_GUID\
  { \
    0xd387d688, 0xeba4, 0x45b5, 0xa1, 0xe5, 0x3f, 0x3e, 0x36, 0xb2, 0xd, 0x37 \
  }

//
// {6936B3BD-4350-46d9-8940-1FA20961AEB1}
//
#define SYSTEM_ROOT_MAIN_GUID\
  { \
     0x6936b3bd, 0x4350, 0x46d9, 0x89, 0x40, 0x1f, 0xa2, 0x9, 0x61, 0xae, 0xb1 \
  }

//
// {21FEE8DB-0D29-477e-B5A9-96EB343BA99C}
//
#define ADDITIONAL_SYSTEM_INFO_GUID\
  { \
     0x21fee8db, 0xd29, 0x477e, 0xb5, 0xa9, 0x96, 0xeb, 0x34, 0x3b, 0xa9, 0x9c \
  }

#define SETUP_GUID { 0xEC87D643, 0xEBA4, 0x4BB5, 0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9 }

#define MAX_CUSTOM_VID_TABLE_STATES 6
//
// Overclocking Source Defines
//
#define OVERCLOCK_SOURCE_BIOS       0
#define OVERCLOCK_SOURCE_OS         1

#define PCH_PCIE_MAX_ROOT_PORTS     4
#define PCH_HSIC_MAX_PORTS          2
#define PCH_SSIC_MAX_PORTS          2

#define EFI_CSM_CONTROL_ALWAYS_ON   0x1
#define EFI_CSM_CONTROL_ALWAYS_OFF  0x0

#pragma pack(1)

// NOTE: When you add anything to this structure,
//   you MUST add it to the very bottom!!!!
//   You must make sure the structure size is able to divide by 32!
typedef struct {
  //
  // System ports
  //
  UINT8         Serial;
  UINT8         SerialLockHide;

  UINT8         Serial2;
  UINT8         Serial2LockHide;

  UINT8         Parallel;
  UINT8         ParallelLockHide;

  UINT8         ParallelMode;
  UINT8         ParallelModeLockHide;

  UINT8         AllUsb;
  UINT8         UsbPortsLockHide;

  UINT8         Usb2;
  UINT8         Usb2LockHide;

  UINT8         UsbLegacy;
  UINT8         UsbLegacyLockHide;

  //
  // Keyboard
  //
  UINT8         Numlock;
  UINT8         NumlockLockHide;

  //
  // ECIR
  //
  UINT8         ECIR;
  UINT8         ECIRLockHide;

  //
  // Power State
  //
  UINT8         PowerState;
  UINT8         PowerStateLockHide;

  //
  // Wake on RTC variables
  //
  UINT8         WakeOnRtcS5;
  UINT8         WakeOnRtcS5LockHide;
  UINT8         RTCWakeupDate;
  UINT8         RTCWakeupDateLockHide;
  UINT8         RTCWakeupTimeHour;
  UINT8         RTCWakeupHourLockHide;
  UINT8         RTCWakeupTimeMinute;
  UINT8         RTCWakeupMinuteLockHide;
  UINT8         RTCWakeupTimeSecond;
  UINT8         RTCWakeupSecondLockHide;

  //
  // Video Adaptor
  //
  UINT8         PrimaryVideoAdaptor;
  UINT8         PrimaryVideoAdaptorLockHide;

  //
  // Hybrid Graphics
  //
  UINT16        DelayAfterPwrEn;
  UINT16        DelayAfterHoldReset;
  //
  // Chassis intrusion
  //
  UINT8         IntruderDetection;
  UINT8         IntruderDetectionLockHide;

  //
  // Maximum FSB Automatic/Disable
  //
  UINT8         MaxFsb;
  UINT8         MaxFsbLockHide;

  //
  // Hard Disk Pre-delay
  //
  UINT8         HddPredelay;
  UINT8         HddPredelayLockHide;

  //
  // S.M.A.R.T. Mode
  //
  UINT8         SmartMode;
  UINT8         SmartModeLockHide;

  //
  // ACPI Suspend State
  //
  UINT8         AcpiSuspendState;
  UINT8         AcpiSuspendStateLockHide;

  //
  // PCI Latency Timer
  //
  UINT8         PciLatency;
  UINT8         PciLatencyLockHide;

  //
  // Fan Control
  //
  UINT8         FanControl;
  UINT8         FanControlLockHide;

  //
  // CPU Fan Control
  //
  UINT8         CpuFanControl;
  UINT8         CpuFanControlLockHide;

  //
  // Lowest Fan Speed
  //
  UINT8         LowestFanSpeed;
  UINT8         LowestFanSpeedLockHide;

  //
  // Processor (CPU)
  //
  UINT8         CpuFlavor;

  UINT8         CpuidMaxValue;
  UINT8         CpuidMaxValueLockHide;

  UINT8         ExecuteDisableBit;
  UINT8         ExecuteDisableBitLockHide;

  //
  // EIST or GV3 setup option
  //
  UINT8         ProcessorEistEnable;
  UINT8         ProcessorEistEnableLockHide;

  //
  // C1E Enable
  //
  UINT8         ProcessorC1eEnable;
  UINT8         ProcessorC1eEnableLockHide;

  //
  // Enabling CPU C-States of processor
  //
  UINT8         ProcessorCcxEnable;
  UINT8         ProcessorCcxEnableLockHide;

  //
  // Package C-State Limit
  //
  UINT8         PackageCState;
  UINT8         PackageCStateLockHide;

  //
  // Enable/Disable NHM C3(ACPI C2) report to OS
  //
  UINT8         OSC2Report;
  UINT8         OSC2ReportLockHide;

  //
  // Enable/Disable NHM C6(ACPI C3) report to OS
  //
  UINT8         C6Enable;
  UINT8         C6EnableLockHide;

  //
  // Enable/Disable NHM C7(ACPI C3) report to OS
  //
  UINT8         C7Enable;
  UINT8         C7EnableLockHide;

  //
  // EIST/PSD Function select option
  //
  UINT8         ProcessorEistPsdFunc;
  UINT8         ProcessorEistPsdFuncLockHide;

  //
  // CPU Active Cores and SMT
  //
  UINT8         ActiveProcessorCores;
  UINT8         ActiveProcessorCoresLockHide;

  //
  // Hyper Threading
  //
  UINT8         ProcessorHyperThreadingDisable;
  UINT8         ProcessorHyperThreadingDisableLockHide;

  //
  // Enabling VMX
  //
  UINT8         ProcessorVmxEnable;
  UINT8         ProcessorVmxEnableLockHide;

  //
  // Enabling BIST
  //
  UINT8         ProcessorBistEnable;
  UINT8         ProcessorBistEnableLockHide;

  //
  // Disabling XTPR
  //
  UINT8         ProcessorxTPRDisable;
  UINT8         ProcessorxTPRDisableLockHide;

  //
  // Enabling XE
  //
  UINT8         ProcessorXEEnable;
  UINT8         ProcessorXEEnableLockHide;

  //
  // Fast String
  //
  UINT8         FastStringEnable;
  UINT8         FastStringEnableLockHide;

  //
  // Monitor/Mwait
  //
  UINT8         MonitorMwaitEnable;
  UINT8         MonitorMwaitEnableLockHide;

  //
  // Machine Check
  //
  UINT8         MachineCheckEnable;
  UINT8         MachineCheckEnableLockHide;

  //
  // Turbo mode
  //
  UINT8         TurboModeEnable;
  UINT8         TurboModeEnableLockHide;

  //
  // DCA setup option
  //
  UINT8         DcaEnable;
  UINT8         DcaEnableLockHide;

  //
  // DCA Prefetch Delay Value
  //
  UINT8         DcaPrefetchDelayValue;
  UINT8         DcaPrefetchDelayValueLockHide;

  //
  // Hardware Prefetch
  //
  UINT8         MlcStreamerPrefetcherEnable;
  UINT8         MlcStreamerPrefetcherEnableLockHide;

  //
  // Adjacent Cache Line Prefetch
  //
  UINT8         MlcSpatialPrefetcherEnable;
  UINT8         MlcSpatialPrefetcherEnableLockHide;

  //
  // DCU Streamer Prefetcher
  //
  UINT8         DCUStreamerPrefetcherEnable;
  UINT8         DCUStreamerPrefetcherEnableLockHide;

  //
  // DCU IP Prefetcher
  //
  UINT8         DCUIPPrefetcherEnable;
  UINT8         DCUIPPrefetcherEnableLockHide;

  //
  // Enable Processor XAPIC
  //
  UINT8         ProcessorXapic;
  UINT8         ProcessorXapicLockHide;

  //
  // Select BSP
  //
  UINT8         BspSelection;
  UINT8         BspSelectionLockHide;

  //
  // Non-Turbo Mode Processor Core Ratio Multiplier
  //
  UINT8         ProcessorFlexibleRatio;
  UINT8         ProcessorFlexibleRatioLockHide;

  //
  // Turbo-XE Mode Processor TDC Limit Override Enable
  //
  UINT8         ProcessorTDCLimitOverrideEnable;
  UINT8         ProcessorTDCLimitOverrideEnableLockHide;

  //
  // Turbo-XE Mode Processor TDC Limit
  //
  UINT16        ProcessorTDCLimit;
  UINT8         ProcessorTDCLimitLockHide;

  //
  // Turbo-XE Mode Processor TDP Limit Override Enable
  //
  UINT8         ProcessorTDPLimitOverrideEnable;
  UINT8         ProcessorTDPLimitOverrideEnableLockHide;

  //
  // Turbo-XE Mode Processor TDP Limit
  //
  UINT16        ProcessorTDPLimit;
  UINT8         ProcessorTDPLimitLockHide;

  //
  // For changing UC to WB
  //
  UINT8         MTRRDefTypeUncachable;
  UINT8         MTRRDefTypeUncachableLockHide;

  //
  // Virtual wire A or B
  //
  UINT8         ProcessorVirtualWireMode;
  UINT8         ProcessorVirtualWireModeLockHide;

  //
  // Ext Burn in
  //
  UINT8         ExtBurnInEnable;
  UINT8         ExtBurnInEnableLockHide;

  //
  // CPU Burn-in Enable 0/1 No/Yes
  //
  UINT8         CpuBurnInEnable;
  UINT8         CpuBurnInEnableLockHide;

  //
  // CPU Power selection 0/1 Low/High
  //
  UINT8         CPUPow;
  UINT8         CPUPowLockHide;

  //
  // VID Value to use (0-63)
  //
  UINT8         VIDVal;
  UINT8         VIDValLockHide;

  //
  // BSEL Value to use (0-8)
  //
  UINT8         BSELVal;
  UINT8         BSELValLockHide;

  //
  // VCore Burn-in Mode 0/1/2/3 1.500V/1.550V/1.600V/1.625V
  //
  UINT8         VCoreBurnIn;
  UINT8         VCoreBurnInLockHide;

  //
  // VTT (Front Side Bus) Voltage Override
  //
  UINT8         VTtBurnIn;
  UINT8         VTtBurnInLockHide;

  //
  // PCI E Burn In
  //
  UINT8         PCIeBurnIn;
  UINT8         PCIeBurnInLockHide;

  //
  // FSB Override Automatic/Manual
  //
  UINT8         FsbOverride;
  UINT8         FsbOverrideLockHide;

  //
  // FSB Frequency Override in MHz
  //
  UINT16        FsbFrequency;
  UINT8         FsbFrequencyLockHide;

  //
  // Mailbox variables to store default, CPU Multiplier and FSB Frequency.
  //
  UINT16        DefFsbFrequency;

  //
  // Used as a CPU Voltage Status.
  //
  UINT8         VIDValStatus;

  //
  // Ecc  0/1 Disable/Enable if supported
  //
  UINT8         EccEnable;
  UINT8         EccEnableLockHide;

  //
  // Memory
  //
  UINT8         MemoryMode;
  UINT8         MemoryModeLockHide;

  UINT16        MemorySpeed;
  UINT8         MemorySpeedLockHide;

  UINT8         UclkRatio;
  UINT8         UclkRatioLockHide;

  UINT8         MemoryRatio;
  UINT8         MemoryRatioLockHide;

  UINT8         MemoryTcl;
  UINT8         MemoryTclLockHide;

  UINT8         MemoryTrcd;
  UINT8         MemoryTrcdLockHide;

  UINT8         MemoryTrp;
  UINT8         MemoryTrpLockHide;

  UINT8         MemoryTras;
  UINT8         MemoryTrasLockHide;

  UINT16        MemoryTrfc;
  UINT8         MemoryTrfcLockHide;

  UINT8         MemoryTrrd;
  UINT8         MemoryTrrdLockHide;

  UINT8         MemoryTwr;
  UINT8         MemoryTwrLockHide;

  UINT8         MemoryTwtr;
  UINT8         MemoryTwtrLockHide;

  UINT8         MemoryTrtp;
  UINT8         MemoryTrtpLockHide;

  UINT8         MemoryTrc;
  UINT8         MemoryTrcLockHide;

  UINT8         MemoryTfaw;
  UINT8         MemoryTfawLockHide;

  UINT8         MemoryTcwl;
  UINT8         MemoryTcwlLockHide;

  UINT8         MemoryVoltage;
  UINT8         MemoryVoltageLockHide;

  //
  // Reference Voltage Override
  //
  UINT8         DimmDqRef;
  UINT8         DimmDqRefLockHide;
  UINT8         DimmCaRef;
  UINT8         DimmCaRefLockHide;

  //
  // Ratio Limit options for Turbo-Mode
  //
  UINT8         RatioLimit4C;
  UINT8         RatioLimit4CLockHide;
  UINT8         RatioLimit3C;
  UINT8         RatioLimit3CLockHide;
  UINT8         RatioLimit2C;
  UINT8         RatioLimit2CLockHide;
  UINT8         RatioLimit1C;
  UINT8         RatioLimit1CLockHide;

  //
  // Port 80 decode 0/1 - PCI/LPC
  UINT8         Port80Route;
  UINT8         Port80RouteLockHide;

  //
  // ECC Event Logging
  //
  UINT8         EccEventLogging;
  UINT8         EccEventLoggingLockHide;

  //
  // LT Technology 0/1 -> Disable/Enable
  //
  UINT8         LtTechnology;
  UINT8         LtTechnologyLockHide;

  //
  // ICH Function Level Reset enable/disable
  //
  UINT8         FlrCapability;
  UINT8         FlrCapabilityLockHide;

  //
  // VT-d Option
  //
  UINT8         VTdSupport;
  UINT8         VTdSupportLockHide;

  UINT8         InterruptRemap;
  UINT8         InterruptRemapLockHide;

  UINT8         Isoc;
  UINT8         IsocLockHide;

  UINT8         CoherencySupport;
  UINT8         CoherencySupportLockHide;

  UINT8         ATS;
  UINT8         ATSLockHide;

  UINT8         PassThroughDma;
  UINT8         PassThroughDmaLockHide;

  //
  // IGD option
  //
  UINT8         GraphicsDriverMemorySize;
  UINT8         GraphicsDriverMemorySizeLockHide;

  //
  // Hyper Threading
  //
  UINT8         ProcessorHtMode;
  UINT8         ProcessorHtModeLockHide;

  //
  // IGD Aperture Size question
  //
  UINT8         IgdApertureSize;
  UINT8         IgdApertureSizeLockHide;

  //
  // Boot Display Device
  //
  UINT8         BootDisplayDevice;
  UINT8         BootDisplayDeviceLockHide;

  //
  // System fan speed duty cycle
  //
  UINT8         SystemFanDuty;
  UINT8         SystemFanDutyLockHide;

  //
  // S3 state LED indicator
  //
  UINT8         S3StateIndicator;
  UINT8         S3StateIndicatorLockHide;

  //
  // S1 state LED indicator
  //
  UINT8         S1StateIndicator;
  UINT8         S1StateIndicatorLockHide;

  //
  // PS/2 Wake from S5
  //
  UINT8         WakeOnS5Keyboard;
  UINT8         WakeOnS5KeyboardLockHide;

  //
  // PS2 port
  //
  UINT8         PS2;

  //
  // No VideoBeep
  //
  UINT8         NoVideoBeepEnable;

  //
  // Integrated Graphics Device
  //
  UINT8         Igd;

  //
  // Video Device select order
  //
  UINT8         VideoSelectOrder[8];

  // Flash update sleep delay
  UINT8         FlashSleepDelay;
  UINT8         FlashSleepDelayLockHide;

  //
  // Boot Display Device2
  //
  UINT8         BootDisplayDevice2;
  UINT8         BootDisplayDevice2LockHide;

  //
  // Flat Panel
  //
  UINT8         EdpInterfaceType;
  UINT8         EdpInterfaceTypeLockHide;

  UINT8         LvdsInterfaceType;
  UINT8         LvdsInterfaceTypeLockHide;

  UINT8         ColorDepth;
  UINT8         ColorDepthLockHide;

  UINT8         EdidConfiguration;
  UINT8         EdidConfigurationLockHide;

  UINT8         MaxInverterPWM;
  UINT8         MaxInverterPWMLockHide;

  UINT8         PreDefinedEdidConfiguration;
  UINT8         PreDefinedEdidConfigurationLockHide;

  UINT16        ScreenBrightnessResponseTime;
  UINT8         ScreenBrightnessResponseTimeLockHide;

  UINT8         CurrentSetupProfile;
  UINT8         CurrentSetupProfileLockHide;

  //
  // FSC system Variable
  //
  UINT8         CPUFanUsage;
  UINT8         CPUFanUsageLockHide;
  UINT16        CPUUnderSpeedthreshold;
  UINT8         CPUUnderSpeedthresholdLockHide;
  UINT8         CPUFanControlMode;
  UINT8         CPUFanControlModeLockHide;
  UINT16        Voltage12UnderVolts;
  UINT8         Voltage12UnderVoltsLockHide;
  UINT16        Voltage12OverVolts;
  UINT8         Voltage12OverVoltsLockHide;
  UINT16        Voltage5UnderVolts;
  UINT8         Voltage5UnderVoltsLockHide;
  UINT16        Voltage5OverVolts;
  UINT8         Voltage5OverVoltsLockHide;
  UINT16        Voltage3p3UnderVolts;
  UINT8         Voltage3p3UnderVoltsLockHide;
  UINT16        Voltage3p3OverVolts;
  UINT8         Voltage3p3OverVoltsLockHide;
  UINT16        Voltage2p5UnderVolts;
  UINT8         Voltage2p5UnderVoltsLockHide;
  UINT16        Voltage2p5OverVolts;
  UINT8         Voltage2p5OverVoltsLockHide;
  UINT16        VoltageVccpUnderVolts;
  UINT8         VoltageVccpUnderVoltsLockHide;
  UINT16        VoltageVccpOverVolts;
  UINT8         VoltageVccpOverVoltsLockHide;
  UINT16        Voltage5BackupUnderVolts;
  UINT8         Voltage5BackupUnderVoltsLockHide;
  UINT16        Voltage5BackupOverVolts;
  UINT8         Voltage5BackupOverVoltsLockHide;
  UINT16        VS3p3StbyUnderVolt;
  UINT8         VS3p3StbyUnderVoltLockHide;
  UINT16        VS3p3StbyOverVolt;
  UINT8         VS3p3StbyOverVoltLockHide;
  UINT8         CPUFanMinDutyCycle;
  UINT8         CPUFanMinDutyCycleLockHide;
  UINT8         CPUFanMaxDutyCycle;
  UINT8         CPUFanMaxDutyCycleLockHide;
  UINT8         CPUFanOnDutyCycle;
  UINT8         CPUFanOnDutyCycleLockHide;
  UINT16        CpuOverTemp;
  UINT8         CpuOverTempLockHide;
  UINT16        CpuControlTemp;
  UINT8         CpuControlTempLockHide;
  UINT16        CpuAllOnTemp;
  UINT8         CpuAllOnTempLockHide;
  UINT8         CpuResponsiveness;
  UINT8         CpuResponsivenessLockHide;
  UINT8         CpuDamping;
  UINT8         CpuDampingLockHide;
  UINT8         PchDamping;
  UINT8         PchDampingLockHide;
  UINT16        MemoryOverTemp;
  UINT8         MemoryOverTempLockHide;
  UINT16        MemoryControlTemp;
  UINT8         MemoryControlTempLockHide;
  UINT16        MemoryAllOnTemp;
  UINT8         MemoryAllOnTempLockHide;
  UINT8         MemoryResponsiveness;
  UINT8         MemoryResponsivenessLockHide;
  UINT8         MemoryDamping;
  UINT8         MemoryDampingLockHide;
  UINT16        VROverTemp;
  UINT8         VROverTempLockHide;
  UINT16        VRControlTemp;
  UINT8         VRControlTempLockHide;
  UINT16        VRAllOnTemp;
  UINT8         VRAllOnTempLockHide;
  UINT8         VRResponsiveness;
  UINT8         VRResponsivenessLockHide;
  UINT8         VRDamping;
  UINT8         VRDampingLockHide;

  UINT8         LvdsBrightnessSteps;
  UINT8         LvdsBrightnessStepsLockHide;
  UINT8         EdpDataRate;
  UINT8         EdpDataRateLockHide;
  UINT16        LvdsPowerOnToBacklightEnableDelayTime;
  UINT8         LvdsPowerOnToBacklightEnableDelayTimeLockHide;
  UINT16        LvdsPowerOnDelayTime;
  UINT8         LvdsPowerOnDelayTimeLockHide;
  UINT16        LvdsBacklightOffToPowerDownDelayTime;
  UINT8         LvdsBacklightOffToPowerDownDelayTimeLockHide;
  UINT16        LvdsPowerDownDelayTime;
  UINT8         LvdsPowerDownDelayTimeLockHide;
  UINT16        LvdsPowerCycleDelayTime;
  UINT8         LvdsPowerCycleDelayTimeLockHide;

  UINT8         IgdFlatPanel;
  UINT8         IgdFlatPanelLockHide;

  UINT8         SwapMode;
  UINT8         SwapModeLockHide;

  UINT8         UsbCharging;
  UINT8         UsbChargingLockHide;

  UINT8         Cstates;
  UINT8         EnableC4;
  UINT8         EnableC6;

  UINT8         FastBoot;
  UINT8         EfiNetworkSupport;
  UINT8         PxeRom;

  //
  // Add for PpmPlatformPolicy
  //
  UINT8         EnableGv;
  UINT8         EnableCx;
  UINT8         EnableCxe;
  UINT8         EnableTm;
  UINT8         EnableProcHot;
  UINT8         TStatesEnable;
  UINT8         HTD;
  UINT8         SingleCpu;
  UINT8         BootPState;
  UINT8         FlexRatio;
  UINT8         FlexVid;
  UINT8         QuietBoot;
  UINT8         CsmControl;
  UINT8         BoardId; // Need to detect Board Id during setup option for CR

  UINT8         MinInverterPWM;
  //
  // Thermal Policy Values
  //
  UINT8         EnableDigitalThermalSensor;
  UINT8         PassiveThermalTripPoint;
  UINT8         PassiveTc1Value;
  UINT8         PassiveTc2Value;
  UINT8         PassiveTspValue;
  UINT8         DisableActiveTripPoints;
  UINT8         CriticalThermalTripPoint;
  UINT8         DeepStandby;
  UINT8         AlsEnable;
  UINT8         IgdLcdIBia;
  UINT8         LogBootTime;
  //
  // EM-1 related
  //
  UINT16        IaAppsRun;
  UINT16        IaAppsRunCR;
  UINT8         IaAppsCap;
  UINT8         CapOrVoltFlag;
  UINT8         BootOnInvalidBatt;

  UINT8         IffsEnable;
  UINT8         IffsOnS3RtcWake;
  UINT8         IffsS3WakeTimerMin;
  UINT8         IffsOnS3CritBattWake;
  UINT8         IffsCritBattWakeThreshold;
  UINT8         ScramblerSupport;
  UINT8         SecureBoot;
  UINT8         SecureBootCustomMode;
  UINT8         SecureBootUserPhysicalPresent;
  UINT8         CoreFreMultipSelect;
  UINT8         MaxCState;
  UINT8         PanelScaling;
  UINT8         IgdLcdIGmchBlc;
  UINT8         SecEnable;
  UINT8         SecFlashUpdate;
  UINT8         SecFirmwareUpdate;
  UINT8         MeasuredBootEnable;
  UINT8         UseProductKey;
  //
  // Image Signal Processor PCI Device Configuration
  //
  UINT8         ISPDevSel;
  UINT8         ISPEn;

  UINT8         Tdt;
  UINT8         Recovery;
  UINT8         Suspend;

  UINT8         TdtState;
  UINT8         TdtEnrolled;
  UINT8         PBAEnable;
  //
  // ISCT Configuration
  //
  UINT8         IsctConfiguration;
  UINT8         IsctNotificationControl;
  UINT8         IsctWlanPowerControl;
  UINT8         IsctWwanPowerControl;
  UINT8         IsctSleepDurationFormat;
  UINT8         IsctRFKillSupport;
  UINT8         WlanNGFFCardPresence;
  UINT8         WlanUHPAMCardPresence;
  UINT8         PchFSAOn;       //FSA control

  //
  // South Cluster Area - START
  //
  //
  // Miscellaneous options
  //
  UINT8         SmbusEnabled;
  UINT8         PchSirq;
  UINT8         PchSirqMode;
  UINT8         Hpet;
  UINT8         HpetBootTime;
  UINT8         EnableClockSpreadSpec;
  UINT8         EnablePciClockSpreadSpec;
  UINT8         EnableUsb3ClockSpreadSpec;
  UINT8         EnableDisplayClockSpreadSpec;
  UINT8         EnableSataClockSpreadSpec;
  UINT8         StateAfterG3;
  UINT8         UartInterface;
  UINT8         IspLpePltClk;
  UINT8         UsbDebug;
  UINT8         ConfigureCfioOnSx;
  //
  // Security Config
  //
  UINT8         PchRtcLock;
  UINT8         PchBiosLock;

  //
  // SCC Configuration
  //
  UINT8         ScceMMCEnabled;
  UINT8         SccSdioEnabled;
  UINT8         SccSdcardEnabled;
  //
  // LPSS Configuration
  //
  UINT8         GpioAcpiEnabled;
  UINT16        Sdcard1p8vSwitchingDelay;
  UINT16        Sdcard3p3vDischargeDelay;
  UINT8         LpssDma1Enabled;
  UINT8         LpssI2C0Enabled;
  UINT8         LpssI2C1Enabled;
  UINT8         LpssI2C2Enabled;
  UINT8         LpssI2C3Enabled;
  UINT8         LpssI2C4Enabled;
  UINT8         LpssI2C5Enabled;
  UINT8         LpssI2C6Enabled;
  UINT8         LpssDma0Enabled;
  UINT8         LpssPwm0Enabled;
  UINT8         LpssPwm1Enabled;
  UINT8         LpssHsuart0Enabled;
  UINT8         LpssHsuart1Enabled;
  UINT8         LpssSpi1Enabled;
  UINT8         LpssSpi2Enabled;
  UINT8         LpssSpi3Enabled;
  UINT8         I2CTouchAd;
  UINT8         BTModule;
  UINT8         RvpCameraDevSel;
  UINT8         EbCameraDevSel;
  UINT8         SecureNfc;
  UINT8         Bcm4356;
  UINT8         GpsEnable;

  //
  // Usb Config
  //
  UINT8         PchUsb30Mode;
  UINT8         PchSsicEnable;
  UINT8         PchUsbSsicHsRate;
  UINT8         PchUsbSsicInitSequence;
  UINT8         PchUsbSsicPort[PCH_SSIC_MAX_PORTS];
  UINT8         PchUsbHsicPort[PCH_HSIC_MAX_PORTS];
  UINT8         PchUsb2PhyPgEnabled;
  UINT8         PchUsbOtg;
  UINT8         PchUsbVbusOn;       // OTG VBUS control

  //
  // Ish Config
  //
  UINT8         PchIshEnabled;
  UINT8         IshDebuggerEnabled;

  //
  // SATA Config
  //
  UINT8         PchSata;
  UINT8         SataInterfaceMode;
  UINT8         SataInterfaceSpeed;
  UINT8         SataPort[2];
  UINT8         SataHotPlug[2];
  UINT8         SataMechanicalSw[2];
  UINT8         SataSpinUp[2];
  UINT8         SataDevSlp[2];
  UINT8         SataExternal[2];
  UINT8         SataRaidR0;
  UINT8         SataRaidR1;
  UINT8         SataRaidR10;
  UINT8         SataRaidR5;
  UINT8         SataRaidIrrt;
  UINT8         SataRaidOub;
  UINT8         SataHddlk;
  UINT8         SataLedl;
  UINT8         SataRaidIooe;
  UINT8         SataAlternateId;
  UINT8         SataSalp;
  UINT8         SataTestMode;

  //
  // PCI_EXPRESS_CONFIG, 4 ROOT PORTS
  //
  UINT8         PcieRootPortEn[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortAspm[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortURE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortFEE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortNFE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortCEE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortSFE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortSNE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortSCE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortPMCE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortESE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortHPE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortSpeed[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortTHS[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortL1SubStates[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortNccSsc[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieRootPortTxEqDeemphSelection[PCH_PCIE_MAX_ROOT_PORTS];

  //
  // PCI Bridge Resources
  //
  UINT8         PcieExtraBusRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieMemRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8         PcieIoRsvd[PCH_PCIE_MAX_ROOT_PORTS];

  //
  // PCI Express S0ix Config
  //
  UINT8         PcieS0iX;
  UINT8         D0S0IxPolicy;
  UINT8         ClkReqEnable;
  UINT8         ClkReq;
  UINT8         LtrLatencyScale;
  UINT8         LtrLatencyValue;

  //
  // Audio Configuration
  //
  UINT8         PchLpeEnabled;
  UINT8         PchAzalia;
  UINT8         AzaliaVCiEnable;
  UINT8         AzaliaDs;
  UINT8         AzaliaPme;
  UINT8         HdmiCodec;
  UINT8         HdmiCodecPortB;
  UINT8         HdmiCodecPortC;
  UINT8         HdmiCodecPortD;
  //
  // South Cluster Area - END
  //

  UINT8         GTTSize;
  //
  // DVMT5.0 Graphic memory setting
  //
  UINT8         IgdDvmt50PreAlloc;
  UINT8         IgdDvmt50TotalAlloc;
  UINT8         IgdTurboEnabled;
  UINT8         EnableRenderStandby;
  UINT8         GOPEnable;
  UINT8         GOPBrightnessLevel;                     // Gop Brightness level
  UINT8         PanelConfig;
  UINT8         PanelVendor;
  UINT8         PavpMode;
  UINT8         EnablePR3;
  UINT8         Wopcmsz;
  UINT8         UnsolicitedAttackOverride;

  UINT8         SeCOpEnable;
  UINT8         SeCModeEnable;
  UINT8         SeCEOPEnable;
  UINT8         SeCEOPDone;

  UINT8         LidStatus;
  UINT8         PowerMeterLock;
  UINT8         EuControl;
  UINT8         SdpProfile;                              // DPTF:  an enumeration for Brand Strings.
  UINT8         CameraSelect;
  UINT8         F22Rework;
  UINT8         EnableDptf;                              // Option to enable/disable DPTF
  UINT16        ProcCriticalTemp;                        // Processor critical temperature
  UINT16        ProcPassiveTemp;                         // Processor passive temperature

  UINT16        ActiveThermalTripPointSA;                // Processor active temperature
  UINT16        CriticalThermalTripPointSA;              // Processor critical temperature
  UINT16        CR3ThermalTripPointSA;                   // Processor CR3 temperature
  UINT16        HotThermalTripPointSA;                   // Processor Hot temperature
  UINT16        PassiveThermalTripPointSA;               // Processor passive temperature

  UINT16        GenericActiveTemp0;                      // Active temperature value for generic sensor0 participant
  UINT16        GenericCriticalTemp0;                    // Critical temperature value for generic sensor0 participant
  UINT16        GenericCR3Temp0;                         // CR3 temperature value for generic sensor0 participant
  UINT16        GenericHotTemp0;                         // Hot temperature value for generic sensor0 participant
  UINT16        GenericPassiveTemp0;                     // Passive temperature value for generic sensor0 participant
  UINT16        GenericActiveTemp1;                      // Active temperature value for generic sensor1 participant
  UINT16        GenericCriticalTemp1;                    // Critical temperature value for generic sensor1 participant
  UINT16        GenericCR3Temp1;                         // CR3 temperature value for generic sensor1 participant
  UINT16        GenericHotTemp1;                         // Hot temperature value for generic sensor1 participant
  UINT16        GenericPassiveTemp1;                     // Passive temperature value for generic sensor1 participant
  UINT16        GenericActiveTemp2;                      // Active temperature value for generic sensor2 participant
  UINT16        GenericCriticalTemp2;                    // Critical temperature value for generic sensor2 participant
  UINT16        GenericCR3Temp2;                         // CR3 temperature value for generic sensor2 participant
  UINT16        GenericHotTemp2;                         // Hot temperature value for generic sensor2 participant
  UINT16        GenericPassiveTemp2;                     // Passive temperature value for generic sensor2 participant
  UINT16        GenericCriticalTemp3;                    // Critical temperature value for generic sensor3 participant
  UINT16        GenericPassiveTemp3;                     // Passive temperature value for generic sensor3 participant
  UINT16        GenericCriticalTemp4;                    // Critical temperature value for generic sensor3 participant
  UINT16        GenericPassiveTemp4;                     // Passive temperature value for generic sensor3 participant
  UINT8         Clpm;                                    // Current low power mode
  UINT8         SuperDebug;                              // DPTF Super debug option
  UINT32        LPOEnable;                               // DPTF: Instructs the policy to use Active Cores if they are available. If this option is set to 0, then policy does not use any active core controls ?even if they are available
  UINT32        LPOStartPState;                          // DPTF: Instructs the policy when to initiate Active Core control if enabled. Returns P state index.
  UINT32        LPOStepSize;                             // DPTF: Instructs the policy to take away logical processors in the specified percentage steps
  UINT32        LPOPowerControlSetting;                  // DPTF: Instructs the policy whether to use Core offliing or SMT offlining if Active core control is enabled to be used in P0 or when power control is applied. 1 ?SMT Off lining 2- Core Off lining
  UINT32        LPOPerformanceControlSetting;            // DPTF: Instructs the policy whether to use Core offliing or SMT offlining if Active core control is enabled to be used in P1 or when performance control is applied.1 ?SMT Off lining 2- Core Off lining
  UINT8         EnableDppm;                              // DPTF: Controls DPPM Policies (enabled/disabled)
  UINT8         DptfProcessor;
  UINT8         DptfSysThermal0;
  UINT8         DptfSysThermal1;
  UINT8         DptfSysThermal2;
  UINT8         DptfSysThermal3;
  UINT8         DptfSysThermal4;
  UINT8         DptfChargerDevice;
  UINT8         DptfDisplayDevice;
  UINT8         DptfSocDevice;
  UINT8         BidirectionalProchotEnable;
  UINT8         ThermalMonitoring;
  UINT8         ThermalMonitoringHot;
  UINT8         ThermalMonitoringSystherm0Hot;
  UINT8         ThermalMonitoringSystherm1Hot;
  UINT8         ThermalMonitoringSystherm2Hot;
  UINT8         DisplayHighLimit;
  UINT8         DisplayLowLimit;
  UINT8         AmbientConstants[6];
  UINT8         AmbientConstantSign[6];
  UINT8         AmbientTripPointChange;                  // DPTF: Controls whether _ATI changes other participant's trip point(enabled/disabled)
  UINT8         DptfAllowHigherPerformance;              // DPTF: Allow higher performance on AC/USB - (Enable/Disable)
  UINT8         DptfWwanDevice;                          // DPTF: Presence of WWAN Device
  UINT32        DptfWwanCrt;                             // DPTF: WWAN critical temperature
  UINT32        DptfWwanPsv;                             // DPTF: WWAN Passive Temperature
  UINT8         EnablePassivePolicy;                     // DPTF: Passive Policy enable/disable
  UINT8         EnableCriticalPolicy;                    // DPTF: Critical Policy enable/disable
  UINT8         EnableActivePolicy;                      // DPTF: Active Policy enable/disable
  UINT8         PmicEnable;
  UINT8         S0ix;
  UINT8         TSEGSizeSel;
  UINT8         ACPIMemDbg;
  UINT8         ExISupport;
  UINT8         BatteryChargingSolution;                 // 0-non ULPMC 1-ULPMC

  UINT8         PnpSettings;
  UINT8         MfgMode;
  UINT8         CRIDSettings;
  UINT8         ULPMCFWLock;
  UINT8         PssEnabled;
  UINT8         PmWeights;
  UINT8         PdmEnable;
  UINT8         PDMConfig;
  UINT16        LmMemSize;
  UINT8         PunitBIOSConfig;
  UINT8         LpssSdioMode;
  UINT8         ENDBG2;
  UINT8         IshPullUp;
  UINT8         TristateLpc;
  UINT8         UsbXhciLpmSupport;
  UINT8         VirtualKbEnable;
  UINT8         SlpS0ixN;
  UINT8         EnableAESNI;
  UINT8         SecureErase;

  //
  // Memory Config Tools
  //
  UINT8         MrcEvRMT;
  UINT8         MrcCmdRMT;
  UINT8         MrcDvfsEnable;
  UINT8         MrcFreqASel;
  UINT8         MrcFreqBSel;
  UINT8         MrcLPDDR3ChipSel;
  UINT8         MrcChannelSel;
  UINT8         MrcDynamicSr;
  UINT8         MrcChannelSel_3_0;
  UINT8         MrcChannelSel_4;
  UINT8         MrcAutoDetectDram;
  UINT8         Sku;
  UINT8         MrcPm5Enable;
  UINT8         MrcBankAddressHashingEnable;
  UINT8         MrcRankSelInterleave;
  UINT8         MrcConfigChanged;
  UINT8         MrcDdrType;
  UINT8         MrcDdr2nMode;
  UINT8         MrcRxPwrTrainingDisable;
  UINT8         MrcTxPwrTrainingDisable;
  UINT8         MrcFastBootDisable;
  UINT8         MrcScramblerDisable;
  UINT8         MrcSpeedGrade;
  UINT8         MrcLPDDR3DeviceDensity;
  UINT8         MrcDebugMsgLevel;
  UINT8         DrpLockDisable;
  UINT8         ReutLockDisable;
  UINT8         RhPrevention;

  UINT8         MmioSize;
  UINT8         DroidBoot;
  UINT8         AndroidBoot;
  UINT8         Ellensburg;
  UINT8         CriticalBatteryLimit;
  UINT8         CriticalBatteryLimitFeature;
  UINT8         EmmcDriverMode;
  UINT8         EmmcRxTuningEnable;
  UINT8         EmmcTxTuningEnable;

  UINT8         SAR1;

  UINT8         DisableCodec262;
  UINT8         PcieDynamicGating;        // Need PMC enable it first from PMC 0x3_12 MCU 318.
  UINT8         VirtualButtonEnable;
  UINT8         RotationLock;
  UINT8         ConvertibleState;
  UINT8         DockIndicator;
  UINT8         WIFIModule;
  UINT8         SvidConfig;
  UINT8         PciExpNative;
  UINT8         OsSelection;
  UINT8         PlatformDirectOsSelect; //If set to 1 (TRUE), platform method (GPI on Cherry Hill) will be used to select OS.
                                        //If set to 0 (FALSE), OS selection option in setup menu will be used to select OS.

  UINT8         MipiDsi;
  UINT8         AndroidSBIntegration;
  UINT8         AcpiDevNodeDis;
  UINT8         AcpiModemSel;

  //
  // SPID config region
  //
  UINT8         SPIDAutoDetect;
  UINT16        SPIDCustomerID;
  UINT16        SPIDVendorID;
  UINT16        SPIDDeviceManufacturerID;
  UINT16        SPIDPlatformFamilyID;
  UINT16        SPIDProductLineID;
  UINT16        SPIDHardwareID;
  UINT8         SPIDFru[20];
  //
  // OEM1 table
  //
  UINT8         BatIdDbiBase;
  UINT8         BatIdAnlgBase;
  UINT16        VBattFreqLimit;
  UINT8         CapFreqIdx;
  UINT8         BTHStatus;
  UINT8         AudioCodecSuppport;
  UINT8         ChargingEnable;
  UINT8         ChargingLpmEnable;
  UINT16        Str2TspValue;
  UINT8         VBIOS5f35h;
  UINT8         VBIOS5f40h;
  UINT8         Backlight;
  UINT8         PunitPwrConfigDisable;
  UINT8         FlashLessMdm;
  UINT8         EnableMSCustomSdbusDriver;

#ifdef PRAM_SUPPORT
  UINT8         PramSize;
#endif
  UINT8         XdbGpioTrst;
  UINT8         FirstBootIndicator;
  UINT8         ConnectAllCtrlsFlag;
  UINT8         EnterDnxFastBoot;
  UINT8         ToggleSelfClkDisabling;
  UINT8         GPSHIDSelection;
  UINT8         HighPerfMode;

} SYSTEM_CONFIGURATION;
#pragma pack()

#ifndef PLATFORM_SETUP_VARIABLE_NAME
#define PLATFORM_SETUP_VARIABLE_NAME             L"Setup"
#endif

//
// #defines for Drive Presence
//
#define EFI_HDD_PRESENT       0x01
#define EFI_HDD_NOT_PRESENT   0x00
#define EFI_CD_PRESENT        0x02
#define EFI_CD_NOT_PRESENT    0x00

#define EFI_HDD_WARNING_ON    0x01
#define EFI_CD_WARNING_ON     0x02
#define EFI_SMART_WARNING_ON  0x04
#define EFI_HDD_WARNING_OFF   0x00
#define EFI_CD_WARNING_OFF    0x00
#define EFI_SMART_WARNING_OFF 0x00

#ifndef VFRCOMPILE
extern EFI_GUID gEfiSetupVariableGuid;
#endif

#define SETUP_DATA SYSTEM_CONFIGURATION

#endif // #ifndef _SETUP_VARIABLE
