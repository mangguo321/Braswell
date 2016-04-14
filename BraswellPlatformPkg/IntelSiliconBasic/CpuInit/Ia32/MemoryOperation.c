/** @file
  Memory Operation Functions for IA32 Architecture.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

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

VOID
InitializeIdt (
  IN EFI_CPU_INTERRUPT_HANDLER      *TableStart,
  IN UINTN                          *IdtTablePtr,
  IN UINT16                         IdtTableLimit
  );

extern EFI_CPU_INTERRUPT_HANDLER   mExternalVectorTable[];
extern EFI_PHYSICAL_ADDRESS        mBackupBuffer;

UINT64   mValidMtrrAddressMask = EFI_CACHE_VALID_ADDRESS;
UINT64   mValidMtrrBitsMask    = EFI_MSR_VALID_MASK;

/**
  Slick around interrupt routines.

  @retval  EFI_SUCCESS - If interrupt settings are initialized successfully

**/
EFI_STATUS
InitializeSlick (
  VOID
  )
{
  INTERRUPT_GATE_DESCRIPTOR       *IdtTable;
  INTERRUPT_HANDLER_TEMPLATE_MAP  TemplateMap;
  UINT16                          CodeSegment;
  INTERRUPT_GATE_DESCRIPTOR       *IdtEntry;
  UINT8                           *InterruptHandler;
  UINT8                           *CurrentHandler;
  UINTN                           Index;

  IdtTable  = AllocatePool (sizeof (INTERRUPT_GATE_DESCRIPTOR) * INTERRUPT_VECTOR_NUMBER);
  ASSERT(IdtTable != NULL);
  IdtEntry  = IdtTable;

  GetTemplateAddressMap (&TemplateMap);
  InterruptHandler  = AllocatePool (TemplateMap.Size * INTERRUPT_VECTOR_NUMBER);
  ASSERT(InterruptHandler != NULL);
  CurrentHandler    = InterruptHandler;

  CodeSegment       = GetCodeSegment ();

  for (Index = 0; Index < INTERRUPT_VECTOR_NUMBER; Index++) {
    CopyMem (CurrentHandler, TemplateMap.Start, TemplateMap.Size);
    *(UINT32 *) (CurrentHandler + TemplateMap.FixOffset)  = Index;

    IdtEntry[Index].OffsetLow = (UINT16) (UINTN) CurrentHandler;
    IdtEntry[Index].SegmentSelector = CodeSegment;
    IdtEntry[Index].Attributes = INTERRUPT_GATE_ATTRIBUTE;
    //
    // 8e00;
    //
    IdtEntry[Index].OffsetHigh = (UINT16) ((UINTN) CurrentHandler >> 16);

    CurrentHandler += TemplateMap.Size;
  }

  InitializeIdt (
    &(mExternalVectorTable[0]),
    (UINTN *) IdtTable,
    sizeof (INTERRUPT_GATE_DESCRIPTOR) * INTERRUPT_VECTOR_NUMBER
    );

  return EFI_SUCCESS;
}

/**
  Prepare memory for essential system tables.

  @retval EFI_SUCCESS                Memory successfully prepared.
  @retval  Other                     Error occurred while initializating memory.
  
**/
EFI_STATUS
PrepareMemory (
  VOID
  )
{
  EFI_STATUS  Status;

  ZeroMem (mExternalVectorTable, 0x100 * 4);

  //
  // Initialize the Interrupt Descriptor Table
  //
  Status = InitializeSlick ();
  
  return Status;
}

/**
  Prepare Wakeup Buffer and stack for APs.

  @param[out]  WakeUpBuffer             - Pointer to the address of wakeup buffer for output.
  @param[out]  StackAddressStart        - Pointer to the stack address of APs for output.
  @param[in]  MaximumCPUsForThisSystem  - Maximum CPUs in this system.
  
  @retval  EFI_SUCCESS                  - Memory successfully prepared for APs.
  @retval  Other                        - Error occurred while allocating memory.
  
**/
EFI_STATUS
PrepareMemoryForAPs (
  OUT EFI_PHYSICAL_ADDRESS       *WakeUpBuffer,
  OUT VOID                       **StackAddressStart,
  IN UINTN                       MaximumCPUsForThisSystem
  )
{
  EFI_STATUS                                  Status;
  MP_ASSEMBLY_ADDRESS_MAP                     AddressMap;

  //
  // Release All APs with a lock and wait for them to retire to rendezvous procedure.
  // We need a 64 aligned 4K aligned area for IA-32 to use broadcast APIs. But we need it only
  // on a temporary basis.
  //
  Status = AllocateWakeUpBuffer (WakeUpBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Claim memory for AP stack.
  //
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  MaximumCPUsForThisSystem * STACK_SIZE_PER_PROC,
                  StackAddressStart
                  );
  if (EFI_ERROR (Status)) {
    gBS->FreePages (*WakeUpBuffer, 1);
    return Status;
  }

  AsmGetAddressMap (&AddressMap);
  CopyMem ((VOID *) (UINTN) *WakeUpBuffer, AddressMap.RendezvousFunnelAddress, AddressMap.Size);
  *(UINT32 *) (UINTN) (*WakeUpBuffer + AddressMap.FlatJumpOffset + 3) = (UINT32) (*WakeUpBuffer + AddressMap.PModeEntryOffset);

  return EFI_SUCCESS;
}

