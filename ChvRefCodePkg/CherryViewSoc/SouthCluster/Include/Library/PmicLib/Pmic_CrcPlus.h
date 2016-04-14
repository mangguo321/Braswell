/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_REG_CC_P_H_
#define _PMIC_REG_CC_P_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/I2CLib.h>
#include <Library/PmicLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>

#include "PmicReg.h"
#include "ChvAccess.h"
#include <Guid/PlatformInfo.h>

#define DELAY_BETWEEN_INSTRUCTION_1 50
#define DELAY_BETWEEN_INSTRUCTION   10
#define R_PMIC_PBCONFIG        0x26
#define MASK_PMIC_PB_DISABLE   0xf0  //disable power button
#define MASK_PBCONFIG_FCOT     0x0f //last 4 bits corresponds to FCOT

RegInit_st g_Diaglog_ThermRegInit[]=
{
  {PMIC_REG_SYS0_THRMALRT0_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS0_THRMALRT0_L, REG_OVERRIDE, 0xFF, 0x0},
  {PMIC_REG_SYS0_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS0_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x00},

  {PMIC_REG_SYS1_THRMALRT0_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS1_THRMALRT0_L, REG_OVERRIDE, 0xFF, 0x0},
  {PMIC_REG_SYS1_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS1_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x0},

  {PMIC_REG_SYS2_THRMALRT0_H, REG_OVERRIDE, 0xFF, 0xCD}, // A0_P/A0_Alert/67 C
  {PMIC_REG_SYS2_THRMALRT0_L, REG_OVERRIDE, 0xFF, 0x08},
  {PMIC_REG_SYS2_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS2_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x0},

  {PMIC_REG_TS_ENABLE,        REG_OVERRIDE,    0xFF, 0x3f},    //enable all thermsistors
};
RegInit_st g_Rohm_ThermRegInit[]=
{
  {PMIC_REG_SYS0_THRMALRT3_H, REG_OVERRIDE, 0xFF, 0xCD},  // the values are from Peter on B0 PO
  {PMIC_REG_SYS0_THRMALRT3_L, REG_OVERRIDE, 0xFF, 0x08},  // enabled but threshold is 0
  {PMIC_REG_SYS0_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x4C},  //
  {PMIC_REG_SYS0_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x00},

  {PMIC_REG_SYS1_THRMALRT3_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS1_THRMALRT3_L, REG_OVERRIDE, 0xFF, 0x00},
  {PMIC_REG_SYS1_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS1_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x00},

  {PMIC_REG_SYS2_THRMALRT3_H, REG_OVERRIDE, 0xFF, 0x4C},   // A0_P/A0_Alert/67 C
  {PMIC_REG_SYS2_THRMALRT3_L, REG_OVERRIDE, 0xFF, 0x00},
  {PMIC_REG_SYS2_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x4C},
  {PMIC_REG_SYS2_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x00},

  {PMIC_REG_BAT0_THRMALRT3_H, REG_OVERRIDE, 0xFF, 0x3D},   // battery #0 alert0 50C   (disabled)
  {PMIC_REG_BAT0_THRMALRT3_L, REG_OVERRIDE, 0xFF, 0x97},
  {PMIC_REG_BAT0_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x3D},   // alert 1:45C
  {PMIC_REG_BAT0_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0xCA},
  {PMIC_REG_BAT0_THRMCRIT_H,  REG_OVERRIDE, 0xFF, 0x6A},   // 75C
  {PMIC_REG_BAT0_THRMCRIT_L,  REG_OVERRIDE, 0xFF, 0xF0},   //-55C

  {PMIC_REG_BAT1_THRMALRT3_H, REG_OVERRIDE, 0xFF, 0x3D},   //disabled
  {PMIC_REG_BAT1_THRMALRT3_L, REG_OVERRIDE, 0xFF, 0x97},
  {PMIC_REG_BAT1_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x3D},
  {PMIC_REG_BAT1_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0xCA},
  {PMIC_REG_BAT1_THRMCRIT_H,  REG_OVERRIDE, 0xFF, 0x6A},
  {PMIC_REG_BAT1_THRMCRIT_L,  REG_OVERRIDE, 0xFF, 0xF0},

  {PMIC_REG_PMIC_THRMALRT3_H, REG_OVERRIDE, 0xFF, 0xFE},   // 110C
  {PMIC_REG_PMIC_THRMALRT3_L, REG_OVERRIDE, 0xFF, 0x17},
  {PMIC_REG_PMIC_THRMALRT1_H, REG_OVERRIDE, 0xFF, 0x7E},   // 100C
  {PMIC_REG_PMIC_THRMALRT1_L, REG_OVERRIDE, 0xFF, 0x2A},
  {PMIC_REG_PMIC_THRMCRIT,    REG_OVERRIDE, 0xFF, 0xFF},   // 123C     overflow MSB1 bit in the design. so only value higher than 123 is acceptable.

  {PMIC_REG_TS_ENABLE,        REG_OVERRIDE, 0xFF, 0x3F},    // Enable all thermistors

  {PMIC_REG_THRMMONCTL0,      REG_OVERRIDE,  0xFF, 0xB},   //enable thermal automatic monitoring timer, 1s sample interval
};

