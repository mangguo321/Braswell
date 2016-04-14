/** @file
  MP Support driver.

  Copyright (c) 2007 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CpuDxe.h"
#include "MpCommon.h"
#include "PlatformMpService.h"

extern EFI_CPU_MICROCODE_HEADER **mMicrocodePointerBuffer;

ACPI_CPU_DATA_COMPATIBILITY     *mAcpiCpuData;
MP_SYSTEM_DATA                  *mMPSystemData;

/**
  Initializes MP support in the system.

  @param[in]  ImageHandle         Image handle of the loaded driver
  @param[in]  SystemTable         Pointer to the System Table

  @retval  EFI_SUCCESS            Multiple processors are initialized successfully.
  @retval  EFI_NOT_FOUND          The ACPI variable is not found in S3 boot path.
  @retval  EFI_OUT_OF_RESOURCES   No enough resoruces (such as out of memory).

**/
EFI_STATUS
InitializeMpSupport (
  IN EFI_HANDLE                       ImageHandle,
  IN EFI_SYSTEM_TABLE                 *SystemTable
  )
{
  EFI_STATUS            Status;
  MP_CPU_RESERVED_DATA  *MpCpuReservedData;
  
  MpCpuReservedData = NULL;

  Status = AllocateReservedMemoryBelow4G (
             sizeof (MP_CPU_RESERVED_DATA),
             (VOID **) &MpCpuReservedData
             );

  if ((EFI_ERROR (Status)) || (MpCpuReservedData == NULL)) {
    return Status;
  }

  ZeroMem (MpCpuReservedData, sizeof (MP_CPU_RESERVED_DATA));

  mMPSystemData   = &(MpCpuReservedData->MPSystemData);
  mAcpiCpuData    = &(MpCpuReservedData->AcpiCpuData);

  CopyMem (
    MpCpuReservedData->MicrocodePointerBuffer,
    mMicrocodePointerBuffer,
    sizeof (EFI_CPU_MICROCODE_HEADER *) * (NUMBER_OF_MICROCODE_UPDATE + 1)
    );

  mAcpiCpuData->CpuPrivateData          = (EFI_PHYSICAL_ADDRESS)(UINTN)(&(mMPSystemData->S3DataPointer));
  mAcpiCpuData->S3BootPath              = FALSE;
  mAcpiCpuData->MicrocodePointerBuffer  = (EFI_PHYSICAL_ADDRESS) MpCpuReservedData->MicrocodePointerBuffer;
  mAcpiCpuData->GdtrProfile             = (EFI_PHYSICAL_ADDRESS) & (MpCpuReservedData->GdtrProfile);
  mAcpiCpuData->IdtrProfile             = (EFI_PHYSICAL_ADDRESS) & (MpCpuReservedData->IdtrProfile);

  MpServiceInitialize ();

  return EFI_SUCCESS;
}
