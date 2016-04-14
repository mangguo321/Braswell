/** @file
  Header file for SMM Access Driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PEI_SMM_ACCESS_DRIVER_H
#define _PEI_SMM_ACCESS_DRIVER_H

#include "PiPei.h"
#include "Library/HobLib.h"
#include "Guid/SmramMemoryReserve.h"
#include "Ppi/SmmAccess.h"
#include "Library/DebugLib.h"
#include "Library/BaseMemoryLib.h"
#include "ChvAccess.h"

//
// Chipset specific constants
// 
#define MAX_SMRAM_RANGES 2

//
// Chipset register(s)
//
typedef UINT8 SMRAM;        // System Management RAM Control

//
// Private data structure
// 
#define  SMM_ACCESS_PRIVATE_DATA_SIGNATURE SIGNATURE_32 ('i', 's', 'm', 'a')

typedef struct {
    UINTN                           Signature;
    EFI_HANDLE                      Handle;
    PEI_SMM_ACCESS_PPI              SmmAccess;
    //
    // Local Data for SMM Access interface goes here
    //
    UINTN                           NumberRegions;
    EFI_SMRAM_DESCRIPTOR            SmramDesc[MAX_SMRAM_RANGES];
} SMM_ACCESS_PRIVATE_DATA;

#define SMM_ACCESS_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, SMM_ACCESS_PRIVATE_DATA, SmmAccess, SMM_ACCESS_PRIVATE_DATA_SIGNATURE)

//
// Prototypes
// Driver model protocol interface
//

EFI_STATUS
EFIAPI
SmmAccessDriverEntryPoint(
  IN EFI_PEI_FILE_HANDLE       FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

EFI_STATUS
EFIAPI
Open (
  IN EFI_PEI_SERVICES        **PeiServices,
  IN PEI_SMM_ACCESS_PPI      *This,
  IN UINTN                   DescriptorIndex
  );

EFI_STATUS
EFIAPI
Close (
  IN EFI_PEI_SERVICES        **PeiServices,
  IN PEI_SMM_ACCESS_PPI      *This,
  IN UINTN                   DescriptorIndex
  );

EFI_STATUS
EFIAPI
Lock (
  IN EFI_PEI_SERVICES        **PeiServices,
  IN PEI_SMM_ACCESS_PPI      *This,
  IN UINTN                   DescriptorIndex
  );

EFI_STATUS
EFIAPI
GetCapabilities (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN PEI_SMM_ACCESS_PPI          *This,
  IN OUT   UINTN                 *SmramMapSize, 
  IN OUT   EFI_SMRAM_DESCRIPTOR  *SmramMap
  );

#endif
