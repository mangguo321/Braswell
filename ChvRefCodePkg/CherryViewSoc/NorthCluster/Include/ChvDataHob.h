/** @file
  The GUID definition for SaDataHob

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CHV_DATA_HOB_H_
#define _CHV_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID gChvDataHobGuid;

#define GP_ENABLE   1
#define GP_DISABLE  0

#pragma pack(1)
//
// HgMode settings
//
typedef enum {
  HgModeDisabled = 0,
  HgModeReserved,
  HgModeMuxless,
  HgModeDgpu,
  HgModeMax
} HG_MODE;

//
// HG GPIO Data Structure
//
typedef struct {
  UINT32   CommunityOffset;  //< GPIO Community
  UINT32   PinOffset;        //< GPIO Pin
  BOOLEAN  Active;           //< 0=Active Low; 1=Active High
} HG_GPIO;

//
// HG Info HOB
//
typedef struct _HG_INFO_HOB {
  EFI_HOB_GUID_TYPE EfiHobGuidType;
  UINT8             RevisionId;     //< Revision ID
  HG_MODE           HgMode;
  BOOLEAN           HgGpioSupport;  //< 0=Not Supported; 1=Supported.
  HG_GPIO           HgDgpuHoldRst;
  HG_GPIO           HgDgpuPwrEnable;
  UINT16            HgDelayAfterPwrEn;
  UINT16            HgDelayAfterHoldReset;
} HG_INFO_HOB;

//
// CherryView Data Hob
//
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                    //< GUID Hob type structure
  HG_INFO_HOB              HgInfo;                            //< HG Info HOB
} CHV_DATA_HOB;
#pragma pack()
#endif
