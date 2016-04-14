/** @file
  The function of Microcode.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CpuDxe.h"
#include "MpCommon.h"

//
// Array of pointers which each points to 1 microcode update binary (in memory)
//
EFI_CPU_MICROCODE_HEADER  **mMicrocodePointerBuffer;
BOOLEAN                   mVerifyMicrocodeChecksum = TRUE;

//
//	Function declaration
//
EFI_STATUS
FindLoadMicrocode (
  IN     UINT32                     Cpuid,
  IN     EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  IN OUT UINT32                     *Revision
  );

/**
  Returns the processor microcode revision of the processor installed in the system.

  @retval  Processor Microcode Revision

**/
UINT32
GetCpuUcodeRevision (
  VOID
  )
{
  AsmWriteMsr64 (EFI_MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
  return (UINT32) RShiftU64 (AsmReadMsr64 (EFI_MSR_IA32_BIOS_SIGN_ID), 32);
}

/**
  This will locate a processor microcode and if it finds a newer revision, it will
  load it to the processor.

  @param[in]   MicrocodePointerBuffer        The Array of pointers which each points to 1 microcode update binary (in memory)
  @param[out]  FailedRevision                The microcode revision that fails to be loaded
  @param[in]   IsBsp                         Add parameter description

  @retval  EFI_SUCCESS                       A new microcode update is loaded
  @retval  Other                             Due to some reason, no new microcode update is loaded
  
**/
EFI_STATUS
InitializeMicrocode (
  IN      EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  OUT     UINT32                     *FailedRevision,
  IN      BOOLEAN                    IsBsp
  )
{
  EFI_STATUS          Status;
  UINT32              Cpuid_RegEax;
  UINT32              UcodeRevision;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &Cpuid_RegEax, NULL, NULL, NULL);

  if (IsBsp) {
    //
    // Force Microcode to be loaded for BSP anyway
    //
    UcodeRevision = 0;
  } else {
    UcodeRevision   = GetCpuUcodeRevision ();
  }

  Status          = FindLoadMicrocode (Cpuid_RegEax, MicrocodePointerBuffer, &UcodeRevision);
  *FailedRevision = UcodeRevision;

  return Status;
}

