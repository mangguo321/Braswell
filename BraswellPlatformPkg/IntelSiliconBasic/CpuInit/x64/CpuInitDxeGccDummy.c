/** @file
  CPU Dxe Gcc Support - Dummy functions.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

UINTN AsmIdtVector00;

///
/// Function declarations
///
/**
  This is dummy function equivelant to the asm function CpuEnableInterrupt(), made for GCC build.
**/
VOID
CpuEnableInterrupt (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function CpuDisableInterrupt(), made for GCC build.
**/
VOID
CpuDisableInterrupt (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function AsmAcquireMPLock(), made for GCC build.
**/
VOID
AsmAcquireMPLock (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function AsmReleaseMPLock(), made for GCC build.
**/
VOID
AsmReleaseMPLock (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function AsmExchangeRole(), made for GCC build.
**/
VOID
AsmExchangeRole (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function CpuInitFloatPointUnit(), made for GCC build.
**/
VOID
CpuInitFloatPointUnit (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function MpMtrrSynchUpEntry(), made for GCC build.
**/
VOID
MpMtrrSynchUpEntry (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function MpMtrrSynchUpExit(), made for GCC build.
**/
VOID
MpMtrrSynchUpExit (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function CpuLoadGlobalDescriptorTable(), made for GCC build.
**/
VOID
CpuLoadGlobalDescriptorTable (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function CpuLoadInterruptDescriptorTable(), made for GCC build.
**/
VOID
CpuLoadInterruptDescriptorTable (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function InitializeExternalVectorTablePtr(), made for GCC build.
**/
VOID
InitializeExternalVectorTablePtr (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function CpuCodeSegment(), made for GCC build.
**/
VOID
CpuCodeSegment (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function AsmGetAddressMap(), made for GCC build.
**/
VOID
AsmGetAddressMap (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function AsmGetGdtrIdtr(), made for GCC build.
**/
VOID
AsmGetGdtrIdtr (
  VOID
  )
{
  return;
}

/**
  This is dummy function equivelant to the asm function AsmGetCr3(), made for GCC build.
**/
VOID
AsmGetCr3 (
  VOID
  )
{
  return;
}