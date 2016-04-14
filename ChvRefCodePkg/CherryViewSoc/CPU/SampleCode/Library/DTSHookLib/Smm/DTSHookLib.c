/** @file
  Digital Thermal Sensor (DTS) SMM Library.
  This SMM Library configures and supports the DigitalThermalSensor features
  for the platform.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/DTSHookLib.h>

/**
  Prepare data and protocol for Dts Hooe Lib

  @retval	 EFI_SUCCESS - Initialize complete

**/
EFI_STATUS
InitializeDtsHookLib (
  VOID
)

{
  //
  //Nothing to do on CRB.
  //
  return EFI_SUCCESS;
}

/**
  Platform may need to register some data to private data structure before generate 
  software SMI or SCI.

**/
VOID
PlatformHookBeforeGenerateSCI (
  VOID
)

{
  //
  //Nothing to do on CRB.
  //
}

/**
  Read CPU temperature from platform diode

  @retval  TemperatureOfDiode   -  Return the CPU temperature of platform diode

**/
UINT8
ReadPlatformThermalDiode(
   VOID
)

{
   UINT8                               CurrentTemperatureOfDiode=0;
   EFI_STATUS                          Status;

  //
  // Call KSC to get Diode Temperature
  //    

  while (CurrentTemperatureOfDiode == 0) {
    Status = SendKscCommand (KSC_C_GET_DTEMP);
    if (Status == EFI_SUCCESS) {
      ReceiveKscData ((UINT8 *)&CurrentTemperatureOfDiode);  
    }  
  } 
  return CurrentTemperatureOfDiode;

}
/**
  When system temperature out of specification, do platform specific programming to prevent 
  system damage.

**/
VOID
PlatformEventOutOfSpec (
  VOID
)

{
   EFI_STATUS                          Status;
  //
  // Handle critical event by shutting down via EC
  //
  Status = InitializeKscLib ();
  if (Status == EFI_SUCCESS) {
    SendKscCommand (KSC_C_SYSTEM_POWER_OFF);
  }
}

