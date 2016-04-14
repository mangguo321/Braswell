/** @file
  Header file for Platform Initialization Driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PLATFORM_DRIVER_H
#define _PLATFORM_DRIVER_H

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/HobLib.h>
#include <Library/EfiRegTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/S3BootScriptLib.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/IsaAcpi.h>
#include <Framework/FrameworkInternalFormRepresentation.h>
#include <Protocol/FrameworkHii.h>
#include <Protocol/FrameworkFormCallback.h>
#include <Protocol/CpuIo2.h>
#include <Guid/GlobalVariable.h>
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci22.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#define _EFI_H_    //skip efi.h
#include "PchAccess.h"
#include "ChvAccess.h"
#include "PlatformBaseAddresses.h"

#define ENABLE_USB_HOST_MODE     0
#define ENABLE_USB_DEVICE_MODE   1
#define ENABLE_USB_DEVICE_MODE_WITH_SW_VBUS_VALID   2

#define GPIO_DFX1_PAD0              0x4418
#define GPIO_MMC1_RCLK_PAD0         0x5448

#define PCAT_RTC_ADDRESS_REGISTER   0x74
#define PCAT_RTC_DATA_REGISTER      0x75

#define RTC_ADDRESS_SECONDS           0   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES           2   // R/W  Range 0..59
#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99
#define RTC_ADDRESS_CENTURY           50  // R/W  Range 19..20 Bit 8 is R/W

#define RTC_ADDRESS_SECOND_ALARM    0x01
#define RTC_ADDRESS_MINUTE_ALARM    0x03
#define RTC_ADDRESS_HOUR_ALARM      0x05

#define RTC_ADDRESS_REGISTER_A      0x0A
#define RTC_ADDRESS_REGISTER_B      0x0B
#define RTC_ADDRESS_REGISTER_C      0x0C
#define RTC_ADDRESS_REGISTER_D      0x0D

#define B_RTC_ALARM_INT_ENABLE      0x20
#define B_RTC_ALARM_INT_STATUS      0x20

#define B_RTC_DATE_ALARM_MASK       0x3F

// Default CPU Alternate Duty Cycle (255=100%, 0=0%)
#define DEF_CPU_ALT_DUTY_CYCLE 0xFF

#define MAX_ONBOARD_SATA_DEVICE 2

#define DXE_DEVICE_ENABLED  1
#define DXE_DEVICE_DISABLED 0

#define AZALIA_MAX_LOOP_TIME  0x10000

//
// Platform driver GUID
//

#define EFI_OEM_SPECIFIC      0x8000
#define EFI_CU_PLATFORM_DXE_INIT                     (EFI_OEM_SPECIFIC | 0x00000011)
#define EFI_CU_PLATFORM_DXE_STEP1                    (EFI_OEM_SPECIFIC | 0x00000012)
#define EFI_CU_PLATFORM_DXE_STEP2                    (EFI_OEM_SPECIFIC | 0x00000013)
#define EFI_CU_PLATFORM_DXE_STEP3                    (EFI_OEM_SPECIFIC | 0x00000014)
#define EFI_CU_PLATFORM_DXE_STEP4                    (EFI_OEM_SPECIFIC | 0x00000015)
#define EFI_CU_PLATFORM_DXE_INIT_DONE                (EFI_OEM_SPECIFIC | 0x00000016)

#define EFI_SECTION_STRING                  0x1C
#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
#define PREFIX_BLANK                        0x04

#pragma pack(1)

//
// BUGBUG: should remove these EDK hii definition once Hii transtion is done
//
typedef UINT16  STRING_REF;
typedef UINT16  EFI_FORM_LABEL;

typedef
EFI_STATUS
(EFIAPI *EFI_FORM_ROUTINE) (
  SYSTEM_CONFIGURATION *SetupBuffer
  );

typedef struct{
  UINT16 DeviceNumber;
  UINT16 FunctionNumber;
}PCI_DEVICE_FUNC_INFO;

typedef struct{
  CHAR16 PortNumber[4];
  STRING_REF SataDeviceInfoStringId;
}SATA_DEVICE_STRING_INFO;

typedef struct {
  UINT8   Number;
  UINT32  HorizontalResolution;
  UINT32  VerticalResolution;
} PANEL_RESOLUTION;

#pragma pack()

//
// Prototypes
//
EFI_STATUS
EfiMain (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
ProcessEventLog (
  );

EFI_STATUS
FindDataRecords (
  );

VOID
MemorySetup(
  );

UINTN
EfiValueToString (
  IN  OUT CHAR16  *Buffer,
  IN  INT64       Value,
  IN  UINTN       Flags,
  IN  UINTN       Width
  );

VOID
ReadyToBootFunction (
  EFI_EVENT  Event,
  VOID       *Context
  );

VOID
InstallHiiDataAndGetSettings (
  IN EFI_HII_STRING_PACK            *StringPack,
  ... // 0 or more of => IN EFI_HII_IFR_PACK *IfrPack,
      // Terminate list with NULL
  );

VOID
EfiLogicalOrMem (
  IN VOID   *Destination,
  IN VOID   *Source,
  IN UINTN  Length
  );

VOID
AsciiToUnicode (
  IN    CHAR8     *AsciiString,
  IN    CHAR16    *UnicodeString
  );

VOID
InitializeBoardId (
  );

VOID
InitFeaturePolicy (
  IN EFI_PLATFORM_INFO_HOB      *PlatformInfo
  );

VOID
InitializeSetupVarHide (
  );

VOID
InitializeAllBootStrings (
  VOID
  );

VOID
EFIAPI
SaveLpssSccPciMode (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  );
  
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *mPciRootBridgeIo;

#endif
