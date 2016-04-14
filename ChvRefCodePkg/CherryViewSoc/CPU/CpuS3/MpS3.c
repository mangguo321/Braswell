/** @file
  Cpu driver running on S3 boot path.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MpCommon.h"

EFI_GUID  mSmramCpuNvsHeaderGuid = EFI_SMRAM_CPU_NVS_HEADER_GUID;

/**

  @todo	add description

**/
UINT32
GetApicID (
  VOID
)
{
  UINT32  RegEbx;

  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
  return (UINT32)(RegEbx >> 24);
}

/**

  @todo Add function description

  @param[in]  BroadcastMode -   @todo add argument description
  @param[in]  ApicID        -   @todo add argument description
  @param[in]  VectorNumber  -   @todo add argument description
  @param[in]  DeliveryMode  -   @todo add argument description
  @param[in]  TriggerMode   -   @todo add argument description
  @param[in]  Assert        -   @todo add argument description

  @retval  EFI_INVALID_PARAMETER -   @todo Add description for return value
  @retval  EFI_NOT_READY -   @todo Add description for return value
  @retval  EFI_SUCCESS -   @todo Add description for return value

**/

EFI_STATUS
SendInterrupt (
  IN  UINT32                               BroadcastMode,
  IN  UINT32                               ApicID,
  IN  UINT32                               VectorNumber,
  IN  UINT32                               DeliveryMode,
  IN  UINT32                               TriggerMode,
  IN  BOOLEAN                              Assert,
  IN  CONST EFI_PEI_SERVICES               **PeiServices,
  IN  EFI_PEI_STALL_PPI                    *PeiStall
  )
{
  UINT64                ApicBaseReg;
  EFI_PHYSICAL_ADDRESS  ApicBase;
  UINT32                ICRLow;
  UINT32                ICRHigh;

  //
  // Initialze ICR high dword, since P6 family processor needs
  // the destination field to be 0x0F when it is a broadcast
  //
  ICRHigh = 0x0f000000;
  ICRLow  = VectorNumber | (DeliveryMode << 8);

  if (TriggerMode == TRIGGER_MODE_LEVEL) {
    ICRLow |= 0x8000;
  }

  if (Assert) {
    ICRLow |= 0x4000;
  }

  switch (BroadcastMode) {
  case BROADCAST_MODE_SPECIFY_CPU:
    ICRHigh = ApicID << 24;
    break;

  case BROADCAST_MODE_ALL_INCLUDING_SELF:
    ICRLow |= 0x80000;
    break;

  case BROADCAST_MODE_ALL_EXCLUDING_SELF:
    ICRLow |= 0xC0000;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  ApicBaseReg = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);
  ApicBase    = ApicBaseReg & B_EFI_MSR_IA32_APIC_BASE_APIC_BASE_ADDRESS;

  *(volatile UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_ICR_HIGH_OFFSET)  = ICRHigh;
  *(volatile UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_ICR_LOW_OFFSET)   = ICRLow;

  PeiStall->Stall(
                PeiServices,
                PeiStall,
                10
                );

  ICRLow = *(volatile UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_ICR_LOW_OFFSET);
  if (ICRLow & 0x1000) {
    return EFI_NOT_READY;
  }

  return EFI_SUCCESS;
}

/**

  @todo Add function description

  @param[in]  BSP -   @todo add argument description
  @param[in]  VirtualWireMode -   @todo add argument description

  @retval    @todo add return values

**/
VOID
ProgramVirtualWireMode (
  BOOLEAN                       BSP,
  UINT32                        VirtualWireMode
  )
{
  UINT64                ApicBaseReg;
  EFI_PHYSICAL_ADDRESS  ApicBase;
  volatile UINT32       *EntryAddress;
  UINT32                EntryValue;

  ApicBaseReg = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);
  ApicBase    = ApicBaseReg & B_EFI_MSR_IA32_APIC_BASE_APIC_BASE_ADDRESS;

  //
  // Program the Spurious Vectore entry
  //
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_SPURIOUS_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;
  EntryValue &= 0xFFFFFD0F;
  EntryValue |= 0x10F;
  *EntryAddress = EntryValue;

  //
  // Program the LINT1 vector entry as extINT
  //
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_LINT0_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;
  if (VirtualWireMode != 0) {
    EntryValue |= 0x10000;
  } else {
    if (BSP) {
      EntryValue &= 0xFFFE00FF;
      EntryValue |= 0x700;
    } else {
      EntryValue |= 0x10000;
    }
  }
  *EntryAddress = EntryValue;

  //
  // Program the LINT1 vector entry as NMI
  //
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + APIC_REGISTER_LINT1_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;
  EntryValue &= 0xFFFE00FF;
  if (BSP) {
    EntryValue |= 0x400;
  } else {
    EntryValue |= 0x10400;
  }
  *EntryAddress = EntryValue;

  if (VirtualWireMode && BSP) {
    //
    // Initialize the I0XApic RT table for virtual wire B
    //
    *(volatile UINT8   *)(UINTN)IO_APIC_INDEX_REGISTER = 0x10;
    *(volatile UINT32  *)(UINTN)IO_APIC_DATA_REGISTER  = 0x0700;
    *(volatile UINT8   *)(UINTN)IO_APIC_INDEX_REGISTER = 0x11;
    *(volatile UINT32  *)(UINTN)IO_APIC_DATA_REGISTER  = (GetApicID () << 24);
  }

}

