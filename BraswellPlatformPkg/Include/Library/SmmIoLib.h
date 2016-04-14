/** @file
  This library provides SMM functions for IO and PCI IO access.
  These can be used to save size and simplify code.
  All contents must be runtime and SMM safe.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SMM_IO_LIB_H_
#define _SMM_IO_LIB_H_

#include "PiDxe.h"
#include <Protocol/SmmCpuIo2.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
//
// Utility consumed protocols
//
#include <Protocol/SmmBase2.h>

//
// Global variables that must be defined and initialized to use this library
//
extern EFI_SMM_SYSTEM_TABLE2                *mSmst;

//
// Pci I/O related data structure deifinition
//
typedef enum {
  SmmPciWidthUint8  = 0,
  SmmPciWidthUint16 = 1,
  SmmPciWidthUint32 = 2,
  SmmPciWidthUint64 = 3,
  SmmPciWidthMaximum
} SMM_PCI_IO_WIDTH;

#define SMM_PCI_ADDRESS(bus,dev,func,reg) \
   ((UINT64) ( (((UINT32)bus) << 24) + \
               (((UINT32)dev) << 16) + \
               (((UINT32)func) << 8) + \
               ( (UINT32)reg)) )

typedef struct {
  UINT8     Register;
  UINT8     Function;
  UINT8     Device;
  UINT8     Bus;
  UINT32    ExtendedRegister;
} SMM_PCI_IO_ADDRESS;

//
// CPU I/O Access Functions
// 

UINT8
SmmIoRead8 (
  IN  UINT16    Address
  );
  
VOID
SmmIoWrite8 (
  IN  UINT16    Address,
  IN  UINT8     Data
  );
  
UINT16
SmmIoRead16 (
  IN  UINT16    Address
  );

VOID
SmmIoWrite16 (
  IN  UINT16    Address,
  IN  UINT16    Data
  );

UINT32
SmmIoRead32 (
  IN  UINT16    Address
  );

VOID
SmmIoWrite32 (
  IN  UINT16    Address,
  IN  UINT32    Data
  );

VOID
SmmMemWrite8 (
  IN  UINT64    Dest,
  IN  UINT8     Data
  );

UINT8
SmmMemRead8 (
  IN  UINT64    Dest
  );

VOID
SmmMemWrite16 (
  IN  UINT64    Dest,
  IN  UINT16    Data
  );

UINT16
SmmMemRead16 (
  IN  UINT64    Dest
  );

VOID
SmmMemWrite32 (
  IN  UINT64    Dest,
  IN  UINT32    Data
  );

UINT32
SmmMemRead32 (
  IN  UINT64    Dest
  );

VOID
SmmMemAnd32 (
  IN  UINT64    Dest,
  IN  UINT32    Data
  );
//
// Pci Configuration Space access functions definition
//

/**
  Read value from the specified PCI config space register

  @param[in]   Width               The width (8, 16 or 32 bits) of accessed pci config space register
  @param[in]   Address             The address of the accessed pci register (bus, dev, func, offset)
  @param[in, out] Buffer           The returned value

  @retval  EFI_SUCCESS             All operations successfully
  @retval  EFI_INVALID_PARAMETER   Width is not valid or dosn't match register address
  @retval  Other error code        If any error occured when calling libiary functions 
    
**/
EFI_STATUS
SmmPciCfgRead (
  IN SMM_PCI_IO_WIDTH       Width,
  IN SMM_PCI_IO_ADDRESS     *Address,
  IN OUT VOID               *Buffer
  );
  
/**
  Write value into the specified PCI config space register

  @param[in]  Width                The width (8, 16 or 32 bits) of accessed pci config space register
  @param[in]  Address              The address of the accessed pci register (bus, dev, func, offset)
  @param[in, out] Buffer           The returned value

  @retval  EFI_SUCCESS             All operations successfully
  @retval  EFI_INVALID_PARAMETER   Width is not valid or dosn't match register address
  @retval  Other error code        If any error occured when calling libiary functions 
  
**/
EFI_STATUS
SmmPciCfgWrite (
  IN SMM_PCI_IO_WIDTH       Width,
  IN SMM_PCI_IO_ADDRESS     *Address,
  IN OUT VOID               *Buffer
  );

#endif
