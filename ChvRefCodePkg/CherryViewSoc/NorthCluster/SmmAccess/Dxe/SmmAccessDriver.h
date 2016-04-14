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

#ifndef _SMM_ACCESS_DRIVER_H_
#define _SMM_ACCESS_DRIVER_H_

#include <Cherryview.h>
#include <ChvAccess.h>
#include <Protocol/SmmAccess2.h>
#include <Protocol/PciRootBridgeIo.h>

#define SMM_ACCESS_PRIVATE_DATA_SIGNATURE   SIGNATURE_32('4','5','s','a')

//
// Chipset specific constants
// 

#define MAX_SMRAM_RANGES 2

//
// PCI "address" of chipset
//
#define PCI_BUS 0
#define PCI_DEV 0
#define PCI_FNC 0

//
// Chipset identifying registers
//
#define CHIPSET_VID       0x8086
#define CHIPSET_DID       0xA000

//
// Chipset register(s)
//

// SMM configuration register
typedef UINT8 SMRAM;        // System Management RAM Control
#define SMRAM_OFFSET        HUNIT_HSMMCTL

//
// Private data
// 
/**
  @todo	add description

**/
typedef struct {
    UINTN                           Signature;
    EFI_HANDLE                      Handle;
    EFI_SMM_ACCESS2_PROTOCOL        SmmAccess;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
    //
    // Local Data for SMM Access interface goes here
    //
    UINTN                           NumberRegions;
    EFI_SMRAM_DESCRIPTOR            SmramDesc[MAX_SMRAM_RANGES];
} SMM_ACCESS_PRIVATE_DATA;

#define SMM_ACCESS_PRIVATE_DATA_FROM_THIS(a) \
  CR(a, SMM_ACCESS_PRIVATE_DATA, SmmAccess, SMM_ACCESS_PRIVATE_DATA_SIGNATURE)

//
// Prototypes
// Driver model protocol interface
//

EFI_STATUS
EFIAPI
SmmAccessDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
EFIAPI
Open (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  );

EFI_STATUS
EFIAPI
Close (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  );

EFI_STATUS
EFIAPI
Lock (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  );

EFI_STATUS
EFIAPI
GetCapabilities (
  IN CONST EFI_SMM_ACCESS2_PROTOCOL    *This,
  IN OUT UINTN                   *SmramMapSize, 
  IN OUT EFI_SMRAM_DESCRIPTOR    *SmramMap
  );

#endif
