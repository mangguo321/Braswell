/** @file
  Register names for PCH SPI device.

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

#ifndef _PCH_REGS_SPI_H_
#define _PCH_REGS_SPI_H_

//
// SPI Host Interface Registers
//
#define R_PCH_SPI_BFPR                       0x00  // BIOS Flash Primary Region Register (32bits)
#define B_PCH_SPI_BFPR_PRL                   0x7FFF0000 // BIOS Flash Primary Region Limit
#define B_PCH_SPI_BFPR_PRB                   0x7FFF // BIOS Flash Primary Region Base

#define R_PCH_SPI_HSFS                       0x04  // Hardware Sequencing Flash Status Register (16bits)
#define B_PCH_SPI_HSFS_FLOCKDN               BIT15 // Flash Configuration Lock-Down
#define B_PCH_SPI_HSFS_FDV                   BIT14 // Flash Descriptor Valid
#define B_PCH_SPI_HSFS_FDOPSS                BIT13 // Flash Descriptor Override Pin-Strap Status
#define B_PCH_SPI_HSFS_SCIP                  BIT5  // SPI Cycle in Progress
#define B_PCH_SPI_HSFS_BERASE_MASK           (BIT4 | BIT3) // Block / Sector Erase Size
#define V_PCH_SPI_HSFS_BERASE_256B           0x00  // Block/Sector = 256 Bytes
#define V_PCH_SPI_HSFS_BERASE_4K             0x01  // Block/Sector = 4K Bytes
#define V_PCH_SPI_HSFS_BERASE_8K             0x10  // Block/Sector = 8K Bytes
#define V_PCH_SPI_HSFS_BERASE_64K            0x11  // Block/Sector = 64K Bytes
#define B_PCH_SPI_HSFS_AEL                   BIT2  // Access Error Log
#define B_PCH_SPI_HSFS_FCERR                 BIT1  // Flash Cycle Error
#define B_PCH_SPI_HSFS_FDONE                 BIT0  // Flash Cycle Done

#define R_PCH_SPI_HSFC                       0x06  // Hardware Sequencing Flash Control Register (16bits)
#define B_PCH_SPI_HSFC_FSMIE                 BIT15 // Flash SPI SMI# Enable
#define B_PCH_SPI_HSFC_FDBC_MASK             0x3F00 // Flash Data Byte Count ( <= 64), Count = (Value in this field) + 1.
#define B_PCH_SPI_HSFC_FCYCLE_MASK           0x0006 // Flash Cycle.
#define V_PCH_SPI_HSFC_FCYCLE_READ           0     // Flash Cycle Read
#define V_PCH_SPI_HSFC_FCYCLE_WRITE          2     // Flash Cycle Write
#define V_PCH_SPI_HSFC_FCYCLE_ERASE          3     // Flash Cycle Block Erase
#define B_PCH_SPI_HSFC_FCYCLE_FGO            BIT0  // Flash Cycle Go.

#define R_PCH_SPI_FADDR                      0x08  // SPI Flash Address
#define B_PCH_SPI_FADDR_MASK                 0x07FFFFFF // SPI Flash Address Mask (0~26bit)

#define R_PCH_SPI_FDATA00                    0x10  // SPI Data 00 (32 bits)
#define R_PCH_SPI_FDATA01                    0x14  // SPI Data 01
#define R_PCH_SPI_FDATA02                    0x18  // SPI Data 02
#define R_PCH_SPI_FDATA03                    0x1C  // SPI Data 03
#define R_PCH_SPI_FDATA04                    0x20  // SPI Data 04
#define R_PCH_SPI_FDATA05                    0x24  // SPI Data 05
#define R_PCH_SPI_FDATA06                    0x28  // SPI Data 06
#define R_PCH_SPI_FDATA07                    0x2C  // SPI Data 07
#define R_PCH_SPI_FDATA08                    0x30  // SPI Data 08
#define R_PCH_SPI_FDATA09                    0x34  // SPI Data 09
#define R_PCH_SPI_FDATA10                    0x38  // SPI Data 10
#define R_PCH_SPI_FDATA11                    0x3C  // SPI Data 11
#define R_PCH_SPI_FDATA12                    0x40  // SPI Data 12
#define R_PCH_SPI_FDATA13                    0x44  // SPI Data 13
#define R_PCH_SPI_FDATA14                    0x48  // SPI Data 14
#define R_PCH_SPI_FDATA15                    0x4C  // SPI Data 15

#define R_PCH_SPI_FRAP                       0x50  // SPI Flash Regions Access Permissions Register
#define B_PCH_SPI_FRAP_BMWAG_MASK            0xFF000000 // Master Write Access Grant MASK
#define B_PCH_SPI_FRAP_BMWAG_SEC             BIT26 // Master Write Access Grant for SEC
#define B_PCH_SPI_FRAP_BMWAG_BIOS            BIT25 // Master Write Access Grant for Host CPU/BIOS
#define B_PCH_SPI_FRAP_BMRAG_MASK            0x00FF0000 // Master Read Access Grant Grant MASK
#define B_PCH_SPI_FRAP_BMRAG_SEC             BIT18 // Master Read Access Grant for SEC
#define B_PCH_SPI_FRAP_BMRAG_BIOS            BIT17 // Master Read Access Grant for Host CPU/BIOS
#define B_PCH_SPI_FRAP_BRWA_MASK             0x0000FF00 // BIOS Region Write Access MASK
#define B_PCH_SPI_FRAP_BRWA_SEC              BIT10 // Region Write Access for Region2 SEC
#define B_PCH_SPI_FRAP_BRWA_BIOS             BIT9  // Region Write Access for Region1 BIOS
#define B_PCH_SPI_FRAP_BRWA_FLASHD           BIT8  // Region Write Access for Region0 Flash Descriptor
#define B_PCH_SPI_FRAP_BRRA_MASK             0x000000FF // BIOS Region Read Access MASK
#define B_PCH_SPI_FRAP_BRRA_SEC              BIT2  // Region Read Access for Region2 SEC
#define B_PCH_SPI_FRAP_BRRA_BIOS             BIT1  // Region Read Access for Region1 BIOS
#define B_PCH_SPI_FRAP_BRRA_FLASHD           BIT0  // Region Read Access for Region0 Flash Descriptor

#define R_PCH_SPI_FREG0_FLASHD               0x54  // Flash Region 0 (Flash Descriptor) (32bits)
#define B_PCH_SPI_FREG0_LIMIT_MASK           0x7FFF0000 // Size, [30:16] here represents limit[26:12]
#define B_PCH_SPI_FREG0_BASE_MASK            0x00007FFF // Base, [14:0]  here represents base [26:12]

#define R_PCH_SPI_FREG1_BIOS                 0x58  // Flash Region 1 (BIOS) (32bits)
#define B_PCH_SPI_FREG1_LIMIT_MASK           0x7FFF0000 // Size, [30:16] here represents limit[26:12]
#define B_PCH_SPI_FREG1_BASE_MASK            0x00007FFF // Base, [14:0]  here represents base [26:12]

#define R_PCH_SPI_FREG2_SEC                  0x5C  // Flash Region 2 (SEC) (32bits)
#define B_PCH_SPI_FREG2_LIMIT_MASK           0x7FFF0000 // Size, [30:16] here represents limit[26:12]
#define B_PCH_SPI_FREG2_BASE_MASK            0x00007FFF // Base, [14:0]  here represents base [26:12]

#define R_PCH_SPI_FREG4_PLATFORM_DATA        0x64  // Flash Region 4 (Platform Data) (32bits)
#define B_PCH_SPI_FREG4_LIMIT_MASK           0x7FFF0000 // Size, [30:16] here represents limit[26:12]
#define B_PCH_SPI_FREG4_BASE_MASK            0x00007FFF // Base, [14:0]  here represents base [26:12]

#define R_PCH_SPI_PR0                        0x74  // Protected Region 0 Register
#define B_PCH_SPI_PR0_WPE                    BIT31 // Write Protection Enable
#define B_PCH_SPI_PR0_PRL_MASK               0x7FFF0000 // Protected Range Limit Mask, [30:16] here represents upper limit of address [26:12]
#define B_PCH_SPI_PR0_RPE                    BIT15 // Read Protection Enable
#define B_PCH_SPI_PR0_PRB_MASK               0x00007FFF // Protected Range Base Mask, [14:0] here represents base limit of address [26:12]

#define R_PCH_SPI_PR1                        0x78  // Protected Region 1 Register
#define B_PCH_SPI_PR1_WPE                    BIT31 // Write Protection Enable
#define B_PCH_SPI_PR1_PRL_MASK               0x7FFF0000 // Protected Range Limit Mask
#define B_PCH_SPI_PR1_RPE                    BIT15 // Read Protection Enable
#define B_PCH_SPI_PR1_PRB_MASK               0x00007FFF // Protected Range Base Mask

#define R_PCH_SPI_PR2                        0x7C  // Protected Region 2 Register
#define B_PCH_SPI_PR2_WPE                    BIT31 // Write Protection Enable
#define B_PCH_SPI_PR2_PRL_MASK               0x7FFF0000 // Protected Range Limit Mask
#define B_PCH_SPI_PR2_RPE                    BIT15 // Read Protection Enable
#define B_PCH_SPI_PR2_PRB_MASK               0x00007FFF // Protected Range Base Mask

#define R_PCH_SPI_PR3                        0x80  // Protected Region 3 Register
#define B_PCH_SPI_PR3_WPE                    BIT31 // Write Protection Enable
#define B_PCH_SPI_PR3_PRL_MASK               0x7FFF0000 // Protected Range Limit Mask
#define B_PCH_SPI_PR3_RPE                    BIT15 // Read Protection Enable
#define B_PCH_SPI_PR3_PRB_MASK               0x00007FFF // Protected Range Base Mask

#define R_PCH_SPI_PR4                        0x84  // Protected Region 4 Register
#define B_PCH_SPI_PR4_WPE                    BIT31 // Write Protection Enable
#define B_PCH_SPI_PR4_PRL_MASK               0x7FFF0000 // Protected Range Limit Mask
#define B_PCH_SPI_PR4_RPE                    BIT15 // Read Protection Enable
#define B_PCH_SPI_PR4_PRB_MASK               0x00007FFF // Protected Range Base Mask

#define R_PCH_SPI_SSFCS                      0x90  // Software Sequencing Flash Control Status Register
#define B_PCH_SPI_SSFCS_SCF_MASK             (BIT26 | BIT25 | BIT24) // SPI Cycle Frequency
#define V_PCH_SPI_SSFCS_SCF_20MHZ            0     // SPI Cycle Frequency = 20MHz
#define V_PCH_SPI_SSFCS_SCF_33MHZ            1     // SPI Cycle Frequency = 33MHz
#define V_PCH_SPI_SSFCS_SCF_50MHZ            4     // SPI Cycle Frequency = 50MHz
#define B_PCH_SPI_SSFCS_SME                  BIT23 // SPI SMI# Enable
#define B_PCH_SPI_SSFCS_DC                   BIT22 // SPI Data Cycle
#define B_PCH_SPI_SSFCS_DBC_MASK             0x3F0000 // SPI Data Byte Count (value here + 1 = count)
#define B_PCH_SPI_SSFCS_COP                  0x7000 // Cycle Opcode Pointer
#define B_PCH_SPI_SSFCS_SPOP                 BIT11 // Sequence Prefix Opcode Pointer
#define B_PCH_SPI_SSFCS_ACS                  BIT10 // Atomic Cycle Sequence
#define B_PCH_SPI_SSFCS_SCGO                 BIT9  // SPI Cycle Go
#define B_PCH_SPI_SSFCS_FRS                  BIT7  // Fast Read Supported
#define B_PCH_SPI_SSFCS_DOFRS                BIT6  // Dual Output Fast Read Supported
#define B_PCH_SPI_SSFCS_AEL                  BIT4  // Access Error Log
#define B_PCH_SPI_SSFCS_FCERR                BIT3  // Flash Cycle Error
#define B_PCH_SPI_SSFCS_CDS                  BIT2  // Cycle Done Status
#define B_PCH_SPI_SSFCS_SCIP                 BIT0  // SPI Cycle in Progress

#define R_PCH_SPI_PREOP                      0x94  // Prefix Opcode Configuration Register (16 bits)
#define B_PCH_SPI_PREOP1_MASK                0xFF00 // Prefix Opcode 1 Mask
#define B_PCH_SPI_PREOP0_MASK                0x00FF // Prefix Opcode 0 Mask

#define R_PCH_SPI_OPTYPE                     0x96  // Opcode Type Configuration
#define B_PCH_SPI_OPTYPE7_MASK               (BIT15 | BIT14) // Opcode Type 7 Mask
#define B_PCH_SPI_OPTYPE6_MASK               (BIT13 | BIT12) // Opcode Type 6 Mask
#define B_PCH_SPI_OPTYPE5_MASK               (BIT11 | BIT10) // Opcode Type 5 Mask
#define B_PCH_SPI_OPTYPE4_MASK               (BIT9 | BIT8) // Opcode Type 4 Mask
#define B_PCH_SPI_OPTYPE3_MASK               (BIT7 | BIT6) // Opcode Type 3 Mask
#define B_PCH_SPI_OPTYPE2_MASK               (BIT5 | BIT4) // Opcode Type 2 Mask
#define B_PCH_SPI_OPTYPE1_MASK               (BIT3 | BIT2) // Opcode Type 1 Mask
#define B_PCH_SPI_OPTYPE0_MASK               (BIT1 | BIT0) // Opcode Type 0 Mask
#define V_PCH_SPI_OPTYPE_RDNOADDR            0x00  // Read cycle type without address
#define V_PCH_SPI_OPTYPE_WRNOADDR            0x01  // Write cycle type without address
#define V_PCH_SPI_OPTYPE_RDADDR              0x02  // Address required; Read cycle type
#define V_PCH_SPI_OPTYPE_WRADDR              0x03  // Address required; Write cycle type

#define R_PCH_SPI_OPMENU0                    0x98  // Opcode Menu Configuration 0 (32bits)
#define R_PCH_SPI_OPMENU1                    0x9C  // Opcode Menu Configuration 1 (32bits)

#define R_PCH_SPI_LOCK                       0xA4  // Individual Lock
#define B_PCH_SPI_LOCK_OPMENULOCKDN          BIT9  // OPMENU Lock Down
#define B_PCH_SPI_LOCK_OPTYPELOCKDN          BIT8  // OPTYPE Lock Down
#define B_PCH_SPI_LOCK_PREOPLOCKDN           BIT7  // PREOP Lock Down
#define B_PCH_SPI_LOCK_FREQLOCKDN            BIT6  // SCF Lock Down
#define B_PCH_SPI_LOCK_PR3LOCKDN             BIT5  // PR3 Lock Down
#define B_PCH_SPI_LOCK_PR2LOCKDN             BIT4  // PR2 Lock Down
#define B_PCH_SPI_LOCK_PR1LOCKDN             BIT3  // PR1 Lock Down
#define B_PCH_SPI_LOCK_PR0LOCKDN             BIT2  // PR0 Lock Down
#define B_PCH_SPI_LOCK_BMRAGLOCKDN           BIT1  // BMRAG Lock Down
#define B_PCH_SPI_LOCK_BMWAGLOCKDN           BIT0  // BMWAG Lock Down

#define R_PCH_SPI_FDOC                       0xB0  // Flash Descriptor Observability Control Register (32 bits)
#define B_PCH_SPI_FDOC_FDSS_MASK             (BIT14 | BIT13 | BIT12) // Flash Descriptor Section Select
#define V_PCH_SPI_FDOC_FDSS_FSDM             0x0000 // Flash Signature and Descriptor Map
#define V_PCH_SPI_FDOC_FDSS_COMP             0x1000 // Component
#define V_PCH_SPI_FDOC_FDSS_REGN             0x2000 // Region
#define V_PCH_SPI_FDOC_FDSS_MSTR             0x3000 // Master
#define V_PCH_SPI_FDOC_FDSS_STRP             0x4000 // Soft Straps
#define B_PCH_SPI_FDOC_FDSI_MASK             0x0FFC // Flash Descriptor Section Index

#define R_PCH_SPI_FDOD                       0xB4  // Flash Descriptor Observability Data Register (32 bits)

#define R_PCH_SPI_AFC                        0xC0  // Additional Flash Control Register
#define B_PCH_SPI_AFC_RRWSP                  (BIT7 | BIT6 | BIT5 | BIT4) // Reserved RW Scratch Pad
#define B_PCH_SPI_AFC_SPFP                   BIT3  // Stop Prefetch on Flush Pending
#define B_PCH_SPI_AFC_INF_DCGE               (BIT2 | BIT1) // Flash Controller Interface Dynamic Clock Gating Enable
#define B_PCH_SPI_AFC_CORE_DCGE              BIT0  // Flash Core Dynamic Clock Gating Enable

#define R_PCH_SPI_VSCC0                      0xC4  // Lower Vendor Specific Component Capabilities Register (32 bits)
#define B_PCH_SPI_VSCC0_CPPTV                BIT31 // Component Property Parameter Table Valid
#define B_PCH_SPI_VSCC0_VCL                  BIT23 // Vendor Component Lock
#define B_PCH_SPI_VSCC0_CAP_MASK             0x0000FFFF // Capabilities Mask
#define B_PCH_SPI_VSCC0_EO_MASK              0x0000FF00 // Erase Opcode
#define B_PCH_SPI_VSCC0_QER                  (BIT7 | BIT6 | BIT5) // Quad Enable Requirements
#define B_PCH_SPI_VSCC0_WEWS                 BIT4  // Write Enable on Write Status
#define B_PCH_SPI_VSCC0_WSR                  BIT3  // Write Status Required
#define B_PCH_SPI_VSCC0_WG_64B               BIT2  // Write Granularity, 0: 1 Byte; 1: 64 Bytes
#define B_PCH_SPI_VSCC0_BSES_MASK            (BIT1 | BIT0) // Block/Sector Erase Size
#define V_PCH_SPI_VSCC0_BSES_256B            0x0   // Block/Sector Erase Size = 256 Bytes
#define V_PCH_SPI_VSCC0_BSES_4K              0x1   // Block/Sector Erase Size = 4K Bytes
#define V_PCH_SPI_VSCC0_BSES_8K              0x2   // Block/Sector Erase Size = 8K Bytes
#define V_PCH_SPI_VSCC0_BSES_64K             0x3   // Block/Sector Erase Size = 64K Bytes

#define R_PCH_SPI_VSCC1                      0xC8  // Upper Vendor Specific Component Capabilities Register (32 bits)
#define B_PCH_SPI_VSCC1_CPPTV                BIT31 // Component Property Parameter Table Valid
#define B_PCH_SPI_VSCC1_CAP_MASK             0x0000FFFF // Capabilities Mask
#define B_PCH_SPI_VSCC1_EO_MASK              0x0000FF00 // Erase Opcode
#define B_PCH_SPI_VSCC1_QER                  (BIT7 | BIT6 | BIT5) // Quad Enable Requirements
#define B_PCH_SPI_VSCC1_WEWS                 BIT4  // Write Enable on Write Status
#define B_PCH_SPI_VSCC1_WSR                  BIT3  // Write Status Required
#define B_PCH_SPI_VSCC1_WG_64B               BIT2  // Write Granularity, 0: 1 Byte; 1: 64 Bytes
#define B_PCH_SPI_VSCC1_BSES_MASK            (BIT1 | BIT0) // Block/Sector Erase Size
#define V_PCH_SPI_VSCC1_BSES_256B            0x0   // Block/Sector Erase Size = 256 Bytes
#define V_PCH_SPI_VSCC1_BSES_4K              0x1   // Block/Sector Erase Size = 4K Bytes
#define V_PCH_SPI_VSCC1_BSES_8K              0x2   // Block/Sector Erase Size = 8K Bytes
#define V_PCH_SPI_VSCC1_BSES_64K             0x3   // Block/Sector Erase Size = 64K Bytes

#define R_PCH_SPI_PTINX                      0xCC  // Parameter Table Index
#define B_PCH_SPI_PTINX_SPT                  (BIT15 | BIT14) // Supported Parameter Table
#define N_PCH_SPI_PTINX_SPT                  14
#define V_PCH_SPI_PTINX_SPT_CPT0             0x0   // Component 0 Property Parameter Table
#define V_PCH_SPI_PTINX_SPT_CPT1             0x1   // Component 1 Property Parameter Table
#define B_PCH_SPI_PTINX_HORD                 (BIT13 | BIT12) // Header or Data
#define N_PCH_SPI_PTINX_HORD                 12
#define V_PCH_SPI_PTINX_HORD_SFDP            0x0   // SFDP Header
#define V_PCH_SPI_PTINX_HORD_PT              0x1   // Parameter Table Header
#define V_PCH_SPI_PTINX_HORD_DATA            0x2   // Data
#define B_PCH_SPI_PTINX_PTDWI                0xFFC // Parameter Table DW Index

#define R_PCH_SPI_PTDATA                     0xD0  // Parameter Table Data

#define R_PCH_SPI_FPB                        0xD4  // Flash Partition Boundary
#define B_PCH_SPI_FPB_FPBA_MASK              0x00001FFF // Flash Partition Boundary Address Mask, reflecting FPBA[24:12]

#define R_PCH_SPI_SCS                        0xF8  // SMI Control Status Register
#define S_PCH_SPI_SCS                        1
#define B_PCH_SPI_SCS_SMIWPEN                BIT7  // SMI WPD Enable
#define B_PCH_SPI_SCS_SMIWPST                BIT6  // SMI WPD Status
#define N_PCH_SPI_SCS_SMIWPEN                7
#define N_PCH_SPI_SCS_SMIWPST                6

#define R_PCH_SPI_BCR                        0xFC  // BIOS Control Register
#define S_PCH_SPI_BCR                        1
#define B_PCH_SPI_BCR_SMM_BWP                BIT5  // SMM BIOS Write Protect Disable
#define B_PCH_SPI_BCR_SRC                    (BIT3 | BIT2) // SPI Read Configuration (SRC)
#define V_PCH_SPI_BCR_SRC_PREF_EN_CACHE_EN   0x08  // Prefetch Enable, Cache Enable
#define V_PCH_SPI_BCR_SRC_PREF_DIS_CACHE_DIS 0x04  // Prefetch Disable, Cache Disable
#define V_PCH_SPI_BCR_SRC_PREF_DIS_CACHE_EN  0x00  // Prefetch Disable, Cache Enable
#define B_PCH_SPI_BCR_BLE                    BIT1  // Lock Enable (LE)
#define B_PCH_SPI_BCR_BIOSWE                 BIT0  // Write Protect Disable (WPD)
#define N_PCH_SPI_BCR_BLE                    1
#define N_PCH_SPI_BCR_BIOSWE                 0

#define R_PCH_SPI_TCGC                       0x100 // Trunk Clock Gating Control
#define B_PCH_SPI_TCGC_FCGDIS                BIT10 // Functional Clock Gating Disable
#define B_PCH_SPI_TCGC_SBCGCDEF              BIT9  // Sideband Control Gating Clock Defeature
#define B_PCH_SPI_TCGC_SBCGEN                BIT8  // Sideband Control Gating Clock Enable
#define B_PCH_SPI_TCGC_SBCGCNT               0xFF  // Sideband Control Gating Clock Counter

//
// Flash Descriptor Base Address Region (FDBAR) from Flash Region 0
//
#define R_PCH_SPI_FDBAR_FLVALSIG             0x00  // Flash Valid Signature
#define V_PCH_SPI_FDBAR_FLVALSIG             0x0FF0A55A

#define R_PCH_SPI_FDBAR_FLASH_MAP0           0x04  // Flash MAP 0
#define B_PCH_SPI_FDBAR_NR                   0x07000000 // Number Of Regions
#define B_PCH_SPI_FDBAR_FRBA                 0x00FF0000 // Flash Region Base Address
#define B_PCH_SPI_FDBAR_NC                   0x00000300 // Number Of Components
#define V_PCH_SPI_FDBAR_NC_2                 0x00000100
#define V_PCH_SPI_FDBAR_NC_1                 0x00000000
#define N_PCH_SPI_FDBAR_NC                   0x08  // Number Of Components
#define B_PCH_SPI_FDBAR_FCBA                 0x000000FF // Flash Component Base Address

//
// Flash Component Base Address (FCBA) from Flash Region 0
//
#define R_PCH_SPI_FCBA_FLCOMP                0x00  // Flash Components Register
#define B_PCH_SPI_FLCOMP_DOFRS               BIT30 // Dual Output Fast Read Support
#define B_PCH_SPI_FLCOMP_RIDS_FREQ           (BIT29 | BIT28 | BIT27) // Read ID and Read Status Clock Frequency
#define B_PCH_SPI_FLCOMP_WE_FREQ             (BIT26 | BIT25 | BIT24) // Write and Erase Clock Frequency
#define B_PCH_SPI_FLCOMP_FRCF_FREQ           (BIT23 | BIT22 | BIT21) // Fast Read Clock Frequency
#define B_PCH_SPI_FLCOMP_FR_SUP              BIT20 // Fast Read Support.
#define B_PCH_SPI_FLCOMP_RC_FREQ             (BIT19 | BIT18 | BIT17) // Read Clock Frequency.
#define V_PCH_SPI_FLCOMP_FREQ_20MHZ          0x00
#define B_PCH_SPI_FLCOMP_COMP2_MASK          0x38  // Flash Component 2 Density
#define V_PCH_SPI_FLCOMP_COMP2_512KB         0x00
#define V_PCH_SPI_FLCOMP_COMP2_1MB           0x08
#define V_PCH_SPI_FLCOMP_COMP2_2MB           0x10
#define V_PCH_SPI_FLCOMP_COMP2_4MB           0x18
#define V_PCH_SPI_FLCOMP_COMP2_8MB           0x20
#define V_PCH_SPI_FLCOMP_COMP2_16MB          0x28
#define B_PCH_SPI_FLCOMP_COMP1_MASK          0x07  // Flash Component 1 Density
#define V_PCH_SPI_FLCOMP_COMP1_512KB         0x00
#define V_PCH_SPI_FLCOMP_COMP1_1MB           0x01
#define V_PCH_SPI_FLCOMP_COMP1_2MB           0x02
#define V_PCH_SPI_FLCOMP_COMP1_4MB           0x03
#define V_PCH_SPI_FLCOMP_COMP1_8MB           0x04
#define V_PCH_SPI_FLCOMP_COMP1_16MB          0x05
#define V_PCH_SPI_FLCOMP_COMP_512KB          0x80000

//
// Flash Soft Strap Base Address (FISBA) from Flash Region 0
//


//
// Descriptor Upper Map Section from Flash Region 0
//
#define R_PCH_SPI_FLASH_UMAP1                0xEFC // Flash Upper Map 1
#define B_PCH_SPI_FLASH_UMAP1_VTL            0x0000FF00 // VSCC Table Length
#define B_PCH_SPI_FLASH_UMAP1_VTBA           0x000000FF // VSCC Table Base Address

#define R_PCH_SPI_VTBA_JID0                  0x00  // JEDEC-ID 0 Register
#define S_PCH_SPI_VTBA_JID0                  4
#define B_PCH_SPI_VTBA_JID0_DID1             0x00FF0000 // SPI Component Device ID 1
#define N_PCH_SPI_VTBA_JID0_DID1             0x10
#define B_PCH_SPI_VTBA_JID0_DID0             0x0000FF00 // SPI Component Device ID 0
#define N_PCH_SPI_VTBA_JID0_DID0             0x08
#define B_PCH_SPI_VTBA_JID0_VID              0x000000FF // SPI Component Vendor ID

#define R_PCH_SPI_VTBA_VSCC0                 0x04  // Vendor Specific Component Capabilities 0
#define S_PCH_SPI_VTBA_VSCC0                 4
#define B_PCH_SPI_VTBA_VSCC0_CAPS            0x0000FFFF
#define B_PCH_SPI_VTBA_VSCC0_EO              0x0000FF00 // Erase Opcode
#define B_PCH_SPI_VTBA_VSCC0_WEWS            BIT4  // Write Enable on Write Status
#define B_PCH_SPI_VTBA_VSCC0_WSR             BIT3  // Write Status Required
#define B_PCH_SPI_VTBA_VSCC0_WG              BIT2  // Write Granularity
#define B_PCH_SPI_VTBA_VSCC0_BES             (BIT1 | BIT0) // Block / Sector Erase Size

#endif
