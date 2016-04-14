/** @file
  CFIO configuraution table for CHV platform.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _BOARD_CFIO_TABLE_H_
#define _BOARD_CFIO_TABLE_H_

#include <PlatformBaseAddresses.h>

//
// PAD registers offsets
//
#define GPIO_MMIO_OFFSET_SW        0x0000
#define GPIO_MMIO_OFFSET_N         0x8000
#define GPIO_MMIO_OFFSET_E         0x10000
#define GPIO_MMIO_OFFSET_SE        0x18000
#define GPIO_MMIO_OFFSET_VIRT      0x20000

#define CHV_GPIO_PAD_CONF0_OFFSET  0x0
#define CHV_GPIO_PAD_CONF1_OFFSET  0x4
#define NA            0xFF
#define LOW           0
#define HIGH          1

// Mask for Pad0 Config Register fields
#define PAD0_INT_SEL_MASK       0xF0000000 // [31:28] Interrupt Select
#define PAD0_GLITCH_FC_MASK     0x0C000000 // [27:26] Glitch Filter config
#define PAD0_RXTX_EN_CONF_MASK  0x03000000 // [25:24] RX/TX Enable Config
#define PAD0_RESERVED_MASK      0x000078FC // [14:11], [7:2] are reserved fields
#define PAD0_GPIO_TX_MASK       0x00000002 // BIT 1 GPIO TX State
#define PAD0_GPIO_RX_MASK       0x00000001 // Bit 0 GPIO RX State

typedef enum
{
    Native = 0xff,
    GPIO   = 0,       // Native, no need to set PAD_VALUE
    GPO    = 1,       // GPI, input only in PAD_VALUE
    GPI    = 2,       // GPO, output only in PAD_VALUE
    HI_Z   = 3,       // input output disabled
} GPIO_En;

typedef enum
{
    P_NONE  = 0,      // Pull None
    P_20K_L = 1,      // Pull Down  20K
    P_5K_L  = 2,      // Pull Down  5K
    P_1K_L  = 4,      // Pull Down  1K
    P_20K_H = 9,      // Pull Up 20K
    P_5K_H  = 10,     // Pull Up  5K
    P_1K_H  = 12      // Pull Up  1K
} PULL_TYPE;

typedef enum
{
    glitch_Disable = 0,
    En_EdgeDetect,
    En_RX_Data,
    En_Edge_RX_Data,
} Glitch_Cfg;

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
    No_Inversion      = 0,
    Inv_RX_Enable     = 0x1,
    Inv_TX_Enable     = 0x2,
    Inv_RX_TX_Enable  = 0x3,
    Inv_RX_Data       = 0x4,
    Inv_TX_Data       = 0x8,
} InvertRX_TX;

typedef union{
    UINT32 padCnf0;
    struct {
      UINT32 GPIORXState:1; // 0     GPIO RX State (GPIORXState).
      UINT32 GPIOTXState:1; // 1     GPIO TX State (GPIOTXState).
      UINT32 Reserved1:6;   // 2-7   Reserved
      UINT32 GPIOCfg:3;     // 8-10  GPIO Config (GPIOCfg).
      UINT32 Reserved2:4;   // 11-14 Reserved
      UINT32 GPIOEn:1;      // 15    GPIO Enable (GPIOEn)
      UINT32 Pmode:4;       // 16-19 Pad Mode (Pmode)
      UINT32 Term:4;        // 20-23 Termination (Term)
      UINT32 RXTXEnCfg:2;   // 24-25 RX/TX Enable Config (RXTXEnCfg)
      UINT32 GFCfg:2;       // 26-27 Glitch Filter Config (GFCfg)
      UINT32 IntSel:4;      // 28-31 Interrupt Select (IntSel)
    }r;
} CHV_CONF_PAD0;

typedef union{
    UINT32 padCnf1;
    struct {
      UINT32 IntWakeCfg:3;  // 0-2   Interrupt and Wake Configuration (IntWakeCfg).
      UINT32 ODEn:1;        // 3     Open Drain Enable (ODEn).
      UINT32 InvRXTX:4;     // 4-7   Invert RX TX (InvRXTX)
      UINT32 Reserved:2;    // 8-9   Reserved.
      UINT32 IOSTerm:2;     // 10-11 I/O Standby Termination (IOSTerm)
      UINT32 IOSState:4;    // 12-15 I/O Standby State (IOSState)
      UINT32 analogmuxen:1; // 16    Analog Mux Enable (analogmuxen)
      UINT32 ODTEN:1;       // 17    Reserved for On Die Termination Enable (ODTEN)
      UINT32 ODTUPDN:1;     // 18    Reserved for On Die Termination Up/Down (ODTUPDN)
      UINT32 HSMODE:1;      // 19    Reserved for High Speed mode (HSMODE)
      UINT32 VP18Mode:1;    // 20    Reserved for 1.8 V Mode (VP18Mode)
      UINT32 VP15MODE:1;    // 21    Reserved for 1.5V Mode (VP15MODE)
      UINT32 CLKNENB:1;     // 22    Reserved for CLKNENB
      UINT32 AZAMODE:1;     // 23    Reserved for AZAMODE
      UINT32 HYSCTL:2;      // 24-25 Reserved for Hysteresis Control (HYSCTL)
      UINT32 PARKMODEENB:1; // 26    Reserved for Parkmode Enable Bar (PARKMODEENB)
      UINT32 csen:1;        // 27    Current Source Enable (csen)
      UINT32 svid_od_en:1;  // 28    SVID 1V Open Drain Mode (svid_1v_od_en)
      UINT32 Reserved1:1;   // 29    Reserved for Pad Configuration.
      UINT32 PadRstCfg:1;   // 30    Pad Reset Config (PadRstCfg)
      UINT32 CfgLock:1;     // 31    Configuration Lock (CfgLock).
    }r;
} CHV_CONF_PAD1;

typedef struct {
    CHV_CONF_PAD0  Modified_PadConfg0;
    CHV_CONF_PAD1  Modified_PadConfg1;
    UINT32         Community;
    UINT32         MMIO_ADDRESS;
    CHAR16*        pad_name;
} CHV_CFIO_PAD_INIT;

#define CHV_CFIO_PAD_CONF(pad_name, GPIO_Config, GPIO_TX_STATE, Term_H_L, Open_Drain, Current_Source, Glitch_Cfg, InvertRX_TX, MMIO_Offset, Community ) { \
    {((((Glitch_Cfg) != NA) ? (Glitch_Cfg << 26) : 0) | (((Term_H_L) != NA) ? (Term_H_L << 20) : 0) | (1 << 15) | (((GPIO_Config) != NA) ? (GPIO_Config << 8 ): 0) | (((GPIO_TX_STATE) == HIGH ) ? 2 : 0))}, \
    {((((Current_Source) != NA) ? (Current_Source << 27) : 0) | (((InvertRX_TX) != NA) ? InvertRX_TX << 4 : 0) | (((Open_Drain) != NA) ? Open_Drain << 3 :0 ) | 0)}, \
  Community, (MMIO_Offset != NA) ? (IO_BASE_ADDRESS + Community + MMIO_Offset) : 0, pad_name}

/// Community Configuration
CHV_CFIO_PAD_INIT mBSW_CfioInitData[] =
{
// North Community
//               Group Pin#:     PAD Name     , GPIO Config, GPIO TX State, Term H/L, Open Drain, Current Source, Glitch Cfg, InvertRX_TX, MMIO Addr,  Community        Remarks
  CHV_CFIO_PAD_CONF  (L"N71: HV_DDI0_DDC_SCL ",    HI_Z   ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5458  ,    NORTH ),  //   DDI0_DDC_SCL
  CHV_CFIO_PAD_CONF  (L"N66: HV_DDI0_DDC_SDA ",    HI_Z   ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5430  ,    NORTH ),  //   DDI_0_DDC_SDA
  CHV_CFIO_PAD_CONF  (L"N67: HV_DDI2_DDC_SCL ",    HI_Z   ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5438  ,    NORTH ),  //   DBG_UART3_TXD  II HV_DDI2_DDC_SCL
  CHV_CFIO_PAD_CONF  (L"N62: HV_DDI2_DDC_SDA ",    HI_Z   ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5410  ,    NORTH ),  //   DBG_UART3_RXD II HV_DDI2_DDC_SDA
// East Community
//               Group Pin#:     PAD Name     , GPIO Config, GPIO TX State, Term H/L, Open Drain, Current Source, Glitch Cfg, InvertRX_TX, MMIO Addr,  Community,        Remarks
// SouthEast Community
//                Group Pin#:     PAD Name    , GPIO Config, GPIO TX State, Term H/L, Open Drain, Current Source, Glitch Cfg, InvertRX_TX, MMIO Addr,  Community          Remarks
  CHV_CFIO_PAD_CONF  (L"SE23: SDMMC1_CMD     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4840  ,  SOUTHEAST ),  //   SDMMC1_CMD
  CHV_CFIO_PAD_CONF  (L"SE17: SDMMC1_D0      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4810  ,  SOUTHEAST ),  //   SDMMC1_D_0
  CHV_CFIO_PAD_CONF  (L"SE24: SDMMC1_D1      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4848  ,  SOUTHEAST ),  //   SDMMC1_D_1
  CHV_CFIO_PAD_CONF  (L"SE20: SDMMC1_D2      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4828  ,  SOUTHEAST ),  //   SDMMC1_D_2
  CHV_CFIO_PAD_CONF  (L"SE26: SDMMC1_D3_CD_B ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4858  ,  SOUTHEAST ),  //   SDMMC1_D_3_CD_B
  CHV_CFIO_PAD_CONF  (L"SE67: MMC1_D4_SD_WE  ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5438  ,  SOUTHEAST ),  //   MMC1_D4_SD_WE II SoC_PCIE_SLOT3_RST_N
  CHV_CFIO_PAD_CONF  (L"SE65: MMC1_D5        ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5428  ,  SOUTHEAST ),  //   MMC1_D5 II SoC_PCIE_SLOT3_PWREN_N
  CHV_CFIO_PAD_CONF  (L"SE63: MMC1_D6        ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5418  ,  SOUTHEAST ),  //   MMC1_D6 II PCIE_SLOT1_RST_N
  CHV_CFIO_PAD_CONF  (L"SE08: MMC1_D7        ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5440  ,  SOUTHEAST ),  //   MMC1_D7 || PCIE_SLOT1_PWREN_N
  CHV_CFIO_PAD_CONF  (L"SE79: ILB_SERIRQ     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5820  ,  SOUTHEAST ),  //   ILB_SERIRQ
  CHV_CFIO_PAD_CONF  (L"SE47: MF_LPC_AD0     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5010  ,  SOUTHEAST ),  //   MF_LPC_AD0
  CHV_CFIO_PAD_CONF  (L"SE52: MF_LPC_AD1     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5038  ,  SOUTHEAST ),  //   MF_LPC_AD1
  CHV_CFIO_PAD_CONF  (L"SE45: MF_LPC_AD2     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5000  ,  SOUTHEAST ),  //   MF_LPC_AD2
  CHV_CFIO_PAD_CONF  (L"SE50: MF_LPC_AD3     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5028  ,  SOUTHEAST ),  //   MF_LPC_AD3
  CHV_CFIO_PAD_CONF  (L"SE46: LPC_CLKRUNB    ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5008  ,  SOUTHEAST ),  //   LPC_CLKRUNB
  CHV_CFIO_PAD_CONF  (L"SE48: LPC_FRAMEB     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5018  ,  SOUTHEAST ),  //   LPC_FRAMEB
  CHV_CFIO_PAD_CONF  (L"SE81: SDMMC3_CD_B    ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5830  ,  SOUTHEAST ),  //   SD_CARD_DET_N
  CHV_CFIO_PAD_CONF  (L"SE34: SDMMC3_CMD     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4C20  ,  SOUTHEAST ),  //   SD_CARD_CMD
  CHV_CFIO_PAD_CONF  (L"SE35: SDMMC3_D0      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4C28  ,  SOUTHEAST ),  //   SD_CARD_D0
  CHV_CFIO_PAD_CONF  (L"SE30: SDMMC3_D1      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4C00  ,  SOUTHEAST ),  //   SD_CARD_D1
  CHV_CFIO_PAD_CONF  (L"SE33: SDMMC3_D2      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4C18  ,  SOUTHEAST ),  //   SD_CARD_D2
  CHV_CFIO_PAD_CONF  (L"SE32: SDMMC3_D3      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4C10  ,  SOUTHEAST ),  //   SD_CARD_D3
  CHV_CFIO_PAD_CONF  (L"SE78: SDMMC3_PWR_EN_B",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5818  ,  SOUTHEAST ),  //   SD_CARD_PWRDN_N
  CHV_CFIO_PAD_CONF  (L"SE22: SDMMC2_CMD     ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4838  ,  SOUTHEAST ),  //   SDIO2_COMBO_CMD
  CHV_CFIO_PAD_CONF  (L"SE25: SDMMC2_D0      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4850  ,  SOUTHEAST ),  //   SDIO2_COMBO_D0
  CHV_CFIO_PAD_CONF  (L"SE18: SDMMC2_D1      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4818  ,  SOUTHEAST ),  //   SDIO2_COMBO_D1
  CHV_CFIO_PAD_CONF  (L"SE21: SDMMC2_D2      ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4830  ,  SOUTHEAST ),  //   SDIO2_COMBO_D2
  CHV_CFIO_PAD_CONF  (L"SE15: SDMMC2_D3_CD_B ",    HI_Z    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x4800  ,  SOUTHEAST ),   //   SDIO2_COMBO_D3
// South west Community
//                Group Pin#:     PAD Name    , GPIO Config, GPIO TX State, Term H/L, Open Drain, Current Source, Glitch Cfg, InvertRX_TX, MMIO Addr,  Community          Remarks
  CHV_CFIO_PAD_CONF  (L"SW65: I2C0_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5428  ,  SOUTHWEST ),  //   3rd party Sensor Card
  CHV_CFIO_PAD_CONF  (L"SW61: I2C0_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5408  ,  SOUTHWEST ),  //   3rd party Sensor Card
  CHV_CFIO_PAD_CONF  (L"SW63: I2C1_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5418  ,  SOUTHWEST ),  //   LP Audio codec
  CHV_CFIO_PAD_CONF  (L"SW60: I2C1_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5400  ,  SOUTHWEST ),  //   LP Audio codec
  CHV_CFIO_PAD_CONF  (L"SW66: I2C2_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5430  ,  SOUTHWEST ),  //   MIPI_CSI CAMERAS, FLASH
  CHV_CFIO_PAD_CONF  (L"SW62: I2C2_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5410  ,  SOUTHWEST ),  //   MIPI_CSI CAMERAS, FLASH
  CHV_CFIO_PAD_CONF  (L"SW67: I2C3_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5438  ,  SOUTHWEST ),  //   MIPI_CSI CAMERAS, FLASH
  CHV_CFIO_PAD_CONF  (L"SW64: I2C3_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5420  ,  SOUTHWEST ),  //   MIPI_CSI CAMERAS, FLASH
  CHV_CFIO_PAD_CONF  (L"SW50: I2C4_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5028  ,  SOUTHWEST ),  //   DDI2 DDC SCL
  CHV_CFIO_PAD_CONF  (L"SW46: I2C4_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5008  ,  SOUTHWEST ),  //   DDI2 DDC SDA
  CHV_CFIO_PAD_CONF  (L"SW48: I2C5_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5018  ,  SOUTHWEST ),  //   Touch/Digitizer
  CHV_CFIO_PAD_CONF  (L"SW45: I2C5_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5000  ,  SOUTHWEST ),  //   Touch/Digitizer
  CHV_CFIO_PAD_CONF  (L"SW51: I2C6_SCL       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5030  ,  SOUTHWEST ),  //   NFC attach point
  CHV_CFIO_PAD_CONF  (L"SW47: I2C6_SDA       ",      NA    ,      NA      ,  P_NONE ,     NA    ,       NA      ,    NA     ,     NA     ,  0x5010  ,  SOUTHWEST )   //   NFC attach point
};

#endif
