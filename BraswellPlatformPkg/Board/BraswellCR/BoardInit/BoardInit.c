/** @file
  Board Init driver.

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/BoardDetection.h>
#include <Ppi/ChvPlatformPolicyPpi.h>
#include <Protocol/PlatformGopPolicy.h>
#include <Library/GpioLib.h>

extern EFI_GUID  gBswCrImageGuid;

UINT8  mBSW_CR_SpdDataMemorySolderDown[] = {
  0x92,       //Byte 0
  0x12,
  0x0b,
  0x03,
  0x04,      //Byte 4
  0x11,
  0x02,
  0x02,
  0x03,
  0x52,
  0x01,
  0x08,
  0x0a,
  0x00,
  0xfe,
  0x00,
  0x69,
  0x78,
  0x69,
  0x3c,
  0x69,       //Byte20
  0x11,
  0x18,
  0x81,
  0x00,
  0x05,
  0x3c,
  0x3c,
  0x01,
  0x40,
  0x83,
  0x01,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,   //Byte40
  0,0,0,0,0,0,0,0,
  0x00,
  0x00,
  0x00,
  0x00,
  0x0f,
  0x11,
  0x22,
  0x00,
  0,0,0,0,0,0,0,0,    //Byte64
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0x00,               //Byte112
  0x00,
  0x00,
  0x00,
  0x00,
  0x80,
  0xad,
  0x01,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00           //Byte 215
};

CHV_GPIO_PAD_INIT  mBSW_CR_GpioInitData[] =
//                         PAD Name              Mode Select    Mode#  GPIO Config  GPIO TX State   GPIO Light       INT Type          INT Select     Term H/L          Open Drain    Current Source        Mask/Un-Mask        Glitch Cfg            InvertRX_TX             WAKE Cap    Wake_Mask_Bit    GPE    MMIO Addr    Community        Remarks
{ CHV_GPIO_PAD_CONF   (L"  N37:CX_PRDY_B         ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x4C38      ,  NORTH  )  ,  //  PRDY_B
  CHV_GPIO_PAD_CONF   (L"  N35:CX_PRDY_B_2       ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x4C28      ,  NORTH  )  ,  //  CX_PRDY_B_2
  CHV_GPIO_PAD_CONF   (L"  N39:CX_PREQ_B         ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x4858      ,  NORTH  )  ,  //  PREQ_B
  CHV_GPIO_PAD_CONF   (L"  N48:GP_CAMERASB00     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5018      ,  NORTH  )  ,  //  UF_CORE_VR_EN
  CHV_GPIO_PAD_CONF   (L"  N53:GP_CAMERASB01     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5040      ,  NORTH  )  ,  //  UF_IO_VR_EN
  CHV_GPIO_PAD_CONF   (L"  N46:GP_CAMERASB02     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5008      ,  NORTH  )  ,  //  UF_ANALOG_VR_EN
  CHV_GPIO_PAD_CONF   (L"  N51:GP_CAMERASB03     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5030      ,  NORTH  )  ,  //  WF_CORE_VR_EN
  CHV_GPIO_PAD_CONF   (L"  N56:GP_CAMERASB04     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5058      ,  NORTH  )  ,  //  WF_IO_VR_EN
  CHV_GPIO_PAD_CONF   (L"  N45:GP_CAMERASB05     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5000      ,  NORTH  )  ,  //  WF_ANALOG_VR_EN
  CHV_GPIO_PAD_CONF   (L"  N49:GP_CAMERASB06     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5020      ,  NORTH  )  ,  //  WF_VCM_EN
  CHV_GPIO_PAD_CONF   (L"  N54:GP_CAMERASB07     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5048      ,  NORTH  )  ,  //  FLASH_RESET_N
  CHV_GPIO_PAD_CONF   (L"  N47:GP_CAMERASB08     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5010      ,  NORTH  )  ,  //  FLASH_TRIGGER
  CHV_GPIO_PAD_CONF   (L"  N52:GP_CAMERASB09     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5038      ,  NORTH  )  ,  //  UF_CAM_RST_N
  CHV_GPIO_PAD_CONF   (L"  N50:GP_CAMERASB10     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5028      ,  NORTH  )  ,  //  WF_CAM_RST_N
  CHV_GPIO_PAD_CONF   (L"  N55:GP_CAMERASB11     ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x5050      ,  NORTH  )  ,  //  CAM_ACT_LED
  CHV_GPIO_PAD_CONF   (L"  N00:GPIO_DFX0         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x4400      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N03:GPIO_DFX1         ",  GPIO    ,  M1      ,  GPIO     ,  LOW        ,  LOW     ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  3   ,    NA  ,      0x4418      ,  NORTH  )  ,  //   GPS_WAKEUP
  CHV_GPIO_PAD_CONF   (L"  N07:GPIO_DFX2         ",  GPIO    ,  M1      ,  GPO      ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,     No_Inversion      ,      NA      ,  NA  ,    NA  ,      0x4438      ,  NORTH  )  ,  //  COMBO_GPS_RESET_N
  CHV_GPIO_PAD_CONF   (L"  N01:GPIO_DFX3         ",  GPIO    ,  M1      ,  GPI      ,  NA         ,  NA      ,  Trig_Edge_Low        ,       Line2    ,  P_NONE      ,          NA    ,          NA    ,      NonMaskable    ,     En_Edge_RX_Data    ,     Inv_TX_Enable     ,      NA      ,  NA  ,    NA  ,      0x4408      ,  NORTH  )  ,  //  SYS_SHDN_N
  CHV_GPIO_PAD_CONF   (L"  N05:GPIO_DFX4         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4428      ,  NORTH  )  ,  //  NGFF_MODEM_RESET_N
  CHV_GPIO_PAD_CONF   (L"  N04:GPIO_DFX5         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4420      ,  NORTH  )  ,  //  WWAN_PWR_EN
  CHV_GPIO_PAD_CONF   (L"  N08:GPIO_DFX6         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4440      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N07:GPIO_DFX7         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4410      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N06:GPIO_DFX8         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,        NA             ,       NA       ,      NA      ,          NA    ,          NA    ,        NA           ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4430      ,  NORTH  )  ,  //  SOC_NGFF_WIFI_EN
  CHV_GPIO_PAD_CONF   (L"  N19:GPIO_SUS1         ",  Native  ,  M6      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4820      ,  NORTH  )  ,  //  NGFF_WWAN_WAKE_GPIO_11_RD
  CHV_GPIO_PAD_CONF   (L"  N24:GPIO_SUS2         ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4848      ,  NORTH  )  ,  //  PROCHOT
  CHV_GPIO_PAD_CONF   (L"  N17:GPIO_SUS3         ",  GPIO    ,  M1      ,  GPI      ,  NA         ,  NA      ,  Trig_Edge_Low        ,       Line4    ,      NA      ,          NA    ,          NA    ,      NonMaskable    ,  En_Edge_RX_Data       ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4810      ,  NORTH  )  ,  //  I2S_IRQ_N
  CHV_GPIO_PAD_CONF   (L"  N22:GPIO_SUS4         ",  Native  ,  M6      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4838      ,  NORTH  )  ,  //  PMU_WAKE_LAN_B
  CHV_GPIO_PAD_CONF   (L"  N20:GPIO_SUS5         ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4828      ,  NORTH  )  ,  //  WiFI_32k_CLK
  CHV_GPIO_PAD_CONF   (L"  N71:HV_DDI0_DDC_SCL   ",  Native  ,  M2      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5458      ,  NORTH  )  ,  //  DDI1_HDMI_SOC_SCL
  CHV_GPIO_PAD_CONF   (L"  N66:HV_DDI0_DDC_SDA   ",  Native  ,  M2      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5430      ,  NORTH  )  ,  //  DDI1_HDMI_SOC_SDA
  CHV_GPIO_PAD_CONF   (L"  N61:HV_DDI0_HPD       ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,    P_NONE    ,          NA    ,          NA    ,         NA          ,          NA            ,    Inv_TX_Enable      ,      NA      ,  NA  ,    NA  ,      0x5408      ,  NORTH  )  ,  //  DDI0_HPD_N
  CHV_GPIO_PAD_CONF   (L"  N64:HV_DDI1_HPD       ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,    P_NONE    ,          NA    ,          NA    ,         NA          ,          NA            ,    Inv_TX_Enable      ,      NA      ,  NA  ,    NA  ,      0x5420      ,  NORTH  )  ,  //  DDI1_HPD_N
  CHV_GPIO_PAD_CONF   (L"  N67:HV_DDI2_DDC_SCL   ",  Native  ,  M3      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5438      ,  NORTH  )  ,  //  DBG_UART3_TXD
  CHV_GPIO_PAD_CONF   (L"  N62:HV_DDI2_DDC_SDA   ",  Native  ,  M3      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5410      ,  NORTH  )  ,  //  DBG_UART3_RXD
  CHV_GPIO_PAD_CONF   (L"  N65:PANEL0_BKLTCTL    ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5428      ,  NORTH  )  ,  //  DDI0_BKLT_CTRL
  CHV_GPIO_PAD_CONF   (L"  N60:PANEL0_BKLTEN     ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5400      ,  NORTH  )  ,  //  DDI0_BKLT_EN
  CHV_GPIO_PAD_CONF   (L"  N72:PANEL0_VDDEN      ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5460      ,  NORTH  )  ,  //  DDI0_VDD_EN
  CHV_GPIO_PAD_CONF   (L"  N63:PANEL1_BKLTCTL    ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x5418      ,  NORTH  )  ,  //  MATA_Adap_ctrl
  CHV_GPIO_PAD_CONF   (L"  N32:PROCHOT_B         ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C10      ,  NORTH  )  ,  //  VR_HOT_N
  CHV_GPIO_PAD_CONF   (L"  N16:SEC_GPIO_SUS10    ",  GPIO    ,  M1      ,  GPO      ,  LOW        ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4808      ,  NORTH  )  ,  //  I2C_RESERT_N_CONN
  CHV_GPIO_PAD_CONF   (L"  N21:SEC_GPIO_SUS11    ",  GPIO    ,  M1      ,  GPI      ,  NA         ,  NA      ,  Trig_Edge_Low        ,      Line5     ,      NA      ,          NA    ,          NA    ,      NonMaskable    ,     En_Edge_RX_Data    ,          NA           , UNMASK_WAKE  ,  15  ,    NA  ,      0x4830      ,  NORTH  )  ,  //  KBC_INT
  CHV_GPIO_PAD_CONF   (L"  N27:SEC_GPIO_SUS9     ",  GPIO    ,  M1      ,  GPO      ,  HIGH       ,  NA      ,         NA            ,       NA       ,    P_20K_H    ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4860      ,  NORTH  )  ,  //  LAN_LED1
  CHV_GPIO_PAD_CONF   (L"  N31:TCK               ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C08      ,  NORTH  )  ,  //  JTAG
  CHV_GPIO_PAD_CONF   (L"  N41:TDI               ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C58      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N39:TDO               ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C48      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N36:TDO_2             ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C30      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N34:TMS               ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C20      ,  NORTH  )  ,  //
  CHV_GPIO_PAD_CONF   (L"  N30:TRST_B            ",  Native  ,  M1      ,  NA       ,  NA         ,  NA      ,         NA            ,       NA       ,      NA      ,          NA    ,          NA    ,         NA          ,          NA            ,          NA           ,      NA      ,  NA  ,    NA  ,      0x4C00      ,  NORTH  )  ,  //
// East Community
//                       PAD Name               Mode Select    Mode#   GPIO Config  GPIO TX State  GPIO Light    INT Type             INT Select        Term H/L    Open Drain  Current Source  Mask/Un-Mask    Glitch Cfg    InvertRX_TX    WAKE Cap    Wake_Mask_Bit    GPE    MMIO Addr    Community        Remarks
  CHV_GPIO_PAD_CONF  (L"  E21: MF_ISH_GPIO_0    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,    P_5K_H   ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4830    ,  EAST  )  ,  //  acce_1
  CHV_GPIO_PAD_CONF  (L"  E18: MF_ISH_GPIO_1    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,    P_5K_H   ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4818    ,  EAST  )  ,  //  acce_2
  CHV_GPIO_PAD_CONF  (L"  E24: MF_ISH_GPIO_2    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,    P_5K_H   ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4848    ,  EAST  )  ,  //  compass
  CHV_GPIO_PAD_CONF  (L"  E15: MF_ISH_GPIO_3    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,    P_5K_H   ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4800    ,  EAST  )  ,  //  Gyro_1
  CHV_GPIO_PAD_CONF  (L"  E22: MF_ISH_GPIO_4    ",  GPIO      ,   M1   ,   GPI     ,  NA      ,      NA      ,   Trig_Level        ,     Line0       ,   P_20K_H   ,      NA  ,      NA  ,     NonMaskable   ,     En_RX_Data  ,Inv_RX_Data,  NA          ,    19   ,  NA    ,    0x4838   , EAST   )  ,  //  SENSOR_HUB_R_INT || Alt_1
  CHV_GPIO_PAD_CONF  (L"  E19: MF_ISH_GPIO_5    ",  GPIO      ,   M1   ,   GPO     ,  LOW     ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA       ,No_Inversion ,  NA          ,  NA     ,  NA    ,   0x4820    ,  EAST  )  ,  //  SENS_HUB_RST_N || Alt_2
  CHV_GPIO_PAD_CONF  (L"  E25: MF_ISH_GPIO_6    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4850    ,  EAST  )  ,  //  ALS
  CHV_GPIO_PAD_CONF  (L"  E16: MF_ISH_GPIO_7    ",  GPIO      ,   M1   ,   GPO     ,  HIGH    ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA       ,No_Inversion ,  NA          ,  NA     ,  NA    ,   0x4808    ,  EAST  )  ,  //  SENS_HUB_WAKE || Prox
  CHV_GPIO_PAD_CONF  (L"  E23: MF_ISH_GPIO_8    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4840    ,  EAST  )  ,  //  SAR
  CHV_GPIO_PAD_CONF  (L"  E20: MF_ISH_GPIO_9    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,    P_5K_H    ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4828    ,  EAST  )  ,  //  Gyro_2
  CHV_GPIO_PAD_CONF  (L"  E04: PMU_AC_PRESENT   ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4420    ,  EAST  )  ,  //  AC_PRESENT
  CHV_GPIO_PAD_CONF  (L"  E01:PMU_BATLOW_B      ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4408    ,  EAST  )  ,  //  PMU_BATLOW_N
  CHV_GPIO_PAD_CONF  (L"  E05:PMU_PLTRST_B      ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4428    ,  EAST  )  ,  //  PLT_RST_N
  CHV_GPIO_PAD_CONF  (L"  E08:PMU_PWRBTN_B      ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4440    ,  EAST  )  ,  //  SOC_PWRBTN_N
  CHV_GPIO_PAD_CONF  (L"  E03:PMU_SLP_S0IX_B    ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4418    ,  EAST  )  ,  //  SLP_S0IX_N
  CHV_GPIO_PAD_CONF  (L"  E00:PMU_SLP_S3_B      ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4400    ,  EAST  )  ,  //  SLP_S3_N
  CHV_GPIO_PAD_CONF  (L"  E09:PMU_SLP_S4_B      ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4448    ,  EAST  )  ,  //  SLP_S4_N
  CHV_GPIO_PAD_CONF  (L"  E06:PMU_SUSCLK        ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4430    ,  EAST  )  ,  //  SLEEP_CLK
  CHV_GPIO_PAD_CONF  (L"  E10:PMU_WAKE_B        ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,    P_1K_H    ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4450    ,  EAST  )  ,  //  PMU_WAKE_N
  CHV_GPIO_PAD_CONF  (L"  E11:PMU_WAKE_LAN_B    ",  GPIO      ,   M1   ,   GPI     ,  NA      ,      NA      ,  Trig_Edge_Low      ,       Line2     ,      NA      ,      NA  ,      NA  ,     NonMaskable   ,  En_Edge_RX_Data ,  Inv_RX_Data      ,  UNMASK_WAKE ,  11     ,  NA    ,   0x4458    ,  EAST  )  ,  //  PMU_WAKE_LAN_N
  CHV_GPIO_PAD_CONF  (L"  E02:SUS_STAT_B        ",  Native    ,   M1   ,   NA      ,  NA      ,      NA      ,         NA          ,       NA        ,      NA      ,      NA  ,      NA  ,        NA         ,     NA           ,  NA      ,  NA          ,  NA     ,  NA    ,   0x4410    ,  EAST  )  ,  //  TBD
// South East Community
//                           PAD Name            Mode Select    Mode#    GPIO Config     GPIO TX State     GPIO Light    INT Type           INT Select       Term H/L           Open Drain       Current Source     Mask/Un-Mask             Glitch Cfg                 InvertRX_TX    WAKE Cap    Wake_Mask_Bit        GPE    MMIO Addr    Community        Remarks
  CHV_GPIO_PAD_CONF    (L"  SE16:SDMMC1_CLK       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4808  ,  SOUTHEAST  )  ,  //  SDMMC1_CLK
  CHV_GPIO_PAD_CONF    (L"  SE23:SDMMC1_CMD       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4840  ,  SOUTHEAST  )  ,  //  SDMMC1_CMD
  CHV_GPIO_PAD_CONF    (L"  SE17:SDMMC1_D0        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,    P_20K_H   ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4810  ,  SOUTHEAST  )  ,  //  SDMMC1_D_0
  CHV_GPIO_PAD_CONF    (L"  SE24:SDMMC1_D1        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4848  ,  SOUTHEAST  )  ,  //  SDMMC1_D_1
  CHV_GPIO_PAD_CONF    (L"  SE20:SDMMC1_D2        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4828  ,  SOUTHEAST  )  ,  //  SDMMC1_D_2
  CHV_GPIO_PAD_CONF    (L"  SE26:SDMMC1_D3_CD_B   ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4858  ,  SOUTHEAST  )  ,  //  SDMMC1_D_3_CD_B
  CHV_GPIO_PAD_CONF    (L"  SE67:MMC1_D4_SD_WE    ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5438  ,  SOUTHEAST  )  ,  //  MMC1_D4
  CHV_GPIO_PAD_CONF    (L"  SE65:MMC1_D5          ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5428  ,  SOUTHEAST  )  ,  //  MMC1_D5
  CHV_GPIO_PAD_CONF    (L"  SE63:MMC1_D6          ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5418  ,  SOUTHEAST  )  ,  //  MMC1_D6
  CHV_GPIO_PAD_CONF    (L"  SE68:MMC1_D7          ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5440  ,  SOUTHEAST  )  ,  //  MMC1_D7
  CHV_GPIO_PAD_CONF    (L"  SE84:MC1_RCLK         ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5448  ,  SOUTHEAST  )  ,  //  SDMMC1_RCLK
  CHV_GPIO_PAD_CONF    (L"  SE77:GPIO_ALERT       ",  GPIO     ,  M1      ,  GPI  ,  NA     ,          NA           ,  Trig_Level          ,       Line1     ,      NA      ,          NA      ,          NA      ,      NonMaskable    ,       En_RX_Data       ,     Inv_RX_Data       ,  NA  ,          46           ,  NA    ,  0x5810  ,  SOUTHEAST  )  ,  //  TCH_PADCONN_INT_N
  CHV_GPIO_PAD_CONF    (L"  SE79:ILB_SERIRQ       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5820  ,  SOUTHEAST  )  ,  //  ILB_SERIRQ
  CHV_GPIO_PAD_CONF    (L"  SE51:MF_LPC_CLKOUT0   ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5030  ,  SOUTHEAST  )  ,  //  MF_LPC_CLKOUT0
  CHV_GPIO_PAD_CONF    (L"  SE47:MF_LPC_AD0       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5010  ,  SOUTHEAST  )  ,  //  MF_LPC_AD0
  CHV_GPIO_PAD_CONF    (L"  SE52:MF_LPC_AD1       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5038  ,  SOUTHEAST  )  ,  //  MF_LPC_AD1
  CHV_GPIO_PAD_CONF    (L"  SE45:MF_LPC_AD2       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5000  ,  SOUTHEAST  )  ,  //  MF_LPC_AD2
  CHV_GPIO_PAD_CONF    (L"  SE50:MF_LPC_AD3       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5028  ,  SOUTHEAST  )  ,  //  MF_LPC_AD3
  CHV_GPIO_PAD_CONF    (L"  SE46:LPC_CLKRUNB      ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5008  ,  SOUTHEAST  )  ,  //  LPC_CLKRUNB
  CHV_GPIO_PAD_CONF    (L"  SE48:LPC_FRAMEB       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5018  ,  SOUTHEAST  )  ,  //  LPC_FRAMEB
  CHV_GPIO_PAD_CONF    (L"  SE00:MF_PLT_CLK0      ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4400  ,  SOUTHEAST  )  ,  //  Camera1_clock
  CHV_GPIO_PAD_CONF    (L"  SE02:MF_PLT_CLK1      ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4410  ,  SOUTHEAST  )  ,  //  Camera2_clock
  CHV_GPIO_PAD_CONF    (L"  SE07:MF_PLT_CLK2      ",  GPIO     ,  M1      ,  GPI  ,  NA     ,          LOW          ,  Trig_Level          ,       Line2     ,      NA      ,          NA      ,          NA      ,      NonMaskable    ,       En_RX_Data       ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4438  ,  SOUTHEAST  )  ,  //  GPS_HOSTREQ
  CHV_GPIO_PAD_CONF    (L"  SE04:MF_PLT_CLK3      ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4420  ,  SOUTHEAST  )  ,  //  I2S_MCLK
  CHV_GPIO_PAD_CONF    (L"  SE03:MF_PLT_CLK4      ",  Native   ,  M3      ,  NA   ,  NA     ,          LOW          ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4418  ,  SOUTHEAST  )  ,  //  I2C0_ISF_SDA
  CHV_GPIO_PAD_CONF    (L"  SE06:MF_PLT_CLK5      ",  Native   ,  M3      ,  NA   ,  NA     ,          LOW          ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4430  ,  SOUTHEAST  )  ,  //  COMBO_GPS_RESET_N
  CHV_GPIO_PAD_CONF    (L"  SE76:PMU_RSTBUTTON_B  ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5808  ,  SOUTHEAST  )  ,  //  SOC_RESETBTN_N
  CHV_GPIO_PAD_CONF    (L"  SE83:SUSPWRDNACK      ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5840  ,  SOUTHEAST  )  ,  //  SUSPWRDNACK
  CHV_GPIO_PAD_CONF    (L"  SE05:PWM0             ",  GPIO     ,  M1      ,  GPO  ,  HIGH   ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4428  ,  SOUTHEAST  )  ,  //  HAPTIC0,
  CHV_GPIO_PAD_CONF    (L"  SE01:PWM1             ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4408  ,  SOUTHEAST  )  ,  //  WIFI_PCIE_RST_N
  CHV_GPIO_PAD_CONF    (L"  SE85:SDMMC3_1P8_EN    ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5850  ,  SOUTHEAST  )  ,  //  SD_CARD_PWR_EN
  CHV_GPIO_PAD_CONF    (L"  SE81:SDMMC3_CD_B      ",  GPIO     ,  M1      ,  GPI  ,  NA     ,          NA           ,  Trig_Level          ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,    En_Edge_RX_Data     ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5830  ,  SOUTHEAST  )  ,  //  SD_CARD_DET_N
  CHV_GPIO_PAD_CONF    (L"  SE31:SDMMC3_CLK       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4C08  ,  SOUTHEAST  )  ,  //  SD_CARD_CLK
  CHV_GPIO_PAD_CONF    (L"  SE34:SDMMC3_CMD       ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4C20  ,  SOUTHEAST  )  ,  //  SD_CARD_CMD
  CHV_GPIO_PAD_CONF    (L"  SE35:SDMMC3_D0        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4C28  ,  SOUTHEAST  )  ,  //  SD_CARD_D0
  CHV_GPIO_PAD_CONF    (L"  SE30:SDMMC3_D1        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4C00  ,  SOUTHEAST  )  ,  //  SD_CARD_D1
  CHV_GPIO_PAD_CONF    (L"  SE33:SDMMC3_D2        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4C18  ,  SOUTHEAST  )  ,  //  SD_CARD_D2
  CHV_GPIO_PAD_CONF    (L"  SE32:SDMMC3_D3        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x4C10  ,  SOUTHEAST  )  ,  //  SD_CARD_D3
  CHV_GPIO_PAD_CONF    (L"  SE78:SDMMC3_PWR_EN_B  ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5818  ,  SOUTHEAST  )  ,  //  SD_CARD_PWRDN_N
  CHV_GPIO_PAD_CONF    (L"  SE62:SPI1_CLK         ",  GPIO     ,  M1      ,  GPI  ,  NA     ,          NA           ,  Trig_Edge_Low       ,       Line4     ,      NA      ,          NA      ,          NA      ,      NonMaskable    ,  En_Edge_RX_Data       ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5410  ,  SOUTHEAST  )  ,  //  SOC_THERM_SENSOR_ALERT_N
  CHV_GPIO_PAD_CONF    (L"  SE61:SPI1_CS0_B       ",  GPIO     ,  M1      ,  GPO  ,  HIGH   ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5408  ,  SOUTHEAST  )  ,  //  CS_WAKE_N
  CHV_GPIO_PAD_CONF    (L"  SE66:SPI1_CS1_B       ",  GPIO     ,  M1      ,  GPO  ,  HIGH   ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5430  ,  SOUTHEAST  )  ,  //  SPI_CS for NFC
  CHV_GPIO_PAD_CONF    (L"  SE60:SPI1_MISO        ",  GPIO     ,  M1      ,  GPI  ,  NA     ,          NA           ,  Trig_Edge_Low       ,       Line5     ,    P_20K_H   ,     _ENABLE      ,          NA      ,      NonMaskable    ,  En_RX_Data            ,      Inv_RX_Enable    ,  UNMASK_WAKE  , 34           ,  NA    ,  0x5400  ,  SOUTHEAST  )  ,  //  PMIC_IRQ_1P8
  CHV_GPIO_PAD_CONF    (L"  SE64:SPI1_MOSI        ",  GPIO     ,  M1      ,  GPO  ,  HIGH   ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5420  ,  SOUTHEAST  )  ,  //  WIFI_RST_GPIO
  CHV_GPIO_PAD_CONF    (L"  SE80:USB_OC0_B        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5828  ,  SOUTHEAST  )  ,  //  USB2_OC0_N
  CHV_GPIO_PAD_CONF    (L"  SE75:USB_OC1_B        ",  Native   ,  M1      ,  NA   ,  NA     ,          NA           ,         NA           ,       NA        ,      NA      ,          NA      ,          NA      ,         NA          ,          NA            ,          NA           ,  NA  ,          NA           ,  NA    ,  0x5800  ,  SOUTHEAST  )  ,  //  USB3_OC_N

// South west Community
//                        PAD Name              Mode Select  Mode#  GPIO Config GPIO TX State     GPIO Light    INT Type              INT Select     Term H/L            Open Drain    Current Source           Mask/Un-Mask               Glitch Cfg           InvertRX_TX      WAKE Cap    Wake_Mask_Bit    GPE    MMIO Addr    Community        Remarks
  CHV_GPIO_PAD_CONF    (L"  SW02:FST_SPI_CLK      ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4410  ,  SOUTHWEST  )  ,  //      FAST_SPI_CLK
  CHV_GPIO_PAD_CONF    (L"  SW06:FST_SPI_CS0_B    ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4430  ,  SOUTHWEST  )  ,  //      FST_SPI_CS0_N( SPI NOR)
  CHV_GPIO_PAD_CONF    (L"  SW04:FST_SPI_CS1_B    ",  GPIO     ,  M1  ,  GPO    ,  HIGH   ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4420  ,  SOUTHWEST  )  ,  //      RF_KILL_GPS_N_1P8
  CHV_GPIO_PAD_CONF    (L"  SW07:FST_SPI_CS2_B    ",  GPIO     ,  M1  ,  GPO    ,  LOW    ,    NA          ,         NA            ,       NA        ,    P_5K_L    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4438  ,  SOUTHWEST  )  ,  //      TCH_PNL_MODE_SEL
  CHV_GPIO_PAD_CONF    (L"  SW01:FST_SPI_D0       ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4408  ,  SOUTHWEST  )  ,  //      FST_SPI_D0
  CHV_GPIO_PAD_CONF    (L"  SW05:FST_SPI_D1       ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4428  ,  SOUTHWEST  )  ,  //      FST_SPI_D1
  CHV_GPIO_PAD_CONF    (L"  SW00:FST_SPI_D2       ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4400  ,  SOUTHWEST  )  ,  //      FST_SPI_D2
  CHV_GPIO_PAD_CONF    (L"  SW03:FST_SPI_D3       ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4418  ,  SOUTHWEST  )  ,  //      FST_SPI_D3
  CHV_GPIO_PAD_CONF    (L"  SW30:MF_HDA_CLK       ",  Native   ,  M2  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,     No_Inversion      ,  NA  ,  NA  ,   NA   ,  0x4C00  ,  SOUTHWEST  )  ,  //      MF_HDA_CLK II GP_SSP_0_I2S_TXD
  CHV_GPIO_PAD_CONF    (L"  SW37:MF_HDA_DOCKENB   ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C38  ,  SOUTHWEST  )  ,  //      I2S_2_RXD_R_AICO(AIC)
  CHV_GPIO_PAD_CONF    (L"  SW34:MF_HDA_DOCKRSTB  ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C20  ,  SOUTHWEST  )  ,  //      I2S_2_TXD_R_AICO (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW31:MF_HDA_RSTB      ",  Native   ,  M2  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C08  ,  SOUTHWEST  )  ,  //      AUD_LINK_RST_N || I2S_0_CLK_R_AICO (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW32:MF_HDA_SDI0      ",  Native   ,  M2  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C10  ,  SOUTHWEST  )  ,  //      AUD_LINK_SDI0 (ALC282) || I2S_2_CLK_R_AICO (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW36:MF_HDA_SDI1      ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C30  ,  SOUTHWEST  )  ,  //      I2S_2_FS_R_AICO
  CHV_GPIO_PAD_CONF    (L"  SW33:MF_HDA_SDO       ",  Native   ,  M2  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C18  ,  SOUTHWEST  )  ,  //      AUD_LINK_SDO_R||I2S_0_RXD_R_AICO (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW35:MF_HDA_SYNC      ",  Native   ,  M2  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4C28  ,  SOUTHWEST  )  ,  //      AUD_LINKSYNC_R|| I2S_0_FS_R_AICO (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW22:UART2_CTS_B      ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_NONE    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4838  ,  SOUTHWEST  )  ,  //      UART2_GPS_CTS (GPS)|| UART2_GPS_CTS_CONN (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW19:UART2_RTS_B      ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_NONE    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4820  ,  SOUTHWEST  )  ,  //      UART2_GPS_RTS (GPS)|| UART2_GPS_RTS_CONN (AIC)
  CHV_GPIO_PAD_CONF    (L"  SW17:UART2_RXD        ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_NONE    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4810  ,  SOUTHWEST  )  ,  //      UART2_GPS_RXD (GPS)|| UART2_GPS_RXD_CONN(AIC)
  CHV_GPIO_PAD_CONF    (L"  SW21:UART2_TXD        ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_NONE    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x4830  ,  SOUTHWEST  )  ,  //      UART2_GPS_TXD(GPS)|| UART2_GPS_TXD_CONN(AIC)
  CHV_GPIO_PAD_CONF    (L"  SW65:I2C0_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5428  ,  SOUTHWEST  )  ,  //      3rd party Sensor Card
  CHV_GPIO_PAD_CONF    (L"  SW61:I2C0_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5408  ,  SOUTHWEST  )  ,  //      3rd party Sensor Card
  CHV_GPIO_PAD_CONF    (L"  SW63:I2C1_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5418  ,  SOUTHWEST  )  ,  //      PMIC
  CHV_GPIO_PAD_CONF    (L"  SW60:I2C1_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5400  ,  SOUTHWEST  )  ,  //      PMIC
  CHV_GPIO_PAD_CONF    (L"  SW66:I2C2_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5430  ,  SOUTHWEST  )  ,  //      MIPI_CSI CAMERAS, FLASH
  CHV_GPIO_PAD_CONF    (L"  SW62:I2C2_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5410  ,  SOUTHWEST  )  ,  //      MIPI_CSI CAMERAS, FLASH
  CHV_GPIO_PAD_CONF    (L"  SW67:I2C3_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5438  ,  SOUTHWEST  )  ,  //      MIPI_CSI CAMERAS, FLASH
  CHV_GPIO_PAD_CONF    (L"  SW64:I2C3_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5420  ,  SOUTHWEST  )  ,  //      MIPI_CSI CAMERAS, FLASH
  CHV_GPIO_PAD_CONF    (L"  SW50:I2C4_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5028  ,  SOUTHWEST  )  ,  //      I2C Audio | Touch PAD
  CHV_GPIO_PAD_CONF    (L"  SW46:I2C4_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5008  ,  SOUTHWEST  )  ,  //      I2C Audio | Touch PAD
  CHV_GPIO_PAD_CONF    (L"  SW48:I2C5_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5018  ,  SOUTHWEST  )  ,  //      Touch Panel
  CHV_GPIO_PAD_CONF    (L"  SW45:I2C5_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5000  ,  SOUTHWEST  )  ,  //      Touch Panel
  CHV_GPIO_PAD_CONF    (L"  SW51:I2C6_SCL         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5030  ,  SOUTHWEST  )  ,  //      INA Decice
  CHV_GPIO_PAD_CONF    (L"  SW47:I2C6_SDA         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,    P_1K_H    ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5010  ,  SOUTHWEST  )  ,  //      INA Device
  CHV_GPIO_PAD_CONF    (L"  SW90:PCIE_CLKREQ0B    ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5C00  ,  SOUTHWEST  )  ,  //      RTL8111G (CLKREQ_N)
  CHV_GPIO_PAD_CONF    (L"  SW91:PCIE_CLKREQ1B    ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5C08  ,  SOUTHWEST  )  ,  //      NGFF(CLKREQ_N)
  CHV_GPIO_PAD_CONF    (L"  SW93:PCIE_CLKREQ2B    ",  GPIO     ,  M1  ,  GPO    ,  HIGH   ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5C18  ,  SOUTHWEST  )  ,  //      NGFF_WWAN_RF_KILL_1P8_N (Active low)
  CHV_GPIO_PAD_CONF    (L"  SW95:PCIE_CLKREQ3B    ",  Native   ,  M2  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5C28  ,  SOUTHWEST  )  ,  //      Tied to Micro SD
  CHV_GPIO_PAD_CONF    (L"  SW75:SATA_GP0         ",  GPIO     ,  M1  ,  GPO    ,  LOW    ,    NA          ,         NA            ,       NA        ,       NA     ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5800  ,  SOUTHWEST  )  ,  //      TOUCH_PNL_RST_N
  CHV_GPIO_PAD_CONF    (L"  SW76:SATA_GP1         ",  GPIO     ,  M1  ,  GPI    ,  NA     ,    NA          ,     Trig_Level        ,       Line0     ,     P_1K_H   ,          NA      ,          NA          ,       NonMaskable    ,          NA            ,     Inv_RX_Data       ,  NA  ,  41  ,   NA   ,  0x5808  ,  SOUTHWEST  )  ,  //      TOUCH_INT_N
  CHV_GPIO_PAD_CONF    (L"  SW78:SATA_GP2         ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5818  ,  SOUTHWEST  )  ,  //      SATA_DEVSLP_R
  CHV_GPIO_PAD_CONF    (L"  SW80:SATA_GP3         ",  Native   ,  M1  ,  GPIO   ,  HIGH   ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5828  ,  SOUTHWEST  )  ,  //      eMMC_RST_N
  CHV_GPIO_PAD_CONF    (L"  SW77:SATA_LEDN        ",  Native   ,  M1  ,  NA     ,  NA     ,    NA          ,         NA            ,       NA        ,      NA      ,          NA      ,          NA          ,          NA          ,          NA            ,          NA           ,  NA  ,  NA  ,   NA   ,  0x5810  ,  SOUTHWEST  )  ,  //      SATA_LED_N
};



EFI_STATUS
GetBoardFabIdsKsc (
  OUT UINT16                    *BoardFabIds
  );

EFI_STATUS
GetBoradFabIdsGpio (
  OUT UINT8 *BoardId,
  OUT UINT8 *FabId
  );

EFI_STATUS
EFIAPI
BraswellCRBoardDetectionCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mBraswellCRDetectionStartNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardDetectionStartPpiGuid,
  BraswellCRBoardDetectionCallback
};

static EFI_PEI_PPI_DESCRIPTOR mBraswellCRDetectedPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardDetectedPpiGuid,
  NULL
};

EFI_STATUS
EFIAPI
BraswellCRBoardDetectionCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_PLATFORM_INFO_HOB            PlatformInfoHob;
  UINTN                            DataSize;
  UINT16                           BoardFabIds;
  UINT8                            BoardId;
  UINT8                            FabId;
  EFI_STATUS                       Status;
  VOID                             *Instance;

  Status = PeiServicesLocatePpi (
             &gBoardDetectedPpiGuid,
             0,
             NULL,
             &Instance
             );
  if (!EFI_ERROR(Status)) {
    return EFI_SUCCESS;
  }

  DEBUG ((EFI_D_INFO, "BraswellCRDetectionCallback\n"));
    
  ZeroMem (&PlatformInfoHob, sizeof (PlatformInfoHob));

  Status = GetBoardFabIdsKsc (&BoardFabIds);
  if (Status == EFI_SUCCESS) {
    FabId = (UINT8) BoardFabIds & 0x07;
    BoardId   = (UINT8) ((BoardFabIds & 0x0F00) >> 8);
    DEBUG ((EFI_D_INFO,  "BoardID detected = %d\n", BoardId));
    if ((BoardId == 0x07)|| (BoardId == 0x06)) {
      PlatformInfoHob.BoardId    = BOARD_ID_BSW_CR;
      PlatformInfoHob.BoardSvidConfig = BSW_I2C_PMIC_CONFIG;
      DEBUG ((EFI_D_INFO,  "I'm BrasWell CR \n\n"));
      PlatformInfoHob.MemCfgID   = 0;
      DEBUG ((EFI_D_INFO,  "PlatformInfoHob->MemCfgID= 0x%x\n", PlatformInfoHob.MemCfgID));
      PlatformInfoHob.FABID = FabId;
      DEBUG ((EFI_D_INFO,  "PlatformInfoHob->FABID  = 0x%x\n", FabId ));
      PlatformInfoHob.PlatformFlavor = FlavorMobile;
      DEBUG ((EFI_D_INFO,  "PlatformInfoHob->PlatformFlavor  = 0x%x\n", PlatformInfoHob.PlatformFlavor ));
      DEBUG ((EFI_D_INFO,  "PlatformInfoHob->BoardSvidConfig  = 0x%x\n", PlatformInfoHob.BoardSvidConfig ));
      DEBUG ((EFI_D_INFO,  "PlatformInfoHob->BoardId  = 0x%x\n", PlatformInfoHob.BoardId ));

      PlatformInfoHob.ECSupport = TRUE;
      PlatformInfoHob.FanSupport = TRUE;
      PlatformInfoHob.BatterySupport = TRUE;

      DataSize = sizeof (EFI_PLATFORM_INFO_HOB);
      PcdSetPtr (PcdPlatformInfo, &DataSize, &PlatformInfoHob);

      DataSize = sizeof (EFI_GUID);
      PcdSetPtr (PcdBmpImageGuid, &DataSize, &gBswCrImageGuid);
  
      PcdSet8 (PcdNfcConnection, 1);
      
      PcdSet8 (PcdOemMemeoryDimmType,SolderDownMemory);
      PcdSet64 (PcdMemorySpdPtr, (UINT64)(UINTN)&mBSW_CR_SpdDataMemorySolderDown);
      
      // Program all the gpios at this moment
      InternalGpioPADConfig(0, sizeof(mBSW_CR_GpioInitData)/sizeof(mBSW_CR_GpioInitData[0]),   mBSW_CR_GpioInitData);

      Status = PeiServicesInstallPpi (&mBraswellCRDetectedPpi);
    }
  }
  
  return EFI_SUCCESS;
}

/**
  This function performs Board initialization in Pre-Memory.

  @retval     EFI_SUCCESS           The PPI is installed and initialized.
  @retval     EFI ERRORS            The PPI is not successfully installed.
  @retval     EFI_OUT_OF_RESOURCES  No enough resoruces (such as out of memory).
**/
EFI_STATUS
EFIAPI
BraswellCRInitConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                        Status;

  Status = PeiServicesNotifyPpi (&mBraswellCRDetectionStartNotifyList);

  return Status;
}
