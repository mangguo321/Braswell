/** @file
  General IOSF-SB structure and register definitions.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _IOSF_SB_DEFINITIONS_H_
#define _IOSF_SB_DEFINITIONS_H_

///
/// Message Bus Registers
///
#define MC_MCR            0x000000D0 // Cunit Message Control Register
#define MC_MDR            0x000000D4 // Cunit Message Data Register
#define MC_MCRX           0x000000D8 // Cunit Message Control Register Extension
#define MC_MCRXX          0x000000DC // Cunit Message Control Register Extension 2

///
/// Message Bus API
///
#define MSG_BUS_ENABLED   0x000000F0
#define MSGBUS_MASKHI     0xFFFFFF00
#define MSGBUS_MASKLO     0x000000FF
#define MESSAGE_DWORD_EN  BIT4 | BIT5 | BIT6 | BIT7

typedef struct _PCH_MSG_BUS_TABLE_STRUCT {
  UINT8       PortId;
  UINT32      Address;
  UINT32      AndMask;
  UINT32      OrMask;
  UINT8       ReadOpCode;
  UINT8       WriteOpCode;
} PCH_MSG_BUS_TABLE_STRUCT;

#endif
