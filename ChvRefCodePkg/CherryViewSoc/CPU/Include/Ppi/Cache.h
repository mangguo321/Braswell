/** @file
  Cache PPI definition.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PEI_CACHE_PPI_H_
#define _PEI_CACHE_PPI_H_

typedef struct _PEI_CACHE_PPI PEI_CACHE_PPI;

//
// EFI_MEMORY_CACHE_TYPE
//
typedef INT32 EFI_MEMORY_CACHE_TYPE;

//
// PEI_SET_CACHE_PPI
//
typedef
EFI_STATUS
(EFIAPI *PEI_SET_CACHE_PPI) (
  IN  EFI_PEI_SERVICES       **PeiServices,
  IN  PEI_CACHE_PPI          *This,
  IN  EFI_PHYSICAL_ADDRESS   MemoryAddress,
  IN  UINT64                 MemoryLength,
  IN  EFI_MEMORY_CACHE_TYPE  MemoryCacheType
  );

//
// PEI_RESET_CACHE_PPI
//
typedef
EFI_STATUS
(EFIAPI *PEI_RESET_CACHE_PPI) (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  PEI_CACHE_PPI      *This
  );

//
// PEI_CACHE_PPI
//
struct _PEI_CACHE_PPI {
  PEI_SET_CACHE_PPI    SetCache;
  PEI_RESET_CACHE_PPI  ResetCache;
};

extern EFI_GUID gPeiCachePpiGuid;

#endif
