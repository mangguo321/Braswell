/** @file
  This library contains power management configuration functions for 
  CherryView processors.

  Acronyms:
    PPM   Platform Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology
    HT    Hyper-Threading Technology

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
#include <CherryviewPpmLib.h>
#include "Cherryview.h"
#include <Library/S3BootScriptLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

//
// Global variables
//

//
// CHV iFSB Frequency Table.
//

CONST UINT16 miFSBFrequencyTable[] = {
  100,          // 100MHz
  100,          // 100MHz
  133,          // 133.4MHz
   83,          // 83.3MHz
   80,          // 80MHz
   90,          // 90MHz
   93,          // 93.3MHz
  100,          // 100MHz
};

UINT32 Isb32Read(UINT8 portid, UINT8 command, UINT16 offset){
  UINT32 data;
  MmioWrite32(EC_BASE + 0xD8, 0);
  MmioWrite32(EC_BASE + 0xD0, (((command << 24) | (portid << 16) | (offset << 8)) + 0xF0));
  data = MmioRead32(EC_BASE + 0xD4);
  return data;
}

/**
  Determine the processor core frequency 

  @param[in]  None
  
  @retval   Processor core frequency multiplied by 3

**/
UINT16
DetermineiFsbFromMsr (
  VOID
  )
{
  //
  // Determine the processor core frequency 
  //
  UINT16    FrequencyIndex;
  UINT16    Hpl_Mdiv;

  FrequencyIndex = (Isb32Read(0x14, 0x06, 0x0008) & 0x1C) >> 2 ;
  DEBUG((EFI_D_INFO, "FrequencyIndex - %x \n", FrequencyIndex));
  if (FrequencyIndex == 4) {
    Hpl_Mdiv = Isb32Read(0x14, 0x06, 0x54) & 0x3F;
    DEBUG((EFI_D_INFO, "Hpl_Mdiv - %x \n", Hpl_Mdiv));
    if (Hpl_Mdiv == 0x26)
      return miFSBFrequencyTable[FrequencyIndex];
    else if (Hpl_Mdiv == 0x19)
      return miFSBFrequencyTable[FrequencyIndex + 1];
    else if (Hpl_Mdiv == 0x1A)
      return miFSBFrequencyTable[FrequencyIndex + 2];
  }
  else if (FrequencyIndex == 5) {
      if ((FrequencyIndex + 3) < (sizeof(miFSBFrequencyTable)/sizeof(miFSBFrequencyTable[0]))) {
         return miFSBFrequencyTable[FrequencyIndex + 3];
      }
  } 

  return miFSBFrequencyTable[FrequencyIndex];
}

/**
  Determines if MCH is capable of dynamic FSB frequency switching(Bus Geyserville)

  @param[in]  None

  @retval  FALSE        Dynamic FSB frequency switching(Bus Geyserville) is NOT supported.
  @retval  TRUE         Dynamic FSB frequency switching(Bus Geyserville) is supported. 

**/
BOOLEAN
MchSupportDynamicFsbFrequencySwitching (
  VOID
  )
{
  return FALSE;      
}

/**
  Enables dynamic FSB frequency switching(Bus Geyserville) on ICH

  @param[in]  None

  @retval  EFI_SUCCESS   Dynamic FSB frequency switching(Bus Geyserville) enabled

**/
EFI_STATUS
EnableMchDynamicFsbFrequencySwitching (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

