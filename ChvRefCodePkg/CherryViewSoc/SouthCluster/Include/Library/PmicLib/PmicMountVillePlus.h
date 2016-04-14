/** @file

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_REG_WC_H_
#define _PMIC_REG_WC_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PmicLib.h>
#include <Library/PcdLib.h>

#include "PmicReg.h"
#include "ChvAccess.h"
#include <Guid/PlatformInfo.h>

UINT8
EFIAPI
MtvPlusPmicRead8 (
  IN      UINT8                     Register
  );

EFI_STATUS
EFIAPI
MtvPlusPmicWrite8 (
  IN      UINT8                     Register,
  IN      UINT8                     Value
  );

EFI_STATUS
EFIAPI
MtvPlusPmicThermInit (void);

EFI_STATUS
EFIAPI
MtvPlusPmicGpioInit (void);

EFI_STATUS
EFIAPI
MtvPlusPmicIntrInit (void);

EFI_STATUS
EFIAPI
MtvPlusPmicBcuInit (void);

EFI_STATUS
EFIAPI
MtvPlusPmicMiscInit (void);

EFI_STATUS
EFIAPI
MtvPlusPmicPage0Init (void *Profile);

UINT8
EFIAPI
MtvPlusPmicRead8_page0 (
  IN      UINT8                     Register
  );

EFI_STATUS
EFIAPI
MtvPlusPmicWrite8_page0 (
  IN      UINT8                     Register,
  IN      UINT8                     Value
  );

EFI_STATUS
EFIAPI
MtvPlusPmicVbusControl (BOOLEAN bTurnOn);

EFI_STATUS
EFIAPI
MtvPlusPmicVhostControl (BOOLEAN bTurnOn);

EFI_STATUS
EFIAPI
MtvPlusPmicGetDevID (UINT8 *DevId, UINT8 *RevId);

UINT16
EFIAPI
MtvPlusPmicGetBATID (void);

UINT8
EFIAPI
MtvPlusPmicGetBoardID(void);

UINT8
EFIAPI
MtvPlusPmicGetMemCfgID(void);

UINT8
EFIAPI
MtvPlusPmicGetFABID(void);

UINT16
EFIAPI
MtvPlusPmicGetVBAT (void);

BOOLEAN
EFIAPI
MtvPlusPmicIsACOn (void);

BOOLEAN
EFIAPI
MtvPlusPmicIsPwrBtnPressed(void);

BOOLEAN
EFIAPI
MtvPlusPmicIsUIBtnPressed(void);

UINT16
EFIAPI
MtvPlusPmicGetResetCause (void);

VOID
EFIAPI
MtvPlusPmicClearResetCause (void);

EFI_STATUS
EFIAPI
MtvPlusPmicGetWakeCause (IN OUT UINT8 *WakeCause);

VOID
EFIAPI
MtvPlusPmicClearWakeCause (void);

EFI_STATUS
EFIAPI
MtvPlusPmicDebugRegDump (void);

BOOLEAN
EFIAPI
MtvPlusPmicIsUsbConnected (void);

BOOLEAN
EFIAPI
MtvPlusPmicIsBatOn (void);

EFI_STATUS
EFIAPI
MtvPlusPmicSetVDDQ (void);

EFI_STATUS
EFIAPI
MtvPlusPmicGpioToggleForLpcConfig(void);

VOID
MtvPlusPmicForceOffModem (
  VOID
  );

EFI_STATUS
EFIAPI
MtvPlusPmicBatchRegisterInit (
  IN      RegInit_st               *RegInit,
  IN      UINT32                    length
  );

EFI_STATUS
EFIAPI
MtvPlusPmicSetVIDDecayWA (void);

VOID
MtvPlusPmicProgramPunitPwrConfigRegisters (
  VOID
  );

#endif