GpioCfg_st g_GPIO_cfg[]= {
  {"GPIO0P0", PMIC_REG_GPIO0P0CTLO, PMIC_REG_GPIO0P0CTLI, DIR_INPUT,  INTERRUPT_EN,  (PMIC_MASK_DRV|PMIC_MASK_REN|PMIC_MASK_RVAL_50K_PU),  (PMIC_MASK_GPIDBNC|PMIC_MASK_INTCNT_BOTH) },//"Volumn up" (debounce enable)
  {"GPIO0P1", PMIC_REG_GPIO0P1CTLO, PMIC_REG_GPIO0P1CTLI, DIR_INPUT,  INTERRUPT_EN,  (PMIC_MASK_DRV|PMIC_MASK_REN|PMIC_MASK_RVAL_50K_PU),  (PMIC_MASK_GPIDBNC|PMIC_MASK_INTCNT_BOTH) },//"Volumn down" (debounce enable)

  {"GPIO1P0", PMIC_REG_GPIO1P0CTLO, PMIC_REG_GPIO1P0CTLI, DIR_INPUT,  INTERRUPT_EN,  (PMIC_MASK_DRV|PMIC_MASK_REN|PMIC_MASK_RVAL_50K_PU|PMIC_MASK_ALTFUNCEN),  (PMIC_MASK_GPIDBNC|PMIC_MASK_INTCNT_BOTH) }, //enable altfunc "UIBTN_B" home screen (debounce enable)
};

GpioCfg_st g_Pmic_Gpio_Lpc_Cfg[]= {
  {"GPIO1P1", PMIC_REG_GPIO1P1CTLO, PMIC_REG_GPIO1P1CTLI, DIR_OUTPUT, INTERRUPT_DIS, (PMIC_MASK_DRV|PMIC_MASK_RVAL_50K_PD|PMIC_MASK_DIR),  (PMIC_MASK_GPIDBNC|PMIC_MASK_INTCNT_BOTH) },//PMIC_GPIO_1_LPC  GPO low 0x3c:0x34

};

