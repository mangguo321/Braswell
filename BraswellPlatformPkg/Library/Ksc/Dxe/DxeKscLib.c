/** @file
  Boot service DXE KSC library implementation.
  
  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "KscLib.h"
#include "DxeKsclib.h"

BOOLEAN mDxeKscLibInitialized = FALSE;

//
// Function implemenations
//
/**
  Initialize the library.
  Read KSC Command port (0x66), if found 0xff means no EC exists else EC exists 

  @return EFI_SUCCESS         KscLib is successfully initialized.

**/
EFI_STATUS
InitializeKscLib (
  VOID
  ) 
{
  if (IoRead8(KSC_C_PORT) == 0xff) {
    mDxeKscLibInitialized = FALSE;
    return EFI_DEVICE_ERROR;        // EC Doesn't exists
  }

  mDxeKscLibInitialized = TRUE;

  return EFI_SUCCESS;              // EC exists
}

/**
  Sends command to Keyboard System Controller.

  @param Command                Command byte to send

  @retval EFI_SUCCESS           Command success
  @retval EFI_DEVICE_ERROR      Command error

**/
EFI_STATUS
SendKscCommand (
  UINT8                 Command
  ) 
{
  UINTN         Index;
  UINT8         KscStatus = 0;

  Index = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus); 
  while (((KscStatus & KSC_S_IBF) != 0) && (Index < KSC_TIME_OUT)) {
    gBS->Stall (15);
    ReceiveKscStatus (&KscStatus);
    Index++;
  }
  if (Index >= KSC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Send the KSC command
  //
  DEBUG ((EFI_D_INFO, "SendKscCommand: command = %x\n", Command));
  IoWrite8(KSC_C_PORT, Command);

  return EFI_SUCCESS;
}

/**
  Receives status from Keyboard System Controller.

  @param  Status            Status byte to receive

  @retval  EFI_SUCCESS      Always success

**/
EFI_STATUS
ReceiveKscStatus (
  UINT8                 *KscStatus
  )
{ 
  //
  // Read and return the status 
  //
  *KscStatus = IoRead8(KSC_C_PORT);

  return EFI_SUCCESS;
}

/**
  Sends data to Keyboard System Controller.

  @param   Data                Data byte to send
  
  @retval  EFI_SUCCESS         Success
  @retval  EFI_TIMEOUT         Timeout
  @retval  Other               Failed

**/
EFI_STATUS
SendKscData (
  UINT8                 Data
  ) 
{
  UINTN         Index;
  UINT8         KscStatus;

  Index = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_IBF) != 0) && (Index < KSC_TIME_OUT)) {
    gBS->Stall (15);
    ReceiveKscStatus (&KscStatus);
    Index++;
  }
  if (Index >= KSC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }

  IoWrite8(KSC_D_PORT, Data);

  return EFI_SUCCESS;
}

/**
  Receives data from Keyboard System Controller.

  @param  Data                Data byte received

  @retval EFI_SUCCESS         Read success
  @retval EFI_DEVICE_ERROR    Read error

**/
EFI_STATUS
ReceiveKscData (
  UINT8   *Data
  ) 
{
  UINTN         Index;
  UINT8         KscStatus;  

  Index = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_OBF) == 0) && (Index < KSC_TIME_OUT)) {
    gBS->Stall (15);
    ReceiveKscStatus (&KscStatus);
    Index++;
  }
  if (Index >= KSC_TIME_OUT) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Read KSC data and return
  //
  *Data = IoRead8(KSC_D_PORT);

  return EFI_SUCCESS;
}
