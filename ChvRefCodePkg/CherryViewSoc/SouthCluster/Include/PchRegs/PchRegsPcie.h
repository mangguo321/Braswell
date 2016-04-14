/** @file
  Register names for PCI-E root port devices

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

#ifndef _PCH_REGS_PCIE_H_
#define _PCH_REGS_PCIE_H_

#define PCH_PCIE_MAX_ROOT_PORTS                            4


/// PCI Express Root Ports (D28:F0~F3)
///
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS              28
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1           0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2           1
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3           2
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4           3

#define R_PCH_PCIE_ID                                      0x00  // Identifiers
#define B_PCH_PCIE_ID_DID                                  0xFFFF0000 // Device ID
#define V_PCH_PCIE_DEVICE_ID_0                             0x22C8  // PCIE Root Port #1
#define V_PCH_PCIE_DEVICE_ID_1                             0x22C9  // PCIE Root Port #1
#define V_PCH_PCIE_DEVICE_ID_2                             0x22CA  // PCIE Root Port #2
#define V_PCH_PCIE_DEVICE_ID_3                             0x22CB  // PCIE Root Port #2
#define V_PCH_PCIE_DEVICE_ID_4                             0x22CC  // PCIE Root Port #3
#define V_PCH_PCIE_DEVICE_ID_5                             0x22CD  // PCIE Root Port #3
#define V_PCH_PCIE_DEVICE_ID_6                             0x22CE  // PCIE Root Port #4
#define V_PCH_PCIE_DEVICE_ID_7                             0x22CF  // PCIE Root Port #4
#define B_PCH_PCIE_ID_VID                                  0x0000FFFF // Vendor ID
#define V_PCH_PCIE_VENDOR_ID                               V_PCH_INTEL_VENDOR_ID

#define R_PCH_PCIE_CMD_PSTS                                0x04  // Device Command; Primary Status
#define S_PCH_PCIE_CMD_PSTS                                4
#define B_PCH_PCIE_CMD_PSTS_DPE                            BIT31 // Detected Parity Error
#define B_PCH_PCIE_CMD_PSTS_SSE                            BIT30 // Signaled System Error
#define B_PCH_PCIE_CMD_PSTS_RMA                            BIT29 // Received Master Abort
#define B_PCH_PCIE_CMD_PSTS_RTA                            BIT28 // Received Target Abort
#define B_PCH_PCIE_CMD_PSTS_STA                            BIT27 // Signaled Target Abort
#define B_PCH_PCIE_CMD_PSTS_DEV_STS                        (BIT26 | BIT25) // Primary DEVSEL# Timing Status
#define B_PCH_PCIE_CMD_PSTS_DPED                           BIT24 // Master Data Parity Error Detected
#define B_PCH_PCIE_CMD_PSTS_FB2BC                          BIT23 // Primary Fast Back to Back Capable
#define B_PCH_PCIE_CMD_PSTS_66MHZ_CAP                      BIT21 // Primary 66 MHz Capable
#define B_PCH_PCIE_CMD_PSTS_CAP_LST                        BIT20 // Capabilities List
#define B_PCH_PCIE_CMD_PSTS_INTR_STS                       BIT19 // Interrupt Status
#define B_PCH_PCIE_CMD_PSTS_ID                             BIT10 // Interrupt Disable
#define B_PCH_PCIE_CMD_PSTS_FBE                            BIT9  // Fast Back to Back Enable
#define B_PCH_PCIE_CMD_PSTS_SEE                            BIT8  // SERR# Enable
#define B_PCH_PCIE_CMD_PSTS_WCC                            BIT7  // Wait Cycle Control
#define B_PCH_PCIE_CMD_PSTS_PER                            BIT6  // Parity Error Response Enable
#define B_PCH_PCIE_CMD_PSTS_VPS                            BIT5  // VGA Palette Snoop
#define B_PCH_PCIE_CMD_PSTS_MWIE                           BIT4  // Memory Write and Invalidate Enable
#define B_PCH_PCIE_CMD_PSTS_SCE                            BIT3  // Special Cycle Enable
#define B_PCH_PCIE_CMD_PSTS_BME                            BIT2  // Bus Master Enable
#define B_PCH_PCIE_CMD_PSTS_MSE                            BIT1  // Memory Space Enable
#define B_PCH_PCIE_CMD_PSTS_IOSE                           BIT0  // I/O Space Enable

#define R_PCH_PCIE_RID_CC                                  0x08  // Revision ID; Class Code
#define B_PCH_PCIE_RID_CC_BCC                              0xFF000000 // Base Class Code
#define B_PCH_PCIE_RID_CC_SCC                              0x00FF0000 // Sub Class Code
#define B_PCH_PCIE_RID_CC_PI                               0x0000FF00 // Programming Interface
#define B_PCH_PCIE_RID_CC_RID                              0x000000FF // Revision ID

#define R_PCH_PCIE_CLS_PLT_HTYPE                           0x0C  // Cache Line size; Primary Latency Timer; Header Type
#define B_PCH_PCIE_CLS_PLT_HTYPE_MFD                       BIT23 // Multi-function Device
#define B_PCH_PCIE_CLS_PLT_HTYPE_HTYPE                     0x007F0000 // Header Type
#define B_PCH_PCIE_CLS_PLT_HTYPE_CT                        0x0000F800 // Latency Count
#define B_PCH_PCIE_CLS_PLT_HTYPE_LS                        0x000000FF // Line Size

#define R_PCH_PCIE_BNUM_SLT                                0x18  // Bus Numbers; Secondary Latency Timer
#define B_PCH_PCIE_BNUM_SLT_SLT                            0xFF000000 // Secondary Latency Timer
#define B_PCH_PCIE_BNUM_SLT_SBBN                           0x00FF0000 // Subordinate Bus Number
#define B_PCH_PCIE_BNUM_SLT_SCBN                           0x0000FF00 // Secondary Bus Number
#define B_PCH_PCIE_BNUM_SLT_PBN                            0x000000FF // Primary Bus Number

#define R_PCH_PCIE_IOBL_SSTS                               0x1C  // I/O Base and Limit; Secondary Status
#define B_PCH_PCIE_IOBL_SSTS_DPE                           BIT31 // Detected Parity Error
#define B_PCH_PCIE_IOBL_SSTS_RSE                           BIT30 // Received System Error
#define B_PCH_PCIE_IOBL_SSTS_RMA                           BIT29 // Received Master Abort
#define B_PCH_PCIE_IOBL_SSTS_RTA                           BIT28 // Received Target Abort
#define B_PCH_PCIE_IOBL_SSTS_STA                           BIT27 // Signaled Target Abort
#define B_PCH_PCIE_IOBL_SSTS_SDTS                          (BIT26 | BIT25) // Secondary DEVSEL# Timing Status
#define B_PCH_PCIE_IOBL_SSTS_DPD                           BIT24 // Data Parity Error Detected
#define B_PCH_PCIE_IOBL_SSTS_SFBC                          BIT23 // Secondary Fast Back to Back Capable
#define B_PCH_PCIE_IOBL_SSTS_SC66                          BIT22 // Secondary 66 MHz Capable
#define B_PCH_PCIE_IOBL_SSTS_IOLA                          0xF000 // I/O Address Limit
#define B_PCH_PCIE_IOBL_SSTS_IOLC                          0x0F00 // I/O Limit Address Capability
#define B_PCH_PCIE_IOBL_SSTS_IOBA                          0x00F0 // I/O Base Address
#define B_PCH_PCIE_IOBL_SSTS_IOBC                          0x000F // I/O Base Address Capability

#define R_PCH_PCIE_MBL                                     0x20  // Memory Base and Limit
#define B_PCH_PCIE_MBL_ML                                  0xFFF00000 // Memory Limit
#define B_PCH_PCIE_MBL_MB                                  0x0000FFF0 // Memory Base

#define R_PCH_PCIE_PMBL                                    0x24  // Prefetchable Memory Base and Limit
#define B_PCH_PCIE_PMBL_PML                                0xFFF00000 // Prefetchable Memory Limit
#define B_PCH_PCIE_PMBL_I64L                               0x000F0000 // 64-bit Indicator
#define B_PCH_PCIE_PMBL_PMB                                0x0000FFF0 // Prefetchable Memory Base
#define B_PCH_PCIE_PMBL_I64B                               0x0000000F // 64-bit Indicator

#define R_PCH_PCIE_PMBU32                                  0x28  // Prefetchable Memory Base Upper 32 Bits
#define B_PCH_PCIE_PMBU32                                  0xFFFFFFFF // Prefetchable Memory Base Upper Portion

#define R_PCH_PCIE_PMLU32                                  0x2C  // Prefetchable Memory Limit Upper 32 Bits
#define B_PCH_PCIE_PMLU32                                  0xFFFFFFFF // Prefetchable Memory Limit Upper Portion

#define R_PCH_PCIE_CAPP                                    0x34  // Capabilities List Pointer
#define B_PCH_PCIE_CAPP                                    0xFF  // Capabilities Pointer

#define R_PCH_PCIE_INTR_BCTRL                              0x3C  // Interrupt Information; Bridge Control
#define B_PCH_PCIE_INTR_BCTRL_DTSE                         BIT27 // Discard Timer SERR# Enable
#define B_PCH_PCIE_INTR_BCTRL_DTS                          BIT26 // Discard Timer Status
#define B_PCH_PCIE_INTR_BCTRL_SDT                          BIT25 // Secondary Discard Timer
#define B_PCH_PCIE_INTR_BCTRL_PDT                          BIT24 // Primary Discard Timer
#define B_PCH_PCIE_INTR_BCTRL_FBE                          BIT23 // Fast Back to Back Enable
#define B_PCH_PCIE_INTR_BCTRL_SBR                          BIT22 // Secondary Bus Reset
#define B_PCH_PCIE_INTR_BCTRL_MAM                          BIT21 // Master Abort Mode
#define B_PCH_PCIE_INTR_BCTRL_V16                          BIT20 // VGA 16-bit Decode
#define B_PCH_PCIE_INTR_BCTRL_VE                           BIT19 // VGA Enable
#define B_PCH_PCIE_INTR_BCTRL_IE                           BIT18 // ISA Enable
#define B_PCH_PCIE_INTR_BCTRL_SE                           BIT17 // SERR# Enable
#define B_PCH_PCIE_INTR_BCTRL_PERE                         BIT16 // Parity Error Response Enable
#define B_PCH_PCIE_INTR_BCTRL_IPIN                         0xFF00 // Interrupt Pin
#define B_PCH_PCIE_INTR_BCTRL_ILINE                        0x00FF // Interrupt Line

#define R_PCH_PCIE_CLIST_XCAP                              0x40  // Capabilities List; PCI Express Capabilities
#define B_PCH_PCIE_CLIST_XCAP_IMN                          0x3E000000 // Interrupt Message Number
#define B_PCH_PCIE_CLIST_XCAP_SI                           BIT24 // Slot Implemented
#define B_PCH_PCIE_CLIST_XCAP_DT                           0x00F00000 // Device / Port Type
#define B_PCH_PCIE_CLIST_XCAP_CV                           0x000F0000 // Capability Version
#define B_PCH_PCIE_CLIST_XCAP_NEXT                         0x0000FF00 // Next Capabilities
#define B_PCH_PCIE_CLIST_XCAP_CID                          0x000000FF // Capability ID

#define R_PCH_PCIE_DCAP                                    0x44  // Device Capabilities
#define S_PCH_PCIE_DCAP                                    4
#define B_PCH_PCIE_DCAP_CSPS                               0x0C000000 // Captured Slot Power Limit Scale (Not Supported)
#define B_PCH_PCIE_DCAP_CSPV                               0x03FC0000 // Captured Slot Power Limit Value (Not Supported)
#define B_PCH_PCIE_DCAP_RBER                               BIT15 // Role Based Error Reporting
#define B_PCH_PCIE_DCAP_PIP                                BIT14 // Reserved, previously was Power Indicator Present
#define B_PCH_PCIE_DCAP_AIP                                BIT13 // Reserved, previously was Attention Indicator Present
#define B_PCH_PCIE_DCAP_ABP                                BIT12 // Reserved, previously was Attention Button Present
#define B_PCH_PCIE_DCAP_E1AL                               0x00000E00 // Endpoint L1 Acceptable Latency
#define B_PCH_PCIE_DCAP_E0AL                               0x000001C0 // Endpoint L0 Acceptable Latency
#define B_PCH_PCIE_DCAP_ETFS                               BIT5  // Extended Tag Field Supported
#define B_PCH_PCIE_DCAP_PFS                                0x00000018 // Phantom Function Supported
#define B_PCH_PCIE_DCAP_MPS                                0x00000007 // Max Payload Size Supported

#define R_PCH_PCIE_DCTL_DSTS                               0x48  // Device Control; Device Status
#define S_PCH_PCIE_DCTL_DSTS                               4
#define B_PCH_PCIE_DCTL_DSTS_TDP                           BIT21 // Transactions Pending
#define B_PCH_PCIE_DCTL_DSTS_APD                           BIT20 // AUX Power Detected
#define B_PCH_PCIE_DCTL_DSTS_URD                           BIT19 // Unsupported Request Detected
#define B_PCH_PCIE_DCTL_DSTS_FED                           BIT18 // Fatal Error Detected
#define B_PCH_PCIE_DCTL_DSTS_NFED                          BIT17 // Non-Fatal Error Detected
#define B_PCH_PCIE_DCTL_DSTS_CED                           BIT16 // Correctable Error Detected
#define B_PCH_PCIE_DCTL_DSTS_MRRS                          0x7000 // Max Read Request Size
#define B_PCH_PCIE_DCTL_DSTS_ENS                           BIT11 // Enable No Snoop
#define B_PCH_PCIE_DCTL_DSTS_APME                          BIT10 // Aux Power PM Enable
#define B_PCH_PCIE_DCTL_DSTS_PFE                           BIT9  // Phantom Function Enable (Not Supported)
#define B_PCH_PCIE_DCTL_DSTS_ETFE                          BIT8  // Extended Tag Field Enable (Not Supported)
#define B_PCH_PCIE_DCTL_DSTS_MPS                           (BIT7 | BIT6 | BIT5) // Max Payload Size
#define B_PCH_PCIE_DCTL_DSTS_ERO                           BIT4 // Enable Relaxed Ordering (Not Supported)
#define B_PCH_PCIE_DCTL_DSTS_URE                           BIT3 // Unsupported Request Reporting Enable
#define B_PCH_PCIE_DCTL_DSTS_FEE                           BIT2 // Fatal Error Reporting Enable
#define B_PCH_PCIE_DCTL_DSTS_NFE                           BIT1 // Non-Fatal Error Reporting Enable
#define B_PCH_PCIE_DCTL_DSTS_CEE                           BIT0 // Correctable Error Reporting Enable

#define R_PCH_PCIE_LCAP                                    0x4C  // Link Capabilities
#define B_PCH_PCIE_LCAP_PN                                 0xFF000000 // Port Number
#define V_PCH_PCIE_LCAP_PN1                                (1 << 24) // Port Number 1
#define V_PCH_PCIE_LCAP_PN2                                (2 << 24) // Port Number 2
#define V_PCH_PCIE_LCAP_PN3                                (3 << 24) // Port Number 3
#define V_PCH_PCIE_LCAP_PN4                                (4 << 24) // Port Number 4
#define V_PCH_PCIE_LCAP_PN5                                (5 << 24) // Port Number 5
#define V_PCH_PCIE_LCAP_PN6                                (6 << 24) // Port Number 6
#define V_PCH_PCIE_LCAP_PN7                                (7 << 24) // Port Number 7
#define V_PCH_PCIE_LCAP_PN8                                (8 << 24) // Port Number 8
#define B_PCH_PCIE_LCAP_LBNC                               BIT21 // Link Bandwidth Notification Capability
#define B_PCH_PCIE_LCAP_LARC                               BIT20 // Link Active Reporting Capable
#define B_PCH_PCIE_LCAP_SDERC                              BIT19 // Surprise Down Error Reporting Capable
#define B_PCH_PCIE_LCAP_CPM                                BIT18 // Clock Power Management
#define B_PCH_PCIE_LCAP_EL1                                (BIT17 | BIT16 | BIT15) // L1 Exit Latency
#define B_PCH_PCIE_LCAP_EL0                                (BIT14 | BIT13 | BIT12) // L0s Exit Latency
#define B_PCH_PCIE_LCAP_APMS                               (BIT11 | BIT10) // Active State Link PM Support
#define V_PCH_PCIE_LCAP_APMS_L0S                           (1 << 10) // L0s Entry Supported
#define V_PCH_PCIE_LCAP_APMS_L0S_L1                        (3 << 10) // Both L0s and L1 Supported
#define B_PCH_PCIE_LCAP_MLW                                0x000003F0 // Maximum Link Width
#define B_PCH_PCIE_LCAP_SLS                                0x0000000F // Supported Link Speeds

#define R_PCH_PCIE_LCTL_LSTS                               0x50  // Link Control; Link Status
#define B_PCH_PCIE_LCTL_LSTS_LABS                          BIT31 // Link Autonomous Bandwidth Status
#define B_PCH_PCIE_LCTL_LSTS_LBMS                          BIT30 // Link Bandwidth Management Status
#define B_PCH_PCIE_LCTL_LSTS_DLLA                          BIT29 // Link Active
#define B_PCH_PCIE_LCTL_LSTS_SCC                           BIT28 // Slot Clock Configuration
#define B_PCH_PCIE_LCTL_LSTS_LT                            BIT27 // Link Training
#define B_PCH_PCIE_LCTL_LSTS_LTE                           BIT26 // Reserved, previously was Link Training Error
#define B_PCH_PCIE_LCTL_LSTS_NLW                           0x03F00000 // Negotiated Link Width
#define V_PCH_PCIE_LCTL_LSTS_NLW_1                         0x00100000
#define V_PCH_PCIE_LCTL_LSTS_NLW_2                         0x00200000
#define V_PCH_PCIE_LCTL_LSTS_NLW_4                         0x00400000
#define B_PCH_PCIE_LCTL_LSTS_LS                            0x000F0000 // Current Link Speed
#define B_PCH_PCIE_LCTL_LSTS_LABIE                         BIT11 // Link Autonomous Bandwidth Interrupt Enable
#define B_PCH_PCIE_LCTL_LSTS_LBMIE                         BIT10 // Link Bandwidth Management Interrupt Enable
#define B_PCH_PCIE_LCTL_LSTS_HAWD                          BIT9  // Hardware Autonomous Width Disable
#define B_PCH_PCIE_LCTL_LSTS_ES                            BIT7  // Extended Synch
#define B_PCH_PCIE_LCTL_LSTS_CCC                           BIT6  // Common Clock Configuration
#define B_PCH_PCIE_LCTL_LSTS_RL                            BIT5  // Retrain Link
#define B_PCH_PCIE_LCTL_LSTS_LD                            BIT4  // Link Disable
#define B_PCH_PCIE_LCTL_LSTS_RCBC                          BIT3  // Read Completion Boundary
#define B_PCH_PCIE_LCTL_LSTS_ASPM                          (BIT1 | BIT0) // Active State Link PM Control
#define V_PCH_PCIE_LCTL_LSTS_ASPM_L0S                      1     // L0s Entry Enabled
#define V_PCH_PCIE_LCTL_LSTS_ASPM_L1                       2     // L1 Entry Enable
#define V_PCH_PCIE_LCTL_LSTS_ASPM_L0S_L1                   3     // L0s and L1 Entry Enabled

#define R_PCH_PCIE_SLCAP                                   0x54  // Slot Capabilities
#define S_PCH_PCIE_SLCAP                                   4
#define B_PCH_PCIE_SLCAP_PSN                               0xFFF80000 // Physical Slot Number
#define N_PCH_PCIE_SLCAP_PSN                               19
#define B_PCH_PCIE_SLCAP_NCCS                              BIT18 // No Command Completed Support
#define B_PCH_PCIE_SLCAP_EMIP                              BIT17 // Electromechanical Interlock Present
#define B_PCH_PCIE_SLCAP_SLS                               (BIT16 | BIT15) // Slot Power Limit Scale
#define B_PCH_PCIE_SLCAP_SLV                               0x00007F80 // Slot Power Limit Value
#define B_PCH_PCIE_SLCAP_HPC                               BIT6  // Hot Plug Capable
#define B_PCH_PCIE_SLCAP_HPS                               BIT5  // Hot Plug Surprise
#define B_PCH_PCIE_SLCAP_PIP                               BIT4  // Power Indicator Present
#define B_PCH_PCIE_SLCAP_AIP                               BIT3  // Attention Indicator Present
#define B_PCH_PCIE_SLCAP_MSP                               BIT2  // MRL Sensor Present
#define B_PCH_PCIE_SLCAP_PCP                               BIT1  // Power Controller Present
#define B_PCH_PCIE_SLCAP_ABP                               BIT0  // Attention Buttion Present

#define R_PCH_PCIE_SLCTL_SLSTS                             0x58  // Slot Control; Slot Status
#define S_PCH_PCIE_SLCTL_SLSTS                             4
#define B_PCH_PCIE_SLCTL_SLSTS_DLLSC                       BIT24 // Data Link Layer State Changed
#define B_PCH_PCIE_SLCTL_SLSTS_PDS                         BIT22 // Presence Detect State
#define B_PCH_PCIE_SLCTL_SLSTS_MS                          BIT21 // MRL Sensor State
#define B_PCH_PCIE_SLCTL_SLSTS_PDC                         BIT19 // Presence Detect Changed
#define B_PCH_PCIE_SLCTL_SLSTS_MSC                         BIT18 // MRL Sensor Changed
#define B_PCH_PCIE_SLCTL_SLSTS_PFD                         BIT17 // Power Fault Detected
#define B_PCH_PCIE_SLCTL_SLSTS_DLLSCE                      BIT12 // Data Link Layer State Changed Enable
#define B_PCH_PCIE_SLCTL_SLSTS_PCC                         BIT10 // Power Controller Control
#define B_PCH_PCIE_SLCTL_SLSTS_HPE                         BIT5  // Hot Plug Interrupt Enable
#define B_PCH_PCIE_SLCTL_SLSTS_CCE                         BIT4  // Command Completed Interrupt Enable
#define B_PCH_PCIE_SLCTL_SLSTS_PDE                         BIT3  // Presence Detect Changed Enable

#define R_PCH_PCIE_RCTL                                    0x5C  // Root Control
#define S_PCH_PCIE_RCTL                                    2
#define B_PCH_PCIE_RCTL_PIE                                BIT3  // PME Interrupt Enable
#define B_PCH_PCIE_RCTL_SFE                                BIT2  // System Error on Fatal Error Enable
#define B_PCH_PCIE_RCTL_SNE                                BIT1  // System Error on Non-Fatal Error Enable
#define B_PCH_PCIE_RCTL_SCE                                BIT0  // System Error on Correctable Error Enable

#define R_PCH_PCIE_RSTS                                    0x60  // Root Status
#define S_PCH_PCIE_RSTS                                    4
#define B_PCH_PCIE_RSTS_PP                                 BIT17 // PME PEnding
#define B_PCH_PCIE_RSTS_PS                                 BIT16 // PME Status
#define B_PCH_PCIE_RSTS_RID                                0x0000FFFF // PME Requestor ID

#define R_PCH_PCIE_DCAP2                                   0x64  // Device Capabilities 2
#define B_PCH_PCIE_DCAP2_OBFFS                             (BIT19 | BIT18) // Optimized Buffer Flush / Fill Supported
#define B_PCH_PCIE_DCAP2_LTRMS                             BIT11 // LTR Mechanism Supported
#define B_PCH_PCIE_DCAP2_CTDS                              BIT4  // Completion Timeout Disable Supported
#define B_PCH_PCIE_DCAP2_CTRS                              0xF   // Completion Timeout Ranges Supported
#define V_PCH_PCIE_DCAP2_CTRS_UNSUPPORTED                  0x0
#define V_PCH_PCIE_DCAP2_CTRS_RANGE_A                      0x1   // 50 us to 10 ms
#define V_PCH_PCIE_DCAP2_CTRS_RANGE_B                      0x2   // 10 ms to 250 ms
#define V_PCH_PCIE_DCAP2_CTRS_RANGE_C                      0x4   // 250 ms to 4 s
#define V_PCH_PCIE_DCAP2_CTRS_RANGE_D                      0x8   // 4 s to 64 s

#define R_PCH_PCIE_DCTL2_DSTS2                             0x68  // Device Control 2; Device Status 2
#define B_PCH_PCIE_DCTL2_DSTS2_OBFFEN                      (BIT14 | BIT13) // Optimized Buffer Flush / Fill Enable
#define B_PCH_PCIE_DCTL2_DSTS2_LTRME                       BIT10 // LTR Mechanism Enable
#define B_PCH_PCIE_DCTL2_DSTS2_CTD                         BIT4  // Completion Timeout Disable
#define B_PCH_PCIE_DCTL2_DSTS2_CTV                         0xF   // Completion Timeout Value
#define V_PCH_PCIE_DCTL2_DSTS2_CTV_DEFAULT                 0x0
#define V_PCH_PCIE_DCTL2_DSTS2_CTV_40MS_50MS               0x5
#define V_PCH_PCIE_DCTL2_DSTS2_CTV_160MS_170MS             0x6
#define V_PCH_PCIE_DCTL2_DSTS2_CTV_400MS_500MS             0x9
#define V_PCH_PCIE_DCTL2_DSTS2_CTV_1P6S_1P7S               0xA

#define R_PCH_PCIE_LCTL2_LSTS2                             0x70  // Link Control 2; Link Status 2
#define B_PCH_PCIE_LCTL2_LSTS2_CDL                         BIT16 // Current De-emphasis Level
#define B_PCH_PCIE_LCTL2_LSTS2_CD                          BIT12 // Compliance De-emphasis
#define B_PCH_PCIE_LCTL2_LSTS2_CSOS                        BIT11 // Compliance SOS
#define B_PCH_PCIE_LCTL2_LSTS2_EMC                         BIT10 // Enter Modified Compliance
#define B_PCH_PCIE_LCTL2_LSTS2_TM                          (BIT9 | BIT8 | BIT7) // Transmit Margin
#define B_PCH_PCIE_LCTL2_LSTS2_SD                          BIT6  // Selectable De-emphasis
#define B_PCH_PCIE_LCTL2_LSTS2_HASD                        BIT5  // Reserved. Hardware Autonomous Speed Disable
#define B_PCH_PCIE_LCTL2_LSTS2_EC                          BIT4  // Enter Compliance
#define B_PCH_PCIE_LCTL2_LSTS2_TLS                         (BIT3 | BIT2 | BIT1 | BIT0) // Target Link Speed

#define R_PCH_PCIE_MID_MC                                  0x80  // Message Signaled Interrupt Identifiers; Message Signaled Interrupt Message Control
#define S_PCH_PCIE_MID_MC                                  4
#define B_PCH_PCIE_MID_MC_C64                              BIT23 // 64 Bit Address Capable
#define B_PCH_PCIE_MID_MC_MME                              (BIT22 | BIT21 | BIT20) // Multiple Message Enable
#define B_PCH_PCIE_MID_MC_MMC                              0x000E0000 // Multiple Message Capable
#define B_PCH_PCIE_MID_MC_MSIE                             BIT16 // MSI Enable
#define B_PCH_PCIE_MID_MC_NEXT                             0xFF00 // Next Pointer
#define B_PCH_PCIE_MID_MC_CID                              0x00FF // Capability ID

#define R_PCH_PCIE_MA                                      0x84  // Message Signaled Interrupt Message Address
#define S_PCH_PCIE_MA                                      4
#define B_PCH_PCIE_MA_ADDR                                 0xFFFFFFFC // Address

#define R_PCH_PCIE_MD                                      0x88  // Message Signaled Interrupt Message data
#define S_PCH_PCIE_MD                                      2
#define B_PCH_PCIE_MD_DATA                                 0xFFFF // Data

#define R_PCH_PCIE_SVCAP                                   0x90  // Subsystem Vendor Capability
#define S_PCH_PCIE_SVCAP                                   2
#define B_PCH_PCIE_SVCAP_NEXT                              0xFF00 // Next Capability
#define B_PCH_PCIE_SVCAP_CID                               0x00FF // Capability Identifier

#define R_PCH_PCIE_SVID                                    0x94  // Subsystem Vendor IDs
#define S_PCH_PCIE_SVID                                    4
#define B_PCH_PCIE_SVID_SID                                0xFFFF0000 // Subsystem Identifier
#define B_PCH_PCIE_SVID_SVID                               0x0000FFFF // Subsystem Vendor Identifier

#define R_PCH_PCIE_PMCAP_PMC                               0xA0  // Power Management Capability; PCI Power Management Capabilities
#define S_PCH_PCIE_PMCAP_PMC                               4
#define B_PCH_PCIE_PMCAP_PMC_PMES                          0xF8000000 // PME Support
#define B_PCH_PCIE_PMCAP_PMC_D2S                           BIT26 // D2 Support
#define B_PCH_PCIE_PMCAP_PMC_D1S                           BIT25 // D1 Support
#define B_PCH_PCIE_PMCAP_PMC_AC                            0x01C00000 // Aux Current
#define B_PCH_PCIE_PMCAP_PMC_DSI                           BIT21 // Device Specific Initialization
#define B_PCH_PCIE_PMCAP_PMC_PMEC                          BIT19 // PME Clock
#define B_PCH_PCIE_PMCAP_PMC_VS                            0x00070000 // Version
#define B_PCH_PCIE_PMCAP_PMC_NEXT                          0x0000FF00 // Next Capability
#define B_PCH_PCIE_PMCAP_PMC_CID                           0x000000FF // Capability Identifier

#define R_PCH_PCIE_PMCS                                    0xA4  // PCI Power Management Control and Status
#define S_PCH_PCIE_PMCS                                    4
#define B_PCH_PCIE_PMCS_BPCE                               BIT23 // Bus Power / Clock Control Enable
#define B_PCH_PCIE_PMCS_B23S                               BIT22 // B2 / B3 Support
#define B_PCH_PCIE_PMCS_PMES                               BIT15 // PME Status
#define B_PCH_PCIE_PMCS_PMEE                               BIT8  // PME Enable
#define B_PCH_PCIE_PMCS_NSR                                BIT3  // No Soft Reset
#define B_PCH_PCIE_PMCS_PS                                 (BIT1 | BIT0) // Power State
#define V_PCH_PCIE_PMCS_D0                                 0x00  // D0 State
#define V_PCH_PCIE_PMCS_D3H                                0x03  // D3 Hot State

#define R_PCH_PCIE_MANID                                   0xF8  // Manufacturer's ID
#define B_PCH_PCIE_MANID_DPID                              0x0F000000 // Dot Portion of Process ID
#define B_PCH_PCIE_MANID_SID                               0x00FF0000 // Stepping Identifier
#define B_PCH_PCIE_MANID_MID                               0x0000FF00 // Manufacturing Identifier
#define B_PCH_PCIE_MANID_PD                                0x000000FF // Process / Dot

#define R_PCH_PCIE_AECH                                    0x100 // Advanced Error Reporting Capability Header
#define B_PCH_PCIE_AECH_NCO                                0xFFF00000 // Next Capability Offset
#define B_PCH_PCIE_AECH_CV                                 0x000F0000 // Capability Version
#define B_PCH_PCIE_AECH_CID                                0x0000FFFF // Capability ID

#define R_PCH_PCIE_UES                                     0x104 // Uncorrectable Error Status
#define S_PCH_PCIE_UES                                     4
#define B_PCH_PCIE_UES_URE                                 BIT20 // Unsupported Request Error Status
#define B_PCH_PCIE_UES_EE                                  BIT19 // ECRC Error Status
#define B_PCH_PCIE_UES_MT                                  BIT18 // Malformed TLP Status
#define B_PCH_PCIE_UES_RO                                  BIT17 // Receiver Overflow Status
#define B_PCH_PCIE_UES_UC                                  BIT16 // Unexpected Completion Status
#define B_PCH_PCIE_UES_CA                                  BIT15 // Completer Abort Status
#define B_PCH_PCIE_UES_CT                                  BIT14 // Completion Timeout Status
#define B_PCH_PCIE_UES_FCPE                                BIT13 // Flow Control Protocol Error Status (Not Supported)
#define B_PCH_PCIE_UES_PT                                  BIT12 // Poisoned TLP Status
#define B_PCH_PCIE_UES_DLPE                                BIT4  // DataLink Protocol Error Status
#define B_PCH_PCIE_UES_TE                                  BIT0  // Training Error Status (Not Supported)

#define R_PCH_PCIE_UEM                                     0x108 // Uncorrectable Error Mask
#define S_PCH_PCIE_UEM                                     4
#define B_PCH_PCIE_UEM_URE                                 BIT20 // Unsupported Request Error Mask
#define B_PCH_PCIE_UEM_EE                                  BIT19 // ECRC Error Mask
#define B_PCH_PCIE_UEM_MT                                  BIT18 // Malformed TLP Mask
#define B_PCH_PCIE_UEM_RO                                  BIT17 // Receiver Overflow Mask
#define B_PCH_PCIE_UEM_UC                                  BIT16 // Unexpected Completion Mask
#define B_PCH_PCIE_UEM_CA                                  BIT15 // Completer Abort Mask
#define B_PCH_PCIE_UEM_CT                                  BIT14 // Completion Timeout Mask
#define B_PCH_PCIE_UEM_FCPE                                BIT13 // Flow Control Protocol Error Mask (Not Supported)
#define B_PCH_PCIE_UEM_PT                                  BIT12 // Poisoned TLP Mask
#define B_PCH_PCIE_UEM_DLPE                                BIT4  // DataLink Protocol Error Mask
#define B_PCH_PCIE_UEM_TE                                  BIT0  // Training Error Mask (Not Supported)

#define R_PCH_PCIE_UEV                                     0x10C // Uncorrectable Error Severity
#define S_PCH_PCIE_UEV                                     4
#define B_PCH_PCIE_UEV_URE                                 BIT20 // Unsupported Request Error Severity
#define B_PCH_PCIE_UEV_EE                                  BIT19 // ECRC Error Severity
#define B_PCH_PCIE_UEV_MT                                  BIT18 // Malformed TLP Severity
#define B_PCH_PCIE_UEV_RO                                  BIT17 // Receiver Overflow Severity
#define B_PCH_PCIE_UEV_UC                                  BIT16 // Unexpected Completion Severity
#define B_PCH_PCIE_UEV_CA                                  BIT15 // Completion Abort Severity
#define B_PCH_PCIE_UEV_CT                                  BIT14 // Completion Timeout Severity
#define B_PCH_PCIE_UEV_FCPE                                BIT13 // Flow Control Protocol Error Severity
#define B_PCH_PCIE_UEV_PT                                  BIT12 // Poisoned TLP Severity
#define B_PCH_PCIE_UEV_DLPE                                BIT4  // Data Link Protocol Error Severity
#define B_PCH_PCIE_UEV_TE                                  BIT0  // Training Error Severity (Not Supported)

#define R_PCH_PCIE_CES                                     0x110 // Correctable Error Status
#define S_PCH_PCIE_CES                                     4
#define B_PCH_PCIE_CES_ANFES                               BIT13 // Advisory Non-Fatal Error Status
#define B_PCH_PCIE_CES_RTT                                 BIT12 // Replay Timer Timeout Status
#define B_PCH_PCIE_CES_RNR                                 BIT8  // Replay Number Rollover Status
#define B_PCH_PCIE_CES_BD                                  BIT7  // Bad DLLP Status
#define B_PCH_PCIE_CES_BT                                  BIT6  // Bad TLP Status
#define B_PCH_PCIE_CES_RE                                  BIT0  // Receiver Error Status

#define R_PCH_PCIE_CEM                                     0x114 // Correctable Error Mask
#define S_PCH_PCIE_CEM                                     4
#define B_PCH_PCIE_CEM_ANFEM                               BIT13 // Advisory Non-Fatal Error Mask
#define B_PCH_PCIE_CEM_RTT                                 BIT12 // Replay Timer Timeout Mask
#define B_PCH_PCIE_CEM_RNR                                 BIT8  // Replay Number Rollover Mask
#define B_PCH_PCIE_CEM_BD                                  BIT7  // Bad DLLP Mask
#define B_PCH_PCIE_CEM_BT                                  BIT6  // Bad TLP Mask
#define B_PCH_PCIE_CEM_RE                                  BIT0  // Receiver Error Mask

#define R_PCH_PCIE_AECC                                    0x118 // Advanced Error Capabilities and Control
#define S_PCH_PCIE_AECC                                    4
#define B_PCH_PCIE_AECC_ECE                                BIT8  // ECRC Check Enable
#define B_PCH_PCIE_AECC_ECC                                BIT7  // ECRC Check Capable
#define B_PCH_PCIE_AECC_EGE                                BIT6  // ECRC Generation Enable
#define B_PCH_PCIE_AECC_EGC                                BIT5  // ECRC Generation Capable
#define B_PCH_PCIE_AECC_FEP                                0x0000001F // First Error Pointer

#define R_PCH_PCIE_HL_DW1                                  0x11C // Header Log DW1

#define R_PCH_PCIE_HL_DW2                                  0x120 // Header Log DW2

#define R_PCH_PCIE_HL_DW3                                  0x124 // Header Log DW3

#define R_PCH_PCIE_HL_DW4                                  0x128 // Header Log DW4

#define R_PCH_PCIE_REC                                     0x12C // Root Error Command
#define B_PCH_PCIE_REC_FERE                                BIT2  // Fatal Error Report Enable
#define B_PCH_PCIE_REC_NERE                                BIT1  // Non-Fatal Error Report Enable
#define B_PCH_PCIE_REC_CERE                                BIT0  // Correctable Error Report Enable

#define R_PCH_PCIE_RES                                     0x130 // Root Error Status
#define S_PCH_PCIE_RES                                     4
#define B_PCH_PCIE_RES_AEMN                                0xF8000000 // Advanced Error Interrupt Message Number
#define B_PCH_PCIE_RES_FEMR                                BIT6  // Fatal Error Messages Received
#define B_PCH_PCIE_RES_NFEMR                               BIT5  // Non-Fatal Error Messages Received
#define B_PCH_PCIE_RES_FUF                                 BIT4  // First Uncorrectable Fatal
#define B_PCH_PCIE_RES_MENR                                BIT3  // Multiple ERR_FATAL / NONFATAL Received
#define B_PCH_PCIE_RES_ENR                                 BIT2  // ERR_FATAL / NONFATAL Received
#define B_PCH_PCIE_RES_MCR                                 BIT1  // Multiple ERR_COR Received
#define B_PCH_PCIE_RES_CR                                  BIT0  // ERR_COR Received

#define R_PCH_PCIE_ESID                                    0x134 // Error Source Identification
#define B_PCH_PCIE_ESID_EFNFSID                            0xFFFF0000 // ERR_FATAL / NONFATAL Source Identification
#define B_PCH_PCIE_ESID_ECSID                              0x0000FFFF // ERR_COR Source Identification

#define R_PCH_PCIE_L1SECH                                  0x200 // L1 Sub-States Extended Capability Header
#define B_PCH_PCIE_L1SECH_NCO                              0xFFF00000
#define B_PCH_PCIE_L1SECH_CV                               (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_PCIE_L1SECH_PCIEEC                           0xFFFF

#define R_PCH_PCIE_L1SCAP                                  0x204 // L1 Sub-States Capabilities
#define B_PCH_PCIE_L1SCAP_PTV                              0xF00000
#define B_PCH_PCIE_L1SCAP_PTPOS                            (BIT17 | BIT16)
#define B_PCH_PCIE_L1SCAP_PCMRT                            0xFF00
#define B_PCH_PCIE_L1SCAP_LFASL1SS                         BIT5  // LFAS L1 Substates Supported
#define B_PCH_PCIE_L1SCAP_CKRQL1SS                         BIT4  // CLKREQ# L1 Substates Supported
#define B_PCH_PCIE_L1SCAP_AL1SS                            BIT3  // ASPM L1 Substates Supported
#define B_PCH_PCIE_L1SCAP_PL1SS                            BIT2  // PCI-PM L1 Substates Supported
#define B_PCH_PCIE_L1SCAP_L1SS                             BIT1  // L1.SNOOZ Sub-State Supported
#define B_PCH_PCIE_L1SCAP_L1OS                             BIT0  // L1.OFF Power Management Supported

#define R_PCH_PCIE_L1SCTL1                                 0x208 // L1 Sub-States Control 1
#define B_PCH_PCIE_L1SCTL1_L1OFFLTRTLSV                    (BIT31 | BIT30 | BIT29) // L1.OFF LTR Threshold Latency Scale Value
#define B_PCH_PCIE_L1SCTL1_L1OFFLTRTLV                     0x3FF0000 // L1.OFF LTR Threshold Latency Value
#define B_PCH_PCIE_L1SCTL1_CMRT                            0xFF00 // Tcommon_mode time
#define B_PCH_PCIE_L1SCTL1_SLSM                            BIT4
#define B_PCH_PCIE_L1SCTL1_AL1SE                           BIT3  // ASPM L1 Substates Enable
#define B_PCH_PCIE_L1SCTL1_PPL1SE                          BIT2  // PCI-PM L1 Substates Enable
#define B_PCH_PCIE_L1SCTL1_L1SE                            BIT1  // L1.SNOOZ Power Management Enable
#define B_PCH_PCIE_L1SCTL1_L1OE                            BIT0  // L1.OFF Power Management Enable

#define R_PCH_PCIE_L1SCTL2                                 0x20C // L1 Sub-States Control 2
#define B_PCH_PCIE_L1SCTL2_POWT                            0xF8
#define B_PCH_PCIE_L1SCTL2_TPOS                            (BIT1 | BIT0)
#define V_PCH_PCIE_L1SCTL2_TPOS_2US                        0
#define V_PCH_PCIE_L1SCTL2_TPOS_10US                       BIT0
#define V_PCH_PCIE_L1SCTL2_TPOS_100US                      BIT1

#define R_PCH_PCIE_PCIESTS1                                0x328 // PCI Express Status 1
#define B_PCH_PCIE_PCIESTS1_LTSMSTATE                      0xFF000000 // LTSM State
#define B_PCH_PCIE_PCIESTS1_LNKSTAT                        0x00780000 // Link Status
#define B_PCH_PCIE_PCIESTS1_REPLAYNUM                      (BIT18 | BIT17) // Replay Number
#define B_PCH_PCIE_PCIESTS1_DLLRETRY                       BIT16 // Data Link Layer Retry
#define B_PCH_PCIE_PCIESTS1_LANESTAT                       0x0000F000 // Lane Status
#define B_PCH_PCIE_PCIESTS1_NXTTXSEQNUM                    0x00000FFF // Next Transmitted Sequence Number

#define R_PCH_PCIE_PCIESTS2                                0x32C // PCI Express Status 2
#define B_PCH_PCIE_PCIESTS2_P48PNCCWSSCMES                 BIT31 // PCIe Port 4/8 Non-Common Clock With SSC Mode Enable Strap
#define B_PCH_PCIE_PCIESTS2_P37PNCCWSSCMES                 BIT30 // PCIe Port 3/7 Non-Common Clock With SSC Mode Enable Strap
#define B_PCH_PCIE_PCIESTS2_P26PNCCWSSCMES                 BIT29 // PCIe Port 2/6 Non-Common Clock With SSC Mode Enable Strap
#define B_PCH_PCIE_PCIESTS2_P15PNCCWSSCMES                 BIT28 // PCIe Port 1/5 Non-Common Clock With SSC Mode Enable Strap
#define B_PCH_PCIE_PCIESTS2_NXTRCVSEQ                      0x0FFF0000 // Next Receive Sequence Number
#define B_PCH_PCIE_PCIESTS2_LASTACKSEQNUM                  0x00000FFF // Last Acknowledged Sequence Number

#define R_PCH_PCIE_PCIECMMPC                               0x330 // PCI Express Compliance Measurement Mode (CMM) Port Control
#define S_PCH_PCIE_PCIECMMPC                               4
#define B_PCH_PCIE_PCIECMMPC_SYM3SEL                       BIT29 // CMM Symbol [3] Select
#define B_PCH_PCIE_PCIECMMPC_SYM2SEL                       BIT28 // CMM Symbol [2] Select
#define B_PCH_PCIE_PCIECMMPC_SYM1SEL                       BIT27 // CMM Symbol [1] Select
#define B_PCH_PCIE_PCIECMMPC_SYM0SEL                       BIT26 // CMM Symbol [0] Select
#define B_PCH_PCIE_PCIECMMPC_ERRLANENUM                    (BIT23 | BIT22) // CMM Error Lane Number
#define B_PCH_PCIE_PCIECMMPC_INVERT                        (BIT15 | BIT14 | BIT13) // CMM Invert
#define B_PCH_PCIE_PCIECMMPC_SYMERRNUMINV                  (BIT12 | BIT11 | BIT10) // CMM Symbol Error Number Invert
#define B_PCH_PCIE_PCIECMMPC_SYMERRNUM                     (BIT9 | BIT8) // CMM Symbol Error Number
#define B_PCH_PCIE_PCIECMMPC_ERRDET                        BIT7  // CMM Error Detected
#define B_PCH_PCIE_PCIECMMPC_SLNINVCMM                     (BIT6 | BIT5) // Select Lane Number to be Inverted for CMM
#define B_PCH_PCIE_PCIECMMPC_AUTOINVERT                    BIT4  // CMM AutoInvert
#define B_PCH_PCIE_PCIECMMPC_STAT                          BIT3  // CMM Status
#define B_PCH_PCIE_PCIECMMPC_INVEN                         BIT2  // CMM Invert Enable
#define B_PCH_PCIE_PCIECMMPC_START                         BIT0  // CMM Start

#define R_PCH_PCIE_PCIECMMSB                               0x334 // PCI Express Compliance Measurement Mode Symbol Buffer
#define B_PCH_PCIE_PCIECMMSB_DATA3                         0xFF000000 // CMM Data [3]
#define B_PCH_PCIE_PCIECMMSB_DATA2                         0x00FF0000 // CMM Data [2]
#define B_PCH_PCIE_PCIECMMSB_DATA1                         0x0000FF00 // CMM Data [1]
#define B_PCH_PCIE_PCIECMMSB_DATA0                         0x000000FF // CMM Data [0]

#define B_PCH_CFIO_PAD_PCIE_CLKREQB(X)                     ((X) < (PCH_PCIE_MAX_ROOT_PORTS/2)?(0x5C00 + (X * 0x8)):(0x5C08 + (0x10 * (X - 1))))


#endif