RegInit_st g_IntrRegInit[]=
{
  {PMIC_REG_MGPIO0IRQS0,   REG_OVERRIDE, 0xff, 0xff},    //disable all GPIO interrupts
  {PMIC_REG_MGPIO1IRQS0,   REG_OVERRIDE, 0xff, 0xff},
  {PMIC_REG_MGPIO0IRQSX,   REG_OVERRIDE, 0xff, 0xff},
  {PMIC_REG_MGPIO1IRQSX,   REG_OVERRIDE, 0xff, 0xff},
  {PMIC_REG_MADCIRQ0,      REG_OVERRIDE, 0xff, 0xff},
  {PMIC_REG_MADCIRQ1,      REG_OVERRIDE, 0x1f, 0x1f},
  {PMIC_REG_MCHGRIRQS0,    REG_CLEAR,    PMIC_MASK_MCHGR,  PMIC_MASK_MCHGR}, //enable external charger interrupt
  {PMIC_REG_MCHGRIRQSX,    REG_CLEAR,    PMIC_MASK_MCHGR,  PMIC_MASK_MCHGR}, //enable external charger interrupt
  {PMIC_REG_MPWRSRCIRQS0,  REG_CLEAR,    (PMIC_MASK_BATDET|PMIC_MASK_DCINDET|PMIC_MASK_VBUSDET), (PMIC_MASK_BATDET|PMIC_MASK_DCINDET|PMIC_MASK_VBUSDET)   }, //enable power src interrupt
  {PMIC_REG_MPWRSRCIRQSX,  REG_CLEAR,    (PMIC_MASK_BATDET|PMIC_MASK_DCINDET|PMIC_MASK_VBUSDET), (PMIC_MASK_BATDET|PMIC_MASK_DCINDET|PMIC_MASK_VBUSDET)   },
  {PMIC_REG_MIRQLVL1,      REG_OVERRIDE,  PMIC_MASK_ALL_IRQ,  (PMIC_MASK_MPWRSRC|PMIC_MASK_MCHGRINT|PMIC_MASK_MADC|PMIC_MASK_MGPIO)},  //only enable 4kinds of intrs
  {PMIC_REG_MTHRMIRQ0,     REG_OVERRIDE,  0xff, 0xff}, //disable all THerm intrs
  {PMIC_REG_MTHRMIRQ1,     REG_OVERRIDE,  0xf,  0xf},
  {PMIC_REG_MTHRMIRQ2,     REG_OVERRIDE,  0x3f, 0x3f},

  {PMIC_REG_GPIO0IRQ,      REG_OVERRIDE, 0xff, 0xff},     //write clear
  {PMIC_REG_GPIO1IRQ,      REG_OVERRIDE, 0xff, 0xff},
};

RegInit_st g_BcuRegInit[]=
{
  {PMIC_REG_VWARNA_CFG,      REG_CLEAR,    PMIC_MASK_VWARNA_EN, PMIC_MASK_VWARNA_EN},  //TODO: require meaningful value from HW teams
  {PMIC_REG_VWARNB_CFG,      REG_CLEAR,    PMIC_MASK_VWARNB_EN, PMIC_MASK_VWARNB_EN},  //disable VWENB
  {PMIC_REG_VCRIT_CFG,       REG_CLEAR,    PMIC_MASK_VCRIT_EN,  PMIC_MASK_VCRIT_EN},   //disable VCRIT
  {PMIC_REG_BCUDISA_BEH,     REG_OVERRIDE, 0x0, 0x0},
  {PMIC_REG_BCUDISB_BEH,     REG_OVERRIDE, 0x0, 0x0},
  {PMIC_REG_BCUDISCRIT_BEH,  REG_OVERRIDE, 0x0, 0x0},
  {PMIC_REG_BCUPROCHOT_B_BEH,REG_OVERRIDE, 0x0, 0x0},
  {PMIC_REG_MBCUIRQ,         REG_OVERRIDE, PMIC_MASK_MBCU_ALL, PMIC_MASK_MBCU_ALL },  //mask all
};

RegInit_st g_MiscRegInit[]=
{
  // V2P8SX rail drops to 2.56V with Camera enabled. The voltage is at 2.8v on startup and drops to 2.56V when the camera is brought out of reset.
  // PMIC_REG_V1P8SXCNT = 0x5D : The default and correct value is 011 for 2.8V. We are overriding the values.
  {PMIC_REG_VBUSCNT,    REG_OVERRIDE,    PMIC_VR_SEL_BIT | PMIC_VR_EN_BIT,  PMIC_VR_SEL_BIT | PMIC_VR_EN_BIT},      //0-VBUS_EN is controlled by ULPI_VBUS_EN
  {PMIC_REG_VHDMICNT,   REG_OVERRIDE,    PMIC_VR_SEL_BIT | PMIC_VR_EN_BIT,  PMIC_VR_SEL_BIT | PMIC_VR_EN_BIT},      //0-VBUS_EN is controlled by ULPI_VBUS_EN
  {PMIC_REG_GPIO0P3CTLO,REG_OVERRIDE,    0xFF,                              0x21},  //for PMIC audio  reset GPIO
  {PMIC_REG_V3P3SXCNT,  REG_OVERRIDE,    PMIC_VR_EN_BIT | PMIC_VR_LPEN_BIT | BIT7 | BIT6 | BIT5,  PMIC_VR_EN_BIT | PMIC_VR_LPEN_BIT | BIT6 | BIT5},
};

