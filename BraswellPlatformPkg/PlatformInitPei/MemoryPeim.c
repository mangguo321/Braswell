/** @file
  Tiano PEIM to provide the platform support functionality.
  This file implements the Platform Memory Range PPI.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformEarlyInit.h"

//
// Need min. of 48MB PEI phase
//
#define  PEI_MIN_MEMORY_SIZE               (6 * 0x800000)
#define  PEI_RECOVERY_MIN_MEMORY_SIZE      (6 * 0x800000)

//
// This is the memory needed for PEI to start up DXE.
//
// Over-estimating this size will lead to higher fragmentation
// of main memory.  Under-estimation of this will cause catastrophic
// failure of PEI to load DXE.  Generally, the failure may only be
// realized during capsule updates.
//
#define PRERESERVED_PEI_MEMORY ( \
  EFI_SIZE_TO_PAGES (3 * 0x800000)   /* PEI Core memory based stack          */ \
  )

EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  { EfiACPIReclaimMemory,       0x2B  },    // ASL
  { EfiACPIMemoryNVS,           0xCF  },    // ACPI NVS
  { EfiReservedMemoryType,      0xC8  },    // BIOS Reserved
  { EfiRuntimeServicesCode,     0x100 },
  { EfiRuntimeServicesData,     0x200 },
  { EfiMaxMemoryType,           0     }
};

/**
  Publish Memory Type Information.

  @param   None

  @retval  EFI_SUCCESS     Success.
  @retval  Others          Errors have occurred.
**/
EFI_STATUS
EFIAPI
PublishMemoryTypeInfo (
  void
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  UINTN                           DataSize;
  EFI_MEMORY_TYPE_INFORMATION     MemoryData[EfiMaxMemoryType + 1];

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             &Variable
             );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "WARNING: Locating Pei variable failed 0x%x \n", Status));
    DEBUG((EFI_D_ERROR, "Build Hob from default\n"));
    //
    // Build the default GUID'd HOB for DXE
    //
    BuildGuidDataHob (&gEfiMemoryTypeInformationGuid, mDefaultMemoryTypeInformation, sizeof (mDefaultMemoryTypeInformation) );

    return Status;
  }

  DataSize = sizeof (MemoryData);
  //
  // This variable is saved in BDS stage. Now read it back
  //
  Status = Variable->GetVariable (
                       Variable,
                       EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME,
                       &gEfiMemoryTypeInformationGuid,
                       NULL,
                       &DataSize,
                       &MemoryData
                       );
  if (EFI_ERROR (Status)) {
    //
    // build default
    //
    DEBUG((EFI_D_ERROR, "Build Hob from default\n"));
    BuildGuidDataHob (&gEfiMemoryTypeInformationGuid, mDefaultMemoryTypeInformation, sizeof (mDefaultMemoryTypeInformation) );
  } else {
    //
    // Build the GUID'd HOB for DXE from variable
    //
    DEBUG((EFI_D_ERROR, "Build Hob from variable \n"));
    BuildGuidDataHob (&gEfiMemoryTypeInformationGuid, MemoryData, DataSize);
  }

  return Status;
}

