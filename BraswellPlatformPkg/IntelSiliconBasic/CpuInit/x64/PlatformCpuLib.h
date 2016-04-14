/** @file
  Library functions that can be called in both PEI and DXE phase.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PLATFORM_CPU_LIB_H_
#define _PLATFORM_CPU_LIB_H_

UINTN
CpuReadCr0 (
  VOID
  );

VOID
CpuWriteCr0 (
  UINTN   Value
  );

UINTN
CpuReadCr3 (
  VOID
  );

VOID
CpuWriteCr3 (
  UINTN   Value
  );

UINT8
CpuMemRead8 (
  IN  EFI_PHYSICAL_ADDRESS  Address
  );

UINT16
CpuMemRead16 (
  IN  EFI_PHYSICAL_ADDRESS  Address
  );

UINT32
CpuMemRead32 (
  IN  EFI_PHYSICAL_ADDRESS  Address
  );

UINT64
CpuMemRead64 (
  IN  EFI_PHYSICAL_ADDRESS  Address
  );

VOID
CpuMemWrite8 (
  IN  EFI_PHYSICAL_ADDRESS  Address,
  IN  UINT8                 Data
  );

VOID
CpuMemWrite16 (
  IN  EFI_PHYSICAL_ADDRESS  Address,
  IN  UINT16                Data
  );

VOID
CpuMemWrite32 (
  IN  EFI_PHYSICAL_ADDRESS  Address,
  IN  UINT32                Data
  );

VOID
CpuMemWrite64 (
  IN  EFI_PHYSICAL_ADDRESS    Address,
  IN  UINT64                  Data
  );

UINTN
CpuSetPower2 (
  IN  UINTN   Input
  );

UINT64
CpuReadTsc (
  VOID
  );

VOID
CpuSwitchStacks (
  IN  UINTN   EntryPoint,
  IN  UINTN   Parameter,
  IN  UINTN   NewStack,
  IN  UINTN   NewBsp
  );

VOID
CpuSwitchStacks2Args (
  IN  UINTN   EntryPoint,
  IN  UINTN   Parameter1,
  IN  UINTN   Parameter2,
  IN  UINTN   NewStack,
  IN  UINTN   NewBsp
  );

UINT16
CpuCodeSegment (
  VOID
  );

VOID
CpuBreak (
  VOID
  );

VOID
CpuLoadGlobalDescriptorTable (
  VOID  *Table16ByteAligned
  );

VOID
CpuInitSelectors (
  VOID
  );

VOID
CpuLoadInterruptDescriptorTable (
  VOID  *Table16ByteAligned
  );

#endif
