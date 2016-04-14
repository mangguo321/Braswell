/** @file
  The definition of Memory Attribute.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _EFI_MEMORY_ATTRIB_H
#define _EFI_MEMORY_ATTRIB_H

extern UINT32 mUsedMtrr;
/**
  @todo	add description

**/
typedef struct {
  UINT32  Msr;
  UINT32  BaseAddress;
  UINT32  Length;
} EFI_FIXED_MTRR;
/**
  @todo	add description

**/
typedef struct {
  UINT64  BaseAddress;
  UINT64  Length;
  UINT64  Type;
  UINT32  Msr;
  BOOLEAN Valid;
} EFI_VARIABLE_MTRR;

#define EFI_MEMORY_CACHETYPE_MASK (EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB | EFI_MEMORY_UCE)

EFI_STATUS
ProgramFixedMtrr (
  IN  UINT64                    MemoryCacheType,
  IN  UINT64                    *Base,
  IN  UINT64                    *Length
  );

VOID
PreMtrrChange (
  VOID
  );
VOID
PostMtrrChange (
  VOID
  );

EFI_STATUS
GetMemoryAttribute (
  VOID
  );

BOOLEAN
CheckMemoryAttributeOverlap (
  IN  EFI_PHYSICAL_ADDRESS      Start,
  IN  EFI_PHYSICAL_ADDRESS      End
  );

EFI_STATUS
CombineMemoryAttribute (
  IN  UINT64                    Attribute,
  IN  UINT64                    *Base,
  IN  UINT64                    *Length
  );

EFI_STATUS
GetDirection (
  IN  UINT64                    Input,
  IN  UINTN                     *MtrrNumber,
  IN  BOOLEAN                   *Direction
  );

UINT64
Power2MaxMemory (
  IN UINT64                     MemoryLength
  );

EFI_STATUS
InvariableMtrr (
  IN  UINTN                     MtrrNumber,
  IN  UINTN                     Index
  );

EFI_STATUS
ProgramVariableMtrr (
  IN  UINTN                     MtrrNumber,
  IN  EFI_PHYSICAL_ADDRESS      BaseAddress,
  IN  UINT64                    Length,
  IN  UINT64                    MemoryCacheType
  );

EFI_STATUS
CleanupVariableMtrr (
  VOID
  );

UINT64
GetMemorySpaceAttributeFromMtrrType (
  IN UINT8                      MtrrAttribute
  );

EFI_STATUS
RefreshGcdMemoryAttributes (
  VOID
  );

EFI_STATUS
SearchGcdMemorySpaces (
  IN EFI_GCD_MEMORY_SPACE_DESCRIPTOR     *MemorySpaceMap,
  IN UINTN                               NumberOfDescriptors,
  IN EFI_PHYSICAL_ADDRESS                BaseAddress,
  IN UINT64                              Length,
  OUT UINTN                              *StartIndex,
  OUT UINTN                              *EndIndex
  );

EFI_STATUS
SetGcdMemorySpaceAttributes (
  IN EFI_GCD_MEMORY_SPACE_DESCRIPTOR     *MemorySpaceMap,
  IN UINTN                               NumberOfDescriptors,
  IN EFI_PHYSICAL_ADDRESS                BaseAddress,
  IN UINT64                              Length,
  IN UINT64                              Attributes
  );
#endif
