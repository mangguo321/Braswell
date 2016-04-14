/** @file
  Register names for PCH USB devices.

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_<generation_name>_" in register/bit names. e.g., "_PCH_CHV_"
  - Registers / bits that are different between SKUs are denoted by "_<SKU_name>"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without <generation_name> inserted.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_REGS_USB_H_
#define _PCH_REGS_USB_H_

//
// USB Definitions
//
#define PCH_USB_MAX_PHYSICAL_PORTS          5
#define PCH_HSIC_MAX_PORTS                  2
#define PCH_SSIC_MAX_PORTS                  2
#define PCH_XHCI_MAX_USB3_PORTS             4


//
// USB3 (XHCI) related definitions
//
typedef enum {
  PchXhci1            = 0,
  PchXhciControllerMax
} PCH_USB30_CONTROLLER_TYPE;

#define PCI_DEVICE_NUMBER_PCH_XHCI          20
#define PCI_FUNCTION_NUMBER_PCH_XHCI        0

//
// XHCI PCI Config Space registers
//
#define R_PCH_XHCI_VENDOR_ID                0x00  // Vendor ID
#define B_PCH_XHCI_VENDOR_ID                0xFFFF

#define R_PCH_XHCI_DEVICE_ID                0x02  // Device ID
#define B_PCH_XHCI_DEVICE_ID                0xFFFF
#define V_PCH_XHCI_DEVICE_ID_0              0x22B5

#define R_PCH_XHCI_COMMAND_REGISTER         0x04  // Command
#define B_PCH_XHCI_COMMAND_ID               BIT10 // Interrupt Disable
#define B_PCH_XHCI_COMMAND_FBE              BIT9  // Fast Back to Back Enable
#define B_PCH_XHCI_COMMAND_SERR             BIT8  // SERR# Enable
#define B_PCH_XHCI_COMMAND_WCC              BIT7  // Wait Cycle Control
#define B_PCH_XHCI_COMMAND_PER              BIT6  // Parity Error Response
#define B_PCH_XHCI_COMMAND_VPS              BIT5  // VGA Palette Snoop
#define B_PCH_XHCI_COMMAND_MWI              BIT4  // Memory Write Invalidate
#define B_PCH_XHCI_COMMAND_SCE              BIT3  // Special Cycle Enable
#define B_PCH_XHCI_COMMAND_BME              BIT2  // Bus Master Enable
#define B_PCH_XHCI_COMMAND_MSE              BIT1  // Memory Space Enable

#define R_PCH_XHCI_MEM_BASE                 0x10  // Memory Base Address
#define B_PCH_XHCI_MEM_BASE_BA              0xFFFFFFFFFFFF0000 // Base Address
#define V_PCH_XHCI_MEM_LENGTH               0x10000 // 64 KB of Memory Length
#define N_PCH_XHCI_MEM_ALIGN                16    // Memory Space Alignment
#define B_PCH_XHCI_MEM_BASE_PREF            BIT3  // Prefetchable
#define B_PCH_XHCI_MEM_BASE_TYPE            (BIT2 | BIT1) // Type
#define B_PCH_XHCI_MEM_BASE_RTE             BIT0  // Resource Type Indicator

#define R_PCH_XHCI_SVID                     0x2C
#define B_PCH_XHCI_SVID                     0xFFFF

#define R_PCH_XHCI_SID                      0x2E
#define B_PCH_XHCI_SID                      0xFFFF

#define R_PCH_XHCI_XHCC1                    0x40  // XHC System Bus Configuration 1
#define B_PCH_XHCI_XHCC1_ACCTRL             BIT31 // Access Control
#define B_PCH_XHCI_XHCC1_RMTASERR           BIT24
#define B_PCH_XHCI_XHCC1_URD                BIT23
#define B_PCH_XHCI_XHCC1_URRE               BIT22
#define B_PCH_XHCI_XHCC1_IIL1E              (BIT21 | BIT20 | BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_DIS          0
#define V_PCH_XHCI_XHCC1_IIL1E_32           (BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_64           (BIT20)
#define V_PCH_XHCI_XHCC1_IIL1E_128          (BIT20 | BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_256          (BIT21)
#define V_PCH_XHCI_XHCC1_IIL1E_512          (BIT21 | BIT19)
#define V_PCH_XHCI_XHCC1_IIL1E_1024         (BIT21 | BIT20)
#define V_PCH_XHCI_XHCC1_IIL1E_131072       (BIT21 | BIT20 | BIT19)
#define B_PCH_XHCI_XHCC1_XHCIL1E            BIT18 // XHC Initiated L1 Enable
#define B_PCH_XHCI_XHCC1_D3IL1E             BIT17 // D3 Initiated L1 Enable
#define B_PCH_XHCI_XHCC1_UNPPA              (BIT16 | BIT15 | BIT14 | BIT13 | BIT12) // Periodic Complete Pre Wake Time
#define B_PCH_XHCI_XHCC1_SWAXHCI            BIT11 // SW Assisted xHC Idle
#define B_PCH_XHCI_XHCC1_L23HRAWC           (BIT10 | BIT9 | BIT8) // L23 to Host Reset Acknowledge Wait Count
#define V_PCH_XHCI_XHCC1_L23HRAWC_DIS       0
#define V_PCH_XHCI_XHCC1_L23HRAWC_128       (BIT8)
#define V_PCH_XHCI_XHCC1_L23HRAWC_256       (BIT9)
#define V_PCH_XHCI_XHCC1_L23HRAWC_512       (BIT9 | BIT8)
#define V_PCH_XHCI_XHCC1_L23HRAWC_1024      (BIT10)
#define V_PCH_XHCI_XHCC1_L23HRAWC_2048      (BIT10 | BIT8)
#define V_PCH_XHCI_XHCC1_L23HRAWC_4096      (BIT10 | BIT9)
#define V_PCH_XHCI_XHCC1_L23HRAWC_131072    (BIT10 | BIT9 | BIT8)
#define B_PCH_XHCI_XHCC1_UTAGCP             (BIT7 | BIT6) // Upstream Type Arbiter Grant Count Posted
#define B_PCH_XHCI_XHCC1_UDAGCNP            (BIT5 | BIT4) // Upstream Type Arbiter Grant Count Non Posted
#define B_PCH_XHCI_XHCC1_UDAGCCP            (BIT3 | BIT2) // Upstream Type Arbiter Grant Count Completion
#define B_PCH_XHCI_XHCC1_UDAGC              (BIT1 | BIT0) // Upstream Type Arbiter Grant Count

#define R_PCH_XHCI_XHCC2                    0x44  // XHC System Bus Configuration 2
#define B_PCH_XHCI_XHCC2_OCCFDONE           BIT31 // OC Configuration Done
#define B_PCH_XHCI_XHCC2_DREQBCC            BIT25 // DMA Request Boundary Crossing Control
#define B_PCH_XHCI_XHCC2_IDMARRSC           (BIT24 | BIT23 | BIT22) // IDMA Read Request Size Control
#define B_PCH_XHCI_XHCC2_XHCUPRDROE         BIT21 // XHC Upstream Read Relaxed Ordering Enable
#define B_PCH_XHCI_XHCC2_IOSFSRAD           BIT20 // IOSF Sideband Register Access Disable
#define B_PCH_XHCI_XHCC2_UNPPA              0xFC000 // Upstream Non-Posted Pre-Allocation
#define B_PCH_XHCI_XHCC2_SWAXHCIP           (BIT13 | BIT12) // SW Assisted xHC Idle Policy
#define B_PCH_XHCI_XHCC2_RAWDD              BIT11 // MMIO Read After MMIO Write Delay Disable
#define B_PCH_XHCI_XHCC2_WAWDE              BIT10 // MMIO Write After MMIO Write Delay Enable
#define B_PCH_XHCI_XHCC2_SWACXIHB           (BIT9 | BIT8) // SW Assisted Cx Inhibit
#define B_PCH_XHCI_XHCC2_SWADMIL1IHB        (BIT7 | BIT6) // SW Assisted DMI L1 Inhibit
#define B_PCH_XHCI_XHCC2_L1FP2CGWC          (BIT5 | BIT4 | BIT3) // L1 Force P2 clock Gating Wait Count
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_DIS      0
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_128      (BIT3)
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_256      (BIT4)
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_512      (BIT4 | BIT3)
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_1024     (BIT5)
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_2048     (BIT5 | BIT3)
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_4096     (BIT5 | BIT4)
#define V_PCH_XHCI_XHCC2_L1FP2CGWC_131072   (BIT5 | BIT4 | BIT3)
#define B_PCH_XHCI_XHCC2_RDREQSZCTRL        (BIT2 | BIT1 | BIT0) // Read Request Size Control
#define V_PCH_XHCI_XHCC2_RDREQSZCTRL_128    0
#define V_PCH_XHCI_XHCC2_RDREQSZCTRL_256    (BIT0)
#define V_PCH_XHCI_XHCC2_RDREQSZCTRL_512    (BIT1)
#define V_PCH_XHCI_XHCC2_RDREQSZCTRL_64     (BIT2 | BIT1 | BIT0)

#define R_PCH_XHCI_AUDSYNC                  0x58  // Audio Time Synchronization
#define B_PCH_XHCI_AUDSYNC_CMF1             0x3FFF0000
#define B_PCH_XHCI_AUDSYNC_CMFB             0x1FFF

#define R_PCH_XHCI_USB_RELNUM               0x60  // Serial Bus Release Number
#define B_PCH_XHCI_USB_RELNUM               0xFF

#define R_PCH_XHCI_FL_ADJ                   0x61  // Frame Length Adjustment
#define B_PCH_XHCI_FL_ADJ                   0x3F

#define R_PCH_XHCI_PWR_CAPID                0x70  // PCI Power Management Capability ID
#define B_PCH_XHCI_PWR_CAPID                0xFF

#define R_PCH_XHCI_NXT_PTR1                 0x71  // Next Item Pointer #1
#define B_PCH_XHCI_NXT_PTR1                 0xFF

#define R_PCH_XHCI_PWR_CAP                  0x72  // Power Management Capabilities
#define B_PCH_XHCI_PWR_CAP_PME_SUP          0xF800
#define B_PCH_XHCI_PWR_CAP_D2_SUP           BIT10
#define B_PCH_XHCI_PWR_CAP_D1_SUP           BIT9
#define B_PCH_XHCI_PWR_CAP_AUX_CUR          (BIT8 | BIT7 | BIT6)
#define B_PCH_XHCI_PWR_CAP_DSI              BIT5
#define B_PCH_XHCI_PWR_CAP_PME_CLK          BIT3
#define B_PCH_XHCI_PWR_CAP_VER              (BIT2 | BIT1 | BIT0)

#define R_PCH_XHCI_PWR_CNTL_STS             0x74  // Power Management Control/Status
#define B_PCH_XHCI_PWR_CNTL_STS_PME_STS     BIT15
#define B_PCH_XHCI_PWR_CNTL_STS_DATASCL     (BIT14 | BIT13)
#define B_PCH_XHCI_PWR_CNTL_STS_DATASEL     (BIT12 | BIT11 | BIT10 | BIT9)
#define B_PCH_XHCI_PWR_CNTL_STS_PME_EN      BIT8
#define B_PCH_XHCI_PWR_CNTL_STS_PWR_STS     (BIT1 | BIT0) // Power State
#define V_PCH_XHCI_PWR_CNTL_STS_PWR_STS_D3  (BIT1 | BIT0)

#define R_PCH_XHCI_FUS                      0x94  // Fuse and Strap
#define B_PCH_XHCI_FUS_USH_DEVID            (BIT13 | BIT12 | BIT11) // USH Device ID
#define B_PCH_XHCI_FUS_XDD_EN               BIT10 // Debug Device Enable
#define B_PCH_XHCI_FUS_SRAMPWRGTDIS         BIT9  // SRAM Power Gating Disable
#define B_PCH_XHCI_FUS_USB2PLLSDIS          BIT8  // USB2 PLL Shutdown Disable
#define B_PCH_XHCI_FUS_USBIOPMDIS           BIT7  // USB I/O Power Management Disable
#define B_PCH_XHCI_FUS_XHCDCGDIS            BIT6  // XHC Dynamic Clock Gating Disable
#define B_PCH_XHCI_FUS_USBR                 BIT5  // USBr Disable
#define V_PCH_XHCI_FUS_USBR_EN              0
#define V_PCH_XHCI_FUS_USBR_DIS             BIT5
#define B_PCH_XHCI_FUS_SSPRTCNT             (BIT4 | BIT3) // SS Port Count
#define V_PCH_XHCI_FUS_SSPRTCNT_00B         0
#define V_PCH_XHCI_FUS_SSPRTCNT_01B         (BIT3)
#define V_PCH_XHCI_FUS_SSPRTCNT_10B         (BIT4)
#define V_PCH_XHCI_FUS_SSPRTCNT_11B         (BIT4 | BIT3)
#define B_PCH_XHCI_FUS_HSPRTCNT             (BIT2 | BIT1) // HS Port Count
#define V_PCH_XHCI_FUS_HSPRTCNT_00B         0
#define V_PCH_XHCI_FUS_HSPRTCNT_01B         (BIT1)
#define V_PCH_XHCI_FUS_HSPRTCNT_10B         (BIT2)
#define V_PCH_XHCI_FUS_HSPRTCNT_11B         (BIT2 | BIT1)
#define V_PCH_XHCI_FUS_SSPRTCNT_00B_CNT     6
#define V_PCH_XHCI_FUS_SSPRTCNT_01B_CNT     4
#define V_PCH_XHCI_FUS_SSPRTCNT_10B_CNT     2
#define V_PCH_XHCI_FUS_SSPRTCNT_11B_CNT     0
#define V_PCH_XHCI_FUS_SSPRTCNT_00B_MASK    0x3F
#define V_PCH_XHCI_FUS_SSPRTCNT_01B_MASK    0x0F
#define V_PCH_XHCI_FUS_SSPRTCNT_10B_MASK    0x03
#define V_PCH_XHCI_FUS_SSPRTCNT_11B_MASK    0x00
#define V_PCH_XHCI_FUS_HSPRTCNT_00B_CNT     14
#define V_PCH_XHCI_FUS_HSPRTCNT_01B_CNT     12
#define V_PCH_XHCI_FUS_HSPRTCNT_10B_CNT     10
#define V_PCH_XHCI_FUS_HSPRTCNT_11B_CNT     8
#define V_PCH_XHCI_FUS_HSPRTCNT_00B_MASK    0x3FFF
#define V_PCH_XHCI_FUS_HSPRTCNT_01B_MASK    0x0FFF
#define V_PCH_XHCI_FUS_HSPRTCNT_10B_MASK    0x03FF
#define V_PCH_XHCI_FUS_HSPRTCNT_11B_MASK    0x00FF
#define B_PCH_XHCI_FUS_XHCFD                BIT0  // XHCI Function Disable

#define R_PCH_XHCI_U2OCM1                   0xB0  // XHCI USB2 Overcurrent Pin Mapping 1
#define B_PCH_XHCI_U2OCM1_PORT_MAP          0x1F

#define R_PCH_XHCI_U2OCM2                   0xB4  // XHCI USB2 Overcurrent Pin Mapping 2
#define B_PCH_XHCI_U2OCM2_PORT_MAP          0x1F

#define R_PCH_XHCI_U3OCM1                   0xD0  // XHCI USB3 Overcurrent Pin Mapping 1
#define B_PCH_XHCI_U3OCM1_PORT_MAP          0x0F

#define R_PCH_XHCI_U3OCM2                   0xD4  // XHCI USB3 Overcurrent Pin Mapping 2
#define B_PCH_XHCI_U3OCM2_PORT_MAP          0x0F

//
// xHCI MMIO registers
//

//
// 0x00 - 0x1F - Capability Registers
//
#define R_PCH_XHCI_CAPLENGTH                0x00  // Capability Registers Length

#define R_PCH_XHCI_HCIVERSION               0x02  // Host Controller Interface Version Number

#define R_PCH_XHCI_HCSPARAMS1               0x04  // Structural Parameters 1
#define B_PCH_XHCI_HCSPARAMS1_MAXPORTS      0xFF000000 // Number of Ports
#define B_PCH_XHCI_HCSPARAMS1_MAXINTRS      0x7FF00 // Number of Interrupters
#define B_PCH_XHCI_HCSPARAMS1_MAXSLOTS      0xFF  // Number of Device Slots

#define R_PCH_XHCI_HCSPARAMS2               0x08  // Structural Parameters 2
#define B_PCH_XHCI_HCSPARAMS2_MSB           0xF8000000 // Max Scratchpad Buffers
#define B_PCH_XHCI_HCSPARAMS2_ERSTMAX       0xF0  // Event Ring Segment Table Max
#define B_PCH_XHCI_HCSPARAMS2_IST           0x0F  // Isochronous Scheduling Threshold

#define R_PCH_XHCI_HCSPARAMS3               0x0C  // Structural Parameters 3
#define B_PCH_XHCI_HCSPARAMS3_U2DEL         0xFFFF0000 // U2 Device Exit Latency
#define B_PCH_XHCI_HCSPARAMS3_U1DEL         0x000000FF // U1 Device Exit Latency

#define R_PCH_XHCI_HCCPARAMS                0x10  // Capability Parameters
#define B_PCH_XHCI_HCCPARAMS_XECP           0xFFFF0000 // xHCI Extended Capabilities Pointer
#define B_PCH_XHCI_HCCPARAMS_MAXPSASIZE     (BIT15 | BIT14 | BIT13 | BIT12) // Maximum Primary Stream Array Size
#define B_PCH_XHCI_HCCPARAMS_CFC            BIT11 // Contiguous Frame ID Capability
#define B_PCH_XHCI_HCCPARAMS_SEC            BIT10 // Stopped EDLTA Capability
#define B_PCH_XHCI_HCCPARAMS_SPC            BIT9  // Stopped Short Packet Capability
#define B_PCH_XHCI_HCCPARAMS_PAE            BIT8  // Parst All Event Data
#define B_PCH_XHCI_HCCPARAMS_NSS            BIT7  // No Secondary SID Support
#define B_PCH_XHCI_HCCPARAMS_LTC            BIT6  // Latency Tolerance Messaging Capability
#define B_PCH_XHCI_HCCPARAMS_LHRC           BIT5  // Light HC Reset Capability
#define B_PCH_XHCI_HCCPARAMS_PIND           BIT4  // Port Indicators
#define B_PCH_XHCI_HCCPARAMS_PPC            BIT3  // Port Power Control
#define B_PCH_XHCI_HCCPARAMS_CSZ            BIT2  // Context Size
#define B_PCH_XHCI_HCCPARAMS_BNC            BIT1  // BW Negotiation Capability
#define B_PCH_XHCI_HCCPARAMS_AC64           BIT0  // 64-bit Addressing Capability

#define R_PCH_XHCI_DBOFF                    0x14  // Doorbell Offset
#define B_PCH_XHCI_DBOFF_DBAO               0xFFFFFFFC // Doorbell Array Offset

#define R_PCH_XHCI_RTSOFF                   0x18  // Runtime Register Space Offset
#define B_PCH_XHCI_RTSOFF_RTRSO             0xFFFFFFE0 // Runtime Register Space Offset

//
// 0x80 - 0xBF - Operational Registers
//
#define R_PCH_XHCI_USBCMD                   0x80  // USB Command
#define B_PCH_XHCI_USBCMD_EU3S              BIT11 // Enable U3 MFINDEX Stop
#define B_PCH_XHCI_USBCMD_EWE               BIT10 // Enable Wrap Event
#define B_PCH_XHCI_USBCMD_CRS               BIT9  // Controller Restore State
#define B_PCH_XHCI_USBCMD_CSS               BIT8  // Controller Save State
#define B_PCH_XHCI_USBCMD_LHCRST            BIT7  // Light Host Controller Reset
#define B_PCH_XHCI_USBCMD_HSEE              BIT3  // Host System Error Enable
#define B_PCH_XHCI_USBCMD_INTE              BIT2  // Interrupter Enable
#define B_PCH_XHCI_USBCMD_HCRST             BIT1  // Host Controller Reset
#define B_PCH_XHCI_USBCMD_RS                BIT0  // Run/Stop

#define R_PCH_XHCI_USBSTS                   0x84  // USB Status
#define B_PCH_XHCI_USBSTS_HCE               BIT12 // Host Controller Error
#define B_PCH_XHCI_USBSTS_CNR               BIT11 // Controller Not Ready
#define B_PCH_XHCI_USBSTS_SRE               BIT10 // Save / Restore Error
#define B_PCH_XHCI_USBSTS_RSS               BIT9  // Restore State Status
#define B_PCH_XHCI_USBSTS_SSS               BIT8  // Save State Status
#define B_PCH_XHCI_USBSTS_PCD               BIT4  // Port Change Detect
#define B_PCH_XHCI_USBSTS_EINT              BIT3  // Event Interrupt
#define B_PCH_XHCI_USBSTS_HSE               BIT2  // Host System Error
#define B_PCH_XHCI_USBSTS_HCH               BIT0  // HC Halted

//
// 0x480 - 0x54F - Port Status and Control Registers
//
#define R_PCH_XHCI_PORTSC01USB2             0x480
#define R_PCH_XHCI_PORTSC02USB2             0x490
#define R_PCH_XHCI_PORTSC03USB2             0x4A0
#define R_PCH_XHCI_PORTSC04USB2             0x4B0
#define R_PCH_XHCI_PORTSC05USB2             0x4C0
#define R_PCH_XHCI_PORTSC06USB2             0x4D0
#define R_PCH_XHCI_PORTSC07USB2             0x4E0
#define B_PCH_XHCI_PORTSCXUSB2_WPR          BIT31 // Warm Port Reset
#define B_PCH_XHCI_PORTSCXUSB2_DR           BIT30 // Device Removable
#define B_PCH_XHCI_PORTSCXUSB2_WOE          BIT27 // Wake on Over-Current Enable
#define B_PCH_XHCI_PORTSCXUSB2_WDE          BIT26 // Wake on Disconnect Enable
#define B_PCH_XHCI_PORTSCXUSB2_WCE          BIT25 // Wake on Connect Enable
#define B_PCH_XHCI_PORTSCXUSB2_CAS          BIT24 // Cold Attach Status
#define B_PCH_XHCI_PORTSCXUSB2_CEC          BIT23 // Port Config Error Change
#define B_PCH_XHCI_PORTSCXUSB2_PLC          BIT22 // Port Link State Change
#define B_PCH_XHCI_PORTSCXUSB2_PRC          BIT21 // Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB2_OCC          BIT20 // Over-current Change
#define B_PCH_XHCI_PORTSCXUSB2_WRC          BIT19 // Warm Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB2_PEC          BIT18 // Port Enabled Disabled Change
#define B_PCH_XHCI_PORTSCXUSB2_CSC          BIT17 // Connect Status Change
#define B_PCH_XHCI_PORTSCXUSB2_LWS          BIT16 // Port Link State Write Strobe
#define B_PCH_XHCI_PORTSCXUSB2_PIC          (BIT15 | BIT14) // Port Indicator Control
#define B_PCH_XHCI_PORTSCXUSB2_PS           (BIT13 | BIT12 | BIT11 | BIT10) // Port Speed
#define B_PCH_XHCI_PORTSCXUSB2_PP           BIT9  // Port Power
#define B_PCH_XHCI_PORTSCXUSB2_PLS          (BIT8 | BIT7 | BIT6 | BIT5) // Port Link State
#define B_PCH_XHCI_PORTSCXUSB2_PR           BIT4  // Port Reset
#define B_PCH_XHCI_PORTSCXUSB2_OCA          BIT3  // Over-Current Active
#define B_PCH_XHCI_PORTSCXUSB2_PED          BIT1  // Port Enabled Disabled
#define B_PCH_XHCI_PORTSCXUSB2_CCS          BIT0  // Current Connect Status

#define R_PCH_XHCI_PORTSC08USB3             0x4F0
#define R_PCH_XHCI_PORTSC09USB3             0x500
#define R_PCH_XHCI_PORTSC10USB3             0x510
#define R_PCH_XHCI_PORTSC11USB3             0x520
#define R_PCH_XHCI_PORTSC12USB3             0x530
#define R_PCH_XHCI_PORTSC13USB3             0x540
#define B_PCH_XHCI_PORTSCXUSB3_WPR          BIT31  // Warm Port Reset
#define B_PCH_XHCI_PORTSCXUSB3_CEC          BIT23  // Port Config Error Change
#define B_PCH_XHCI_PORTSCXUSB3_PLC          BIT22  // Port Link State Change
#define B_PCH_XHCI_PORTSCXUSB3_PRC          BIT21  // Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB3_OCC          BIT20  // Over-current Change
#define B_PCH_XHCI_PORTSCXUSB3_WRC          BIT19  // Warm Port Reset Change
#define B_PCH_XHCI_PORTSCXUSB3_PEC          BIT18  // Port Enabled Disabled Change
#define B_PCH_XHCI_PORTSCXUSB3_CSC          BIT17  // Connect Status Change
#define B_PCH_XHCI_PORTSCXUSB3_PP           BIT9   // Port Power
#define B_PCH_XHCI_PORTSCXUSB3_PR           BIT4   // Port Reset
#define B_PCH_XHCI_PORTSCXUSB3_PED          BIT1   // Port Enabled / Disabled

#define R_PCH_XHCI_USB2PDO                  0x84F8 // USB2 Port Disable Override
#define B_PCH_XHCI_USB2PDO_MASK             0x7F
#define B_PCH_XHCI_USB2PDO_DIS_PORT0        BIT0

#define R_PCH_XHCI_USB3PDO                  0x84FC // USB3 Port Disable Override
#define B_PCH_XHCI_USB3PDO_MASK             0x3F
#define B_PCH_XHCI_USB3PDO_DIS_PORT0        BIT0

//
// USB3 OTG PCI Config Space Registers
//
#define PCI_DEVICE_NUMBER_PCH_OTG           22
#define PCI_FUNCTION_NUMBER_PCH_OTG         0

#define R_PCH_OTG_DEVVENDID                 0x00  // Vendor ID
#define V_PCH_USB_DEVVENDID_VID             V_PCH_INTEL_VENDOR_ID

#define R_PCH_OTG_STSCMD                    0x04  // Command Status
#define B_PCH_OTG_STSCMD_INTR_DIS           BIT10 // Interrupt Disable
#define B_PCH_OTG_STSCMD_BME                BIT2  // Bus Master Enable
#define B_PCH_OTG_STSCMD_MSE                BIT1  // Memory Space Enable

#define R_PCH_OTG_BAR0                      0x10  // BAR 0
#define B_PCH_OTG_BAR0_BA                   0xFFE00000 // Base Address
#define V_PCH_OTG_BAR0_SIZE                 0x200000
#define N_PCH_OTG_BAR0_ALIGNMENT            21
#define B_PCH_OTG_BAR0_PREF                 BIT3  // Prefetchable
#define B_PCH_OTG_BAR0_ADDRNG               (BIT2 | BIT1) // Address Range
#define B_PCH_OTG_BAR0_SPTYP                BIT0  // Space Type (Memory)

#define R_PCH_OTG_BAR1                      0x18  // BAR 1
#define B_PCH_OTG_BAR1_BA                   0xFFFFF000 // Base Address
#define B_PCH_OTG_BAR1_PREF                 BIT3  // Prefetchable
#define B_PCH_OTG_BAR1_ADDRNG               (BIT2 | BIT1) // Address Range
#define B_PCH_OTG_BAR1_SPTYP                BIT0  // Space Type (Memory)
#define V_PCH_OTG_BAR1_SIZE                 (1 << 12)

#define R_PCH_OTG_SSID                      0x2C  // Sub System ID
#define B_PCH_OTG_SSID_SID                  0xFFFF0000 // Sub System ID
#define B_PCH_OTG_SSID_SVID                 0x0000FFFF // Sub System Vendor ID

#define R_PCH_OTG_PMECTLSTS                 0x84  // PME Control Status
#define B_PCH_OTG_PMECTLSTS_POWERSTATE      (BIT1 | BIT0) // Power State

#define R_PCH_OTG_GEN_REGRW1                0xA0
#define B_PCH_OTG_GEN_REGRW1_PPSR           (BIT1 | BIT0) // PM Power State Request

#define R_PCH_OTG_GEN_INPUT_REGRW           0xC0
#define B_PCH_OTG_GEN_INPUT_REGRW_CPSU3     (BIT11 | BIT10) // Current Power State u3pmu
#define B_PCH_OTG_GEN_INPUT_REGRW_CPSU2     (BIT9 | BIT8) // Current Power State u2pmu



#endif
