/** @file
  EFI PEIM support boot paths to provide the platform.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "Platform.h"
#include "PlatformBaseAddresses.h"
#include "PchRegs.h"
#include <Guid/SetupVariable.h>
#include <Ppi/MasterBootMode.h>
#include <Library/GpioLib.h>

static EFI_PEI_PPI_DESCRIPTOR       mPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiMasterBootModePpiGuid,
    NULL
  },
};


EFI_PEI_PPI_DESCRIPTOR mPpiListRecoveryBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiBootInRecoveryModePpiGuid,
  NULL
};

BOOLEAN
GetSleepTypeAfterWakeup (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  OUT UINT16                    *SleepType
  );

UINT32
GPIORead32 (
  IN  UINT32 mmio_conf
  )
{
  UINT32 conf_val;
  UINT32 i;
  conf_val = MmioRead32(mmio_conf);
  for(i=0;i<5;i++){
    if(conf_val == 0xffffffff)
      conf_val = MmioRead32(mmio_conf);
      else
        break;
  }

  return conf_val;
}

BOOLEAN
CheckIfRecoveryMode (
  IN CONST EFI_PEI_SERVICES    **PeiServices
)
{
  PAD_VAL   pad_val;

  //
  //
  // Use GPIO_SUS0 as Recovery Jmper
  // if short GPIO_SUS0 and Gnd then Recovery Jmper is setted
  //
  pad_val.dw = GPIORead32(IO_BASE_ADDRESS + GPIO_MMIO_OFFSET_N + 0x4800);
  if (pad_val.r.pad_val == 0) {
    DEBUG((EFI_D_INFO, "Recovery jumper setted!\n"));
    return TRUE;
  }
  return FALSE;
}

/**
  If the box was opened, it's boot with full config.
  If the box is closed, then
    1. If it's first time to boot, it's boot with full config .
    2. If the ChassisIntrution is selected, force to be a boot with full config
    3. Otherwise it's boot with no change.
  
  @param  PeiServices General purpose services available to every PEIM.   
  
  @retval TRUE  If it's boot with no change.
  
  @retval FALSE If boot with no change.
**/
BOOLEAN 
IsBootWithNoChange (
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  BOOLEAN  IsFirstBoot;
  BOOLEAN  EnableFastBoot;
  
  IsFirstBoot = PcdGetBool(PcdBootState);
  EnableFastBoot = PcdGetBool (PcdEnableFastBoot);

  DEBUG ((EFI_D_INFO, "IsFirstBoot = %x , EnableFastBoot= %x. \n", IsFirstBoot, EnableFastBoot));
  
  if ((!IsFirstBoot) && EnableFastBoot) {
  	PcdSetBool (PcdBootToFirmwareUserInterface, FALSE);
    return TRUE;
  } else {
    return FALSE;
  }
}


EFI_STATUS
UpdateBootMode (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS      Status;
  EFI_BOOT_MODE   BootMode;
  UINT16          SleepType;
#ifdef EFI_DEBUG
  CHAR16          *strBootMode;
#endif

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);

  ASSERT_EFI_ERROR (Status);
  if (BootMode  == BOOT_IN_RECOVERY_MODE) {
    return Status;
  }

  //
  // Let's assume things are OK if not told otherwise
  //
  BootMode = BOOT_WITH_FULL_CONFIGURATION;

  if (GetSleepTypeAfterWakeup (PeiServices, &SleepType)) {
    switch (SleepType) {
      case V_PCH_ACPI_PM1_CNT_S3:
        BootMode = BOOT_ON_S3_RESUME;
        break;

      case V_PCH_ACPI_PM1_CNT_S4:
//        BootMode = BOOT_ON_S4_RESUME;
        break;

      case V_PCH_ACPI_PM1_CNT_S5:
//        BootMode = BOOT_ON_S5_RESUME;
        break;
    } // switch (SleepType)
  }

  
  //
  // Check if we need to boot in forced recovery mode
  //
  if (CheckIfRecoveryMode(PeiServices)) {
    DEBUG ((EFI_D_INFO, "Boot mode on recovery mode\n"));
    Status = PeiServicesInstallPpi (&mPpiListRecoveryBootMode);
    BootMode  = BOOT_IN_RECOVERY_MODE;
  } else if (IsBootWithNoChange(PeiServices)) {
  	  if(BootMode != BOOT_ON_S3_RESUME) {
  	   BootMode = BOOT_ASSUMING_NO_CONFIGURATION_CHANGES;
  	  }  
  }

