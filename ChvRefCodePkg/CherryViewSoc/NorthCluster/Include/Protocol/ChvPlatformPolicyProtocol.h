/** @file
  Interface definition details between MCH and platform drivers during DXE phase.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CHV_PLATFORM_POLICY_PROTOCOL_H_
#define _CHV_PLATFORM_POLICY_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
#ifndef FSP_FLAG
extern EFI_GUID gDxeChvPlatformPolicyGuid;
#endif

//
// Protocol revision number
// Any backwards compatible changes to this protocol will result in an update in the revision number
// Major changes will require publication of a new protocol
//
#define DXE_CHV_PLATFORM_POLICY_PROTOCOL_REVISION 0

typedef struct {
  UINT8  PFITStatus;
  UINT8  IgdTheramlSupport;
  UINT8  ALSEnabled;
  UINT8  LidStatus;
} IGD_PANEL_FEATURES;

//
// CHV DXE Platform Policiy ==================================================
//

#define NO_AUDIO   0
#define HD_AUDIO   1
#define LPE_AUDIO  2

typedef struct _DXE_CHV_PLATFORM_POLICY_PROTOCOL {
  UINT8                   Revision;
  IGD_PANEL_FEATURES      IgdPanelFeatures;
  UINT8                   EnableRenderStandby;
  UINT8                   MaxInverterPWM;
  UINT8                   MinInverterPWM;
  UINT8                   ForceWake;
  UINT8                   EuControl;
  UINT8                   PmWeights;
  UINT8                   PavpMode;
  UINT8                   S0ixSupported;
  UINT8                   AudioTypeSupport;
  UINT8                   EnableIGDTurbo;
  UINT8                   PowerMeterLock;
  UINT8                   PunitPwrConfigDisable;
  UINT8                   Wopcmsz;
  UINT32                  PAVP_PR3_MemLength;
  UINT8                   RPMBLock;
  UINT8                   PavpAsmf;
} DXE_CHV_PLATFORM_POLICY_PROTOCOL;

#endif
