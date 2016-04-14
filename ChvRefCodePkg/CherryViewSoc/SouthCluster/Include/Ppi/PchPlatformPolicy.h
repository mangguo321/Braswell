/** @file
  PCH policy PPI produced by a platform driver specifying various
  expected PCH settings. This PPI is consumed by the PCH PEI modules.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef PCH_PLATFORM_POLICY_H_
#define PCH_PLATFORM_POLICY_H_
//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//

#include "PchRegs.h"

extern EFI_GUID                         gPchPlatformPolicyPpiGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_PLATFORM_POLICY_PPI PCH_PLATFORM_POLICY_PPI;

///
/// PPI revision number
/// Any backwards compatible changes to this PPI will result in an update in the revision number
/// Major changes will require publication of a new PPI
///
/// Revision 1:    Original version
///
#define PCH_PLATFORM_POLICY_PPI_REVISION_1  1
//
// Generic definitions for device enabling/disabling used by PCH code.
//
#define PCH_DEVICE_ENABLE   1
#define PCH_DEVICE_DISABLE  0

//
// ---------------------------- HPET Config -----------------------------
//
typedef struct {
  BOOLEAN Enable; ///< Determines if enable HPET function
  UINT32  Base;   ///< The HPET base address
} PCH_HPET_CONFIG;

#ifdef SATA_SUPPORT

///
/// ---------------------------- SATA Config -----------------------------
///
typedef enum {
  PchSataModeIde  = 0,
  PchSataModeAhci = 1,
  PchSataModeRaid = 2,
  PchSataModeMax
} PCH_SATA_MODE;

typedef enum {
  PchSataSpeedGen1 = 1,
  PchSataSpeedGen2,
  PchSataSpeedGen3
} PCH_SATA_SPEED;

typedef struct {
  UINT8 MechSw          : 1;    /// 0: Disable; 1: Enable
  UINT8 External        : 1;    /// 0: Disable; 1: Enable
  UINT8 SpinUp          : 1;    /// 0: Disable; 1: Enable the COMRESET initialization Sequence to the device
  UINT8 Rsvdbits        : 5;    /// Reserved fields for future expansion w/o protocol change
} PCH_SATA_PORT_SETTINGS_PEI;

typedef struct {
  PCH_SATA_PORT_SETTINGS_PEI  PortSettings[PCH_AHCI_MAX_PORTS];
  UINT8                       SalpSupport     : 1;    ///< 0: Disable; 1: Enable Aggressive Link Power Management
  UINT8                       TestMode        : 1;    ///< 0: Disable; 1: Allow entrance to the PCH SATA test modes
  UINT8                       Rsvdbits        : 6;    ///< Reserved fields for future expansion w/o protocol change
  PCH_SATA_MODE               SataMode;
  PCH_SATA_SPEED              SpeedSupport;           ///< Indicates the maximum speed the SATA controller can support
                                                      ///< 1h: 1.5 Gb/s (Gen 1); 2h: 3 Gb/s (Gen 2); 3h: 6 Gb/s (Gen 3)
} PCH_SATA_CONFIG_PEI;
#endif

#ifdef PCIESC_SUPPORT
//
// ---------------------------- PCI Express Config -----------------------------
//
typedef enum {
  PchPcieAuto,
  PchPcieGen1,
  PchPcieGen2
} PCH_PCIE_SPEED;
///
/// Refer to PCH EDS for the PCH implementation values corresponding
/// to below PCI-E spec defined ranges
///
typedef enum {
  PchPcieL1SubstatesDisabledPei,
  PchPcieL1SubstatesL1_1Pei,
  PchPcieL1SubstatesL1_2Pei,
  PchPcieL1SubstatesL1_1_2Pei,
  PchPcieL1SubstatesMaxPei
} PCH_PCIE_EXPRESS_L1SUBSTATES_CONTROL_PEI;

///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  PchPcieAspmDisabledPei,
  PchPcieAspmL0sPei,
  PchPcieAspmL1Pei,
  PchPcieAspmL0sL1Pei,
  PchPcieAspmAutoConfigPei,
  PchPcieAspmMaxPei
} PCH_PCI_EXPRESS_ASPM_CONTROL_PEI;

typedef enum {
  PchPcieCompletionTO_DefaultPei,
  PchPcieCompletionTO_50_100usPei,
  PchPcieCompletionTO_1_10msPei,
  PchPcieCompletionTO_16_55msPei,
  PchPcieCompletionTO_65_210msPei,
  PchPcieCompletionTO_260_900msPei,
  PchPcieCompletionTO_1_3P5sPei,
  PchPcieCompletionTO_4_13sPei,
  PchPcieCompletionTO_17_64sPei,
  PchPcieCompletionTO_DisabledPei
} PCH_PCIE_COMPLETION_TIMEOUT_PEI;

typedef struct {
  UINT8 Enable                          : 1;        ///< Root Port enabling, 0: Disable; 1: Enable.
  UINT8 Hide                            : 1;        ///< Whether or not to hide the configuration space of this port.
  UINT8 SlotImplemented                 : 1;        ///< Indicates whether the root port is connected to a slot.
  UINT8 HotPlug                         : 1;        ///< Indicate whether the root port is hot plug available.
  UINT8 PmSci                           : 1;        ///< Indicate whether the root port power manager SCI is enabled.
  UINT8 ExtSync                         : 1;        ///< Indicate whether the extended synch is enabled.
  UINT8 Rsvdbits                        : 2;
  //
  // Error handlings
  //
  UINT8 UnsupportedRequestReport        : 1;        ///< Indicate whether the Unsupported Request Report is enabled.
  UINT8 FatalErrorReport                : 1;        ///< Indicate whether the Fatal Error Report is enabled.
  UINT8 NoFatalErrorReport              : 1;        ///< Indicate whether the No Fatal Error Report is enabled.
  UINT8 CorrectableErrorReport          : 1;        ///< Indicate whether the Correctable Error Report is enabled.
  UINT8 PmeInterrupt                    : 1;        ///< Indicate whether the PME Interrupt is enabled.
  UINT8 SystemErrorOnFatalError         : 1;        ///< Indicate whether the System Error on Fatal Error is enabled.
  UINT8 SystemErrorOnNonFatalError      : 1;        ///< Indicate whether the System Error on Non Fatal Error is enabled.
  UINT8 SystemErrorOnCorrectableError   : 1;        ///< Indicate whether the System Error on Correctable Error is enabled.

  UINT8 AdvancedErrorReporting          : 1;        ///< Indicate whether the Advanced Error Reporting is enabled
  UINT8 TransmitterHalfSwing            : 1;        ///< Indicate whether the Transmitter Half Swing is enabled.
  UINT8 NonCommonClockSscMode           : 1;        ///< Indicate whether the root port assumed to be operating in non-common clock mode with SSC enabled.
  UINT8 Reserved                        : 5;        ///< Reserved fields for future expansion w/o protocol change

  UINT8 FunctionNumber;                             ///< The function number this root port is mapped to
  UINT8 PhysicalSlotNumber;                         ///< Indicates the slot number for the root port.
  UINT8 DetectTime;                                 ///< The DetectTime will be utilized to decide how much time we want to poll to detect PCIe device, The unit is millisecond.
  PCH_PCIE_COMPLETION_TIMEOUT_PEI   CompletionTimeout;  ///< The completion timeout configuration of the root port
  PCH_PCI_EXPRESS_ASPM_CONTROL_PEI  Aspm;               ///< The ASPM configuration of the root port
  PCH_PCIE_EXPRESS_L1SUBSTATES_CONTROL_PEI L1Substates; ///< The L1 Substates configuration of the root port
  UINT8 TxEqdeEmphSelection;                        ///< When the Link is operating at 5.0 GT/s speed, select the level of de-emphasis for an Upstream component.
} PCH_PCI_EXPRESS_ROOT_PORT_CONFIG_PEI;

typedef struct {
  ///
  ///< Determines each PCIE Port speed capability. 0: Auto; 1: Gen1; 2: Gen2.
  ///
  PCH_PCIE_SPEED  PcieSpeed[PCH_PCIE_MAX_ROOT_PORTS];
  ///
  /// Temp Bus Number range available to be assigned to each root port and its downstream
  /// devices for initialization of these devices before PCI Bus enumeration.
  ///
  UINT8                             TempRootPortBusNumMin;
  ///
  /// These members describe the configuration of each PCH PCIe root port.
  ///
  PCH_PCI_EXPRESS_ROOT_PORT_CONFIG_PEI  RootPort[PCH_PCIE_MAX_ROOT_PORTS];
  ///
  /// Temp Bus Number range available to be assigned to each root port and its downstream
  /// devices for initialization of these devices before PCI Bus enumeration.
  ///
  UINT8                             TempRootPortBusNumMax;
  ///
  /// This member describes whether the PCI Express Clock Gating for each root port
  /// is enabled by platform modules.
  ///
  UINT8                             RootPortClockGating : 1;
  UINT8                             Rsvdbits            : 7;  ///< Reserved fields for future expansion w/o protocol change
} PCH_PCIE_CONFIG;

//
// --------------------------- Power Optimizer Config ------------------------------
//
typedef struct {
  UINT8         LtrEnable                     :1;   ///< Latency Tolerance Reporting Mechanism.
  UINT8         ObffEnable                    :1;   ///< Pcie end point Optimized Buffer Flush/Fill (OBFF) capability for the root port.
} PCH_PCIE_PWR_OPT_PEI;

typedef struct {
  UINT16  VendorId; ///< PCI configuration space offset 0
  UINT16  DeviceId; ///< PCI configuration space offset 2
  UINT8   RevId;    ///< PCI configuration space offset 8; 0xFF means all steppings
/**
  SnoopLatency bit definition
  Note: All Reserved bits must be set to 0

  BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                When clear values in bits 9:0 will be ignored
  BITS[14:13] - Reserved
  BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                000b - 1 ns
                001b - 32 ns
                010b - 1024 ns
                011b - 32,768 ns
                100b - 1,048,576 ns
                101b - 33,554,432 ns
                110b - Reserved
                111b - Reserved
  BITS[9:0]   - Snoop Latency Value. The value in these bits will be multiplied with
                the scale in bits 12:10
**/
  UINT16  SnoopLatency;
