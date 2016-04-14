/** @file
  Header file for PCH common Initialization Driver.

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_INIT_COMMON_DRIVER_H_
#define _PCH_INIT_COMMON_DRIVER_H_

#include <Protocol/PchPlatformPolicy.h>
#include <Library/S3BootScriptLib.h>

#define PCH_INIT_COMMON_SCRIPT_IO_WRITE(TableName, Width, Address, Count, Buffer) \
          S3BootScriptSaveIoWrite (Width, Address, Count, Buffer)

#define PCH_INIT_COMMON_SCRIPT_IO_READ_WRITE(TableName, Width, Address, Data, DataMask) \
          S3BootScriptSaveIoReadWrite (Width, Address, Data, DataMask)

#define PCH_INIT_COMMON_SCRIPT_MEM_WRITE(TableName, Width, Address, Count, Buffer) \
          S3BootScriptSaveMemWrite (Width, Address, Count, Buffer)

#define PCH_INIT_COMMON_SCRIPT_MEM_READ_WRITE(TableName, Width, Address, Data, DataMask) \
          S3BootScriptSaveMemReadWrite (Width, Address, Data, DataMask)

#define PCH_INIT_COMMON_SCRIPT_PCI_CFG_WRITE(TableName, Width, Address, Count, Buffer) \
          S3BootScriptSavePciCfgWrite (Width, Address, Count, Buffer)

#define PCH_INIT_COMMON_SCRIPT_PCI_CFG_READ_WRITE(TableName, Width, Address, Data, DataMask) \
          S3BootScriptSavePciCfgReadWrite (Width, Address, Data, DataMask)

#define PCH_INIT_COMMON_SCRIPT_STALL(Duration) \
          S3BootScriptSaveStall (Duration)

#define PCH_INIT_COMMON_SCRIPT_MEM_POLL(Width, Address, BitMask, BitValue, Duration, LoopTimes) \
          S3BootScriptSaveMemPoll (Width, Address, BitMask, BitValue, Duration, LoopTimes)

#define PCH_INIT_COMMON_SCRIPT_MSG_BUS_AND_THEN_OR_32_S3_ITEM(PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode) \
          SetMsgBusAndThenOr32S3Item(PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode)

#define PCH_INIT_COMMON_SCRIPT_MSG_BUS_AND_THEN_OR_EX_32_S3_ITEM(PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode, Bar, Device, Function) \
          SetMsgBusAndThenOrEx32S3Item(PortId, Register, AndData, OrData, ReadOpCode, WriteOpCode, Bar, Device, Function)

/**
  Set an IOSF-SB extended programming S3 dispatch item, this function may assert if any error happened

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SetMsgBusAndThenOrEx32S3Item (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          AndData,
  IN     UINT32                          OrData,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           WriteOpCode,
  IN     UINT8                           Bar,
  IN     UINT8                           Device,
  IN     UINT8                           Function
  );

/**
  Set an IOSF-SB programming S3 dispatch item, this function may assert if any error happened

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SetMsgBusAndThenOr32S3Item (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          AndData,
  IN     UINT32                          OrData,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           WriteOpCode
  );

#endif
