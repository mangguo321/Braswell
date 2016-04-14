/** @file
  Header file for SA Initialization Driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _VLV_INITIALIZATION_DRIVER_H_
#define _VLV_INITIALIZATION_DRIVER_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#ifndef FSP_FLAG
#include <Library/UefiBootServicesTableLib.h>
#include "IgdOpRegion.h"
#include "PiDxe.h"
#endif

#include <Protocol/ChvPlatformPolicyProtocol.h>
#include "Cherryview.h"

#ifdef FSP_FLAG
#define  S3BootScriptSaveMemWrite(a,b,c,d)
#define  S3BootScriptSaveMemReadWrite(a,b,c,d)
#define  S3BootScriptSaveIoWrite(a,b,c,d)
#define  S3BootScriptSaveMemPoll(a,b,c,d,e,f)
#endif

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//

typedef struct {
  UINT64  BaseAddr;
  UINT32  Offset;
  UINT32  AndMask;
  UINT32  OrMask;
} BOOT_SCRIPT_REGISTER_SETTING;

typedef struct {
  UINT64  BaseAddr;
  UINT32  Offset;
  UINT32  AndMask;
  UINT32  OrMask;
  UINT32  act;
} BIOS_WA_SCRIPT_REG_SETTING;

typedef union{
    UINT32 sbReqTrigger;
    struct {
         UINT32 SB_Busy:1;           // 0    A write to this register will set this bit = '1' and triggers a IOSF SB request.
         UINT32 SB_BAR:3;            // 1-3  BAR value
         UINT32 SB_ByteEnables:4;    // 4-7  Byte enables to be used for the iOSF SB access.
         UINT32 SB_Port:8;           // 8-15 Port to be used for the IOSF SB access.
         UINT32 SB_Opcode:8;        // 16-23 Opcode to be used for the IOSF SB access.
         UINT32 SB_DevFn:8;         // 24-31 Device and Function number to be used for the IOSF SB access.
       }r;
} SB_REQ_TRIGGER;

/**
  This function prints the DXE phase platform policy.

  @param[in] DxePlatformSaPolicy - Instance of DXE_CHV_PLATFORM_POLICY_PROTOCOL
**/
VOID
ChvDxePolicyDump (
  IN    DXE_CHV_PLATFORM_POLICY_PROTOCOL   *DxePlatformSaPolicy
  );

#endif

