/** @file
  GPIO setting for CherryView.

  This file includes package header files, library classes.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _BOARDGPIOS_H_
#define _BOARDGPIOS_H_

#include <PiPei.h>
#include "PchAccess.h"
#include "PlatformBaseAddresses.h"
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/Timerlib.h>
#include <Library/PchPlatformLib.h>
#include <Library/GpioLib.h>
#include <Guid/PlatformInfo.h>

//#define EFI_DEBUG 1
#define GPIO_DFX1_PAD0            0x4418
#define GPIO_DFX3_PAD0            0x4408
#define GPIO_MF_ISH_I2C1_SCL_PAD0 0x4810
#define GPIO_MF_ISH_I2C1_SDA_PAD0 0x4858
#define GPIO_PCIE_CLKREQ0B_PAD0   0x5C00
#define GPIO_SEC_GPIO_SUS8_PAD0   0x4840
#define GPIO_SEC_GPIO_SUS8_PAD1   0x4844
#define GPIO_SEC_GPIO_SUS9_PAD0   0x4860
#define GPIO_SEC_GPIO_SUS9_PAD1   0x4864
#define GPIO_SEC_GPIO_SUS10_PAD0  0x4808
#define GPIO_SEC_GPIO_SUS10_PAD1  0x480C
#define GPIO_I2C_NFC_SCL_PAD0     0x5038
#define GPIO_I2C_NFC_SCL_PAD1     0x503C
#define GPIO_I2C_NFC_SDA_PAD0     0x5020
#define GPIO_I2C_NFC_SDA_PAD1     0x5024
#define GPIO_MF_ISH_GPIO_7_PAD0   0x4808

#define USB3_CAMERA                0
#define MIPI_CAMERA                1
#define DISABLE                    0
#define NO_PULL_UP                 0
#define SECURE_NFC_ENABLE          1

#define GPIO_SOC_RUNTIME_SCI_N  0x4850

/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************
/// ***************************************************     CHERRYVIEW GPIO CONFIGURATION   *************************************************
/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************

#define ENABLE  1
#define DISABLE 0

/// Community Configuration
/// Family Configuration
/*
*   GPIO Families configuration in CherryView
*
*/
GPIO_SAI_INIT  mBSW_GPIO_SAI_Init_East[] =
{ CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_access_reg                      ", 0x0  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_wake_reg                        ", 0x4  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_interrupt_reg                   ", 0x8  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_community_reg                   ", 0xC  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_broadcast_reg            ", 0x10 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_reg                      ", 0x14 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_pad_broadcast_reg               ", 0x18 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_4x2_12_0_regs     ", 0x1C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_4x2_12_1_regs     ", 0x20 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_access_reg                     ", 0x100, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_wake_reg                       ", 0x104, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_interrupt_reg                  ", 0x108, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_community_reg                  ", 0x10C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_broadcast_reg           ", 0x110, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_reg                     ", 0x114, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_pad_broadcast_reg              ", 0x118, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_4x2_12_0_regs    ", 0x11c, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_4x2_12_1_regs    ", 0x120, 0x18310, ENABLE),
};

GPIO_SAI_INIT  mBSW_GPIO_SAI_Init_North[] =
{ CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_access_reg                          ", 0x0  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_wake_reg                            ", 0x4  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_interrupt_reg                       ", 0x8  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_community_reg                       ", 0xC  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_broadcast_reg                ", 0x10 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_reg                          ", 0x14 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_pad_broadcast_reg                   ", 0x18 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulphsmvfamily_2x4_rcomp_10_0_regs   ", 0x1C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x4_13_0_regs         ", 0x20 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_3x4_12_0_regs         ", 0x24 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulphsmvfamily_2x4_12_0_regs         ", 0x28 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_3x4_13_0_regs         ", 0x2C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_access_reg                         ", 0x100, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_wake_reg                           ", 0x104, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_interrupt_reg                      ", 0x108, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_community_reg                      ", 0x10C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_broadcast_reg               ", 0x110, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_reg                         ", 0x114, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_pad_broadcast_reg                  ", 0x118, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulphsmvfamily_2x4_rcomp_10_0_regs  ", 0x11C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x4_13_0_regs        ", 0x120, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_3x4_12_0_regs        ", 0x124, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulphsmvfamily_2x4_12_0_regs        ", 0x128, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_3x4_13_0_regs        ", 0x12C, 0x18310, ENABLE),
};

