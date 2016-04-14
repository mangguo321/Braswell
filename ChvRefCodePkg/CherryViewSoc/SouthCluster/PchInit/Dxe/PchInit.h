/** @file
  Header file for PCH Initialization Driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_INITIALIZATION_DRIVER_H_
#define _PCH_INITIALIZATION_DRIVER_H_

#include <Library/S3BootScriptLib.h>
#ifndef FSP_FLAG
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#endif
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

#include <Guid/EventGroup.h>

#ifdef FSP_FLAG
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Uefi/UefiBaseType.h>
extern EFI_GUID gDxePchPolicyUpdateProtocolGuid;
#endif
//
// Driver Consumed Protocol Prototypes
//
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/BootScriptSave.h>
#include <Protocol/ExitPmAuth.h>
#include <Protocol/PchInfo.h>
#include <Protocol/PchSccTuning.h>

#include "PchAccess.h"
#include <Library/PchPlatformLib.h>
#include <Library/AslUpdateLib.h>
#include "PlatformBaseAddresses.h"
#include "../Common/PchUsbCommon.h"
#ifdef EFI_S3_RESUME
#include "IosfSbDefinitions.h"
#endif
#include <Library/PchPciExpressHelpersLib.h>

#define AZALIA_MAX_LOOP_TIME  10
#define AZALIA_WAIT_PERIOD    100
#define AZALIA_MAX_SID_NUMBER 4
#define AZALIA_MAX_SID_MASK   ((1 << AZALIA_MAX_SID_NUMBER) - 1)

typedef struct {
  EFI_PCH_INFO_PROTOCOL PchInfo;
} PCH_INSTANCE_PRIVATE_DATA;

///
/// Data definitions
///
extern EFI_HANDLE mImageHandle;

///
/// SVID / SID init table entry
///
typedef struct {
  UINT8 DeviceNumber;
  UINT8 FunctionNumber;
  UINT8 SvidRegOffset;
} PCH_SVID_SID_INIT_ENTRY;

///
/// Function Prototype
///

/**
  Configures extended IOSF-SB and stores this configuration in S3 boot script

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

**/
VOID
PchMsgBusAndThenOrEx32WithScript (
    IN     UINT8                           PortId,
    IN     UINT32                          Register,
    IN     UINT32                          AndData,
    IN     UINT32                          OrData,
    IN     UINT8                           ReadOpCode,
    IN     UINT8                           WriteOpCode,
    IN     UINT8                           Bar,
    IN     UINT8                           Device,
    IN     UINT8                           Function
  );

/**
  Configures IOSF-SB and stores this configuration in S3 boot script

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code

**/
VOID
PchMsgBusAndThenOr32WithScript (
    IN     UINT8                           PortId,
    IN     UINT32                          Register,
    IN     UINT32                          AndData,
    IN     UINT32                          OrData,
    IN     UINT8                           ReadOpCode,
    IN     UINT8                           WriteOpCode
  );

/**
  Detect and initialize the type of codec (AC'97 and HDA) present in the system.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] AzaliaEnable         Returned with TRUE if Azalia High Definition Audio codec
                                  is detected and initialized.

  @retval EFI_SUCCESS            Codec is detected and initialized.
  @retval EFI_OUT_OF_RESOURCES   Failed to allocate resources to initialize the codec.
**/
EFI_STATUS
ConfigureAzalia (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar,
  IN OUT  BOOLEAN                             *AzaliaEnable
  );
/**
  Configure miscellaneous power management settings

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] AcpiBase             The ACPI I/O Base address of the PCH
  @param[in] PmcBase              PMC base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Timeout while polling a bit in a register
**/
EFI_STATUS
ConfigureMiscPm (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN  UINT16                              AcpiBase,
  IN  UINT32                              PmcBase
  );

