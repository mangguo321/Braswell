/** @file
  Library for GPIO Pin Programming.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _GPIO_LIB_H_
#define _GPIO_LIB_H_

#include <PiPei.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <PlatformBaseAddresses.h>
#include "PchAccess.h"
#include <Library/TimerLib.h>
#include <Library/PchPlatformLib.h>

#define SD_MMIO_TIMEOUT                     5
#define HOST_CONTROL_1_REGISTER             0x28
#define SD_POWER_CONTROL_REGISTER           0x29 

#define INVALID_COMMUNITY                   0x01
#define CFIO_COMMUNITY_NORTH                0x01
#define CFIO_COMMUNITY_EAST                 0x02
#define CFIO_COMMUNITY_SOUTH_EAST           0x03
#define CFIO_COMMUNITY_SOUTH_WEST           0x04

// PAD registers offsets
#define GPIO_MMIO_OFFSET_SW                 0x0000
#define GPIO_MMIO_OFFSET_N                  0x8000
#define GPIO_MMIO_OFFSET_E                  0x10000
#define GPIO_MMIO_OFFSET_SE                 0x18000
#define GPIO_MMIO_OFFSET_VIRT               0x20000

#define CHV_GPIO_PAD_CONF0_OFFSET           0x0
#define CHV_GPIO_PAD_CONF1_OFFSET           0x4
#define CHV_GPIO_WAKE_REG0_BITS             0x20

#define LOW                                 0
#define HIGH                                1
#define MASK_WAKE                           0
#define UNMASK_WAKE                         1

// Families registers offsets
#define GPIO_FAMILY_CONF_REGS_OFF           0x4400
#define GPIO_FAMILY_CONF_REGS_SIZE          0x400
#define GPIO_REGS_SIZE                      8

#define RCOMP_CONTROl_REG                   0x0
#define RCOMP_OFFSET_REG                    0x4
#define RCOMP_OVERRIDE_REG                  0x8
#define RCOMP_VALUE_REG                     0xC
#define FAMILY_RCOMP_CONFIG_REG             0x10
#define FAMILY_CONFIG_REG                   0x14

// GPIO Security registers offset
#define GPIO_READ_ACCESS_POLICY_REG         0x0000
#define GPIO_WRITE_ACCESS_POLICY_REG        0x0100
#define GPIO_WAKE_STATUS_REG                0x0200
#define GPIO_WAKE_MASK_REG0                 0x0280
#define GPIO_WAKE_MASK_REG1                 0x0284
#define GPIO_INTERRUPT_STATUS               0x0300
#define GPIO_INTERRUPT_MASK                 0x0380
#define GPE0A_STS_REG                       0x20
#define GPE0A_EN_REG                        0x28
#define ALT_GPIO_SMI_REG                    0x38
#define GPIO_ROUT_REG                       0x58
// Broad cast registers
#define GPIO_FAMILY_BW_MASK_31_0            0x1000
#define GPIO_FAMILY_BW_DATA_31_0            0x1004
#define GPIO_FAMILY_BROADCAST_REG_MASK      0x1008
#define GPIO_PAD_BW_DATA_31_0               0x4004
#define GPIO_PAD_BROADCAST_REG_MASK_0       0x4008

#define MAX_FAMILY_PAD_GPIO_NO              15
#define FAMILY0_PAD_REGS_OFF                0x4400
#define FAMILY_PAD_REGS_SIZE                0x400

#define GPIO_PAD_MODE_MASK                  0xFFF0FFFF
#define GPIO_EN_MASK                        0xFFFF7FFF
#define PULLUP_OR_PULLDOWN_MASK             0xFF8FFFFF
#define PULL_VALUE_MASK                     0xFF8FFFFF
#define GPIO_INV_RXTX_MASK                  0xFFFFFF0F
#define GPIO_INT_MASK                       0x0FFFFFFF
#define GPIO_INT_TYPE                       0xFFFFFFF8

#define GPIO_PAD_LOCK                       0x80000000
#define PULL_UP_EN                          0x00800000
#define GPIO_DIRECTION_EN                   0x00008000
#define GPIO_DIRECTION                      0x00000700
#define GPIO_TX_STATE                       0x00000002

#define GPIO_RETURN_FAIL                    1
#define GPIO_RETURN_PASS                    0

//PAD numbers
#define PAD0                                0x00
#define PAD1                                0x01

//PAD Read or Write
#define PAD_READ                            0x00
#define PAD_WRITE                           0x01

//PAD Lock Status
#define LOCKED                              0x01
#define UNLOCKED                            0x00

//Pull Values
#define PULL_1K                             0x00100000
#define PULL_5K                             0x00200000
#define PULL_20K                            0x00400000

#define NA                                  0xFF

#pragma pack(1)
typedef enum
{
  Native =0xff,
  GPIO = 0, // Native, no need to set PAD_VALUE
  GPO = 1,  // GPI, input only in PAD_VALUE
  GPI = 2,  // GPO, output only in PAD_VALUE
  HI_Z = 3,
} GPIO_En;

typedef enum
{
  LO = 0,
  HI = 1,
} GPO_D4;

typedef enum
{
  F0 = 0,
  F1 = 1,
  F2 = 2,
  F3 = 3
} GPIO_FUNC_NUM;

// Mapping to CONF0 bit 27:24
// Note: Assume "Direct Irq En" is not set, unless specially notified
typedef enum
{
  TRIG_ = 0,
  TRIG_Edge_High = /*BIT3 |*/ BIT1,        // Positive Edge (Rasing)
  TRIG_Edge_Low  = /*BIT3 |*/ BIT2,        // Negative Edge (Falling)
  TRIG_Edge_Both = /*BIT3 |*/ BIT2 | BIT1, // Both Edge
  TRIG_Level_High= /*BIT3 |*/ BIT2 | BIT0, // Level High
  TRIG_Level_Low = /*BIT3 |*/ BIT1 | BIT0, // Level Low
} INT_TYPE;

