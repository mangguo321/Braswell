/** @file
  Dxe Platform Policy driver init.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CHV_PLATFORM_POLICY_INIT_DXE_H_
#define _CHV_PLATFORM_POLICY_INIT_DXE_H_

//
// CHV Platform Policy init
//
/**
  @todo	add desceription

**/
EFI_STATUS
ChvPlatformPolicyInit (
#ifndef FSP_FLAG
  IN EFI_HANDLE     ImageHandle
#else
  IN CONST EFI_PEI_SERVICES    **PeiServices
#endif
  );

#endif
