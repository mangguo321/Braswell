/** @file
  SMM I/O access utility implementation file, for Ia32

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// Include files
//
#include <Library/SmmIoLib.h>
#include <Library/IoLib.h>
#include "PchAccess.h"
#include <Protocol/SmmBase2.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

/**
  Do a one byte IO read

  @param[in]  Address - IO address to read
  
  @retval   Data read

**/  
UINT8
SmmIoRead8 (
  IN  UINT16    Address
  )
{
  UINT8   Buffer;
  
  ASSERT (gSmst);

  gSmst->SmmIo.Io.Read (
                  &gSmst->SmmIo, 
                  SMM_IO_UINT8, 
                  Address, 
                  1, 
                  &Buffer
                  );
  return Buffer;
}

/**
  Do a one byte IO write

  @param[in]  Address - IO address to write
  @param[in]  Data    - Data to write

**/
VOID
SmmIoWrite8 (
  IN  UINT16    Address,
  IN  UINT8     Data
  )
{
  ASSERT (gSmst);

  gSmst->SmmIo.Io.Write (
                    &gSmst->SmmIo, 
                    SMM_IO_UINT8, 
                    Address, 
                    1, 
                    &Data
                    );
}

/**
  Do a two byte IO read

  @param[in]  Address - IO address to read
  
  @retval   Data read

**/  
UINT16
SmmIoRead16 (
  IN  UINT16    Address
  )
{
  UINT16      Buffer;
  
  ASSERT (gSmst);

  gSmst->SmmIo.Io.Read (
                  &gSmst->SmmIo, 
                  SMM_IO_UINT16, 
                  Address,
                  1, 
                  &Buffer 
                  );
  return Buffer;
}

/**
  Do a two byte IO write

  @param[in]  Address - IO address to write
  @param[in]  Data    - Data to write

**/
VOID
SmmIoWrite16 (
  IN  UINT16    Address,
  IN  UINT16    Data
  )
{
  ASSERT (gSmst);

  gSmst->SmmIo.Io.Write ( 
                    &gSmst->SmmIo, 
                    SMM_IO_UINT16, 
                    Address,
                    1, 
                    &Data 
                    );
}

/**
  Do a four byte IO read

  @param[in]  Address - IO address to read
  
  @retval   Data read

**/    
UINT32
SmmIoRead32 (
  IN  UINT16    Address
  )
{
  UINT32        Buffer;

  ASSERT (gSmst);

  gSmst->SmmIo.Io.Read ( 
                    &gSmst->SmmIo, 
                    SMM_IO_UINT32, 
                    Address,
                    1, 
                    &Buffer 
                    );
  return Buffer;
}

/**
  Do a four byte IO write

  @param[in]  Address - IO address to write
  @param[in]	Data    - Data to write

**/
VOID
SmmIoWrite32 (
  IN  UINT16    Address,
  IN  UINT32    Data
  )
{
  ASSERT (gSmst);

  gSmst->SmmIo.Io.Write ( 
                    &gSmst->SmmIo, 
                    SMM_IO_UINT32, 
                    Address,
                    1, 
                    &Data 
                    );
}

/**
  Delay for at least the request number of microseconds.
  Timer used is ACPI time counter, which has 1us granularity.
    
  @param[in]  Microseconds - Number of microseconds to delay.

**/
VOID
SmmStall (
  IN  UINTN   Microseconds
  )
{
  UINTN   Ticks;
  UINTN   Counts;
  UINTN   CurrentTick;
  UINTN   OriginalTick;
  UINTN   RemainingTick;
  UINT16  mAcpiBaseAddr;

  if (Microseconds == 0) {
    return;
  }
  
  mAcpiBaseAddr = PchLpcPciCfg16 (R_PCH_LPC_ACPI_BASE) & B_PCH_LPC_ACPI_BASE_BAR;
   
  OriginalTick = SmmIoRead32 (mAcpiBaseAddr + R_PCH_ACPI_PM1_TMR);
  CurrentTick = OriginalTick;
  
  //
  // The timer frequency is 3.579545 MHz, so 1 ms corresponds 3.58 clocks
  //
  Ticks = Microseconds * 358 / 100 + OriginalTick + 1;
  
  //
  // The loops needed by timer overflow
  //
  Counts = Ticks / ICH_ACPI_TIMER_MAX_VALUE;
  
  //
  // Remaining clocks within one loop
  //
  RemainingTick = Ticks % ICH_ACPI_TIMER_MAX_VALUE;
  
  //
  // not intend to use TMROF_STS bit of register PM1_STS, because this adds extra
  // one I/O operation, and maybe generate SMI
  //
  while ((Counts != 0) || (RemainingTick > CurrentTick)) {
    CurrentTick = SmmIoRead32 (mAcpiBaseAddr + R_PCH_ACPI_PM1_TMR);
    //
    // Check if timer overflow 
    //
    if (CurrentTick < OriginalTick) {
      Counts--;   
    }
    OriginalTick = CurrentTick;
  }
}
