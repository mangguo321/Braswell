/** @file
  A helper driver to save information to SMRAM after SMRR is enabled.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiSmm.h>
#include <PchRegs.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmReadyToLock.h>
#include <Protocol/SmmControl2.h>

#define SMM_FROM_SMBASE_DRIVER        0x55
#define SMM_FROM_CPU_DRIVER_SAVE_INFO 0x81

#define EFI_SMRAM_CPU_NVS_HEADER_GUID \
  { \
    0x429501d9, 0xe447, 0x40f4, 0x86, 0x7b, 0x75, 0xc9, 0x3a, 0x1d, 0xb5, 0x4e \
  }

UINT8    mSmiDataRegister;
BOOLEAN  mLocked = FALSE;
EFI_GUID mSmramCpuNvsHeaderGuid = EFI_SMRAM_CPU_NVS_HEADER_GUID;

/**
  Dispatch function for a Software SMI handler.

  @param[in]  DispatchHandle        The handle of this dispatch function.
  @param[in]  DispatchContext       The pointer to the dispatch function's context.
                                The SwSmiInputValue field is filled in
                                by the software dispatch driver prior to
                                invoking this dispatch function.
                                The dispatch function will only be called
                                for input values for which it is registered.

  @return None

**/
EFI_STATUS
SmramSaveInfoHandler (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext
  )
{
  EFI_STATUS Status;
  UINT64     VarData[3];
  EFI_PHYSICAL_ADDRESS      SmramDataAddress;
  UINTN      VarSize;

  if (DispatchContext == NULL) {
    ASSERT(DispatchContext != NULL);
    return EFI_UNSUPPORTED;
  }
  ASSERT (DispatchContext->SwSmiInputValue == SMM_FROM_SMBASE_DRIVER);

  if (!mLocked && IoRead8 (mSmiDataRegister) == SMM_FROM_CPU_DRIVER_SAVE_INFO) {
    VarSize = sizeof (VarData);
    Status = gRT->GetVariable (
                    L"SmramCpuNvs",
                    &mSmramCpuNvsHeaderGuid,
                    NULL,
                    &VarSize,
                    VarData
                    );
    if (!EFI_ERROR (Status) && VarSize == sizeof (VarData)) {
       Status = gSmst->SmmAllocatePages (
                         AllocateAnyPages,
                         EfiRuntimeServicesData,
                       #if (defined X64_BUILD_SUPPORT) && (X64_BUILD_SUPPORT == 1)
                         EFI_SIZE_TO_PAGES (VarData[2]),
                       #else
                         EFI_SIZE_TO_PAGES ((UINT32)VarData[2]),
                       #endif
                         &SmramDataAddress
                         );
       ASSERT_EFI_ERROR (Status);
       DEBUG ((EFI_D_ERROR, "CPU SMRAM NVS Data - %x\n", SmramDataAddress));
       DEBUG ((EFI_D_ERROR, "CPU SMRAM NVS Data size - %x\n", VarData[2]));
       VarData[0] = (UINT64)SmramDataAddress;
       CopyMem (
         (VOID *)(UINTN)(VarData[0]),
         (VOID *)(UINTN)(VarData[1]),
         (UINTN)(VarData[2])
         );
	   Status = gRT->SetVariable (
                       L"SmramCpuNvs",
                       &mSmramCpuNvsHeaderGuid,
                       EFI_VARIABLE_BOOTSERVICE_ACCESS,
                       VarSize,
                       &VarData
                       );
    }
  }

  return EFI_SUCCESS;
}

/**
  Smm Ready To Lock event notification handler.

  It sets a flag indicating that SMRAM has been locked.
  
  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
SmmReadyToLockEventNotify (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  mLocked = TRUE;
  return EFI_SUCCESS;
}

/**
  Entry point function of this driver.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.  
  @param[in] SystemTable  A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
SmramSaveInfoHandlerSmmMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  VOID                          *Registration;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *SmmSwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT   SmmSwDispatchContext;
  EFI_HANDLE                    DispatchHandle;

  //
  // Get SMI data register
  //
  mSmiDataRegister = R_PCH_APM_STS;

  //
  // Register software SMI handler
  //

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSwDispatch2ProtocolGuid,
                    NULL,
                    (VOID **) &SmmSwDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  SmmSwDispatchContext.SwSmiInputValue = SMM_FROM_SMBASE_DRIVER;
  Status = SmmSwDispatch->Register (
                            SmmSwDispatch,
                            (EFI_SMM_HANDLER_ENTRY_POINT2)&SmramSaveInfoHandler,
                            &SmmSwDispatchContext,
                            &DispatchHandle
                            );
  ASSERT_EFI_ERROR (Status);

  //
  // Register SMM Ready To Lock Protocol notification
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmReadyToLockProtocolGuid,
                    SmmReadyToLockEventNotify,
                    &Registration
                    );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