/**

  @todo	Add description

**/
VOID
InitializeFeatures (
  IN MP_CPU_S3_SCRIPT_DATA      *ScriptTable
  )
{
  UINT32                  RegEbx;
  UINT32                  ApicId;
  UINT8                   SkipMsr;
  //
  // Restore all the MSRs for processor
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
  ApicId = (RegEbx >> 24);

  while (ScriptTable->MsrIndex != 0) {
    if (ApicId == ScriptTable->ApicId) {
      SkipMsr = 0;
      if ( (ScriptTable->MsrIndex == EFI_MSR_PMG_CST_CONFIG) &&
           (AsmReadMsr64(EFI_MSR_PMG_CST_CONFIG) & B_EFI_MSR_PMG_CST_CONFIG_CST_CONTROL_LOCK) ) {
        SkipMsr = 1;
      }
      if (SkipMsr == 0) {
        AsmWriteMsr64 (ScriptTable->MsrIndex, ScriptTable->MsrValue);
      }
    }
    ScriptTable++;
  }
}

/**

  @todo	Add description

**/
VOID
MPRendezvousProcedure (
  MP_CPU_EXCHANGE_INFO      *ExchangeInfo,
  UINT64                    *MtrrValues
  )
{
  UINT32                    FailedRevision;
  BOOLEAN                   VerifyMicrocodeChecksum;

  ProgramVirtualWireMode (FALSE, ExchangeInfo->VirtualWireMode);
  VerifyMicrocodeChecksum = ExchangeInfo->VerifyMicrocodeChecksum;
  InitializeMicrocode ((EFI_CPU_MICROCODE_HEADER **)(UINTN)ExchangeInfo->MicrocodePointer, &FailedRevision, FALSE, &VerifyMicrocodeChecksum);
  // TODO: InitializeFeatures (ExchangeInfo->S3BootScriptTable);
  MpMtrrSynchUp (MtrrValues);

  AsmAcquireMPLock ((UINT8 *)&(ExchangeInfo->SerializeLock));
  ExchangeInfo->FinishedCount++;
  AsmReleaseMPLock ((UINT8 *)&(ExchangeInfo->SerializeLock));
}

