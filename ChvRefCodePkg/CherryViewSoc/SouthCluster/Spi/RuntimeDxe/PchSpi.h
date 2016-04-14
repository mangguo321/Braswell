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

#include "SpiCommon.h"
#include <Protocol/Spi.h>
#include <Guid/EventGroup.h>
#include <TianoApi.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeRuntimePciLibPciExpress.h>
#include <Library/UefiRuntimeLib.h>

//
// Function prototypes used by the SPI protocol.
//

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.

**/
VOID
EFIAPI
PchSpiVirtualAddressChangeEvent (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  );

/**
  This function is a hook for Spi Dxe phase specific initialization

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
