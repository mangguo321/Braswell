/** @file
  PCH policy protocol produced by a platform driver specifying various
  expected PCH settings. This protocol is consumed by the PCH drivers.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_PLATFORM_POLICY_H_
#define _PCH_PLATFORM_POLICY_H_

//
#include "PchRegs.h"
#include "Uefi.h"

extern EFI_GUID                                   gDxePchPlatformPolicyProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _DXE_PCH_PLATFORM_POLICY_PROTOCOL  DXE_PCH_PLATFORM_POLICY_PROTOCOL;

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///

///
/// Revision 1: Original version
///
#define DXE_PCH_PLATFORM_POLICY_PROTOCOL_REVISION_1 1

///
/// Generic definitions for device enabling/disabling used by PCH code.
///
#define PCH_DEVICE_ENABLE   1
#define PCH_DEVICE_DISABLE  0

///
/// SW SMI values which are used by PCH Platform Policy Protocol
///
#define SW_SMI_BIOS_LOCK              0xA9
#ifdef PCIESC_SUPPORT
#define SW_SMI_PCIE_ASPM_OVERRIDE     0xAA
#endif

///
/// Pnp Mode Definitions
///
typedef enum {
  PchPnpDisabled         = 0,
  PchPnpPower            = 1,
  PchPnpPerformance      = 2,
  PchPnpPowerPerformance = 3,
  PchPnpModeMax
} PCH_PNP_MODE;

///
/// Device Operating Mode
///
typedef enum {
  PchDisabled = 0,
  PchPciMode  = 1,
  PchAcpiMode = 2,
  PchDevModeMax
} PCH_DEV_MODE;

///
/// Platform Clocks Operating Mode
///
typedef enum {
  PchPltClkDynamic  = 0,
  PchPltClkForceOn  = 1,
  PchPltClkForceOff = 2,
  PchPltClkMaxMode  = 3
} PCH_PLTCLK_MODE;

///
/// SCC eMMC Driver Operating Mode
///
typedef enum {
  PchSccEmmcModeAuto = 0,
  PchSccEmmcMode52MHz = 1,
  PchSccEmmcMode26MHz = 2,
  PchSccEmmcModeBasic = 3
} PCH_SCC_EMMC_MODE;

///
/// ---------------------------- Device Enabling ------------------------------
///
/// PCH Device enablings
///
typedef struct {
  UINT8           Azalia       : 2;  /// 0: Disable; 1: Enable; 2: Auto
  UINT8           Sata         : 1;  /// 0: Disable; 1: Enable
  UINT8           Smbus        : 1;  /// 0: Disable; 1: Enable
  UINT8           RsvdBits     : 4;  /// Reserved fields for future expansion w/o protocol change
  PCH_PLTCLK_MODE PlatformClock[6];  /// 0: Dynamic; 1: Force On; 2: Force Off
  PCH_DEV_MODE    IshEnabled;        /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE    LpeEnabled;        /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
} PCH_DEVICE_ENABLING;

///
/// ---------------------------- USB Config -----------------------------
///
///
/// Overcurrent pins
///
typedef enum {
  PchUsbOverCurrentPin0 = 0,
  PchUsbOverCurrentPin1,
  PchUsbOverCurrentPinSkip,
  PchUsbOverCurrentPinMax
} PCH_USB_OVERCURRENT_PIN;

typedef struct {
  UINT8   Enable            : 1;    /// 0: Disable; 1: Enable. This would take effect while UsbPerPortCtl is enabled
  UINT8   Rsvdbits          : 7;
} PCH_USB_PORT_SETTINGS;

typedef struct {
  PCH_DEV_MODE Enable;              /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
} PCH_USBOTG_CONTROLLER_SETTINGS;

#define PCH_XHCI_MODE_OFF         0
#define PCH_XHCI_MODE_ON          1

typedef struct {
  UINT8 Mode               : 1;    /// 0: Disable; 1: Enable
  UINT8 Rsvdbits           : 7;
} PCH_USB30_CONTROLLER_SETTINGS;

typedef struct {
  UINT8                          SsicEnable;
  PCH_USB_PORT_SETTINGS          Usb20PortSettings[PCH_USB_MAX_PHYSICAL_PORTS];
  PCH_USB_PORT_SETTINGS          Usb30PortSettings[PCH_XHCI_MAX_USB3_PORTS];
  PCH_USB_PORT_SETTINGS          HsicPortSettings[PCH_HSIC_MAX_PORTS];
  PCH_USB_PORT_SETTINGS          SsicPortSettings[PCH_SSIC_MAX_PORTS];
  PCH_USB30_CONTROLLER_SETTINGS  Usb30Settings;
  PCH_USBOTG_CONTROLLER_SETTINGS UsbOtgSettings;
  PCH_USB_OVERCURRENT_PIN        Usb20OverCurrentPins[PCH_USB_MAX_PHYSICAL_PORTS];
  PCH_USB_OVERCURRENT_PIN        Usb30OverCurrentPins[PCH_XHCI_MAX_USB3_PORTS];
} PCH_USB_CONFIG;

//
// ---------------------------- PCI Express Config ----------------------
//
///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  PchPcieAspmDisabled,
  PchPcieAspmL0s,
  PchPcieAspmL1,
  PchPcieAspmL0sL1,
  PchPcieAspmAutoConfig,
  PchPcieAspmMax
} PCH_PCI_EXPRESS_ASPM_CONTROL;

///
/// Refer to PCH EDS for the PCH implementation values corresponding
/// to below PCI-E spec defined ranges
///
typedef enum {
  PchPcieL1SubstatesDisabled,
  PchPcieL1SubstatesL1_1,
  PchPcieL1SubstatesL1_2,
  PchPcieL1SubstatesL1_1_2,
  PchPcieL1SubstatesMax
} PCH_PCIE_EXPRESS_L1SUBSTATES_CONTROL;

typedef enum {
  PchPcieCompletionTO_Default,
  PchPcieCompletionTO_50_100us,
  PchPcieCompletionTO_1_10ms,
  PchPcieCompletionTO_16_55ms,
  PchPcieCompletionTO_65_210ms,
  PchPcieCompletionTO_260_900ms,
  PchPcieCompletionTO_1_3P5s,
  PchPcieCompletionTO_4_13s,
  PchPcieCompletionTO_17_64s,
  PchPcieCompletionTO_Disabled
} PCH_PCIE_COMPLETION_TIMEOUT;

typedef enum {
  PchPcieOverrideDisabled             = 0x00,
  PchPcieL1L2Override                 = 0x01,
  PchPcieL1SubstatesOverride          = 0x02,
  PchPcieL1L2AndL1SubstatesOverride   = 0x03
} PCH_PCI_EXPRESS_ASPM_OVERRIDE_CONFIG;

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
  PCH_PCIE_COMPLETION_TIMEOUT   CompletionTimeout;  ///< The completion timeout configuration of the root port
  PCH_PCI_EXPRESS_ASPM_CONTROL  Aspm;               ///< The ASPM configuration of the root port
  PCH_PCIE_EXPRESS_L1SUBSTATES_CONTROL L1Substates; ///< The L1 Substates configuration of the root port
  UINT8 TxEqdeEmphSelection;                        ///< When the Link is operating at 5.0 GT/s speed, select the level of de-emphasis for an Upstream component.
} PCH_PCI_EXPRESS_ROOT_PORT_CONFIG;

typedef struct {
  UINT16                                VendorId;             ///< The vendor Id of Pci Express card ASPM setting override, 0xFFFF means any Vendor ID
  UINT16                                DeviceId;             ///< The Device Id of Pci Express card ASPM setting override, 0xFFFF means any Device ID
  UINT8                                 RevId;                ///< The Rev Id of Pci Express card ASPM setting override, 0xFF means all steppings
  UINT8                                 BaseClassCode;        ///< The Base Class Code of Pci Express card ASPM setting override, 0xFF means all base class
  UINT8                                 SubClassCode;         ///< The Sub Class Code of Pci Express card ASPM setting override, 0xFF means all sub class
  PCH_PCI_EXPRESS_ASPM_CONTROL          EndPointAspm;         ///< The override ASPM setting from End point
  PCH_PCI_EXPRESS_ASPM_OVERRIDE_CONFIG  OverrideConfig;       ///< The override configuration. e.g. 0x0 means this subset is applicable to L1L2 override only.
  UINT16                                L1SubstatesCapOffset; ///< The L1Substates Capability Offset
  UINT32                                L1SubstatesCapMask;   ///< The L1Substates Capability Mask
} PCH_PCIE_DEVICE_ASPM_OVERRIDE;

#ifdef PCIESC_SUPPORT
///
/// The PCH_PCI_EXPRESS_CONFIG block describes the expected configuration of the PCH PCI Express controllers
///
typedef struct {
  ///
  /// Temp Bus Number range available to be assigned to each root port and its downstream
  /// devices for initialization of these devices before PCI Bus enumeration.
  ///
  UINT8                             TempRootPortBusNumMin;
  ///
  /// Temp Bus Number range available to be assigned to each root port and its downstream
  /// devices for initialization of these devices before PCI Bus enumeration.
  ///
  UINT8                             TempRootPortBusNumMax;
  ///
  /// These members describe the configuration of each PCH PCIe root port.
  ///
  PCH_PCI_EXPRESS_ROOT_PORT_CONFIG  RootPort[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8                             NumOfDevAspmOverride;     ///< Number of Pci Express card Aspm setting override
  PCH_PCIE_DEVICE_ASPM_OVERRIDE     *DevAspmOverride;         ///< The Pointer which is point to Pci Express card Aspm setting override
  ///
  /// This member describes whether the PCI Express Clock Gating for each root port
  /// is enabled by platform modules.
  ///
  UINT8                             RootPortClockGating : 1;
  UINT8                             Rsvdbits            : 7;  ///< Reserved fields for future expansion w/o protocol change
  //
  // PCI Express S0ix Config
  //
  UINT8                             D0S0IxPolicy;
  UINT8                             ClkReqEnable;
  UINT8                             ClkReq;
  UINT8                             LtrLatencyScale;
  UINT8                             LtrLatencyValue;
} PCH_PCI_EXPRESS_CONFIG;
#endif

//
// --------------------------- Power Optimizer Config ------------------------------
//
typedef struct {
  UINT8         LtrEnable                     :1;   ///< Latency Tolerance Reporting Mechanism.
  UINT8         ObffEnable                    :1;   ///< Pcie end point Optimized Buffer Flush/Fill (OBFF) capability for the root port.
} PCH_PCIE_PWR_OPT;

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
} PCH_PCIE_DEVICE_LTR_OVERRIDE;

#ifdef SATA_SUPPORT

///
/// ---------------------------- SATA Config -----------------------------
///
typedef enum {
  PchSataSpeedSupportGen1 = 1,
  PchSataSpeedSupportGen2,
  PchSataSpeedSupportGen3
} PCH_SATA_SPEED_SUPPORT;

typedef struct {
  UINT8 Enable          : 1;    /// 0: Disable; 1: Enable
  UINT8 HotPlug         : 1;    /// 0: Disable; 1: Enable
  UINT8 MechSw          : 1;    /// 0: Disable; 1: Enable
  UINT8 External        : 1;    /// 0: Disable; 1: Enable
  UINT8 DevSlp          : 1;    /// 0: Disable; 1: Enable DevSlp on the port
  UINT8 Rsvdbits        : 3;    /// Reserved fields for future expansion w/o protocol change
} PCH_SATA_PORT_SETTINGS;

typedef struct {
  PCH_SATA_PORT_SETTINGS  PortSettings[PCH_AHCI_MAX_PORTS];
  UINT8 RaidAlternateId : 1;    /// 0: Disable; 1: Enable
  UINT8 Raid0           : 1;    /// 0: Disable; 1: Enable RAID0
  UINT8 Raid1           : 1;    /// 0: Disable; 1: Enable RAID1
  UINT8 Raid10          : 1;    /// 0: Disable; 1: Enable RAID10
  UINT8 Raid5           : 1;    /// 0: Disable; 1: Enable RAID5
  UINT8 Irrt            : 1;    /// 0: Disable; 1: Enable Intel Rapid Recovery Technology
  UINT8 OromUiBanner    : 1;    /// 0: Disable; 1: Enable OROM UI and BANNER
  UINT8 HddUnlock       : 1;    /// 0: Disable; 1: Indicates that the HDD password unlock in the OS is enabled

  UINT8 LedLocate       : 1;    /// 0: Disable; 1: Indicates that the LED/SGPIO hardware is attached and ping to locate feature is enabled on the OS
  UINT8 IrrtOnly        : 1;    /// 0: Disable; 1: Allow only IRRT drives to span internal and external ports
  UINT8 TestMode        : 1;    /// 0: Disable; 1: Allow entrance to the PCH SATA test modes
  UINT8 SpeedSupport    : 4;    /// Indicates the maximum speed the SATA controller can support
                                /// 1h: 1.5 Gb/s (Gen 1); 2h: 3 Gb/s (Gen 2); 3h: 6 Gb/s (Gen 3)

  UINT8 Rsvdbits        : 1;    // Reserved fields for future expansion w/o protocol change
} PCH_SATA_CONFIG;
#endif

///
/// --------------------------- AZALIA Config ------------------------------
///
typedef struct {
  UINT32  VendorDeviceId;
  UINT16  SubSystemId;
  UINT8   RevisionId;                       /// 0xFF applies to all steppings
  UINT8   FrontPanelSupport;
  UINT16  NumberOfRearJacks;
  UINT16  NumberOfFrontJacks;
} PCH_AZALIA_VERB_TABLE_HEADER;

typedef struct {
  PCH_AZALIA_VERB_TABLE_HEADER  VerbTableHeader;
  UINT32                        *VerbTableData;
} PCH_AZALIA_VERB_TABLE;

typedef struct {
  UINT8                 Pme       : 1;      /// 0: Disable; 1: Enable
  UINT8                 DS        : 1;      /// 0: Docking is not supported; 1:Docking is supported
  UINT8                 DA        : 1;      /// 0: Docking is not attached; 1:Docking is attached
  UINT8                 HdmiCodec : 1;      /// 0: Disable; 1: Enable
  UINT8                 AzaliaVCi : 1;      /// 0: Disable; 1: Enable
  UINT8                 Rsvdbits  : 3;
  UINT8                 AzaliaVerbTableNum; /// Number of verb tables provided by platform
  PCH_AZALIA_VERB_TABLE *AzaliaVerbTable;   /// Pointer to the actual verb table(s)
  UINT16                ResetWaitTimer;     /// The delay timer after Azalia reset, the value is number of microseconds
} PCH_AZALIA_CONFIG;

///
/// --------------------------- Smbus Config ------------------------------
///
typedef struct {
  UINT8 NumRsvdSmbusAddresses;
  UINT8 *RsvdSmbusAddressTable;
} PCH_SMBUS_CONFIG;

///
/// --------------------------- Miscellaneous PM Config ------------------------------
///
typedef struct {
  UINT8  PmeB0S5Dis         : 1;
  UINT8  Rsvdbits           : 7;
} PCH_WAKE_CONFIG;

typedef enum {
  PchSlpS360us,
  PchSlpS31ms,
  PchSlpS350ms,
  PchSlpS32s
} PCH_SLP_S3_MIN_ASSERT;

typedef enum {
  PchSlpS4PchTime,   /// The time defined in EDS Power Sequencing and Reset Signal Timings table
  PchSlpS41s,
  PchSlpS42s,
  PchSlpS43s,
  PchSlpS44s
} PCH_SLP_S4_MIN_ASSERT;

typedef struct {
  ///
  /// Specify Wake Policy
  ///
  PCH_WAKE_CONFIG         WakeConfig;
  ///
  /// SLP_XX Minimum Assertion Width Policy
  ///
  PCH_SLP_S3_MIN_ASSERT   PchSlpS3MinAssert;
  PCH_SLP_S4_MIN_ASSERT   PchSlpS4MinAssert;
  UINT8                   SlpStrchSusUp : 1;  /// Enable/Disable SLP_X Stretching After SUS Well Power Up
  UINT8                   Rsvdbits      : 7;
  UINT8                   ConfigureCfioOnSx;
  UINT8                   *CfioTable;
  UINT8                   CfioEntries;
} PCH_MISC_PM_CONFIG;

///
/// --------------------------- Subsystem Vendor ID / Subsystem ID Config -----
///
typedef struct {
  UINT16  SubSystemVendorId;
  UINT16  SubSystemId;
} PCH_DEFAULT_SVID_SID;

///
/// --------------------------- Lock Down Config ------------------------------
///
typedef struct {
  UINT8  GlobalSmi      : 1;
  UINT8  BiosInterface  : 1;
  UINT8  RtcLock        : 1;
  UINT8  BiosLock       : 1;
  UINT8  Rsvdbits       : 4;
  UINT8  PchBiosLockSwSmiNumber;
} PCH_LOCK_DOWN_CONFIG;

//
// --------------------------- Serial IRQ Config ------------------------------
//
typedef enum {
  PchQuietMode,
  PchContinuousMode
} PCH_SIRQ_MODE;
///
/// Refer to SoC EDS for the details of Start Frame Pulse Width in Continuous and Quiet mode
///

typedef struct {
  BOOLEAN                 SirqEnable;       /// Determines if enable Serial IRQ
  PCH_SIRQ_MODE           SirqMode;         /// Serial IRQ Mode Select
} PCH_LPC_SIRQ_CONFIG;

#ifdef PCIESC_SUPPORT
///
/// --------------------------- Power Optimizer Config ------------------------------
///
typedef struct {
  UINT8  NumOfDevLtrOverride;                            ///< Number of Pci Express card listed in LTR override table
  PCH_PCIE_DEVICE_LTR_OVERRIDE *DevLtrOverride;          ///< Pointer to Pci Express devices LTR override table
  PCH_PCIE_PWR_OPT             PchPwrOptPcie[PCH_PCIE_MAX_ROOT_PORTS]; ///< related configuration for PCIE ports power optimization.
} PCH_PWR_OPT_CONFIG;
#endif

///
/// --------------------- Low Power Input Output Config ------------------------
///
typedef struct {
  PCH_DEV_MODE            Dma0Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Dma1Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C0Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C1Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C2Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C3Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C4Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C5Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            I2C6Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Pwm0Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Pwm1Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Hsuart0Enabled;  /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Hsuart1Enabled;  /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Spi1Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Spi2Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            Spi3Enabled;     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
} PCH_LPSS_CONFIG;

///
/// ----------------------------- SCC Config --------------------------------
///
typedef struct {
  PCH_DEV_MODE            eMMCEnabled;                     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            SdioEnabled;                     /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_DEV_MODE            SdcardEnabled;                   /// 0: Disabled; 1: PCI Mode 2: ACPI Mode
  PCH_SCC_EMMC_MODE       EmmcDriverMode;                  /// Determine the operating mode of eMMC driver
  UINT32                  SccEmmcDllTuningRequired   :  1; /// Determine if DLL Training is required, set to FALSE if Tuning Data is valid
  UINT32                  SccEmmcRxDllTuningEnabled  :  1; /// Determine if Rx DLL Tuning should be enabled
  UINT32                  SccEmmcTxDllTuningEnabled  :  1; /// Determine if Tx DLL Tuning should be enabled
  UINT32                  SccEmmcRxDllDataValid      :  1; /// Set if Rx DLL Tuning Data Valid
  UINT32                  SccEmmcTxDllDataValid      :  1; /// Set if Tx DLL Tuning Data Valid
  UINT32                  SccEmmcRxStrobeDllValue    :  7; /// Rx Strobe Delay Line Value
  UINT32                  SccEmmcTxDataDllValue      :  7; /// Tx Data Delay Line Value
  UINT32                  RsvdBits                   : 13;
} PCH_SCC_CONFIG;

///
/// ------------ General PCH Platform Policy protocol definition ------------
///
struct _DXE_PCH_PLATFORM_POLICY_PROTOCOL {
  ///
  /// This member specifies the revision of the PCH Policy protocol.
  /// This field is used to indicate backwards compatible changes to the protocol.
  /// Platform code that produces this protocol must fill with the correct revision
  /// value for the PCH reference code to correctly interpret the content of the
  /// protocol fields.
  ///
  UINT8                   Revision;
  ///
  /// This member describes the desired bus number of the PCH controller.
  ///
  UINT8                   BusNumber;
  ///
  /// This member describes which PCH devices should be enabled or disabled.
  ///
  PCH_DEVICE_ENABLING     *DeviceEnabling;
  ///
  /// This member describes USB controller's related configuration.
  ///
  PCH_USB_CONFIG          *UsbConfig;
  ///
  /// This member describes PCI Express controller's related configuration.
  ///
#ifdef PCIESC_SUPPORT
  PCH_PCI_EXPRESS_CONFIG  *PciExpressConfig;
#endif

  ///
  /// This member describes SATA controller's related configuration.
  ///
#ifdef SATA_SUPPORT
  PCH_SATA_CONFIG         *SataConfig;
#endif
  ///
  /// This member describes the Intel HD Audio (Azalia) related configuration.
  ///
  PCH_AZALIA_CONFIG       *AzaliaConfig;
  ///
  /// This member describes SMBus related configuration.
  ///
  PCH_SMBUS_CONFIG        *SmbusConfig;
  ///
  /// This member describes miscellaneous platform power management configurations.
  ///
  PCH_MISC_PM_CONFIG      *MiscPmConfig;
  ///
  /// This member describes default SVID and Sid for PCH devices.
  ///
  PCH_DEFAULT_SVID_SID    *DefaultSvidSid;
  ///
  /// This member describes LockDown related configuration.
  ///
  PCH_LOCK_DOWN_CONFIG    *LockDownConfig;
  ///
  /// This member describes the expected configuration of the PCH for Serial IRQ.
  ///
  PCH_LPC_SIRQ_CONFIG     *SerialIrqConfig;
#ifdef PCIESC_SUPPORT
  ///
  /// This member describes the Power Optimizer configuration.
  ///
  PCH_PWR_OPT_CONFIG      *PwrOptConfig;
  ///
  /// This member describes the SwSmi value for override PCIe ASPM table.
  ///
  UINT8                   PchPcieAspmSwSmiNumber;
#endif
  ///
  /// This member describes the Low Power Sub-System related configuration
  ///
  PCH_LPSS_CONFIG         *LpssConfig;
  ///
  /// This member describes the Storage Control Cluster related configuration
  ///
  PCH_SCC_CONFIG          *SccConfig;
  ///
  /// This member describes the PnP Settings related configuration
  ///
  PCH_PNP_MODE            PnpSettings;
  ///
  /// This member describes the S0iX related configuration
  ///
  UINT8                   S0ixSupport;
  ///
  /// This member describes the Hardware Reduced Mode related configuration
  ///
  UINT8                   AcpiHwRed;
};

#endif