/**
  This will load the microcode to all the processors.

  @param[in]  MicrocodeEntryPoint   The microcode update pointer
  @param[in]  Revision              The current (before load this microcode update) microcode revision

  @retval  EFI_SUCCESS              Microcode loaded
  @retval  EFI_LOAD_ERROR           Microcode not loaded

**/
EFI_STATUS
LoadMicrocode (
  IN  EFI_CPU_MICROCODE_HEADER  *MicrocodeEntryPoint,
  IN  UINT32                    *Revision
  )
{
  //
  // Load the Processor Microcode
  //
  AsmWriteMsr64 (
    EFI_MSR_IA32_BIOS_UPDT_TRIG,
    (UINT64) ((UINTN) MicrocodeEntryPoint + sizeof (EFI_CPU_MICROCODE_HEADER))
    );

  //
  // Verify that the microcode has been loaded
  //
  if (GetCpuUcodeRevision () == *Revision) {
    return EFI_LOAD_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Verify the DWORD type checksum

  @param[in]  ChecksumAddr         The start address to be checkumed 
  @param[in]  ChecksumType         The type of data to be checksumed  

  @retval  EFI_SUCCESS             Checksum correct
  @retval  EFI_CRC_ERROR           Checksum incorrect  
  
**/
EFI_STATUS
Checksum32Verify (
  IN UINT32 *ChecksumAddr,
  IN UINT32 ChecksumLen
  )
{
  UINT32   Checksum;
  UINT32   Index;
  
  Checksum = 0;
  
  for (Index = 0; Index < ChecksumLen; Index ++) {
    Checksum += ChecksumAddr[Index];
  }
  
  return (Checksum == 0) ? EFI_SUCCESS : EFI_CRC_ERROR;
}

/**
  This will locate a processor microcode and if it finds a newer revision, it will
  load it to the processor.

  @param[in]  Cpuid                       Data returned by cpuid instruction
  @param[in]  **MicrocodePointerBuffer    The Array of pointers which each points to 1 microcode update binary (in memory)
  @param[in, out] *Revision               As input parameter, the current microcode revision;
                                          as output parameter, the microcode revision after microcode update is loaded

  @retval  EFI_SUCCESS                    A new microcode update is loaded
  @retval  Other                          Due to some reason, no new microcode update is loaded
  
**/
EFI_STATUS
FindLoadMicrocode (
  IN      UINT32                     Cpuid,
  IN      EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  IN OUT  UINT32                     *Revision
  )
{
  EFI_STATUS                              Status;
  EFI_CPU_MICROCODE_HEADER                *MicrocodeEntryPoint;
  UINT8                                   Index;
  UINT8                                   Index2;
  UINT8                                   MsrPlatform;
  UINT32                                  ExtendedTableLength;
  UINT32                                  ExtendedTableCount;
  BOOLEAN                                 CorrectMicrocode;
  EFI_CPU_MICROCODE_EXTENDED_TABLE        *ExtendedTable;
  EFI_CPU_MICROCODE_EXTENDED_TABLE_HEADER *ExtendedTableHeader;

  Status = EFI_NOT_FOUND;
  ExtendedTableLength = 0;

  //
  // The index of platform information resides in bits 50:52 of MSR IA32_PLATFORM_ID
  //
  MsrPlatform = (UINT8) (RShiftU64 ((AsmReadMsr64 (EFI_MSR_IA32_PLATFORM_ID) & B_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS_MASK),  \
                                       N_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS));

  Index       = 0;
  while (MicrocodePointerBuffer[Index] != NULL) {
    MicrocodeEntryPoint = MicrocodePointerBuffer[Index];
    CorrectMicrocode    = FALSE;
    //
    // Check if the microcode is for the Cpu and the version is newer
    // and the update can be processed on the platform
    //
    if (MicrocodeEntryPoint->HeaderVersion == 0x00000001) {
      if ((MicrocodeEntryPoint->ProcessorId == Cpuid)                &&
          (MicrocodeEntryPoint->UpdateRevision >= *Revision)          &&
          (MicrocodeEntryPoint->ProcessorFlags & (1 << MsrPlatform)) ) {
        if (mVerifyMicrocodeChecksum == TRUE) {
          if (MicrocodeEntryPoint->DataSize == 0) {
            Status = Checksum32Verify ((UINT32 *)MicrocodeEntryPoint, 2048 / sizeof(UINT32));
          } else {
            Status = Checksum32Verify ((UINT32 *)MicrocodeEntryPoint, (MicrocodeEntryPoint->DataSize + sizeof(EFI_CPU_MICROCODE_HEADER)) / sizeof(UINT32));
          }
          if (!EFI_ERROR (Status)) {
            mVerifyMicrocodeChecksum = FALSE;
            CorrectMicrocode = TRUE;
          }
        } else {
          CorrectMicrocode = TRUE;
        }
      } else if ((MicrocodeEntryPoint->DataSize !=0) && (MicrocodeEntryPoint->UpdateRevision > *Revision)) {
        //
        // Check the  Extended Signature if the entended signature exist
        // Only the data size != 0 the extended signature may exist
        //
        ExtendedTableLength = MicrocodeEntryPoint->TotalSize - (MicrocodeEntryPoint->DataSize + sizeof (EFI_CPU_MICROCODE_HEADER));
        if (ExtendedTableLength != 0) {
          //
          // Extended Table exist, check if the CPU in support list
          //
          ExtendedTableHeader = (EFI_CPU_MICROCODE_EXTENDED_TABLE_HEADER *)((UINT8 *)(MicrocodeEntryPoint) + MicrocodeEntryPoint->DataSize + 48);
          //
          // Calulate Extended Checksum
          //          
          if ((ExtendedTableLength % 4) == 0) {
            Status = Checksum32Verify ((UINT32 *)ExtendedTableHeader, ExtendedTableLength / sizeof(UINT32));
            if (!EFI_ERROR (Status)) {
              //
              // Checksum correct
              //
              ExtendedTableCount = ExtendedTableHeader->ExtendedSignatureCount;
              ExtendedTable      = (EFI_CPU_MICROCODE_EXTENDED_TABLE *)(ExtendedTableHeader + 1);
              for (Index2 = 0; Index2 < ExtendedTableCount; Index2 ++) {
                Status = Checksum32Verify ((UINT32 *)ExtendedTable, sizeof(EFI_CPU_MICROCODE_EXTENDED_TABLE) / sizeof(UINT32)); 
                if (!EFI_ERROR (Status)) {
                  // 
                  // Verify Header
                  //
                  if ((ExtendedTable->ProcessorSignature == Cpuid)        &&
                      (ExtendedTable->ProcessorFlag & (1 << MsrPlatform)) ) {
                    //
                    // Find one
                    //
                    CorrectMicrocode = TRUE; 
                    break;
                  }
                }
                ExtendedTable ++;
              }
            }
          }
        }
      }
    }

    if (CorrectMicrocode) {
      Status    = LoadMicrocode (MicrocodeEntryPoint, Revision);
      *Revision = MicrocodeEntryPoint->UpdateRevision;
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }

    Index++;
  }

  return Status;
}

/**
  Get the microcode patch.

  @param[in]  This              Driver context.
  @param[out] MicrocodeData     Retrieved image of the microcode.

  @retval  EFI_SUCCESS          Image found.
  @retval  EFI_NOT_FOUND        Image not found.

**/
EFI_STATUS
EFIAPI
PlatformCpuRetrieveMicrocode (
  OUT UINT8                           **MicrocodeData
  )
{
  EFI_CPU_MICROCODE_HEADER *Microcode;
  UINTN                    MicrocodeStart;
  UINTN                    MicrocodeEnd;
  UINTN                    TotalSize;

  DEBUG ((EFI_D_INFO, "PcdFlashMicroCodeRegionBase - 0x%x\n", PcdGet32 (PcdFlashMicroCodeRegionBase)));
  DEBUG ((EFI_D_INFO, "PcdFlashMicroCodeOffset - 0x%x\n", PcdGet32 (PcdFlashMicroCodeOffset)));
  DEBUG ((EFI_D_INFO, "PcdFlashMicroCodeRegionSize    - 0x%x\n", PcdGet32 (PcdFlashMicroCodeRegionSize)));

  MicrocodeStart =(UINTN)PcdGet32 (PcdFlashMicroCodeRegionBase) + PcdGet32 (PcdFlashMicroCodeOffset);
  MicrocodeEnd = (UINTN)PcdGet32 (PcdFlashMicroCodeRegionBase) + (UINTN)PcdGet32 (PcdFlashMicroCodeRegionSize);

  if (*MicrocodeData == NULL) {
    *MicrocodeData = (UINT8 *) (UINTN) MicrocodeStart;
  } else {
    if (*MicrocodeData < (UINT8 *) (UINTN) MicrocodeStart) {
      return EFI_NOT_FOUND;
    }

    TotalSize = (UINTN) (((EFI_CPU_MICROCODE_HEADER *) *MicrocodeData)->TotalSize);
    if (TotalSize == 0) {
      TotalSize = 2048;
    }
    //
    // Add alignment check - begin
    //
    if ((TotalSize & 0x7FF) != 0) {
      TotalSize = (TotalSize & 0xFFFFF800) + 0x800;
    }
    //
    // Add alignment check - end
    //
    *MicrocodeData += TotalSize;
    Microcode = (EFI_CPU_MICROCODE_HEADER *) *MicrocodeData;
    if (*MicrocodeData >= (UINT8 *) (UINTN) (MicrocodeEnd) || Microcode->TotalSize == (UINT32) -1) {
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}

/**
  Load all microcode updates to memory. Since in S3 resume boot path, CPUs should be
  patched again, these microcode updates are copied to OS reserved memory.

  @retval  EFI_SUCCESS             All microcode updates are loaded to memory successfully
  @retval  EFI_OUT_OF_RESOURCES    Not enough memory to accomodate all the microcode updates

**/
EFI_STATUS
LoadAllMicrocodeUpdates (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_CPU_MICROCODE_HEADER  *MicrocodeEntryPoint;
  EFI_CPU_MICROCODE_HEADER  *MicrocodeBuffer;
  UINTN                     MicrocodeNumber;
  UINTN                     Index;
  UINTN                     TotalSize[NUMBER_OF_MICROCODE_UPDATE + 1];

  MicrocodeNumber = 0;
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  sizeof (EFI_CPU_MICROCODE_HEADER *) * (NUMBER_OF_MICROCODE_UPDATE + 1),
                  (VOID *) (&mMicrocodePointerBuffer)
                  );
  ASSERT_EFI_ERROR (Status);

  ZeroMem (mMicrocodePointerBuffer, sizeof (EFI_CPU_MICROCODE_HEADER *) * (NUMBER_OF_MICROCODE_UPDATE + 1));
  ZeroMem (TotalSize, sizeof(TotalSize));

  MicrocodeEntryPoint = NULL;
  MicrocodeBuffer     = NULL;
  while (TRUE) {
    //
    // Continue to try to find patch
    //
    Status = PlatformCpuRetrieveMicrocode ((VOID *) &MicrocodeEntryPoint);

    if (EFI_ERROR (Status)) {
      if (Status == EFI_NOT_FOUND) {
        Status = EFI_SUCCESS;
      }
      break;

    } else {
      if (MicrocodeNumber >= NUMBER_OF_MICROCODE_UPDATE) {
        DEBUG ((EFI_D_ERROR, "CPU Too Many Microcode available > %d\n", (UINTN)NUMBER_OF_MICROCODE_UPDATE));
        Status = EFI_SUCCESS;
        break;
      }

      if (MicrocodeEntryPoint->DataSize == 0) {
        TotalSize[MicrocodeNumber] = 2048;
      } else {
        TotalSize[MicrocodeNumber] = MicrocodeEntryPoint->TotalSize;
      }

      Status = AllocateReservedMemoryBelow4G (
                 TotalSize[MicrocodeNumber],
                 (VOID **) &MicrocodeBuffer
                 );
      if (EFI_ERROR (Status)) {
        break;
      }

      CopyMem (MicrocodeBuffer, MicrocodeEntryPoint, TotalSize[MicrocodeNumber]);
      mMicrocodePointerBuffer[MicrocodeNumber] = MicrocodeBuffer;
      MicrocodeNumber++;
    }
  }

  if (EFI_ERROR (Status)) {
    Index = 0;
    while (mMicrocodePointerBuffer[Index] != NULL) {
      gBS->FreePages (
             (EFI_PHYSICAL_ADDRESS)(UINTN)mMicrocodePointerBuffer[Index],
             EFI_SIZE_TO_PAGES(TotalSize[Index])
             );
      mMicrocodePointerBuffer[Index] = NULL;
      Index++;
    }
  }

  return Status;
}

/**
  Check if loading microcode update fails, if so, report proper status code

  @param[in]  CpuNumber         The CPU number
  @param[in]  Status            The return value of InitializeMicrocode()
  @param[in]  FailedRevision    The revision of the microcode update that failed to be loaded

  @retval  EFI_SUCCESS          The status is check and proper status code is reported

**/
EFI_STATUS
CheckMicrocodeUpdate (
  IN  UINTN              CpuNumber,
  IN  EFI_STATUS         Status,
  IN  UINT32             FailedRevision
  )
{
  EFI_STATUS_CODE_VALUE                           StatusCode;
  EFI_STATUS_CODE_DATA                            *ErrorDataPointer;
  EFI_COMPUTING_UNIT_MICROCODE_UPDATE_ERROR_DATA  ErrorData;
  UINTN                                           DataSize;
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  if (Status == EFI_LOAD_ERROR) {
    ErrorData.DataHeader.HeaderSize = sizeof (EFI_STATUS_CODE_DATA);
    ErrorData.DataHeader.Size       = sizeof (EFI_COMPUTING_UNIT_MICROCODE_UPDATE_ERROR_DATA) - sizeof (EFI_STATUS_CODE_DATA);

    CopyMem (&ErrorData.DataHeader.Type, &gEfiStatusCodeSpecificDataGuid, sizeof (EFI_GUID));
    ErrorData.Version = FailedRevision;
    StatusCode        = EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_MICROCODE_UPDATE;
    ErrorDataPointer  = (EFI_STATUS_CODE_DATA *) &ErrorData;
  } else if (Status == EFI_NOT_FOUND) {
    if (GetCpuUcodeRevision () != 0) {
      //
      // Some other driver (for example, SEC) already updated CPU microcode
      //
      return EFI_SUCCESS;
    }

    StatusCode        = EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_NO_MICROCODE_UPDATE;
    ErrorDataPointer  = NULL;

  } else {
    return Status;
  }
  if (ErrorDataPointer == NULL) {
    DataSize = 0;
  } else {
    DataSize = sizeof(EFI_COMPUTING_UNIT_MICROCODE_UPDATE_ERROR_DATA);
  }
  return REPORT_STATUS_CODE_EX (
           EFI_ERROR_MINOR | EFI_ERROR_CODE,
           StatusCode,
           (UINT32) CpuNumber,
           &gEfiCallerIdGuid,
           NULL, 
           ErrorDataPointer,
           DataSize
           );
}
