/** @file
  Header file for MCH power management functionality

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MCH_PPM_LIB_H_
#define _MCH_PPM_LIB_H_

//
// Statements that include other files
//
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Initialize the MCH support library.
  This must be called once during driver initialization before using
  any of the other library services provided.

  @param[in]    None

  @retval   EFI_SUCCESS      The library has been initialized correctly.

**/
EFI_STATUS
InitializeMchLib (
  VOID
  );

/**
  Determine the processor core frequency 

  @param[in]  None

  @retval    Processor core frequency multiplied by 3

**/
UINT16
DetermineiFsbFromMsr (
  VOID
  );

/**
  Determines if MCH is capable of dynamic FSB frequency switching(Bus Geyserville)

  @param[in]   None

  @retval  FALSE         Dynamic FSB frequency switching(Bus Geyserville) is NOT supported.
  @retval  TRUE          Dynamic FSB frequency switching(Bus Geyserville) is supported. 

**/
BOOLEAN
MchSupportDynamicFsbFrequencySwitching (
  VOID
  );

/**
  Enables dynamic FSB frequency switching(Bus Geyserville) on MCH

  @param[in]  None

  @retval   EFI_SUCCESS   Dynamic FSB frequency switching(Bus Geyserville) enabled

**/
EFI_STATUS
EnableMchDynamicFsbFrequencySwitching (
  VOID
  );

#endif
