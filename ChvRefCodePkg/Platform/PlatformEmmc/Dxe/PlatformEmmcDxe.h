/** @file
  Header file for Platform eMMC Info Dxe Driver.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PLATFORM_EMMC_INFO_DRIVER_H_
#define _PLATFORM_EMMC_INFO_DRIVER_H_

#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>

#include <MediaDeviceDriver.h>

#include <PchAccess.h>
#include <Library/PchPlatformLib.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/PchSccTuning.h>
#include <Protocol/EmmcCardInfoProtocol.h>

#define B_EMMC_HS400_12V          BIT7  ///< HS400 Dual Data Rate eMMC @ 200MHz - 1.2V I/O
#define B_EMMC_HS400_18V          BIT6  ///< HS400 Dual Data Rate eMMC @ 200MHz - 1.8V I/O
#define B_EMMC_HS200_12V          BIT5  ///< HS200 Single Data Rate eMMC @ 200MHz - 1.2V I/O
#define B_EMMC_HS200_18V          BIT4  ///< HS200 Single Data Rate eMMC @ 200MHz - 1.8V I/O
#define V_EMMC_HS200_BUS_WIDTH_4  1     ///< BUS_WIDTH [183] 4 bit data bus (SDR)
#define V_EMMC_HS200_BUS_WIDTH_8  2     ///< BUS_WIDTH [183] 8 bit data bus (SDR)
#endif
