/** @file
  Platform Early Stage header file

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _EFI_PLATFORM_EARLY_INIT_H_
#define _EFI_PLATFORM_EARLY_INIT_H_

#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x

#include <FrameworkPei.h>
#include "PlatformBaseAddresses.h"
#include "PchAccess.h"
#include "ChvAccess.h"
#include "CpuRegs.h"
#include "Platform.h"
#include "CpuRegs.h"
#include <Ppi/Stall.h>
#include <Guid/PlatformInfo.h>
#include <Library/GpioLib.h>
#include <Guid/SetupVariable.h>
#include <Ppi/AtaController.h>
#include <Ppi/FindFv.h>
#include <Ppi/BootInRecoveryMode.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/Capsule.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/Pci22.h>
#include <Guid/FirmwareFileSystem.h>
#include <Guid/MemoryTypeInformation.h>
#include <Ppi/Cache.h>
#include <Library/PchPlatformLib.h>
#include <Ppi/SmbusPolicy.h>
#include <Ppi/Reset.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/ChvPlatformPolicyPpi.h>
#include <Guid/GlobalVariable.h>
#include <Ppi/RecoveryModule.h>
#include <Ppi/DeviceRecoveryModulePei.h>
#include <Guid/Capsule.h>
#include <Guid/RecoveryDevice.h>
#include <Ppi/MasterBootMode.h>
#include <Library/I2CLib.h>
#include <Library/TimerLib.h>

#define SMC_LAN_ON       0x46
#define SMC_LAN_OFF    0x47
#define SMC_DEEP_S3_STS    0xB2
#define EXT_MODEL_ID_VLV2 0x3

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

//
// Wake Event Types
//
#define SMBIOS_WAKEUP_TYPE_RESERVED           0x00
#define SMBIOS_WAKEUP_TYPE_OTHERS             0x01
#define SMBIOS_WAKEUP_TYPE_UNKNOWN            0x02
#define SMBIOS_WAKEUP_TYPE_APM_TIMER          0x03
#define SMBIOS_WAKEUP_TYPE_MODEM_RING         0x04
#define SMBIOS_WAKEUP_TYPE_LAN_REMOTE         0x05
#define SMBIOS_WAKEUP_TYPE_POWER_SWITCH       0x06
#define SMBIOS_WAKEUP_TYPE_PCI_PME            0x07
#define SMBIOS_WAKEUP_TYPE_AC_POWER_RESTORED  0x08

//
// Defines for stall ppi
//
#define PEI_STALL_RESOLUTION  1

//
// Used in PEI memory test routines
//
#define MEMORY_TEST_COVER_SPAN  0x40000
#define MEMORY_TEST_PATTERN     0x5A5A5A5A

#define EFI_LOW_BEEP_FREQUENCY            0x31B
#define EFI_HIGH_BEEP_FREQUENCY           0x254

//
// General Purpose Constants
//
#define ICH_ACPI_TIMER_MAX_VALUE  0x1000000 //The timer is 24 bit overflow

//
// Function Prototypes
//
EFI_STATUS
PlatformPchInit (
  IN SYSTEM_CONFIGURATION         *SystemConfiguration,
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN UINT16                       PlatformType
  );

EFI_STATUS
PeimInitializeFlashMap (
  IN EFI_FFS_FILE_HEADER          *FfsHeader,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
PeimInstallFlashMapPpi (
  IN EFI_FFS_FILE_HEADER          *FfsHeader,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
EFIAPI
IchReset (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

BOOLEAN
GetSleepTypeAfterWakeup (
  IN  CONST EFI_PEI_SERVICES      **PeiServices,
  OUT UINT16                      *SleepType
  );

EFI_STATUS
EFIAPI
GetWakeupEventAndSaveToHob (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
PeiGetVariable (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN CHAR16                       *VariableName,
  IN EFI_GUID                     * VendorGuid,
  OUT UINT32                      *Attributes OPTIONAL,
  IN OUT UINTN                    *DataSize,
  OUT VOID                        *Data
  );

EFI_STATUS
EFIAPI
PeiGetNextVariableName (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT UINTN                    *VariableNameSize,
  IN OUT CHAR16                   *VariableName,
  IN OUT EFI_GUID                 *VendorGuid
  );

EFI_STATUS
UpdateBootMode (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT EFI_PLATFORM_INFO_HOB    *PlatformInfoHob
  );

EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
PeimInitializeRecovery (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

VOID
CheckPowerOffNow (
  VOID
  );

VOID
IchGpioInit (
  IN UINT16                       PlatformType,
  IN SYSTEM_CONFIGURATION         *SystemConfiguration
  );

EFI_STATUS
PcieSecondaryBusReset (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN UINT8                        Bus,
  IN UINT8                        Dev,
  IN UINT8                        Fun
  );

VOID
SetPlatformBootMode (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT EFI_PLATFORM_INFO_HOB    *PlatformInfoHob
  );

BOOLEAN
CheckIfJumperSetForRecovery (
  VOID
  );

EFI_STATUS
FindFv (
  IN EFI_PEI_FIND_FV_PPI          *This,
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT UINT8                    *FvNumber,
  OUT EFI_FIRMWARE_VOLUME_HEADER  **FVAddress
  );

BOOLEAN
IsA16Inverted (
  );

EFI_STATUS
EFIAPI
CpuOnlyReset (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
EFIAPI
InitLan (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN SYSTEM_CONFIGURATION         *Buffer
  );

EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN CONST EFI_PEI_STALL_PPI      *This,
  IN UINTN                        Microseconds
  );

EFI_STATUS
MultiPlatformInfoInit (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT EFI_PLATFORM_INFO_HOB    *PlatformInfoHob
  );

BOOLEAN
IsRecoveryJumper (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT EFI_PLATFORM_INFO_HOB    *PlatformInfoHob
  );

EFI_STATUS
PlatformInfoUpdate (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN OUT EFI_PLATFORM_INFO_HOB    *PlatformInfoHob,
  IN SYSTEM_CONFIGURATION         *SystemConfiguration
  );

VOID
PlatformSaInit (
IN SYSTEM_CONFIGURATION           *SystemConfiguration,
IN CONST EFI_PEI_SERVICES         **PeiServices
  );

EFI_STATUS
InitializePlatform (
  IN CONST EFI_PEI_SERVICES       **PeiServices,
  IN EFI_PLATFORM_INFO_HOB        *PlatformInfoHob,
  IN SYSTEM_CONFIGURATION         *SystemConfiguration
  );

EFI_STATUS
EFIAPI
SetPeiCacheMode (
  IN  CONST EFI_PEI_SERVICES      **PeiServices
  );

EFI_STATUS
EFIAPI
SetDxeCacheMode (
  IN  CONST EFI_PEI_SERVICES      **PeiServices
  );

EFI_STATUS
GPIO_initialization (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *SmbusPpi
  );

EFI_STATUS
GeneralPowerFailureHandler (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

BOOLEAN
IsRtcUipAlwaysSet (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
RtcPowerFailureHandler (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
InitPchUsb (
  IN CONST EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
EFIAPI
PublishMemoryTypeInfo (
  void
  );

#endif
