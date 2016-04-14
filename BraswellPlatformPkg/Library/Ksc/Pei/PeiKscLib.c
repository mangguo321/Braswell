/** @file
  Ksc settings in PEI phase.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PeiKsc.h"

//
// Function implemenations
//
/**
  The PEI function requires CPU IO protocol, through which it reads KSC Command port
  and ensures that EC exists or not.
   
  Retval EFI_SUCCESS         EC found and KscLib is successfully initialized.
  Retval EFI_DEVICE_ERROR    EC is NOT present on the system.

**/ 
EFI_STATUS
InitializeKscLib ( )
{
  //
  // Read from EC Command/Status port (0x66), if value is 0xFF means EC dose not exist.
  //
  if (IoRead8 (KSC_C_PORT) == 0xFF) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;

}

/**
  Sends command to Keyboard System Controller.

  @param[in]  Command           Command byte to send
 
  @retval  EFI_SUCCESS          Command success
  @retval  EFI_DEVICE_ERROR     Command error
  @retval  EFI_TIMEOUT          Command timeout

**/
EFI_STATUS
SendKscCommand (
  IN UINT8                Command
  ) 
{
  UINTN   Index = 0;
  UINT8   KscStatus = 0;

  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_IBF) != 0) && (Index < KSC_TIME_OUT)) {
    ReceiveKscStatus (&KscStatus);
    Index++;
  }

  if (Index >= KSC_TIME_OUT) {
    DEBUG ((EFI_D_INFO,  "SendKscCommand Time Out Error\n"));
    return EFI_TIMEOUT;
  }
  //
  // Send the KSC command
  //
  IoWrite8 (KSC_C_PORT, Command);

  return EFI_SUCCESS;
}

/**
  Receives status from Keyboard System Controller.

  @param[out] KscStatus         Status byte to receive
 
  @retval  EFI_DEVICE_ERROR     Ksc library has not initialized yet or KSC not present
  @retval  EFI_SUCCESS          Get KSC status successfully

**/
EFI_STATUS
ReceiveKscStatus (
  OUT UINT8    *KscStatus
  )
{
  *KscStatus = IoRead8 (KSC_C_PORT);

  return EFI_SUCCESS;
}

/**
  Sends data to Keyboard System Controller.

  @param[in]  Data             Data byte to send
 
  @retval  EFI_SUCCESS         Success
  @retval  EFI_DEVICE_ERROR    Error
  @retval  EFI_TIMEOUT         Command timeout

**/
EFI_STATUS
SendKscData (
  IN UINT8              Data
  ) 
{
  UINTN   Index = 0;
  UINT8   KscStatus = 0;
  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_IBF) != 0) && (Index < KSC_TIME_OUT)) {
    ReceiveKscStatus (&KscStatus);
    Index++;
  }

  if (Index >= KSC_TIME_OUT) {
    DEBUG ((EFI_D_INFO,  "SendKscData Time Out Error\n"));
    return EFI_TIMEOUT;
  }
  //
  // Write Data at EC Port (0x62)
  //
  IoWrite8 (KSC_D_PORT, Data);

  return EFI_SUCCESS;
}

/**
  Receives data from Keyboard System Controller.

  @param[out] Data               Data byte received
 
  @retval  EFI_SUCCESS           Read success
  @retval  EFI_DEVICE_ERROR      Read error
  @retval  EFI_TIMEOUT           Command timeout

**/
EFI_STATUS
ReceiveKscData (
  OUT UINT8           *Data
  ) 
{
  UINTN         Index = 0;
  UINT8         KscStatus = 0;  
  //
  // Wait for KSC to be ready (with a timeout)
  //
  ReceiveKscStatus (&KscStatus);
  while (((KscStatus & KSC_S_OBF) == 0) && (Index < KSC_TIME_OUT)) {
    ReceiveKscStatus (&KscStatus);
    Index++;
  }

  if (Index >= KSC_TIME_OUT) {
    DEBUG ((EFI_D_INFO,  "ReceiveKscData Time Out Error\n"));
    return EFI_TIMEOUT;
  }
  //
  // Read KSC data and return
  //
  *Data = IoRead8 (KSC_D_PORT);

  return EFI_SUCCESS;
}

