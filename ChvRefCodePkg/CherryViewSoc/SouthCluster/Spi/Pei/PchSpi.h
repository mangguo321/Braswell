/** @file
  Header file for the PCH SPI Runtime Driver.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_SPI_H_
#define _PCH_SPI_H_

#include <Library/MemoryAllocationLib.h>

#include <Ppi/Spi.h>
#include "SpiCommon.h"

typedef struct {
  EFI_PEI_PPI_DESCRIPTOR  PpiDescriptor;
  SPI_INSTANCE            SpiInstance;
} PEI_SPI_INSTANCE;

/**
  This function is a hook for Spi Pei phase specific initialization
  
**/
VOID
EFIAPI
SpiPhaseInit (
  VOID
  );

/**
  This function is a hook for Spi to disable BIOS Write Protect

**/
VOID
EFIAPI
DisableBiosWriteProtect (
  IN UINTN        SpiBase
  );

/**
  This function is a hook for Spi to enable BIOS Write Protect

**/
VOID
EFIAPI
EnableBiosWriteProtect (
  IN UINTN        SpiBase
  );

#endif
