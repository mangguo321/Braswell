/** @file
  PCH GPIO Porting driver.

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
#include "BoardGpios.h"

VOID
SAI_SettingOfGpioFamilies (
  GPIO_SAI_INIT*    SAI_Conf_Data,
  UINT32 familySize
  );

VOID
BraswellCRGpioLateInit (
  VOID
  )
{

    DEBUG ((EFI_D_ERROR, "Programming BSW CR Board Gpio Tables which are not done properly by FSP...\n"));
   
    // PAD programming to initialize GPIOs with SCI/SMI support, which depends on FSP PchInit to program basic chipset resources.
    InternalGpioPADConfig(0, sizeof(mBSW_CR_GpioInitData_N)/sizeof(mBSW_CR_GpioInitData_N[0]),   mBSW_CR_GpioInitData_N);
    
    // GPIO lock if really want
    SAI_SettingOfGpioFamilies(mBSW_GPIO_SAI_Init_East, sizeof(mBSW_GPIO_SAI_Init_East)/sizeof(mBSW_GPIO_SAI_Init_East[0]));
    SAI_SettingOfGpioFamilies(mBSW_GPIO_SAI_Init_North, sizeof(mBSW_GPIO_SAI_Init_North)/sizeof(mBSW_GPIO_SAI_Init_North[0]));
    SAI_SettingOfGpioFamilies(mBSW_GPIO_SAI_Init_SouthEast, sizeof(mBSW_GPIO_SAI_Init_SouthEast)/sizeof(mBSW_GPIO_SAI_Init_SouthEast[0]));
    SAI_SettingOfGpioFamilies(mBSW_GPIO_SAI_Init_SouthWest, sizeof(mBSW_GPIO_SAI_Init_SouthWest)/sizeof(mBSW_GPIO_SAI_Init_SouthWest[0]));
}

/**
  This function performs Board initialization in Pre-Memory.

  @retval     EFI_SUCCESS           The PPI is installed and initialized.
  @retval     EFI ERRORS            The PPI is not successfully installed.
  @retval     EFI_OUT_OF_RESOURCES  No enough resoruces (such as out of memory).
**/
EFI_STATUS
EFIAPI
BraswellCRInitLateConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_PLATFORM_INFO_HOB            *PlatformInfoHob;

  PlatformInfoHob = PcdGetPtr (PcdPlatformInfo);
  if (PlatformInfoHob->BoardId == BOARD_ID_BSW_CR) {
    PcdSet64 (PcdGpioInitFunc, (UINT64)(UINTN)BraswellCRGpioLateInit);
  }

  return EFI_SUCCESS;
}
