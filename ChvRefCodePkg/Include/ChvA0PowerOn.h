/** @file

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
//Use SPI Register as a scratch pad for A0 PO
//

#ifndef __CHVA0POWERON_H__
#define __CHVA0POWERON_H__

/**
@todo	add description

**/
UINT32 GetSskpdReg( UINT32 PortId, UINT32 Register )  //Use the C-Unit Msg Bus to read from SB
{
  UINT32 OpCode = 0x10000000;
  //
  // The Msg Bus addresses can be simplifed since the C-Unit B-D-F is 0-0-0.
  // eg:  *(MmPcieAddr32(0, 0, 0, MC_MCRX)) = *(EC_BASE + MC_MCRX) = 0xE00000D8
  //
  // MCRX
  *((UINT32 *)((UINTN) 0xE00000D8)) = (UINT32) (Register & 0xFFFFFF00);
  // MCR: Opcode[31:24], PortId[23:16], AddrL[15:08], WriteEnBytes[07:04]
  *((UINT32 *)((UINTN) 0xE00000D0)) = (UINT32) (OpCode | (PortId <<16) | ((Register & 0x000000FF)<<8) | 0xF0);
  // MDR
  return *((UINT32 *)((UINTN) 0xE00000D4));
}

//Bypass is active low, so get the value and invert it.
#define A0_PO_REG   ~(*((volatile UINT16*)(UINTN)0xFED010A2))
#define A0_PO_REG1  ~GetSskpdReg(0x1, 0x4A)

//Feature Enable Register, not inverted
#define A0_PO_FEATURE_REG_0  GetSskpdReg(0x1, 0x4B)

//A0_PO_REG                            // These are the Values of A0_PO_REG, NOT the actual Register.
#define ENABLE_SWSMI     0x00000001    // BIT0,  1 = Enable SWSMI triggering,    0 = Bypass SWSMI triggering
#define BYPASS_GEN       0x00000002    // BIT1,  1 = Bypass access to B0:D2:F0,  0 = Allow access to B0:D2:F0
#define BYPASS_ISH       0x00000004    // BIT2   1 = Bypass ISH Init,  0 = Regular ISH flow
#define BYPASS_IUNIT     0x00000008    // BIT3,  1 = Bypass access to B0:D3:F0,  0 = Allow access to B0:D3:F0
#define ENABLE_INT_UART  0x00000010    // BIT4,  1 = Enable Internal-UART,  0 = Bypass Int-UART (use External-UART)
#define BYPASS_DISPLAY   0x00000020    // BIT5,  1 = Bypass GOP Policy,  0 = Regular GOP flow (default Enabled)
#define BYPASS_GPIO      0x00000040    // BIT6,  1 = Bypass GPIO,   0 = Regular GPIO flow
#define BYPASS_AUDIO     0x00000080    // BIT7,  1 = Bypass Audio,  0 = Regular Audio flow
#define BYPASS_USB2PHY   0x00000100    // BIT8,  1 = Bypass USB2 PHY Workaround,  0 = Perform USB2 PHY Workaround
#define BYPASS_UNUSED9   0x00000200    // BIT9
#define BYPASS_UNUSED10  0x00000400    // BIT10
#define NO_LPC_REWORK    0x00000800    // BIT11,  1 = Board w/3V LPC,  0 = Board has 1.8V LPC Rework
#define GOP_RSVD         0x00001000    // BIT12,  Reserved for GOP driver - not currently used
#define BYPASS_MMC_HS    0x00002000    // BIT13,  1 = ByPass MMC HighSpeed,   0 = Enable MMC HighSpeed
#define BYPASS_USB       0x00004000    // BIT14,  1 = Bypass EFI USB driver,  0 = Load EFI USB driver
#define BYPASS_UCODE     0x00008000    // BIT15,  1 = Bypass Microcode loading,  0 = Allow Microcode loading

