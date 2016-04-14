/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CPU_DATA_STRUCT_H
#define _CPU_DATA_STRUCT_H

#include <Guid/AcpiVariableCompatibility.h>
//
// The data saved in SMRAM.
// In S3 path, CPUS3 runs before SMMS3. SMRAM is open at that time.
//

#define EFI_SMRAM_CPU_NVS_HEADER_GUID \
  { \
    0x429501d9, 0xe447, 0x40f4, 0x86, 0x7b, 0x75, 0xc9, 0x3a, 0x1d, 0xb5, 0x4e \
  }

typedef struct {
  //
  // Guid as Signature.
  //
  EFI_GUID              HeaderGuid;
  EFI_PHYSICAL_ADDRESS  AcpiCpuPointer;
  ACPI_CPU_DATA_COMPATIBILITY         AcpiCpuData;

  //
  // It points the data defined below.
  //
  EFI_PHYSICAL_ADDRESS  GdtrProfileOffset;
  EFI_PHYSICAL_ADDRESS  GdtOffset;
  EFI_PHYSICAL_ADDRESS  IdtrProfileOffset;
  EFI_PHYSICAL_ADDRESS  IdtOffset;
  EFI_PHYSICAL_ADDRESS  CpuPrivateDataOffset;
  EFI_PHYSICAL_ADDRESS  S3BootScriptTableOffset;
  EFI_PHYSICAL_ADDRESS  S3BspMtrrTableOffset;
  EFI_PHYSICAL_ADDRESS  MicrocodePointerBufferOffset; // It is pointer to pointer array.
  EFI_PHYSICAL_ADDRESS  MicrocodeDataBufferOffset; // It is pointer to the data.

  //
  // We need put all the data buffer here as well.
  // These data will be copied to original location in S3.
  //

  //
  // DataBuffer size
  //
  UINT32                GdtrProfileSize;
  UINT32                GdtSize;
  UINT32                IdtrProfileSize;
  UINT32                IdtSize;
  UINT32                CpuPrivateDataSize;
  UINT32                S3BootScriptTableSize;
  UINT32                S3BspMtrrTableSize;
  UINT32                MicrocodePointerBufferSize;
  UINT32                MicrocodeDataBufferSize;
} SMRAM_CPU_DATA;

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

typedef struct {
  UINT32  HeaderVersion;
  UINT32  UpdateRevision;
  UINT32  Date;
  UINT32  ProcessorId;
  UINT32  Checksum;
  UINT32  LoaderRevision;
  UINT32  ProcessorFlags;
  UINT32  DataSize;
  UINT32  TotalSize;
  UINT8   Reserved[12];
} EFI_CPU_MICROCODE_HEADER;

typedef struct {
  UINT32  ExtSigCount;
  UINT32  ExtChecksum;
  UINT8   Reserved[12];
  UINT32  ProcessorId;
  UINT32  ProcessorFlags;
  UINT32  Checksum;
} EFI_CPU_MICROCODE_EXT_HEADER;

typedef struct {
  UINT32  ExtendedSignatureCount;
  UINT32  ExtendedTableChecksum;  
  UINT8   Reserved[12];
} EFI_CPU_MICROCODE_EXTENDED_TABLE_HEADER;

typedef struct {
  UINT32  ProcessorSignature;
  UINT32  ProcessorFlag;
  UINT32  ProcessorChecksum;
} EFI_CPU_MICROCODE_EXTENDED_TABLE;

typedef struct {
  UINT32  Stepping       : 4;
  UINT32  Model          : 4;
  UINT32  Family         : 4;
  UINT32  Type           : 2;
  UINT32  Reserved1      : 2;
  UINT32  ExtendedModel  : 4;
  UINT32  ExtendedFamily : 8;
  UINT32  Reserved2      : 4;
} EFI_CPU_VERSION;

#endif