UINT8
EFIAPI
CrcPlusPmicRead8 (
  IN      UINT8                     Register
  );

EFI_STATUS
EFIAPI
CrcPlusPmicWrite8 (
  IN      UINT8                     Register,
  IN      UINT8                     Value
  );

EFI_STATUS
EFIAPI
CrcPlusPmicThermInit (void);

EFI_STATUS
EFIAPI
CrcPlusPmicGpioInit (void);

EFI_STATUS
EFIAPI
CrcPlusPmicIntrInit (void);

EFI_STATUS
EFIAPI
CrcPlusPmicBcuInit (void);

EFI_STATUS
EFIAPI
CrcPlusPmicMiscInit (void);

EFI_STATUS
EFIAPI
CrcPlusPmicPage0Init (void *Profile);

UINT8
EFIAPI
CrcPlusPmicRead8_page0 (
  IN      UINT8                     Register
  );

EFI_STATUS
EFIAPI
CrcPlusPmicWrite8_page0 (
  IN      UINT8                     Register,
  IN      UINT8                     Value
  );

EFI_STATUS
EFIAPI
CrcPlusPmicVbusControl (BOOLEAN bTurnOn);

EFI_STATUS
EFIAPI
CrcPlusPmicVhostControl (BOOLEAN bTurnOn);

EFI_STATUS
EFIAPI
CrcPlusPmicGetDevID (UINT8 *DevId, UINT8 *RevId);

UINT16
EFIAPI
CrcPlusPmicGetBATID (void);

UINT8
EFIAPI
CrcPlusPmicGetBoardID(void);

UINT8
EFIAPI
CrcPlusPmicGetMemCfgID(void);

UINT8
EFIAPI
CrcPlusPmicGetFABID(void);

UINT16
EFIAPI
CrcPlusPmicGetVBAT (void);

BOOLEAN
EFIAPI
CrcPlusPmicIsACOn (void);

BOOLEAN
EFIAPI
CrcPlusPmicIsPwrBtnPressed(void);

BOOLEAN
EFIAPI
CrcPlusPmicIsUIBtnPressed(void);

UINT16
EFIAPI
CrcPlusPmicGetResetCause (void);

VOID
EFIAPI
CrcPlusPmicClearResetCause (void);

EFI_STATUS
EFIAPI
CrcPlusPmicGetWakeCause (IN OUT UINT8 *WakeCause);

VOID
EFIAPI
CrcPlusPmicClearWakeCause (void);

EFI_STATUS
EFIAPI
CrcPlusPmicDebugRegDump (void);

BOOLEAN
EFIAPI
CrcPlusPmicIsUsbConnected (void);

BOOLEAN
EFIAPI
CrcPlusPmicIsBatOn (void);

EFI_STATUS
EFIAPI
CrcPlusPmicSetVDDQ (void);

EFI_STATUS
EFIAPI
CrcPlusPmicGpioToggleForLpcConfig(void);

VOID
CrcPlusPmicModemWa(void);

VOID
CrcPlusPmicForceOffModem (
  VOID
  );

EFI_STATUS
EFIAPI
CrcPlusPmicBatchRegisterInit (
  IN      RegInit_st               *RegInit,
  IN      UINT32                    length
  );

EFI_STATUS
EFIAPI
CrcPlusPmicSetVIDDecayWA (void);

VOID
CrcPlusProgramPunitPwrConfigRegisters (
  VOID
  );

EFI_STATUS
CrcPlusPmicDisablePowerButton(
  OUT UINT8 *ButtonHoldTime
);

EFI_STATUS
CrcPlusPmicEnablePowerButton (
  IN UINT8  ButtonHoldTime
);

#endif