GPIO_SAI_INIT  mBSW_GPIO_SAI_Init_SouthEast[] =
{ CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_access_reg                              ", 0x0  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_wake_reg                                ", 0x4  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_interrupt_reg                           ", 0x8  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_community_reg                           ", 0xC  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_broadcast_reg                    ", 0x10 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_reg                              ", 0x14 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_pad_broadcast_reg                       ", 0x18 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_2_regs              ", 0x1C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulphsmvfamily_3x3_rcomp_13_0_regs       ", 0x20 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_hshvfamily_2x3_rcomp_7_0_regs           ", 0x24 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_hshvfamily_3x3_rcomp_9_0_regs           ", 0x28 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulphsmvfamily_3x3_10_0_regs             ", 0x2C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_3x3_11_0_regs             ", 0x30 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_access_reg                             ", 0x100, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_wake_reg                               ", 0x104, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_interrupt_reg                          ", 0x108, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_community_reg                          ", 0x10C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_broadcast_reg                   ", 0x110, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_reg                             ", 0x114, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_pad_broadcast_reg                      ", 0x118, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_2_regs             ", 0x11C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulphsmvfamily_3x3_rcomp_13_0_regs      ", 0x120, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_hshvfamily_2x3_rcomp_7_0_regs          ", 0x124, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_hshvfamily_3x3_rcomp_9_0_regs          ", 0x128, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulphsmvfamily_3x3_10_0_regs            ", 0x12C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_3x3_11_0_regs            ", 0x130, 0x18310, ENABLE),
};

GPIO_SAI_INIT  mBSW_GPIO_SAI_Init_SouthWest[] =
{ CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_access_reg                      ",0x0  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_wake_reg                        ",0x4  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_interrupt_reg                   ",0x8  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_community_reg                   ",0xC  , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_broadcast_reg            ",0x10 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_family_reg                      ",0x14 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_pad_broadcast_reg               ",0x18 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulphsmvfamily_2x3_8_1_regs      ",0x1C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_6_regs      ",0x20 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_5_regs      ",0x24 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_4_regs      ",0x28 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_3_regs      ",0x2C , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_1_regs      ",0x30 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_read_access_policy_ulpmsmvfamily_2x3_8_0_regs      ",0x34 , 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_access_reg                     ",0x100, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_wake_reg                       ",0x104, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_interrupt_reg                  ",0x108, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_community_reg                  ",0x10C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_broadcast_reg           ",0x110, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_family_reg                     ",0x114, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_pad_broadcast_reg              ",0x118, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulphsmvfamily_2x3_8_1_regs     ",0x11C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_6_regs     ",0x120, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_5_regs     ",0x124, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_4_regs     ",0x128, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_3_regs     ",0x12C, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_1_regs     ",0x130, 0x18310, ENABLE),
  CHV_GPIO_SAI_INIT(L"gpio_write_access_policy_ulpmsmvfamily_2x3_8_0_regs     ",0x134, 0x18310, ENABLE),
};

CHV_GPIO_PAD_INIT  mBSW_CR_GpioInitData_N[] =
//                         PAD Name              Mode Select    Mode#  GPIO Config  GPIO TX State   GPIO Light       INT Type          INT Select     Term H/L          Open Drain    Current Source        Mask/Un-Mask        Glitch Cfg            InvertRX_TX             WAKE Cap    Wake_Mask_Bit    GPE    MMIO Addr    Community        Remarks
{ 
  CHV_GPIO_PAD_CONF   (L"  N15:GPIO_SUS0         ",  GPIO    ,  M1      ,  GPI      ,  NA         ,  NA      ,  Trig_Edge_Low        ,       Line3    ,  P_NONE      ,          NA    ,          NA    ,      NonMaskable    ,     En_Edge_RX_Data    ,          NA           , UNMASK_WAKE  ,  9   ,   SCI  ,      0x4800      ,  NORTH  )  ,  //  SOC_WAKE_SCI_N
  CHV_GPIO_PAD_CONF   (L"  N25:GPIO_SUS6         ",  GPIO    ,  M1      ,  GPI      ,  NA         ,  NA      ,  Trig_Edge_Low        ,       Line14   ,      NA      ,          NA    ,          NA    ,      NonMaskable    ,     En_Edge_RX_Data    ,          NA           ,      NA      ,  19  ,   SCI  ,      0x4850      ,  NORTH  )  ,  //  SOC_RUNTIME_SCI_N
  CHV_GPIO_PAD_CONF   (L"  N18:GPIO_SUS7         ",  GPIO    ,  M1      ,  GPI      ,  NA         ,  NA      ,  Trig_Edge_Low        ,       Line15   ,      NA      ,          NA    ,          NA    ,      NonMaskable    ,     En_Edge_RX_Data    ,          NA           ,      NA      ,  12  ,   SMI  ,      0x4818      ,  NORTH  )  ,  //  SOC_EXTSMI_N

};

#endif
