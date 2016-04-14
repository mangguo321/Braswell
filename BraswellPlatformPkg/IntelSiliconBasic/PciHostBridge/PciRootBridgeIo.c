/** @file
  EFI Memory Controller PCI Root Bridge Io Protocol

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PciRootBridge.h"
#include <IndustryStandard/Pci22.h>

typedef struct {
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR SpaceDesp[TypeMax];
  EFI_ACPI_END_TAG_DESCRIPTOR       EndDesp;
} RESOURCE_CONFIGURATION;

RESOURCE_CONFIGURATION  Configuration = {
  {
    {
      0x8A,
      0x2B,
      1,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    },
    {
      0x8A,
      0x2B,
      0,
      0,
      0,
      32,
      0,
      0,
      0,
      0
    },
    {
      0x8A,
      0x2B,
      0,
      0,
      6,
      32,
      0,
      0,
      0,
      0
    },
    {
      0x8A,
      0x2B,
      0,
      0,
      0,
      64,
      0,
      0,
      0,
      0
    },
    {
      0x8A,
      0x2B,
      0,
      0,
      6,
      64,
      0,
      0,
      0,
      0
    },
    {
      0x8A,
      0x2B,
      2,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    }
  },
  {
    0x79,
    0
  }
};

//
// Protocol Member Function Prototypes
//
EFI_STATUS
EFIAPI
RootBridgeIoPollMem (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN  UINT64                                 Address,
  IN  UINT64                                 Mask,
  IN  UINT64                                 Value,
  IN  UINT64                                 Delay,
  OUT UINT64                                 *Result
  );

EFI_STATUS
EFIAPI
RootBridgeIoPollIo (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN  UINT64                                 Address,
  IN  UINT64                                 Mask,
  IN  UINT64                                 Value,
  IN  UINT64                                 Delay,
  OUT UINT64                                 *Result
  );

EFI_STATUS
EFIAPI
RootBridgeIoMemRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  );

EFI_STATUS
EFIAPI
RootBridgeIoMemWrite (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  );

EFI_STATUS
EFIAPI
RootBridgeIoIoRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 UserAddress,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *UserBuffer
  );

EFI_STATUS
EFIAPI
RootBridgeIoIoWrite (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 UserAddress,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *UserBuffer
  );

EFI_STATUS
EFIAPI
RootBridgeIoCopyMem (
  IN     struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    Width,
  IN     UINT64                                   DestAddress,
  IN     UINT64                                   SrcAddress,
  IN     UINTN                                    Count
  );

EFI_STATUS
EFIAPI
RootBridgeIoPciRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  );

EFI_STATUS
EFIAPI
RootBridgeIoPciWrite (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  );

EFI_STATUS
EFIAPI
RootBridgeIoMap (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL            *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION  Operation,
  IN     VOID                                       *HostAddress,
  IN OUT UINTN                                      *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS                       *DeviceAddress,
  OUT    VOID                                       **Mapping
  );

EFI_STATUS
EFIAPI
RootBridgeIoUnmap (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  VOID                             *Mapping
  );

EFI_STATUS
EFIAPI
RootBridgeIoAllocateBuffer (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  EFI_ALLOCATE_TYPE                Type,
  IN  EFI_MEMORY_TYPE                  MemoryType,
  IN  UINTN                            Pages,
  OUT VOID                             **HostAddress,
  IN  UINT64                           Attributes
  );

EFI_STATUS
EFIAPI
RootBridgeIoFreeBuffer (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  UINTN                            Pages,
  OUT VOID                             *HostAddress
  );

EFI_STATUS
EFIAPI
RootBridgeIoFlush (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This
  );

EFI_STATUS
EFIAPI
RootBridgeIoGetAttributes (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  OUT UINT64                           *Supported,
  OUT UINT64                           *Attributes
  );

EFI_STATUS
EFIAPI
RootBridgeIoSetAttributes (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN     UINT64                           Attributes,
  IN OUT UINT64                           *ResourceBase,
  IN OUT UINT64                           *ResourceLength
  );

EFI_STATUS
EFIAPI
RootBridgeIoConfiguration (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  OUT    VOID                             **Resources
  );

//
// Sub Function Prototypes
//
typedef union {
  UINT8 volatile  *buf;
  UINT8 volatile  *ui8;
  UINT16 volatile *ui16;
  UINT32 volatile *ui32;
  UINT64 volatile *ui64;
  UINTN volatile  ui;
} PTR;

STATIC
EFI_STATUS
RootBridgeIoPciRW (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     BOOLEAN                                Write,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 UserAddress,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *UserBuffer
  );

//
// Memory Controller Pci Root Bridge Io Module Variables
//
EFI_METRONOME_ARCH_PROTOCOL *mMetronome;
EFI_CPU_IO2_PROTOCOL        *mCpuIo;

/**
  Construct the Pci Root Bridge Io protocol

  @param[in]    Protocol            protocol to initialize
  @param[in]    HostBridgeHandle    Host Bridge Handle
  @param[in]    Attri               Host Bridge Attribute
  @param[in]    ResAppeture         Host Bridge resource appeture
  
  @retval  EFI_SUCCESS              Pci Root Bridge Io protocol was constructed successfully.

**/
EFI_STATUS
RootBridgeConstructor (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *Protocol,
  IN EFI_HANDLE                         HostBridgeHandle,
  IN UINT64                             Attri,
  IN PCI_ROOT_BRIDGE_RESOURCE_APPETURE  *ResAppeture
  )
{
  EFI_STATUS                Status;
  PCI_ROOT_BRIDGE_INSTANCE  *PrivateData;
  PCI_RESOURCE_TYPE         Index;

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (Protocol);

  //
  // The host to pci bridge, the host memory and io addresses are
  // direct mapped to pci addresses, so no need translate, set bases to 0.
  //
  PrivateData->MemBase  = ResAppeture->MemBase;
  PrivateData->IoBase   = ResAppeture->IoBase;

  //
  // The host bridge only supports 32bit addressing for memory
  // and standard IA32 16bit io
  //
  PrivateData->MemLimit = ResAppeture->MemLimit;
  PrivateData->IoLimit  = ResAppeture->IoLimit;

  //
  // Bus Appeture for this Root Bridge (Possible Range)
  //
  PrivateData->BusBase  = ResAppeture->BusBase;
  PrivateData->BusLimit = ResAppeture->BusLimit;

  //
  // Specific for this chipset
  //
  for (Index = TypeIo; Index < TypeMax; Index++) {
    PrivateData->ResAllocNode[Index].Type   = Index;
    PrivateData->ResAllocNode[Index].Base   = 0;
    PrivateData->ResAllocNode[Index].Length = 0;
    PrivateData->ResAllocNode[Index].Status = ResNone;
  }

  EfiInitializeLock (&PrivateData->PciLock, TPL_HIGH_LEVEL);
  PrivateData->PciAddress       = 0xCF8;
  PrivateData->PciData          = 0xCFC;

  PrivateData->RootBridgeAttrib = Attri;

  PrivateData->Attributes       = 0;
  PrivateData->Supports = EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO |
    EFI_PCI_ATTRIBUTE_ISA_IO |
    EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO |
    EFI_PCI_ATTRIBUTE_VGA_MEMORY |
    EFI_PCI_ATTRIBUTE_VGA_IO;

  Protocol->ParentHandle    = HostBridgeHandle;

  Protocol->PollMem         = RootBridgeIoPollMem;
  Protocol->PollIo          = RootBridgeIoPollIo;

  Protocol->Mem.Read        = RootBridgeIoMemRead;
  Protocol->Mem.Write       = RootBridgeIoMemWrite;

  Protocol->Io.Read         = RootBridgeIoIoRead;
  Protocol->Io.Write        = RootBridgeIoIoWrite;

  Protocol->CopyMem         = RootBridgeIoCopyMem;

  Protocol->Pci.Read        = RootBridgeIoPciRead;
  Protocol->Pci.Write       = RootBridgeIoPciWrite;

  Protocol->Map             = RootBridgeIoMap;
  Protocol->Unmap           = RootBridgeIoUnmap;

  Protocol->AllocateBuffer  = RootBridgeIoAllocateBuffer;
  Protocol->FreeBuffer      = RootBridgeIoFreeBuffer;

  Protocol->Flush           = RootBridgeIoFlush;

  Protocol->GetAttributes   = RootBridgeIoGetAttributes;
  Protocol->SetAttributes   = RootBridgeIoSetAttributes;

  Protocol->Configuration   = RootBridgeIoConfiguration;

  Protocol->SegmentNumber   = 0;

  Status                    = gBS->LocateProtocol (&gEfiMetronomeArchProtocolGuid, NULL, (VOID **)&mMetronome);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiCpuIo2ProtocolGuid, NULL, (VOID **) &mCpuIo);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  @todo	add description

