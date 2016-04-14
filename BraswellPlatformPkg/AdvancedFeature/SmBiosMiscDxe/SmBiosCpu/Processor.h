/** @file
  Include file for record processor subclass data with Smbios protocol.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

//
// This is the string tool generated data representing our strings.
//
extern EFI_SMBIOS_PROTOCOL      *mSmbios;

extern UINT32                   mPopulatedSocketCount;

//
// This constant defines the maximum length of the CPU brand string. According to the
// IA manual, the brand string is in EAX through EDX (thus 16 bytes) after executing
// the CPUID instructions with EAX as 80000002, 80000003, 80000004.
//
#define MAXIMUM_CPU_BRAND_STRING_LENGTH 48

typedef struct {
  BOOLEAN           StringValid;
  CHAR16            BrandString[MAXIMUM_CPU_BRAND_STRING_LENGTH + 1];
  EFI_STRING_ID     StringRef;
} CPU_PROCESSOR_VERSION_INFORMATION;

//
// It is defined for SMBIOS_TABLE_TYPE4.Status.
//
typedef struct {
  UINT8 CpuStatus       :3; // Indicates the status of the processor.
  UINT8 Reserved1       :3; // Reserved for future use. Should be set to zero.
  UINT8 SocketPopulated :1; // Indicates if the processor socket is populated or not.
  UINT8 Reserved2       :1; // Reserved for future use. Should be set to zero.
} CPU_PROCESSOR_STATUS_DATA;

//
// It is defined for SMBIOS_TABLE_TYPE4.ProcessorCharacteristics.
//
typedef struct {
  UINT16 Reserved       :1;
  UINT16 Unknown        :1;
  UINT16 Capable64Bit   :1;
  UINT16 Reserved2      :13;
} CPU_PROCESSOR_CHARACTERISTICS_DATA;

#endif

