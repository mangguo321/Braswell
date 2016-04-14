/** @file
  Gpio setting programming for platform.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <BoardGpios.h>

typedef
VOID
(EFIAPI *GPIO_INIT) (
  VOID
  );


/**
  Set GPIO PAD SAI registers for N/E/SW/SE GPIO communities

  @param  SAI_Conf_Data         GPIO_SAI_INIT data array for each GPIO communities.

**/
VOID
SAI_SettingOfGpioFamilies (
  GPIO_SAI_INIT*    SAI_Conf_Data,
  UINT32 familySize
  )
{

  UINT32 count=0;
  DEBUG ((EFI_D_ERROR, "++SAI Setting of GPIO families%x\n ",familySize));

  //
  // community SAI programming
  //
  for (count=0;count < familySize;count++) {
    MmioWrite32 (SAI_Conf_Data[count].Offset, SAI_Conf_Data[count].val_.secSAI);
  }

   DEBUG ((EFI_D_ERROR, "--SAI Setting of GPIO families %x\n ",familySize));
}

/**
  Returns the Correct GPIO table for Mobile/Desktop respectively.
  Before call it, make sure PlatformInfoHob->BoardId&PlatformFlavor is get correctly.

  @param[in] PeiServices            General purpose services available to every PEIM.
  @param[in] PlatformInfoHob        PlatformInfoHob pointer with PlatformFlavor specified.

  @retval  EFI_SUCCESS               The function completed successfully.
  @retval  EFI_DEVICE_ERROR          KSC fails to respond.

**/
EFI_STATUS
MultiPlatformGpioProgram (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PLATFORM_INFO_HOB      *PlatformInfoHob
  )
{
  CHV_CONF_PAD0                     lpadvar;
  GPIO_INIT                         GpioInit;

  DEBUG ((EFI_D_ERROR, "MultiPlatformGpioProgram()...\n"));

  //
  // Clearing Interrupt unmask registers
  //
  MmioWrite32 (IO_BASE_ADDRESS + GPIO_MMIO_OFFSET_N + GPIO_INTERRUPT_MASK, 0);
  MmioWrite32 (IO_BASE_ADDRESS + GPIO_MMIO_OFFSET_E + GPIO_INTERRUPT_MASK, 0);
  MmioWrite32 (IO_BASE_ADDRESS + GPIO_MMIO_OFFSET_SW + GPIO_INTERRUPT_MASK, 0);
  MmioWrite32 (IO_BASE_ADDRESS + GPIO_MMIO_OFFSET_SE + GPIO_INTERRUPT_MASK, 0);
  MmioWrite32 (IO_BASE_ADDRESS + GPIO_MMIO_OFFSET_VIRT + GPIO_INTERRUPT_MASK, 0);

  GpioInit = (GPIO_INIT)(UINTN)PcdGet64 (PcdGpioInitFunc);
  if (GpioInit != NULL) {
    GpioInit ();
  }

  //
  // TODO: W/A [HSD # 4963135] HPD GPIOs drive 0 in mode 1.
  //
  lpadvar.padCnf0 = 0;
  lpadvar.r.Pmode = 0x1;
  lpadvar.r.RXTXEnCfg = 0x3;

  MmioOr32 (IO_BASE_ADDRESS+0x8000 + 0x5408,lpadvar.padCnf0 );  // HV_DDI0_HPD
  MmioOr32 (IO_BASE_ADDRESS+0x8000 + 0x5420,lpadvar.padCnf0 );  // HV_DDI1_HPD
  MmioOr32 (IO_BASE_ADDRESS+0x8000 + 0x5440,lpadvar.padCnf0 );  // HV_DDI2_HPD

  //
  // Set SDCard RCOMP Trigger Delay to 5ms
  //
  MmioWrite32 (IO_BASE_ADDRESS + R_PCH_CFIO_SOUTHEAST + 0x1080, 0x1E848);

  return EFI_SUCCESS;
}