/**
  NonSnoopLatency bit definition
  Note: All Reserved bits must be set to 0

  BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                When clear values in bits 9:0 will be ignored
  BITS[14:13] - Reserved
  BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                000b - 1 ns
                001b - 32 ns
                010b - 1024 ns
                011b - 32,768 ns
                100b - 1,048,576 ns
                101b - 33,554,432 ns
                110b - Reserved
                111b - Reserved
  BITS[9:0]   - Non Snoop Latency Value. The value in these bits will be multiplied with
                the scale in bits 12:10
**/
  UINT16  NonSnoopLatency;
} PCH_PCIE_DEVICE_LTR_OVERRIDE_PEI;

typedef struct {
  UINT8  NumOfDevLtrOverride;                            ///< Number of Pci Express card listed in LTR override table
  PCH_PCIE_DEVICE_LTR_OVERRIDE_PEI *DevLtrOverride;          ///< Pointer to Pci Express devices LTR override table
  PCH_PCIE_PWR_OPT_PEI             PchPwrOptPcie[PCH_PCIE_MAX_ROOT_PORTS]; ///< related configuration for PCIE ports power optimization.
} PCH_PCIE_PWR_OPT_CONFIG;
#endif

//
// ---------------------------- IO APIC Config -----------------------------
//
typedef struct {
  UINT8 IoApicId;            ///< This member determines IOAPIC ID.
} PCH_IOAPIC_CONFIG;