**/
/**
  Memory Poll
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Mask - add argument and description to function comment
//   @todo    Value - add argument and description to function comment
//   @todo    Delay - add argument and description to function comment
//   @todo    Result - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_TIMEOUT - add return value to function comment
**/
EFI_STATUS
EFIAPI
RootBridgeIoPollMem (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN  UINT64                                 Address,
  IN  UINT64                                 Mask,
  IN  UINT64                                 Value,
  IN  UINT64                                 Delay,
  OUT UINT64                                 *Result
  )
{
  EFI_STATUS  Status;
  UINT64      NumberOfTicks;
  UINTN       Remainder;

  if (Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width > EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // No matter what, always do a single poll.
  //
  Status = This->Mem.Read (This, Width, Address, 1, Result);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((*Result & Mask) == Value) {
    return EFI_SUCCESS;
  }

  if (Delay == 0) {
    return EFI_SUCCESS;

  } else {
    //
    // Determine the proper # of metronome ticks to wait for polling the
    // location.  The nuber of ticks is Roundup (Delay / mMetronome->TickPeriod)+1
    // The "+1" to account for the possibility of the first tick being short
    // because we started in the middle of a tick.
    //
    // BugBug: overriding mMetronome->TickPeriod with UINT32 until Metronome
    // protocol definition is updated.
    //
    NumberOfTicks = DivU64x32Remainder (Delay, (UINT32) mMetronome->TickPeriod, (UINT32 *)&Remainder);
    if (Remainder != 0) {
      NumberOfTicks += 1;
    }

    NumberOfTicks += 1;

    while (NumberOfTicks) {

      mMetronome->WaitForTick (mMetronome, 1);

      Status = This->Mem.Read (This, Width, Address, 1, Result);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if ((*Result & Mask) == Value) {
        return EFI_SUCCESS;
      }

      NumberOfTicks -= 1;
    }
  }

  return EFI_TIMEOUT;
}

/**
  @todo	add description

**/
/**
  Io Poll
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Mask - add argument and description to function comment
//   @todo    Value - add argument and description to function comment
//   @todo    Delay - add argument and description to function comment
//   @todo    Result - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_TIMEOUT - add return value to function comment  

**/
EFI_STATUS
EFIAPI
RootBridgeIoPollIo (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN  UINT64                                 Address,
  IN  UINT64                                 Mask,
  IN  UINT64                                 Value,
  IN  UINT64                                 Delay,
  OUT UINT64                                 *Result
  )
{
  EFI_STATUS  Status;
  UINT64      NumberOfTicks;
  UINTN       Remainder;

  //
  // No matter what, always do a single poll.
  //
  if (Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width > EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }

  Status = This->Io.Read (This, Width, Address, 1, Result);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((*Result & Mask) == Value) {
    return EFI_SUCCESS;
  }

  if (Delay == 0) {
    return EFI_SUCCESS;

  } else {
    //
    // Determine the proper # of metronome ticks to wait for polling the
    // location.  The number of ticks is Roundup (Delay / mMetronome->TickPeriod)+1
    // The "+1" to account for the possibility of the first tick being short
    // because we started in the middle of a tick.
    //
    NumberOfTicks = DivU64x32Remainder (Delay, (UINT32) mMetronome->TickPeriod, (UINT32 *)&Remainder);
    if (Remainder != 0) {
      NumberOfTicks += 1;
    }

    NumberOfTicks += 1;

    while (NumberOfTicks) {

      mMetronome->WaitForTick (mMetronome, 1);

      Status = This->Io.Read (This, Width, Address, 1, Result);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if ((*Result & Mask) == Value) {
        return EFI_SUCCESS;
      }

      NumberOfTicks -= 1;
    }
  }

  return EFI_TIMEOUT;
}

/**
  @todo	add description

**/
/**
  Memory read
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    Buffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment  
**/
EFI_STATUS
EFIAPI
RootBridgeIoMemRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  )
{
  PCI_ROOT_BRIDGE_INSTANCE              *PrivateData;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH OldWidth;
  UINTN                                 OldCount;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  //
  // Check memory access limit
  //
  if (Address < PrivateData->MemBase) {
    return EFI_INVALID_PARAMETER;
  }

  OldWidth  = Width;
  OldCount  = Count;

  if (Width >= EfiPciWidthFifoUint8 && Width <= EfiPciWidthFifoUint64) {
    Count = 1;
  }

  Width &= 0x03;

  if (Address + MultU64x32 (LShiftU64(1, Width), (UINT32)Count) - 1 > PrivateData->MemLimit) {
    return EFI_INVALID_PARAMETER;
  }

  return mCpuIo->Mem.Read (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) OldWidth,
                      Address,
                      OldCount,
                      Buffer
                      );
}