/**

  Wake up all the application processors

  @param[in]  ImageHandle - Image handle of the loaded driver

  @retval		EFI_SUCCESS - APs are successfully waked up

**/
EFI_STATUS
WakeUpAPs (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_STALL_PPI         *PeiStall,
  ACPI_CPU_DATA_COMPATIBILITY  *mAcpiCpuData,
  UINT64                       *MtrrValues,
  IN BOOLEAN                   VerifyMicrocodeChecksum
  )
{
  EFI_PHYSICAL_ADDRESS                        WakeUpBuffer;
  MP_CPU_EXCHANGE_INFO                        *ExchangeInfo;
  MP_CPU_S3_DATA_POINTER                      *CpuS3DataPtr;

  WakeUpBuffer = mAcpiCpuData->WakeUpBuffer;
  CopyMem ((VOID *) (UINTN) WakeUpBuffer, AsmGetAddressMap(), 0x1000 - 0x200);

  ExchangeInfo = (MP_CPU_EXCHANGE_INFO *) (UINTN) (WakeUpBuffer + (0x1000 - 0x200));

  ExchangeInfo->Lock               = 0;
  ExchangeInfo->StackStart         = (UINTN) mAcpiCpuData->StackAddress;
  ExchangeInfo->StackSize          = STACK_SIZE_PER_PROC;
  ExchangeInfo->ApFunction         = (UINT32) MPRendezvousProcedure;
  ExchangeInfo->BufferStart        = (UINT32) WakeUpBuffer;
  ExchangeInfo->PmodeOffset        = (UINT32) (AsmGetPmodeOffset ());
  ExchangeInfo->AcpiCpuDataAddress = (UINT32) mAcpiCpuData;
  ExchangeInfo->MtrrValuesAddress  = (UINT32) MtrrValues;
  ExchangeInfo->FinishedCount      = (UINT32) 0;
  ExchangeInfo->SerializeLock      = (UINT32) 0;
  ExchangeInfo->MicrocodePointer   = (UINT32) (UINTN)mAcpiCpuData->MicrocodePointerBuffer;
  ExchangeInfo->StartState         = (UINT32) 0;

  CpuS3DataPtr = (MP_CPU_S3_DATA_POINTER *)(UINTN)mAcpiCpuData->CpuPrivateData;
  ExchangeInfo->S3BootScriptTable  = (MP_CPU_S3_SCRIPT_DATA *)(UINTN)CpuS3DataPtr->S3BootScriptTable;
  ExchangeInfo->VirtualWireMode    = CpuS3DataPtr->VirtualWireMode;
  ExchangeInfo->VerifyMicrocodeChecksum = VerifyMicrocodeChecksum;

  CopyMem ((VOID *) (UINTN) &ExchangeInfo->GdtrProfile, (VOID *) (UINTN) mAcpiCpuData->GdtrProfile, sizeof (IA32_DESCRIPTOR));
  CopyMem ((VOID *) (UINTN) &ExchangeInfo->IdtrProfile, (VOID *) (UINTN) mAcpiCpuData->IdtrProfile, sizeof (IA32_DESCRIPTOR));

  DEBUG ((DEBUG_ERROR, "Cpu S3 Bootscript at %08X\n", (UINT32)ExchangeInfo->S3BootScriptTable));

  //
  // Don't touch MPCPU private data
  // Here we use ExchangeInfo instead
  //

  //
  // Send INIT IPI - SIPI to all APs
  //
  SendInterrupt (
    BROADCAST_MODE_ALL_EXCLUDING_SELF,
    0,
    0,
    DELIVERY_MODE_INIT,
    TRIGGER_MODE_EDGE,
    TRUE,
    PeiServices,
    PeiStall
    );

  SendInterrupt (
    BROADCAST_MODE_ALL_EXCLUDING_SELF,
    0,
    (UINT32) RShiftU64 (WakeUpBuffer, 12),
    DELIVERY_MODE_SIPI,
    TRIGGER_MODE_EDGE,
    TRUE,
    PeiServices,
    PeiStall
  );
  while (ExchangeInfo->FinishedCount < mAcpiCpuData->NumberOfCpus - 1) {
    CpuPause ();
  }

  return EFI_SUCCESS;
}

