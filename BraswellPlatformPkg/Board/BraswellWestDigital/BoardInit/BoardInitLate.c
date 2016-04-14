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
#include "BoardGpios.h"

VOID
SAI_SettingOfGpioFamilies (
  GPIO_SAI_INIT*    SAI_Conf_Data,
  UINT32 familySize
  );

VOID
BraswellWestDigitalGpioInit (
  VOID
  )
{
  CHV_GPIO_PAD_INIT   GpioInitTable[3];

  DEBUG ((EFI_D_ERROR, "Programming BSW CR Board Gpio Tables which are not done properly by FSP...\n"));
    
  //
  // Copy the default settings for first three external gpios
  //
  CopyMem (GpioInitTable, mBSW_CH_GpioInitData_N, (sizeof(CHV_GPIO_PAD_INIT) * 3 ));
    
  //
  // Update the gpio setting with the values from patchable pcds
  //
  // Update GPIO_SUS6
  GpioInitTable[0].padConfg0.padCnf0 = (UINT32)(PcdGet64 (PcdGPIOSUS6Configuration) & 0xFCFF8782);
  GpioInitTable[0].padConfg1.padCnf1 &= 0xFFFFFF00;
  GpioInitTable[0].padConfg1.padCnf1 |= (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS6Configuration), 32) & 0xFF);
  GpioInitTable[0].PAD_MISC.r.GPE_ENABLE = (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS6Configuration),40) & 0x03);
  GpioInitTable[0].PAD_MISC.r.wake_able = (PcdGet64 (PcdGPIOSUS6Configuration) & BIT42) ? 1:0;
  GpioInitTable[0].PAD_MISC.r.intr_mask = (PcdGet64 (PcdGPIOSUS6Configuration) & BIT43) ? 1:0;
  GpioInitTable[0].PAD_MISC.r.Wake_Mask_Position = (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS6Configuration), 44) & 0xFF);
    
  // Update GPIO_SUS7
  GpioInitTable[1].padConfg0.padCnf0 = (UINT32)(PcdGet64 (PcdGPIOSUS7Configuration) & 0xFCFF8782);
  GpioInitTable[1].padConfg1.padCnf1 &= 0xFFFFFF00;
  GpioInitTable[1].padConfg1.padCnf1 |= (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS7Configuration), 32) & 0xFF);
  GpioInitTable[1].PAD_MISC.r.GPE_ENABLE = (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS7Configuration), 40) & 0x03);
  GpioInitTable[1].PAD_MISC.r.wake_able = (PcdGet64 (PcdGPIOSUS7Configuration) & BIT42) ? 1:0;
  GpioInitTable[1].PAD_MISC.r.intr_mask = (PcdGet64 (PcdGPIOSUS7Configuration) & BIT43) ? 1:0;
  GpioInitTable[1].PAD_MISC.r.Wake_Mask_Position = (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS7Configuration), 44) & 0xFF);
    
  // Update GPIO_SUS8
  GpioInitTable[2].padConfg0.padCnf0 = (UINT32)(PcdGet64 (PcdGPIOSUS8Configuration) & 0xFCFF8782);
  GpioInitTable[2].padConfg1.padCnf1 &= 0xFFFFFF00;
  GpioInitTable[2].padConfg1.padCnf1 |= (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS8Configuration), 32) & 0xFF);
  GpioInitTable[2].PAD_MISC.r.GPE_ENABLE = (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS8Configuration), 40) & 0x03);
  GpioInitTable[2].PAD_MISC.r.wake_able = (PcdGet64 (PcdGPIOSUS8Configuration) & BIT42) ? 1:0;
  GpioInitTable[2].PAD_MISC.r.intr_mask = (PcdGet64 (PcdGPIOSUS8Configuration) & BIT43) ? 1:0;
  GpioInitTable[2].PAD_MISC.r.Wake_Mask_Position = (UINT32) (RShiftU64(PcdGet64 (PcdGPIOSUS8Configuration), 44) & 0xFF);    
   
  // PAD programming to initialize GPIOs with SCI/SMI support, which depends on FSP PchInit to program basic chipset resources.
  InternalGpioPADConfig(3, (sizeof(mBSW_CH_GpioInitData_N)/sizeof(mBSW_CH_GpioInitData_N[0]) - 3),   mBSW_CH_GpioInitData_N);
    
  // Initialize the three gpios at the gpio header
  InternalGpioPADConfig(0, sizeof(GpioInitTable)/sizeof(GpioInitTable[0]),   GpioInitTable);   
    
  // GPIO lock if really want
  SAI_SettingOfGpioFamilies(mBSW_CH_GPIO_SAI_Init_East, sizeof(mBSW_CH_GPIO_SAI_Init_East)/sizeof(mBSW_CH_GPIO_SAI_Init_East[0]));
  SAI_SettingOfGpioFamilies(mBSW_CH_GPIO_SAI_Init_North, sizeof(mBSW_CH_GPIO_SAI_Init_North)/sizeof(mBSW_CH_GPIO_SAI_Init_North[0]));
  SAI_SettingOfGpioFamilies(mBSW_CH_GPIO_SAI_Init_SouthEast, sizeof(mBSW_CH_GPIO_SAI_Init_SouthEast)/sizeof(mBSW_CH_GPIO_SAI_Init_SouthEast[0]));
  SAI_SettingOfGpioFamilies(mBSW_CH_GPIO_SAI_Init_SouthWest, sizeof(mBSW_CH_GPIO_SAI_Init_SouthWest)/sizeof(mBSW_CH_GPIO_SAI_Init_SouthWest[0]));
}

/**
  This function performs Board initialization in Pre-Memory.

  @retval     EFI_SUCCESS           The PPI is installed and initialized.
  @retval     EFI ERRORS            The PPI is not successfully installed.
  @retval     EFI_OUT_OF_RESOURCES  No enough resoruces (such as out of memory).
**/
EFI_STATUS
EFIAPI
BraswellWestDigitalInitLateConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_PLATFORM_INFO_HOB            *PlatformInfoHob;

  PlatformInfoHob = PcdGetPtr (PcdPlatformInfo);
  if (PlatformInfoHob->BoardId == BOARD_ID_BSW_WD) {
    PcdSet64 (PcdGpioInitFunc, (UINT64)(UINTN)BraswellWestDigitalGpioInit);
  }

  return EFI_SUCCESS;
}
