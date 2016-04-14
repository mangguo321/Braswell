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

#ifndef _SETUP_VARIABLE_DEFAULT_H
#define _SETUP_VARIABLE_DEFAULT_H
#include<Guid/SetupVariable.h>
//
// NOTE: This is the default setup variable table
//
SYSTEM_CONFIGURATION  gDefaultSystemConfiguration = {
  //
  // System ports
  //
  0x0,         //UINT8         Serial;
  0x0,         //UINT8         SerialLockHide;

  0x0,         //UINT8         Serial2;
  0x0,         //UINT8         Serial2LockHide;

  0x0,         //UINT8         Parallel;
  0x0,         //UINT8         ParallelLockHide;

  0x0,         //UINT8         ParallelMode;
  0x0,         //UINT8         ParallelModeLockHide;

  0x0,         //UINT8         AllUsb;
  0x0,         //UINT8         UsbPortsLockHide;

  0x0,         //UINT8         Usb2;
  0x0,         //UINT8         Usb2LockHide;

  0x0,         //UINT8         UsbLegacy;
  0x0,         //UINT8         UsbLegacyLockHide;

  //
  // Keyboard
  //
  0x0,         //UINT8         Numlock;
  0x0,         //UINT8         NumlockLockHide;

  //
  // ECIR
  //
  0x0,         //UINT8         ECIR;
  0x0,         //UINT8         ECIRLockHide;

  //
  // Power State
  //
  0x0,         //UINT8         PowerState;
  0x0,         //UINT8         PowerStateLockHide;

  //
  // Wake on RTC variables
  //
  0x0,         //UINT8         WakeOnRtcS5;
  0x0,         //UINT8         WakeOnRtcS5LockHide;
  0x0,         //UINT8         RTCWakeupDate;
  0x0,         //UINT8         RTCWakeupDateLockHide;
  0x0,         //UINT8         RTCWakeupTimeHour;
  0x0,         //UINT8         RTCWakeupHourLockHide;
  0x0,         //UINT8         RTCWakeupTimeMinute;
  0x0,         //UINT8         RTCWakeupMinuteLockHide;
  0x0,         //UINT8         RTCWakeupTimeSecond;
  0x0,         //UINT8         RTCWakeupSecondLockHide;

  //
  // Video Adaptor
  //
  0x0,         //UINT8         PrimaryVideoAdaptor;
  0x0,         //UINT8         PrimaryVideoAdaptorLockHide;

  //
  // Hybrid Graphics
  //
  0x0,         //UINT16        DelayAfterPwrEn;
  0x0,         //UINT16        DelayAfterHoldReset;
  //
  // Chassis intrusion
  //
  0x0,         //UINT8         IntruderDetection;
  0x0,         //UINT8         IntruderDetectionLockHide;

  //
  // Maximum FSB Automatic/Disable
  //
  0x0,         //UINT8         MaxFsb;
  0x0,         //UINT8         MaxFsbLockHide;

  //
  // Hard Disk Pre-delay
  //
  0x0,         //UINT8         HddPredelay;
  0x0,         //UINT8         HddPredelayLockHide;

  //
  // S.M.A.R.T. Mode
  //
  0x0,         //UINT8         SmartMode;
  0x0,         //UINT8         SmartModeLockHide;

  //
  // ACPI Suspend State
  //
  0x0,         //UINT8         AcpiSuspendState;
  0x0,         //UINT8         AcpiSuspendStateLockHide;

  //
  // PCI Latency Timer
  //
  0x0,         //UINT8         PciLatency;
  0x0,         //UINT8         PciLatencyLockHide;

  //
  // Fan Control
  //
  0x0,         //UINT8         FanControl;
  0x0,         //UINT8         FanControlLockHide;

  //
  // CPU Fan Control
  //
  0x0,         //UINT8         CpuFanControl;
  0x0,         //UINT8         CpuFanControlLockHide;

  //
  // Lowest Fan Speed
  //
  0x0,         //UINT8         LowestFanSpeed;
  0x0,         //UINT8         LowestFanSpeedLockHide;

  //
  // Processor (CPU)
  //
  0x0,         //UINT8         CpuFlavor;

  0x0,         //UINT8         CpuidMaxValue;
  0x0,         //UINT8         CpuidMaxValueLockHide;

  0x0,         //UINT8         ExecuteDisableBit;
  0x0,         //UINT8         ExecuteDisableBitLockHide;

  //
  // EIST or GV3 setup option
  //
  0x0,         //UINT8         ProcessorEistEnable;
  0x0,         //UINT8         ProcessorEistEnableLockHide;

  //
  // C1E Enable
  //
  0x0,         //UINT8         ProcessorC1eEnable;
  0x0,         //UINT8         ProcessorC1eEnableLockHide;

  //
  // Enabling CPU C-States of processor
  //
  0x0,         //UINT8         ProcessorCcxEnable;
  0x0,         //UINT8         ProcessorCcxEnableLockHide;

  //
  // Package C-State Limit
  //
  0x0,         //UINT8         PackageCState;
  0x0,         //UINT8         PackageCStateLockHide;

  //
  // Enable/Disable NHM C3(ACPI C2) report to OS
  //
  0x0,         //UINT8         OSC2Report;
  0x0,         //UINT8         OSC2ReportLockHide;

  //
  // Enable/Disable NHM C6(ACPI C3) report to OS
  //
  0x0,         //UINT8         C6Enable;
  0x0,         //UINT8         C6EnableLockHide;

  //
  // Enable/Disable NHM C7(ACPI C3) report to OS
  //
  0x0,         //UINT8         C7Enable;
  0x0,         //UINT8         C7EnableLockHide;

  //
  // EIST/PSD Function select option
  //
  0x0,         //UINT8         ProcessorEistPsdFunc;
  0x0,         //UINT8         ProcessorEistPsdFuncLockHide;

  //
  // CPU Active Cores and SMT
  //
  0x0,         //UINT8         ActiveProcessorCores;
  0x0,         //UINT8         ActiveProcessorCoresLockHide;

  //
  // Hyper Threading
  //
  0x0,         //UINT8         ProcessorHyperThreadingDisable;
  0x0,         //UINT8         ProcessorHyperThreadingDisableLockHide;

  //
  // Enabling VMX
  //
  0x0,         //UINT8         ProcessorVmxEnable;
  0x0,         //UINT8         ProcessorVmxEnableLockHide;

  //
  // Enabling BIST
  //
  0x0,         //UINT8         ProcessorBistEnable;
  0x0,         //UINT8         ProcessorBistEnableLockHide;

  //
  // Disabling XTPR
  //
  0x0,         //UINT8         ProcessorxTPRDisable;
  0x0,         //UINT8         ProcessorxTPRDisableLockHide;

  //
  // Enabling XE
  //
  0x0,         //UINT8         ProcessorXEEnable;
  0x0,         //UINT8         ProcessorXEEnableLockHide;

  //
  // Fast String
  //
  0x0,         //UINT8         FastStringEnable;
  0x0,         //UINT8         FastStringEnableLockHide;

  //
  // Monitor/Mwait
  //
  0x0,         //UINT8         MonitorMwaitEnable;
  0x0,         //UINT8         MonitorMwaitEnableLockHide;

  //
  // Machine Check
  //
  0x0,         //UINT8         MachineCheckEnable;
  0x0,         //UINT8         MachineCheckEnableLockHide;

  //
  // Turbo mode
  //
  0x0,         //UINT8         TurboModeEnable;
  0x0,         //UINT8         TurboModeEnableLockHide;

  //
  // DCA setup option
  //
  0x0,         //UINT8         DcaEnable;
  0x0,         //UINT8         DcaEnableLockHide;

  //
  // DCA Prefetch Delay Value
  //
  0x0,         //UINT8         DcaPrefetchDelayValue;
  0x0,         //UINT8         DcaPrefetchDelayValueLockHide;

  //
  // Hardware Prefetch
  //
  0x0,         //UINT8         MlcStreamerPrefetcherEnable;
  0x0,         //UINT8         MlcStreamerPrefetcherEnableLockHide;

  //
  // Adjacent Cache Line Prefetch
  //
  0x0,         //UINT8         MlcSpatialPrefetcherEnable;
  0x0,         //UINT8         MlcSpatialPrefetcherEnableLockHide;

  //
  // DCU Streamer Prefetcher
  //
  0x0,         //UINT8         DCUStreamerPrefetcherEnable;
  0x0,         //UINT8         DCUStreamerPrefetcherEnableLockHide;

  //
  // DCU IP Prefetcher
  //
  0x0,         //UINT8         DCUIPPrefetcherEnable;
  0x0,         //UINT8         DCUIPPrefetcherEnableLockHide;

  //
  // Enable Processor XAPIC
  //
  0x0,         //UINT8         ProcessorXapic;
  0x0,         //UINT8         ProcessorXapicLockHide;

  //
  // Select BSP
  //
  0x0,         //UINT8         BspSelection;
  0x0,         //UINT8         BspSelectionLockHide;

  //
  // Non-Turbo Mode Processor Core Ratio Multiplier
  //
  0x0,         //UINT8         ProcessorFlexibleRatio;
  0x0,         //UINT8         ProcessorFlexibleRatioLockHide;

  //
  // Turbo-XE Mode Processor TDC Limit Override Enable
  //
  0x0,        //UINT8         ProcessorTDCLimitOverrideEnable;
  0x0,        //UINT8         ProcessorTDCLimitOverrideEnableLockHide;

  //
  // Turbo-XE Mode Processor TDC Limit
  //
  0x0,        //UINT16        ProcessorTDCLimit;
  0x0,        //UINT8         ProcessorTDCLimitLockHide;

  //
  // Turbo-XE Mode Processor TDP Limit Override Enable
  //
  0x0,         //UINT8         ProcessorTDPLimitOverrideEnable;
  0x0,         //UINT8         ProcessorTDPLimitOverrideEnableLockHide;

  //
  // Turbo-XE Mode Processor TDP Limit
  //
  0x0,         //UINT16        ProcessorTDPLimit;
  0x0,         //UINT8         ProcessorTDPLimitLockHide;

  //
  // For changing UC to WB
  //
  0x0,         //UINT8         MTRRDefTypeUncachable;
  0x0,         //UINT8         MTRRDefTypeUncachableLockHide;

  //
  // Virtual wire A or B
  //
  0x0,         //UINT8         ProcessorVirtualWireMode;
  0x0,         //UINT8         ProcessorVirtualWireModeLockHide;

  //
  // Ext Burn in
  //
  0x0,         //UINT8         ExtBurnInEnable;
  0x0,         //UINT8         ExtBurnInEnableLockHide;

  //
  // CPU Burn-in Enable 0/1 No/Yes
  //
  0x0,         //UINT8         CpuBurnInEnable;
  0x0,         //UINT8         CpuBurnInEnableLockHide;

  //
  // CPU Power selection 0/1 Low/High
  //
  0x0,         //UINT8         CPUPow;
  0x0,         //UINT8         CPUPowLockHide;

  //
  // VID Value to use (0-63)
  //
  0x0,         //UINT8         VIDVal;
  0x0,         //UINT8         VIDValLockHide;

  //
  // BSEL Value to use (0-8)
  //
  0x0,         //UINT8         BSELVal;
  0x0,         //UINT8         BSELValLockHide;

  //
  // VCore Burn-in Mode 0/1/2/3 1.500V/1.550V/1.600V/1.625V
  //
  0x0,         //UINT8         VCoreBurnIn;
  0x0,         //UINT8         VCoreBurnInLockHide;

  //
  // VTT (Front Side Bus) Voltage Override
  //
  0x0,         //UINT8         VTtBurnIn;
  0x0,         //UINT8         VTtBurnInLockHide;

  //
  // PCI E Burn In
  //
  0x0,         //UINT8         PCIeBurnIn;
  0x0,         //UINT8         PCIeBurnInLockHide;

  //
  // FSB Override Automatic/Manual
  //
  0x0,         //UINT8         FsbOverride;
  0x0,         //UINT8         FsbOverrideLockHide;

  //
  // FSB Frequency Override in MHz
  //
  0x0,         //UINT16        FsbFrequency;
  0x0,         //UINT8         FsbFrequencyLockHide;

  //
  // Mailbox variables to store default, CPU Multiplier and FSB Frequency.
  //
  0x0,       //UINT16        DefFsbFrequency;

  //
  // Used as a CPU Voltage Status.
  //
  0x0,         //UINT8         VIDValStatus;

  //
  // Ecc  0/1 Disable/Enable if supported
  //
  0x0,         //UINT8         EccEnable;
  0x0,         //UINT8         EccEnableLockHide;

  //
  // Memory
  //
  0x0,         //UINT8         MemoryMode;
  0x0,         //UINT8         MemoryModeLockHide;

  0x0,         //UINT16        MemorySpeed;
  0x0,         //UINT8         MemorySpeedLockHide;

  0x0,         //UINT8         UclkRatio;
  0x0,         //UINT8         UclkRatioLockHide;

  0x0,         //UINT8         MemoryRatio;
  0x0,         //UINT8         MemoryRatioLockHide;

  0x0,         //UINT8         MemoryTcl;
  0x0,         //UINT8         MemoryTclLockHide;

  0x0,         //UINT8         MemoryTrcd;
  0x0,         //UINT8         MemoryTrcdLockHide;

  0x0,         //UINT8         MemoryTrp;
  0x0,         //UINT8         MemoryTrpLockHide;

  0x0,         //UINT8         MemoryTras;
  0x0,         //UINT8         MemoryTrasLockHide;

  0x0,         //UINT16        MemoryTrfc;
  0x0,         //UINT8         MemoryTrfcLockHide;

  0x0,         //UINT8         MemoryTrrd;
  0x0,         //UINT8         MemoryTrrdLockHide;

  0x0,         //UINT8         MemoryTwr;
  0x0,         //UINT8         MemoryTwrLockHide;

  0x0,         //UINT8         MemoryTwtr;
  0x0,         //UINT8         MemoryTwtrLockHide;

  0x0,         //UINT8         MemoryTrtp;
  0x0,         //UINT8         MemoryTrtpLockHide;

  0x0,         //UINT8         MemoryTrc;
  0x0,         //UINT8         MemoryTrcLockHide;

  0x0,         //UINT8         MemoryTfaw;
  0x0,         //UINT8         MemoryTfawLockHide;

  0x0,         //UINT8         MemoryTcwl;
  0x0,         //UINT8         MemoryTcwlLockHide;

  0x0,         //UINT8         MemoryVoltage;
  0x0,         //UINT8         MemoryVoltageLockHide;

  //
  // Reference Voltage Override
  //
  0x0,         //UINT8         DimmDqRef;
  0x0,         //UINT8         DimmDqRefLockHide;
  0x0,         //UINT8         DimmCaRef;
  0x0,         //UINT8         DimmCaRefLockHide;

  //
  // Ratio Limit options for Turbo-Mode
  //
  0x0,         //UINT8         RatioLimit4C;
  0x0,         //UINT8         RatioLimit4CLockHide;
  0x0,         //UINT8         RatioLimit3C;
  0x0,         //UINT8         RatioLimit3CLockHide;
  0x0,         //UINT8         RatioLimit2C;
  0x0,         //UINT8         RatioLimit2CLockHide;
  0x0,         //UINT8         RatioLimit1C;
  0x0,         //UINT8         RatioLimit1CLockHide;

  //
  // Port 80 decode 0/1 - PCI/LPC
  0x0,         //UINT8         Port80Route;
  0x0,         //UINT8         Port80RouteLockHide;

  //
  // ECC Event Logging
  //
  0x0,         //UINT8         EccEventLogging;
  0x0,         //UINT8         EccEventLoggingLockHide;

  //
  // LT Technology 0/1 -> Disable/Enable
  //
  0x0,         //UINT8         LtTechnology;
  0x0,         //UINT8         LtTechnologyLockHide;

  //
  // ICH Function Level Reset enable/disable
  //
  0x0,         //UINT8         FlrCapability;
  0x0,         //UINT8         FlrCapabilityLockHide;

  //
  // VT-d Option
  //
  0x0,         //UINT8         VTdSupport;
  0x0,         //UINT8         VTdSupportLockHide;

  0x0,         //UINT8         InterruptRemap;
  0x0,         //UINT8         InterruptRemapLockHide;

  0x0,         //UINT8         Isoc;
  0x0,         //UINT8         IsocLockHide;

  0x0,         //UINT8         CoherencySupport;
  0x0,         //UINT8         CoherencySupportLockHide;

  0x0,         //UINT8         ATS;
  0x0,         //UINT8         ATSLockHide;

  0x0,         //UINT8         PassThroughDma;
  0x0,         //UINT8         PassThroughDmaLockHide;

  //
  // IGD option
  //
  0x0,         //UINT8         GraphicsDriverMemorySize;
  0x0,         //UINT8         GraphicsDriverMemorySizeLockHide;

  //
  // Hyper Threading
  //
  0x0,         //UINT8         ProcessorHtMode;
  0x0,         //UINT8         ProcessorHtModeLockHide;

  //
  // IGD Aperture Size question
  //
  0x2,         //UINT8         IgdApertureSize;
  0x0,         //UINT8         IgdApertureSizeLockHide;

  //
  // Boot Display Device
  //
  0x0,         //UINT8         BootDisplayDevice;
  0x0,         //UINT8         BootDisplayDeviceLockHide;

  //
  // System fan speed duty cycle
  //
  0x0,         //UINT8         SystemFanDuty;
  0x0,         //UINT8         SystemFanDutyLockHide;

  //
  // S3 state LED indicator
  //
  0x0,         //UINT8         S3StateIndicator;
  0x0,         //UINT8         S3StateIndicatorLockHide;

  //
  // S1 state LED indicator
  //
  0x0,         //UINT8         S1StateIndicator;
  0x0,         //UINT8         S1StateIndicatorLockHide;

  //
  // PS/2 Wake from S5
  //
  0x0,         //UINT8         WakeOnS5Keyboard;
  0x0,         //UINT8         WakeOnS5KeyboardLockHide;

  //
  // PS2 port
  //
  0x0,         //UINT8         PS2;

  //
  // No VideoBeep
  //
  0x0,         //UINT8         NoVideoBeepEnable;

  //
  // Integrated Graphics Device
  //
  0x1,         //UINT8         Igd;

  //
  // Video Device select order
  //
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        //UINT8         VideoSelectOrder[8];

  // Flash update sleep delay
  0x0,         //UINT8         FlashSleepDelay;
  0x0,         //UINT8         FlashSleepDelayLockHide;

  //
  // Boot Display Device2
  //
  0x0,         //UINT8         BootDisplayDevice2;
  0x0,         //UINT8         BootDisplayDevice2LockHide;

  //
  // Flat Panel
  //
  0x0,         //UINT8         EdpInterfaceType;
  0x0,         //UINT8         EdpInterfaceTypeLockHide;

  0x0,         //UINT8         LvdsInterfaceType;
  0x0,         //UINT8         LvdsInterfaceTypeLockHide;

  0x0,         //UINT8         ColorDepth;
  0x0,         //UINT8         ColorDepthLockHide;

  0x0,         //UINT8         EdidConfiguration;
  0x0,         //UINT8         EdidConfigurationLockHide;

  0x0,         //UINT8         MaxInverterPWM;
  0x0,         //UINT8         MaxInverterPWMLockHide;

  0x0,         //UINT8         PreDefinedEdidConfiguration;
  0x0,         //UINT8         PreDefinedEdidConfigurationLockHide;

  0x0,         //UINT16        ScreenBrightnessResponseTime;
  0x0,         //UINT8         ScreenBrightnessResponseTimeLockHide;

  0x0,         //UINT8         CurrentSetupProfile;
  0x0,         //UINT8         CurrentSetupProfileLockHide;

  //
  // FSC system Variable
  //
  0x0,         //UINT8         CPUFanUsage;
  0x0,         //UINT8         CPUFanUsageLockHide;
  0x0,         //UINT16        CPUUnderSpeedthreshold;
  0x0,         //UINT8         CPUUnderSpeedthresholdLockHide;
  0x0,         //UINT8         CPUFanControlMode;
  0x0,         //UINT8         CPUFanControlModeLockHide;
  0x0,         //UINT16        Voltage12UnderVolts;
  0x0,         //UINT8         Voltage12UnderVoltsLockHide;
  0x0,         //UINT16        Voltage12OverVolts;
  0x0,         //UINT8         Voltage12OverVoltsLockHide;
  0x0,         //UINT16        Voltage5UnderVolts;
  0x0,         //UINT8         Voltage5UnderVoltsLockHide;
  0x0,         //UINT16        Voltage5OverVolts;
  0x0,         //UINT8         Voltage5OverVoltsLockHide;
  0x0,         //UINT16        Voltage3p3UnderVolts;
  0x0,         //UINT8         Voltage3p3UnderVoltsLockHide;
  0x0,         //UINT16        Voltage3p3OverVolts;
  0x0,         //UINT8         Voltage3p3OverVoltsLockHide;
  0x0,         //UINT16        Voltage2p5UnderVolts;
  0x0,         //UINT8         Voltage2p5UnderVoltsLockHide;
  0x0,         //UINT16        Voltage2p5OverVolts;
  0x0,         //UINT8         Voltage2p5OverVoltsLockHide;
  0x0,         //UINT16        VoltageVccpUnderVolts;
  0x0,         //UINT8         VoltageVccpUnderVoltsLockHide;
  0x0,         //UINT16        VoltageVccpOverVolts;
  0x0,         //UINT8         VoltageVccpOverVoltsLockHide;
  0x0,         //UINT16        Voltage5BackupUnderVolts;
  0x0,         //UINT8         Voltage5BackupUnderVoltsLockHide;
  0x0,         //UINT16        Voltage5BackupOverVolts;
  0x0,         //UINT8         Voltage5BackupOverVoltsLockHide;
  0x0,         //UINT16        VS3p3StbyUnderVolt;
  0x0,         //UINT8         VS3p3StbyUnderVoltLockHide;
  0x0,         //UINT16        VS3p3StbyOverVolt;
  0x0,         //UINT8         VS3p3StbyOverVoltLockHide;
  0x0,         //UINT8         CPUFanMinDutyCycle;
  0x0,         //UINT8         CPUFanMinDutyCycleLockHide;
  0x0,         //UINT8         CPUFanMaxDutyCycle;
  0x0,         //UINT8         CPUFanMaxDutyCycleLockHide;
  0x0,         //UINT8         CPUFanOnDutyCycle;
  0x0,         //UINT8         CPUFanOnDutyCycleLockHide;
  0x0,         //UINT16        CpuOverTemp;
  0x0,         //UINT8         CpuOverTempLockHide;
  0x0,         //UINT16        CpuControlTemp;
  0x0,         //UINT8         CpuControlTempLockHide;
  0x0,         //UINT16        CpuAllOnTemp;
  0x0,         //UINT8         CpuAllOnTempLockHide;
  0x0,         //UINT8         CpuResponsiveness;
  0x0,         //UINT8         CpuResponsivenessLockHide;
  0x0,         //UINT8         CpuDamping;
  0x0,         //UINT8         CpuDampingLockHide;
  0x0,         //UINT8         PchDamping;
  0x0,         //UINT8         PchDampingLockHide;
  0x0,         //UINT16        MemoryOverTemp;
  0x0,         //UINT8         MemoryOverTempLockHide;
  0x0,         //UINT16        MemoryControlTemp;
  0x0,         //UINT8         MemoryControlTempLockHide;
  0x0,         //UINT16        MemoryAllOnTemp;
  0x0,         //UINT8         MemoryAllOnTempLockHide;
  0x0,         //UINT8         MemoryResponsiveness;
  0x0,         //UINT8         MemoryResponsivenessLockHide;
  0x0,         //UINT8         MemoryDamping;
  0x0,         //UINT8         MemoryDampingLockHide;
  0x0,         //UINT16        VROverTemp;
  0x0,         //UINT8         VROverTempLockHide;
  0x0,         //UINT16        VRControlTemp;
  0x0,         //UINT8         VRControlTempLockHide;
  0x0,         //UINT16        VRAllOnTemp;
  0x0,         //UINT8         VRAllOnTempLockHide;
  0x0,         //UINT8         VRResponsiveness;
  0x0,         //UINT8         VRResponsivenessLockHide;
  0x0,         //UINT8         VRDamping;
  0x0,         //UINT8         VRDampingLockHide;

  0x0,         //UINT8         LvdsBrightnessSteps;
  0x0,         //UINT8         LvdsBrightnessStepsLockHide;
  0x0,         //UINT8         EdpDataRate;
  0x0,         //UINT8         EdpDataRateLockHide;
  0x0,         //UINT16        LvdsPowerOnToBacklightEnableDelayTime;
  0x0,         //UINT8         LvdsPowerOnToBacklightEnableDelayTimeLockHide;
  0x0,         //UINT16        LvdsPowerOnDelayTime;
  0x0,         //UINT8         LvdsPowerOnDelayTimeLockHide;
  0x0,         //UINT16        LvdsBacklightOffToPowerDownDelayTime;
  0x0,         //UINT8         LvdsBacklightOffToPowerDownDelayTimeLockHide;
  0x0,         //UINT16        LvdsPowerDownDelayTime;
  0x0,         //UINT8         LvdsPowerDownDelayTimeLockHide;
  0x0,         //UINT16        LvdsPowerCycleDelayTime;
  0x0,         //UINT8         LvdsPowerCycleDelayTimeLockHide;

  0x0,         //UINT8         IgdFlatPanel;
  0x0,         //UINT8         IgdFlatPanelLockHide;

  0x0,         //UINT8         SwapMode;
  0x0,         //UINT8         SwapModeLockHide;

  0x0,         //UINT8         UsbCharging;
  0x0,         //UINT8         UsbChargingLockHide;

  0x0,         //UINT8         Cstates;
  0x0,         //UINT8         EnableC4;
  0x0,         //UINT8         EnableC6;

  0x0,         //UINT8         FastBoot;
  0x0,         //UINT8         EfiNetworkSupport;
  0x0,         //UINT8         PxeRom;

  //
  // Add for PpmPlatformPolicy
  //
  0x0,         //UINT8         EnableGv;
  0x0,         //UINT8         EnableCx;
  0x0,         //UINT8         EnableCxe;
  0x0,         //UINT8         EnableTm;
  0x0,         //UINT8         EnableProcHot;
  0x0,         //UINT8         TStatesEnable;
  0x0,         //UINT8         HTD;
  0x0,         //UINT8         SingleCpu;
  0x0,         //UINT8         BootPState;
  0x0,         //UINT8         FlexRatio;
  0x0,         //UINT8         FlexVid;
  0x0,         //UINT8         QuietBoot;
  0x0,         //UINT8         CsmControl;
  0x0,         //UINT8         BoardId; // Need to detect Board Id during setup option for CR

  0x0,         //UINT8         MinInverterPWM;
  //
  // Thermal Policy Values
  //
  0x1,         //UINT8         EnableDigitalThermalSensor;
  0x0,         //UINT8         PassiveThermalTripPoint;
  0x1,         //UINT8         PassiveTc1Value;
  0x5,         //UINT8         PassiveTc2Value;
  0x32,         //UINT8         PassiveTspValue;
  0x0,         //UINT8         DisableActiveTripPoints;
  0x0,         //UINT8         CriticalThermalTripPoint;
  0x0,         //UINT8         DeepStandby;
  0x0,         //UINT8         AlsEnable;
  0x0,         //UINT8         IgdLcdIBia;
  0x1,         //UINT8         LogBootTime;
  //
  // EM-1 related
  //
  0x0,         //UINT16        IaAppsRun;
  0x0,         //UINT16        IaAppsRunCR;
  0x0,         //UINT8         IaAppsCap;
  0x0,         //UINT8         CapOrVoltFlag;
  0x0,         //UINT8         BootOnInvalidBatt;

  0x0,         //UINT8         IffsEnable;
  0x0,         //UINT8         IffsOnS3RtcWake;
  0x0,         //UINT8         IffsS3WakeTimerMin;
  0x0,         //UINT8         IffsOnS3CritBattWake;
  0x0,         //UINT8         IffsCritBattWakeThreshold;
  0x0,         //UINT8         ScramblerSupport;
  0x0,         //UINT8         SecureBoot;
  0x0,         //UINT8         SecureBootCustomMode;
  0x0,         //UINT8         SecureBootUserPhysicalPresent;
  0x0,         //UINT8         CoreFreMultipSelect;
  0x0,         //UINT8         MaxCState;
  0x0,         //UINT8         PanelScaling;
  0x0,         //UINT8         IgdLcdIGmchBlc;
  0x0,         //UINT8         SecEnable;
  0x0,         //UINT8         SecFlashUpdate;
  0x0,         //UINT8         SecFirmwareUpdate;
  0x0,         //UINT8         MeasuredBootEnable;
  0x0,         //UINT8         UseProductKey;
  //
  // Image Signal Processor PCI Device Configuration
  //
  0x0,         //UINT8         ISPDevSel;
  0x0,         //UINT8         ISPEn;

  0x0,         //UINT8         Tdt;
  0x0,         //UINT8         Recovery;
  0x0,         //UINT8         Suspend;

  0x0,         //UINT8         TdtState;
  0x0,         //UINT8         TdtEnrolled;
  0x0,         //UINT8         PBAEnable;
  //
  // ISCT Configuration
  //
  0x0,         //UINT8         IsctConfiguration;
  0x1,         //UINT8         IsctNotificationControl;
  0x1,         //UINT8         IsctWlanPowerControl;
  0x0,         //UINT8         IsctWwanPowerControl;
  0x1,         //UINT8         IsctSleepDurationFormat;
  0x1,         //UINT8         IsctRFKillSupport;
  0x0,         //UINT8         WlanNGFFCardPresence;
  0x0,         //UINT8         WlanUHPAMCardPresence;
  0x0,         //UINT8         PchFSAOn;       //FSA control

  //
  // South Cluster Area - START
  //
  //
  // Miscellaneous options
  //
  0x0,         //UINT8         SmbusEnabled;
  0x0,         //UINT8         PchSirq;
  0x0,         //UINT8         PchSirqMode;
  0x0,         //UINT8         Hpet;
  0x0,         //UINT8         HpetBootTime;
  0x0,         //UINT8         EnableClockSpreadSpec;
  0x0,         //UINT8         EnablePciClockSpreadSpec;
  0x0,         //UINT8         EnableUsb3ClockSpreadSpec;
  0x0,         //UINT8         EnableDisplayClockSpreadSpec;
  0x0,         //UINT8         EnableSataClockSpreadSpec;
  0x1,         //UINT8         StateAfterG3;
  0x0,         //UINT8         UartInterface;
  0x0,         //UINT8         IspLpePltClk;
  0x0,         //UINT8         UsbDebug;
  0x0,         //UINT8         ConfigureCfioOnSx;
  //
  // Security Config
  //
  0x1,         //UINT8         PchRtcLock;
  0x1,         //UINT8         PchBiosLock;

  //
  // SCC Configuration
  //
  0x2,         //UINT8         ScceMMCEnabled;
  0x0,         //UINT8         SccSdioEnabled;
  0x2,         //UINT8         SccSdcardEnabled;
  //
  // LPSS Configuration
  //
  0x1,         //UINT8         GpioAcpiEnabled;
  0x0,         //UINT16        Sdcard1p8vSwitchingDelay;
  0xFA,         //UINT16        Sdcard3p3vDischargeDelay;
  0x2,         //UINT8         LpssDma1Enabled;
  0x2,         //UINT8         LpssI2C0Enabled;
  0x2,         //UINT8         LpssI2C1Enabled;
  0x2,         //UINT8         LpssI2C2Enabled;
  0x2,         //UINT8         LpssI2C3Enabled;
  0x2,         //UINT8         LpssI2C4Enabled;
  0x2,         //UINT8         LpssI2C5Enabled;
  0x2,         //UINT8         LpssI2C6Enabled;
  0x2,         //UINT8         LpssDma0Enabled;
  0x0,         //UINT8         LpssPwm0Enabled;
  0x0,         //UINT8         LpssPwm1Enabled;
  0x2,         //UINT8         LpssHsuart0Enabled;
  0x2,         //UINT8         LpssHsuart1Enabled;
  0x0,         //UINT8         LpssSpi1Enabled;
  0x0,         //UINT8         LpssSpi2Enabled;
  0x0,         //UINT8         LpssSpi3Enabled;
  0x0,         //UINT8         I2CTouchAd;
  0x1,         //UINT8         BTModule;
  0x1,         //UINT8         RvpCameraDevSel;
  0x1,         //UINT8         EbCameraDevSel;
  0x1,         //UINT8         SecureNfc;
  0x0,         //UINT8         Bcm4356;
  0x0,         //UINT8         GpsEnable;

  //
  // Usb Config
  //
  0x1,         //UINT8         PchUsb30Mode;
  0x0,         //UINT8         PchSsicEnable;
  0x1,         //UINT8         PchUsbSsicHsRate;
  0x1,         //UINT8         PchUsbSsicInitSequence;
  0x0,  0x0,        //UINT8         PchUsbSsicPort[PCH_SSIC_MAX_PORTS];
  0x1,  0x1,        //UINT8         PchUsbHsicPort[PCH_HSIC_MAX_PORTS];
  0x2,         //UINT8         PchUsb2PhyPgEnabled;
  0x0,         //UINT8         PchUsbOtg;
  0x1,         //UINT8         PchUsbVbusOn;       // OTG VBUS control

  //
  // Ish Config
  //
  0x0,         //UINT8         PchIshEnabled;
  0x0,         //UINT8         IshDebuggerEnabled;

  //
  // SATA Config
  //
  0x1,         //UINT8         PchSata;
  0x1,         //UINT8         SataInterfaceMode;
  0x3,         //UINT8         SataInterfaceSpeed;
  0x1,   0x1,       //UINT8         SataPort[2];
  0x0,   0x0,       //UINT8         SataHotPlug[2];
  0x1,   0x1,       //UINT8         SataMechanicalSw[2];
  0x0,   0x0,       //UINT8         SataSpinUp[2];
  0x0,   0x0,       //UINT8         SataDevSlp[2];
  0x0,   0x0,       //UINT8         SataExternal[2];
  0x0,         //UINT8         SataRaidR0;
  0x0,         //UINT8         SataRaidR1;
  0x0,         //UINT8         SataRaidR10;
  0x0,         //UINT8         SataRaidR5;
  0x0,         //UINT8         SataRaidIrrt;
  0x0,         //UINT8         SataRaidOub;
  0x1,         //UINT8         SataHddlk;
  0x1,         //UINT8         SataLedl;
  0x0,         //UINT8         SataRaidIooe;
  0x0,         //UINT8         SataAlternateId;
  0x1,         //UINT8         SataSalp;
  0x0,         //UINT8         SataTestMode;

  //
  // PCI_EXPRESS_CONFIG, 4 ROOT PORTS
  //
  0x1, 0x1, 0x1, 0x1,          //UINT8         PcieRootPortEn[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortAspm[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortURE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortFEE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortNFE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortCEE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortSFE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortSNE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortSCE[PCH_PCIE_MAX_ROOT_PORTS];
  0x1, 0x1, 0x1, 0x1,        //UINT8         PcieRootPortPMCE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortESE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,          //UINT8         PcieRootPortHPE[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,          //UINT8         PcieRootPortSpeed[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortTHS[PCH_PCIE_MAX_ROOT_PORTS];
  0x3, 0x3, 0x3, 0x3,          //UINT8         PcieRootPortL1SubStates[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieRootPortNccSsc[PCH_PCIE_MAX_ROOT_PORTS];
  0x0, 0x0, 0x0, 0x0,          //UINT8         PcieRootPortTxEqDeemphSelection[PCH_PCIE_MAX_ROOT_PORTS];

  //
  // PCI Bridge Resources
  //
  0x0, 0x0, 0x0, 0x0,         //UINT8         PcieExtraBusRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  0xA, 0xA, 0xA, 0xA,         //UINT8         PcieMemRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  0x4, 0x4, 0x4, 0x4,         //UINT8         PcieIoRsvd[PCH_PCIE_MAX_ROOT_PORTS];

  //
  // PCI Express S0ix Config
  //
  0x1,         //UINT8         PcieS0iX;
  0x2,         //UINT8         D0S0IxPolicy;
  0x1,         //UINT8         ClkReqEnable;
  0x0,         //UINT8         ClkReq;
  0x2,         //UINT8         LtrLatencyScale;
  0x96,         //UINT8         LtrLatencyValue;

  //
  // Audio Configuration
  //
  0x0,         //UINT8         PchLpeEnabled;
  0x1,         //UINT8         PchAzalia;
  0x1,         //UINT8         AzaliaVCiEnable;
  0x0,         //UINT8         AzaliaDs;
  0x1,         //UINT8         AzaliaPme;
  0x1,         //UINT8         HdmiCodec;
  0x1,         //UINT8         HdmiCodecPortB;
  0x1,         //UINT8         HdmiCodecPortC;
  0x1,         //UINT8         HdmiCodecPortD;
  //
  // South Cluster Area - END
  //

  0x2,         //UINT8         GTTSize;
  //
  // DVMT5.0 Graphic memory setting
  //
  0x2,         //UINT8         IgdDvmt50PreAlloc;
  0x2,         //UINT8         IgdDvmt50TotalAlloc;
  0x0,         //UINT8         IgdTurboEnabled;
  0x0,         //UINT8         EnableRenderStandby;
  0x1,         //UINT8         GOPEnable;
  0x5,         //UINT8         GOPBrightnessLevel;                     // Gop Brightness level
  0x0,         //UINT8         PanelConfig;
  0x0,         //UINT8         PanelVendor;
  0x1,         //UINT8         PavpMode;
  0x0,         //UINT8         EnablePR3;
  0x0,         //UINT8         Wopcmsz;
  0x0,         //UINT8         UnsolicitedAttackOverride;

  0x0,         //UINT8         SeCOpEnable;
  0x0,         //UINT8         SeCModeEnable;
  0x0,         //UINT8         SeCEOPEnable;
  0x0,         //UINT8         SeCEOPDone;

  0x2,         //UINT8         LidStatus;
  0x0,         //UINT8         PowerMeterLock;
  0x0,         //UINT8         EuControl;
  0x0,         //UINT8         SdpProfile;                              // DPTF:  an enumeration for Brand Strings.
  0x0,         //UINT8         CameraSelect;
  0x0,         //UINT8         F22Rework;
  0x0,         //UINT8         EnableDptf;                              // Option to enable/disable DPTF
  0x0,         //UINT16        ProcCriticalTemp;                        // Processor critical temperature
  0x0,         //UINT16        ProcPassiveTemp;                         // Processor passive temperature

  0x0,         //UINT16        ActiveThermalTripPointSA;                // Processor active temperature
  0x0,         //UINT16        CriticalThermalTripPointSA;              // Processor critical temperature
  0x0,         //UINT16        CR3ThermalTripPointSA;                   // Processor CR3 temperature
  0x0,         //UINT16        HotThermalTripPointSA;                   // Processor Hot temperature
  0x0,         //UINT16        PassiveThermalTripPointSA;               // Processor passive temperature

  0x0,         //UINT16        GenericActiveTemp0;                      // Active temperature value for generic sensor0 participant
  0x0,         //UINT16        GenericCriticalTemp0;                    // Critical temperature value for generic sensor0 participant
  0x0,         //UINT16        GenericCR3Temp0;                         // CR3 temperature value for generic sensor0 participant
  0x0,         //UINT16        GenericHotTemp0;                         // Hot temperature value for generic sensor0 participant
  0x0,         //UINT16        GenericPassiveTemp0;                     // Passive temperature value for generic sensor0 participant
  0x0,         //UINT16        GenericActiveTemp1;                      // Active temperature value for generic sensor1 participant
  0x0,         //UINT16        GenericCriticalTemp1;                    // Critical temperature value for generic sensor1 participant
  0x0,         //UINT16        GenericCR3Temp1;                         // CR3 temperature value for generic sensor1 participant
  0x0,         //UINT16        GenericHotTemp1;                         // Hot temperature value for generic sensor1 participant
  0x0,         //UINT16        GenericPassiveTemp1;                     // Passive temperature value for generic sensor1 participant
  0x0,         //UINT16        GenericActiveTemp2;                      // Active temperature value for generic sensor2 participant
  0x0,         //UINT16        GenericCriticalTemp2;                    // Critical temperature value for generic sensor2 participant
  0x0,         //UINT16        GenericCR3Temp2;                         // CR3 temperature value for generic sensor2 participant
  0x0,         //UINT16        GenericHotTemp2;                         // Hot temperature value for generic sensor2 participant
  0x0,         //UINT16        GenericPassiveTemp2;                     // Passive temperature value for generic sensor2 participant
  0x0,         //UINT16        GenericCriticalTemp3;                    // Critical temperature value for generic sensor3 participant
  0x0,         //UINT16        GenericPassiveTemp3;                     // Passive temperature value for generic sensor3 participant
  0x0,         //UINT16        GenericCriticalTemp4;                    // Critical temperature value for generic sensor3 participant
  0x0,         //UINT16        GenericPassiveTemp4;                     // Passive temperature value for generic sensor3 participant
  0x0,         //UINT8         Clpm;                                    // Current low power mode
  0x0,         //UINT8         SuperDebug;                              // DPTF Super debug option
  0x0,         //UINT32        LPOEnable;                               // DPTF: Instructs the policy to use Active Cores if they are available. If this option is set to 0, then policy does not use any active core controls ?even if they are available
  0x0,         //UINT32        LPOStartPState;                          // DPTF: Instructs the policy when to initiate Active Core control if enabled. Returns P state index.
  0x0,         //UINT32        LPOStepSize;                             // DPTF: Instructs the policy to take away logical processors in the specified percentage steps
  0x0,         //UINT32        LPOPowerControlSetting;                  // DPTF: Instructs the policy whether to use Core offliing or SMT offlining if Active core control is enabled to be used in P0 or when power control is applied. 1 ?SMT Off lining 2- Core Off lining
  0x0,         //UINT32        LPOPerformanceControlSetting;            // DPTF: Instructs the policy whether to use Core offliing or SMT offlining if Active core control is enabled to be used in P1 or when performance control is applied.1 ?SMT Off lining 2- Core Off lining
  0x0,         //UINT8         EnableDppm;                              // DPTF: Controls DPPM Policies (enabled/disabled)
  0x0,         //UINT8         DptfProcessor;
  0x0,         //UINT8         DptfSysThermal0;
  0x0,         //UINT8         DptfSysThermal1;
  0x0,         //UINT8         DptfSysThermal2;
  0x0,         //UINT8         DptfSysThermal3;
  0x0,         //UINT8         DptfSysThermal4;
  0x0,         //UINT8         DptfChargerDevice;
  0x0,         //UINT8         DptfDisplayDevice;
  0x0,         //UINT8         DptfSocDevice;
  0x0,         //UINT8         BidirectionalProchotEnable;
  0x0,         //UINT8         ThermalMonitoring;
  0x0,         //UINT8         ThermalMonitoringHot;
  0x0,         //UINT8         ThermalMonitoringSystherm0Hot;
  0x0,         //UINT8         ThermalMonitoringSystherm1Hot;
  0x0,         //UINT8         ThermalMonitoringSystherm2Hot;
  0x0,         //UINT8         DisplayHighLimit;
  0x0,         //UINT8         DisplayLowLimit;
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        //UINT8         AmbientConstants[6];
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        //UINT8         AmbientConstantSign[6];
  0x0,         //UINT8         AmbientTripPointChange;                  // DPTF: Controls whether _ATI changes other participant's trip point(enabled/disabled)
  0x1,         //UINT8         DptfAllowHigherPerformance;              // DPTF: Allow higher performance on AC/USB - (Enable/Disable)
  0x0,         //UINT8         DptfWwanDevice;                          // DPTF: Presence of WWAN Device
  0x300,         //UINT32        DptfWwanCrt;                             // DPTF: WWAN critical temperature
  0x0,         //UINT32        DptfWwanPsv;                             // DPTF: WWAN Passive Temperature
  0x0,         //UINT8         EnablePassivePolicy;                     // DPTF: Passive Policy enable/disable
  0x0,         //UINT8         EnableCriticalPolicy;                    // DPTF: Critical Policy enable/disable
  0x0,         //UINT8         EnableActivePolicy;                      // DPTF: Active Policy enable/disable
  0x0,         //UINT8         PmicEnable;
  0x3,         //UINT8         S0ix;
  0x1,         //UINT8         TSEGSizeSel;
  0x0,         //UINT8         ACPIMemDbg;
  0x0,         //UINT8         ExISupport;
  0x0,         //UINT8         BatteryChargingSolution;                 // 0-non ULPMC 1-ULPMC

  0x0,         //UINT8         PnpSettings;
  0x0,         //UINT8         MfgMode;
  0x0,         //UINT8         CRIDSettings;
  0x0,         //UINT8         ULPMCFWLock;
  0x0,         //UINT8         PssEnabled;
  0x0,         //UINT8         PmWeights;
  0x0,         //UINT8         PdmEnable;
  0x0,         //UINT8         PDMConfig;
  0x0,         //UINT16        LmMemSize;
  0x0,         //UINT8         PunitBIOSConfig;
  0x0,         //UINT8         LpssSdioMode;
  0x0,         //UINT8         ENDBG2;
  0x0,         //UINT8         IshPullUp;
  0x0,         //UINT8         TristateLpc;
  0x0,         //UINT8         UsbXhciLpmSupport;
  0x0,         //UINT8         VirtualKbEnable;
  0x0,         //UINT8         SlpS0ixN;
  0x0,         //UINT8         EnableAESNI;
  0x0,         //UINT8         SecureErase;

  //
  // Memory Config Tools
  //
  0x0,         //UINT8         MrcEvRMT;
  0x0,         //UINT8         MrcCmdRMT;
  0x0,         //UINT8         MrcDvfsEnable;
  0x0,         //UINT8         MrcFreqASel;
  0x0,         //UINT8         MrcFreqBSel;
  0x0,         //UINT8         MrcLPDDR3ChipSel;
  0x0,         //UINT8         MrcChannelSel;
  0x0,         //UINT8         MrcDynamicSr;
  0x0,         //UINT8         MrcChannelSel_3_0;
  0x0,         //UINT8         MrcChannelSel_4;
  0x0,         //UINT8         MrcAutoDetectDram;
  0x0,         //UINT8         Sku;
  0x4,         //UINT8         MrcPm5Enable;
  0x0,         //UINT8         MrcBankAddressHashingEnable;
  0x0,         //UINT8         MrcRankSelInterleave;
  0x0,         //UINT8         MrcConfigChanged;
  0x0,         //UINT8         MrcDdrType;
  0x0,         //UINT8         MrcDdr2nMode;
  0x0,         //UINT8         MrcRxPwrTrainingDisable;
  0x0,         //UINT8         MrcTxPwrTrainingDisable;
  0x0,         //UINT8         MrcFastBootDisable;
  0x0,         //UINT8         MrcScramblerDisable;
  0x0,         //UINT8         MrcSpeedGrade;
  0x0,         //UINT8         MrcLPDDR3DeviceDensity;
  0x0,         //UINT8         MrcDebugMsgLevel;
  0x0,         //UINT8         DrpLockDisable;
  0x0,         //UINT8         ReutLockDisable;
  0x0,         //UINT8         RhPrevention;

  0x0,         //UINT8         MmioSize;
  0x0,         //UINT8         DroidBoot;
  0x0,         //UINT8         AndroidBoot;
  0x0,         //UINT8         Ellensburg;
  0x0,         //UINT8         CriticalBatteryLimit;
  0x0,         //UINT8         CriticalBatteryLimitFeature;
  0x0,         //UINT8         EmmcDriverMode;
  0x0,         //UINT8         EmmcRxTuningEnable;
  0x0,         //UINT8         EmmcTxTuningEnable;

  0x0,         //UINT8         SAR1;

  0x0,         //UINT8         DisableCodec262;
  0x0,         //UINT8         PcieDynamicGating;        // Need PMC enable it first from PMC 0x3_12 MCU 318.
  0x0,         //UINT8         VirtualButtonEnable;
  0x0,         //UINT8         RotationLock;
  0x0,         //UINT8         ConvertibleState;
  0x0,         //UINT8         DockIndicator;
  0x0,         //UINT8         WIFIModule;
  0x0,         //UINT8         SvidConfig;
  0x0,         //UINT8         PciExpNative;
  0x0,         //UINT8         OsSelection;
  0x0,         //UINT8         PlatformDirectOsSelect; //If set to 1 (TRUE), platform method (GPI on Cherry Hill) will be used to select OS.
                                        //If set to 0 (FALSE), OS selection option in setup menu will be used to select OS.

  0x0,         //UINT8         MipiDsi;
  0x0,         //UINT8         AndroidSBIntegration;
  0x0,         //UINT8         AcpiDevNodeDis;
  0x0,         //UINT8         AcpiModemSel;

  //
  // SPID config region
  //
  0x0,         //UINT8         SPIDAutoDetect;
  0x0,         //UINT16        SPIDCustomerID;
  0x0,         //UINT16        SPIDVendorID;
  0x0,         //UINT16        SPIDDeviceManufacturerID;
  0x0,         //UINT16        SPIDPlatformFamilyID;
  0x0,         //UINT16        SPIDProductLineID;
  0x0,         //UINT16        SPIDHardwareID;
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,         //UINT8         SPIDFru[20];
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  //
  // OEM1 table
  //
  0x0,         //UINT8         BatIdDbiBase;
  0x0,         //UINT8         BatIdAnlgBase;
  0x0,         //UINT16        VBattFreqLimit;
  0x4,         //UINT8         CapFreqIdx;
  0x0,         //UINT8         BTHStatus;
  0x0,         //UINT8         AudioCodecSuppport;
  0x0,         //UINT8         ChargingEnable;
  0x0,         //UINT8         ChargingLpmEnable;
  0x0,         //UINT16        Str2TspValue;
  0x0,         //UINT8         VBIOS5f35h;
  0x0,         //UINT8         VBIOS5f40h;
  0x0,         //UINT8         Backlight;
  0x0,         //UINT8         PunitPwrConfigDisable;
  0x0,         //UINT8         FlashLessMdm;
  0x0,         //UINT8         EnableMSCustomSdbusDriver;
  0x0,         //UINT8         XdbGpioTrst;
  0x0,         //UINT8         FirstBootIndicator;
  0x0,         //UINT8         ConnectAllCtrlsFlag;
  0x0,         //UINT8         EnterDnxFastBoot;
  0x0,         //UINT8         ToggleSelfClkDisabling;
  0x0,         //UINT8         GPSHIDSelection;
  0x0        //UINT8         HighPerfMode;

};

#endif // #ifndef _SETUP_VARIABLE_DEFAULT_H
