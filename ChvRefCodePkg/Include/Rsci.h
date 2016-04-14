/** @file

  Copyright (c) 1996 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _RSCI_H
#define _RSCI_H

//
// Statements that include other files
//
#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi20.h>
#include <IndustryStandard/Acpi30.h>

//
// Definitions
//
#define EFI_ACPI_OEM_RSCI_REVISION                      0x00000000
#define EFI_ACPI_RSCI_REVISION                          1
#define EFI_ACPI_RESET_SHUTDOWN_COMMUNICATION_INTERFACE_SIGNATURE  SIGNATURE_32('R', 'S', 'C', 'I')
#ifndef EFI_ACPI_OEM_ID
#define EFI_ACPI_OEM_ID                                 'I','N','T','E','L',' '    // 6 Bytes long
#endif
#define EFI_ACPI_RSCI_OEM_TABLE_ID                      SIGNATURE_64 ('B','O','O','T','S','R','C',' ') // OEM table id 8 bytes long
//
// RSCI structure
//
//
// Ensure proper structure formats
//

#pragma pack(1)

typedef enum {
  FBR_NONE = 0,
  FBR_WATCHDOG_COUNTER_EXCEEDED,
  FBR_NO_MATCHING_OS,

  FBR_INVALID_OSNIB_CHECKSUM,
} FALL_BACK_POLICY_REASON;

typedef struct {
  FALL_BACK_POLICY_REASON     FallbackPolicyReason;
  UINT8                       FastbootCombo;
  UINT8                       Reserved[2];
} EFI_ACPI_RSCI_INDICATORS;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT8                       WakeSrc;
  UINT8                       Resets;
  UINT8                       ResetType;
  UINT8                       ShuntdownSrc;
  EFI_ACPI_RSCI_INDICATORS    Indicators;
} EFI_ACPI_RESET_SHUTDOWN_COMMUNICATION_INTERFACE;

#pragma pack()

//
//Android Wake Sources for S4/S5
//
typedef enum {
  WAKE_NOT_APPLICABLE = 0,
  WAKE_BATTERY_INSERTED,
  WAKE_USB_CHARGER_INSERTED,
  WAKE_ACDC_CHARGER_INSERTED,
  WAKE_POWER_BUTTON_PRESSED,
  WAKE_RTC_TIMER,
  WAKE_BATTERY_REACHED_IA_THRESHOLD
} ANDROID_WAKE_SOURCE;

//
//Android Reset Sources
//
typedef enum {
  RESET_NOT_APPLICABLE = 0,
  RESET_OS_INITIATED,
  RESET_FORCED,
  RESET_FW_UPDATE,
  RESET_KERNEL_WATCHDOG, 
  RESET_SECURITY_WATCHDOG,
  RESET_SECURITY_INITIATED,
  RESET_PMC_WATCHDOG,
  RESET_EC_WATCHDOG,
  RESET_PLATFORM_WATCHDOG,
} ANDROID_RESET_SOURCE;

typedef enum {
  NOT_APPLICABLE_RESET = 0,
  WARM_RESET = 1,
  COLD_RESET = 2,
  GLOBAL_RESET = 7,
}ANDROID_RESET_TYPE;

//
//Android Shutdown sources
//
typedef enum {
  SHTDWN_NOT_APPLICABLE = 0,
  SHTDWN_POWER_BUTTON_OVERRIDE,
  SHTDWN_BATTERY_REMOVAL,
  SHTDWN_VCRIT,
  SHTDWN_THERMTRIP,
  SHTDWN_PMICTEMP,
  SHTDWN_SYSTEMP,
  SHTDWN_BATTEMP,
  SHTDWN_SYSUVP,
  SHTDWN_SYSOVP,
  SHTDWN_SECURITY_WATCHDOG,
  SHTDWN_SECURITY_INITIATED,
  SHTDWN_PMC_WATCHDOG,
  SHTDWN_EC_WATCHDOG,
  SHTDWN_PLATFORM_WATCHDOG,
  SHTDWN_KERNEL_WATCHDOG
}ANDROID_SHUTDOWN_SOURCE;

#define EFI_OS_INDICATIONS_OS_INITIATED_RESET 0x00000040
#endif
