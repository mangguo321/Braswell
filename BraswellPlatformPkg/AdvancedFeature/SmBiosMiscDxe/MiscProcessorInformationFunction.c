/** @file
  Onboard processor information boot time changes for SMBIOS type 4.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "MiscSubclassDriver.h"
#include <Protocol/MpService.h>
#include <Guid/DataHubRecords.h>
#include <Library/CpuIA32.h>

#define EfiProcessorFamilyIntelAtomProcessor    0x2B

EFI_GUID                        mProcessorProducerGuid;

/**
  Get cache SMBIOS record handle.

  @param[in]  Smbios        Pointer to SMBIOS protocol instance.
  @param[in]  CacheLevel    Level of cache, starting from one.
  @param[out] Handle        Returned record handle.

**/
VOID
GetCacheHandle (
  IN  EFI_SMBIOS_PROTOCOL      *Smbios,
  IN  UINT8                    CacheLevel,
  OUT EFI_SMBIOS_HANDLE        *Handle
  )
{
  UINT16                     CacheConfig;
  EFI_STATUS                 Status;
  EFI_SMBIOS_TYPE            RecordType;
  EFI_SMBIOS_TABLE_HEADER    *Buffer;

  *Handle = 0;
  RecordType = EFI_SMBIOS_TYPE_CACHE_INFORMATION;

  do {
    Status = Smbios->GetNext (
                       Smbios,
                       Handle,
                       &RecordType,
                       &Buffer,
                       NULL
                       );
    if (!EFI_ERROR (Status)) {
      CacheConfig = *(UINT16*)((UINT8*)Buffer + 5);
      if ((CacheConfig & 0x7) == (CacheLevel -1)) {
        return;
      }
    }
  } while (!EFI_ERROR(Status));

  *Handle = 0xFFFF;
}

/**
  This function makes boot time changes to the contents of the
  MiscProcessorInformation (Type 4).

  @param[in]  RecordData             Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
UINT32
ConvertBase10ToRaw (
  IN  EFI_EXP_BASE10_DATA             *Data)
{
  UINTN         Index;
  UINT32        RawData;

  RawData = Data->Value;
  for (Index = 0; Index < (UINTN) Data->Exponent; Index++) {
     RawData *= 10;
  }

  return  RawData;
}

#define BSEL_CR_OVERCLOCK_CONTROL	0xCD
#define	FUSE_BSEL_MASK				0x03

UINT16 miFSBFrequencyTable[4] = {
  83,          	// 83.3MHz
  100,          // 100MHz
  133,          // 133MHz
  117           // 116.7MHz
};

/**
  Determine the processor core frequency

  @param None

  @retval Processor core frequency multiplied by 3

**/
UINT16
DetermineiFsbFromMsr (
  VOID
  )
{
  //
  // Determine the processor core frequency
  //
  UINT64	Temp;
  Temp = (EfiReadMsr (BSEL_CR_OVERCLOCK_CONTROL)) & FUSE_BSEL_MASK;

  return miFSBFrequencyTable[(UINT32)(Temp)];
}

MISC_SMBIOS_TABLE_FUNCTION (MiscProcessorInformation)
{
  return EFI_SUCCESS;
}

