/** @file
  SMM KSC library implementation.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
                   
**/

#include "SmmKscLib.h"

BOOLEAN mSmmKscLibInitialized = FALSE;

//
// Function implemenations
//
/**
  Initialize the library.
  The SMM library only requires SMM IO library and has no initialization.
  However, this must be called prior to use of any other KSC library functions
  for future compatibility.

  @Return  EFI_SUCCESS          KscLib is successfully initialized.

**/
EFI_STATUS
InitializeKscLib (
  VOID
  ) 
{
  // 
  // Fail if EC doesn't exist.
  //
  if (SmmIoRead8(KSC_C_PORT) == 0xff) {
    mSmmKscLibInitialized = FALSE;
    return EFI_DEVICE_ERROR;
  }

  mSmmKscLibInitialized = TRUE;

  return EFI_SUCCESS;
}

/**
  Sends command to Keyboard System Controller.

  @param     Command           Command byte to send

  @retval  EFI_SUCCESS         Command success
  @retval  EFI_DEVICE_ERROR    Command error

**/
EFI_STATUS
SendKscCommand (
  UINT8   Command
  ) 
{
  UINTN   Index;
  UINT8   KscStatus = 0;

  // 
  // Verify if KscLib has been initialized, NOT if EC dose not exist.
  //
  if (mSmmKscLibInitialized == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  Index = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus); 
  while (((KscStatus & KSC_S_IBF) != 0) && (Index < KSC_TIME_OUT)) {
    SmmStall (15);
    ReceiveKscStatus (&KscStatus);
    Index++;
  }
  if (Index >= KSC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Send the KSC command
  //
  SmmIoWrite8 (KSC_C_PORT, Command);

  return EFI_SUCCESS;
}

/**
  Receives status from Keyboard System Controller.

  @param  KscStatus           Status byte to receive 

  @Return   EFI_SUCCESS      Always success

**/
EFI_STATUS
ReceiveKscStatus (
  UINT8   *KscStatus
  )
{
  // 
  // Verify if KscLib has been initialized, NOT if EC dose not exist.
  //
  if (mSmmKscLibInitialized == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Read and return the status 
  //
  *KscStatus = SmmIoRead8 (KSC_C_PORT);
  
  return EFI_SUCCESS;
}

/**
  Sends data to Keyboard System Controller.

  @param   Data                   Data byte to send

  @retval EFI_SUCCESS             Success
  @retval EFI_DEVICE_ERROR        Error

**/
EFI_STATUS
SendKscData (
  UINT8   Data
  ) 
{
  UINTN   Index;
  UINT8   KscStatus;

  // 
  // Verify if KscLib has been initialized, NOT if EC dose not exist.
  //
  if (mSmmKscLibInitialized == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  Index = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_IBF) != 0) && (Index < KSC_TIME_OUT)) {
    SmmStall (15);
    ReceiveKscStatus (&KscStatus);
    Index++;
  }
  if (Index >= KSC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }
  
  //
  // Send the data and return
  //
  SmmIoWrite8 (KSC_D_PORT, Data);

  return EFI_SUCCESS;
}

/**
  Receives data from Keyboard System Controller.

  @param   Data                Data byte received

  @retval  EFI_SUCCESS         Read success
  @retval  EFI_DEVICE_ERROR    Read error

**/
EFI_STATUS
ReceiveKscData (
  UINT8   *Data
  ) 
{
  UINTN   Index;
  UINT8   KscStatus;

  // 
  // Verify if KscLib has been initialized, NOT if EC dose not exist.
  //
  if (mSmmKscLibInitialized == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  
  Index = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_OBF) == 0) && (Index < KSC_TIME_OUT)) {
    SmmStall (15);
    ReceiveKscStatus (&KscStatus);
    Index++;
  }
  if (Index >= KSC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Read KSC data and return
  //
  *Data = SmmIoRead8 (KSC_D_PORT);

  return EFI_SUCCESS;
}