/**

  This routine is used to search SMRAM and get SmramCpuData point.

  @param[in]  PeiServices  - PEI services global pointer
  @param[in]  SmmAccessPpi - SmmAccess PPI instance

@returns  SmramCpuData - The pointer of CPU information in SMRAM.

**/
STATIC
SMRAM_CPU_DATA *
GetSmmCpuData (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN PEI_SMM_ACCESS_PPI         *SmmAccessPpi
  )
{
  EFI_SMRAM_DESCRIPTOR              *SmramRanges;
  UINTN                             SmramRangeCount;
  UINTN                             Size;
  EFI_STATUS                        Status;
  UINT32                            Address;
  SMRAM_CPU_DATA                    *SmramCpuData;

  //
  // Get all SMRAM range
  //
  Size = 0;
  Status = SmmAccessPpi->GetCapabilities (PeiServices, SmmAccessPpi, &Size, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  Status = PeiServicesAllocatePool (
                             Size,
                             (VOID **)&SmramRanges
                             );
  ASSERT_EFI_ERROR (Status);

  Status = SmmAccessPpi->GetCapabilities (PeiServices, SmmAccessPpi, &Size, SmramRanges);
  ASSERT_EFI_ERROR (Status);

  Size /= sizeof (*SmramRanges);
  SmramRangeCount = Size;

  //
  //  BUGBUG: We assume TSEG is the last range of SMRAM in SmramRanges
  //
  SmramRanges += SmramRangeCount - 1;

  DEBUG (( DEBUG_ERROR, "TsegBase - %x\n", SmramRanges->CpuStart));
  DEBUG (( DEBUG_ERROR, "TsegTop - %x\n", SmramRanges->CpuStart + SmramRanges->PhysicalSize));

  //
  // Search SMRAM on page alignment for the SMMNVS signature
  //
  for (Address = (UINT32)(SmramRanges->CpuStart + SmramRanges->PhysicalSize - EFI_PAGE_SIZE);
       Address >= (UINT32)SmramRanges->CpuStart;
       Address -= EFI_PAGE_SIZE) {
    SmramCpuData = (SMRAM_CPU_DATA *)(UINTN)Address;
    if (CompareGuid (&SmramCpuData->HeaderGuid, &mSmramCpuNvsHeaderGuid)) {
      //
      // Find it
      //
      return SmramCpuData;
    }
  }

  ASSERT (FALSE);

  return NULL;
}

/**

  This routine is restore the CPU information from SMRAM to original reserved memory region.

  @param[in]  PeiServices  - PEI services global pointer

  @returns  AcpiCpuData - The pointer of CPU information in reserved memory.

**/
ACPI_CPU_DATA_COMPATIBILITY *
RestoreSmramCpuData (
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  PEI_SMM_ACCESS_PPI                *SmmAccessPpi;
  SMRAM_CPU_DATA                    *SmramCpuData;
  EFI_STATUS                        Status;
  ACPI_CPU_DATA_COMPATIBILITY       *AcpiCpuData;
  MP_CPU_S3_DATA_POINTER            *CpuS3DataPtr;
  IA32_DESCRIPTOR                   *Idtr;
  IA32_DESCRIPTOR                   *Gdtr;
  UINTN                             MicrocodeSize;
  EFI_CPU_MICROCODE_HEADER          **Microcode;
  EFI_CPU_MICROCODE_HEADER          *LockBoxMicrocode;
  UINTN                             Index;

  Status = PeiServicesLocatePpi (
                            &gPeiSmmAccessPpiGuid,
                            0,
                            NULL,
                            (VOID **)&SmmAccessPpi
                            );
  ASSERT_EFI_ERROR (Status);

  //
  // Open all SMM regions
  //
  Index = 0;
  do {
    Status = SmmAccessPpi->Open ((EFI_PEI_SERVICES**)PeiServices, SmmAccessPpi, Index);
    Index++;
  } while (!EFI_ERROR (Status));

  SmramCpuData = GetSmmCpuData ((EFI_PEI_SERVICES**)PeiServices, SmmAccessPpi);
  if (SmramCpuData == NULL) {
    ASSERT(SmramCpuData != NULL);
    return NULL;
  }

  DEBUG ((DEBUG_ERROR, "CpuS3 SmramCpuData - 0x%x \n", SmramCpuData));

  DEBUG ((DEBUG_ERROR, "SmramCpuData->GdtrProfileSize            - %x\n", SmramCpuData->GdtrProfileSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->GdtSize                    - %x\n", SmramCpuData->GdtSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->IdtrProfileSize            - %x\n", SmramCpuData->IdtrProfileSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->IdtSize                    - %x\n", SmramCpuData->IdtSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->CpuPrivateDataSize         - %x\n", SmramCpuData->CpuPrivateDataSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->S3BootScriptTableSize      - %x\n", SmramCpuData->S3BootScriptTableSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->S3BspMtrrTableSize         - %x\n", SmramCpuData->S3BspMtrrTableSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->MicrocodePointerBufferSize - %x\n", SmramCpuData->MicrocodePointerBufferSize));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->MicrocodeDataBufferSize    - %x\n", SmramCpuData->MicrocodeDataBufferSize));

  DEBUG ((DEBUG_ERROR, "SmramCpuData->GdtrProfileOffset            - %x\n", SmramCpuData->GdtrProfileOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->GdtOffset                    - %x\n", SmramCpuData->GdtOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->IdtrProfileOffset            - %x\n", SmramCpuData->IdtrProfileOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->IdtOffset                    - %x\n", SmramCpuData->IdtOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->CpuPrivateDataOffset         - %x\n", SmramCpuData->CpuPrivateDataOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->S3BootScriptTableOffset      - %x\n", SmramCpuData->S3BootScriptTableOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->S3BspMtrrTableOffset         - %x\n", SmramCpuData->S3BspMtrrTableOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->MicrocodePointerBufferOffset - %x\n", SmramCpuData->MicrocodePointerBufferOffset));
  DEBUG ((DEBUG_ERROR, "SmramCpuData->MicrocodeDataBufferOffset    - %x\n", SmramCpuData->MicrocodeDataBufferOffset));

  //
  // Start restore data to NVS
  //
  AcpiCpuData = (ACPI_CPU_DATA_COMPATIBILITY *)(UINTN)SmramCpuData->AcpiCpuPointer;
  CopyMem (AcpiCpuData, &SmramCpuData->AcpiCpuData, sizeof (ACPI_CPU_DATA_COMPATIBILITY));

  DEBUG ((DEBUG_ERROR, "AcpiCpuData->WakeUpBuffer    - %x\n", AcpiCpuData->WakeUpBuffer));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->GdtrProfile    - %x\n", AcpiCpuData->GdtrProfile));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->IdtrProfile    - %x\n", AcpiCpuData->IdtrProfile));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->CpuPrivateData    - %x\n", AcpiCpuData->CpuPrivateData));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->StackAddress    - %x\n", AcpiCpuData->StackAddress));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->MicrocodePointerBuffer    - %x\n", AcpiCpuData->MicrocodePointerBuffer));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->SmramBase    - %x\n", AcpiCpuData->SmramBase));
  DEBUG ((DEBUG_ERROR, "AcpiCpuData->SmmStartImageBase    - %x\n", AcpiCpuData->SmmStartImageBase));
  
  CopyMem (
    (VOID *)(UINTN)AcpiCpuData->GdtrProfile,
    (UINT8 *)SmramCpuData + SmramCpuData->GdtrProfileOffset,
    SmramCpuData->GdtrProfileSize
    );
  Gdtr = (IA32_DESCRIPTOR *)(UINTN)AcpiCpuData->GdtrProfile;
  CopyMem (
    (VOID *)(UINTN)Gdtr->Base,
    (UINT8 *)SmramCpuData + SmramCpuData->GdtOffset,
    SmramCpuData->GdtSize
    );
  DEBUG ((DEBUG_ERROR, "Gdtr->Base    - %x\n", (UINT32) (Gdtr->Base)));
  CopyMem (
    (VOID *)(UINTN)AcpiCpuData->IdtrProfile,
    (UINT8 *)SmramCpuData + SmramCpuData->IdtrProfileOffset,
    SmramCpuData->IdtrProfileSize
    );
  Idtr = (IA32_DESCRIPTOR *)(UINTN)AcpiCpuData->IdtrProfile;
  CopyMem (
    (VOID *)(UINTN)Idtr->Base,
    (UINT8 *)SmramCpuData + SmramCpuData->IdtOffset,
    SmramCpuData->IdtSize
    );
  DEBUG ((DEBUG_ERROR, "Idtr->Base    - %x\n", (UINT32) (Idtr->Base)));
  CopyMem (
    (VOID *)(UINTN)AcpiCpuData->CpuPrivateData,
    (UINT8 *)SmramCpuData + SmramCpuData->CpuPrivateDataOffset,
    SmramCpuData->CpuPrivateDataSize
    );
  CpuS3DataPtr = (MP_CPU_S3_DATA_POINTER *)(UINTN)AcpiCpuData->CpuPrivateData;
  
  //
  // TODO: 
  // CopyMem (
  //  (VOID *)(UINTN)CpuS3DataPtr->S3BootScriptTable,
  //  (UINT8 *)SmramCpuData + SmramCpuData->S3BootScriptTableOffset,
  //  SmramCpuData->S3BootScriptTableSize
  //  );
  //
  DEBUG ((DEBUG_ERROR, "CpuS3DataPtr->S3BootScriptTable    - %x\n", (UINT32) (CpuS3DataPtr->S3BootScriptTable)));
  
  // 
  // TODO: BSP MTRRs are initialized by FSP.
  //  CopyMem (
  //  (VOID *)(UINTN)CpuS3DataPtr->S3BspMtrrTable,
  //  (UINT8 *)SmramCpuData + SmramCpuData->S3BspMtrrTableOffset,
  //  SmramCpuData->S3BspMtrrTableSize
  //  );
  DEBUG ((DEBUG_ERROR, "CpuS3DataPtr->S3BspMtrrTable    - %x\n", (UINT32) (CpuS3DataPtr->S3BspMtrrTable)));
  CopyMem (
    (VOID *)(UINTN)AcpiCpuData->MicrocodePointerBuffer,
    (UINT8 *)SmramCpuData + SmramCpuData->MicrocodePointerBufferOffset,
    SmramCpuData->MicrocodePointerBufferSize
    );
    
  //
  // Restore Microcode Data
  //
  Microcode = (VOID *)(UINTN)AcpiCpuData->MicrocodePointerBuffer;
  LockBoxMicrocode = (EFI_CPU_MICROCODE_HEADER *)((UINT8 *)SmramCpuData + SmramCpuData->MicrocodeDataBufferOffset);
  if (Microcode != NULL) {
    Index = 0;
    MicrocodeSize = 0;
    while (Microcode[Index] != NULL) {
      if (LockBoxMicrocode->DataSize == 0) {
        MicrocodeSize = 2048;
      } else {
        MicrocodeSize = LockBoxMicrocode->TotalSize;
      }
      CopyMem (Microcode[Index], LockBoxMicrocode, MicrocodeSize);
      LockBoxMicrocode = (EFI_CPU_MICROCODE_HEADER *)((UINT8 *)LockBoxMicrocode + MicrocodeSize);
      Index ++;
    }
  }

  //
  // Close all SMM regions
  //
  Index = 0;
  do {
    Status = SmmAccessPpi->Close ((EFI_PEI_SERVICES**)PeiServices, SmmAccessPpi, Index);
    Index++;
  } while (!EFI_ERROR (Status));

  return AcpiCpuData;
}