/**
  Prepare exchange information for APs.

  @param[out]  *ExchangeInfo      - Pointer to the exchange info buffer for output.
  @param[in]  *StackAddressStart  - Start address of APs' stacks.
  @param[in]  *ApFunction         - Address of function assigned to AP.
  @param[in]  WakeUpBuffer        - Pointer to the address of wakeup buffer.

  @retval  EFI_SUCCESS            - Exchange Info successfully prepared for APs.

**/
EFI_STATUS
PrepareExchangeInfo (
  OUT MP_CPU_EXCHANGE_INFO           *ExchangeInfo,
  IN  VOID                           *StackAddressStart,
  IN  VOID                           *ApFunction,
  IN  EFI_PHYSICAL_ADDRESS           WakeUpBuffer
  )
{
  gBS->SetMem ((VOID *) ExchangeInfo, EFI_PAGE_SIZE - MP_CPU_EXCHANGE_INFO_OFFSET, 0);

  ExchangeInfo->Lock        = VacantFlag;
  ExchangeInfo->StackStart  = StackAddressStart;
  ExchangeInfo->StackSize   = STACK_SIZE_PER_PROC;
  ExchangeInfo->ApFunction  = ApFunction;

  CopyMem (&ExchangeInfo->GdtrProfile, (VOID *) (UINTN) mAcpiCpuData->GdtrProfile, sizeof (IA32_DESCRIPTOR));
  CopyMem (&ExchangeInfo->IdtrProfile, (VOID *) (UINTN) mAcpiCpuData->IdtrProfile, sizeof (IA32_DESCRIPTOR));

  ExchangeInfo->BufferStart = (UINT32) WakeUpBuffer;
  ExchangeInfo->InitFlag    = 1;

  return EFI_SUCCESS;
}

/**
  Prepare Wakeup Buffer and stack for APs during S3.

  @param[out]  WakeUpBuffer      - Pointer to the address of wakeup buffer for output.
  @param[out]  StackAddressStart - Pointer to the stack address of APs for output.

  @retval  EFI_SUCCESS           - Memory successfully prepared for APs.

**/
EFI_STATUS
S3PrepareMemoryForAPs (
  OUT EFI_PHYSICAL_ADDRESS       *WakeUpBuffer,
  OUT VOID                       **StackAddressStart
  )
{
  MP_ASSEMBLY_ADDRESS_MAP        AddressMap;

  *WakeUpBuffer      = mAcpiCpuData->WakeUpBuffer;
  *StackAddressStart = (VOID *) (UINTN) mAcpiCpuData->StackAddress;

  AsmGetAddressMap (&AddressMap);
  CopyMem ((VOID *) (UINTN) *WakeUpBuffer, AddressMap.RendezvousFunnelAddress, AddressMap.Size);
  *(UINT32 *) (UINTN) (*WakeUpBuffer + AddressMap.FlatJumpOffset + 3) = (UINT32) (*WakeUpBuffer + AddressMap.PModeEntryOffset);

  return EFI_SUCCESS;
}