/**
  Configure additional power management settings

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureAdditionalPm (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  );

/**
  Configure S0ix Settings

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] PmcBase              PmcBase value of this PCH device
  @param[in] IlbBase              IlbBase value of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureS0ix (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN  UINT32                              PmcBase,
  IN  UINT32                              IlbBase
  );

/**
  Configure for platforms with ACPI Hardware Reduced Mode enabled

  @param[in] AcpiBase             The ACPI I/O Base address of the PCH
  @param[in] PmcBase              PmcBase value of this PCH device
  @param[in] IlbBase              IlbBase value of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureAcpiHwRed (
  IN  UINT16                              AcpiBase,
  IN  UINT32                              PmcBase,
  IN  UINT32                              IlbBase
  );

/**
  Perform miscellany PCH initialization

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] PmcBase              PmcBase value of this PCH device
  @param[in] IlbBase              IlbBase value of this PCH device
  @param[in,out] FuncDisableReg  The value of Function disable register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureMiscItems (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar,
  IN      UINT32                              PmcBase,
  IN      UINT32                              IlbBase,
  IN OUT  UINT32                              *FuncDisableReg
  );

/**
  Hide PCI config space of OTG device and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureOtgAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  );

/**
  Configure OTG devices.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in,out] FuncDisableReg  The value of Function disable register
  @param[in,out] FuncDisabl2eReg The value of Function disable 2 register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureOtg (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy,
  IN OUT UINT32                            *FuncDisableReg,
  IN OUT UINT32                            *FuncDisable2Reg
  );

/**
  Hide PCI config space of LPE device and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureLpeAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  );

/**
  Configure LPE devices.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in,out] FuncDisableReg  The value of Function disable register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureLpe (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy,
  IN OUT UINT32                            *FuncDisableReg
  );

/**
  Hide PCI config space of LPSS device and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureLpssAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  );

/**
  Configure LPSS devices.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in,out] FuncDisableReg  The value of Function disable register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureLpss (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy,
  IN OUT UINT32                            *FuncDisableReg
  );

/**
  Hide PCI config space of SCC devices and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureSccAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  );

/**
  Configure SCC devices.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in,out] FuncDisableReg  The value of Function disable register
  @param[in] IoBase               IO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureScc (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy,
  IN OUT UINT32                            *FuncDisableReg,
  IN     UINT32                            IoBase
  );

/**
  Configure eMMC Tuning Mode

  @param[in] This                         A pointer to PCH_EMMC_TUNING_PROTOCOL structure
  @param[in] Revision                     Revision parameter used to verify the layout of EMMC_INFO and TUNINGDATA.
  @param[in] EmmcInfo                     A pointer to EMMC_INFO structure
  @param[out] EmmcTuningData              A pointer to EMMC_TUNING_DATA structure

  @retval EFI_SUCCESS                     The function completed successfully
  @retval EFI_NOT_FOUND                   The item was not found
  @retval EFI_OUT_OF_RESOURCES            The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER           A parameter was incorrect.
  @retval EFI_DEVICE_ERROR                Hardware Error
  @retval EFI_NO_MEDIA                    No media
  @retval EFI_MEDIA_CHANGED               Media Change
  @retval EFI_BAD_BUFFER_SIZE             Buffer size is bad
  @retval EFI_CRC_ERROR                   Command or Data CRC Error
**/
EFI_STATUS
EFIAPI
ConfigureEmmcTuningMode (
  IN  PCH_EMMC_TUNING_PROTOCOL          *This,
  IN  UINT8                             Revision,
  IN  EMMC_INFO                         *EmmcInfo,
  OUT EMMC_TUNING_DATA                  *EmmcTuningData
  );

/**
  Install PCH SCC Tuning Protocol

  @param[in] PchPlatformPolicy            A pointer to DXE_PCH_PLATFORM_POLICY_PROTOCOL

**/
VOID
InstallPchSccTuningProtocol (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy
  );

/**
  Hide PCI config space of ISH device and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/

/**
  Configure ISH device.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in,out] FuncDisable2Reg The value of Function disable 2 register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureIsh (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy,
  IN OUT UINT32                            *FuncDisable2Reg
  );

/**
  Configures PCH USB controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in,out] FuncDisableReg  Function Disable Register
  @param[in,out] FuncDisable2Reg Function Disable 2 Register

  @retval EFI_INVALID_PARAMETER   The parameter of PchPlatformPolicy is invalid
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureUsb (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN OUT  UINT32                              *FuncDisableReg,
  IN OUT  UINT32                              *FuncDisable2Reg
  );
#ifdef SATA_SUPPORT

/**
  Configures PCH Sata Controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in,out] FuncDisableReg  Function Disable Register
  @param[in] IoBase               IO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureSata (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar,
  IN OUT  UINT32                              *FuncDisableReg,
  IN      UINT32                              IoBase
  );

/**
  Do any final initialization on SATA controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureSataAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  );
#endif
/**
  Perform Clock Gating programming
  Enables clock gating in various PCH interfaces and the registers must be restored during S3 resume.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] PmcBase              PmcBase value of this PCH device
  @param[in] SpiBase              SpiBase value of this PCH device
  @param[in] FuncDisableReg       The Function Disable Register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureClockGating (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN  UINT32                              RootComplexBar,
  IN  UINT32                              PmcBase,
  IN  UINT32                              SpiBase,
  IN  UINT32                              FuncDisableReg
  );

/**
  Configure IoApic Controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureIoApic (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN  UINT32                              IlbBase
  );

/**
  Enforce S0ix for PCIe Ports.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol
  @param[in] PmcBase              PmcBase of the PCH

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   The PCIe Root Port Number of D28:F0 is not found
                                  or invalid
**/
EFI_STATUS
PchPciEnforceS0ix (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL           *PchPlatformPolicy,
  IN UINT32                                     PmcBase
  );
/**
  Dump whole DXE_PCH_PLATFORM_POLICY_PROTOCOL and serial out.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance

**/
VOID
PchDumpPlatformProtocol (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  );
#endif