//A0_PO_REG1                                // These are the Values of A0_PO_REG1, NOT the actual Register.
#define BYPASS1_UNUSED0       0x00000001    //bit0
#define BYPASS1_UNUSED1       0x00000002    //bit1
#define BYPASS1_UNUSED2       0x00000004    //bit2
#define BYPASS1_UNUSED3       0x00000008    //bit3
#define BYPASS1_UNUSED4       0x00000010    //bit4
#define BYPASS1_UNUSED5       0x00000020    //bit5
#define BYPASS1_UNUSED6       0x00000040    //bit6
#define BYPASS1_UNUSED7       0x00000080    //bit7
#define BYPASS1_UNUSED8       0x00000100    //bit8
#define BYPASS1_UNUSED9       0x00000200    //bit9
#define BYPASS1_UNUSED10      0x00000400    //bit10
#define BYPASS1_UNUSED11      0x00000800    //bit11
#define BYPASS1_UNUSED12      0x00001000    //bit12
#define BYPASS1_UNUSED13      0x00002000    //bit13
#define BYPASS1_UNUSED14      0x00004000    //bit14
#define BYPASS1_UNUSED15      0x00008000    //bit15
#define BYPASS1_UNUSED16      0x00010000    //bit16
#define BYPASS1_UNUSED17      0x00020000    //bit17
#define BYPASS1_UNUSED18      0x00040000    //bit18
#define BYPASS1_UNUSED19      0x00080000    //bit19
#define BYPASS1_UNUSED20      0x00100000    //bit20
#define BYPASS1_UNUSED21      0x00200000    //bit21
#define BYPASS1_UNUSED22      0x00400000    //bit22
#define BYPASS1_UNUSED23      0x00800000    //bit23
#define BYPASS1_UNUSED24      0x01000000    //bit24
#define BYPASS1_UNUSED25      0x02000000    //bit25
#define BYPASS1_UNUSED26      0x04000000    //bit26
#define BYPASS1_UNUSED27      0x08000000    //bit27
#define BYPASS1_UNUSED28      0x10000000    //bit28
#define BYPASS1_UNUSED29      0x20000000    //bit29
#define BYPASS1_UNUSED30      0x40000000    //bit30
#define BYPASS1_UNUSED31      0x80000000    //bit31

//A0_PO_FEATURE_REG_0                                         // These are the Values of A0_PO_REG1, NOT the actual Register.
#define FEATURE_0_MRC_VERBOSE                   0x00000001    //bit0,  0 = Silent; 1 = Verbose
#define FEATURE_0_SELECT_MRC_CH                 0x00000002    //bit1   0 = Single Channel,  1 = Dual Channel
#define FEATURE_0_SELECT_DVFS_EN                0x00000004    //bit2   0 = DVFS Disabled, 1 = DVFS Enabled
#define FEATURE_0_DUAL_RANK_EN                  0x00000008    //bit3   0 = Single Rank, 1 = Dual Rank
#define FEATURE_0_SELECT_MRC_FA                 0x00000030    //bit[5:4] 00 = 800MHz, 01 = 1066MHz, 10 = 1333MHz, 11 = 1600MHz //DVFS Frequency A
#define FEATURE_0_UNUSED6                       0x00000040    //bit6
#define FEATURE_0_UNUSED7                       0x00000080    //bit7
#define FEATURE_0_SELECT_MRC_FB                 0x00000300    //bit[9:8] 00 = 800MHz, 01 = 1066MHz, 10 = 1333MHz, 11 = 1600MHz //DVFS Frequency B
#define FEATURE_0_UNUSED10                      0x00000400    //bit10
#define FEATURE_0_UNUSED11                      0x00000800    //bit11
#define FEATURE_0_TX_PWR_T_DIS                  0x00001000    //bit12
#define FEATURE_0_RX_PWR_T_DIS                  0x00002000    //bit13
#define FEATURE_0_AUTODETECT_DIS                0x00004000    //bit14 - 0 = AutoDetectEnabled; 1 = AutoDetectDisabled
#define FEATURE_0_UNUSED15                      0x00008000    //bit15
#define FEATURE_0_UNUSED16                      0x00010000    //bit16
#define FEATURE_0_UNUSED17                      0x00020000    //bit17
#define FEATURE_0_UNUSED18                      0x00040000    //bit18
#define FEATURE_0_UNUSED19                      0x00080000    //bit19
#define FEATURE_0_UNUSED20                      0x00100000    //bit20
#define FEATURE_0_UNUSED21                      0x00200000    //bit21
#define FEATURE_0_UNUSED22                      0x00400000    //bit22
#define FEATURE_0_UNUSED23                      0x00800000    //bit23
#define FEATURE_0_UNUSED24                      0x01000000    //bit24
#define FEATURE_0_UNUSED25                      0x02000000    //bit25
#define FEATURE_0_UNUSED26                      0x04000000    //bit26
#define FEATURE_0_UNUSED27                      0x08000000    //bit27
#define FEATURE_0_UNUSED28                      0x10000000    //bit28
#define FEATURE_0_MRC_EV_RMT                    0x20000000    //bit29   0 = Disabled, 1 = Enabled
#define FEATURE_0_MRC_EV_RMT_COMMAND_MODE       0x40000000    //bit30   0 = Disabled, 1 = Enabled
#define FEATURE_0_MRC_OVRRIDE_EN                0x80000000    //bit31   0 = Disable memory configuration override, 1 = Enable memory configuration override

#endif