typedef enum
{
  P_NONE  = 0,  // Pull None
  P_20K_L = 1,  // Pull Down  20K
  P_5K_L  = 2,  // Pull Down  5K
  P_1K_L  = 4,  // Pull Down  1K
  P_20K_H = 9,  // Pull Up 20K
  P_5K_H  = 10, // Pull Up  5K
  P_1K_H  = 12  // Pull Up  1K
} PULL_TYPE;

typedef enum
{
  _DISABLE = 0, // Disable
  _ENABLE = 1,  // Enable
} PARKMODEENB;

typedef enum
{
  VOLT_3_3 = 0, // Working on 3.3 Volts
  VOLT_1_8 = 1, // Working on 1.8 Volts
} VP18MODE;

typedef enum
{
  DISABLE_HS = 0, // Disable high speed mode
  ENABLE_HS  = 1, // Enable high speed mode
} HSMODE;

typedef enum
{
  PULL_UP = 0,    // On Die Termination Up
  PULL_DOWN  = 1, // On Die Termination Down
} ODTUPDN;

typedef enum
{
  DISABLE_OD =0, // On Die Termination Disable
  ENABLE_OD  =1, // On Die Termination Enable
} ODTEN;

typedef enum
{
  ONE_BIT   = 1,
  TWO_BIT   = 3,
  THREE_BIT = 7,
  FOUR_BIT  = 15,
  FIVE_BIT  = 31,
  SIX_BIT   = 63,
  SEVEN_BIT = 127,
  EIGHT_BIT = 255
}BITS;

typedef union {
  UINT32 famCnf;
  struct {
    UINT32 CurrSrcStr : 3;
    UINT32 reserve    : 14;
    UINT32 odpullEn   : 1;
    UINT32 odpulldir  : 1;
    UINT32 hsMode     : 1;
    UINT32 Reseve3    : 1;
    UINT32 vpMode     : 1;
    UINT32 Reseve2    : 2;
    UINT32 HYSCTL     : 2;
    UINT32 parkMode   : 1;
    UINT32 reseve1    : 5;
  }r;
} CONF_FAMILY;

typedef enum
{
  M0 =0,
  M1,
  M2,
  M3,
  M4,
  M5,
  M6,
  M7,
  M8,
  M9,
  M10,
  M11,
  M12,
  M13,
  M14,
  M15,
} ModeList;

typedef enum
{
  Line0 =0,
  Line1 =1,
  Line2 =2,
  Line3 =3,
  Line4 =4,
  Line5 =5,
  Line6 =6,
  Line7 =7,
  Line8 =8,
  Line9 =9,
  Line10 =10,
  Line11 =11,
  Line12 =12,
  Line13 =13,
  Line14 =14,
  Line15 =15,
} Int_Select;

typedef enum
{
  INT_DIS =0,
  Trig_Edge_Low =1,
  Trig_Edge_High =2,
  Trig_Edge_Both =3,
  Trig_Level =4,
} INT_Type;

typedef enum
{
  glitch_Disable = 0,
  En_EdgeDetect,
  En_RX_Data,
  En_Edge_RX_Data,
} Glitch_Cfg;

typedef enum
{
  Maskable,
  NonMaskable,
}mask;

typedef enum
{
  NORTH     = GPIO_MMIO_OFFSET_N,
  EAST      = GPIO_MMIO_OFFSET_E,
  SOUTHWEST = GPIO_MMIO_OFFSET_SW,
  SOUTHEAST = GPIO_MMIO_OFFSET_SE,
  VIRTUAL   = GPIO_MMIO_OFFSET_VIRT,
} coms;

typedef enum
{
  SMI = 1,
  SCI = 2,
} GPE_config;

//
// InvertRxTx 7:4
// 0 - No Inversion
// 1 - Inversion
// [0] RX Enable
// [1] TX Enable
// [2] RX Data
// [3] TX Data
//
typedef enum
{
  No_Inversion      = 0,
  Inv_RX_Enable     = 0x1,
  Inv_TX_Enable     = 0x2,
  Inv_RX_TX_Enable  = 0x3,
  Inv_RX_Data       = 0x4,
  Inv_RX_Data_TX_Enable = 0x6, 
  Inv_TX_Data       = 0x8,
} InvertRX_TX;

typedef union {
  UINT32 padCnf0;
  struct {
    UINT32 GPIORXState     : 1; // 0     GPIO RX State (GPIORXState).
    UINT32 GPIOTXState     : 1; // 1     GPIO TX State (GPIOTXState).
    UINT32 Reserved1       : 5; // 2-6   Reserved
    UINT32 Gpio_Light_Mode : 1; // 7     GPIO Light Mode
    UINT32 GPIOCfg         : 3; // 8-10  GPIO Config (GPIOCfg).
    UINT32 Reserved2       : 4; // 11-14 Reserved
    UINT32 GPIOEn          : 1; // 15    GPIO Enable (GPIOEn)
    UINT32 Pmode           : 4; // 16-19 Pad Mode (Pmode)
    UINT32 Term            : 4; // 20-23 Termination (Term)
    UINT32 RXTXEnCfg       : 2; // 24-25 RX/TX Enable Config (RXTXEnCfg)
    UINT32 GFCfg           : 2; // 26-27 Glitch Filter Config (GFCfg)
    UINT32 IntSel          : 4; // 28-31 Interrupt Select (IntSel)
  }r;
} CHV_CONF_PAD0;

