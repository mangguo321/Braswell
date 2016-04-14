/** @file
  This protocol provides the memory information data, such as
  total physical memory size, memory frequency, memory size 
  of each dimm and rank. 

  This protocol is EFI compatible.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MEM_INFO_PROTOCOL_H_
#define _MEM_INFO_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                   gMemInfoProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _MEM_INFO_PROTOCOL MEM_INFO_PROTOCOL;

//
// Protocol definitions
//

#define CH_NUM    2
#define DIMM_NUM  1
#define RANK_NUM  2

#pragma pack(1)
typedef struct {
  UINT32  memSize;
  UINT16  ddrFreq;
  UINT8   ddrType;
  BOOLEAN EccSupport;
  UINT16  dimmSize[CH_NUM * DIMM_NUM];
  UINT8   DimmPresent[CH_NUM * DIMM_NUM];
  UINT8   *DimmsSpdData[CH_NUM * DIMM_NUM];
  UINT8   reserved;
  UINT16  reserved2;
  UINT8   BusWidth;
} MEMORY_INFO_DATA;
#pragma pack()

/**
  Data definition:

  memSize         Total physical memory size
  ddrFreq         DDR Frequency
  EccSupport      ECC Support
  dimmSize        Dimm Size
  DimmExist       Dimm Present or not
  RankInDimm      No. of ranks in a dimm
  
**/

//
// Protocol definition
//
struct _MEM_INFO_PROTOCOL {
  MEMORY_INFO_DATA  MemInfoData;
};

#endif