/**
  @todo	add description

**/
/**
  Memory write
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    Buffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
  

**/
EFI_STATUS
EFIAPI
RootBridgeIoMemWrite (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  )
{
  PCI_ROOT_BRIDGE_INSTANCE              *PrivateData;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH OldWidth;
  UINTN                                 OldCount;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  //
  // Check memory access limit
  //
  if (Address < PrivateData->MemBase) {
    return EFI_INVALID_PARAMETER;
  }

  OldWidth  = Width;
  OldCount  = Count;
  if (Width >= EfiPciWidthFifoUint8 && Width <= EfiPciWidthFifoUint64) {
    Count = 1;
  }

  Width &= 0x03;

  if (Address + MultU64x32 (LShiftU64(1, Width), (UINT32)Count) - 1 > PrivateData->MemLimit) {
    return EFI_INVALID_PARAMETER;
  }

  return mCpuIo->Mem.Write (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) OldWidth,
                      Address,
                      OldCount,
                      Buffer
                      );
}

/**
  @todo	add description

**/
/**
  Io read
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    Buffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment  

**/
EFI_STATUS
EFIAPI
RootBridgeIoIoRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN OUT VOID                                   *Buffer
  )
{

  UINTN                                 AlignMask;
  PCI_ROOT_BRIDGE_INSTANCE              *PrivateData;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH OldWidth;
  UINTN                                 OldCount;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  //
  // AlignMask = (1 << Width) - 1;
  //
  AlignMask = (1 << (Width & 0x03)) - 1;

  //
  // check Io access limit
  //
  if (Address < PrivateData->IoBase) {
    return EFI_INVALID_PARAMETER;
  }

  OldWidth  = Width;
  OldCount  = Count;
  if (Width >= EfiPciWidthFifoUint8 && Width <= EfiPciWidthFifoUint64) {
    Count = 1;
  }

  Width &= 0x03;

  if (Address + MultU64x32 (LShiftU64(1, Width), (UINT32)Count) - 1 >= PrivateData->IoLimit) {
    return EFI_INVALID_PARAMETER;
  }

  if (Address & AlignMask) {
    return EFI_INVALID_PARAMETER;
  }

  return mCpuIo->Io.Read (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) OldWidth,
                      Address,
                      OldCount,
                      Buffer
                      );

}