typedef union {
  UINT32 padCnf1;
  struct {
    UINT32 IntWakeCfg  : 3; // 0-2   Interrupt and Wake Configuration (IntWakeCfg).
    UINT32 ODEn        : 1; // 3     Open Drain Enable (ODEn).
    UINT32 InvRXTX     : 4; // 4-7   Invert RX TX (InvRXTX)
    UINT32 Reserved    : 2; // 8-9   Reserved.
    UINT32 IOSTerm     : 2; // 10-11 I/O Standby Termination (IOSTerm)
    UINT32 IOSState    : 4; // 12-15 I/O Standby State (IOSState)
    UINT32 analogmuxen : 1; // 16    Analog Mux Enable (analogmuxen)
    UINT32 ODTEN       : 1; // 17    Reserved for On Die Termination Enable (ODTEN)
    UINT32 ODTUPDN     : 1; // 18    Reserved for On Die Termination Up/Down (ODTUPDN)
    UINT32 HSMODE      : 1; // 19    Reserved for High Speed mode (HSMODE)
    UINT32 VP18Mode    : 1; // 20    Reserved for 1.8 V Mode (VP18Mode)
    UINT32 VP15MODE    : 1; // 21    Reserved for 1.5V Mode (VP15MODE)
    UINT32 CLKNENB     : 1; // 22    Reserved for CLKNENB
    UINT32 AZAMODE     : 1; // 23    Reserved for AZAMODE
    UINT32 HYSCTL      : 2; // 24-25 Reserved for Hysteresis Control (HYSCTL)
    UINT32 PARKMODEENB : 1; // 26    Reserved for Parkmode Enable Bar (PARKMODEENB)
    UINT32 csen        : 1; // 27    Current Source Enable (csen)
    UINT32 svid_od_en  : 1; // 28    SVID 1V Open Drain Mode (svid_1v_od_en)
    UINT32 Reserved1   : 1; // 29    Reserved for Pad Configuration.
    UINT32 PadRstCfg   : 1; // 30    Pad Reset Config (PadRstCfg)
    UINT32 CfgLock     : 1; // 31    Configuration Lock (CfgLock).
  }r;
} CHV_CONF_PAD1;

  #ifdef EFI_DEBUG
   #define CHV_GPIO_PAD_CONF(pad_name, Mode_Select, Mode, GPIO_Config, GPIO_STATE, Gpio_Light_Mode, INT_Type, INT_S, Term_H_L, Open_Drain, Current_Source, Int_Mask, Glitch_Cfg, InvertRX_TX, WAKE_Mask,Wake_Mask_Bit, GPE, MMIO_Offset, Community ) { \
    ((((INT_S) != NA) ? ((UINT32) INT_S << 28) : 0) | (((Glitch_Cfg) != NA) ? (Glitch_Cfg << 26) : 0) | (((Term_H_L) != NA) ? (Term_H_L << 20) : 0) | (((Mode_Select) == GPIO) ? ((Mode << 16 ) | (1 << 15) ) : ( (Mode << 16 ))) | (((GPIO_Config) != NA) ? (GPIO_Config << 8 ) : 0) | (((Gpio_Light_Mode) != NA) ? (Gpio_Light_Mode << 7) : 0) | (((GPIO_STATE) == HIGH ) ? 2 : 0)), \
    ((((INT_S) != NA) ? ((UINT32) FOUR_BIT << 28) : 0) | (((Glitch_Cfg) != NA) ? (TWO_BIT << 26) : 0) | (((Term_H_L) != NA) ? (FOUR_BIT << 20) : 0) | (FIVE_BIT << 15) | (((GPIO_Config) != NA) ? (THREE_BIT << 8) : 0) | (((Gpio_Light_Mode) != NA) ? (ONE_BIT << 7) : 0) | (((GPIO_STATE) != NA ) ? ONE_BIT << 1 : 0)), \
    ((((Current_Source) != NA) ? (Current_Source << 27) : 0) | (((InvertRX_TX) != NA) ? InvertRX_TX << 4 : 0) | (((Open_Drain) != NA) ? Open_Drain << 3 : 0 ) | (((INT_Type) != NA) ? INT_Type : 0)), \
    ((((Current_Source) != NA) ? (ONE_BIT << 27) : 0) | (((InvertRX_TX) != NA) ? FOUR_BIT << 4 : 0) | (((Open_Drain) != NA) ? ONE_BIT << 3 : 0) | (((INT_Type) != NA) ? THREE_BIT : 0)), Community, (MMIO_Offset != NA) ? (IO_BASE_ADDRESS+Community +MMIO_Offset) : 0, pad_name, ((((GPE) != NA) ? (GPE << 0) : 0) | (((WAKE_Mask) != NA) ? (WAKE_Mask << 2) : 0) | (((Int_Mask) != NA) ? (Int_Mask << 3) : 0))| (((Wake_Mask_Bit) != NA) ? (Wake_Mask_Bit << 4) : (NA << 4))}
  #else
       #define CHV_GPIO_PAD_CONF(pad_name, Mode_Select, Mode, GPIO_Config, GPIO_STATE, Gpio_Light_Mode, INT_Type, INT_S, Term_H_L, Open_Drain, Current_Source, Int_Mask, Glitch_Cfg, InvertRX_TX, WAKE_Mask,Wake_Mask_Bit, GPE, MMIO_Offset, Community ) { \
    ((((INT_S) != NA) ? ((UINT32) INT_S << 28) : 0) | (((Glitch_Cfg) != NA) ? (Glitch_Cfg << 26) : 0) | (((Term_H_L) != NA) ? (Term_H_L << 20) : 0) | (((Mode_Select) == GPIO) ? ((Mode << 16 ) | (1 << 15) ) : ( (Mode << 16 ))) | (((GPIO_Config) != NA) ? (GPIO_Config << 8 ) : 0) |(((Gpio_Light_Mode) != NA) ? (Gpio_Light_Mode << 7) : 0) |  (((GPIO_STATE) == HIGH ) ? 2 : 0)), \
    ((((INT_S) != NA) ? ((UINT32) FOUR_BIT << 28) : 0) | (((Glitch_Cfg) != NA) ? (TWO_BIT << 26) : 0) | (((Term_H_L) != NA) ? (FOUR_BIT << 20) : 0) | (FIVE_BIT << 15) | (((GPIO_Config) != NA) ? (THREE_BIT << 8) : 0)  | (((Gpio_Light_Mode) != NA) ? (ONE_BIT << 7) : 0) |  (((GPIO_STATE) != NA ) ? ONE_BIT << 1 : 0)), \
    ((((Current_Source) != NA) ? (Current_Source << 27) : 0) | (((InvertRX_TX) != NA) ? InvertRX_TX << 4 : 0) | (((Open_Drain) != NA) ? Open_Drain << 3 : 0 ) | (((INT_Type) != NA) ? INT_Type : 0)), \
    ((((Current_Source) != NA) ? (ONE_BIT << 27) : 0) | (((InvertRX_TX) != NA) ? FOUR_BIT << 4 : 0) | (((Open_Drain) != NA) ? ONE_BIT << 3 : 0) | (((INT_Type) != NA) ? THREE_BIT : 0)), Community, (MMIO_Offset != NA) ? (IO_BASE_ADDRESS+Community +MMIO_Offset) : 0,((((GPE) != NA) ? (GPE << 0) : 0) | (((WAKE_Mask) != NA) ? (WAKE_Mask << 2) : 0) | (((Int_Mask) != NA) ? (Int_Mask << 3) : 0)) | (((Wake_Mask_Bit) != NA) ? (Wake_Mask_Bit << 4) : (NA << 4))}
  #endif

