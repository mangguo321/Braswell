/** @file
  ACPI DSDT table

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

  //
  // Define a Global region of ACPI NVS Region that may be used for any
  // type of implementation.  The starting offset and size will be fixed
  // up by the System BIOS during POST.  Note that the Size must be a word
  // in size to be fixed up correctly.
  //

  OperationRegion(GNVS,SystemMemory,0xFFFF0000,0xAA55)
  Field(GNVS,AnyAcc,Lock,Preserve)
  {
    Offset(0),      // Miscellaneous Dynamic Registers:
    OSYS,    16,    //   (00) Operating System
    SMIF,    8,     //   (02) SMI Function Call (ASL to SMI via I/O Trap)
    P80D,    32,    //   (03) Port 80 Debug Port Value
    REVS,    8,     //   (07) Revision of the structure EFI_GLOBAL_NVS_AREA
    OSID,    8,     //   (08) OS identification.
    ADED,    8,     //   (09) Enable Or Disable Acpi Device Nodes
    BDID,    8,     //   (10) Board ID
    FBID,    8,     //   (11) FAB ID
    ECSP,    8,     //   (12) EC Support
    FNSP,    8,     //   (13) Fan Support
    BTSP,    8,     //   (14) Battery Support
//   Reserved51[9];          // Reserved for Platform Policies

//
// Processor Configuration Values
//
    Offset(24),
    APIC,    8,     //   (24) APIC Enabled by SBIOS (APIC Enabled = 1)
    MPEN,    8,     //   (25) Number of Logical Processors if MP Enabled != 0
    PPMF,    32,    //   (26) PPM configuration Flags (Same as CFGD)
    PWRS,    8,     //   (30) Power State (AC Mode = 1)
//  Reserved52[10];          // Reserved for CPU Policies

//
// System Agent Policies
//
    Offset(41),     // Internal Graphics Registers:
    IGDS,    8,     //   (41) IGD State (Primary Display = 1)
    TLST,    8,     //   (42) Display Toggle List Selection
    CADL,    8,     //   (43) Current Attached Device List
    PADL,    8,     //   (44) Previous Attached Device List
    CSTE,    16,    //   (45) Current Display State
    NSTE,    16,    //   (47) Next Display State
    NDID,    8,     //   (49) Number of Valid Device IDs
    DID1,    32,    //   (50) Device ID 1
    DID2,    32,    //   (54) Device ID 2
    DID3,    32,    //   (58) Device ID 3
    DID4,    32,    //   (62) Device ID 4
    DID5,    32,    //   (66) Device ID 5
    DID6,    32,    //   (70) Device ID 6
    DID7,    32,    //   (74) Device ID 7
    DID8,    32,    //   (78) Device ID 8
    ASLB,    32,    //   (82) IGD OpRegion base address
    IBTT,    8,     //   (86) IGD Boot Display Device
    IPAT,    8,     //   (87) IGD Panel Type CMOs option
    ITVF,    8,     //   (88) IGD TV Format CMOS option
    ITVM,    8,     //   (89) IGD TV Minor Format CMOS option
    IPSC,    8,     //   (90) IGD Panel Scaling
    IBLC,    8,     //   (91) IGD BLC Configuration
    IBIA,    8,     //   (92) IGD BIA Configuration
    ISSC,    8,     //   (93) IGD SSC Configuration
    IDMM,    8,     //   (94) IGD Power Conservation Feature Flag
    IDMS,    8,     //   (95) IGD DVMT Memory Size
    IF1E,    8,     //   (96) IGD Function 1 Enable
    GSMI,    8,     //   (97) GMCH SMI/SCI mode (0=SCI)
    PAVP,    8,     //   (98) IGD PAVP data
    PVDR,    8,     //   (99) MIPI Display Panel Vendor
    BLCS,    8,     //   (100) Backlight Control Registers:
    BRTL,    8,     //   (101) Brightness Level Percentage
//    Offset(102),    //   Ambient Light Sensor Registers:
    ALSE,    8,     //   (102) ALS Enable
    ALAF,    8,     //   (103) Ambient Light Adjusment Factor
    LLOW,    8,     //   (104) LUX Low Value
    LHIH,    8,     //   (105) LUX High Value
    ISPA,    32,    //   (106) ISP Base Address
    ISPD,    8,     //   (110) ISP Device Selection 0: Disabled; 1: PCI Device 2; 2: PCI Device 3
    RCAM,    8,     //   (111) Camera selection 0 - Cynthiana_A, 1 - Cynthiana_2B
    ECAM,    8,     //   (112) COMBO AIC  0 - Disable, 1 - Enable
    NVGA,    32,    //   (113) NVIG opregion address
    NVHA,    32,    //   (117) NVHM opregion address
    AMDA,    32,    //   (121) AMDA opregion address
    HGMD,    8,     //   (125) HG Mode (0=Disabled, 1=HG Muxed, 2=HG Muxless, 3=DGPU Only)
    GBAS,   32,     //   (126) GPIO Base Address
    XBAS,   32,     //   (130) Any Device's PCIe Config Space Base Address
    HGGP,    8,     //   (134) HG GPIO Support
    DLPW,   16,     //   (135) Delay after Power Enable
    DLHR,   16,     //   (137) Delay after Hold Reset
    HRCO,   32,     //   (139) dGPU HLD RST GPIO Community Offset
    HRPO,   32,     //   (143) dGPU HLD RST GPIO Pin Offset
    HRAI,    8,     //   (147) dGPU HLD RST GPIO Active Information
    PECO,   32,     //   (148) dGPU PWR Enable GPIO Community Offset
    PEPO,   32,     //   (152) dGPU PWR Enable GPIO Pin Offset
    PEAI,    8,     //   (156) dGPU PWR Enable GPIO Active Information
    CPSP,   32,     //   (157) PEG Endpoint Capability Structure Presence (Bit 0: Virtual Channel Capability)
    EECP,   8,      //   (161) PEG Endpoint PCIe Capability Structure Offset
    EVCP,   16,     //   (162) PEG Endpoint Virtual Channel Capability Structure Offset
    RPBA,   32,     //   (164) dGPU Root Port Base Address
    DIDX,   32,     //   (168) Device ID for eDP device
    EDPV,   8,      //   (172) Check for eDP display device 

//  Reserved53[9];  // (173 : 181) Reserved for North Cluster policies

//
// Security Policies
//
    Offset(182),    // 
        ,    8,     //   (182) 
        ,    8,     //   (183) 
        ,    32,    //   (184) 
        ,    32,    //   (188) 
        ,    8,     //   (192) 
        ,    8,     //   (193) 
        ,    32,    //   (194) 
        ,    8,     //   (198) 
        ,    8,     //   (199) 
    I0D3,    8,     //   (200) I2C 1 Runtime D3 Support
    I1D3,    8,     //   (201) I2C 2 Runtime D3 Support
    I2D3,    8,     //   (202) I2C 3 Runtime D3 Support
    I3D3,    8,     //   (203) I2C 4 Runtime D3 Support
    I4D3,    8,     //   (204) I2C 5 Runtime D3 Support
    I5D3,    8,     //   (205) I2C 6 Runtime D3 Support
    I6D3,    8,     //   (206) I2C 7 Runtime D3 Support
//  Reserved54[3];  //   (207: 209) Reserved for Security policies

//
// PCH policies
//
    Offset(210),
    LPES,    8,     //   (210) LPE Audio Setup Control 0: Disabled   1:Enabled
//Reserved [9]

    Offset(220),
    OTG0,    32,    //   (220) USB OTG BAR0
    OTG1,    32,    //   (224) USB OTG BAR1
    OTGM,    8,     //   (228) OTG mode
    LPE0,    32,    //   (229) LPE Bar0
    LPE1,    32,    //   (233) LPE Bar1
    LPE2,    32,    //   (237) LPE Bar2
    PFLV,    8,     //   (241) Platform Flavor 0:unknown 1: Mobile; 2: desktop
    XHCI,    8,     //   (242) xHCI controller mode
    PMEN,    8,     //   (243) PMIC enable/disable
    USEL,    8,     //   (244) UART Interface Selection
    BTHS,    8,     //   (245) BlueTooth selection 0 - Other than PR0, 1 - PR0 which swap the GPIO
    EMVR,    8,     //   (246) eMMC controller version
    GOAE,    8,     //   (247) GPIO ACPI Devices Enabled
//  Offset(248),    // LPSS Controllers
    D10A,    32,    //   (248) DMA1 BAR0
    D10L,    32,    //   (252) DMA1 BAR0 Length
    D11A,    32,    //   (256) DMA1 BAR1
    D11L,    32,    //   (260) DMA1 BAR1 Length
    P10A,    32,    //   (264) PWM1 BAR0
    P10L,    32,    //   (268) PWM1 BAR0 Length
    P11A,    32,    //   (272) PWM1 BAR1
    P11L,    32,    //   (276) PWM1 BAR1 Length
    P20A,    32,    //   (280) PWM2 BAR0
    P20L,    32,    //   (284) PWM2 BAR0 Length
    P21A,    32,    //   (288) PWM2 BAR1
    P21L,    32,    //   (292) PWM2 BAR1 Length
    U10A,    32,    //   (296) HSUART BAR0
    U10L,    32,    //   (300) HSUART BAR0 Length
    U11A,    32,    //   (304) HSUART BAR1
    U11L,    32,    //   (308) HSUART BAR1 Length
    U20A,    32,    //   (312) HSUART2 BAR0
    U20L,    32,    //   (316) HSUART2 BAR0 Length
    U21A,    32,    //   (320) HSUART2 BAR1
    U21L,    32,    //   (324) HSUART2 BAR1 Length
    SP0A,    32,    //   (328) SPI BAR0
    SP0L,    32,    //   (332) SPI BAR0 Length
    SP1A,    32,    //   (336) SPI BAR1
    SP1L,    32,    //   (340) SPI BAR1 Length
    S20A,    32,    //   (344) SPI2 BAR0
    S20L,    32,    //   (348) SPI2 BAR0 Length
    S21A,    32,    //   (352) SPI2 BAR1
    S21L,    32,    //   (356) SPI2 BAR1 Length
    S30A,    32,    //   (360) SPI3 BAR0
    S30L,    32,    //   (364) SPI3 BAR0 Length
    S31A,    32,    //   (368) SPI3 BAR1
    S31L,    32,    //   (372) SPI3 BAR1 Length
    D20A,    32,    //   (376) DMA2 BAR0
    D20L,    32,    //   (380) DMA2 BAR0 Length
    D21A,    32,    //   (384) DMA2 BAR1
    D21L,    32,    //   (388) DMA2 BAR1 Length
    I10A,    32,    //   (392) I2C1 BAR0
    I10L,    32,    //   (396) I2C1 BAR0 Length
    I11A,    32,    //   (400) I2C1 BAR1
    I11L,    32,    //   (404) I2C1 BAR1 Length
    I20A,    32,    //   (408) I2C2 BAR0
    I20L,    32,    //   (412) I2C2 BAR0 Length
    I21A,    32,    //   (416) I2C2 BAR1
    I21L,    32,    //   (420) I2C2 BAR1 Length
    I30A,    32,    //   (424) I2C3 BAR0
    I30L,    32,    //   (428) I2C3 BAR0 Length
    I31A,    32,    //   (432) I2C3 BAR1
    I31L,    32,    //   (436) I2C3 BAR1 Length
    I40A,    32,    //   (440) I2C4 BAR0
    I40L,    32,    //   (444) I2C4 BAR0 Length
    I41A,    32,    //   (448) I2C4 BAR1
    I41L,    32,    //   (452) I2C4 BAR1 Length
    I50A,    32,    //   (456) I2C5 BAR0
    I50L,    32,    //   (460) I2C5 BAR0 Length
    I51A,    32,    //   (464)I2C5 BAR1
    I51L,    32,    //   (468) I2C5 BAR1 Length
    I60A,    32,    //   (472) I2C6 BAR0
    I60L,    32,    //   (476) I2C6 BAR0 Length
    I61A,    32,    //   (480) I2C6 BAR1
    I61L,    32,    //   (484) I2C6 BAR1 Length
    I70A,    32,    //   (488) I2C7 BAR0
    I70L,    32,    //   (492) I2C7 BAR0 Length
    I71A,    32,    //   (496) I2C7 BAR1
    I71L,    32,    //   (500) I2C7 BAR1 Length
//  Offset(504),    // SCC Controllers
    eM0A,    32,    //   (504) eMMC BAR0
    eM0L,    32,    //   (508) eMMC BAR0 Length
    eM1A,    32,    //   (512) eMMC BAR1
    eM1L,    32,    //   (516) eMMC BAR1 Length
    SI0A,    32,    //   (520) SDIO BAR0
    SI0L,    32,    //   (524) SDIO BAR0 Length
    SI1A,    32,    //   (528) SDIO BAR1
    SI1L,    32,    //   (532) SDIO BAR1 Length
    SD0A,    32,    //   (536) SDCard BAR0
    SD0L,    32,    //   (540) SDCard BAR0 Length
    SD1A,    32,    //   (544) SDCard BAR1
    SD1L,    32,    //   (548) SDCard BAR1 Length
    ISH0,    32,    //   (552) ISH BAR0
    ISH1,    32,    //   (556) ISH BAR1
    VS18,    16,    //   (560) SD Card 1.8v Switching Delay (ms)
    VD33,    16,    //   (562) SD Card 3.3v Discahrge Delay (ms)
        ,    16,    //   (564)
        ,    16,    //   (566)
    ITSA,    8,     //   (568) I2C Touch Screen Address
    S0IX,    8,     //   (569) S0ix status
    SDMD,    8,     //   (570) SDIO Mode
    ELLN,    8,     //   (571) EllensBurg Support 1 - Enable, 0 - Disable
    AUCD,    8,     //   (572) Audio Codec Support 5645 - EB, 5640/5672 - RVP
    BTMS,    8,     //   (573) BT Module Select: BCRM or STP
    PMCT,    8,     //   (574) Pmic Type
    PSSD,    8,     //   (575) Pre-Sillicon Work Arounds for ASL. 1-PSSD, 0-Normal
//  Reserved55[9];  //   (576: 584) Reserved for PCH policies

//
// Platform policies
//
    Offset(585),
    OSCC,    8,     //   (585) PCIE OSC Control
    NEXP,    8,     //   (586) Native PCIE Setup Value
    GPIC,    8,     //   (587) Global IOAPIC/8259 Interrupt Mode Flag.
    L01C,    8,     //   (588) Global L01 Counter.
    MODS,    8,     //   (589) Modem Selection
    PB1E,    8,     //   (590) Virtual Power Button Support
                          //   Bit0: Enable/Disable Virtual Button (0 - Disable; 1- Enable)
                          //   Bit1: Internal Flag
                          //   Bit2: Rotation Lock flag, 0:unlock, 1:lock
                          //   Bit3: Slate/Laptop Mode Flag, 0: Slate, 1: Laptop / CalmShell
                          //   Bit4: Undock / Dock Flag, 0: Undock, 1: Dock
                          //   Bit5, 6: reserved for future use.
                          //   Bit7: EC 10sec PB Override state for S3/S4 wake up.
    PSDE,    8,     //   (591) PSS Device Selection
    ISCT,    8,     //   (592) Reserved
    LIDS,    8,     //   (593) Lid State (Lid Open = 1)
    DSEN,    8,     //   (594) _DOS Display Support Flag.
    ECON,    8,     //   (595) Embedded Controller Availability Flag.

//    Offset(596),     // Thermal Policy Registers:
    ACT1,    8,     //   (596) Active Trip Point 1
    ACTT,    8,     //   (597) Active Trip Point
    PSVT,    8,     //   (598) Passive Trip Point
    TC1V,    8,     //   (599) Passive Trip Point TC1 Value
    TC2V,    8,     //   (600) Passive Trip Point TC2 Value
    TSPV,    8,     //   (601) Passive Trip Point TSP Value
    CRTT,    8,     //   (602) Critical Trip Point
    DTSE,    8,     //   (603) Digital Thermal Sensor Enable
    DTS1,    8,     //   (604) Digital Thermal Sensor 1 Reading Temperature of BSP
    DTS2,    8,     //   (605) Digital Thermal Sensor 2 Reading Temperature of AP
    DTSF,    8,     //   (606) DTS SMI Function Call  via DTS IO Trap
    CTYP,    8,     //   (607) Global Cooling Type Flag.
    VFN0,    8,     //   (608) Virtual Fan0 Status.
    STTV,   16,     //   (609) 10 - Minimum 500 - Maximum

//    Offset(611),     // Battery Support Registers:
    BNUM,    8,     //   (611) Battery Number Present
    B0SC,    8,     //   (612) Battery 0 Stored Capacity
    B1SC,    8,     //   (613) Battery 1 Stored Capacity
    B0SS,    8,     //   (614) Battery 0 Stored Status
    B1SS,    8,     //   (615) Battery 1 Stored Status
    BCSL,    8,     //   (616) Battery charging solution 0-CLV 1-ULPMC
    DBAT,    8,     //   (617) Disable Battery MSFT PAC Requirement
    NATP,    8,     //   (618) Dock SIO Present
    CMCP,    8,     //   (619) NPCE COM Port
    CIRP,    8,     //   (620) Onboard COM CIR Port
    W381,    8,     //   (621) W8374L

//    Offset(622),    // DPTF
    DPTE,    8,     //   (622) DPTF Enable
    THM0,    8,     //   (623) System Thermal Sensor 0
    THM1,    8,     //   (624) System Thermal Sensor 1
    THM2,    8,     //   (625) System Thermal Sensor 2
    THM3,    8,     //   (626) System Thermal Sensor 3
    THM4,    8,     //   (627) System Thermal Sensor 4
    CHGR,    8,     //   (628) DPTF Changer Device
    DDSP,    8,     //   (629) DPTF Display Device
    DSOC,    8,     //   (630) DPTF SoC device
    DPSR,    8,     //   (631) DPTF Processor device
    DPCT,    32,    //   (632) DPTF Processor participant critical temperature
    DPPT,    32,    //   (636) DPTF Processor participant passive temperature
    DGC0,    32,    //   (640) DPTF Generic sensor0 participant critical temperature
    DGP0,    32,    //   (644) DPTF Generic sensor0 participant passive temperature
    DGC1,    32,    //   (648) DPTF Generic sensor1 participant critical temperature
    DGP1,    32,    //   (652) DPTF Generic sensor1 participant passive temperature
    DGC2,    32,    //   (656) DPTF Generic sensor2 participant critical temperature
    DGP2,    32,    //   (660) DPTF Generic sensor2 participant passive temperature
    DGC3,    32,    //   (664) DPTF Generic sensor3 participant critical temperature
    DGP3,    32,    //   (668) DPTF Generic sensor3 participant passive temperature
    DGC4,    32,    //   (672) DPTF Generic sensor4 participant critical temperature
    DGP4,    32,    //   (676) DPTF Generic sensor4 participant passive temperature
    DLPM,    8,     //   (680) DPTF Current low power mode setting
    DSC0,    32,    //   (681) DPTF Critical threshold0 for SCU
    DSC1,    32,    //   (685) DPTF Critical threshold1 for SCU
    DSC2,    32,    //   (689) DPTF Critical threshold2 for SCU
    DSC3,    32,    //   (693) DPTF Critical threshold3 for SCU
    DSC4,    32,    //   (697) DPTF Critical threshold4 for SCU
    DDBG,    8,     //   (701) DPTF Super Debug option. 0 - Disabled, 1 - Enabled
    LPOE,    32,    //   (702) DPTF LPO Enable
    LPPS,    32,    //   (706) P-State start index
    LPST,    32,    //   (710) Step size
    LPPC,    32,    //   (714) Power control setting
    LPPF,    32,    //   (718) Performance control setting
    DPME,    8,     //   (722) DPTF DPPM enable/disable
    WWEN,    8,     //   (723) WWAN Enable
    WWCR,    32,    //   (724) WWAN CRT
    WWPS,    32,    //   (728) WWAN PSV
    RSVD,    8,     //   (732) DPTF Memory Participant
    ABS0,    8,     //   (733) Sign for ABC0
    ABS1,    8,     //   (734) Sign for ABC1
    ABS2,    8,     //   (735) Sign for ABC2
    ABS3,    8,     //   (736) Sign for ABC3
    ABS4,    8,     //   (737) Sign for ABC4
    ABS5,    8,     //   (738) Sign for ABC5
    AMTE,    8,     //   (739) Ambient Trip point change
    ABC0,    8,     //   (740) Ambient Participant Constants
    ABC1,    8,     //   (741)
    ABC2,    8,     //   (742)
    ABC3,    8,     //   (743)
    ABC4,    8,     //   (744)
    ABC5,    8,     //   (745)
    DPHL,    8,     //   (746)
    DPLL,    8,     //   (747)

    NFCS,    8,     //   (748) NFCx Connect 1: NFC1 to I2C7    2:NFC2 to SEC
    BMBD,    32,    //   (749) BM Bound
    FSAS,    8,     //   (753) FSA Status

    WIFD,    8,     //   (754) WIFI device selection (0:STP 1:BCM)
    DPPP,    8,     //   (755) EnablePassivePolicy
    DPCP,    8,     //   (756) EnableCriticalPolicy
    DPAP,    8,     //   (757) EnableActivePolicy
    DGA0,    32,    //   (758) DPTF Generic sensor0 participant Active temperature
    DGS0,    32,    //   (762) DPTF Generic sensor0 participant CR3 temperature
    DGH0,    32,    //   (766) DPTF Generic sensor0 participant HOT temperature
    DGA1,    32,    //   (770) DPTF Generic sensor1 participant Active temperature
    DGS1,    32,    //   (774) DPTF Generic sensor1 participant CR3 temperature
    DGH1,    32,    //   (778) DPTF Generic sensor1 participant HOT temperature
    DGA2,    32,    //   (782) DPTF Generic sensor2 participant Active temperature
    DGS2,    32,    //   (786) DPTF Generic sensor2 participant CR3 temperature
    DGH2,    32,    //   (790) DPTF Generic sensor2 participant HOT temperature
    SAAT,    32,    //   (794) DPTF Processor participant active temperature
    SACT,    32,    //   (798) DPTF Processor participant critical temperature
    SAST,    32,    //   (802) DPTF Processor participant CR3 temperature
    SAHT,    32,    //   (806) DPTF Processor participant Hot temperature
    SAPT,    32,    //   (810) DPTF Processor participant passive temperature
    SCDD,    8,     //   (814) Toggle Self Clock Disabling feature in ASL, 0-Disable 1-Enable
    ISPE,    8,     //   (815) ISP Device Enable/Disable
    PAVB,    32,    //   (816) PAVP Memory Base
    PAVL,    32,    //   (820) PAVP Memory Length
    DRMB,    64,    //   (824) DRM DMA Allocated Buffer Address
  }

