/** @file
  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MANIFEST_H_
#define _MANIFEST_H_

#define MAX_DIGEST_SIZE         64
#define SHA256_TOTAL_BYTE       32
#define MANIFEST_ADDRESS        0xFFFE0058

typedef struct _MANIFEST_OEM_DATA {
  UINT32    Signature;
  UINT8     SystemFirmwareGuid[16];
  UINT32    BIOSVersion;
  UINT32    BIOSCompatibleVersion;
  UINT8     Bios2ndStageHashLen;
  UINT8     Sha256Hash[32];
  UINT8     Reserved1[7];
  UINT8     OSPubKeyHash[32];
  UINT8     OSBLPubKeyHash[32];
  UINT8     Reserved2[32];
  UINT8     RecoveryFVHashLen;
  UINT8     RecoveryFVHash[32];
  UINT8     Reserved[32];
  UINT32    IFWIVersionLen;
  UINT8     IFWIVersion[32];
  UINT8     Reserved3[131];
} MANIFEST_OEM_DATA;

#endif