typedef union {
  UINT32 micsData;
  struct {
    UINT32 GPE_ENABLE         : 2; // 0  set if GPIO GPE is enable.
    UINT32 wake_able          : 1; // 2  set if GPIO Wake capable
    UINT32 intr_mask          : 1; // 3  set if GPIO interrupt masked
    UINT32 Wake_Mask_Position : 8; // 4  set the GPIO wake mask bit position
  }r;
} CHV_PAD_MISC;

typedef struct {
  CHV_CONF_PAD0 padConfg0;
  CHV_CONF_PAD0 padConfg0_changes;
  CHV_CONF_PAD1 padConfg1;
  CHV_CONF_PAD1 padConfg1_changes;
  UINT32        Community;
  UINT32        MMIO_ADDRESS;
#ifdef EFI_DEBUG
  CHAR16 *pad_name; // GPIO Pin Name for debug purpose
#endif
  CHV_PAD_MISC PAD_MISC;
} CHV_GPIO_PAD_INIT;

typedef union {
  UINT32 mics1;
  struct {
    UINT32 RCOMP_ENABLE : 1; // 0  set if rcomp is enable.
  }r;
} CHV_FAMILY_MISC;

typedef struct {
  CONF_FAMILY     confg;
  CONF_FAMILY     confg_change;
  CHV_FAMILY_MISC family_misc;
  UINT32          mmioAddr;
#ifdef EFI_DEBUG
  CHAR16 *family_name; // GPIO Family Name for debug purpose
#endif
} GPIO_CONF_FAMILY_INIT;

typedef union {
  UINT32 wake_;
  struct {
    UINT32 GPIO_ : 32; // 1  set if GPIO 1 wake enable.
  }r;
} GPIO_WAKE;

typedef union {
  UINT32 intr_;
  struct {
    UINT32 GPIO_ : 32; // 1  set if GPIO 1 wake enable.
  }r;
} GPIO_INTERRUPT;

typedef union {
  UINT32 rout_;
  struct {
    UINT32 GPIO_ROUT : 32;
  }r;
} GPIO_ROUT;

typedef union {
  UINT32 gpe0a_;
  struct {
    UINT32 GPIO_EN : 32; //Expand the structure if required.
  }r;
} GPE0a_EN;

typedef union {
  UINT32 smi_;
  struct {
    UINT32 GPIO_SMI : 32; //Expand the structure if required.
  }r;
} ALT_GPIO_SMI;

