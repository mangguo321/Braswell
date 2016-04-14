/** @file
  Read, modify or write IOSF-SB register.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchPlatformLibrary.h"
#include "IosfSbDefinitions.h"

/**
  Write DWord data to extended IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] Value                Value to be written
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

**/
VOID
PchMsgBusWriteEx32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          Data32,
  IN     UINT8                           WriteOpCode,
  IN     UINT8                           Bar,          OPTIONAL
  IN     UINT8                           Device,       OPTIONAL
  IN     UINT8                           Function      OPTIONAL
  )
{
  UINTN  PciExpressBase;

  PciExpressBase = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);

  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRXX),
    (UINT32) (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF))
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRX),
    (UINT32) (Register & MSGBUS_MASKHI)
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MDR),
    (UINT32) (Data32)
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCR),
    (UINT32) ((WriteOpCode << 24) | (PortId << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN)
    );
  return;
}

/**
  Write DWord data to IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] Value                Value to be written
  @param[in] WriteOpCode          Write Op Code

**/
VOID
PchMsgBusWrite32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          Data32,
  IN     UINT8                           WriteOpCode
  )
{
  PchMsgBusWriteEx32 (PortId, Register, Data32, WriteOpCode, 0x00, 0x00, 0x00);
  return;
}

/**
  Read DWord data from extended IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] ReadOpCode           Read Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

  @retval Data32                  Value to be read.
**/
UINT32
PchMsgBusReadEx32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           Bar,          OPTIONAL
  IN     UINT8                           Device,       OPTIONAL
  IN     UINT8                           Function      OPTIONAL
  )
{
  UINT32  Data32;
  UINTN   PciExpressBase;

  PciExpressBase = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);

  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRXX),
    (UINT32) (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF))
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRX),
    (UINT32) (Register & MSGBUS_MASKHI)
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCR),
    (UINT32) ((ReadOpCode << 24) | (PortId << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN)
    );
  Data32 = MmioRead32 ((UINTN) (PciExpressBase + MC_MDR));

  return Data32;
}

/**
  Read DWord data from IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] ReadOpCode           Read Op Code

  @retval Data32                  Value to be read.
**/
UINT32
PchMsgBusRead32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT8                           ReadOpCode
  )
{
  return (PchMsgBusReadEx32 (PortId, Register, ReadOpCode, 0x00, 0x00, 0x00));
}

/**
  Read-modify-write DWord data from extended IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

  @retval EFI_SUCCESS             Successfully completed.
**/
EFI_STATUS
EFIAPI
PchMsgBusAndThenOrEx32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          AndData,
  IN     UINT32                          OrData,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           WriteOpCode,
  IN     UINT8                           Bar,          OPTIONAL
  IN     UINT8                           Device,       OPTIONAL
  IN     UINT8                           Function      OPTIONAL
  )
{
  UINT32  Data32;
  UINTN   PciExpressBase;

  PciExpressBase = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);

  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRXX),
    (UINT32) (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF))
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRX),
    (UINT32) (Register & MSGBUS_MASKHI)
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCR),
    (UINT32) ((ReadOpCode << 24) | (PortId << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN)
    );
  Data32 = MmioRead32 ((UINTN) (PciExpressBase + MC_MDR));
  Data32 = (UINT32) ((Data32 & AndData) | OrData);

  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRXX),
    (UINT32) (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF))
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCRX),
    (UINT32) (Register & MSGBUS_MASKHI)
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MDR),
    (UINT32) (Data32)
    );
  MmioWrite32 (
    (UINTN) (PciExpressBase + MC_MCR),
    (UINT32) ((WriteOpCode << 24) | (PortId << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN)
    );
  return EFI_SUCCESS;
}

/**
  Read-modify-write DWord data from IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code

  @retval EFI_SUCCESS             Successfully completed.
**/
EFI_STATUS
EFIAPI
PchMsgBusAndThenOr32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          AndData,
  IN     UINT32                          OrData,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           WriteOpCode
  )
{
  return (PchMsgBusAndThenOrEx32 (PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode, 0x00, 0x00, 0x00));
}

