/** @file
  Power management driver.
  This SMM driver configures and supports the power management features
  for the platform.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// Statements that include other files
//
#include "PowerManagement.h"

//
// Global variables
//

//
// FVID Table Information (Must be initialized by caller)
//
extern FVID_TABLE                   *mFvidPointer;

//
// PPM Processor support protocol
//
PPM_PROCESSOR_SUPPORT_PROTOCOL_2    *mPpmProcessorSupportProtocol = NULL;

//
// Desired platform policy information
//
PPM_PLATFORM_POLICY_PROTOCOL        mPpmPlatformPolicyProtocol;

//
// Globals used by the reference code
//
EFI_GLOBAL_NVS_AREA                 *mGlobalNvsAreaPtr  = NULL;
UINT16                              mNumberOfStates     = 0;
UINT16                              mNumberOfStatesTurbo     = 0;

//
// Driver entry point
//

/**
  PPM must restore runtime state of MSR.  This is not supported by the S3 boot script.
  In order to accomplish this, the ASL is modified to generate an SMI on S3 in the _WAK method.
  This SMI handler reponds to that SW SMI.
  
  @param[in]

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

  @retval

  None.

**/
EFI_STATUS
S3RestoreMsrCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  )
{
  mPpmProcessorSupportProtocol->S3RestoreMsr (mPpmProcessorSupportProtocol);
  return EFI_SUCCESS;
} 

/**
  Initialize the power management support.
  This function will do boot time configuration:
    Install into SMRAM/SMM
    Detect HW capabilities and SW configuration
    Initialize HW and software state (primarily MSR and ACPI tables)
    Install SMI handlers for runtime interfacess

  @param[in]
  
  ImageHandle   - Pointer to the loaded image protocol for this driver
  SystemTable   - Pointer to the EFI System Table

  @retval

  EFI_SUCCESS   The driver installes/initialized correctly.
  Driver will ASSERT in debug builds on error.  PPM functionality is considered critical for mobile systems.

**/
EFI_STATUS
EFIAPI
InitializePowerManagement (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;  
  EFI_HANDLE                                Handle;
  EFI_HANDLE                                SwHandle;

  EFI_SMM_SW_DISPATCH2_PROTOCOL             *SwDispatch;
  EFI_GLOBAL_NVS_AREA_PROTOCOL              *GlobalNvsAreaProtocol;
  EFI_SMM_SW_REGISTER_CONTEXT               SwContext;

  PPM_PLATFORM_POLICY_PROTOCOL              *PpmPlatformPolicyProtocolPointer;

  Handle              = NULL;
  SwHandle            = NULL;

  //
  // Locate the ICH SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID **) &SwDispatch);
  ASSERT_EFI_ERROR (Status);
  
  //
  // Locate our shared data area
  //
  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, (VOID **) &GlobalNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  mGlobalNvsAreaPtr = GlobalNvsAreaProtocol->Area;

  //
  // Locate platform configuration information
  // Then copy it to a global variable that we can utilize during SMM/Runtime
  //
  Status = gBS->LocateProtocol (&gPpmPlatformPolicyProtocolGuid, NULL, (VOID **) &PpmPlatformPolicyProtocolPointer);
  ASSERT_EFI_ERROR (Status);
  CopyMem (&mPpmPlatformPolicyProtocol, PpmPlatformPolicyProtocolPointer, sizeof (PPM_PLATFORM_POLICY_PROTOCOL));
    
  //
  // Initialize the PPM processor support protocol
  // This is not currently a publicly exposed protocol, so the 
  // library just returns an updated pointer to the protocol
  // rather then using locate protocol services.
  //

  mPpmProcessorSupportProtocol = NULL;
  InitializeCherryviewPowerManagementLib(&mPpmProcessorSupportProtocol);

  if (mPpmProcessorSupportProtocol == NULL) {
    ASSERT(mPpmProcessorSupportProtocol != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Processor support library may or may not provide a P state transition function
  // If it does not, we fill in a generic function
  //
  if (mPpmProcessorSupportProtocol->PStateTransition == NULL) {
    mPpmProcessorSupportProtocol->PStateTransition = PpmTransition;
  }

  InitializeAslUpdateLib ();

  //
  // Register ACPI S3 MSR restore handler
  //
  SwContext.SwSmiInputValue = mPpmPlatformPolicyProtocol.S3RestoreMsrSwSmiNumber;
  Status = SwDispatch->Register (
    SwDispatch, 
    (EFI_SMM_HANDLER_ENTRY_POINT2) S3RestoreMsrCallback, 
    &SwContext, 
    &SwHandle
    );
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize PPM code (determine HW and configured state, configure hardware and software accordingly
  //
  InitializePpm (mPpmProcessorSupportProtocol);

  return EFI_SUCCESS;
}
