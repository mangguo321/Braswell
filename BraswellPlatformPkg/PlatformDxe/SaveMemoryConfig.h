/** @file
  Header file for Save Previous Memory Configuration Driver.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#ifndef _SAVE_MEMORY_CONFIG_DRIVER_H
#define _SAVE_MEMORY_CONFIG_DRIVER_H

#include <Guid/PlatformInfo.h>
#include <Library/Hoblib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/UefiLib.h>
#include <Guid/HobList.h>
#include <Guid/MemoryConfigData.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/Chv2Variable.h>
#include <Guid/SetupVariable.h>
#include <Library/IoLib.h>

extern EFI_GUID  gFspNonVolatileStorageHobGuid;

/**
  This is the standard EFI driver that detects whether there is a
  MemoryConfigurationData HOB and, if so, saves its data to nvRAM.

  @return  EFI_SUCCESS     The data is successfully saved or there was no data
  @return  EFI_NOT_FOUND   The HOB list could not be located.
  @return  EFI_UNLOAD_IMAGE It is not success.
**/
EFI_STATUS
EFIAPI
SaveMemoryConfigEntryPoint (
  VOID
  );
#endif