//PAD_CONF0_MMIO_ADDR  = IOBASE + COMMUNITY_BASE + 0x4400+0x400*Family# + 0x8*Pad#
  #ifdef EFI_DEBUG
     #define CHV_GPIO_FAMILY_INIT(family_name, PARKMODEENB, HYSCTL, VP18MODE, HSMODE, ODTUPDN, ODTEN, CurrSrcStr, rComp, familyNo, Community) {    ((((PARKMODEENB) != NA) ? (UINT32)PARKMODEENB << 26 : 0) | (((HYSCTL) != NA) ? HYSCTL << 24 : 0)       | (((VP18MODE) != NA) ? VP18MODE << 21 : 0) | (((HSMODE) != NA) ? HSMODE << 19 : 0) | (((ODTUPDN) != NA) ? ODTUPDN << 18 : 0) | (((ODTEN) != NA) ? ODTEN << 17 : 0) | (CurrSrcStr)), \
                                                                                                                                       ((((PARKMODEENB) != NA) ? (UINT32)ONE_BIT << 26 : 0)     | (((HYSCTL) != NA) ? TWO_BIT << 24 : 0)       | (((VP18MODE) != NA) ? ONE_BIT << 21 : 0) | (((HSMODE) != NA) ? ONE_BIT << 19 : 0) | (((ODTUPDN) != NA) ? ONE_BIT << 18 : 0) | (((ODTEN) != NA) ? ONE_BIT << 17 : 0) | (THREE_BIT)), \
                                                                                                                                       ((rComp == ENABLE) ? 1 : 0), (familyNo != NA) ? (IO_BASE_ADDRESS+Community +(0x80*familyNo)+0x1080) : 0, family_name }
  #else
     #define CHV_GPIO_FAMILY_INIT(family_name, PARKMODEENB, HYSCTL, VP18MODE, HSMODE, ODTUPDN, ODTEN, CurrSrcStr, rComp, familyNo, Community) {    ((((PARKMODEENB) != NA) ? (UINT32)PARKMODEENB << 26 : 0) | (((HYSCTL) != NA) ? HYSCTL << 24 : 0)       | (((VP18MODE) != NA) ? VP18MODE << 21 : 0) | (((HSMODE) != NA) ? HSMODE << 19 : 0) | (((ODTUPDN) != NA) ? ODTUPDN << 18 : 0) | (((ODTEN) != NA) ? ODTEN << 17 : 0) | (CurrSrcStr)), \
                                                                                                                                       ((((PARKMODEENB) != NA) ? (UINT32)ONE_BIT << 26 : 0)     | (((HYSCTL) != NA) ? TWO_BIT << 24 : 0)       | (((VP18MODE) != NA) ? ONE_BIT << 21 : 0) | (((HSMODE) != NA) ? ONE_BIT << 19 : 0) | (((ODTUPDN) != NA) ? ONE_BIT << 18 : 0) | (((ODTEN) != NA) ? ONE_BIT << 17 : 0) | (THREE_BIT)), \
                                                                                                                                       ((rComp == ENABLE) ? 1 : 0), (familyNo != NA) ? (IO_BASE_ADDRESS+Community +(0x80*familyNo)+0x1080) : 0 }
  #endif

//family_rcomp = IOBASE + COMMUNITY_BASE + 0x1080 + 0x20*family + rcompOffset

/*
  *      RCOMP REGISTERS
  */

typedef union {
  UINT32 rcomCtrl;
  struct {
    UINT32 InitCalValN     : 8; // 0-7   Initial Pull Down Value (InitCalValN)
    UINT32 InitCalValP     : 8; // 8-15  Initial Pull Up Value (InitCalValP)
    UINT32 RFSMStopCyc     : 4; // 16-19 RCOMP FSM Stop Cycles  (RFSMStopCyc)
    UINT32 RFSMStop        : 2; // 20-21 RCOMP FSM Stop Condition (RFSMStop)
    UINT32 ircintclkperiod : 2; // 22-23 IRC Internal Clock Period (ircintclkperiod)
    UINT32 ircfreq_select  : 2; // 24-25 IRCCLK Frequency Select (ircfreq_select)
    UINT32 reserved        : 3; // 26-28    reserved (reserved)
    UINT32 chicken_bit     : 1; // 29    RCOMP FSM Binary Chicken Bit (chicken_bit)
    UINT32 RcalStart       : 1; // 30    RCOMP Calibration Start  (RcalStart)
    UINT32 RFSMEn          : 1; // 31    RCOMP FSM Enable (RFSMEn)
  }r;
} CHV_RCOMP_CTRL;

typedef union {
  UINT32 rcompOffset;
  struct {
    UINT32 OffNStrVal   : 5; // 0-4   Pull Down Offset Strength Value (OffNStrVal).
    UINT32 OffNStrSign  : 1; // 5     Pull Down Offset Strength Sign (OffNStrSign).
    UINT32 Reserved     : 2; // 6-7   Reserved
    UINT32 OffPStrVal   : 5; // 8-12  Pull Up Offset Strength Value (OffPStrVal).
    UINT32 OffPStrSign  : 1; // 13    Pull Up Offset Strength Sign (OffPStrSign)
    UINT32 Reserved1    : 2; // 14-15 Reserved
    UINT32 OffNSlewVal  : 4; // 16-19 Pull Down Slew Offset (OffNSlewVal)
    UINT32 OffNSlewSign : 1; // 20    Pull Down Offset Slew Sign (OffNSlewSign)
    UINT32 Reserved3    : 3; // 21-23 Reserved
    UINT32 OffPSlewVal  : 4; // 24-27    Pull Up Slew Offset (OffPSlewVal)
    UINT32 OffPSlewSign : 1; // 28    Pull Up Offset Slew Sign (OffPSlewSign)
    UINT32 Reserved4    : 3; // 29-31    Reserved
  }r;
} CHV_RCOMP_OFFSET;

typedef union {
  UINT32 rcomOver;
  struct {
    UINT32 OvrNStrVal  : 8; // 0-7   Override Pull Down Strength Value (OvrNStrVal).
    UINT32 OvrPStrVal  : 8; // 8-15  Override Pull Up Strength Value (OvrPStrVal).
    UINT32 OvrNSlewVal : 4; // 16-19 Override N Slew Value (OvrNSlewVal).
    UINT32 OvrPSlewVal : 2; // 20-23 Override P Slew Value (OvrPSlewVal).
    UINT32 Reserved    : 6; // 24-29 Reserved.
    UINT32 OvrLoad     : 1; // 30    Override Load (OvrLoad)
    UINT32 OvrREn      : 1; // 31    Override RCOMP Enable (OvrREn)
  }r;
} CHV_RCOMP_OVERRIDE;

