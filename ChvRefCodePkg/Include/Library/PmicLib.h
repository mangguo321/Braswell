/** @file
  Header file of PMIC hardware definition.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_LIB_H_
#define _PMIC_LIB_H_

#define CRC_PLUS_SVID_CONFIGURATION    1
#define WHISKEYCOVE_SVID_CONFIGURATION 2
#define DC_XPOWER_SVID_CONFIGURATION   3
#define DC_TI_SVID_CONFIGURATION       4
#define MTV_PLUS_SVID_CONFIGURATION    5

typedef enum {
  REG_OVERRIDE    = 0,
  REG_AND         = 1,
  REG_OR          = 2,
  REG_CLEAR       = 3
} RegAction;

typedef struct RegInit{
    UINT8     regoffset;
    RegAction action;
    UINT8     mask;
    UINT8     value;
}RegInit_st;

typedef enum {
   DIR_INPUT  = 0,
   DIR_OUTPUT = 1,
}GpioAttrib;

typedef struct Gpio{
    UINT8 *PinName;
    UINT8 OutputReg;
    UINT8 InputReg;
    GpioAttrib Direction;
    UINT8 IsIntr;       //is it an interrupt
    UINT8 GPOCtrlVal;
    UINT8 GPICtrlVal;
}GpioCfg_st;

#define PMIC_I2C_BUSNO        6     //I2C6 is used. index from 0

//Separated registers into two pages: page 0 (accessible through I2C bus address 0x5E) and page 1 (accessible through I2C bus address 0x6E).
//Page 0 is for OTP. Assigned addresses to registers on page 1.
#define PMIC_PAGE_0_I2C_ADDR  0x5E
#define PMIC_PAGE_1_I2C_ADDR  0x6E  

#define INTERRUPT_EN   1
#define INTERRUPT_DIS  0

#define PMIC_DEV_ROHM            0x1F
#define PMIC_DEV_DIALOG          0x00
#define PMIC_DEV_DIALOG_1        0x2B
#define PMIC_DEV_MOUNTVILLE      0x01

#define PMIC_REVID_B0            0xB0

#define PMIC_ROHM_REVID_A1       0xA1
#define PMIC_ROHM_REVID_A2       0xA2
#define PMIC_ROHM_REVID_B0       0xB0
#define PMIC_ROHM_REVID_B1       0xB1
#define PMIC_ROHM_REVID_B2       0xB2
#define PMIC_ROHM_REVID_B3       0xB3
#define PMIC_MOUNTVILLE_REVID_1  0x15

#define PMIC_DIALOG_REVID_A0      0x0A
#define PMIC_DIALOG_REVID_B0      0xB0
#define PMIC_DIALOG_REVID_C0      0xC0

#define PMIC_REG_MODEMCTRL_CRCP   0x29

// USB charger type definition for SMSC3750
#define CHARGER_TYPE_DCP         BIT5
#define CHARGER_TYPE_CDP         BIT6
#define CHARGER_TYPE_SDP         (BIT5 | BIT6)
#define CHARGER_TYPE_SEL1_LOW    BIT7
#define CHARGER_TYPE_SEL1_HIGH   (BIT5 | BIT7)
#define CHARGER_TYPE_INVALID     0x00

/**
  @todo	add description

**/
UINT8
EFIAPI
PmicRead8 (
  IN      UINT8                     Register
  );

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicWrite8 (
  IN      UINT8                     Register,
  IN      UINT8                     Value
  );

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicThermInit (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicGpioInit (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicIntrInit (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicBcuInit (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicMiscInit (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicPage0Init (void *Profile);

/**
  @todo	add description

**/
UINT8
EFIAPI
PmicRead8_page0 (
  IN      UINT8                     Register
  );

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicWrite8_page0 (
  IN      UINT8                     Register,
  IN      UINT8                     Value
  );

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicVbusControl (BOOLEAN bTurnOn);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicClearGPIOIRQ (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicVhostControl (BOOLEAN bTurnOn);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicGetDevID (UINT8 *DevId, UINT8 *RevId);

/**
  @todo	add description

**/
UINT16
EFIAPI
PmicGetBATID (void);

EFI_STATUS
EFIAPI
PmicEnableDCPWakeSource(BOOLEAN IsEnabled);

BOOLEAN
EFIAPI
PmicGetBPTherm (UINT16 *PmicBattId);

EFI_STATUS
EFIAPI
PmicGetBatTSAdc (
  IN OUT UINT16 *TSAdc
  );
/**
  @todo	add description

**/
BOOLEAN
EFIAPI
PmicGetBatteryPresence (VOID);

/**
  @todo	add description

**/
UINT8
EFIAPI
PmicGetBoardID(void);

/**
  @todo	add description

**/
UINT8
EFIAPI
PmicGetMemCfgID(void);

/**
  @todo	add description

**/
UINT8
EFIAPI
PmicGetFABID(void);

/**
  @todo	add description

**/
UINT16
EFIAPI
PmicGetVBAT (void);

/**
  @todo	add description

**/
BOOLEAN
EFIAPI
PmicIsACOn (void);

/**
  @todo	add description

**/
BOOLEAN
EFIAPI
PmicIsPwrBtnPressed(void);

/**
  @todo	add description

**/
BOOLEAN
EFIAPI
PmicIsUIBtnPressed(void);

/**
  @todo	add description

**/
UINT16
EFIAPI
PmicGetResetCause (void);

EFI_STATUS
EFIAPI
PmicSetVDDQ (void);

UINT32
EFIAPI
PmicStateOfCharge(void);

EFI_STATUS
EFIAPI
PmicConfigureCharger(void);

BOOLEAN
EFIAPI
PmicCheckForChargerPresent(UINT8* Type);

EFI_STATUS
EFIAPI
PmicCheckWakeSrc(UINTN *Source);

EFI_STATUS
EFIAPI
PmicSetChargeCurrent(UINT32 ChargeRate);

UINT16
EFIAPI
PmicGetVoltage (void);

UINT32
EFIAPI
PmicGetACCUMCURR (void);

UINT32
EFIAPI
PmicGetSAMPLCURR (void);

UINT8 
EFIAPI
PmicClearPowerButtonStatus(void);

EFI_STATUS
EFIAPI
TiReadEeprom(void);

EFI_STATUS
EFIAPI
TiReadOffsetGainCompensation(void);

UINT16
EFIAPI
TiPmicGetCorrectedVoltage(void);

UINT32
EFIAPI
TiCCCompensation(INT32 DeltaQ, INT32 DeltaSample);

/**
  @todo	add description

**/
VOID
EFIAPI
PmicClearResetCause (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicGetWakeCause (UINT8 *WakeCause);

/**
  @todo	add description

**/
VOID
EFIAPI
PmicClearWakeCause (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicDebugRegDump (void);

/**
  @todo	add description

**/
BOOLEAN
EFIAPI
PmicIsUsbConnected (void);

/**
  @todo	add description

**/
BOOLEAN
EFIAPI
PmicIsBatOn (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicSetVDDQ (void);

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicGpioToggleForLpcConfig(void);

/**
  @todo	add description

**/
VOID
PmicForceVggPwmModeWA(void);

VOID     
PmicModemWa (
  VOID
  );

/**
  @todo	add description

**/
UINT16
EFIAPI
PmicGetBatteryCap (VOID);

/**
  @todo	add description

**/
VOID
PmicForceOffModem (
  VOID
  );
  
/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicBatchRegisterInit (
  IN      RegInit_st               *RegInit,
  IN      UINT32                    length
  );
  
/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
PmicSetVIDDecayWA (void);

/**
  @todo	add description

**/
EFI_STATUS
PmicGetShutdownCause (
  IN OUT UINT32 *ShutdownCause
  );
  
/**
  @todo	add description

**/
EFI_STATUS 
PmicGetPwrSrcIrq (
  IN OUT UINT32 *PowerSrcIrq
  );

/**
  @todo	add description

**/
EFI_STATUS
PmicClearShutdownCause (VOID);

/**
  @todo	add description

**/
VOID
PmicForceOffModem (
  VOID
  );

EFI_STATUS
PmicDisablePowerButton(
  OUT UINT8 *
);

EFI_STATUS
PmicEnablePowerButton (
  IN UINT8
);

EFI_STATUS
EFIAPI
PmicGetBatTSAdc (
  IN OUT UINT16 *TSAdc
  );

BOOLEAN
EFIAPI
PmicIsPowerOnReset (
  VOID
  );

#endif