/**
  Prepare exchange information for APs during S3.

  @param[out]  ExchangeInfo      - Pointer to the exchange info for output.
  @param[in]  StackAddressStart  - Start address of APs' stacks.
  @param[in]  ApFunction         - Address of function assigned to AP.
  @param[in]  WakeUpBuffer       - Pointer to the address of wakeup buffer.

  @retval  EFI_SUCCESS           - Exchange Info successfully prepared for APs.

**/
EFI_STATUS
S3PrepareExchangeInfo (
  OUT MP_CPU_EXCHANGE_INFO           *ExchangeInfo,
  IN  VOID                           *StackAddressStart,
  IN  VOID                           *ApFunction,
  IN  EFI_PHYSICAL_ADDRESS           WakeUpBuffer
  )
{
  ExchangeInfo->Lock            = VacantFlag;
  ExchangeInfo->StackStart      = (VOID *) (UINTN) StackAddressStart;
  ExchangeInfo->StackSize       = STACK_SIZE_PER_PROC;
  ExchangeInfo->ApFunction      = ApFunction;

  CopyMem (&ExchangeInfo->GdtrProfile, (VOID *) (UINTN) mAcpiCpuData->GdtrProfile, sizeof (IA32_DESCRIPTOR));
  CopyMem (&ExchangeInfo->IdtrProfile, (VOID *) (UINTN) mAcpiCpuData->IdtrProfile, sizeof (IA32_DESCRIPTOR));

  ExchangeInfo->BufferStart     = (UINT32) WakeUpBuffer;
  ExchangeInfo->InitFlag        = 2;

  //
  // There is no need to initialize CpuNumber and BistBuffer fields in ExchangeInfo here.
  //
  return EFI_SUCCESS;
}

/**
  Dynamically write the far jump destination in APs' wakeup buffer,
  in order to refresh APs' CS registers for mode switching.

**/
VOID
RedirectFarJump (
  VOID
  )
{
  MP_ASSEMBLY_ADDRESS_MAP    AddressMap;

  AsmGetAddressMap (&AddressMap);
  *(UINT32 *) (UINTN) (mAcpiCpuData->WakeUpBuffer + AddressMap.FlatJumpOffset + 3) = (UINT32) (mAcpiCpuData->WakeUpBuffer + AddressMap.PModeEntryOffset);
  return;
}

/**
  Set specified IDT entry with given function pointer.

  @param[in]  FunctionPointer  - Function pointer for IDT entry.
  @param[out]  IdtEntry        - The IDT entry to update.

  @retval                      - The original IDT entry value.

**/
UINTN
SetIdtEntry (
  IN  UINTN                       FunctionPointer,
  OUT INTERRUPT_GATE_DESCRIPTOR   *IdtEntry
)
{
  UINTN  OriginalEntry;

  OriginalEntry = ((UINT32) IdtEntry->OffsetHigh << 16) + IdtEntry->OffsetLow;

  IdtEntry->OffsetLow  = (UINT16) FunctionPointer;
  IdtEntry->OffsetHigh = (UINT16) (FunctionPointer >> 16);

  return OriginalEntry;
}

/**
  @todo Add function description

  @param[out]  *Gdtr   -   @todo add argument description
  @param[out]  *Idtr   -   @todo add argument description

  @retval  EFI_SUCCESS -   @todo Add description for return value

**/
EFI_STATUS
PrepareGdtIdtForAP (
  OUT IA32_DESCRIPTOR          *Gdtr,
  OUT IA32_DESCRIPTOR          *Idtr
  )
{
  INTERRUPT_GATE_DESCRIPTOR   *IdtForAP;
  SEGMENT_DESCRIPTOR          *GdtForAP;
  IA32_DESCRIPTOR             *IdtrForBSP;
  IA32_DESCRIPTOR             *GdtrForBSP;
  UINT8                       *MceHandler;
  EFI_STATUS                  Status;

  AsmGetGdtrIdtr (&GdtrForBSP, &IdtrForBSP);

  //
  // Allocate reserved memory for IDT
  //
  Status = AllocateAlignedReservedMemory (
            IdtrForBSP->Limit + 1,
            8,
            (VOID **) &IdtForAP
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Allocate reserved memory for GDT
  //
  Status = AllocateAlignedReservedMemory (
            GdtrForBSP->Limit + 1,
            8,
            (VOID **) &GdtForAP
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  SIZE_OF_MCE_HANDLER,
                  (VOID **) &MceHandler
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // McheHandler content: iret (opcode = 0xcf)
  //
  *MceHandler = 0xCF;

  CopyMem (GdtForAP, (VOID *) GdtrForBSP->Base, GdtrForBSP->Limit + 1);
  CopyMem (IdtForAP, (VOID *) IdtrForBSP->Base, IdtrForBSP->Limit + 1);

  IdtForAP[INTERRUPT_HANDLER_MACHINE_CHECK].OffsetLow   = (UINT16) (UINTN) MceHandler;
  IdtForAP[INTERRUPT_HANDLER_MACHINE_CHECK].OffsetHigh  = (UINT16) ((UINTN) MceHandler >> 16);

  //
  // Create Gdtr, IDTR profile
  //
  Gdtr->Base  = (UINTN) GdtForAP;
  Gdtr->Limit = GdtrForBSP->Limit;

  Idtr->Base  = (UINTN) IdtForAP;
  Idtr->Limit = IdtrForBSP->Limit;

  return EFI_SUCCESS;
}

