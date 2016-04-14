/** @file
  Smbus Policy PPI as defined in EFI 2.0

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PEI_SMBUS_POLICY_PPI_H
#define _PEI_SMBUS_POLICY_PPI_H

typedef struct _PEI_SMBUS_POLICY_PPI PEI_SMBUS_POLICY_PPI;

typedef struct _PEI_SMBUS_POLICY_PPI {
  UINTN   BaseAddress;
  UINT32  PciAddress;
  UINT8   NumRsvdAddress;
  UINT8   *RsvdAddress;
} PEI_SMBUS_POLICY_PPI;

extern EFI_GUID gPeiSmbusPolicyPpiGuid;

#endif