typedef union {
  UINT32 rcompValue;
  struct {
    UINT32 NStrVal  : 8; // 0-7   Pull Down Strength Value (NStrVal).
    UINT32 PStrVal  : 8; // 8-15  Pull Up Strength Value (PStrVal).
    UINT32 NSlewVal : 4; // 16-19 N Slew Value (NSlewVal)
    UINT32 PSlewVal : 4; // 20-23 P Slew Value (PSlewVal)
    UINT32 Reserved : 8; // 24-31 Reserved
  }r;
} CHV_RCOMP_VALUE;

typedef union {
  UINT32 secSAI;
  struct {
    UINT32 IA_Untrusted        : 1;
    UINT32 IA_Ucode            : 1;
    UINT32 IA_Smm              : 1;
    UINT32 ucode_NPP           : 1;
    UINT32 IA_Boot             : 1;
    UINT32 IA_Untrusted_5      : 1;
    UINT32 IA_Untrusted_6      : 1;
    UINT32 IA_XUcode           : 1;
    UINT32 Punit_Trusted       : 1;
    UINT32 SEC_Trusted         : 1;
    UINT32 Drm                 : 1;
    UINT32 FuseStrap_Puller    : 1;
    UINT32 Fuse_Provider       : 1;
    UINT32 Strap_Provider      : 1;
    UINT32 DFX_Untrusted       : 1;
    UINT32 DFX_Trusted         : 1;
    UINT32 PMC_Trusted         : 1;
    UINT32 DRNG                : 1;
    UINT32 ISH_t               : 1;
    UINT32 Device_Untrusted_19 : 1;
    UINT32 Device_Untrusted_20 : 1;
    UINT32 Device_Untrusted_21 : 1;
    UINT32 Device_Untrusted_22 : 1;
    UINT32 Device_Untrusted_23 : 1;
    UINT32 Device_Untrusted_24 : 1;
    UINT32 Device_Untrusted_25 : 1;
    UINT32 Device_Untrusted_26 : 1;
    UINT32 Device_Untrusted_27 : 1;
    UINT32 Device_Untrusted_28 : 1;
    UINT32 Device_Untrusted_29 : 1;
    UINT32 Device_Untrusted_30 : 1;
    UINT32 Device_Untrusted    : 1;
  }r;
} policy_access_reg;

typedef union {
  UINT32 familyRcompCnfg;
  struct {
    UINT32 NSlewVal     : 4; // 0-3   N Slew Value (NSlewVal).
    UINT32 PSlewVal     : 4; // 4-7   P Slew Value (PSlewVal).
    UINT32 NStaticLegEn : 1; // 8     N Static Leg Enable (NStaticLegEn)
    UINT32 PStaticLegEn : 1; // 9     P Static Leg Enable (PStaticLegEn)
    UINT32 reserved     : 6; // 10-15 reserved (reserved)
    UINT32 NStrVal      : 8; // 16-23 Pull Down Strength Value (NStrVal)
    UINT32 PStrVal      : 8; // 24-31 Pull Up Strength Value (PStrVal)
  }r;
} CHV_FAMILY_RCOMP_CNFG;

typedef struct {
  UINT32            Offset;
  policy_access_reg val_;
  UINT32            Community;
#ifdef EFI_DEBUG
  CHAR16 *family_name; // GPIO Family Name for debug purpose
#endif
} GPIO_SAI_INIT;

#ifdef EFI_DEBUG
    #define CHV_GPIO_SAI_INIT(family_name, Offset, value, Community) { Offset, value, Community, family_name}
#else
    #define CHV_GPIO_SAI_INIT(family_name, Offset, value, Community) { Offset, value, Community}
#endif

typedef struct {

#ifdef EFI_DEBUG
  CHAR16 *pad_name; // GPIO Pin Name for debug purpose
#endif

  GPIO_En       usage; // GPIO pin used as Native mode or GPI/GPO/GPIO mode
  GPO_D4        gpod4; // GPO default value
  GPIO_FUNC_NUM func;  // Function Number (F0~F7)
  INT_TYPE  int_type; // Edge or Level trigger, low or high active
  PULL_TYPE pull;     // Pull Up or Down
  UINT8     offset;   // Equal with (PCONF0 register offset >> 4 bits)
} GPIO_CONF_PAD_INIT;

#ifdef EFI_DEBUG
  #define GPIO_INIT_ITEM(pad_name, usage, gpod4, func, int_cap, int_type, pull, offset) {pad_name, usage, gpod4, func, /*int_cap,*/ TRIG_ ## int_type, P_ ## pull, offset}
#else
  #define GPIO_INIT_ITEM(pad_name, usage, gpod4, func, int_cap, int_type, pull, offset) {          usage, gpod4, func, /*int_cap,*/ TRIG_ ## int_type, P_ ## pull, offset}
#endif

