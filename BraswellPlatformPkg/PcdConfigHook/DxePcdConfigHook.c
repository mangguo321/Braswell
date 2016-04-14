/** @file
  PCD DXE driver manage all PCD entry initialized in PEI phase and DXE phase, and
  produce the implementation of native PCD protocol and EFI_PCD_PROTOCOL defined in
  PI 1.2 Vol3.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Protocol/PiPcd.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/SetupVariable.h>

typedef struct {
  GUID    *PcdGuid;
  UINTN   Token;
  CHAR16  *VarName;
  GUID    *VarGuid;
  UINTN   Size;
  UINT32  Attributes;
} PCD_HOOK_STRUCT;

PCD_HOOK_STRUCT  mPcdHookToken[] = {
  {&gEfiEdkIIPlatformTokenSpaceGuid, 0, L"Setup", &gEfiSetupVariableGuid, sizeof(SYSTEM_CONFIGURATION), EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS},
};

PCD_HOOK_STRUCT *
GetPcdHookStructByToken (
  IN UINTN Token
  )
{
  UINTN  Index;

  for (Index = 0; Index < sizeof(mPcdHookToken)/sizeof(mPcdHookToken[0]); Index++) {
    if (mPcdHookToken[Index].Token == Token) {
      return &mPcdHookToken[Index];
    }
  }
  return NULL;
}

EFI_STATUS
EFIAPI
PcdAccessOnSetHook (
  IN     EFI_GUID *Guid     OPTIONAL,
  IN     UINTN    CallBackToken,
  IN     VOID     *TokenData,
  IN     UINTN    TokenDataSize
  )
{
  PCD_HOOK_STRUCT                 *PcdHookStruct;
  EFI_STATUS                      Status;

  DEBUG ((EFI_D_INFO, "PcdAccessOnSetHook - 0x%x\n", CallBackToken));
  
  PcdHookStruct = GetPcdHookStructByToken (CallBackToken);
  ASSERT (PcdHookStruct == NULL);

  Status  = gRT->SetVariable (
                   PcdHookStruct->VarName,
                   PcdHookStruct->VarGuid,
                   PcdHookStruct->Attributes,
                   TokenDataSize,
                   TokenData
                   );
  DEBUG ((EFI_D_INFO, "PcdAccessOnSetHook - %r\n", Status));

  return Status;
}

EFI_STATUS
EFIAPI
DxePcdConfigHookEntrypoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_PCD_PROTOCOL    *PcdProtocol;
  UINTN               Index;
  
  Status = gBS->LocateProtocol (
                  &gEfiPcdProtocolGuid,
                  NULL,
                  &PcdProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  mPcdHookToken[0].Token = PcdTokenEx(&gEfiEdkIIPlatformTokenSpaceGuid, PcdSystemConfiguration);
  
  for (Index = 0; Index < sizeof(mPcdHookToken)/sizeof(mPcdHookToken[0]); Index++) {
    Status = PcdProtocol->CallbackOnSet (mPcdHookToken[Index].PcdGuid, mPcdHookToken[Index].Token, PcdAccessOnSetHook);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}