//
// ------------------------------ USB Config -------------------------------
//
typedef enum {
  SsicHsRate_A = 0,
  SsicHsRate_B = 1
} PCH_SSIC_HS_RATE;

typedef enum {
  SsicLaneHs_G1 = 0,
  SsicLaneHs_G2 = 1,
  SsicLaneHs_G3 = 2
} PCH_SSIC_LANE_HS;

typedef enum {
  PchUsb2PhyPgDisabled = 0,
  PchUsb2PhyPgEnabled = 1,
  PchUsbPhyPgAuto = 2
} PCH_USB2_PHY_PG;

//
// ---------------------------- PCH Platform Data -----------------------------
//
typedef struct {
  UINT8            SmmBwp           : 1;                          ///< 0: Clear SMM_BWP bit; 1: Set SMM_BWP bit.
  UINT8            Rsvdbits         : 7;
  UINT32           TempMemBaseAddr;                               ///< Temporary Memory Base Address for PCI devices to be
                                                                  ///< used to initialize MMIO registers. Minimum size is
                                                                  ///< 64KB bytes
  UINT8            SsicEnable;                                    ///< Enable / Disable SSIC initialization
  PCH_SSIC_HS_RATE SsicHsRate;                                    ///< SSIC HS Rate
  UINT8            SsicInitSequence;                              ///< Sequence 1 / Sequence 2
  PCH_SSIC_LANE_HS SsicLaneHs[PCH_SSIC_MAX_PORTS];                ///< SSIC Lane HS Gear
  UINT8            SsicPortEnable[PCH_SSIC_MAX_PORTS];            ///< SSIC Port Enable, 0: Disable; 1: Enable.
  UINT8            HsicPortEnable[PCH_HSIC_MAX_PORTS];            ///< HSIC Port Enable, 0: Disable; 1: Enable.
  PCH_USB2_PHY_PG  Usb2PhyPgEnabled;                              ///< USB2 PHY Power Gating Enable, 0: Disable; 1: Enable; 2: Auto.
  UINT8            PerPortPeTxiSet[PCH_USB_MAX_PHYSICAL_PORTS];   ///< Value of USB2 PHY PERPORTPETXISET Register, USB2_PER_PORT_PPX [13:11]
  UINT8            PerPortTxiSet[PCH_USB_MAX_PHYSICAL_PORTS];     ///< Value of USB2 PHY PERPORTTXISET Register, USB2_PER_PORT_PPX [10:8]
  UINT8            IUsbTxEmphasisEn[PCH_USB_MAX_PHYSICAL_PORTS];  ///< Value of USB2 PHY IUSBTXEMPHASISEN Register, USB2_PER_PORT_2_PPX [24:23]
  UINT8            PerPortTxPeHalf[PCH_USB_MAX_PHYSICAL_PORTS];   ///< Value of USB2 PHY PERPORTTXPEHALF Register, USB2_PER_PORT_PPX [14]
  UINT8            Ow2tapgen2deemph3p5[PCH_XHCI_MAX_USB3_PORTS];   ///< Value of USB3 PHY ow2tapgen2deemph3p5 Register, TX_DWORD4[15:8]
} PCH_PLATFORM_DATA;

