/** @file
  GUID used for extended VBT Info Data entries in the HOB list.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _VBT_EXT_INFO_H_
#define _VBT_EXT_INFO_H_
#include <PiPei.h>

#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>

#define EFI_VBT_EXT_INFO_GUID \
  { \
    0x82411907, 0xdbc9, 0x4ece, 0xbe, 0x7c, 0xd0, 0x74, 0x4f, 0x19, 0x61, 0x26 \
  }

#pragma pack(1)

typedef struct _EFI_VBT_EXT_INFO_HOB {
  EFI_PHYSICAL_ADDRESS        VbtExtAddress;
  UINT32                      VbtExtSize;  
} EFI_VBT_EXT_INFO_HOB;

#pragma pack()

extern EFI_GUID gEfiVbtExtInfoGuid;

#endif
