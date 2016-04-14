/** @file
  Definition of the global NVS area protocol.  This protocol
  publishes the address and format of a global ACPI NVS buffer used as a communications
  buffer between SMM code and ASL code.
  The format is derived from the ACPI reference code, version 0.95.

  Note:  Data structures defined in this protocol are not naturally aligned.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _GLOBAL_NVS_AREA_H_
#define _GLOBAL_NVS_AREA_H_

//
// Includes
//
#define GLOBAL_NVS_DEVICE_ENABLE 1
#define GLOBAL_NVS_DEVICE_DISABLE 0

//
// Forward reference for pure ANSI compatibility
//

//EFI_FORWARD_DECLARATION (EFI_GLOBAL_NVS_AREA_PROTOCOL);

//
// Global NVS Area Protocol GUID
//
#define EFI_GLOBAL_NVS_AREA_PROTOCOL_GUID \
{ 0x74e1e48, 0x8132, 0x47a1, 0x8c, 0x2c, 0x3f, 0x14, 0xad, 0x9a, 0x66, 0xdc }
//
// Revision id - Added TPM related fields
//
#define GLOBAL_NVS_AREA_RIVISION_1       1
//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiGlobalNvsAreaProtocolGuid;

//
// Global NVS Area definition
//
#pragma pack (1)
typedef struct {
  ///
  /// Miscellaneous Dynamic Values, the definitions below need to be matched
  /// GNVS definitions in Platform.ASL
  ///
  UINT16      OperatingSystem;        // 00   Operating System
  UINT8       SmiFunction;            // 02   SMI function call via IO Trap
  UINT32      Port80DebugValue;       // 03   Port 80 Debug Port Value
  UINT8       Revision;               // 07   Revision of the structure EFI_GLOBAL_NVS_AREA
  UINT8       OSImageId;              // 08   OS identification.
  UINT8       AcpiDevDis;             // 09   Enable Or Disable Acpi Device Nodes
  UINT8       BoardID;                // 10   Board ID
  UINT8       FabID;                  // 11   FAB ID
  UINT8       ECSupport;              // 12   EC Support
  UINT8       FanSupport;             // 13   Fan Support
  UINT8       BatterySupport;         // 14   Battery Support
  UINT8       Reserved51[9];          // 15:23   Reserved for Platform Policies

  ///
  /// Processor Configuration Values
  ///
  UINT8       ApicEnable;             // 24   APIC Enabled by SBIOS (APIC Enabled = 1)
  UINT8       LogicalProcessorCount;  // 25   Number of Logical Processors if MP Enabled != 0
  UINT32      PpmFlags;               // 26   PPM configuration flags, same as CFGD
  UINT8       PowerState;             // 30   Power State (AC Mode = 1)
  UINT8       Reserved52[10];         // 31:40   Reserved for CPU Policies

  ///
  /// System Agent Policies
  ///
  /// Internal Graphics Device Values
  ///
  UINT8       IgdState;               // 41   IGD State (Primary Display = 1)
  UINT8       DisplayToggleList;      // 42   Display Toggle List Selection
  UINT8       CurrentDeviceList;      // 43   Current Attached Device List
  UINT8       PreviousDeviceList;     // 44   Previous Attached Device List
  UINT16      CurrentDisplayState;    // 45   Current Display State
  UINT16      NextDisplayState;       // 47   Next Display State
  UINT8       NumberOfValidDeviceId;  // 49   Number of Valid Device IDs
  UINT32      DeviceId1;              // 50   Device ID 1
  UINT32      DeviceId2;              // 54   Device ID 2
  UINT32      DeviceId3;              // 58   Device ID 3
  UINT32      DeviceId4;              // 62   Device ID 4
  UINT32      DeviceId5;              // 66   Device ID 5
  UINT32      DeviceId6;              // 70   Device ID 6
  UINT32      DeviceId7;              // 74   Device ID 7
  UINT32      DeviceId8;              // 78   Device ID 8
  UINT32      IgdOpRegionAddress;     // 82   IGD OpRegion base Address
  UINT8       IgdBootType;            // 86   IGD Boot Display Device
  UINT8       IgdPanelType;           // 87   IGD Panel Type CMOs option
  UINT8       IgdTvFormat;            // 88   IGD TV Format CMOS option
  UINT8       IgdTvMinor;             // 89   IGD TV Minor Format CMOS option
  UINT8       IgdPanelScaling;        // 90   IGD Panel Scaling
  UINT8       IgdBlcConfig;           // 91   IGD BLC Configuration
  UINT8       IgdBiaConfig;           // 92   IGD BIA Configuration
  UINT8       IgdSscConfig;           // 93   IGD SSC Configuration
  UINT8       IgdPowerConservation;   // 94   IGD Power Conservation Feature Flag
  UINT8       IgdDvmtMemSize;         // 95   IGD DVMT Memory Size
  UINT8       IgdFunc1Enable;         // 96   IGD Function 1 Enable
  UINT8       IgdSciSmiMode;          // 97   GMCH SMI/SCI mode (0=SCI)
  UINT8       IgdPAVP;                // 98   IGD PAVP data
  UINT8       PanelVendor;            // 99   MIPI Display Panel Vendor

  ///
  /// Backlight Control Values
  ///
  UINT8       BacklightControlSupport;  // 100   Backlight Control Support
  UINT8       BrightnessPercentage;     // 101   Brightness Level Percentage

  ///
  /// Ambient Light Sensor Values
  ///
  UINT8       AlsEnable;              // 102   Ambient Light Sensor Enable
  UINT8       AlsAdjustmentFactor;    // 103   Ambient Light Adjustment Factor
  UINT8       LuxLowValue;            // 104   LUX Low Value
  UINT8       LuxHighValue;           // 105   LUX High Value

  ///
  /// Camera
  ///
  UINT32      ISPAddr;                // 106  ISP Base address
  UINT8       ISPDevSel;              // 110  ISP device enabled selection 0: Disabled; 1: PCI Device 2; 2: PCI Device 3
  UINT8       RvpCameraDevSel;        // 111  Camera selection 0 - Cynthiana_A, 1 - Cynthiana_2B, 2 - Cynthiana_2B_CR
  UINT8       EbCameraDevSel;         // 112  COMBO AIC  0 - Disable, 1 - Enable

  UINT32      NvIgOpRegionAddress;          // 113 NVIG support
  UINT32      NvHmOpRegionAddress;          // 117 NVHM support
  UINT32      ApXmOpRegionAddress;          // 121 AMDA support

  ///
  /// Hybrid Graphics Support
  ///
  UINT8       HgMode;                            // 125 HG Mode (0=Disabled, 1=HG Muxed, 2=HG Muxless, 3=DGPU Only)
  UINT32      GpioBaseAddress;                   // 126 GPIO Base Address
  UINT32      XPcieCfgBaseAddress;               // 130 Any Device's PCIe Config Space Base Address
  UINT8       HgGpioSupport;                     // 134 HG GPIO Support
  UINT16      HgDelayAfterPwrEn;                 // 135 Delay after Power Enable
  UINT16      HgDelayAfterHoldReset;             // 137 Delay after Hold Reset
  UINT32      HgHoldRstCommOffset;               // 139 dGPU HLD RST GPIO Community Offset
  UINT32      HgHoldRstPinOffset;                // 143 dGPU HLD RST GPIO Pin Offset
  UINT8       HgHoldRstActiveInfo;               // 147 dGPU HLD RST GPIO Active Information
  UINT32      HgPwrEnableCommOffset;             // 148 dGPU PWR Enable GPIO Community Offset
  UINT32      HgPwrEnablePinOffset;              // 152 dGPU PWR Enable GPIO Pin Offset
  UINT8       HgPwrEnableActiveInfo;             // 156 dGPU PWR Enable GPIO Active Information
  UINT32      CapStrPresence;                    // 157 PEG Endpoint Capability Structure Presence
  UINT8       EndpointPcieCapOffset;             // 161 PEG Endpoint PCIe Capability Structure Offset
  UINT16      EndpointVcCapOffset;               // 162 PEG Endpoint Virtual Channel Capability Structure Offset
  UINT32      RootPortBaseAddress;               // 164 dGPU Root Port Base Address
  UINT32      DeviceIdX;                         // 168 Device ID for eDP device
  UINT8       EdpValid;                          // 172 Check for eDP display device 

  UINT8       Reserved53[9];                     // 173: 181 Reserved for North Cluster policies

  ///
  /// Security Policies
  ///
  ///
  /// TPM Registers
  ///
  UINT8       Reserved_182;           // 182
  UINT8       Reserved_183;           // 183
  UINT32      Reserved_184;           // 184
  UINT32      Reserved_188;           // 188
  UINT8       Reserved_192;           // 192
  UINT8       Reserved_193;           // 193
  UINT32      Reserved_194;           // 194
  UINT8       Reserved_198;           // 198
  UINT8       Reserved_199;           // 199
  UINT8       I2C0RuntimeD3Enabled;   // 200  I2C 1 Runtime D3 Support
  UINT8       I2C1RuntimeD3Enabled;   // 201  I2C 2 Runtime D3 Support
  UINT8       I2C2RuntimeD3Enabled;   // 202  I2C 3 Runtime D3 Support
  UINT8       I2C3RuntimeD3Enabled;   // 203  I2C 4 Runtime D3 Support
  UINT8       I2C4RuntimeD3Enabled;   // 204  I2C 5 Runtime D3 Support
  UINT8       I2C5RuntimeD3Enabled;   // 205  I2C 6 Runtime D3 Support
  UINT8       I2C6RuntimeD3Enabled;   // 206  I2C 7 Runtime D3 Support
  UINT8       Reserved54[3];          // 207: 209  Reserved for Security policies

  ///
  /// PCH policies
  ///
  UINT8       PchLpeEnabled;           // 210  LPE Audio Enabled
  UINT8       Reserved11[9];           // 211: 219
  ///
  /// USB Sideband Deferring Support
  ///
  UINT32      UsbOtgAddr;              // 220   USB OTG BAR0
  UINT32      UsbOtgAddr1;             // 224   USB OTG BAR1
  UINT8       OtgMode;                 // 228   OTG Mode 0- OTG disable 1- OTG PCI mode
  UINT32      LPEBar0;                 // 229   LPE Bar0
  UINT32      LPEBar1;                 // 233   LPE Bar1
  UINT32      LPEBar2;                 // 237   LPE Bar2
  UINT8       PlatformFlavor;          // 241   Platform Flavor 0:unknown 1: Mobile; 2: desktop
  UINT8       XhciMode;                // 242   xHCI controller mode
  UINT8       PmicEnable;              // 243   PMIC enable/disable
  UINT8       UartSelection;           // 244   UART Interface Selection 0: Internal; 1: SIO
  UINT8       BTHStatus;               // 245   BTH Device Select
  UINT8       emmcVersion;             // 246   eMMC controller version 0 - 4.41 1 - 4.5
  UINT8       GpioAcpiEnabled;         // 247   GPIO ACPI Devices Enable
  UINT32      LDMA1Addr;               // 248   DMA1 BAR0
  UINT32      LDMA1Len;                // 252   DMA1 BAR0 Length
  UINT32      LDMA11Addr;              // 256   DMA1 BAR1
  UINT32      LDMA11Len;               // 260   DMA1 BAR1 Length
  UINT32      PWM1Addr;                // 264   PWM1 BAR0
  UINT32      PWM1Len;                 // 268   PWM1 BAR0 Length
  UINT32      PWM11Addr;               // 272   PWM1 BAR1
  UINT32      PWM11Len;                // 276   PWM1 BAR1 Length
  UINT32      PWM2Addr;                // 280   PWM2 BAR0
  UINT32      PWM2Len;                 // 284   PWM2 BAR0 Length
  UINT32      PWM21Addr;               // 288   PWM2 BAR1
  UINT32      PWM21Len;                // 292   PWM2 BAR1 Length
  UINT32      UART1Addr;               // 296   HSUART1 BAR0
  UINT32      UART1Len;                // 300   HSUART1 BAR0 Length
  UINT32      UART11Addr;              // 304   HSUART1 BAR1
  UINT32      UART11Len;               // 308   HSUART1 BAR1 Length
  UINT32      UART2Addr;               // 312   HSUART2 BAR0
  UINT32      UART2Len;                // 316   HSUART2 BAR0 Length
  UINT32      UART21Addr;              // 320   HSUART2 BAR1
  UINT32      UART21Len;               // 324   HSUART2 BAR1 Length
  UINT32      SPIAddr;                 // 328   SPI BAR0
  UINT32      SPILen;                  // 332   SPI BAR0 Length
  UINT32      SPI1Addr;                // 336   SPI BAR1
  UINT32      SPI1Len;                 // 340   SPI BAR1 Length
  UINT32      SPI2Addr;                // 344   SPI2 BAR0
  UINT32      SPI2Len;                 // 348   SPI2 BAR0 Length
  UINT32      SPI21Addr;               // 352   SPI2 BAR1
  UINT32      SPI21Len;                // 356   SPI2 BAR1 Length
  UINT32      SPI3Addr;                // 360   SPI3 BAR0
  UINT32      SPI3Len;                 // 364   SPI3 BAR0 Length
  UINT32      SPI31Addr;               // 368   SPI3 BAR1
  UINT32      SPI31Len;                // 372   SPI3 BAR1 Length
  UINT32      LDMA2Addr;               // 376   DMA2 BAR0
  UINT32      LDMA2Len;                // 380   DMA2 BAR0 Length
  UINT32      LDMA21Addr;              // 384   DMA2 BAR1
  UINT32      LDMA21Len;               // 388   DMA2 BAR1 Length
  UINT32      I2C1Addr;                // 392   I2C1 BAR0
  UINT32      I2C1Len;                 // 396   I2C1 BAR0 Length
  UINT32      I2C11Addr;               // 400   I2C1 BAR1
  UINT32      I2C11Len;                // 404   I2C1 BAR1 Length
  UINT32      I2C2Addr;                // 408   I2C2 BAR0
  UINT32      I2C2Len;                 // 412   I2C2 BAR0 Length
  UINT32      I2C21Addr;               // 416   I2C2 BAR1
  UINT32      I2C21Len;                // 420   I2C2 BAR1 Length
  UINT32      I2C3Addr;                // 424   I2C3 BAR0
  UINT32      I2C3Len;                 // 428   I2C3 BAR0 Length
  UINT32      I2C31Addr;               // 432   I2C3 BAR1
  UINT32      I2C31Len;                // 436   I2C3 BAR1 Length
  UINT32      I2C4Addr;                // 440   I2C4 BAR0
  UINT32      I2C4Len;                 // 444   I2C4 BAR0 Length
  UINT32      I2C41Addr;               // 448   I2C4 BAR1
  UINT32      I2C41Len;                // 452   I2C4 BAR1 Length
  UINT32      I2C5Addr;                // 456   I2C5 BAR0
  UINT32      I2C5Len;                 // 460   I2C5 BAR0 Length
  UINT32      I2C51Addr;               // 464   I2C5 BAR1
  UINT32      I2C51Len;                // 468   I2C5 BAR1 Length
  UINT32      I2C6Addr;                // 472   I2C6 BAR0
  UINT32      I2C6Len;                 // 476   I2C6 BAR0 Length
  UINT32      I2C61Addr;               // 480   I2C6 BAR1
  UINT32      I2C61Len;                // 484   I2C6 BAR1 Length
  UINT32      I2C7Addr;                // 488   I2C7 BAR0
  UINT32      I2C7Len;                 // 492   I2C7 BAR0 Length
  UINT32      I2C71Addr;               // 496   I2C7 BAR1
  UINT32      I2C71Len;                // 500   I2C7 BAR1 Length
  UINT32      eMMCAddr;                // 504   eMMC BAR0
  UINT32      eMMCLen;                 // 508   eMMC BAR0 Length
  UINT32      eMMC1Addr;               // 512   eMMC BAR1
  UINT32      eMMC1Len;                // 516   eMMC BAR1 Length
  UINT32      SDIOAddr;                // 520   SDIO BAR0
  UINT32      SDIOLen;                 // 524   SDIO BAR0 Length
  UINT32      SDIO1Addr;               // 528   SDIO BAR1
  UINT32      SDIO1Len;                // 532   SDIO BAR1 Length
  UINT32      SDCardAddr;              // 536   SDCard BAR0
  UINT32      SDCardLen;               // 540   SDCard BAR0 Length
  UINT32      SDCard1Addr;             // 544   SDCard BAR1
  UINT32      SDCard1Len;              // 548   SDSCard BAR1 Length
  UINT32      IshAddr;                 // 552   ISH BAR0
  UINT32      IshAddr1;                // 556   ISH BAR1
  UINT16      Sdcard1p8vSwitchingDelay;// 560   SD Card 1.8 Volt Switching Delay (ms)
  UINT16      Sdcard3p3vDischargeDelay;// 562   SD Card 3.3 Volt Discharge Delay (ms)
  UINT16      Reserved25[2];           // 564 : 567
  UINT8       I2CTouchAddress;         // 568   I2C touch address, 0x4B:RVP   0x4A:FFRD
  UINT8       S0ix;                    // 569   S0ix status 0 - disabled 1 - enabled
  UINT8       SDIOMode;                // 570   SDIO Mode 3 - Default  2 - DDR50
  UINT8       Ellensburg;              // 571   Support EllensBurg
  UINT8       AudioCodecSuppport;      // 572   Support for Audio Codec
  UINT8       BTModuleSelect;          // 573   BT Module Select: BCRM or STP
  UINT8       PmicType;                // 574   PmicType
  UINT8       PssdFix;                 // 575   Pre-Sillicon Work Arounds for ASL. 1-PSSD, 0-Normal
  UINT8       Reserved55[9];           // 576 : 584   Reserved for PCH policies

  ///
  /// Platform policies
  ///
  UINT8       PcieOSCControl;          // 585   PCIE OSC Control
  UINT8       NativePCIESupport;       // 586   Native PCI Express Support
  UINT8       IoApicInterruptFlag;     // 587   Global IOAPIC/8259 Interrupt Mode Flag
  UINT8       L01Counter;              // 588   Global L01 Counter
  UINT8       AcpiModemSel;            // 589
  UINT8       VirtualButtonSupport;    // 590   BSW Virtual Button Support
  UINT8       PssDevice;               // 591
  UINT8       IsctCfg;                 // 592

  ///
  /// EC
  ///
  UINT8       LidState;                // 593    Lid State (Lid Open = 1)
  UINT8       DosEn;                   // 594    _DOS Display Support Flag.
  UINT8       EcAvl;                   // 595    Embedded Controller Availability Flag.

  ///
  /// Thermal Policy Values
  ///
  UINT8       Reserved1;                           // 596    Active Trip Point 1
  UINT8       Reserved2;                           // 597    Active Trip Point
  UINT8       PassiveThermalTripPoint;             // 598    Passive Trip Point
  UINT8       PassiveTc1Value;                     // 599    Passive Trip Point TC1 Value
  UINT8       PassiveTc2Value;                     // 600    Passive Trip Point TC2 Value
  UINT8       PassiveTspValue;                     // 601    Passive Trip Point TSP Value
  UINT8       CriticalThermalTripPoint;            // 602    Critical Trip Point
  UINT8       EnableDigitalThermalSensor;          // 603    Digital Thermal Sensor Enable
  UINT8       BspDigitalThermalSensorTemperature;  // 604    Digital Thermal Sensor 1 Reading Temperature of BSP
  UINT8       ApDigitalThermalSensorTemperature;   // 605    Digital Thermal Sensor 2 Reading Temperature of AP
  UINT8       DigitalThermalSensorSmiFunction;     // 606    DTS SMI function call via DTS IO Trap
  UINT8       CoolingType;                         // 607    Global Cooling Type Flag
  UINT8       VirtualFan0Status;                   // 608    Virtual Fan 0 Status
  UINT16      Str2TspValue;                        // 609   10 - Minimum 500 - Maximum

  ///
  /// Battery Support Values
  ///
  UINT8       NumberOfBatteries;                 // 611    Battery Number Present
  UINT8       BatteryCapacity0;                  // 612    Battery 0 Stored Capacity
  UINT8       BatteryCapacity1;                  // 613    Battery 1 Stored Capacity
  UINT8       BatteryStatus0;                    // 614    Battery 0 Stored Status
  UINT8       BatteryStatus1;                    // 615    Battery 1 Stored Status
  UINT8       BatteryChargingSolution;           // 616    Battery charging solution 0-CLV 1-ULPMC
  UINT8       DisableBattery;                    // 617

  ///
  /// SIO Configuration Values
  ///
  UINT8       DockedSioPresent;                  // 618   Dock SIO Present
  UINT8       OnboardCom;                        // 619   Onboard COM Port
  UINT8       OnboardComCir;                     // 620   Onboard COM CIR Port
  UINT8       WPCN381U;                          // 621

  ///
  /// DPTF
  ///
  UINT8       DptfEnable;                        // 622   DPTF Enable/Disable
  UINT8       DptfSysThermal0;                   // 623   System Thermal Sensor 0
  UINT8       DptfSysThermal1;                   // 624   System Thermal Sensor 1
  UINT8       DptfSysThermal2;                   // 625   System Thermal Sensor 2
  UINT8       DptfSysThermal3;                   // 626   System Thermal Sensor 3
  UINT8       DptfSysThermal4;                   // 627   System Thermal Sensor 4
  UINT8       DptfCharger;                       // 628   DPTF Changer Device
  UINT8       DptfDisplayDevice;                 // 629   DPTF Display Device
  UINT8       DptfSocDevice;                     // 630   DPTF SoC Device
  UINT8       DptfProcessor;                     // 631   DPTF Processor Device
  UINT32      DptfProcCriticalTemperature;       // 632   DPTF Processor participant critical temperature
  UINT32      DptfProcPassiveTemperature;        // 636   DPTF Processor participant passive temperature
  UINT32      DptfGenericCriticalTemperature0;   // 640   DPTF Generic sensor0 participant critical temperature
  UINT32      DptfGenericPassiveTemperature0;    // 644   DPTF Generic sensor0 participant passive temperature
  UINT32      DptfGenericCriticalTemperature1;   // 648   DPTF Generic sensor1 participant critical temperature
  UINT32      DptfGenericPassiveTemperature1;    // 652   DPTF Generic sensor1 participant passive temperature
  UINT32      DptfGenericCriticalTemperature2;   // 656   DPTF Generic sensor2 participant critical temperature
  UINT32      DptfGenericPassiveTemperature2;    // 660   DPTF Generic sensor2 participant passive temperature
  UINT32      DptfGenericCriticalTemperature3;   // 664   DPTF Generic sensor3 participant critical temperature
  UINT32      DptfGenericPassiveTemperature3;    // 668   DPTF Generic sensor3 participant passive temperature
  UINT32      DptfGenericCriticalTemperature4;   // 672   DPTF Generic sensor4 participant critical temperature
  UINT32      DptfGenericPassiveTemperature4;    // 676   DPTF Generic sensor4 participant passive temperature
  UINT8       CLpmSetting;                       // 680   DPTF current low power mode setting
  UINT32      DptfCriticalThreshold0;            // 681   DPTF Critical threshold0 for SCU
  UINT32      DptfCriticalThreshold1;            // 685   DPTF Critical threshold1 for SCU
  UINT32      DptfCriticalThreshold2;            // 689   DPTF Critical threshold2 for SCU
  UINT32      DptfCriticalThreshold3;            // 693   DPTF Critical threshold3 for SCU
  UINT32      DptfCriticalThreshold4;            // 697   DPTF Critical threshold4 for SCU
  UINT8       DptfSuperDbg;                      // 701   DPTF Super Debug option. 0 - Disabled, 1 - Enabled
  UINT32      LPOEnable;                         // 702   DPTF LPO Enable
  UINT32      LPOStartPState;                    // 706   P-State start index
  UINT32      LPOStepSize;                       // 710   Step size
  UINT32      LPOPowerControlSetting;            // 714   Power control setting
  UINT32      LPOPerformanceControlSetting;      // 718   Performance control setting
  UINT8       DppmEnabled;                       // 722   DPTF: DPPM enable/disable
  UINT8       DptfWwanDevice;                    // 723   DPTF WWAN
  UINT32      DptfWwanCrt;                       // 724   DPTF WWAN critical temp
  UINT32      DptfWwanPsv;                       // 728   DPTF WWAN Passive temp
  UINT8       DptfMemDevice;                     // 732   DPTF Memory Participant
  UINT8       AmbientConstantSign[6];            // 733:738   DPTF:Sign of Amb. Const
  UINT8       AmbientTripPointChange;            // 739   DPTF: Controls whether _ATI changes other participant's trip point(enabled/disabled)

  UINT8       AmbientConstants[6];               // 740:745  DPTF: #IMP# Takes 6 bytes
  UINT8       DisplayHighLimit;                  // 746
  UINT8       DisplayLowLimit;                   // 747

  UINT8       NfcConnection;                     // 748   NFC Connection 1:I2C7 2:SEC

  UINT32      BmBound;                           // 749   BM Bound  //USED
  UINT8       FsaStatus;                         // 753   FSA Status 0 - Fsa is off, 1- Fsa is on

  UINT8       WIFIModuleSelect;                  // 754 WIFI Module Select: BCRM or STP
  UINT8       EnablePassivePolicy;               // 755 DPTF: Passive Policy enable/disable
  UINT8       EnableCriticalPolicy;              // 756 DPTF: Critical Policy enable/disable
  UINT8       EnableActivePolicy;                // 757 DPTF: Active Policy enable/disable
  UINT32      DptfGenericActiveTemperature0;     // 758 DPTF Generic sensor0 participant Active temperature
  UINT32      DptfGenericCR3Temperature0;        // 762 DPTF Generic sensor0 participant CR3 temperature
  UINT32      DptfGenericHotTemperature0;        // 766 DPTF Generic sensor0 participant HOT temperature
  UINT32      DptfGenericActiveTemperature1;     // 770 DPTF Generic sensor1 participant Active temperature
  UINT32      DptfGenericCR3Temperature1;        // 774 DPTF Generic sensor1 participant CR3 temperature
  UINT32      DptfGenericHotTemperature1;        // 778 DPTF Generic sensor1 participant HOT temperature
  UINT32      DptfGenericActiveTemperature2;     // 782 DPTF Generic sensor2 participant Active temperature
  UINT32      DptfGenericCR3Temperature2;        // 786 DPTF Generic sensor2 participant CR3 temperature
  UINT32      DptfGenericHotTemperature2;        // 790 DPTF Generic sensor2 participant HOT temperature
  UINT32      DptfProcActiveTemperatureSA;       // 794 DPTF Processor participant active temperature
  UINT32      DptfProcCriticalTemperatureSA;     // 798 DPTF Processor participant critical temperature
  UINT32      DptfProcCR3TemperatureSA;          // 802 DPTF Processor participant CR3 temperature
  UINT32      DptfProcHotTemperatureSA;          // 806 DPTF Processor participant Hot temperature
  UINT32      DptfProcPassiveTemperatureSA;      // 810 DPTF Processor participant passive temperature

  UINT8       ToggleSelfClkDisabling;            // 814 Toggle Self Clock Disabling Feature: Enable/Disable in ASL
  UINT8       ISPEn;                             // 815 ISP Device Enable/Disable
  UINT32      PAVPSerpentMemBase;                // 816 PAVP Memory Base
  UINT32      PAVPSerpentMemLength;              // 820 PAVP Memory Length
  UINT64      Drmb;                              // 824 DRM DMA Allocated Buffer Address

} EFI_GLOBAL_NVS_AREA;

#pragma pack ()

//
// Global NVS Area Protocol
//
typedef struct _EFI_GLOBAL_NVS_AREA_PROTOCOL {
  EFI_GLOBAL_NVS_AREA     *Area;
} EFI_GLOBAL_NVS_AREA_PROTOCOL;

#endif