#ifdef EFI_DEBUG
  switch (BootMode) {
    case BOOT_WITH_FULL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION";
      break;
    case BOOT_WITH_MINIMAL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_MINIMAL_CONFIGURATION";
      break;
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
      strBootMode = L"BOOT_ASSUMING_NO_CONFIGURATION_CHANGES";
      break;
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS";
      break;
    case BOOT_WITH_DEFAULT_SETTINGS:
      strBootMode = L"BOOT_WITH_DEFAULT_SETTINGS";
      break;
    case BOOT_ON_S4_RESUME:
      strBootMode = L"BOOT_ON_S4_RESUME";
      break;
    case BOOT_ON_S5_RESUME:
      strBootMode = L"BOOT_ON_S5_RESUME";
      break;
    case BOOT_ON_S2_RESUME:
      strBootMode = L"BOOT_ON_S2_RESUME";
      break;
    case BOOT_ON_S3_RESUME:
      strBootMode = L"BOOT_ON_S3_RESUME";
      break;
    case BOOT_ON_FLASH_UPDATE:
      strBootMode = L"BOOT_ON_FLASH_UPDATE";
      break;
    case BOOT_IN_RECOVERY_MODE:
      strBootMode = L"BOOT_IN_RECOVERY_MODE";
      break;
    default:
      strBootMode = L"Unknown boot mode";
  } // switch (BootMode)

  DEBUG ((EFI_D_ERROR, "Setting BootMode to %s\n", strBootMode));
#endif
  Status = (*PeiServices)->SetBootMode (PeiServices, BootMode);
  ASSERT_EFI_ERROR (Status);
  
  Status = (*PeiServices)->InstallPpi (PeiServices, &mPpiList[0]);
  ASSERT_EFI_ERROR (Status);
  
  return Status;
}

/**
  Get sleep type after wakeup

  @param[in]  PeiServices       Pointer to the PEI Service Table.
  @param[out]  SleepType        Sleep type to be returned.

  @retval   TRUE                A wake event occured without power failure.
  @retval   FALSE               Power failure occured or not a wakeup.

**/
BOOLEAN
GetSleepTypeAfterWakeup (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  OUT UINT16                    *SleepType
  )
{
  UINT16  Pm1Sts;
  UINT16  Pm1Cnt;
  UINT16  GenPmCon1;

  //
  // VLV BIOS Specification 0.6.2 - Section 18.4, "Power Failure Consideration"
  //
  // When the SUS_PWR_FLR bit is set, it indicates the SUS well power is lost.
  // This bit is in the SUS Well and defaults to 1’b1 based on RSMRST# assertion (not cleared by any type of reset).
  // System BIOS should follow cold boot path if SUS_PWR_FLR (PBASE + 0x20[14]),
  // GEN_RST_STS (PBASE + 0x20[9]) or PWRBTNOR_STS (ABASE + 0x00[11]) is set to 1’b1
  // regardless of the value in the SLP_TYP (ABASE + 0x04[12:10]) field.
  //
  GenPmCon1 = MmioRead16 (PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1);
  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS);
  Pm1Cnt  = IoRead16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT);

  if ((GenPmCon1 & (B_PCH_PMC_GEN_PMCON_SUS_PWR_FLR | B_PCH_PMC_GEN_PMCON_GEN_RST_STS)) ||
     (Pm1Sts & B_PCH_ACPI_PM1_STS_PRBTNOR)) {
    // If power failure indicator, then don't attempt s3 resume.
    // Clear PM1_CNT of S3 and set it to S5 as we just had a power failure, and memory has
    // lost already.  This is to make sure no one will use PM1_CNT to check for S3 after
    // power failure.
    if ((Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP) == V_PCH_ACPI_PM1_CNT_S3) {
      Pm1Cnt = ((Pm1Cnt & ~B_PCH_ACPI_PM1_CNT_SLP_TYP) | V_PCH_ACPI_PM1_CNT_S5);
      IoWrite16 (ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
    }
  }
  //
  // Get sleep type if a wake event occurred and there is no power failure
  //
  if ((Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP) == V_PCH_ACPI_PM1_CNT_S3) {
    *SleepType = Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP;
    return TRUE;
  } else if ((Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP) == V_PCH_ACPI_PM1_CNT_S4) {
    *SleepType = Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP;
    return TRUE;
  }

  return FALSE;
}

