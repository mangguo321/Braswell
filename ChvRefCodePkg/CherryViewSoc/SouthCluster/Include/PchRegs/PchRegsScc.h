/** @file
  Register names for SCC module.

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

#ifndef _PCH_REGS_SCC_H_
#define _PCH_REGS_SCC_H_

//
// SCC Modules Registers
//

//
// SCC SDIO Modules
// PCI Config Space Registers
//
#define PCI_DEVICE_NUMBER_PCH_SCC_SDIO_0                   16
#define PCI_DEVICE_NUMBER_PCH_SCC_SDIO_1                   17
#define PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2                   18
#define PCI_FUNCTION_NUMBER_PCH_SCC_SDIO                   0

#define R_PCH_SCC_SDIO_DEVVENDID                           0x00  // Device ID & Vendor ID
#define B_PCH_SCC_SDIO_DEVVENDID_DID                       0xFFFF0000 // Device ID
#define B_PCH_SCC_SDIO_DEVVENDID_VID                       0x0000FFFF // Vendor ID

#define R_PCH_SCC_SDIO_STSCMD                              0x04  // Status & Command
#define B_PCH_SCC_SDIO_STSCMD_RMA                          BIT29 // RMA
#define B_PCH_SCC_SDIO_STSCMD_RCA                          BIT28 // RCA
#define B_PCH_SCC_SDIO_STSCMD_CAPLIST                      BIT20 // Capability List
#define B_PCH_SCC_SDIO_STSCMD_INTRSTS                      BIT19 // Interrupt Status
#define B_PCH_SCC_SDIO_STSCMD_INTRDIS                      BIT10 // Interrupt Disable
#define B_PCH_SCC_SDIO_STSCMD_SERREN                       BIT8  // SERR# Enable
#define B_PCH_SCC_SDIO_STSCMD_BME                          BIT2  // Bus Master Enable
#define B_PCH_SCC_SDIO_STSCMD_MSE                          BIT1  // Memory Space Enable

#define R_PCH_SCC_SDIO_REVCC                               0x08  // Revision ID & Class Code
#define B_PCH_SCC_SDIO_REVCC_CC                            0xFFFFFF00 // Class Code
#define B_PCH_SCC_SDIO_REVCC_RID                           0x000000FF // Revision ID

#define R_PCH_SCC_SDIO_CLHB                                0x0C
#define B_PCH_SCC_SDIO_CLHB_MULFNDEV                       BIT23 // Multi Function Device
#define B_PCH_SCC_SDIO_CLHB_HT                             0x007F0000 // Header Type
#define B_PCH_SCC_SDIO_CLHB_LT                             0x0000FF00 // Latency Timer
#define B_PCH_SCC_SDIO_CLHB_CLS                            0x000000FF // Cache Line Size

#define R_PCH_SCC_SDIO_BAR                                 0x10  // BAR
#define B_PCH_SCC_SDIO_BAR_BA                              0xFFFFF000 // Base Address
#define V_PCH_SCC_SDIO_BAR_SIZE                            0x1000
#define N_PCH_SCC_SDIO_BAR_ALIGNMENT                       12
#define B_PCH_SCC_SDIO_BAR_SI                              0x00000FF0 // Size Indicator
#define B_PCH_SCC_SDIO_BAR_PF                              BIT3  // Prefetchable
#define B_PCH_SCC_SDIO_BAR_TYPE                            (BIT2 | BIT1) // Type
#define B_PCH_SCC_SDIO_BAR_MS                              BIT0  // Message Space

#define R_PCH_SCC_SDIO_BAR_HIGH                            0x14  // BAR High
#define B_PCH_SCC_SDIO_BAR_HIGH_BA                         0xFFFFFFFF // Base Address

#define R_PCH_SCC_SDIO_BAR1                                0x18  // BAR 1
#define B_PCH_SCC_SDIO_BAR1_BA                             0xFFFFF000 // Base Address
#define V_PCH_SCC_SDIO_BAR1_SIZE                           0x1000
#define B_PCH_SCC_SDIO_BAR1_SI                             0x00000FF0 // Size Indicator
#define B_PCH_SCC_SDIO_BAR1_PF                             BIT3  // Prefetchable
#define B_PCH_SCC_SDIO_BAR1_TYPE                           (BIT2 | BIT1) // Type
#define B_PCH_SCC_SDIO_BAR1_MS                             BIT0  // Message Space

#define R_PCH_SCC_SDIO_BAR1_HIGH                           0x1C  // BAR 1 High
#define B_PCH_SCC_SDIO_BAR1_HIGH_BA                        0xFFFFFFFF // Base Address

#define R_PCH_SCC_SDIO_SSID                                0x2C  // Sub System ID
#define B_PCH_SCC_SDIO_SSID_SID                            0xFFFF0000 // Sub System ID
#define B_PCH_SCC_SDIO_SSID_SVID                           0x0000FFFF // Sub System Vendor ID

#define R_PCH_SCC_SDIO_ERBAR                               0x30  // Expansion ROM BAR
#define B_PCH_SCC_SDIO_ERBAR_BA                            0xFFFFFFFF // Expansion ROM Base Address

#define R_PCH_SCC_SDIO_CAPPTR                              0x34  // Capability Pointer
#define B_PCH_SCC_SDIO_CAPPTR_CPPWR                        0xFF  // Capability Pointer Power

#define R_PCH_SCC_SDIO_INTR                                0x3C  // Interrupt
#define B_PCH_SCC_SDIO_INTR_ML                             0xFF000000 // Max Latency
#define B_PCH_SCC_SDIO_INTR_MG                             0x00FF0000
#define B_PCH_SCC_SDIO_INTR_IP                             0x00000F00 // Interrupt Pin
#define B_PCH_SCC_SDIO_INTR_IL                             0x000000FF // Interrupt Line

#define R_PCH_SCC_SDIO_PCAPID                              0x80  // Power Capability ID
#define B_PCH_SCC_SDIO_PCAPID_PS                           0xF8000000 // PME Support
#define B_PCH_SCC_SDIO_PCAPID_VS                           0x00070000 // Version
#define B_PCH_SCC_SDIO_PCAPID_NC                           0x0000FF00 // Next Capability
#define B_PCH_SCC_SDIO_PCAPID_PC                           0x000000FF // Power Capability

#define R_PCH_SCC_SDIO_PCS                                 0x84  // PME Control Status
#define B_PCH_SCC_SDIO_PCS_PMESTS                          BIT15 // PME Status
#define B_PCH_SCC_SDIO_PCS_PMEEN                           BIT8  // PME Enable
#define B_PCH_SCC_SDIO_PCS_NSS                             BIT3  // No Soft Reset
#define B_PCH_SCC_SDIO_PCS_PS                              (BIT1 | BIT0) // Power State

#define R_PCH_SCC_SDIO_GEN_REGRW1                          0xA0  // General Purpose Read Write Register 1

#define R_PCH_SCC_SDIO_GEN_REGRW2                          0xA4  // General Purpose Read Write Register 2

#define R_PCH_SCC_SDIO_GEN_REGRW3                          0xA8  // General Purpose Read Write Register 3

#define R_PCH_SCC_SDIO_GEN_REGRW4                          0xAC  // General Purpose Read Write Register 4

#define R_PCH_SCC_SDIO_MANID                               0xF8  // Manufacturer ID
#define B_PCH_SCC_SDIO_MANID_MANID                         0xFFFFFFFF // Manufacturer ID

//
// SCC SDIO Module
// MMIO Space Register
//
#define R_PCH_SCC_SDIO_MEM_SYS_ADR                         0x00  // SDMA System Address

#define R_PCH_SCC_SDIO_MEM_BLK_SIZE                        0x04  // Block Size

#define R_PCH_SCC_SDIO_MEM_BLK_COUNT                       0x06  // Block Count

#define R_PCH_SCC_SDIO_MEM_ARGUMENT                        0x08  // Argument

#define R_PCH_SCC_SDIO_MEM_TX_MODE                         0x0C  // Transfer Mode
#define B_PCH_SCC_SDIO_MEM_TX_MODE_BLK_SEL                 BIT5  // Multi / Single Block Select, 1: Multiple Block; 0: Single Block
#define B_PCH_SCC_SDIO_MEM_TX_MODE_DATA_TR_DIR             BIT4  // Data Transfer Direction Select, 1: Read (Card to Host); 0: Write (Host to Card)
#define B_PCH_SCC_SDIO_MEM_TX_MODE_AUTO_CMD_EN             (BIT3 | BIT2) // Auto Command Function Enable
#define B_PCH_SCC_SDIO_MEM_TX_MODE_BLK_COUNT_EN            BIT1  // Block Count Enable
#define B_PCH_SCC_SDIO_MEM_TX_MODE_DMA_EN                  BIT0  // DMA Enable

#define R_PCH_SCC_SDIO_MEM_CMD                             0x0E  // Command
#define B_PCH_SCC_SDIO_MEM_CMD_DATA_PR_SEL                 BIT5  // Data Present Select
#define B_PCH_SCC_SDIO_MEM_CMD_CMD_INDEX_CHK_EN            BIT4  // Command Index Check Enable
#define B_PCH_SCC_SDIO_MEM_CMD_CMD_CRC_CHK_EN              BIT3  // Command CRC Check Enable
#define B_PCH_SCC_SDIO_MEM_CMD_RTS_MASK                    (BIT1 | BIT0) // Response Type Select
#define V_PCH_SCC_SDIO_MEM_CMD_RTS_NO_RESP                 0
#define V_PCH_SCC_SDIO_MEM_CMD_RTS_RESP136                 1
#define V_PCH_SCC_SDIO_MEM_CMD_RTS_RESP48                  2
#define V_PCH_SCC_SDIO_MEM_CMD_RTS_RESP48_CHK              3

#define R_PCH_SCC_SDIO_MEM_RESPONSE0                       0x10  // Response 0

#define R_PCH_SCC_SDIO_MEM_BUF_DATA_PORT                   0x20  // Buffer Data Port

#define R_PCH_SCC_SDIO_MEM_PRE_STATE                       0x24  // Present State
#define B_PCH_SCC_SDIO_MEM_PRE_STATE_DLSL0                 BIT20 // DAT[0] Line Signal Level

#define R_PCH_SCC_SDIO_MEM_HOST_CTL                        0x28  // Host Control
#define B_PCH_SCC_SDIO_MEM_HOST_CTL_CDSS                   BIT7  // Card Detect Signal Selection
#define B_PCH_SCC_SDIO_MEM_HOST_CTL_CDTL                   BIT6  // Card Detect Test Level

#define R_PCH_SCC_SDIO_MEM_PWR_CTL                         0x29  // Power Control
#define B_PCH_SCC_SDIO_MEM_PWR_CTL_SD_VOLSEL               (BIT3 | BIT2 | BIT1) // SD Bus Voltage Select
#define V_PCH_SCC_SDIO_MEM_PWR_CTL_SD_VOLSEL_3P3           (BIT3 | BIT2 | BIT1) // 3.3V
#define V_PCH_SCC_SDIO_MEM_PWR_CTL_SD_VOLSEL_1P8           (BIT3 | BIT1) // 1.8V
#define B_PCH_SCC_SDIO_MEM_PWR_CTL_SD_PWR                  BIT0  // SD Bus Power

#define R_PCH_SCC_SDIO_MEM_CLK_CTL                         0x2C  // Clock Control

#define R_PCH_SCC_SDIO_MEM_TIMEOUT_CTL                     0x2E  // Timeout Control
#define B_PCH_SCC_SDIO_MEM_TIMEOUT_CTL_DTCV                0x0F  // Data Timeout Counter Value

#define R_PCH_SCC_SDIO_MEM_SW_RST                          0x2F  // Software Reset
#define B_PCH_SCC_SDIO_MEM_SW_RST_SW_RST_DAT_LN            BIT2  // Software Reset For DAT Line
#define B_PCH_SCC_SDIO_MEM_SW_RST_SW_RST_CMD_LN            BIT1  // Software Reset For CMD Line
#define B_PCH_SCC_SDIO_MEM_SW_RST_SW_RST_ALL               BIT0  // Software Reset For All

#define R_PCH_SCC_SDIO_MEM_NINTSTS                         0x30  // Normal Interrupt Status
#define B_PCH_SCC_SDIO_MEM_NINTSTS_MASK                    0xFFFF
#define B_PCH_SCC_SDIO_MEM_NINTSTS_CLEAR_MASK              0x60FF
#define B_PCH_SCC_SDIO_MEM_NINTSTS_BUF_RD_RDY              BIT5  // Buffer Read Ready
#define B_PCH_SCC_SDIO_MEM_NINTSTS_DMA_INT                 BIT3  // DMA Interrupt
#define B_PCH_SCC_SDIO_MEM_NINTSTS_TX_COMP                 BIT1  // Transfer Complete
#define B_PCH_SCC_SDIO_MEM_NINTSTS_CMD_COMP                BIT0  // Command Complete

#define R_PCH_SCC_SDIO_MEM_ERINTSTS                        0x32  // Error Interrupt Status
#define B_PCH_SCC_SDIO_MEM_ERINTSTS_MASK                   0x13FF
#define B_PCH_SCC_SDIO_MEM_ERINTSTS_CLEAR_MASK             0x13FF

#define R_PCH_SCC_SDIO_MEM_NINTEN                          0x34  // Normal Interrupt Status Enable
#define B_PCH_SCC_SDIO_MEM_NINTEN_MASK                     0x7FFF

#define R_PCH_SCC_SDIO_MEM_ERINTEN                         0x36  // Error Interrupt Status Enable
#define B_PCH_SCC_SDIO_MEM_ERINTEN_MASK                    0x13FF

#define R_PCH_SCC_SDIO_MEM_NINTSIGNEN                      0x38  // Normal Interrupt Signal Enable
#define B_PCH_SCC_SDIO_MEM_NINTSIGNEN_MASK                 0x7FFF

#define R_PCH_SCC_SDIO_MEM_ERINTSIGNEN                     0x3A  // Error Interrupt Signal Enable
#define B_PCH_SCC_SDIO_MEM_ERINTSIGNEN_MASK                0x13FF

#define R_PCH_SCC_SDIO_MEM_CESHC2                          0x3C  // Auto CMD12 Error Status Register & Host Control 2
#define B_PCH_SCC_SDIO_MEM_CESHC2_ASYNC_INT                BIT30 // Asynchronous Interrupt Enable
#define B_PCH_SCC_SDIO_MEM_CESHC2_MODE_MASK                (BIT18 | BIT17 | BIT16)
#define V_PCH_SCC_SDIO_MEM_CESHC2_MODE_HS400               (5 << 16)
#define V_PCH_SCC_SDIO_MEM_CESHC2_MODE_SDR104              (3 << 16)

#define R_PCH_SCC_SDIO_MEM_CAP                             0x40  // Capabilities
#define B_PCH_SCC_SDIO_MEM_CAP_BASE_CLK_FREQ               0xFF00 // Base Clock Frequency

#define R_PCH_SCC_SDIO_MEM_CAP2                            0x44  // Capabilities 2
#define B_PCH_SCC_SDIO_MEM_CAP2_HS400_SUPPORT              BIT31
#define B_PCH_SCC_SDIO_MEM_CAP2_SDR104_SUPPORT             BIT1
///
/// SCC Private Space
///
#define PCH_SCC_EP_PORT_ID                                 0x63  // SCC EP Private Space PortID
#define PCH_SCC_EP_PRIVATE_READ_OPCODE                     0x06  // CUnit to SCC EP Private Space Read Opcode
#define PCH_SCC_EP_PRIVATE_WRITE_OPCODE                    0x07  // CUnit to SCC EP Private Space Write Opcode

#define R_PCH_SCC_EP_PCICFGCTR1                            0x500 // PCI Configuration Control 1 - eMMC
#define B_PCH_SCC_EP_PCICFGCTR1_ACPI_INT_EN1               BIT1  // ACPI Interrupt Enable
#define B_PCH_SCC_EP_PCICFGCTR1_PCI_CFG_DIS1               BIT0  // PCI Configuration Space Disable

#define R_PCH_SCC_EP_PCICFGCTR2                            0x504 // PCI Configuration Control 2 - SD Card
#define B_PCH_SCC_EP_PCICFGCTR2_ACPI_INT_EN1               BIT1  // ACPI Interrupt Enable
#define B_PCH_SCC_EP_PCICFGCTR2_PCI_CFG_DIS1               BIT0  // PCI Configuration Space Disable

#define R_PCH_SCC_EP_PCICFGCTR3                            0x508 // PCI Configuration Control 3 - SDIO
#define B_PCH_SCC_EP_PCICFGCTR3_ACPI_INT_EN1               BIT1  // ACPI Interrupt Enable
#define B_PCH_SCC_EP_PCICFGCTR3_PCI_CFG_DIS1               BIT0  // PCI Configuration Space Disable

#endif