typedef union {
  UINT32 dw;
  struct {
    UINT32 Func_Pin_Mux  : 3; // 0:2 Function of CFIO selection
    UINT32 ipslew        : 2; // 3:4 Pad (P) Slew Rate Controls PAD slew rate check Width
    UINT32 inslew        : 2; // 5:6 Pad (N) Slew Rate Controls PAD slew rate
    UINT32 Pull_assign   : 2; // 7:8 Pull assignment
    UINT32 Pull_strength : 2; // 9:10 Pull strength
    UINT32 Bypass_flop   : 1; // 11 Bypass flop
    UINT32 Filter_en     : 1; // 12 Filter Enable
    UINT32 Hist_ctrl     : 2; // 13:14 hysteresis control
    UINT32 Hist_enb      : 1; // 15 Hysteresis enable, active low
    UINT32 Delay_line    : 6; // 16:21 Delay line values - Delay values for input or output
    UINT32 Reserved      : 3; // 22:24 Reserved
    UINT32 TPE           : 1; // 25 Trigger Positive Edge Enable
    UINT32 TNE           : 1; // 26 Trigger Negative Edge Enable
    UINT32 Reserved2     : 3; // 27:29 Reserved
    UINT32 i1p5sel       : 1; // 30
    UINT32 IODEN         : 1; // 31 : Open Drain enable. Active high
  } r;
} PAD_CONF0;

typedef union {
  UINT32 dw;
  struct {
    UINT32 pad_val  : 1;  // 0 These registers are implemented as dual read/write with dedicated storage each.
    UINT32 ioutenb  : 1;  // 1 output enable
    UINT32 iinenb   : 1;  // 2 input enable
    UINT32 Reserved : 29; // 3:31 Reserved
  }r;
} PAD_VAL;

#pragma pack()

#define CONF_MMIO_ADDRESS(CommunityOffset, GpioNumber, PadXOffset)  (IO_BASE_ADDRESS + CommunityOffset + FAMILY0_PAD_REGS_OFF + \
                                                                     (FAMILY_PAD_REGS_SIZE * (GpioNumber / MAX_FAMILY_PAD_GPIO_NO)) + \
                                                                     (GPIO_REGS_SIZE * (GpioNumber % MAX_FAMILY_PAD_GPIO_NO)) + PadXOffset)

/**
  Set GPIO PAD control 0 and 1 registers for N/E/SW/SE GPIO communities

  @param[in] GPIOTable_Index          Begin Pin Number to start configuring in GPIO_Conf_Data table
  @param[in] GPIOTable_NumberofPins   Number of Pins to configure in GPIO_Conf_Data table
  @param[in] Gpio_Conf_Data           GPIO_CONF_PAD_INIT data array

**/
VOID
InternalGpioPADConfig (
  IN UINT32         Gpio_Pin_Begin_Num,
  IN UINT32         Gpio_Pin_Size,
  CHV_GPIO_PAD_INIT *Gpio_Conf_Data
  );

/**
  Set GPIO families register configuration for N/E/SW/SE GPIO communities

  @param[in] Gpio_Families_Mmio_Offset         GPIO_MMIO_OFFSET_SW / GPIO_MMIO_OFFSET_N / GPIO_MMIO_OFFSET_E / GPIO_MMIO_OFFSET_SE.
  @param[in] Gpio_Communities_Num              Families numbers to config for GPIO communities.
  @param[in] Gpio_Families_Data                GPIO_CONF_FAMILY_INIT data array for each GPIO family.

**/
VOID
InternalGpioFamiliesConfig (
  IN UINT32             Gpio_Families_Mmio_Offset,
  IN UINT32             Gpio_Communities_Num,
  GPIO_CONF_FAMILY_INIT *Gpio_Families_Data
  );

/**
  Set GPIO PAD SAI registers for N/E/SW/SE GPIO communities

  @param[in] SAI_Conf_Data    GPIO_SAI_INIT data array for each GPIO communities.
  @param[in] UINT32           Size of the table
**/
VOID
SaiSettingOfGpioFamilies (
  GPIO_SAI_INIT *SAI_Conf_Data,
  UINT32        familySize
  );

/**
  Function to get the Pin Pad0 MMIO offset

  @param[in] Community  NORTH-0x01
                        EAST-0x02
                        SOUTH_EAST-0x03
                        SOUTH_WEST-0x04
  @param[in] PinNum     Pin number.

  @retval    UINT32     Pin Mmio Offset
**/
UINT32
EFIAPI
GetPinMmioOffset (
  IN UINT32 Community,
  IN UINT32 PinNum
  );

/**
  Function to Read/Write Pad0 or Pad1 control register of given Pin from commnity

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Padx          This can be either Pad0 or Pad1, 0 - PAD0
  @param[in] ReadOrWrite   0 - Read and 1 - Write of Padx
  @param[in] PadxValue     Pinter to hold the Read/Write value.

**/
VOID
ReadWritePadXConf (
  IN UINT32     Community,
  IN UINT32     PinNum,
  IN BOOLEAN    Padx,
  IN BOOLEAN    ReadOrWrite,
  IN OUT UINT32 *PadxValue
  );

/**
  Function to configure pin to any one of GPIO fucntionalities for specified pin in the community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Direction     Gpio direction
                           GPIO - 0
                           GPO  - 1
                           GPI  - 2
                           HI-Z - 3
  @param[in] Value         Configure HIGH or LOW for GPO (Tx)
                           1-HIGH,0-Low
**/
VOID
SetGPIOMode (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN UINT8   Direction,
  IN BOOLEAN Value
  );

/**
  Function to get the GPIO direction configuration for specified pin in the community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    UINT8         The value for GPIO directin
                           GPIO-0
                           GPO-1
                           GPI-2
                           HI-Z-3
**/
UINT8
EFIAPI
GetGPIOMode (
  IN UINT32 Community,
  IN UINT32 PinNum
  );

