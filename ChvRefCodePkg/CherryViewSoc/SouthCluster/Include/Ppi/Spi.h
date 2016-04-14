/** @file
  This file defines the EFI SPI PPI which implements the
  Intel(R) PCH SPI Host Controller Compatibility Interface.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PEI_SPI_H_
#define _PEI_SPI_H_

#include <Protocol/Spi.h>

//
// Extern the GUID for PPI users.
//
extern EFI_GUID           gPeiSpiPpiGuid;

//
// Reuse the EFI_SPI_PROTOCOL definitions
// This is possible becaues the PPI implementation does not rely on a PeiService pointer,
// as it uses EDKII Glue Lib to do IO accesses
//
typedef EFI_SPI_PROTOCOL  PEI_SPI_PPI;

#endif
