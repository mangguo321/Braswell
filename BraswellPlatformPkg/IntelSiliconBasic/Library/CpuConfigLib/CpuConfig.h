/** @file
  Header for the status code data hub logging component

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#include <PiDxe.h>

#include <Protocol/MpService.h>

#include <Library/CpuConfigLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/SocketLga775Lib.h>

/**
  Worker function to create a new feature entry.
  This is a worker function to create a new feature entry. The new entry is added to
  the feature list by other functions in the library.

  @param[in]  FeatureID          The ID of the feature.
  @param[in]  Attribute          Feature-specific data.
  
  @return     CPU_FEATURE_ENTRY  The address of the created entry.

**/
CPU_FEATURE_ENTRY *
CreateFeatureEntry (
  IN  CPU_FEATURE_ID      FeatureID,
  IN  VOID                *Attribute
  );

/**
  Worker function to search for a feature entry in processor feature list.
  This is a worker function to search for a feature entry in processor feature list.

  @param[in]  ProcessorNumber    Handle number of specified logical processor
  @param[in]  FeatureIndex       The index of the new node in feature list.

  @retval     CPU_FEATURE_ENTRY  Pointer to the feature node. If not found, NULL is returned.

**/
CPU_FEATURE_ENTRY *
SearchFeatureEntry (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               FeatureIndex
  );

#endif