/**
  @todo	add description

**/
/**
  Io write
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    Buffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment  

**/
EFI_STATUS
EFIAPI
RootBridgeIoIoWrite (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL         *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH   Width,
  IN UINT64                                  Address,
  IN UINTN                                   Count,
  IN OUT VOID                                *Buffer
  )
{
  UINTN                                 AlignMask;
  PCI_ROOT_BRIDGE_INSTANCE              *PrivateData;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH OldWidth;
  UINTN                                 OldCount;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  //
  // AlignMask = (1 << Width) - 1;
  //
  AlignMask = (1 << (Width & 0x03)) - 1;

  //
  // Check Io access limit
  //
  if (Address < PrivateData->IoBase) {
    return EFI_INVALID_PARAMETER;
  }

  OldWidth  = Width;
  OldCount  = Count;
  if (Width >= EfiPciWidthFifoUint8 && Width <= EfiPciWidthFifoUint64) {
    Count = 1;
  }

  Width &= 0x03;

  if (Address + MultU64x32 (LShiftU64(1, Width), (UINT32)Count) - 1 >= PrivateData->IoLimit) {
    return EFI_INVALID_PARAMETER;
  }

  if (Address & AlignMask) {
    return EFI_INVALID_PARAMETER;
  }

  return mCpuIo->Io.Write (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) OldWidth,
                      Address,
                      OldCount,
                      Buffer
                      );

}

