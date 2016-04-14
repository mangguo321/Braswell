/** @file
  Register names for Integrated Sensor Hub.

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

#ifndef _PCH_REGS_ISH_H_
#define _PCH_REGS_ISH_H_

//
// ISH Config Registers (D10:F0)
//
#define PCI_DEVICE_NUMBER_PCH_ISH          10
#define PCI_FUNCTION_NUMBER_PCH_ISH        0

#define R_PCH_ISH_DEVVENID                 0x00  // Device / Vendor ID
#define B_PCH_ISH_DEVVENID_DEVICE_ID       0xFFFF0000 // Device ID
#define B_PCH_ISH_DEVVENID_VENDOR_ID       0x0000FFFF // Vendor ID
#define V_PCH_ISH_DEVVENID_VENDOR_ID       V_PCH_INTEL_VENDOR_ID // Intel Vendor ID

#define R_PCH_ISH_STSCMD                   0x04  // Status Command
#define B_PCH_ISH_STSCMD_RMA               BIT29 // Received Master Abort
#define B_PCH_ISH_STSCMD_RTA               BIT28 // Received Target Abort
#define B_PCH_ISH_STSCMD_CAP_LST           BIT20 // Capabilities List
#define B_PCH_ISH_STSCMD_INTR_STS          BIT19 // Interrupt Status
#define B_PCH_ISH_STSCMD_INTR_DIS          BIT10 // Interrupt Disable
#define B_PCH_ISH_STSCMD_SERR_EN           BIT8  // SERR Enable
#define B_PCH_ISH_STSCMD_BME               BIT2  // Bus Master Enable
#define B_PCH_ISH_STSCMD_MSE               BIT1  // Memory Space Enable

#define R_PCH_ISH_RID_CC                   0x08  // Revision ID and Class Code
#define B_PCH_ISH_RID_CC_BCC               0xFF000000 // Base Class Code
#define B_PCH_ISH_RID_CC_SCC               0x00FF0000 // Sub Class Code
#define B_PCH_ISH_RID_CC_PI                0x0000FF00 // Programming Interface
#define B_PCH_ISH_RID_CC_RID               0x000000FF // Revision Identification

#define R_PCH_ISH_BAR0                     0x10  // BAR 0
#define B_PCH_ISH_BAR0_BA                  0xFFFFF000 // Base Address
#define V_PCH_ISH_BAR0_SIZE                0x1000
#define N_PCH_ISH_BAR0_ALIGNMENT           12
#define B_PCH_ISH_BAR0_PREF                BIT3  // Prefetchable
#define B_PCH_ISH_BAR0_ADDRNG              (BIT2 | BIT1) // Address Range
#define B_PCH_ISH_BAR0_SPTYP               BIT0  // Space Type (Memory)

#define R_PCH_ISH_BAR1                     0x18  // BAR 1
#define B_PCH_ISH_BAR1_BA                  0xFFFFF000 // Base Address
#define B_PCH_ISH_BAR1_PREF                BIT3  // Prefetchable
#define B_PCH_ISH_BAR1_ADDRNG              (BIT2 | BIT1) // Address Range
#define B_PCH_ISH_BAR1_SPTYP               BIT0  // Space Type (Memory)
#define V_PCH_ISH_BAR1_SIZE                (1 << 12)

#define R_PCH_ISH_SSID                     0x2C  // Sub System ID
#define B_PCH_ISH_SSID_SID                 0xFFFF0000 // Sub System ID
#define B_PCH_ISH_SSID_SVID                0x0000FFFF // Sub System Vendor ID

#define R_PCH_ISH_ERBAR                    0x30  // Expansion ROM BAR
#define B_PCH_ISH_ERBAR_BA                 0xFFFFFFFF // Expansion ROM Base Address

#define R_PCH_ISH_CAPPTR                   0x34  // Capability Pointer
#define B_PCH_ISH_CAPPTR_CPPWR             0xFF  // Capability Pointer Power

#define R_PCH_ISH_INTR                     0x3C  // Interrupt
#define B_PCH_ISH_INTR_ML                  0xFF000000 // Max Latency
#define B_PCH_ISH_INTR_MG                  0x00FF0000
#define B_PCH_ISH_INTR_IP                  0x00000F00 // Interrupt Pin
#define B_PCH_ISH_INTR_IL                  0x000000FF // Interrupt Line

#endif