/**

  Initialize multiple processors.

  @param[in]  ImageHandle - Image handle of the loaded driver
  @param[in]  SystemTable - Pointer to the System Table

  @retval    EFI_SUCCESS - Multiple processors are intialized successfully

**/
EFI_STATUS
InitializeCpu (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                 Status;
  EFI_PEI_STALL_PPI          *PeiStall;
  ACPI_CPU_DATA_COMPATIBILITY *mAcpiCpuData;
  EFI_BOOT_MODE              BootMode;
  UINT32                     FailedRevision;
  UINT64                     *MtrrValues;
  MP_CPU_S3_DATA_POINTER     *CpuS3DataPtr;
  BOOLEAN                    VerifyMicrocodeChecksum;

  DEBUG ((DEBUG_ERROR, "Cpu S3 dispatch\n"));

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    //
    // If not in S3 boot path. do nothing
    //
    return EFI_SUCCESS;
  }

  if (BootMode != BOOT_ON_S3_RESUME) {
    return EFI_SUCCESS;
  }

  //
  // Restore ACPI Nvs data from SMRAM
  //

  mAcpiCpuData = RestoreSmramCpuData (PeiServices);

  DEBUG ((DEBUG_ERROR, "CpuS3 RestoreSmramCpuData - 0x%x \n", mAcpiCpuData));
  if (mAcpiCpuData == NULL) {
    ASSERT(mAcpiCpuData != NULL);
    return EFI_NOT_READY;
  }

  mAcpiCpuData->S3BootPath  = TRUE;

  Status =  PeiServicesLocatePpi (
                    &gEfiPeiStallPpiGuid,
                    0,
                    NULL,
                    (VOID **)&PeiStall
                  );
  ASSERT_EFI_ERROR (Status);

  CpuS3DataPtr = (MP_CPU_S3_DATA_POINTER *)(UINTN)mAcpiCpuData->CpuPrivateData;

  //
  // Program virtual wire mode for BSP
  //
  ProgramVirtualWireMode (TRUE, CpuS3DataPtr->VirtualWireMode);
  VerifyMicrocodeChecksum = TRUE;
  InitializeMicrocode ((EFI_CPU_MICROCODE_HEADER **)(UINTN) mAcpiCpuData->MicrocodePointerBuffer, &FailedRevision, TRUE, &VerifyMicrocodeChecksum);

  //
  // Restore features for BSP
  //
  // TODO: InitializeFeatures ((MP_CPU_S3_SCRIPT_DATA *)CpuS3DataPtr->S3BootScriptTable);

  Status =  PeiServicesAllocatePool (
                               (FixedMtrrNumber + MtrrDefTypeNumber + VariableMtrrNumber) * sizeof (UINT64),
                               (VOID **) &MtrrValues
                               );
  // 
  // TODO: This has been done by FSP.
  // SetBspMtrrRegisters ((EFI_MTRR_VALUES *)CpuS3DataPtr->S3BspMtrrTable);
  //
  ReadMtrrRegisters (MtrrValues);

  //
  // Restore configuration for APs and synch up Ap's MTRRs with BSP
  //
  WakeUpAPs ((CONST EFI_PEI_SERVICES**)PeiServices, PeiStall, mAcpiCpuData, MtrrValues, VerifyMicrocodeChecksum);
  
  return EFI_SUCCESS;
}

