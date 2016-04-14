/** @file
  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.   
                                                                                   
**/

#ifndef _EFI_SMM_SPI_DEVICE_H_
#define _EFI_SMM_SPI_DEVICE_H_

#include <Protocol/SpiDevice.h>

//
// GUID definition for protocol interface
//
#define SMM_SPI_DEVICE_PROTOCOL_GUID \
  {0xd963c5cd, 0x8cac, 0x498a, {0xbf, 0x78, 0xd1, 0x56, 0x49, 0x1, 0x85, 0x38}};

extern EFI_GUID gSmmSpiDeviceProtocolGuid;

//
// Common definition for regular and SMM versions of the protocol.
//
typedef SPI_DEVICE_PROTOCOL SMM_SPI_DEVICE_PROTOCOL;

#endif