/**
  @todo	add description

**/
/**
  Memory copy
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    DestAddress - add argument and description to function comment
//   @todo    SrcAddress - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment  

**/
EFI_STATUS
EFIAPI
RootBridgeIoCopyMem (
  IN     struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    Width,
  IN     UINT64                                   DestAddress,
  IN     UINT64                                   SrcAddress,
  IN     UINTN                                    Count
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Direction;
  UINTN       Stride;
  UINTN       Index;
  UINT64      Result;

  if (Width < 0 || Width > EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }

  if (DestAddress == SrcAddress) {
    return EFI_SUCCESS;
  }

  Stride    = (UINTN) (LShiftU64 (1, Width));

  Direction = TRUE;
  if ((DestAddress > SrcAddress) && (DestAddress < (SrcAddress + Count * Stride))) {
    Direction   = FALSE;
    SrcAddress  = SrcAddress + (Count - 1) * Stride;
    DestAddress = DestAddress + (Count - 1) * Stride;
  }

  for (Index = 0; Index < Count; Index++) {
    Status = RootBridgeIoMemRead (
              This,
              Width,
              SrcAddress,
              1,
              &Result
              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = RootBridgeIoMemWrite (
              This,
              Width,
              DestAddress,
              1,
              &Result
              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (Direction) {
      SrcAddress += Stride;
      DestAddress += Stride;
    } else {
      SrcAddress -= Stride;
      DestAddress -= Stride;
    }
  }

  return EFI_SUCCESS;
}

/**
  @todo	add description

**/
/**
  Pci read
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    Buffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment 
**/
EFI_STATUS
EFIAPI
RootBridgeIoPciRead (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN UINT64                                 Address,
  IN UINTN                                  Count,
  IN OUT VOID                               *Buffer
  )
{

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Read Pci configuration space
  //
  return RootBridgeIoPciRW (This, FALSE, Width, Address, Count, Buffer);
}

/**
  @todo	add description

**/
/**
  Pci write
//   @todo    This - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    Address - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    Buffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
**/
EFI_STATUS
EFIAPI
RootBridgeIoPciWrite (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN UINT64                                 Address,
  IN UINTN                                  Count,
  IN OUT VOID                               *Buffer
  )
{

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Write Pci configuration space
  //
  return RootBridgeIoPciRW (This, TRUE, Width, Address, Count, Buffer);
}

/**
    @todo Add function description

  @param[in]  This          -   @todo add argument description
  @param[in]  Operation     -   @todo add argument description
  @param[in]  HostAddress   -   @todo add argument description
  @param[in,out]  NumberOfBytes -   @todo add argument description
  @param[out]  DeviceAddress -   @todo add argument description
  @param[out]  Mapping       -   @todo add argument description

  @retval  EFI_INVALID_PARAMETER -   @todo Add description for return value
  @retval  EFI_INVALID_PARAMETER -   @todo Add description for return value
  @retval  EFI_UNSUPPORTED -   @todo Add description for return value
  @retval  EFI_SUCCESS -   @todo Add description for return value

**/
EFI_STATUS
EFIAPI
RootBridgeIoMap (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL            *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION  Operation,
  IN     VOID                                       *HostAddress,
  IN OUT UINTN                                      *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS                       *DeviceAddress,
  OUT    VOID                                       **Mapping
  )
{
  //
  // Make sure inputs are valid
  //
  if (HostAddress == NULL || NumberOfBytes == NULL || DeviceAddress == NULL || Mapping == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Make sure that Operation is valid
  //
  if (Operation < 0 || Operation >= EfiPciOperationMaximum) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // No mapping required
  //
  *Mapping = NULL;
  *DeviceAddress = (EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress;
  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @param[in]  This    -   @todo add argument description
  @param[in]  Mapping -   @todo add argument description

  @retval  EFI_SUCCESS -   @todo Add description for return value

**/
EFI_STATUS
EFIAPI
RootBridgeIoUnmap (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN VOID                             *Mapping
  )
{
  MAP_INFO  *MapInfo;

  //
  // See if the Map() operation associated with this Unmap() required a mapping buffer.
  // If a mapping buffer was not required, then this function simply returns EFI_SUCCESS.
  //
  if (Mapping != NULL) {
    //
    // Get the MAP_INFO structure from Mapping
    //
    MapInfo = (MAP_INFO *) Mapping;

    //
    // If this is a write operation from the Bus Master's point of view,
    // then copy the contents of the mapped buffer into the real buffer
    // so the processor can read the contents of the real buffer.
    //
    if (MapInfo->Operation == EfiPciOperationBusMasterWrite || MapInfo->Operation == EfiPciOperationBusMasterWrite64) {
      CopyMem (
        (VOID *) (UINTN) MapInfo->HostAddress,
        (VOID *) (UINTN) MapInfo->MappedHostAddress,
        MapInfo->NumberOfBytes
        );
    }
    //
    // Free the mapped buffer and the MAP_INFO structure.
    //
#ifndef __GNUC__
#pragma warning (disable: 4305)
#endif
    FreePages((VOID *)(UINTN)MapInfo->MappedHostAddress, MapInfo->NumberOfPages);
#ifndef __GNUC__
#pragma warning (default: 4305)
#endif
    FreePool(Mapping);
  }

  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @param[in]  This        -   @todo add argument description
  @param[in]  Type        -   @todo add argument description
  @param[in]  MemoryType  -   @todo add argument description
  @param[in]  Pages       -   @todo add argument description
  @param[out]  HostAddress -   @todo add argument description
  @param[in]  Attributes  -   @todo add argument description

  @retval  EFI_UNSUPPORTED -   @todo Add description for return value
  @retval  EFI_INVALID_PARAMETER -   @todo Add description for return value
  @retval  EFI_UNSUPPORTED -   @todo Add description for return value
  @retval  EFI_INVALID_PARAMETER -   @todo Add description for return value
  @retval  EFI_SUCCESS -   @todo Add description for return value

**/
EFI_STATUS
EFIAPI
RootBridgeIoAllocateBuffer (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  EFI_ALLOCATE_TYPE                Type,
  IN  EFI_MEMORY_TYPE                  MemoryType,
  IN  UINTN                            Pages,
  OUT VOID                             **HostAddress,
  IN  UINT64                           Attributes
  )
{
  EFI_PHYSICAL_ADDRESS  PhysicalAddress;

  //
  // Validate Attributes
  //
  if (Attributes & EFI_PCI_ATTRIBUTE_INVALID_FOR_ALLOCATE_BUFFER) {
    return EFI_UNSUPPORTED;
  }
  //
  // Check for invalid inputs
  //
  if (HostAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((EFI_PHYSICAL_ADDRESS) ((UINTN)*HostAddress) > 0xffffffff) {
    return EFI_UNSUPPORTED;
  }
  //
  // The only valid memory types are EfiBootServicesData and EfiRuntimeServicesData
  //
  if (MemoryType != EfiBootServicesData && MemoryType != EfiRuntimeServicesData) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Limit allocations to memory below 4GB
  //
  PhysicalAddress = (EFI_PHYSICAL_ADDRESS) (0xffffffff);

  PhysicalAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)AllocatePages(Pages);
  if (!PhysicalAddress) {
    return EFI_OUT_OF_RESOURCES;
  }

  *HostAddress = (VOID *) (UINTN) PhysicalAddress;

  return EFI_SUCCESS;
}

/**
  @todo Add function description

  @param[in]  This          -   @todo add argument description
  @param[in]  Pages         -   @todo add argument description
  @param[out]  HostAddress  -   @todo add argument description

  @retval                   -   @todo add return values

**/
EFI_STATUS
EFIAPI
RootBridgeIoFreeBuffer (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  UINTN                            Pages,
  OUT VOID                             *HostAddress
  )
{
  FreePages ((VOID *) HostAddress, Pages);
  return EFI_SUCCESS;
}

/**
  @todo	add description

**/
/**
//   @todo    This - add argument and description to function comment
//   @todo    EFI_SUCCESS - add return value to function comment

**/
EFI_STATUS
EFIAPI
RootBridgeIoFlush (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL           *This
  )
{
  //
  // not supported yet
  //
  return EFI_SUCCESS;
}

/**
  @todo	add description

**/
/**
//   @todo    This - add argument and description to function comment
//   @todo    Supported - add argument and description to function comment
//   @todo    Attributes - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment

**/
EFI_STATUS
EFIAPI
RootBridgeIoGetAttributes (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  OUT UINT64                           *Supported,
  OUT UINT64                           *Attributes
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *PrivateData;

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  if (Attributes == NULL && Supported == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Set the return value for Supported and Attributes
  //
  if (Supported) {
    *Supported = PrivateData->Supports;
  }

  if (Attributes) {
    *Attributes = PrivateData->Attributes;
  }

  return EFI_SUCCESS;
}

/**
//   @todo    This - add argument and description to function comment
//   @todo    Attributes - add argument and description to function comment
//   @todo    ResourceBase - add argument and description to function comment
//   @todo    ResourceLength - add argument and description to function comment
//   @todo    EFI_UNSUPPORTED - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment

**/
EFI_STATUS
EFIAPI
RootBridgeIoSetAttributes (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN     UINT64                           Attributes,
  IN OUT UINT64                           *ResourceBase,
  IN OUT UINT64                           *ResourceLength
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *PrivateData;
  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  if (Attributes) {
    if ((Attributes & (~(PrivateData->Supports))) != 0) {
      return EFI_UNSUPPORTED;
    }
  }

  if (Attributes == PrivateData->Attributes) {
    return EFI_SUCCESS;
  }
  //
  // It is just a trick for some attribute can only be enabled or disabled
  // otherwise it can impact on other devices
  //
  PrivateData->Attributes = Attributes;

  return EFI_SUCCESS;
}

/**
  @todo	add description

**/
/**
//   @todo    This - add argument and description to function comment
//   @todo    Resources - add argument and description to function comment
//   @todo    EFI_SUCCESS - add return value to function comment

**/
EFI_STATUS
EFIAPI
RootBridgeIoConfiguration (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  OUT    VOID                             **Resources
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *PrivateData;
  UINTN                     Index;

  PrivateData = DRIVER_INSTANCE_FROM_PCI_ROOT_BRIDGE_IO_THIS (This);

  for (Index = 0; Index < TypeMax; Index++) {
    if (PrivateData->ResAllocNode[Index].Status == ResAllocated) {
      Configuration.SpaceDesp[Index].AddrRangeMin = PrivateData->ResAllocNode[Index].Base;
      Configuration.SpaceDesp[Index].AddrRangeMax = PrivateData->ResAllocNode[Index].Base +
        PrivateData->ResAllocNode[Index].Length -
        1;
      Configuration.SpaceDesp[Index].AddrLen = PrivateData->ResAllocNode[Index].Length;
    }
  }

  *Resources = &Configuration;
  return EFI_SUCCESS;
}

/**
  @todo	add description

**/
/**
//   @todo    This - add argument and description to function comment
//   @todo    Write - add argument and description to function comment
//   @todo    Width - add argument and description to function comment
//   @todo    UserAddress - add argument and description to function comment
//   @todo    Count - add argument and description to function comment
//   @todo    UserBuffer - add argument and description to function comment
//   @todo    EFI_INVALID_PARAMETER - add return value to function comment
//   @todo    EFI_SUCCESS - add return value to function comment

**/
STATIC
EFI_STATUS
RootBridgeIoPciRW (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN BOOLEAN                                Write,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN UINT64                                 UserAddress,
  IN UINTN                                  Count,
  IN OUT VOID                               *UserBuffer
  )
{
  UINT32                                      InStride;
  UINT32                                      OutStride;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *PciRbAddr;
  UINT8                                       *PcieRegAddr;

  if ((Width & 0x03) >= EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }

  InStride  = 1 << (Width & 0x03);
  OutStride = InStride;
  if (Width >= (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH)EfiCpuIoWidthFifoUint8 && Width <= (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH)EfiCpuIoWidthFifoUint64) {
    InStride = 0;
  }

  if (Width >= (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH)EfiCpuIoWidthFillUint8 && Width <= (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH)EfiCpuIoWidthFillUint64) {
    OutStride = 0;
  }

  PciRbAddr = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &UserAddress;

  PcieRegAddr = (UINT8 *) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + 
                                   PCI_EXPRESS_LIB_ADDRESS(PciRbAddr->Bus, PciRbAddr->Device, PciRbAddr->Function, 0));

  //
  // Add the register offset to the address
  //
  if (PciRbAddr->ExtendedRegister != 0) {
    PcieRegAddr += PciRbAddr->ExtendedRegister;
  } else {
    PcieRegAddr += PciRbAddr->Register;
  }

  while (Count) {
    if (Write) {
      This->Mem.Write (This, Width, (UINTN) PcieRegAddr, 1, UserBuffer);
    } else {
      This->Mem.Read (This, Width, (UINTN) PcieRegAddr, 1, UserBuffer);
    }

    UserBuffer = ((UINT8 *) UserBuffer) + OutStride;
    PcieRegAddr += InStride;
    Count -= 1;
  }

  return EFI_SUCCESS;
}