///
/// ---------------------------- PCH SSC Settings -----------------------------
///
typedef struct {
  UINT8                         Usb3ClkSsc; /// Enable Spread for USB3 clocks. 0: Disable; 1: Enable
  UINT8                         DispClkSsc; /// Enable Spread for Display clocks. 0: Disable; 1: Enable
  UINT8                         SataClkSsc; /// Enable Spread for SATA clocks. 0: Disable; 1: Enable
} PCH_SSC_SETTINGS;

///
/// ------------ General PCH Platform Policy PPI definition ------------
///
struct _PCH_PLATFORM_POLICY_PPI {
  ///
  /// This member specifies the revision of the PCH policy PPI. This field is used to
  /// indicate backwards compatible changes to the protocol. Platform code that produces
  /// this PPI must fill with the correct revision value for the PCH reference code
  /// to correctly interpret the content of the PPI fields.
  ///
  UINT8                         Revision;
  UINT8                         BusNumber;      ///< Bus Number of the PCH device.
  UINT32                        SpiBase;        ///< SPI Base Address.
  UINT32                        PmcBase;        ///< PMC Base Address.
  UINT32                        IoBase;         ///< IO Base Address.
  UINT32                        IlbBase;        ///< Intel Legacy Block Base Address.
  UINT32                        Rcba;           ///< Root Complex Base Address.
  UINT32                        MphyBase;       ///< MPHY Base Address.
  UINT32                        PunitBase;      ///< PUNIT Base Address.
  UINT16                        AcpiBase;       ///< ACPI I/O Base address.
  PCH_HPET_CONFIG               *HpetConfig;
#ifdef SATA_SUPPORT
  PCH_SATA_CONFIG_PEI           *SataConfig;
#endif
#ifdef PCIESC_SUPPORT
  ///
  /// This member describes PCI Express controller's related configuration.
  ///
  PCH_PCIE_CONFIG               *PcieConfig;
  ///
  /// This member describes the Power Optimizer configuration.
  ///
  PCH_PCIE_PWR_OPT_CONFIG       *PwrOptConfig;
#endif
  PCH_IOAPIC_CONFIG             *IoApicConfig;
  PCH_PLATFORM_DATA             *PlatformData;
  ///
  /// This member describes the PCH SSC Settings
  ///
  PCH_SSC_SETTINGS              *PchSscSettings;
};

#endif