/**
  Function to configure the specified pin to any one of native mode

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Mode          Native Mode - M0, M1, .., M15

  @retval    Boolean       0 - successful native mode configuration,1 - unsuccessful, Pad Could be locked
**/
BOOLEAN
EFIAPI
SetNativeMode (
  IN UINT32 Community,
  IN UINT32 PinOffset,
  IN UINT8  Mode
  );

/**
  Function to get the native mode number

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    Mode          Native Mode M0-0, M1-11, .., M15-15
                           Failure -  Value 0xFF.
**/
UINT8
EFIAPI
GetNativeMode (
  IN UINT32 Community,
  IN UINT32 PinOffset
  );

/**
  Function to configure GPO Tx State for specified pin in the community.

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Value         1-HIGH, 0-LOW

**/
VOID
GPIOSetTXValue (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN BOOLEAN Value
  );

/**
  Function to get the GPO Tx state of specified pin in the community.

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    Boolean       1-HIGH,0-LOW
**/
BOOLEAN
EFIAPI
GPIOGetTXValue (
  IN UINT32 Community,
  IN UINT32 PinNum
  );

/**
  Function to configure pin to any one of GPIO fucntionalities for specified pin in the community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Pullup        Enable pullup or pulldown
                           1-Pull Up
                           0-Pull Down
  @param[in] Value         Pull value
                           1-1K
                           2-5K
                           4-20K
  @retval    Boolean       0-Successful termination configured, 1-Unsuccessful could be due to unsupported value.
**/
BOOLEAN
EFIAPI
GPIOSetPadTerm (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN BOOLEAN PullUp,
  IN UINT8   Value
  );

/**
  Function to current pin termantaion value of specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[out] *PullValue    Pull value
                            1 - 1K
                            2 - 5K
                            4 - 20K

  @retval    Boolean        1-Pull Up,0-Pull Down
**/
BOOLEAN
GPIOGetPadTerm (
  IN UINT32    Community,
  IN UINT32    PinNum,
  IN OUT UINT8 *PullValue
  );

/**
  Function to Configure the Invert Rx/Tx Data and Rx/Tx Enable of specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[out] InvRxTxValue  0=No Inversion, 1=Inversion Enabled
                            [0] RX Enable
                            [1] TX Enable
                            [2] RX Data
                            [3] TX Data

**/
VOID
GPIOSetInvRxTx (
  IN UINT32 Community,
  IN UINT32 PinNum,
  IN UINT8  InvRxTxValue
  );

/**
  Function to get current the Invert Rx/Tx Data and Rx/Tx Enable configuration of specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number

  @retval     InvRxTxValue  0-No Inversion, 1-Inversion Enabled
                            [0] RX Enable
                            [1] TX Enable
                            [2] RX Data
                            [3] TX Data
**/
UINT8
EFIAPI
GPIOGetInvRxTx (
  IN UINT32 Community,
  IN UINT32 PinNum
  );

/**
  Function to configure the interrupt line and type for specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[in]  LineNum       0 - Interrupt Line 0
                            1 - Interrupt Line 1

                            15 = Interrupt Line 15
  @param[in]  Type          Type of interrrupt
                            0 = Interrupt/Wake Disable (Disabled and no Pad State toggles trigger the Wake/Interrupt logic)
                            1 = Falling Edge Detect Interrupt/Wake
                            2 = Rising Edge Detect Interrupt/Wake
                            3 = Falling or Rising Edge Detect Interrupt/Wake
                            4 = Level Interrupt/Wake (Bypass edge detect logic and Pad state is directly toggling the Wake/Interrupt logic)
**/
VOID
ConfigureGpioInt (
  IN UINT32 Community,
  IN UINT32 PinNum,
  IN UINT32 LineNum,
  IN UINT8  Type
  );

/**
  Function to lock the pad of specified Pin
  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Enable        1-Lock,0-UnLock

**/
VOID
LockUnlockGPIOPin (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN BOOLEAN Enable
  );

/**
  Function to Read the Pad Write Lock status
  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    Boolean       1-Locked,0-Unlocked
**/
BOOLEAN
EFIAPI
GetPinPadLockStatus (
  IN UINT32 Community,
  IN UINT32 PinNum
  );

/**
  Function to Get the current interrupt configure for specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[out] LineNum       Interrupt line number refer Int_Select from PlatfromInfo.h
                            0 = Interrupt Line 0
                            1 = Interrupt Line 1

                            15 = Interrupt Line 15
  @param[out]  Type         Type of interrrupt
                            0 = Interrupt/Wake Disable (Disabled and no Pad State toggles trigger the Wake/Interrupt logic)
                            1 = Falling Edge Detect Interrupt/Wake
                            2 = Rising Edge Detect Interrupt/Wake
                            3 = Falling or Rising Edge Detect Interrupt/Wake
                            4 = Level Interrupt/Wake (Bypass edge detect logic and Pad state is directly toggling the Wake/Interrupt logic)
                            4>  Unsupported value
**/
VOID
GetGpioIntConfig (
  IN UINT32    Community,
  IN UINT32    PinNum,
  IN OUT UINT8 *LineNum,
  IN OUT UINT8 *IntType
  );

/**
  Set particular GPIO pin as SMI / SCI

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  IntSel        Selected interrupt number to rout
  @param[in]  Type          Configure SMI / SCI ?

**/
VOID
GpioGPEConfig (
  IN UINT32 Community,
  IN UINT32 IntSel,
  IN UINT8  Type
  );

/**
  Set particular GPIO Wake Cable

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  CHV_PAD_MISC  Wake capable mask details
**/
VOID
ConfigureGPIOWake (
  IN UINT32       community,
  IN CHV_PAD_MISC Pad_misc
  );

#endif
