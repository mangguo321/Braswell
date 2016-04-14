/** @file
  Register names for SATA controllers.

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

#ifndef _PCH_REGS_SATA_H_
#define _PCH_REGS_SATA_H_

//
//  SATA Controller Registers (D19:F0)
//
#define PCI_DEVICE_NUMBER_PCH_SATA          19
#define PCI_FUNCTION_NUMBER_PCH_SATA        0

#define R_PCH_SATA_ID                       0x00  // Identifiers
#define B_PCH_SATA_ID_DID                   0xFFFF0000 // Device ID
#define B_PCH_SATA_ID_VID                   0x0000FFFF // Vendor ID
#define V_PCH_SATA_VENDOR_ID                V_PCH_INTEL_VENDOR_ID

#define V_PCH_SATA_DEVICE_ID_D_IDE          0x22A0  // Desktop IDE Mode (Ports 0 and 1)
#define V_PCH_SATA_DEVICE_ID_M_IDE          0x22A1  // Mobile IDE Mode (Ports 0 and 1)
#define V_PCH_SATA_DEVICE_ID_D_AHCI         0x22A2  // Desktop AHCI Mode (Ports 0 and 1)
#define V_PCH_SATA_DEVICE_ID_M_AHCI         0x22A3  // Mobile AHCI Mode (Ports 0 and 1)
#define V_PCH_SATA_DEVICE_ID_D_RAID         0x2822  // Desktop RAID 0/1/5/10 Mode, based on D19:F0:9Ch[7]
#define V_PCH_SATA_DEVICE_ID_M_RAID         0x282A  // Mobile RAID 0/1/5/10 Mode, based on D19:F0:9Ch[7]

#define R_PCH_SATA_COMMAND                  0x04  // Command
#define B_PCH_SATA_COMMAND_INT_DIS          BIT10 // Interrupt Disable
#define B_PCH_SATA_COMMAND_FBE              BIT9  // Fast Back-to-back Enable
#define B_PCH_SATA_COMMAND_SERR_EN          BIT8  // SERR# Enable
#define B_PCH_SATA_COMMAND_WCC              BIT7  // Wait Cycle Enable
#define B_PCH_SATA_COMMAND_PER              BIT6  // Parity Error Response Enable
#define B_PCH_SATA_COMMAND_VPS              BIT5  // VGA Palette Snooping Enable
#define B_PCH_SATA_COMMAND_PMWE             BIT4  // Memory Write and Invalidate Enable
#define B_PCH_SATA_COMMAND_SCE              BIT3  // Special Cycle Enable
#define B_PCH_SATA_COMMAND_BME              BIT2  // Bus Master Enable
#define B_PCH_SATA_COMMAND_MSE              BIT1  // Memory Space Enable
#define B_PCH_SATA_COMMAND_IOSE             BIT0  // I/O Space Enable

#define R_PCH_SATA_PCISTS                   0x06  // Device Status
#define B_PCH_SATA_PCISTS_DPE               BIT15 // Detected Parity Error
#define B_PCH_SATA_PCISTS_SSE               BIT14 // Signaled System Error
#define B_PCH_SATA_PCISTS_RMA               BIT13 // Received Master-Abort Status
#define B_PCH_SATA_PCISTS_RTA               BIT12 // Received Target-Abort Status
#define B_PCH_SATA_PCISTS_STA               BIT11 // Signaled Target-Abort Status
#define B_PCH_SATA_PCISTS_DEV_STS_MASK      (BIT10 | BIT9) // DEVSEL# Timing Status
#define B_PCH_SATA_PCISTS_DPED              BIT8  // Master Data Parity Error Detected
#define B_PCH_SATA_PCISTS_CAP_LIST          BIT4  // Capabilities List
#define B_PCH_SATA_PCISTS_ITNS              BIT3  // Interrupt Status

#define R_PCH_SATA_RID                      0x08  // Revision ID (8 bits)

#define R_PCH_SATA_PI_REGISTER              0x09  // Programming Interface (8 bits)
#define B_PCH_SATA_PI_REGISTER_SNC          BIT3  // Secondary Mode Native Capable
#define B_PCH_SATA_PI_REGISTER_SNE          BIT2  // Secondary Mode Native Enable
#define B_PCH_SATA_PI_REGISTER_PNC          BIT1  // Primary Mode Native Capable
#define B_PCH_SATA_PI_REGISTER_PNE          BIT0  // Primary Mode Native Enable

#define R_PCH_SATA_CC                       0x0A  // Class Code
#define B_PCH_SATA_CC_BCC                   0xFF00 // Base Class Code
#define B_PCH_SATA_CC_SCC                   0x00FF // Sub Class Code
#define V_PCH_SATA_CC_SCC_IDE               0x01
#define V_PCH_SATA_CC_SCC_AHCI              0x06
#define V_PCH_SATA_CC_SCC_RAID              0x04

#define R_PCH_SATA_CLS                      0x0C  // Cache Line Size (8 bits)
#define B_PCH_SATA_CLS                      0xFF

#define R_PCH_SATA_MLT                      0x0D  // Master Latency Timer (8 bits)
#define B_PCH_SATA_MLT                      0xFF

#define R_PCH_SATA_HTYPE                    0x0E  // Header Type
#define B_PCH_SATA_HTYPE_MFD                BIT7  // Multi-function Device
#define B_PCH_SATA_HTYPE_HL                 0x7F  // Header Layout
#define R_PCH_SATA_PCMD_BAR                 0x10  // Primary Command Block Base Address
#define B_PCH_SATA_PCMD_BAR_BA              0x0000FFF8 // Base Address
#define B_PCH_SATA_PCMD_BAR_RTE             BIT0  // Resource Type Indicator

#define R_PCH_SATA_PCTL_BAR                 0x14  // Primary Control Block Base Address
#define B_PCH_SATA_PCTL_BAR_BA              0x0000FFFC // Base Address
#define B_PCH_SATA_PCTL_BAR_RTE             BIT0  // Resource Type Indicator

#define R_PCH_SATA_SCMD_BAR                 0x18  // Secondary Command Block Base Address
#define B_PCH_SATA_SCMD_BAR_BA              0x0000FFF8 // Base Address
#define B_PCH_SATA_SCMD_BAR_RTE             BIT0  // Resource Type Indicator

#define R_PCH_SATA_SCTL_BAR                 0x1C  // Secondary Control Block Base Address
#define B_PCH_SATA_SCTL_BAR_BA              0x0000FFFC // Base Address
#define B_PCH_SATA_SCTL_BAR_RTE             BIT0  // Resource Type Indicator

#define R_PCH_SATA_LBAR                     0x20  // AHCI Index Data Pair Base Address
#define B_PCH_SATA_LBAR_BA                  0x0000FFE0 // Base Address
#define B_PCH_SATA_LBAR_RTE                 BIT0  // Resource Type Indicator

#define R_PCH_SATA_ABAR                     0x24  // AHCI Base Address
#define B_PCH_SATA_ABAR_BA                  0xFFFFF800 // AHCI Memory Base Address (When CC.SCC not equal 0x01)
#define V_PCH_SATA_ABAR_LENGTH              0x800 // AHCI Memory Length (When CC.SCC not equal 0x01)
#define N_PCH_SATA_ABAR_ALIGNMENT           11    // AHCI Base Address Alignment (When CC.SCC not equal 0x01)
#define B_PCH_SATA_ABAR_PF                  BIT3  // Prefetchable
#define B_PCH_SATA_ABAR_TP                  (BIT2 | BIT1) // Type
#define B_PCH_SATA_ABAR_RTE                 BIT0  // Resource Type Indicator

#define R_PCH_SATA_SS                       0x2C  // Sub System Identifiers
#define B_PCH_SATA_SS_SSID                  0xFFFF0000 // Subsystem ID
#define B_PCH_SATA_SS_SSVID                 0x0000FFFF // Subsystem Vendor ID

#define R_PCH_SATA_AHCI_CAP_PTR             0x34  // Capabilities Pointer (8 bits)
#define B_PCH_SATA_AHCI_CAP_PTR             0xFF

#define R_PCH_SATA_INTR                     0x3C  // Interrupt Information
#define B_PCH_SATA_INTR_IPIN                0xFFFF0000 // Interrupt Pin
#define B_PCH_SATA_INTR_ILINE               0x0000FFFF // Interrupt Line

#define R_PCH_SATA_PID                      0x70  // PCI Power Management Capability ID
#define B_PCH_SATA_PID_NEXT                 0xFF00 // Next Capability
#define V_PCH_SATA_PID_NEXT_0               0xB000
#define V_PCH_SATA_PID_NEXT_1               0xA800
#define B_PCH_SATA_PID_CID                  0x00FF // Cap ID

#define R_PCH_SATA_PC                       0x72  // PCI Power Management Capabilities
#define S_PCH_SATA_PC                       2
#define B_PCH_SATA_PC_PME                   (BIT15 | BIT14 | BIT13 | BIT12 | BIT11) // PME Support
#define V_PCH_SATA_PC_PME_0                 0x0000
#define V_PCH_SATA_PC_PME_1                 0x4000
#define B_PCH_SATA_PC_D2_SUP                BIT10 // D2 Support
#define B_PCH_SATA_PC_D1_SUP                BIT9  // D1 Support
#define B_PCH_SATA_PC_AUX_CUR               (BIT8 | BIT7 | BIT6) // Aux Current
#define B_PCH_SATA_PC_DSI                   BIT5  // Device Specific Initialization
#define B_PCH_SATA_PC_PME_CLK               BIT3  // PME Clock
#define B_PCH_SATA_PC_VER                   (BIT2 | BIT1 | BIT0) // VS

#define R_PCH_SATA_PMCS                     0x74  // PCI Power Management Control and Status
#define B_PCH_SATA_PMCS_PMES                BIT15 // PME Status
#define B_PCH_SATA_PMCS_PMEE                BIT8  // PME Enable
#define B_PCH_SATA_PMCS_NSFRST              BIT3  // No Soft Reset
#define V_PCH_SATA_PMCS_NSFRST_1            0x01
#define V_PCH_SATA_PMCS_NSFRST_0            0x00
#define B_PCH_SATA_PMCS_PS                  (BIT1 | BIT0) // Power State
#define V_PCH_SATA_PMCS_PS_3                0x03
#define V_PCH_SATA_PMCS_PS_0                0x00

#define R_PCH_SATA_MID                      0x80  // Message Signaled Interrupt Identifier
#define B_PCH_SATA_MID_NEXT                 0xFF00 // Next Pointer
#define B_PCH_SATA_MID_CID                  0x00FF // Capability ID

#define R_PCH_SATA_MC                       0x82  // Message Signaled Interrupt Message Control
#define B_PCH_SATA_MC_C64                   BIT7  // 64 Bit Address Capable
#define B_PCH_SATA_MC_MME                   (BIT6 | BIT5 | BIT4) // Multiple Message Enable
#define V_PCH_SATA_MC_MME_4                 0x04
#define V_PCH_SATA_MC_MME_2                 0x02
#define V_PCH_SATA_MC_MME_1                 0x01
#define V_PCH_SATA_MC_MME_0                 0x00
#define B_PCH_SATA_MC_MMC                   (BIT3 | BIT2 | BIT1) // Multiple Message Capable
#define V_PCH_SATA_MC_MMC_4                 0x04
#define V_PCH_SATA_MC_MMC_0                 0x00
#define B_PCH_SATA_MC_MSIE                  BIT0  // MSI Enable
#define V_PCH_SATA_MC_MSIE_1                0x01
#define V_PCH_SATA_MC_MSIE_0                0x00

#define R_PCH_SATA_MA                       0x84  // Message Signaled Interrupt Message Address
#define B_PCH_SATA_MA                       0xFFFFFFFC // Address

#define R_PCH_SATA_MD                       0x88  // Message Signaled Interrupt Message Data
#define B_PCH_SATA_MD_MSIMD                 0xFFFF // Data

#define R_PCH_SATA_MAP                      0x90  // Port Mapping Register
#define B_PCH_SATA_MAP_SPD                  (BIT11 | BIT10 | BIT9 | BIT8) // SATA Port Disable
#define B_PCH_SATA_PORT3_DISABLED           BIT11
#define B_PCH_SATA_PORT2_DISABLED           BIT10
#define B_PCH_SATA_PORT1_DISABLED           BIT9
#define B_PCH_SATA_PORT0_DISABLED           BIT8
#define B_PCH_SATA_MAP_SMS_MASK             BIT6  // SATA Mode Select
#define V_PCH_SATA_MAP_SMS_AHCI             0x00
#define V_PCH_SATA_MAP_SMS_RAID             0x40

//
// SATA Workaround Bits
//
#define B_PCH_SATA_MAP_SMS_WA_BIT7          BIT7
#define B_PCH_SATA_MAP_SMS_WA_BIT5          BIT5

#define R_PCH_SATA_PCS                      0x92  // Port Control and Status
#define S_PCH_SATA_PCS                      0x2
#define B_PCH_SATA_PCS_OOB_RETRY            BIT15 // OOB Retry Mode
#define B_PCH_SATA_PCS_PORT5_DET            BIT13 // Port 5 Present
#define B_PCH_SATA_PCS_PORT4_DET            BIT12 // Port 4 Present
#define B_PCH_SATA_PCS_PORT3_DET            BIT11 // Port 3 Present
#define B_PCH_SATA_PCS_PORT2_DET            BIT10 // Port 2 Present
#define B_PCH_SATA_PCS_PORT1_DET            BIT9  // Port 1 Present
#define B_PCH_SATA_PCS_PORT0_DET            BIT8  // Port 0 Present
#define B_PCH_SATA_PCS_PORT5_EN             BIT5  // Port 5 Enabled
#define B_PCH_SATA_PCS_PORT4_EN             BIT4  // Port 4 Enabled
#define B_PCH_SATA_PCS_PORT3_EN             BIT3  // Port 3 Enabled
#define B_PCH_SATA_PCS_PORT2_EN             BIT2  // Port 2 Enabled
#define B_PCH_SATA_PCS_PORT1_EN             BIT1  // Port 1 Enabled
#define B_PCH_SATA_PCS_PORT0_EN             BIT0  // Port 0 Enabled

#define R_PCH_SATA_TM                       0x94  // Test Mode Register
#define B_PCH_SATA_TM_SNADD                 BIT31 // Speed Negotiation ALIGN Detection FSM Disable
#define B_PCH_SATA_TM_PORT5_PCD             BIT29 // Port 5 Clock Disable
#define B_PCH_SATA_TM_PORT4_PCD             BIT28 // Port 4 Clock Disable
#define B_PCH_SATA_TM_PORT3_PCD             BIT27 // Port 3 Clock Disable
#define B_PCH_SATA_TM_PORT2_PCD             BIT26 // Port 2 Clock Disable
#define B_PCH_SATA_TM_PORT1_PCD             BIT25 // Port 1 Clock Disable
#define B_PCH_SATA_TM_PORT0_PCD             BIT24 // Port 0 Clock Disable
#define B_PCH_SATA_TM_SSD                   BIT23 // SyncFIFO Exposure Disable
#define B_PCH_SATA_TM_SWTD                  BIT22 // SRST Watchdog Timer Disable
#define B_PCH_SATA_TM_SFS                   BIT20 // Method select to qualify the SyncFIFO space
#define B_PCH_SATA_TM_SLWS                  BIT19 // SyncFIFO Low Watermark Selection
#define B_PCH_SATA_TM_FRRD                  BIT18 // FIS Retry on RERR Disable
#define B_PCH_SATA_TM_FFBDC                 BIT17 // Fail FIS if BSY / DRQ Clear
#define B_PCH_SATA_TM_WTTM                  BIT16 // Command FIS Watchdog Timer Test Mode
#define B_PCH_SATA_TM_BEIE                  BIT14 // BERT Error Injection Enable
#define B_PCH_SATA_TM_RSIXE                 (BIT12 | BIT11 | BIT10) // Relaxed SATA Initialization Extension Enable
#define B_PCH_SATA_TM_MCO                   BIT9  // MSI Capability Override
#define B_PCH_SATA_TM_NQIUFD                BIT8  // AHCI Non-Queue Data-In Underflow Detection Enable
#define B_PCH_SATA_TM_SCTI                  BIT7  // DMA Stops CI Progress Enable
#define B_PCH_SATA_TM_PFAD                  BIT6  // PRD Fetch-Ahead Disable
#define B_PCH_SATA_TM_CPGD                  BIT5  // CONT Primitive Generation Disable
#define B_PCH_SATA_TM_BCUUD                 BIT4  // AHCI PRD Byte Count Update During AHCI data-In Underflow Disable
#define B_PCH_SATA_TM_SDFTSEL               (BIT3 | BIT2) // Short Data FIS Testmode Select [1:0]
#define B_PCH_SATA_TM_RRSSEL                (BIT1 | BIT0) // Read Request Size Select [1:0]

#define R_PCH_SATA_SATAGC                   0x9C  // SATA General Configuration
#define B_PCH_SATA_SATAGC_REGLOCK           BIT31 // Register Lock
#define B_PCH_SATA_SATAGC_WRRSELMPS         (BIT14 | BIT13 | BIT12) // Write Request Size Select / Max Payload Size
#define B_PCH_SATA_SATAGC_URRE              BIT9  // Unsupported Request Reporting Enable
#define B_PCH_SATA_SATAGC_URD               BIT8  // Unsupported Request Detected
#define B_PCH_SATA_SATAGC_AIE               BIT7  // Alternate ID Enable
#define B_PCH_SATA_SATAGC_DEVIDSEL          BIT6  // AIE0 DevID Selection
#define B_PCH_SATA_SATAGC_FLRCSSEL          BIT5  // FLR Capability Selection
#define B_PCH_SATA_SATAGC_MSS               (BIT4 | BIT3) // MXTBA Size Select
#define B_PCH_SATA_SATAGC_ASSEL             (BIT2 | BIT1 | BIT0) // ABAR Size Select

#define R_PCH_SATA_SIRI                     0xA0  // SATA Initialization Register Index
#define B_PCH_SATA_SIRI_IDX                 0xFC  // Index

#define R_PCH_SATA_SIRD                     0xA4  // SATA Initialization Register Data
#define B_PCH_SATA_SIRD_DTA                 0xFFFFFFFF // Data

#define R_PCH_SATA_CR0                      0xA8  // Serial ATA Capability Register 0
#define B_PCH_SATA_CR0_MAJREV               0x00F00000 // Major Revision
#define B_PCH_SATA_CR0_MINREV               0x000F0000 // Minor Revision
#define B_PCH_SATA_CR0_NEXT                 0x0000FF00 // Next Capability Pointer
#define B_PCH_SATA_CR0_CAP                  0x000000FF // Capability ID

#define R_PCH_SATA_CR1                      0xAC  // Serial ATA Capability Register 1
#define B_PCH_SATA_CR1_BAROFST              0xFFF0 // BAR Offset
#define B_PCH_SATA_CR1_BARLOC               0x000F // BAR Location

#define R_PCH_SATA_FLR_CID                  0xB0  // FLR Capability ID
#define B_PCH_SATA_FLR_CID_NEXT             0xFF00 // Next Capability Pointer
#define B_PCH_SATA_FLR_CID                  0x00FF // Capability ID
#define V_PCH_SATA_FLR_CID_1                0x0009
#define V_PCH_SATA_FLR_CID_0                0x0013

#define R_PCH_SATA_FLR_CLV                  0xB2  // FLR Capability Length and Version
#define B_PCH_SATA_FLR_CLV_FLRC_FLRCSSEL_0  BIT9  // FLR Capability
#define B_PCH_SATA_FLR_CLV_TXPC_FLRCSSEL_0  BIT8  // TXP Capability
#define B_PCH_SATA_FLR_CLV_VSCID_FLRCSSEL_0 0x00FF // Capability Length
#define B_PCH_SATA_FLR_CLV_VSCID_FLRCSSEL_1 0x00FF // Capability Length
#define V_PCH_SATA_FLR_CLV_VSCID_FLRCSSEL   0x0006

#define R_PCH_SATA_FLRC                     0xB4  // FLR Control
#define B_PCH_SATA_FLRC_TXP                 BIT8  // Transactions Pending
#define B_PCH_SATA_FLRC_INITFLR             BIT0  // Initiate FLR

#define R_PCH_SATA_BFCS                     0xE0  // BIST FIS Control / Status
#define B_PCH_SATA_BFCS_P5BFI               BIT15 // Port 5 BIST FIS Initiate
#define B_PCH_SATA_BFCS_P4BFI               BIT14 // Port 4 BIST FIS Initiate
#define B_PCH_SATA_BFCS_P3BFI               BIT13 // Port 3 BIST FIS Initiate
#define B_PCH_SATA_BFCS_P2BFI               BIT12 // Port 2 BIST FIS Initiate
#define B_PCH_SATA_BFCS_BFS                 BIT11 // BIST FIS Successful
#define B_PCH_SATA_BFCS_BFF                 BIT10 // BIST FIS Failed
#define B_PCH_SATA_BFCS_P1BFI               BIT9  // Port 1 BIST FIS Initiate
#define B_PCH_SATA_BFCS_P0BFI               BIT8  // Port 0 BIST FIS Initiate
#define B_PCH_SATA_BFCS_BIST_FIS_T          BIT7
#define B_PCH_SATA_BFCS_BIST_FIS_A          BIT6
#define B_PCH_SATA_BFCS_BIST_FIS_S          BIT5
#define B_PCH_SATA_BFCS_BIST_FIS_L          BIT4
#define B_PCH_SATA_BFCS_BIST_FIS_F          BIT3
#define B_PCH_SATA_BFCS_BIST_FIS_P          BIT2

#define R_PCH_SATA_BFTD1                    0xE4  // BIST FIS Transmit Data 1
#define B_PCH_SATA_BFTD1                    0xFFFFFFFF // Data

#define R_PCH_SATA_BFTD2                    0xE8  // BIST FIS Transmit Data 2
#define B_PCH_SATA_BFTD2                    0xFFFFFFFF // Data

#define R_PCH_SATA_MFID                     0xF8  // Manufacturing ID
#define B_PCH_SATA_MFID_DID                 0x0F000000 // DOT ID
#define B_PCH_SATA_MFID_SID                 0x00FF0000 // Stepping ID
#define B_PCH_SATA_MFID_MID                 0x0000FF00 // Manufacturer ID
#define B_PCH_SATA_MFID_PID                 0x000000FF // Process ID

//
// Memory AHCI BAR Area Related Registers
//
#define R_PCH_SATA_AHCI_CAP                 0x0   // HBA Capabilities
#define B_PCH_SATA_AHCI_CAP_S64A            BIT31 // Supports 64-bit Addressing
#define B_PCH_SATA_AHCI_CAP_SCQA            BIT30 // Support Native Command Queuing Acceleration
#define B_PCH_SATA_AHCI_CAP_SSNTF           BIT29 // Supports SNotification Register
#define B_PCH_SATA_AHCI_CAP_SMPS            BIT28 // Supports Mechanical Presence (Formerly Interlock Switch)
#define B_PCH_SATA_AHCI_CAP_SSS             BIT27 // Supports Staggered Spin-up
#define B_PCH_SATA_AHCI_CAP_SALP            BIT26 // Supports Aggressive Link Power Management
#define B_PCH_SATA_AHCI_CAP_SAL             BIT25 // Supports Activity LED
#define B_PCH_SATA_AHCI_CAP_SCLO            BIT24 // Supports Command List Override
#define B_PCH_SATA_AHCI_CAP_ISS_MASK        (BIT23 | BIT22 | BIT21 | BIT20) // Interface Speed Support
#define N_PCH_SATA_AHCI_CAP_ISS             20    // Interface Speed Support
#define V_PCH_SATA_AHCI_CAP_ISS_6_0_G       0x03  // Gen 3 (6.0 Gbps)
#define V_PCH_SATA_AHCI_CAP_ISS_3_0_G       0x02  // Gen 2 (3.0 Gbps)
#define V_PCH_SATA_AHCI_CAP_ISS_1_5_G       0x01  // Gen 1 (1.5 Gbps)
#define B_PCH_SATA_AHCI_CAP_SNZO            BIT19 // Supports Non-Zero DMA Offsets
#define B_PCH_SATA_AHCI_CAP_SAM             BIT18 // Supports AHCI mode only
#define B_PCH_SATA_AHCI_CAP_PMS             BIT17 // Supports Port Multiplier
#define B_PCH_SATA_AHCI_CAP_PMD             BIT15 // PIO Multiple DRQ Block
#define B_PCH_SATA_AHCI_CAP_SSC             BIT14 // Slumber Slate Capable
#define B_PCH_SATA_AHCI_CAP_PSC             BIT13 // Partial State Capable
#define B_PCH_SATA_AHCI_CAP_NCS             0x1F00 // Indicating Support for 32 slots
#define B_PCH_SATA_AHCI_CAP_CCCS            BIT7  // Command Completion Coalescing Supported
#define B_PCH_SATA_AHCI_CAP_EMS             BIT6  // Enclosure Management Supported
#define B_PCH_SATA_AHCI_CAP_SXS             BIT5  // Supports External SATA
#define B_PCH_SATA_AHCI_CAP_NPS             0x001F // Number of Ports

#define R_PCH_SATA_AHCI_GHC                 0x04  // Global HBA Control
#define B_PCH_SATA_AHCI_GHC_AE              BIT31 // AHCI Enable
#define B_PCH_SATA_AHCI_GHC_MRSM            BIT2  // MSI Revert to Single Message
#define B_PCH_SATA_AHCI_GHC_IE              BIT1  // Interrupt Enable
#define B_PCH_SATA_AHCI_GHC_HR              BIT0  // HBA Reset

#define R_PCH_SATA_AHCI_IS                  0x08  // Interrupt Status Register
#define B_PCH_SATA_AHCI_IS_PORT5            BIT5  // Interrupt Pending Status Port 5
#define B_PCH_SATA_AHCI_IS_PORT4            BIT4  // Interrupt Pending Status Port 4
#define B_PCH_SATA_AHCI_IS_PORT3            BIT3  // Interrupt Pending Status Port 3
#define B_PCH_SATA_AHCI_IS_PORT2            BIT2  // Interrupt Pending Status Port 2
#define B_PCH_SATA_AHCI_IS_PORT1            BIT1  // Interrupt Pending Status Port 1
#define B_PCH_SATA_AHCI_IS_PORT0            BIT0  // Interrupt Pending Status Port 0

#define R_PCH_SATA_AHCI_PI                  0x0C  // Ports Implemented
#define B_PCH_SATA_PORT_MASK                0x3F
#define B_PCH_SATA_PORT5_IMPLEMENTED        BIT5  // Port 5 Implemented
#define B_PCH_SATA_PORT4_IMPLEMENTED        BIT4  // Port 4 Implemented
#define B_PCH_SATA_PORT3_IMPLEMENTED        BIT3  // Port 3 Implemented
#define B_PCH_SATA_PORT2_IMPLEMENTED        BIT2  // Port 2 Implemented
#define B_PCH_SATA_PORT1_IMPLEMENTED        BIT1  // Port 1 Implemented
#define B_PCH_SATA_PORT0_IMPLEMENTED        BIT0  // Port 0 Implemented

#define R_PCH_SATA_AHCI_VS                  0x10  // AHCI Version
#define B_PCH_SATA_AHCI_VS_MJR              0xFFFF0000 // Major Version Number
#define B_PCH_SATA_AHCI_VS_MNR              0x0000FFFF // Minor Version Number

#define R_PCH_SATA_AHCI_EM_LOC              0x1C  // Enclosure Management Location
#define B_PCH_SATA_AHCI_EM_LOC_OFST         0xFFFF0000 // Offset
#define B_PCH_SATA_AHCI_EM_LOC_SZ           0x0000FFFF // Buffer Size

#define R_PCH_SATA_AHCI_EM_CTRL             0x20  // Enclosure Management Control
#define B_PCH_SATA_AHCI_EM_CTRL_ATTR_PM     BIT27 // Port Multiplier Support
#define B_PCH_SATA_AHCI_EM_CTRL_ATTR_ALHD   BIT26 // Activity LED Hardware Driven
#define B_PCH_SATA_AHCI_EM_CTRL_ATTR_XMT    BIT25 // Transmit Only
#define B_PCH_SATA_AHCI_EM_CTRL_ATTR_SMB    BIT24 // Single Message Buffer
#define B_PCH_SATA_AHCI_EM_CTRL_SUPP_SGPIO  BIT19 // SGPIO Enclosure Management Messages
#define B_PCH_SATA_AHCI_EM_CTRL_SUPP_SES2   BIT18 // SES-2 Enclosure Management Messages
#define B_PCH_SATA_AHCI_EM_CTRL_SUPP_SAFTE  BIT17 // SAF-TE Enclosure Management Messages
#define B_PCH_SATA_AHCI_EM_CTRL_SUPP_LED    BIT16 // LED Message Types
#define B_PCH_SATA_AHCI_EM_CTRL_RST         BIT9  // Reset
#define B_PCH_SATA_AHCI_EM_CTRL_CTL_TM      BIT8  // Transmit Message
#define B_PCH_SATA_AHCI_EM_CTRL_STS_MR      BIT0  // Message Received

#define R_PCH_SATA_AHCI_CAP2                0x24  // HBA Capabilities Extended
#define B_PCH_SATA_AHCI_CAP2_DESO           BIT5  // DEVSLP Entrance from Slumber Only
#define B_PCH_SATA_AHCI_CAP2_SADM           BIT4  // Supports Aggressive DEVSLP Management
#define B_PCH_SATA_AHCI_CAP2_SDS            BIT3  // Supports DEVSLP
#define B_PCH_SATA_AHCI_CAP2_APST           BIT2  // Automatic Partial to Slumber Transitions
#define B_PCH_SATA_AHCI_CAP2_BOH            BIT0  // BIOS / OS Handoff (Not Supported)

#define R_PCH_SATA_AHCI_VSP                 0xA0  // Vendor Specific
#define B_PCH_SATA_AHCI_VSP_SFMS            BIT6  // Software Feature Mask Supported
#define B_PCH_SATA_AHCI_VSP_PFS             BIT5  // Premium Features Supported
#define B_PCH_SATA_AHCI_VSP_PT              BIT4  // Platform Type
#define B_PCH_SATA_AHCI_VSP_SRPIR           BIT3  // Supports RAID Platform ID Reporting

#define R_PCH_SATA_AHCI_VSCAP               0xA4  // Vendor Specific Capabilities Register
#define B_PCH_SATA_AHCI_VSCAP_PNRRO         0xFFFF0000 // PCIe NAND Remapped Register Offset
#define B_PCH_SATA_AHCI_VSCAP_MSL           0x00000FFE
#define B_PCH_SATA_AHCI_VSCAP_PNABRE        BIT0  // PCIe NAND AHCI BAR Remapped Enable

#define R_PCH_SATA_AHCI_RPID                0xC0  // RAID Platform ID
#define B_PCH_SATA_AHCI_RPID_OFST           0xFFFF0000 // Offset
#define B_PCH_SATA_AHCI_RPID_RPID           0x0000FFFF // RAID Platform ID

#define R_PCH_SATA_AHCI_PFB                 0xC4  // Premium Feature Block
#define B_PCH_SATA_AHCI_PFB_SEA             BIT1  // Supports Email Alert
#define B_PCH_SATA_AHCI_PFB_SOI             BIT0  // Supports OEM IOCTL

#define R_PCH_SATA_AHCI_SFM                 0xC8  // SW Feature Mask
#define B_PCH_SATA_AHCI_SFM_OUND            (BIT11 | BIT10) // OROM UI Normal Delay
#define B_PCH_SATA_AHCI_SFM_SRT             BIT9  // Smart Response Technology
#define B_PCH_SATA_AHCI_SFM_IROES           BIT8  // IRRT Only on ESATA
#define B_PCH_SATA_AHCI_SFM_LEDL            BIT7  // LED Locate
#define B_PCH_SATA_AHCI_SFM_HDDLK           BIT6  // HDD Unlock
#define B_PCH_SATA_AHCI_SFM_OROMUNB         BIT5  // OROM UI and Banner
#define B_PCH_SATA_AHCI_SFM_IRRT            BIT4  // IRRT
#define B_PCH_SATA_AHCI_SFM_R5E             BIT3  // R5 Enable
#define B_PCH_SATA_AHCI_SFM_R10E            BIT2  // R10 Enable
#define B_PCH_SATA_AHCI_SFM_R1E             BIT1  // R1 Enable
#define B_PCH_SATA_AHCI_SFM_R0E             BIT0  // R0 Enable
#define B_PCH_SATA_AHCI_SFM_LOWBYTES        0x1FF

#define R_PCH_SATA_AHCI_P0CLB               0x100 // Port 0 Command List Base Address
#define R_PCH_SATA_AHCI_P1CLB               0x180 // Port 1 Command List Base Address
#define B_PCH_SATA_AHCI_PXCLB               0xFFFFFC00  // Command List Base Address

#define R_PCH_SATA_AHCI_P0CLBU              0x104 // Port 0 Command List Base Address Upper 32-bits
#define R_PCH_SATA_AHCI_P1CLBU              0x184 // Port 1 Command List Base Address Upper 32-bits
#define B_PCH_SATA_AHCI_PXCLBU              0xFFFFFFFF // Command List Base Address Upper

#define R_PCH_SATA_AHCI_P0FB                0x108 // Port 0 FIS Base Address
#define R_PCH_SATA_AHCI_P1FB                0x188 // Port 1 FIS Base Address
#define B_PCH_SATA_AHCI_PXFB                0xFFFFFF00 // FIS Base Address

#define R_PCH_SATA_AHCI_P0FBU               0x10C // Port 0 FIS Base Address Upper 32-bits
#define R_PCH_SATA_AHCI_P1FBU               0x18C // Port 1 FIS Base Address Upper 32-bits
#define B_PCH_SATA_AHCI_PXFBU               0xFFFFFFFF // FIS Base Address Upper

#define R_PCH_SATA_AHCI_P0IS                0x110 // Port 0 Interrupt Status
#define R_PCH_SATA_AHCI_P1IS                0x190 // Port 1 Interrupt Status
#define B_PCH_SATA_AHCI_PXIS_CPDS           BIT31 // Cold Presence Detect Status
#define B_PCH_SATA_AHCI_PXIS_TFES           BIT30 // Task File Error Status
#define B_PCH_SATA_AHCI_PXIS_HBFS           BIT29 // Host Bus Fatal Error Status
#define B_PCH_SATA_AHCI_PXIS_HBDS           BIT28 // Host Bus Data Error Status
#define B_PCH_SATA_AHCI_PXIS_IFS            BIT27 // Interface Fatal Error Status
#define B_PCH_SATA_AHCI_PXIS_INFS           BIT26 // Interface Non-Fatal Error Status
#define B_PCH_SATA_AHCI_PXIS_OFS            BIT24 // Overflow Status
#define B_PCH_SATA_AHCI_PXIS_IPMS           BIT23 // Incorrect Port Multiplier Status
#define B_PCH_SATA_AHCI_PXIS_PRCS           BIT22 // PhyRdy Change Status
#define B_PCH_SATA_AHCI_PXIS_DMPS           BIT7  // Device Mechanical Presence Status (Formerly Interlock Switch)
#define B_PCH_SATA_AHCI_PXIS_PCS            BIT6  // Port Connect Change Status
#define B_PCH_SATA_AHCI_PXIS_DPS            BIT5  // Descriptor Processed
#define B_PCH_SATA_AHCI_PXIS_UFS            BIT4  // Unknown FIS Interrupt
#define B_PCH_SATA_AHCI_PXIS_SDBS           BIT3  // Set Device Bits Interrupt
#define B_PCH_SATA_AHCI_PXIS_DSS            BIT2  // DMA Setup FIS Interrupt
#define B_PCH_SATA_AHCI_PXIS_PSS            BIT1  // PIO Setup FIS Interrupt
#define B_PCH_SATA_AHCI_PXIS_DHRS           BIT0  // Device to Host Register FIS Interrupt

#define R_PCH_SATA_AHCI_P0IE                0x114 // Port 0 Interrupt Enable
#define R_PCH_SATA_AHCI_P1IE                0x194 // Port 1 Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_CPDE           BIT31 // Cold Presence Detect Enable
#define B_PCH_SATA_AHCI_PXIE_TFEE           BIT30 // Task File Error Enable
#define B_PCH_SATA_AHCI_PXIE_HBFE           BIT29 // Host Bus Fatal Error Enable
#define B_PCH_SATA_AHCI_PXIE_HBDE           BIT28 // Host Bus Data Error Enable
#define B_PCH_SATA_AHCI_PXIE_IFE            BIT27 // Interface Fatal Error Enable
#define B_PCH_SATA_AHCI_PXIE_INFE           BIT26 // Interface Non-Fatal Error Enable
#define B_PCH_SATA_AHCI_PXIE_OFE            BIT24 // Overflow Enable
#define B_PCH_SATA_AHCI_PXIE_IPME           BIT23 // Incorrect Port Multiplier Enable
#define B_PCH_SATA_AHCI_PXIE_PRCE           BIT22 // PhyRdy Change Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_DIE            BIT7  // Device Mechanical Enable (Formerly Interlock Switch)
#define B_PCH_SATA_AHCI_PXIE_PCE            BIT6  // Port Change Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_DPE            BIT5  // Descriptor Processed Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_UFIE           BIT4  // Unknown FIS Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_SDBE           BIT3  // Set Device Bits FIS Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_DSE            BIT2  // DMA Setup FIS Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_PSE            BIT1  // PIO Setup FIS Interrupt Enable
#define B_PCH_SATA_AHCI_PXIE_DHRE           BIT0  // Device to Host Register FIS Interrupt Enable

#define R_PCH_SATA_AHCI_P0CMD               0x118 // Port 0 Command
#define R_PCH_SATA_AHCI_P1CMD               0x198 // Port 1 Command
#define B_PCH_SATA_AHCI_PxCMD_ICC           (BIT31 | BIT30 | BIT29 | BIT28) // Interface Communication Control
#define B_PCH_SATA_AHCI_PxCMD_MASK          (BIT27 | BIT26 | BIT21 | BIT22 | BIT19 | BIT18)
#define B_PCH_SATA_AHCI_PxCMD_ASP           BIT27 // Aggressive Slumber Partial
#define B_PCH_SATA_AHCI_PxCMD_ALPE          BIT26 // Aggressive Link Power Management Enable
#define B_PCH_SATA_AHCI_PxCMD_DLAE          BIT25 // Drive LED on ATAPI Enable
#define B_PCH_SATA_AHCI_PxCMD_ATAPI         BIT24 // Device is ATAPI
#define B_PCH_SATA_AHCI_PxCMD_APSTE         BIT23 // Automatic Partial to Slumber Transitions Enable
#define B_PCH_SATA_AHCI_PxCMD_ESP           BIT21 // External SATA Port
#define B_PCH_SATA_AHCI_PxCMD_CPD           BIT20 // Cold Presence Detection
#define B_PCH_SATA_AHCI_PxCMD_MPSP          BIT19 // Mechanical Presence Switch Attached to Port
#define B_PCH_SATA_AHCI_PxCMD_HPCP          BIT18 // Hot Plug Capable Port
#define B_PCH_SATA_AHCI_PxCMD_CR            BIT15 // Command List Running
#define B_PCH_SATA_AHCI_PxCMD_FR            BIT14 // FIS Receive Running
#define B_PCH_SATA_AHCI_PxCMD_MPSS          BIT13 // Mechanical Presence Switch State (Formerly Interlock Switch)
#define B_PCH_SATA_AHCI_PxCMD_CCS           0x00001F00 // Current Command Slot
#define B_PCH_SATA_AHCI_PxCMD_FRE           BIT4  // FIS Receive Enable
#define B_PCH_SATA_AHCI_PxCMD_CLO           BIT3  // Command List Overide
#define B_PCH_SATA_AHCI_PxCMD_POD           BIT2  // Power On Device
#define B_PCH_SATA_AHCI_PxCMD_SUD           BIT1  // Spin-Up Device
#define B_PCH_SATA_AHCI_PxCMD_ST            BIT0  // Start

#define R_PCH_SATA_AHCI_P0TFD               0x120 // Port 0 Task File Data
#define R_PCH_SATA_AHCI_P1TFD               0x1A0 // Port 1 Task File Data
#define B_PCH_SATA_AHCI_PXTFD_ERR           0x0000FF00 // Error
#define B_PCH_SATA_AHCI_PXTFD_STS           0x000000FF // Status
#define B_PCH_SATA_AHCI_PXTFD_STS_BSY       BIT7  // Status Busy
#define B_PCH_SATA_AHCI_PXTFD_STS_DRQ       BIT3  // Status DRQ
#define B_PCH_SATA_AHCI_PXTFD_STS_ERR       BIT0  // Status Error

#define R_PCH_SATA_AHCI_P0SIG               0x124 // Port 0 Signature
#define R_PCH_SATA_AHCI_P1SIG               0x1A4 // Port 1 Signature
#define B_PCH_SATA_AHCI_PXSIG_LBA_HR        0xFF000000
#define B_PCH_SATA_AHCI_PXSIG_LBA_MR        0x00FF0000
#define B_PCH_SATA_AHCI_PXSIG_LBA_LR        0x0000FF00
#define B_PCH_SATA_AHCI_PXSIG_SCR           0x000000FF

#define R_PCH_SATA_AHCI_P0SSTS              0x128 // Port 0 Serial ATA Status
#define R_PCH_SATA_AHCI_P1SSTS              0x1A8 // Port 1 Serial ATA Status
#define B_PCH_SATA_AHCI_PXSSTS_IPM          0x00000F00 // Interface Power Management
#define B_PCH_SATA_AHCI_PXSSTS_IPM_0        0x00000000
#define B_PCH_SATA_AHCI_PXSSTS_IPM_1        0x00000100
#define B_PCH_SATA_AHCI_PXSSTS_IPM_2        0x00000200
#define B_PCH_SATA_AHCI_PXSSTS_IPM_6        0x00000600
#define B_PCH_SATA_AHCI_PXSSTS_SPD          0x000000F0 // Current Interface Speed
#define B_PCH_SATA_AHCI_PXSSTS_SPD_0        0x00000000 // Device not present or communication not established
#define B_PCH_SATA_AHCI_PXSSTS_SPD_1        0x00000010 // Generation 1 communication rate negotiated
#define B_PCH_SATA_AHCI_PXSSTS_SPD_2        0x00000020 // Generation 2 communication rate negotiated
#define B_PCH_SATA_AHCI_PXSSTS_SPD_3        0x00000030 // Generation 3 communication rate negotiated
#define B_PCH_SATA_AHCI_PXSSTS_DET          0x0000000F // Device Detection
#define B_PCH_SATA_AHCI_PXSSTS_DET_0        0x00000000 // No device detected and Phy communication not established
#define B_PCH_SATA_AHCI_PXSSTS_DET_1        0x00000001 // Device presence detected but Phy communication not established
#define B_PCH_SATA_AHCI_PXSSTS_DET_3        0x00000003 // Device presence detected and Phy communication established
#define B_PCH_SATA_AHCI_PXSSTS_DET_4        0x00000004 // Phy in offline mode

#define R_PCH_SATA_AHCI_P0SCTL              0x12C // Port 0 Serial ATA Control
#define R_PCH_SATA_AHCI_P1SCTL              0x1AC // Port 1 Serial ATA Control
#define B_PCH_SATA_AHCI_PXSCTL_IPM          0x00000F00 // Interface Power Management Transitions Allowed
#define V_PCH_SATA_AHCI_PXSCTL_IPM_0        0x00000000
#define V_PCH_SATA_AHCI_PXSCTL_IPM_1        0x00000100
#define V_PCH_SATA_AHCI_PXSCTL_IPM_2        0x00000200
#define V_PCH_SATA_AHCI_PXSCTL_IPM_3        0x00000300
#define B_PCH_SATA_AHCI_PXSCTL_SPD          0x000000F0 // Highest Allowable Speed
#define V_PCH_SATA_AHCI_PXSCTL_SPD_0        0x00000000 // No Speed Restriction
#define V_PCH_SATA_AHCI_PXSCTL_SPD_1        0x00000010 // Max Gen 1 Speed
#define V_PCH_SATA_AHCI_PXSCTL_SPD_2        0x00000020 // Max Gen 2 Speed
#define V_PCH_SATA_AHCI_PXSCTL_SPD_3        0x00000030 // Max Gen 3 Speed
#define B_PCH_SATA_AHCI_PXSCTL_DET          0x0000000F // Device Detection Initialization
#define V_PCH_SATA_AHCI_PXSCTL_DET_0        0x00000000 // No device detection or initialization action requested
#define V_PCH_SATA_AHCI_PXSCTL_DET_1        0x00000001 // Perform interface communication initialization sequence to establish communication
#define V_PCH_SATA_AHCI_PXSCTL_DET_4        0x00000004 // Disable the Serial ATA interface and put Phy in offline mode

#define R_PCH_SATA_AHCI_P0SERR              0x130 // Port 0 Serial ATA Error
#define R_PCH_SATA_AHCI_P1SERR              0x1B0 // Port 1 Serial ATA Error
#define B_PCH_SATA_AHCI_PXSERR_DIAG         0xFFFF0000 // Diagnostics
#define B_PCH_SATA_AHCI_PXSERR_ERR          0x0000FFFF // Error
#define B_PCH_SATA_AHCI_PXSERR_EXCHG        BIT26
#define B_PCH_SATA_AHCI_PXSERR_UN_FIS_TYPE  BIT25
#define B_PCH_SATA_AHCI_PXSERR_TRSTE_24     BIT24
#define B_PCH_SATA_AHCI_PXSERR_TRSTE_23     BIT23
#define B_PCH_SATA_AHCI_PXSERR_HANDSHAKE    BIT22
#define B_PCH_SATA_AHCI_PXSERR_CRC_ERROR    BIT21
#define B_PCH_SATA_AHCI_PXSERR_10B8B_DECERR BIT19
#define B_PCH_SATA_AHCI_PXSERR_COMM_WAKE    BIT18
#define B_PCH_SATA_AHCI_PXSERR_PHY_ERROR    BIT17
#define B_PCH_SATA_AHCI_PXSERR_PHY_RDY_CHG  BIT16
#define B_PCH_SATA_AHCI_PXSERR_INTRNAL_ERR  BIT11
#define B_PCH_SATA_AHCI_PXSERR_PROTOCOL_ERR BIT10
#define B_PCH_SATA_AHCI_PXSERR_PCDIE        BIT9
#define B_PCH_SATA_AHCI_PXSERR_TDIE         BIT8
#define B_PCH_SATA_AHCI_PXSERR_RCE          BIT1
#define B_PCH_SATA_AHCI_PXSERR_RDIE         BIT0

#define R_PCH_SATA_AHCI_P0SACT              0x134 // Port 0 Serial ATA Active
#define R_PCH_SATA_AHCI_P1SACT              0x1B4 // Port 1 Serial ATA Active
#define B_PCH_SATA_AHCI_PXSACT_DS           0xFFFFFFFF

#define R_PCH_SATA_AHCI_P0CI                0x138 // Port 0 Commands Issued
#define R_PCH_SATA_AHCI_P1CI                0x1B8 // Port 1 Commands Issued
#define B_PCH_SATA_AHCI_PXCI                0xFFFFFFFF

#define R_PCH_SATA_AHCI_P0DEVSLP            0x144 // Port [0] Device Sleep
#define R_PCH_SATA_AHCI_P1DEVSLP            0x1C4 // Port [1] Device Sleep
#define B_PCH_SATA_AHCI_PXDEVSLP_DM         0x1E000000 // DITO Multiplier
#define V_PCH_SATA_AHCI_PXDEVSLP_DM_16      0x1E000000
#define B_PCH_SATA_AHCI_PXDEVSLP_DITO       0x01FF8000 // DEVSLP Idle Timeout
#define V_PCH_SATA_AHCI_PXDEVSLP_DITO_625   0x01388000
#define B_PCH_SATA_AHCI_PXDEVSLP_MDAT       0x00007C00 // DEVSLP Minimum Assertion Time
#define B_PCH_SATA_AHCI_PXDEVSLP_DETO       0x000003FC // DEVSLP Exit Timeout
#define B_PCH_SATA_AHCI_PXDEVSLP_DSP        BIT1  // Device Sleep Present
#define B_PCH_SATA_AHCI_PXDEVSLP_ADSE       BIT0  // Aggressive DEVSLP Enable

#define R_PCH_SATA_AHCI_EM_MF               0x580 // Enclosure Management Message Format
#define B_PCH_SATA_AHCI_EM_MF_MTYPE         0x0F000000 // Message Type
#define B_PCH_SATA_AHCI_EM_MF_DSIZE         0x00FF0000 // Data Size
#define B_PCH_SATA_AHCI_EM_MF_MSIZE         0x0000FF00 // Message Size

#define R_PCH_SATA_AHCI_EM_LED              0x584 // Enclosure Management LED
#define B_PCH_SATA_AHCI_EM_LED_VAL          0xFFFF0000 // Value
#define B_PCH_SATA_AHCI_EM_LED_PM           0x0000FF00 // Port Multiplier Information
#define B_PCH_SATA_AHCI_EM_LED_HBA          0x000000FF // HBA Information

//
// Macros of capabilities for SATA controller which are used by SATA controller driver
//
//
//
// Define the individual capabilities of each SATA controller
//
#define PCH_SATA_MAX_CONTROLLERS            1     // Max SATA controllers number supported
#define PCH_SATA_MAX_DEVICES                2     // Max SATA devices number of single SATA channel
#define PCH_IDE_MAX_CHANNELS                2     // Max IDE channels number of single SATA controller
#define PCH_IDE_MAX_DEVICES                 2     // Max IDE devices number of single SATA channel
#define PCH_AHCI_MAX_PORTS                  2     // Max number of SATA ports
#define PCH_IDE_MAX_PORTS                   2     // Max number of IDE ports

//
// CFIO SouthWest SATAGP0 is the SATA port 0 reset pin.
//
#define R_PCH_CFIO_SATA_GP0_PAD_CFG0        0x5800
//
// CFIO SouthWest SATAGP1 is the SATA port 1 reset pin.
//
#define R_PCH_CFIO_SATA_GP1_PAD_CFG0        0x5808

#endif
