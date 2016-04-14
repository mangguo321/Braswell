/** @file
  Functions for loading Microcode. 

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MpCommon.h"

//
// Array of pointers which each points to 1 microcode update binary (in memory)
//
EFI_CPU_MICROCODE_HEADER  **mMicrocodePointerBuffer;

//
// Function declarations
//
EFI_STATUS
FindLoadMicrocode (
  IN     UINT32                     Cpuid,
  IN     EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  IN OUT UINT32                     *Revision,
  IN OUT BOOLEAN                    *VerifyMicrocodeChecksum
  );

#ifdef __GNUC__ // GCC compiler
UINT32
GetCpuUcodeRevision (
  VOID
  )
{
  AsmWriteMsr64 (EFI_MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
  return (UINT32) RShiftU64 (AsmReadMsr64 (EFI_MSR_IA32_BIOS_SIGN_ID), 32);
}
#else // MSFT compiler

/**

  @todo	add description

**/
UINT32
GetCpuUcodeRevision (
  VOID
  )
{
  UINT32 registers[4];
  __writemsr (EFI_MSR_IA32_BIOS_SIGN_ID, 0);
  __cpuid (registers, EFI_CPUID_VERSION_INFO);
  return (UINT32) (__readmsr (EFI_MSR_IA32_BIOS_SIGN_ID) >> 32);
}
#endif

/**

  This will locate a processor microcode and if it finds a newer revision, it will
  load it to the processor.

  @param[in]

  MicrocodePointerBuffer - The Array of pointers which each points to 1 microcode update binary (in memory)

  FailedRevision         - The microcode revision that fails to be loaded

  @retval

  EFI_SUCCESS           - A new microcode update is loaded
  Other                 - Due to some reason, no new microcode update is loaded
  
**/
EFI_STATUS
InitializeMicrocode (
  IN      EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  OUT     UINT32                     *FailedRevision,
  IN      BOOLEAN                    IsBsp,
  IN OUT  BOOLEAN                    *VerifyMicrocodeChecksum
  )

{
  EFI_STATUS          Status;
  UINT32              UcodeRevision;
  UINT32              RegEax;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  
  if (IsBsp) {
    //
    // Force Microcode to be loaded for BSP anyway
    //
    UcodeRevision = 0;
  } else {
    UcodeRevision   = GetCpuUcodeRevision ();
  }

  Status          = FindLoadMicrocode (RegEax, MicrocodePointerBuffer, &UcodeRevision, VerifyMicrocodeChecksum);
  *FailedRevision = UcodeRevision;
  return Status;
}

/**

  This will load the microcode to all the processors.

  @param[in]
  MicrocodeEntryPoint - The microcode update pointer
  Revision            - The current (before load this microcode update) microcode revision

  @retval

  EFI_SUCCESS           - Microcode loaded
  EFI_LOAD_ERROR        - Microcode not loaded

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

  @param[in]  *ChecksumAddr  - The start address to be checkumed 

  @param[in]  ChecksumType  - The type of data to be checksumed  

  @retval  EFI_SUCCESS           - Checksum correct
  @retval  EFI_CRC_ERROR         - Checksum incorrect  
  
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

  @param[in]  Cpuid                  - Data returned by cpuid instruction

  @param[in]  MicrocodePointerBuffer - The Array of pointers which each points to 1 microcode update binary (in memory)

  @param[in]  Revision               - As input parameter, the current microcode revision;
                           as output parameter, the microcode revision after microcode update is loaded

  @retval  EFI_SUCCESS           - A new microcode update is loaded
  @retval  Other                 - Due to some reason, no new microcode update is loaded
  
**/
EFI_STATUS
FindLoadMicrocode (
  IN      UINT32                     Cpuid,
  IN      EFI_CPU_MICROCODE_HEADER   **MicrocodePointerBuffer,
  IN OUT  UINT32                     *Revision,
  IN OUT  BOOLEAN                    *VerifyMicrocodeChecksum
  )

{
  EFI_STATUS                              Status;
  EFI_CPU_MICROCODE_HEADER                *MicrocodeEntryPoint;
  UINT8                                   Index;
  UINT8                                   MsrPlatform;
  UINT32                                  ExtendedTableLength;
  UINT32                                  ExtendedTableCount;
  BOOLEAN                                 CorrectMicrocode;
  EFI_CPU_MICROCODE_EXTENDED_TABLE        *ExtendedTable;
  EFI_CPU_MICROCODE_EXTENDED_TABLE_HEADER *ExtendedTableHeader;
  UINT8                                   IndexStep;

  Status = EFI_NOT_FOUND;
  ExtendedTableLength = 0;

  //
  // The index of platform information resides in bits 50:52 of MSR IA32_PLATFORM_ID
  //
  MsrPlatform = (UINT8) (RShiftU64 ((AsmReadMsr64 (EFI_MSR_IA32_PLATFORM_ID) & B_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS_MASK), \
                                       N_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS));

  IndexStep = (UINT32) (MicrocodePointerBuffer [1]) == 0 ? 2 : 1;

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
          (MicrocodeEntryPoint->UpdateRevision > *Revision)          &&
          (MicrocodeEntryPoint->ProcessorFlags & (1 << MsrPlatform)) ) {
        if (*VerifyMicrocodeChecksum == TRUE) {
          if (MicrocodeEntryPoint->DataSize == 0) {
            Status = Checksum32Verify ((UINT32 *)MicrocodeEntryPoint, 2048 / sizeof(UINT32));
          } else {
            Status = Checksum32Verify ((UINT32 *)MicrocodeEntryPoint, (MicrocodeEntryPoint->DataSize + sizeof(EFI_CPU_MICROCODE_HEADER)) / sizeof(UINT32));
          }
          if (!EFI_ERROR (Status)) {
            *VerifyMicrocodeChecksum = FALSE;
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
              for (Index = 0; Index < ExtendedTableCount; Index ++) {
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

    //
    // Increase table index, 32-bit : 1, 64-bit : 2
    //
    Index += IndexStep;
    
  }

  return Status;
}
