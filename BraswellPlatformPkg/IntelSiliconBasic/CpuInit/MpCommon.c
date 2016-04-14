/** @file
  Code which support multi-processor.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MpCommon.h"
#include "CpuDxe.h"
#include "MiscFuncs.h"
#include <Library/IoLib.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmAccess2.h>

extern MP_SYSTEM_DATA               *mMPSystemData;
extern EFI_PHYSICAL_ADDRESS         mOriginalBuffer;
extern EFI_PHYSICAL_ADDRESS         mBackupBuffer;
extern EFI_METRONOME_ARCH_PROTOCOL  *mMetronome;
volatile UINTN                      mSwitchToLegacyRegionCount = 0;

EFI_GUID  mSmramCpuNvsHeaderGuid = EFI_SMRAM_CPU_NVS_HEADER_GUID;
CHAR16    EfiPlatformCpuInfoVariable[] = L"PlatformCpuInfo";

/**
  @todo	add description

**/
BOOLEAN
IsXapicEnabled (
  VOID
  )
{
  UINT64 MsrValue;

  MsrValue = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);
  if (MsrValue & B_EFI_MSR_IA32_APIC_BASE_APIC_GLOBAL_ENABLE) {
    if (MsrValue & B_EFI_MSR_IA32_APIC_BASE_M_XAPIC) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  @todo Add function description

  @param[in]    @todo add argument description

  @retval       @todo add return values

**/
UINT64
ReadApicMsrOrMemory (
  BOOLEAN   XapicEnabled,
  UINT32    MsrIndex,
  UINT64    MemoryMappedIo
)
{
  UINT64  Value;

  if (XapicEnabled) {
    Value = AsmReadMsr64 (MsrIndex);
  } else {
    Value = (UINT64)*(volatile UINT32 *)(UINTN)MemoryMappedIo;
  }

  return Value;
}

/**
  @todo Add function description

  @param[in]  BSP -   @todo add argument description

  @retval             @todo add return values

**/
VOID
WriteApicMsrOrMemory (
  BOOLEAN   XapicEnabled,
  UINT32    MsrIndex,
  UINT64    MemoryMappedIo,
  UINT64    Value
)
{
  if (XapicEnabled) {
    AsmWriteMsr64 (MsrIndex, Value);
  } else {
    if (MsrIndex == EFI_MSR_EXT_XAPIC_ICR) {
      *(volatile UINT32 *)(UINTN)(MemoryMappedIo - APIC_REGISTER_ICR_LOW_OFFSET + APIC_REGISTER_ICR_HIGH_OFFSET) = (UINT32)(Value >> 32);
    }
    *(volatile UINT32 *)(UINTN)MemoryMappedIo = (UINT32)Value;
  }
}

/**
  @todo Add function description

  @param[in]  BroadcastMode          -   @todo add argument description
  @param[in]  ApicID                 -   @todo add argument description
  @param[in]  VectorNumber           -   @todo add argument description
  @param[in]  DeliveryMode           -   @todo add argument description
  @param[in]  TriggerMode            -   @todo add argument description
  @param[in]  Assert                 -   @todo add argument description

  @retval  EFI_INVALID_PARAMETER     -   @todo Add description for return value
  @retval  EFI_NOT_READY             -   @todo Add description for return value
  @retval  EFI_SUCCESS               -   @todo Add description for return value

**/
EFI_STATUS
SendInterrupt (
  IN  UINT32                               BroadcastMode,
  IN  UINT32                               ApicID,
  IN  UINT32                               VectorNumber,
  IN  UINT32                               DeliveryMode,
  IN  UINT32                               TriggerMode,
  IN  BOOLEAN                              Assert
  )
{
  UINT64                ApicBaseReg;
  EFI_PHYSICAL_ADDRESS  ApicBase;
  UINT32                ICRLow;
  UINT32                ICRHigh;
  BOOLEAN               XapicEnabled;

  if (SINGLE_THREAD_BOOT_FLAG != 0) {
    //
    // Only for Debug to use Single Thread Boot
    //
    return EFI_SUCCESS;
  }

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

  XapicEnabled = IsXapicEnabled ();

  switch (BroadcastMode) {
    case BROADCAST_MODE_SPECIFY_CPU:
      if (XapicEnabled) {
        ICRHigh = (UINT32)ApicID;
      } else {
        ICRHigh = ApicID << 24;
      }
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

  //
  // According Nehalem BWG, if Extended XAPIC Mode is enabled,
  // legacy xAPIC is no longer working.
  // So, previous MMIO offset must be transferred to MSR offset R/W.
  // ----------------------------------------------------------------
  //     MMIO Offset     MSR Offset     Register Name
  // ----------------------------------------------------------------
  //      300h-310h        830h         Interrupt Command Register [63:0]
  //                       831h         [Reserved]
  // ----------------------------------------------------------------
  //
  WriteApicMsrOrMemory (XapicEnabled, EFI_MSR_EXT_XAPIC_ICR, ApicBase + APIC_REGISTER_ICR_LOW_OFFSET, (((UINT64)ICRHigh << 32) | (UINT64)ICRLow));

  gBS->Stall (10);

  ICRLow = (UINT32)ReadApicMsrOrMemory (XapicEnabled, EFI_MSR_EXT_XAPIC_ICR, ApicBase + APIC_REGISTER_ICR_LOW_OFFSET);

  if (ICRLow & 0x1000) {
    return EFI_NOT_READY;
  }

  gBS->Stall (100);

  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @param[out]  *ApicBase OPTIONAL    -   @todo add argument description
  @param[out]  *ApicVersion OPTIONAL -   @todo add argument description

  @retval                            -   @todo add return values

**/
UINT32
GetApicID (
  OUT EFI_PHYSICAL_ADDRESS      *ApicBase OPTIONAL,
  OUT UINT32                    *ApicVersion OPTIONAL
  )
{
  UINT64  ApicBaseReg;
  UINT32  ApicID;
  UINT32  LocalApicVersion;
  UINT64  LocalApicBase;
  UINTN   MsrValue;
  BOOLEAN XapicEnabled;

  XapicEnabled = IsXapicEnabled ();

  if (XapicEnabled) {
    //
    // According to Nehalem BWG, if Extended XAPIC Mode
    // is enabled, legacy xAPIC is no longer working.
    // So, previous MMIO offset must be transfered
    // to MSR offset R/W.
    // MMIO Offset     MSR Offset     Register Name
    //  020h             802h         EFI_EXT_XAPIC_LOGICAL_APIC_ID
    //  030h             803h         EFI_EXT_XAPIC_VERSION
    //
    MsrValue = (UINTN) AsmReadMsr64 (EFI_MSR_EXT_XAPIC_VERSION);
    *ApicVersion  = (UINT32) (MsrValue & B_EFI_MSR_EXT_XAPIC_VERSION_VERSION);
    *ApicBase = 0;

    MsrValue = (UINTN) AsmReadMsr64 (EFI_MSR_EXT_XAPIC_LOGICAL_APIC_ID);
    ApicID  = (UINT32) MsrValue;
    return (ApicID);
  }

  ApicBaseReg   = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);
  LocalApicBase = ApicBaseReg & B_EFI_MSR_IA32_APIC_BASE_APIC_BASE_ADDRESS;
  if (ApicBase) {
    *ApicBase = LocalApicBase;
  }
  //
  // if Apic is not enabled yet, enable it here
  //
  if ((ApicBaseReg & B_EFI_MSR_IA32_APIC_BASE_APIC_GLOBAL_ENABLE) == 0) {
    ApicBaseReg |= B_EFI_MSR_IA32_APIC_BASE_APIC_GLOBAL_ENABLE;
    AsmWriteMsr64 (EFI_MSR_IA32_APIC_BASE, ApicBaseReg);
  }

  if (ApicVersion) {
    LocalApicVersion  = *(volatile UINT32 *) (UINTN) (LocalApicBase + APIC_REGISTER_APIC_VERSION_OFFSET);
    *ApicVersion      = LocalApicVersion & B_APIC_REGISTER_APIC_VERSION_OFFSET_VERSION_MASK;
  }

  ApicID = *(volatile UINT32 *) (UINTN) (LocalApicBase + APIC_REGISTER_LOCAL_ID_OFFSET);
  return ((ApicID & B_APIC_REGISTER_LOCAL_ID_OFFSET_XAPIC_ID_MASK) >> N_APIC_REGISTER_LOCAL_ID_OFFSET_XAPIC_ID_MASK);
}

/**
  @todo Add function description

  @param[in]  BSP -   @todo add argument description

  @retval         -   @todo add return values

**/
VOID
ProgramVirtualWireMode (
  BOOLEAN                       BSP
  )
{
  UINT64                ApicBaseReg;
  EFI_PHYSICAL_ADDRESS  ApicBase;
  UINT64                EntryValue;
  BOOLEAN               XapicEnabled;
  UINT32                VirtualWire;

  VirtualWire = 0;

  ApicBaseReg = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);
  ApicBase    = ApicBaseReg & B_EFI_MSR_IA32_APIC_BASE_APIC_BASE_ADDRESS;

  XapicEnabled = IsXapicEnabled ();

  //
  // Program the Spurious Vector entry if XAPIC is enabled
  //
  EntryValue = ReadApicMsrOrMemory (XapicEnabled, EFI_MSR_EXT_XAPIC_SVR, ApicBase + APIC_REGISTER_SPURIOUS_VECTOR_OFFSET);
  EntryValue &= 0xFFFFFD0F;
  EntryValue |= 0x10F;
  WriteApicMsrOrMemory (XapicEnabled, EFI_MSR_EXT_XAPIC_SVR, ApicBase + APIC_REGISTER_SPURIOUS_VECTOR_OFFSET, EntryValue);

  //
  // Double check if it is BSP
  //
  if (!BSP) {
    CpuDisableInterrupt ();
  }

  //
  // Program the LINT0 vector entry as EntInt
  //
  EntryValue = ReadApicMsrOrMemory (XapicEnabled, EFI_MSR_EXT_XAPIC_LVT_LINT0, ApicBase + APIC_REGISTER_LINT0_VECTOR_OFFSET);
  if ((VirtualWire == VIRT_WIRE_A) && BSP) {
    EntryValue &= 0xFFFE00FF;
    EntryValue |= 0x700;
  } else {
    EntryValue |= 0x10000;      // set bit 16 as mask for LINT0
  }
  WriteApicMsrOrMemory (XapicEnabled, EFI_MSR_EXT_XAPIC_LVT_LINT0, ApicBase + APIC_REGISTER_LINT0_VECTOR_OFFSET, EntryValue);

  if ((VirtualWire != VIRT_WIRE_A) && BSP) {
    //
    // Initialize the IOXApic RT table
    //
    *(volatile UINT8   *)(UINTN)IO_APIC_INDEX_REGISTER = 0x10;
    *(volatile UINT32  *)(UINTN)IO_APIC_DATA_REGISTER  = 0x0700;
    *(volatile UINT8   *)(UINTN)IO_APIC_INDEX_REGISTER = 0x11;
    *(volatile UINT32  *)(UINTN)IO_APIC_DATA_REGISTER  = (GetApicID (NULL, NULL) << 24);
  }
}

/**
  @todo Add function description

  @param[out]  WakeUpBuffer  -   @todo add argument description

  @retval                    -   @todo add return values

**/
EFI_STATUS
AllocateWakeUpBuffer (
  OUT EFI_PHYSICAL_ADDRESS          *WakeUpBuffer
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  for (*WakeUpBuffer = 0x8F000; *WakeUpBuffer >= 0x2000; *WakeUpBuffer -= 0x1000) {
    Status = gBS->AllocatePages (
                    AllocateAddress,
                    EfiACPIMemoryNVS,
                    1,
                    WakeUpBuffer
                    );

    if (!EFI_ERROR (Status)) {
      break;
    }
  }

  return Status;
}

/**
  @todo Add function description

  @param[in]  Size         -   @todo add argument description
  @param[in]  Alignment    -   @todo add argument description
  @param[out]  **Pointer   -   @todo add argument description

  @retval  EFI_SUCCESS     -   @todo Add description for return value

**/
EFI_STATUS
AllocateAlignedReservedMemory (
  IN  UINTN         Size,
  IN  UINTN         Alignment,
  OUT VOID          **Pointer
  )
{
  EFI_STATUS  Status;
  UINTN       PointerValue;

  Status = AllocateReservedMemoryBelow4G (
             Size + Alignment - 1,
             (VOID **) Pointer
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PointerValue  = (UINTN) *Pointer;
  PointerValue  = (PointerValue + Alignment - 1) / Alignment * Alignment;

  *Pointer      = (VOID *) PointerValue;

  return EFI_SUCCESS;
}

/**
  Allocate EfiACPIMemoryNVS below 4G memory address.

  @param[in]  Size       Size of memory to allocate.
  @param[out]  Buffer    Allocated address for output.

  @retval  EFI_SUCCESS   Memory successfully allocated.
  @retval  Other         Other errors occur.

**/
EFI_STATUS
AllocateReservedMemoryBelow4G (
  IN   UINTN   Size,
  OUT  VOID    **Buffer
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;

  Pages = EFI_SIZE_TO_PAGES (Size);
  Address = 0xffffffff;

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );

  *Buffer = (VOID *) (UINTN) Address;

  return Status;
}

#ifdef __GNUC__
#define _ReadWriteBarrier() do { __asm__ __volatile__ ("": : : "memory"); } while(0)

int _outp(
  unsigned short port,
  int databyte
  )
{
  __asm__ __volatile__ ("outb %b0,%w1" : : "a" (databyte), "d" ((UINT16)port));
  return databyte;
}
#else
#pragma intrinsic(_outp, _ReadWriteBarrier)
#endif

/**
  @todo	add description

**/
VOID
EFIAPI
CpuIoWrite8 (
  IN  UINT16  Port,
  IN  UINT32  Data
  )
{
  _ReadWriteBarrier();
  _outp (Port, (int)Data);
}

/**
  This function is invoked when SMM_BASE protocol is installed, then we
  allocate SMRAM and save all information there.

  @param[in]  Event        The triggered event.
  @param[in]  Context      Context for this event.

**/
VOID
EFIAPI
InitSmramDataContent (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  )
{
  EFI_SMM_ACCESS2_PROTOCOL   *SmmAccess;
  EFI_SMRAM_DESCRIPTOR       *SmramRanges;
  UINTN                      Size;
  SMRAM_CPU_DATA             SmramCpuDataTemplate;
  UINTN                      LockBoxSize;
  IA32_DESCRIPTOR          *Idtr;
  IA32_DESCRIPTOR          *Gdtr;
  UINTN                      MicrocodeSize;
  EFI_CPU_MICROCODE_HEADER   **Microcode;
  UINT8                      *LockBoxMicrocode;
  UINTN                      Index;
  EFI_STATUS                 Status;
  EFI_SMM_CONTROL2_PROTOCOL  *SmmControl;
  UINT8                      *SmramCpuData;
  UINTN                      VarSize;
  UINT64                     VarData[3];
  UINTN                      ArgBufferSize;
  UINT8                      ArgBuffer;

  DEBUG ((EFI_D_ERROR, "InitSmramDataContent\n"));
  Status = gBS->LocateProtocol(&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **) &SmmAccess);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol(&gEfiSmmControl2ProtocolGuid, NULL, (VOID **) &SmmControl);
  ASSERT_EFI_ERROR (Status);

  //
  // Get SMRAM information
  //
  Size = 0;
  Status = SmmAccess->GetCapabilities (SmmAccess, &Size, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  Size,
                  (VOID **)&SmramRanges
                  );
  ASSERT_EFI_ERROR (Status);

  Status = SmmAccess->GetCapabilities (
                        SmmAccess,
                        &Size,
                        SmramRanges
                        );
  ASSERT_EFI_ERROR (Status);

  //
  // Open all SMRAM ranges
  //
  Size /= sizeof (*SmramRanges);
  Status = SmmAccess->Open (SmmAccess);
  ASSERT_EFI_ERROR (Status);

  //
  // Init
  //
  CopyMem (&SmramCpuDataTemplate.HeaderGuid, &mSmramCpuNvsHeaderGuid, sizeof(EFI_GUID));
  SmramCpuDataTemplate.AcpiCpuPointer  = (EFI_PHYSICAL_ADDRESS)(UINTN)mAcpiCpuData;
  CopyMem (&SmramCpuDataTemplate.AcpiCpuData, mAcpiCpuData, sizeof(ACPI_CPU_DATA_COMPATIBILITY));

  //
  // Calculate size
  //
  SmramCpuDataTemplate.GdtrProfileSize            = sizeof (IA32_DESCRIPTOR);
  Gdtr = (IA32_DESCRIPTOR *)(UINTN)mAcpiCpuData->GdtrProfile;
  SmramCpuDataTemplate.GdtSize                    = Gdtr->Limit + 1;
  SmramCpuDataTemplate.IdtrProfileSize            = sizeof (IA32_DESCRIPTOR);
  Idtr = (IA32_DESCRIPTOR *)(UINTN)mAcpiCpuData->GdtrProfile;
  SmramCpuDataTemplate.IdtSize                    = Idtr->Limit + 1;
  SmramCpuDataTemplate.CpuPrivateDataSize         = sizeof(MP_CPU_S3_DATA_POINTER);
  SmramCpuDataTemplate.S3BootScriptTableSize      = sizeof(mMPSystemData->S3BootScriptTable);
  SmramCpuDataTemplate.S3BspMtrrTableSize         = sizeof(mMPSystemData->S3BspMtrrTable);
  //
  // Record best match for each CPU Microcode and NULL for end
  //
  SmramCpuDataTemplate.MicrocodePointerBufferSize = sizeof(UINT32) * (mAcpiCpuData->NumberOfCpus + 1);
  //
  // Calculate Microcode DataSize
  //
  SmramCpuDataTemplate.MicrocodeDataBufferSize    = 0;
  Microcode = (VOID *)(UINTN)mAcpiCpuData->MicrocodePointerBuffer;
  if (Microcode != NULL) {
    Index = 0;
    MicrocodeSize = 0;
    while (Microcode[Index] != NULL) {
      if (Microcode[Index]->DataSize == 0) {
        MicrocodeSize = 2048;
      } else {
        MicrocodeSize = Microcode[Index]->TotalSize;
      }
      SmramCpuDataTemplate.MicrocodeDataBufferSize += (UINT32)MicrocodeSize;
      Index ++;
    }
  }

  SmramCpuDataTemplate.GdtrProfileOffset            = sizeof(SMRAM_CPU_DATA);
  SmramCpuDataTemplate.GdtOffset                    = SmramCpuDataTemplate.GdtrProfileOffset +
                                                      SmramCpuDataTemplate.GdtrProfileSize;
  SmramCpuDataTemplate.IdtrProfileOffset            = SmramCpuDataTemplate.GdtOffset +
                                                      SmramCpuDataTemplate.GdtSize;
  SmramCpuDataTemplate.IdtOffset                    = SmramCpuDataTemplate.IdtrProfileOffset +
                                                      SmramCpuDataTemplate.IdtrProfileSize;
  SmramCpuDataTemplate.CpuPrivateDataOffset         = SmramCpuDataTemplate.IdtOffset +
                                                      SmramCpuDataTemplate.IdtSize;
  SmramCpuDataTemplate.S3BootScriptTableOffset      = SmramCpuDataTemplate.CpuPrivateDataOffset +
                                                      SmramCpuDataTemplate.CpuPrivateDataSize;
  SmramCpuDataTemplate.S3BspMtrrTableOffset         = SmramCpuDataTemplate.S3BootScriptTableOffset +
                                                      SmramCpuDataTemplate.S3BootScriptTableSize;
  SmramCpuDataTemplate.MicrocodePointerBufferOffset = SmramCpuDataTemplate.S3BspMtrrTableOffset +
                                                      SmramCpuDataTemplate.S3BspMtrrTableSize;
  SmramCpuDataTemplate.MicrocodeDataBufferOffset    = SmramCpuDataTemplate.MicrocodePointerBufferOffset +
                                                      SmramCpuDataTemplate.MicrocodePointerBufferSize;

  LockBoxSize = sizeof(SMRAM_CPU_DATA) +
                SmramCpuDataTemplate.GdtrProfileSize +
                SmramCpuDataTemplate.GdtSize +
                SmramCpuDataTemplate.IdtrProfileSize +
                SmramCpuDataTemplate.IdtSize +
                SmramCpuDataTemplate.CpuPrivateDataSize +
                SmramCpuDataTemplate.S3BootScriptTableSize +
                SmramCpuDataTemplate.S3BspMtrrTableSize +
                SmramCpuDataTemplate.MicrocodePointerBufferSize +
                SmramCpuDataTemplate.MicrocodeDataBufferSize;

  DEBUG ((EFI_D_ERROR, "LockBoxSize - %x\n", LockBoxSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.GdtrProfileSize            - %x\n", SmramCpuDataTemplate.GdtrProfileSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.GdtSize                    - %x\n", SmramCpuDataTemplate.GdtSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.IdtrProfileSize            - %x\n", SmramCpuDataTemplate.IdtrProfileSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.IdtSize                    - %x\n", SmramCpuDataTemplate.IdtSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.CpuPrivateDataSize         - %x\n", SmramCpuDataTemplate.CpuPrivateDataSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.S3BootScriptTableSize      - %x\n", SmramCpuDataTemplate.S3BootScriptTableSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.S3BspMtrrTableSize         - %x\n", SmramCpuDataTemplate.S3BspMtrrTableSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.MicrocodePointerBufferSize - %x\n", SmramCpuDataTemplate.MicrocodePointerBufferSize));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.MicrocodeDataBufferSize    - %x\n", SmramCpuDataTemplate.MicrocodeDataBufferSize));

  DEBUG ((EFI_D_ERROR, "SmramCpuData.GdtrProfileOffset            - %x\n", SmramCpuDataTemplate.GdtrProfileOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.GdtOffset                    - %x\n", SmramCpuDataTemplate.GdtOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.IdtrProfileOffset            - %x\n", SmramCpuDataTemplate.IdtrProfileOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.IdtOffset                    - %x\n", SmramCpuDataTemplate.IdtOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.CpuPrivateDataOffset         - %x\n", SmramCpuDataTemplate.CpuPrivateDataOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.S3BootScriptTableOffset      - %x\n", SmramCpuDataTemplate.S3BootScriptTableOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.S3BspMtrrTableOffset         - %x\n", SmramCpuDataTemplate.S3BspMtrrTableOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.MicrocodePointerBufferOffset - %x\n", SmramCpuDataTemplate.MicrocodePointerBufferOffset));
  DEBUG ((EFI_D_ERROR, "SmramCpuData.MicrocodeDataBufferOffset    - %x\n", SmramCpuDataTemplate.MicrocodeDataBufferOffset));

  //
  // Allocate Normal Memory
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  LockBoxSize,
                  (VOID **)&SmramCpuData
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate SMRAM
  //
  //
  // Copy data buffer
  //
  CopyMem (SmramCpuData, &SmramCpuDataTemplate, sizeof(SmramCpuDataTemplate));

  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.GdtrProfileOffset,
    (VOID *)(UINTN)mAcpiCpuData->GdtrProfile,
    SmramCpuDataTemplate.GdtrProfileSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.GdtOffset,
    (VOID *)(UINTN)Gdtr->Base,
    SmramCpuDataTemplate.GdtSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.IdtrProfileOffset,
    (VOID *)(UINTN)mAcpiCpuData->IdtrProfile,
    SmramCpuDataTemplate.IdtrProfileSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.IdtOffset,
    (VOID *)(UINTN)Idtr->Base,
    SmramCpuDataTemplate.IdtSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.CpuPrivateDataOffset,
    (VOID *)(UINTN)mAcpiCpuData->CpuPrivateData,
    SmramCpuDataTemplate.CpuPrivateDataSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.S3BootScriptTableOffset,
    (VOID *)(UINTN)mMPSystemData->S3DataPointer.S3BootScriptTable,
    SmramCpuDataTemplate.S3BootScriptTableSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.S3BspMtrrTableOffset,
    (VOID *)(UINTN)mMPSystemData->S3DataPointer.S3BspMtrrTable,
    SmramCpuDataTemplate.S3BspMtrrTableSize
    );
  CopyMem (
    SmramCpuData + SmramCpuDataTemplate.MicrocodePointerBufferOffset,
    (VOID *)(UINTN)mAcpiCpuData->MicrocodePointerBuffer,
    SmramCpuDataTemplate.MicrocodePointerBufferSize
    );
  //
  // Copy Microcode
  //
  LockBoxMicrocode = SmramCpuData + SmramCpuDataTemplate.MicrocodeDataBufferOffset;
  Microcode = (VOID *)(UINTN)mAcpiCpuData->MicrocodePointerBuffer;
  if (Microcode != NULL) {
    Index = 0;
    MicrocodeSize = 0;
    while (Microcode[Index] != NULL) {
      if (Microcode[Index]->DataSize == 0) {
        MicrocodeSize = 2048;
      } else {
        MicrocodeSize = Microcode[Index]->TotalSize;
      }
      CopyMem (LockBoxMicrocode, Microcode[Index], MicrocodeSize);
      LockBoxMicrocode += MicrocodeSize;
      Index ++;
    }
  }

  //
  // Copy to SMRAM
  //
  //
  // We have to use SMI to copy SMRAM, because we can not access SMRAM after SMRR enabled.
  // SMM_ACCESS.Open () takes no effect.
  //
  VarSize = sizeof(VarData);
  VarData[0] = 0;
  VarData[1] = (UINT64)(UINTN)SmramCpuData;
  VarData[2] = (UINT64)LockBoxSize;
  Status = gRT->SetVariable (
                  L"SmramCpuNvs",
                  &mSmramCpuNvsHeaderGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  VarSize,
                  VarData
                  );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Fill SMI data port
  ///
  IoWrite8 (0xB3, 0x81);

  //
  // Trigger SMI
  //
  ArgBufferSize = sizeof (ArgBuffer);
  ArgBuffer = 0x55;
  Status = SmmControl->Trigger (SmmControl, (UINT8 *)&ArgBuffer, (UINT8 *)&ArgBufferSize, FALSE, 0);
  Status = SmmControl->Clear (SmmControl, 0);

  //
  // Close all SMRAM ranges
  //
  Status = SmmAccess->Close (SmmAccess);
  ASSERT_EFI_ERROR (Status);

  //
  // Free resource
  //
  gBS->FreePool (SmramRanges);

  //
  // Done
  //
  return ;
}

/**
  This function is invoked when LegacyBios protocol is installed, we must
  allocate reserved memory under 1M for AP.

  @param[in]   Event         The triggered event.
  @param[in]   Context       Context for this event.

  @retval      None

**/
VOID
EFIAPI
ReAllocateMemoryForAP (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_PHYSICAL_ADDRESS        LegacyRegion;
  EFI_STATUS                  Status;
  MP_CPU_EXCHANGE_INFO       *ExchangeInfo;

  STATIC BOOLEAN              InitDone = FALSE;
  if (InitDone) {
    return ;
  }
  InitDone = TRUE;

  while (ApRunning()) {
    CpuPause ();
  }

  {
    //
    // The BackBuffer is 4k.
    // Allocate 0x2000 bytes from below 640K memory to make sure I can
    // get a 4k aligned spaces of 0x1000 bytes, since Alignment argument does not work.
    //
    LegacyRegion = 0x9FFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES (0x2000),
                    &LegacyRegion
                    );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, "LegacyRegion NonCSM - %x\n", LegacyRegion));
    if (EFI_ERROR (Status)) {
      return ;
    }
  }

  DEBUG((EFI_D_INFO, "LegacyRegion: 0x%08x\r\n", (UINT32)(UINTN)LegacyRegion));
  //
  // This address should be less than A seg.
  // And it should be aligned to 4K
  //
  ASSERT (!((UINTN)LegacyRegion & 0x0FFF) && ((UINTN)LegacyRegion < 0xA0000));

  mAcpiCpuData->WakeUpBuffer = (EFI_PHYSICAL_ADDRESS) LegacyRegion;
  mAcpiCpuData->WakeUpBuffer = (mAcpiCpuData->WakeUpBuffer + 0x0fff) & 0x0fffff000;

  ExchangeInfo               = (MP_CPU_EXCHANGE_INFO *) (UINTN) (mBackupBuffer + MP_CPU_EXCHANGE_INFO_OFFSET);
  ExchangeInfo->BufferStart  = (UINT32) mAcpiCpuData->WakeUpBuffer;
  ExchangeInfo->ApFunction   = (VOID *) (UINTN) LegacyRegionAPCount;

  gBS->CopyMem (
         (VOID *) (UINTN) mAcpiCpuData->WakeUpBuffer,
         (VOID *) (UINTN) mBackupBuffer,
         EFI_PAGE_SIZE
         );
  RedirectFarJump();

  //
  // Do final intialization for APs
  // Is it neccessary?
  //
  SendInterrupt (BROADCAST_MODE_ALL_EXCLUDING_SELF, 0, 0, DELIVERY_MODE_INIT, TRIGGER_MODE_EDGE, TRUE);
  SendInterrupt (
      BROADCAST_MODE_ALL_EXCLUDING_SELF,
      0,
      (UINT32) RShiftU64 (mAcpiCpuData->WakeUpBuffer, 12),
      DELIVERY_MODE_SIPI,
      TRIGGER_MODE_EDGE,
      TRUE
    );
  //
  // Wait until all APs finish
  //
  while (mSwitchToLegacyRegionCount < mAcpiCpuData->NumberOfCpus - 1) {
    CpuPause ();
  }

  ExchangeInfo->ApFunction   = (VOID *) (UINTN) ApProcWrapper;
  gBS->CopyMem (
         (VOID *) (UINTN) mAcpiCpuData->WakeUpBuffer,
         (VOID *) (UINTN) mBackupBuffer,
         EFI_PAGE_SIZE
         );
  RedirectFarJump();

  InitSmramDataContent (NULL, NULL);
}

/**
  This function is invoked by EFI_EVENT_SIGNAL_LEGACY_BOOT.
  Before booting to legacy OS, reset AP's wakeup buffer address,
  preparing for S3 usage.

  @param[in]  Event        The triggered event.
  @param[in]  Context      Context for this event.

  @retval     None

**/
VOID
ResetAPs (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
}
