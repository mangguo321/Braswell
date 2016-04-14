/** @file
  Structure and location in reserved memory used to store the location of ACPI
  reserved memory used in S3 resume.

  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _RESERVED_ACPI_S3_RANGE_H_
#define _RESERVED_ACPI_S3_RANGE_H_

//
// This structure stores the base and size of the ACPI reserved memory used when
// resuming from S3.  This region must be allocated by the platform code. 
// It is at the top of the first page of the TSEG. 
//
typedef struct {
  UINT32  AcpiReservedMemoryBase;
  UINT32  AcpiReservedMemorySize;
  UINT32  SystemMemoryLength;
} RESERVED_ACPI_S3_RANGE;

#define RESERVED_ACPI_S3_RANGE_OFFSET (EFI_PAGE_SIZE - sizeof (RESERVED_ACPI_S3_RANGE))

#endif
