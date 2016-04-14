/** @file
  GPIO setting for Platform.
  This file includes package header files, library classes.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _BOARDGPIOS_H_
#define _BOARDGPIOS_H_

#include <PiPei.h>
#include "PchAccess.h"
#include "PlatformBaseAddresses.h"
#include <../MultiPlatformLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/Timerlib.h>
#include <Library/PchPlatformLib.h>
#include <Library/GpioLib.h>
#include <Guid/PlatformInfo.h>
#include <Ppi/Smbus2.h>

#define GPIO_DFX1_PAD0            0x4418
#define GPIO_DFX3_PAD0            0x4408
#define GPIO_MF_ISH_I2C1_SCL_PAD0 0x4810
#define GPIO_MF_ISH_I2C1_SDA_PAD0 0x4858
#define GPIO_PCIE_CLKREQ0B_PAD0   0x5C00
#define GPIO_SEC_GPIO_SUS8_PAD0   0x4840
#define GPIO_SEC_GPIO_SUS8_PAD1   0x4844
#define GPIO_SEC_GPIO_SUS9_PAD0   0x4860
#define GPIO_SEC_GPIO_SUS9_PAD1   0x4864
#define GPIO_SEC_GPIO_SUS10_PAD0  0x4808
#define GPIO_SEC_GPIO_SUS10_PAD1  0x480C
#define GPIO_I2C_NFC_SCL_PAD0     0x5038
#define GPIO_I2C_NFC_SCL_PAD1     0x503C
#define GPIO_I2C_NFC_SDA_PAD0     0x5020
#define GPIO_I2C_NFC_SDA_PAD1     0x5024
#define GPIO_MF_ISH_GPIO_7_PAD0   0x4808

#define USB3_CAMERA                0
#define MIPI_CAMERA                1
#define DISABLE                    0
#define NO_PULL_UP                 0
#define SECURE_NFC_ENABLE          1

#define GPIO_SOC_RUNTIME_SCI_N  0x4850

/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************
/// ***************************************************     CHERRYVIEW GPIO CONFIGURATION   *************************************************
/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************
/// *****************************************************************************************************************************************

#define ENABLE  1
#define DISABLE 0

#endif
