/** @file
  SD Host I/O protocol implementation

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SdHostDriver.h"

#define STALL_US(us)       gBS->Stall (us)
#define STALL_MS(ms)       STALL_US ((ms) * 1000)
#define SD_HOST_DEBUG(a)    do { \
                                if (SdHostData->EnableVerboseDebug) { \
                                    DEBUG (a); \
                                  } \
                                } while (0);

UINT32  gSdHostDebugLevel = EFI_D_ERROR;

/**
  Get Error Reason from Host

  @param[in] CommandIndex         Command Index which return error
  @param[in] ErrorCode            Command Error Code

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Command Timeout Error
  @retval EFI_TIMEOUT             Data Timeout Error
  @retval EFI_CRC_ERROR           Command CRC Error
  @retval EFI_CRC_ERROR           Data CRC Error
  @retval EFI_DEVICE_ERROR        Command End Bit Error
                                  Command Index Error
                                  Data End Bit Error
                                  Current Limit Error
                                  Auto CMD12 Error
                                  ADMA Error
**/
STATIC
EFI_STATUS
GetErrorReason (
  IN  UINT16    CommandIndex,
  IN  UINT16    ErrorCode
  )
{
  EFI_STATUS    Status;

  Status = EFI_DEVICE_ERROR;

  DEBUG ((gSdHostDebugLevel, "[%2d] -- ", CommandIndex));

  if (ErrorCode & BIT0) {
    Status = EFI_TIMEOUT;
    DEBUG ((gSdHostDebugLevel, "Command Timeout Error"));
  }

  if (ErrorCode & BIT1) {
    Status = EFI_CRC_ERROR;
    DEBUG ((gSdHostDebugLevel, "Command CRC Error"));
  }

  if (ErrorCode & BIT2) {
    DEBUG ((gSdHostDebugLevel, "Command End Bit Error"));
  }

  if (ErrorCode & BIT3) {
    DEBUG ((gSdHostDebugLevel, "Command Index Error"));
  }
  if (ErrorCode & BIT4) {
    Status = EFI_TIMEOUT;
    DEBUG ((gSdHostDebugLevel, "Data Timeout Error"));
  }

  if (ErrorCode & BIT5) {
    Status = EFI_CRC_ERROR;
    DEBUG ((gSdHostDebugLevel, "Data CRC Error"));
  }

  if (ErrorCode & BIT6) {
    DEBUG ((gSdHostDebugLevel, "Data End Bit Error"));
  }

  if (ErrorCode & BIT7) {
    DEBUG ((gSdHostDebugLevel, "Current Limit Error"));
  }

  if (ErrorCode & BIT8) {
    DEBUG ((gSdHostDebugLevel, "Auto CMD12 Error"));
  }

  if (ErrorCode & BIT9) {
    DEBUG ((gSdHostDebugLevel, "ADMA Error"));
  }

  DEBUG ((gSdHostDebugLevel, "\n"));

  return Status;
}

/**
  Read SD Host PCI Config Space Register

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Width                Width of PCI IO protocol width
  @param[in] Offset               PCI config space register offset
  @param[in] Buffer               A pointer to buffer of read value

  @retval EFI_SUCCESS             The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED         BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED         The address range specified by Offset, Width, and Count is not
                                  valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES    The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
**/
STATIC
EFI_STATUS
SdHostRead (
  IN     SDHOST_DATA                  *SdHost,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINTN                        Offset,
  IN OUT VOID                         *Buffer
  )
{
  return SdHost->PciIo->Mem.Read (
                              SdHost->PciIo,
                              Width,
                              0,
                              (UINT64) Offset,
                              1,
                              Buffer
                              );
}

/**
  Read SD Host PCI Config Space Register in 8 bits

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Offset               PCI config space register offset

  @retval Data                    Register value read in 8 bits
**/
UINT8
SdHostRead8 (
  IN     SDHOST_DATA                  *SdHost,
  IN     UINTN                        Offset
  )
{
  UINT8 Data;

  SdHostRead (SdHost, EfiPciIoWidthUint8, Offset, &Data);

  return Data;
}

/**
  Read SD Host PCI Config Space Register in 16 bits

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Offset               PCI config space register offset

  @retval Data                    Register value read in 16 bits
**/
UINT16
SdHostRead16 (
  IN     SDHOST_DATA                  *SdHost,
  IN     UINTN                        Offset
  )
{
  UINT16 Data;

  SdHostRead (SdHost, EfiPciIoWidthUint16, Offset, &Data);

  return Data;
}

/**
  Read SD Host PCI Config Space Register in 32 bits

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Offset               PCI config space register offset

  @retval Data                    Register value read in 32 bits
**/
UINT32
SdHostRead32 (
  IN     SDHOST_DATA                  *SdHost,
  IN     UINTN                        Offset
  )
{
  UINT32 Data;

  SdHostRead (SdHost, EfiPciIoWidthUint32, Offset, &Data);

  return Data;
}

/**
  Debug Print for SD Host Registers

  @param[in] SdHost               A pointer to SDHOST_DATA structure

**/
VOID
DebugPrintSdHostRegisters (
  IN     SDHOST_DATA                  *SdHost
  )
{
#ifdef VERBOSE_REGISTER_ACCESS_DEBUG
  UINTN Loop;

  if (!SdHost->EnableVerboseDebug) {
    return;
  }

  for (Loop = 0; Loop < 0x40; Loop++) {
    DEBUG ((EFI_D_INFO, " %02x", SdHostRead8 (SdHost, Loop)));
    if ((Loop % 0x10) == 0xf) {
      DEBUG ((EFI_D_INFO, "\n"));
    }
  }
#endif
}
/**
  Debug Print for SD Host Registers before write

  @param[in] SdHost               A pointer to SDHOST_DATA structure

**/
VOID
DebugPreSdHostWrite (
  IN     SDHOST_DATA                  *SdHost
  )
{
#ifdef VERBOSE_REGISTER_ACCESS_DEBUG
  STATIC UINTN DebugCount = 0;

  if (!SdHost->EnableVerboseDebug) {
    return;
  }

  DebugCount++;

  if (DebugCount < 0x100) {
    DEBUG ((EFI_D_INFO, "SD HOST Registers before write:\n"));
    DebugPrintSdHostRegisters (SdHost);
  }
#endif
}

/**
  Debug Print for SD Host Registers post write

  @param[in] SdHost               A pointer to SDHOST_DATA structure

**/
VOID
DebugPostSdHostWrite (
  IN     SDHOST_DATA                  *SdHost
  )
{
#ifdef VERBOSE_REGISTER_ACCESS_DEBUG
  STATIC UINTN DebugCount = 0;

  if (!SdHost->EnableVerboseDebug) {
    return;
  }

  DebugCount++;

  if (DebugCount < 0x10) {
    DEBUG ((EFI_D_INFO, "SD HOST Registers after write:\n"));
    DebugPrintSdHostRegisters (SdHost);
  }
#endif
}

/**
  Write to SD Host PCI Config Space Register

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Width                Width of PCI IO protocol width
  @param[in] Offset               PCI config space register offset
  @param[in] Buffer               A pointer to buffer of read value

  @retval EFI_SUCCESS             The data was read from or written to the PCI controller.
  @retval EFI_UNSUPPORTED         BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED         The address range specified by Offset, Width, and Count is not
                                  valid for the PCI BAR specified by BarIndex.
  @retval EFI_OUT_OF_RESOURCES    The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
**/
STATIC
EFI_STATUS
SdHostWrite (
  IN     SDHOST_DATA                  *SdHost,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINTN                        Offset,
  IN OUT VOID                         *Buffer
  )
{
  EFI_STATUS Status;

  DebugPreSdHostWrite (SdHost);

  Status = SdHost->PciIo->Mem.Write (
                                SdHost->PciIo,
                                Width,
                                0,
                                (UINT64) Offset,
                                1,
                                Buffer
                                );

  DebugPostSdHostWrite (SdHost);

  return Status;
}

/**
  Write SD Host PCI Config Space Register in 8 bits

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Offset               PCI config space register offset
  @param[in] Data                 Register value write in 8 bits

**/
UINT8
SdHostWrite8 (
  IN     SDHOST_DATA                  *SdHost,
  IN     UINTN                        Offset,
  IN     UINT8                        Data
  )
{
  SdHostWrite (SdHost, EfiPciIoWidthUint8, Offset, &Data);

  return Data;
}

/**
  Write SD Host PCI Config Space Register in 16 bits

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Offset               PCI config space register offset
  @param[in] Data                 Register value write in 16 bits

**/
UINT16
SdHostWrite16 (
  IN     SDHOST_DATA                  *SdHost,
  IN     UINTN                        Offset,
  IN     UINT16                       Data
  )
{
  SdHostWrite (SdHost, EfiPciIoWidthUint16, Offset, &Data);

  return Data;
}

/**
  Write SD Host PCI Config Space Register in 32 bits

  @param[in] SdHost               A pointer to SDHOST_DATA structure
  @param[in] Offset               PCI config space register offset
  @param[in] Data                 Register value write in 32 bits

**/
UINT32
SdHostWrite32 (
  IN     SDHOST_DATA                  *SdHost,
  IN     UINTN                        Offset,
  IN     UINT32                       Data
  )
{
  SdHostWrite (SdHost, EfiPciIoWidthUint32, Offset, &Data);

  return Data;
}

/**
  Check Host Controller Version

  @param[in] SdHost               A pointer to SDHOST_DATA structure

  @retval Data16                  Controller version number
**/
UINT32
CheckControllerVersion (
  IN SDHOST_DATA     *SdHost
  )
{
  UINT16 Data16;
  Data16 = SdHostRead16 (SdHost, MMIO_CTRLRVER);
  DEBUG ((EFI_D_INFO, "CheckControllerVersion: %x \n", Data16 & 0xFF));
  return (Data16 & 0xFF);
}

/**
  Power on/off the LED associated with the slot

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] Enable               TRUE to set LED on, FALSE to set LED off

  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
EFI_STATUS
HostLEDEnable (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  BOOLEAN                    Enable
  )
{
  SDHOST_DATA            *SdHostData;
  UINT8                  Data;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  Data = SdHostRead8 (SdHostData, MMIO_HOSTCTL);

  if (Enable) {
    //
    // LED On
    //
    Data |= BIT0;
  } else {
    //
    // LED Off
    //
    Data &= ~BIT0;
  }

  SdHostWrite8 (SdHostData, MMIO_HOSTCTL, Data);

  return EFI_SUCCESS;
}
/**
  The main function used to send the command to the card inserted into the SD/MMC host
  slot. It will assemble the arguments to set the command register and wait for the command
  and transfer completed until timeout. Then it will read the response register to fill
  the ResponseData.

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] CommandIndex         The command index to set the command index field of command register
  @param[in] Argument             Command argument to set the argument field of command register
  @param[in] DataType             TRANSFER_TYPE, indicates no data, data in or data out
  @param[in] Buffer               Contains the data read from / write to the device
  @param[in] BufferSize           The size of the buffer
  @param[in] ResponseType         RESPONSE_TYPE
  @param[in] TimeOut              Time out value in 1 ms unit
  @param[in] ResponseData         Depending on the ResponseType, such as CSD or card status

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_OUT_OF_RESOURCES    A resource has run out.
  @retval EFI_TIMEOUT             The timeout time expired.
  @retval EFI_DEVICE_ERROR        The physical device reported an error while attempting the operation
**/
EFI_STATUS
EFIAPI
SendCommand (
  IN   EFI_SD_HOST_IO_PROTOCOL    *This,
  IN   UINT16                     CommandIndex,
  IN   UINT32                     Argument,
  IN   TRANSFER_TYPE              DataType,
  IN   UINT8                      *Buffer, OPTIONAL
  IN   UINT32                     BufferSize,
  IN   RESPONSE_TYPE              ResponseType,
  IN   UINT32                     TimeOut,
  OUT  UINT32                     *ResponseData OPTIONAL
  )
{
  EFI_STATUS            Status;
  SDHOST_DATA           *SdHostData;
  UINT32                ResponseDataCount;
  UINT16                Data16;
  UINT32                Data32;
  UINT64                Data64;
  UINT8                 Index;
  BOOLEAN               CommandCompleted;
  INT32                 Timeout;

  Status             = EFI_SUCCESS;
  ResponseDataCount  = 1;
  SdHostData         = SDHOST_DATA_FROM_THIS (This);
  Timeout            = 1000;

  if (Buffer != NULL && DataType == NoData) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((gSdHostDebugLevel, "SendCommand: Invalid parameter -> \n"));
    goto Exit;
  }

  if (((UINTN)Buffer & (This->HostCapability.BoundarySize - 1)) != (UINTN)NULL) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((gSdHostDebugLevel, "SendCommand: Invalid parameter -> \n"));
    goto Exit;
  }

  DEBUG ((EFI_D_INFO, "SendCommand: CMD%d \n", CommandIndex));
  if (SdHostData->EnableVerboseDebug) {
    DEBUG ((EFI_D_INFO, "00 -10:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x00),SdHostRead32 (SdHostData, 0x04),SdHostRead32 (SdHostData, 0x08),SdHostRead32 (SdHostData, 0x0C)));
    DEBUG ((EFI_D_INFO, "10 -20:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x10),SdHostRead32 (SdHostData, 0x14),SdHostRead32 (SdHostData, 0x18),SdHostRead32 (SdHostData, 0x1C)));
    DEBUG ((EFI_D_INFO, "20 -30:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x20),SdHostRead32 (SdHostData, 0x24),SdHostRead32 (SdHostData, 0x28),SdHostRead32 (SdHostData, 0x2C)));
    DEBUG ((EFI_D_INFO, "30 -40:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x30),SdHostRead32 (SdHostData, 0x34),SdHostRead32 (SdHostData, 0x38),SdHostRead32 (SdHostData, 0x3C)));
    DEBUG ((EFI_D_INFO, "40 -50:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x40),SdHostRead32 (SdHostData, 0x44),SdHostRead32 (SdHostData, 0x48),SdHostRead32 (SdHostData, 0x4C)));
    DEBUG ((EFI_D_INFO, "50 -60:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x50),SdHostRead32 (SdHostData, 0x54),SdHostRead32 (SdHostData, 0x58),SdHostRead32 (SdHostData, 0x5C)));
  }

  //
  // Check CMD INHIBIT and DATA INHIBIT before send command
  //
  Timeout = 10000;
  do {
    Data32 = SdHostRead32 (SdHostData, MMIO_PSTATE);
    if (SdHostData->EnableVerboseDebug) {
      DEBUG ((EFI_D_INFO, "Wait CMD INHIBIT %x\n", Data32));
    }
    STALL_US (10);
  } while ((Timeout-- > 0) && (Data32 & BIT0));

  Timeout = 90;
  do {
    Data32 = SdHostRead32 (SdHostData, MMIO_PSTATE);
    if (SdHostData->EnableVerboseDebug) {
      DEBUG ((EFI_D_INFO, "Wait DATA INHIBIT %x\n", Data32));
    }
    STALL_US (1100);
  } while ((Timeout-- > 0) && (Data32 & BIT1));

  //
  // Clear status bits
  //
  SD_HOST_DEBUG ((EFI_D_INFO, "NINTSTS(0x30) <- 0x%x\n", (UINTN) 0xffff));
  SdHostWrite16 (SdHostData, MMIO_NINTSTS, 0xffff);
  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: NINTSTS(0x30): 0x%x\n", SdHostRead16 (SdHostData, MMIO_NINTSTS)));

  SD_HOST_DEBUG ((EFI_D_INFO, "ERINTSTS <- 0x%x\n", (UINTN) 0xffff));
  SdHostWrite16 (SdHostData, MMIO_ERINTSTS, 0xffff);
  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: ERINTSTS(0x32): 0x%x\n", SdHostRead16 (SdHostData, MMIO_ERINTSTS)));

  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: DMAADR(0x00) <- 0x%x\n", (UINTN) Buffer));
  SdHostWrite32 (SdHostData, MMIO_DMAADR, (UINT32)(UINTN) Buffer);

  if (Buffer != NULL) {
    Data16 = SdHostRead16 (SdHostData, MMIO_BLKSZ);
    Data16 &= ~(0xFFF);
    if (BufferSize <= SdHostData->BlockLength) {
      Data16 |= BufferSize;
    } else {
      Data16 |= SdHostData->BlockLength;
    }

    Data16 |= 0x7000; // Set to 512KB for SDMA block size

  } else {
    Data16 = 0;
  }

  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: BLKSZ(0x04) <- 0x%x\n", Data16));
  SdHostWrite16 (SdHostData, MMIO_BLKSZ, Data16);

  if (Buffer != NULL) {
    if (BufferSize <= SdHostData->BlockLength) {
      Data16 = 1;
    } else {
      Data16 = (UINT16) (BufferSize / SdHostData->BlockLength);
    }
  } else {
    Data16 = 0;
  }

  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: BLKCNT(0x06) <- 0x%x\n", Data16));
  SdHostWrite16 (SdHostData, MMIO_BLKCNT, Data16);

  //
  // Argument
  //
  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: CMDARG(0x08) <- 0x%x\n", Argument));
  SdHostWrite32 (SdHostData, MMIO_CMDARG, Argument);

  //
  // Transfer Mode
  //
  Data16 = SdHostRead16 (SdHostData, MMIO_XFRMODE);
  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: XFRMODE(0x0C) mode read 0x%x\n", Data16));

  //
  // BIT0 - DMA Enable
  // BIT2 - Auto Cmd12
  //
  if (DataType == InData) {
    Data16 |= BIT4 | BIT0;
  } else if (DataType == OutData) {
    Data16 &= ~BIT4;
    Data16 |= BIT0;
  } else {
    Data16 &= ~(BIT4 | BIT0);
  }

  if (BufferSize <= SdHostData->BlockLength) {
    Data16 &= ~ (BIT5 | BIT1 | BIT2);
  } else {
    if (SdHostData->IsAutoStopCmd && !SdHostData->IsEmmc) {
      Data16 |= (BIT5 | BIT1 | BIT2);
    } else {
      Data16 |= (BIT5 | BIT1);
    }
  }
  if (CommandIndex == SEND_EXT_CSD) {
    Data16 |= BIT1;
    SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand:Enable bit 1, XFRMODE <- 0x%x\n", Data16));
  }

  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: XFRMODE(0x0C) <- 0x%x\n", Data16));
  SdHostWrite16 (SdHostData, MMIO_XFRMODE, Data16);

  //
  // Command
  //
  // ResponseTypeSelect    IndexCheck    CRCCheck    ResponseType
  //  00                     0            0           NoResponse
  //  01                     0            1           R2
  //  10                     0            0           R3, R4
  //  10                     1            1           R1, R5, R6, R7
  //  11                     1            1           R1b, R5b
  //
  switch (ResponseType) {
    case ResponseNo:
      Data16 = (CommandIndex << 8);
      ResponseDataCount = 0;
      break;

    case ResponseR1:
    case ResponseR5:
    case ResponseR6:
    case ResponseR7:
      Data16 = (CommandIndex << 8) | BIT1 | BIT4| BIT3;
      ResponseDataCount = 1;
      break;

    case ResponseR1b:
    case ResponseR5b:
      Data16 = (CommandIndex << 8) | BIT0 | BIT1 | BIT4| BIT3;
      ResponseDataCount = 1;
      break;

    case ResponseR2:
      Data16 = (CommandIndex << 8) | BIT0 | BIT3;
      ResponseDataCount = 4;
      break;

    case ResponseR3:
    case ResponseR4:
      Data16 = (CommandIndex << 8) | BIT1;
      ResponseDataCount = 1;
      break;

    default:
      ASSERT (0);
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
  }

  if (DataType != NoData) {
    Data16 |= BIT5;
  }

  HostLEDEnable (This, TRUE);

  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: SDCMD(0x0E) <- 0x%x\n", Data16));
  CommandCompleted = FALSE;
  SdHostWrite16 (SdHostData, MMIO_SDCMD, Data16);

  if (SdHostData->EnableVerboseDebug) {
    DEBUG ((EFI_D_INFO, "After write to  Command\n"));
    DEBUG ((EFI_D_INFO, "00 -10:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x00),SdHostRead32 (SdHostData, 0x04),SdHostRead32 (SdHostData, 0x08),SdHostRead32 (SdHostData, 0x0C)));
    DEBUG ((EFI_D_INFO, "10 -20:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x10),SdHostRead32 (SdHostData, 0x14),SdHostRead32 (SdHostData, 0x18),SdHostRead32 (SdHostData, 0x1C)));
    DEBUG ((EFI_D_INFO, "20 -30:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x20),SdHostRead32 (SdHostData, 0x24),SdHostRead32 (SdHostData, 0x28),SdHostRead32 (SdHostData, 0x2C)));
    DEBUG ((EFI_D_INFO, "30 -40:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x30),SdHostRead32 (SdHostData, 0x34),SdHostRead32 (SdHostData, 0x38),SdHostRead32 (SdHostData, 0x3C)));
    DEBUG ((EFI_D_INFO, "40 -50:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x40),SdHostRead32 (SdHostData, 0x44),SdHostRead32 (SdHostData, 0x48),SdHostRead32 (SdHostData, 0x4C)));
    DEBUG ((EFI_D_INFO, "50 -60:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x50),SdHostRead32 (SdHostData, 0x54),SdHostRead32 (SdHostData, 0x58),SdHostRead32 (SdHostData, 0x5C)));
  }

  TimeOut *= 1000;  // ms to us conversion
  do {
    Data16 = SdHostRead16 (SdHostData, MMIO_ERINTSTS);
    if (SdHostData->EnableVerboseDebug && (Data16 != 0)) {
      DEBUG ((EFI_D_INFO, "SendCommand: ERINTSTS(0x32): 0x%x\n", Data16));
      DEBUG ((EFI_D_INFO, "SendCommand: ERINTEN(0x36): 0x%x\n", SdHostRead16 (SdHostData, MMIO_ERINTEN)));
      DEBUG ((EFI_D_INFO, "SendCommand: NINTSTS(0x30): 0x%x\n", SdHostRead16 (SdHostData, MMIO_NINTSTS)));
      DEBUG ((EFI_D_INFO, "SendCommand: NINTEN(0x34): 0x%x\n", SdHostRead16 (SdHostData, MMIO_NINTEN)));
    }

    if ((Data16 & 0x17FF) != 0) {
      Status = GetErrorReason (CommandIndex, Data16);
      DEBUG ((EFI_D_INFO, "00 -10:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x00),SdHostRead32 (SdHostData, 0x04),SdHostRead32 (SdHostData, 0x08),SdHostRead32 (SdHostData, 0x0C)));
      DEBUG ((EFI_D_INFO, "10 -20:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x10),SdHostRead32 (SdHostData, 0x14),SdHostRead32 (SdHostData, 0x18),SdHostRead32 (SdHostData, 0x1C)));
      DEBUG ((EFI_D_INFO, "20 -30:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x20),SdHostRead32 (SdHostData, 0x24),SdHostRead32 (SdHostData, 0x28),SdHostRead32 (SdHostData, 0x2C)));
      DEBUG ((EFI_D_INFO, "30 -40:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x30),SdHostRead32 (SdHostData, 0x34),SdHostRead32 (SdHostData, 0x38),SdHostRead32 (SdHostData, 0x3C)));
      DEBUG ((EFI_D_INFO, "40 -50:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x40),SdHostRead32 (SdHostData, 0x44),SdHostRead32 (SdHostData, 0x48),SdHostRead32 (SdHostData, 0x4C)));
      DEBUG ((EFI_D_INFO, "50 -60:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x50),SdHostRead32 (SdHostData, 0x54),SdHostRead32 (SdHostData, 0x58),SdHostRead32 (SdHostData, 0x5C)));
      goto Exit;
    }

    Data16 = SdHostRead16 (SdHostData, MMIO_NINTSTS) & 0x1ff;
    if (SdHostData->EnableVerboseDebug && (Data16 > 1)) {
      DEBUG ((EFI_D_INFO, "SendCommand: NINTSTS(0x30): 0x%x\n", Data16));
    }

    if ((Data16 & BIT0) == BIT0) {
      //
      // Command completed
      //
      CommandCompleted = TRUE;

      SdHostWrite16 (SdHostData, MMIO_NINTSTS, BIT0);

      if ((DataType == NoData) && (ResponseType != ResponseR1b)) {
        break;
      }
    }

    if (CommandCompleted) {
      //
      // DMA interrupted
      //
      if ((Data16 & BIT3) == BIT3) {
        SdHostWrite16 (SdHostData, MMIO_NINTSTS, BIT3);
        Data32 = SdHostRead32 (SdHostData, MMIO_DMAADR);

        SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: DMAADR(0x00) <- 0x%x\n", Data32));
        SdHostWrite32 (SdHostData, MMIO_DMAADR, Data32);
      }

      //
      // Transfer completed
      //
      if ((Data16 & BIT1) == BIT1) {
        SdHostWrite16 (SdHostData, MMIO_NINTSTS, BIT1);
        break;
      }
    }

    STALL_US (1);

    TimeOut --;

  } while (TimeOut > 0);

  if (TimeOut == 0) {
    Status = EFI_TIMEOUT;
    DEBUG ((gSdHostDebugLevel, "SendCommand: Time out \n"));
    goto Exit;
  }

  if (ResponseData != NULL) {
    UINT32 *ResDataPtr = NULL;

    ResDataPtr = ResponseData;
    for (Index = 0; Index < ResponseDataCount; Index++) {
      *ResDataPtr = SdHostRead32 (SdHostData, MMIO_RESP + Index * 4);
      ResDataPtr++;
    }
    SD_HOST_DEBUG ((EFI_D_INFO, "Response Data 0: RESPONSE(0x10) <- 0x%x\n", *ResponseData));

    if (ResponseType == ResponseR2) {
      //
      // Adjustment for R2 response
      //
      Data32 = 1;
      for (Index = 0; Index < ResponseDataCount; Index++) {
        Data64 = LShiftU64(*ResponseData, 8);
        *ResponseData = (UINT32)((Data64 & 0xFFFFFFFF) | Data32);
        Data32 =  (UINT32)RShiftU64 (Data64, 32);
        ResponseData++;
      }
    }
  }

  if (SdHostData->EnableVerboseDebug) {
    DEBUG ((EFI_D_INFO, "Before Exit Send Command\n"));
    DEBUG ((EFI_D_INFO, "00 -10:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x00),SdHostRead32 (SdHostData, 0x04),SdHostRead32 (SdHostData, 0x08),SdHostRead32 (SdHostData, 0x0C)));
    DEBUG ((EFI_D_INFO, "10 -20:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x10),SdHostRead32 (SdHostData, 0x14),SdHostRead32 (SdHostData, 0x18),SdHostRead32 (SdHostData, 0x1C)));
    DEBUG ((EFI_D_INFO, "20 -30:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x20),SdHostRead32 (SdHostData, 0x24),SdHostRead32 (SdHostData, 0x28),SdHostRead32 (SdHostData, 0x2C)));
    DEBUG ((EFI_D_INFO, "30 -40:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x30),SdHostRead32 (SdHostData, 0x34),SdHostRead32 (SdHostData, 0x38),SdHostRead32 (SdHostData, 0x3C)));
    DEBUG ((EFI_D_INFO, "40 -50:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x40),SdHostRead32 (SdHostData, 0x44),SdHostRead32 (SdHostData, 0x48),SdHostRead32 (SdHostData, 0x4C)));
    DEBUG ((EFI_D_INFO, "50 -60:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x50),SdHostRead32 (SdHostData, 0x54),SdHostRead32 (SdHostData, 0x58),SdHostRead32 (SdHostData, 0x5C)));
  }

Exit:
  HostLEDEnable (This, FALSE);
  SD_HOST_DEBUG ((EFI_D_INFO, "SendCommand: Status -> %r\n", Status));

  return Status;
}

/**
  Set max clock frequency of the host, the actual frequency may not be the same
  as MaxFrequency. It depends on the max frequency the host can support, divider,
  and host speed mode.

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] MaxFrequency         Max frequency in HZ

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             The timeout time expired.
**/
EFI_STATUS
EFIAPI
SetClockFrequency (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     MaxFrequency
  )
{
  UINT16                 Data16;
  UINT32                 Frequency;
  UINT32                 Divider = 0;
  SDHOST_DATA            *SdHostData;
  UINT32                 TimeOutCount;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  DEBUG ((gSdHostDebugLevel, "SetClockFrequency: BaseClockInMHz = %d \n", SdHostData->BaseClockInMHz));

  Frequency = (SdHostData->BaseClockInMHz * 1000 * 1000) / MaxFrequency;
  DEBUG ((gSdHostDebugLevel, "SetClockFrequency: FrequencyInHz = %d \n", Frequency));

  if ((SdHostData->BaseClockInMHz * 1000 * 1000 % MaxFrequency) != 0) {
    Frequency += 1;
  }

  Divider = 1;
  while (Frequency > Divider) {
    Divider = Divider * 2;
  }
  if (Divider >= 0x400) {
    Divider = 0x200;
  }
  DEBUG ((gSdHostDebugLevel, "SetClockFrequency: before shift: Base Clock Divider = 0x%x \n", Divider));

  Divider = Divider >> 1;

  DEBUG ((gSdHostDebugLevel, "SetClockFrequency: after shift: Base Clock Divider = 0x%x \n", Divider));

  SdHostData->CurrentClockInKHz = (SdHostData->BaseClockInMHz * 1000);
  if (Divider != 0) {
    SdHostData->CurrentClockInKHz = SdHostData->CurrentClockInKHz / (Divider * 2);
  }

  //
  // Set frequency
  //  Bit[15:8] SDCLK Frequency Select at offset 2Ch
  //    80h - base clock divided by 256
  //    40h - base clock divided by 128
  //    20h - base clock divided by 64
  //    10h - base clock divided by 32
  //    08h - base clock divided by 16
  //    04h - base clock divided by 8
  //    02h - base clock divided by 4
  //    01h - base clock divided by 2
  //    00h - Highest Frequency the target support (10MHz-63MHz)
  //
  //  Bit [07:06] are assigned to bit 09-08 of clock divider in SDCLK Frequency Select on SD controller 3.0
  //
  if (2 == CheckControllerVersion(SdHostData)) {
    Data16 =(UINT16)( (Divider & 0xFF) << 8 | (((Divider & 0xFF00) >>8)<<6));
  } else {
    Data16 = (UINT16) ( Divider << 8);
  }

  DEBUG ((gSdHostDebugLevel,
    "SetClockFrequency: base=%dMHz, clkctl=0x%04x, f=%dKHz\n",
    SdHostData->BaseClockInMHz,
    Data16,
    SdHostData->CurrentClockInKHz
    ));
  DEBUG ((gSdHostDebugLevel, "SetClockFrequency: set MMIO_CLKCTL value = 0x%x \n", Data16));
  SdHostWrite16 (SdHostData, MMIO_CLKCTL, Data16);

  STALL_US (100);
  Data16 |= BIT0;
  SdHostWrite16 (SdHostData, MMIO_CLKCTL, Data16);

  TimeOutCount = TIME_OUT_1S;
  do {
    Data16 = SdHostRead16 (SdHostData, MMIO_CLKCTL);
    STALL_US (100);
    TimeOutCount --;
    if (TimeOutCount == 0) {
      DEBUG ((gSdHostDebugLevel, "SetClockFrequency: Timeout\n"));
      return EFI_TIMEOUT;
    }
  } while ((Data16 & BIT1) != BIT1);

  STALL_MS (1);
  Data16 |= BIT2;
  SdHostWrite16 (SdHostData, MMIO_CLKCTL, Data16);

  return EFI_SUCCESS;
}

/**
  Set bus width of the host

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] BusWidth             Bus width in 1, 4, 8 bits

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
**/
EFI_STATUS
EFIAPI
SetBusWidth (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     BusWidth
  )
{
  SDHOST_DATA            *SdHostData;
  UINT8                  Data;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  if ((BusWidth != 1) && (BusWidth != 4) && (BusWidth != 8)) {
    DEBUG ((gSdHostDebugLevel, "SetBusWidth: Invalid parameter \n"));
    return EFI_INVALID_PARAMETER;
  }

  if ((SdHostData->SdHostIo.HostCapability.BusWidth8 == FALSE) && (BusWidth == 8)) {
    DEBUG ((gSdHostDebugLevel, "SetBusWidth: Invalid parameter \r\n"));
    return EFI_INVALID_PARAMETER;
  }

  Data = SdHostRead8 (SdHostData, MMIO_HOSTCTL);

  //
  // BIT3 8-bit MMC Support (MMC8):
  // If set, SCH supports 8-bit MMC. When cleared, SCH does not support this feature
  //
  if (BusWidth == 8) {
    DEBUG ((gSdHostDebugLevel, "SetBusWidth: Bus Width is 8-bit ... \r\n"));
    Data |= BIT5;
  } else if (BusWidth == 4) {
    DEBUG ((gSdHostDebugLevel, "SetBusWidth: Bus Width is 4-bit ... \r\n"));
    Data &= ~BIT5;
    Data |= BIT1;
  } else {
    DEBUG ((gSdHostDebugLevel, "SetBusWidth: Bus Width is 1-bit ... \r\n"));
    Data &= ~BIT5;
    Data &= ~BIT1;
  }

  SdHostWrite8 (SdHostData, MMIO_HOSTCTL, Data);
  DEBUG ((gSdHostDebugLevel, "SetBusWidth:MMIO_HOSTCTL value: 0x%x  \n", SdHostRead8 (SdHostData, MMIO_HOSTCTL)));

  return EFI_SUCCESS;
}

/**
  Set voltage which could supported by the host.
  Support 0(Power off the host), 1.8V, 3.0V, 3.3V

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] Voltage              Units in 0.1 V

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
**/
EFI_STATUS
EFIAPI
SetHostVoltage (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     Voltage
  )
{
  SDHOST_DATA            *SdHostData;
  UINT8                  Data;
  EFI_STATUS             Status;

  SdHostData = SDHOST_DATA_FROM_THIS (This);
  Status     = EFI_SUCCESS;

  Data = SdHostRead8 (SdHostData, MMIO_PWRCTL);

  if (Voltage == 0) {
    //
    // Power Off the host
    //
    Data &= ~BIT0;
  } else if (Voltage <= 18 && This->HostCapability.V18Support) {
     //
     // 1.8V
     //
     Data |= (BIT1 | BIT3 | BIT0);
  } else if (Voltage > 18 &&  Voltage <= 30 && This->HostCapability.V30Support) {
     //
     // 3.0V
     //
     Data |= (BIT2 | BIT3 | BIT0);
  } else if (Voltage > 30 && Voltage <= 33 && This->HostCapability.V33Support) {
     //
     // 3.3V
     //
     Data |= (BIT1 | BIT2 | BIT3 | BIT0);
  } else {
     Status = EFI_UNSUPPORTED;
     goto Exit;
  }

  SdHostWrite8 (SdHostData, MMIO_PWRCTL, Data);
  STALL_MS (1);

Exit:
  return Status;
}

/**
  Set Host High Speed

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] HighSpeed            True for High Speed Mode set, false for normal mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
**/
EFI_STATUS
EFIAPI
SetHostSpeedMode (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     HighSpeed
  )
{
  SDHOST_DATA           *SdHostData;
  UINT8                 Data8;

  SdHostData         = SDHOST_DATA_FROM_THIS (This);

  if (SdHostData->IsEmmc) {
    Data8 = SdHostRead8 (SdHostData, MMIO_HOSTCTL);
    Data8 &= ~(BIT2);
    if (HighSpeed) {
      Data8 |= BIT2;
      DEBUG ((gSdHostDebugLevel, "High Speed mode: Data8=0x%x \n", Data8));
    } else {
      DEBUG ((gSdHostDebugLevel, "Normal Speed mode: Data8=0x%x \n", Data8));
    }
    SdHostWrite8 (SdHostData, MMIO_HOSTCTL, Data8);
    STALL_US (100);
    DEBUG ((gSdHostDebugLevel, "MMIO_HOSTCTL value: 0x%x  \n", SdHostRead8 (SdHostData, MMIO_HOSTCTL)));
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**
  Set Host DDR Mode

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] DdrMode              True for DDR Mode set, false for normal mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
**/
EFI_STATUS
EFIAPI
SetHostDdrMode (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     DdrMode
  )
{
  SDHOST_DATA           *SdHostData;
  UINT16                ModeSet;

  SdHostData         = SDHOST_DATA_FROM_THIS (This);

  if (SdHostData->IsEmmc) {
    ModeSet = SdHostRead16 (SdHostData, MMIO_HOST_CTL2);
    ModeSet &= ~(BIT0 | BIT1 | BIT2);
    if (DdrMode) {
      ModeSet |= 0x0004;
      ModeSet |= BIT3;  // 1.8V Signaling Enable
      DEBUG ((gSdHostDebugLevel, "DDR mode: Data16=0x%x \n", ModeSet));
    } else {
      if (CheckControllerVersion(SdHostData) != 2) {
        ModeSet =  0x0;
      }
    }

    SdHostWrite16 (SdHostData, MMIO_HOST_CTL2, ModeSet);
    STALL_US (100);
    DEBUG ((gSdHostDebugLevel, "MMIO_HOST_CTL2 value: 0x%x  \n", SdHostRead16 (SdHostData, MMIO_HOST_CTL2)));
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**
  Set Host SDR Mode

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] DdrMode              True for SDR Mode set, false for normal mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
**/
EFI_STATUS
EFIAPI
SetHostSdrMode (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     SdrMode
  )
{
  SDHOST_DATA           *SdHostData;
  UINT16                ModeSet;

  SdHostData         = SDHOST_DATA_FROM_THIS (This);

  if (SdHostData->IsEmmc) {
    ModeSet = SdHostRead16 (SdHostData, MMIO_HOST_CTL2);
    ModeSet &= ~(BIT0 | BIT1 | BIT2);
    if (SdrMode) {
      if (SdHostData->SdHostIo.HostCapability.SDR104Support) {
        ModeSet |= 3;
      } else if (SdHostData->SdHostIo.HostCapability.SDR50Support) {
        ModeSet |= 2;
      }
      ModeSet |= 1;
      ModeSet |= BIT3;  // 1.8V Signaling Enable
      DEBUG ((gSdHostDebugLevel, "SDR mode: Data16=0x%x \n", ModeSet));
    } else {
      if (CheckControllerVersion(SdHostData) != 2) {
        ModeSet =  0x0;
      }
    }

    SdHostWrite16 (SdHostData, MMIO_HOST_CTL2, ModeSet);
    STALL_US (100);
    DEBUG ((gSdHostDebugLevel, "MMIO_HOST_CTL2 value: 0x%x  \n", SdHostRead16 (SdHostData, MMIO_HOST_CTL2)));
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**
  Reset the host

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] ResetAll             TRUE to reset all

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             The timeout time expired.
**/
EFI_STATUS
EFIAPI
ResetSdHost (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  RESET_TYPE                 ResetType
  )
{
  SDHOST_DATA            *SdHostData;
  UINT8                  Data8;
  UINT32                 Data;
  UINT16                 ErrStatus;
  UINT8                  Mask;
  UINT32                 TimeOutCount;
  UINT16                 SaveClkCtl;
  UINT16                 SavePwrCtl = 0;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  if ((SdHostData->PciDid == 0x2294)) {
    SdHostData->IsEmmc = TRUE;
    } else {
    SdHostData->IsEmmc = FALSE;  
  }

  Mask       = 0;
  ErrStatus  = 0;

  if (ResetType == Reset_Auto) {
    ErrStatus = SdHostRead16 (SdHostData, MMIO_ERINTSTS);
    if ((ErrStatus & 0xF) != 0) {
      //
      // Command Line
      //
      Mask |= BIT1;
    }
    if ((ErrStatus & 0x70) != 0) {
      //
      // Data Line
      //
      Mask |= BIT2;
    }
  }

  if (ResetType == Reset_DAT || ResetType == Reset_DAT_CMD) {
    Mask |= BIT2;
  }
  if (ResetType == Reset_CMD || ResetType == Reset_DAT_CMD) {
    Mask |= BIT1;
  }
  if (ResetType == Reset_All) {
    Mask = BIT0;
  }
  if (ResetType == Reset_HW) {
    SavePwrCtl = SdHostRead16 (SdHostData, MMIO_PWRCTL);
    DEBUG ((gSdHostDebugLevel, "Write SavePwrCtl: %x \n", (SavePwrCtl|BIT4)));

    SdHostWrite16 (SdHostData, MMIO_PWRCTL, SavePwrCtl|BIT4);
    STALL_US (100);
    DEBUG ((gSdHostDebugLevel, "Write SavePwrCtl: %x \n", (SavePwrCtl&(~BIT4))));
    SdHostWrite16 (SdHostData, MMIO_PWRCTL, SavePwrCtl&(~BIT4));
    STALL_US (100);
  }

  if (Mask == 0) {
    return EFI_SUCCESS;
  }

  //
  // To improve SD stability, we zero the MMIO_CLKCTL register and
  // stall for 50 microseconds before reseting the controller.  We
  // restore the register setting following the reset operation.
  //
  SaveClkCtl = SdHostRead16 (SdHostData, MMIO_CLKCTL);

  SdHostWrite16 (SdHostData, MMIO_CLKCTL, 0);

  STALL_US (5);

  //
  // Reset the SD host controller
  //
  SdHostWrite8 (SdHostData, MMIO_SWRST, Mask);

  Data          = 0;
  TimeOutCount  = TIME_OUT_1S;
  do {

    STALL_US (100);

    TimeOutCount --;

    Data8 = SdHostRead8 (SdHostData, MMIO_SWRST);
    if ((Data8 & Mask) == 0) {
      break;
    }
  } while (TimeOutCount > 0);

  //
  // We now restore the MMIO_CLKCTL register which we set to 0 above.
  //
  SdHostWrite16 (SdHostData, MMIO_CLKCTL, SaveClkCtl);

  if (TimeOutCount == 0) {
    DEBUG ((gSdHostDebugLevel, "ResetSDHost: Time out \n"));
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  Enable Auto Stop Command

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] Enable               TRUE to enable Auto Stop Command

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             The timeout time expired.
**/
EFI_STATUS
EFIAPI
EnableAutoStopCmd (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  BOOLEAN                    Enable
  )
{
  SDHOST_DATA            *SdHostData;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  SdHostData->IsAutoStopCmd = Enable;

  return EFI_SUCCESS;
}

/**
  Set the Block length

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] BlockLength          Card supported block length

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             The timeout time expired.
**/
EFI_STATUS
EFIAPI
SetBlockLength (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This,
  IN  UINT32                     BlockLength
  )
{
  SDHOST_DATA            *SdHostData;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  DEBUG ((gSdHostDebugLevel, "SetBlockLength: Block length on the host controller: %d \n", BlockLength));
  SdHostData->BlockLength = BlockLength;

  return EFI_SUCCESS;
}

/**
  Find whether these is a card inserted into the slot. If so
  init the host. If not, return EFI_NOT_FOUND.

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           The item was not found.
**/
EFI_STATUS
EFIAPI
DetectCardAndInitHost (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This
  )
{
  SDHOST_DATA            *SdHostData;
  UINT16                 Data16;
  UINT32                 Data;
  EFI_STATUS             Status;
  UINT8                  Voltages[] = { 33, 30, 18 };
  UINTN                  Loop;

  SdHostData = SDHOST_DATA_FROM_THIS (This);

  Data = 0;
  Data = SdHostRead32 (SdHostData, MMIO_PSTATE);

  if ((Data & (BIT16 | BIT17 | BIT18)) != (BIT16 | BIT17 | BIT18)) {
    //
    // Has no card inserted
    //
    DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: No card\n"));
    Status =  EFI_NOT_FOUND;
    goto Exit;
  }
  DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: Card present\n"));

  //
  // Enable normal status change
  //
  SdHostWrite16 (SdHostData, MMIO_NINTEN, BIT1 | BIT0);

  //
  // Enable error status change
  //
  Data16 = SdHostRead16 (SdHostData, MMIO_ERINTEN);
  Data16 |= 0xFFFF; //(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8);
  SdHostWrite16 (SdHostData, MMIO_ERINTEN, Data16);

  //
  // Data transfer Timeout control
  //
  SdHostWrite8 (SdHostData, MMIO_TOCTL, 0x0E);

  //
  // Stall 1 milliseconds to increase SD stability.
  //
  STALL_US (100);

  Status = SetClockFrequency (This, FREQUENCY_OD);
  if (EFI_ERROR (Status)) {
    DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: SetClockFrequency failed\n"));
    goto Exit;
  }
  DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: SetClockFrequency done\n"));

  Status =  EFI_NOT_FOUND;
  for (Loop = 0; Loop < sizeof (Voltages); Loop++) {
    DEBUG ((
      EFI_D_INFO,
      "DetectCardAndInitHost: SetHostVoltage %d.%dV\n",
      Voltages[Loop] / 10,
      Voltages[Loop] % 10
      ));
    Status = SetHostVoltage (This, Voltages[Loop]);
    if (EFI_ERROR (Status)) {
      DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: SetHostVoltage failed\n"));
    } else {
      DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: SetHostVoltage done\n"));
      break;
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((gSdHostDebugLevel, "DetectCardAndInitHost: Failed to SetHostVoltage\n"));
    goto Exit;
  }

Exit:
  return Status;
}

/**
  Setup Device for card initialization

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
SetupDevice (
  IN  EFI_SD_HOST_IO_PROTOCOL    *This
  )
{
  UINT32            tempD;
  INT32             timeOut;
  UINT8             temp8;
  UINT16            temp16;
  SDHOST_DATA       *SdHostData;

  tempD      = 0;
  timeOut    = 1000;
  temp8      = 0;
  temp16     = 0;
  SdHostData = SDHOST_DATA_FROM_THIS (This);

  //
  // Disable SDCLK
  //
  SdHostWrite32 (SdHostData, MMIO_CLKCTL, 0);
  STALL_US(5 * 1000);
  //
  // Disable Bus Power
  //
  SdHostWrite8 (SdHostData, MMIO_PWRCTL, 0);
  STALL_US(5 * 1000);
  //
  // Reset HC and wait for self-clear
  //
  SdHostWrite8 (SdHostData, MMIO_SWRST, 0x7);
  STALL_US(5 * 1000);
  timeOut = 1000;
  do {
    temp8 = SdHostRead8 (SdHostData, MMIO_SWRST);
    timeOut--;
  } while ((temp8 & (1 << 0)) && (timeOut > 0));

  DEBUG ((EFI_D_ERROR, "Reset HC and wait for self-clear Done\n"));
  //
  // Enable all interrupt status bits (NO CARD_INTERRUPT!)
  //
  SdHostWrite16 (SdHostData, MMIO_NINTEN, 0x3);
  //
  // Clear all interrupt status bits
  //
  SdHostWrite16 (SdHostData, MMIO_NINTSTS, 0xFFFF);

  SdHostWrite16 (SdHostData, MMIO_ERINTEN, 0xFFFF);

  if (2 == CheckControllerVersion(SdHostData)) {
    //
    // SD Controller 3.0 uses bit 6, bit7 for Divider bit 8, bit 9. 200M base clock needs 0x100.
    //
    temp16 = (UINT16) (0x1 << 6);
  } else {
    //
    // SD controller 2.0, 100M base clock needs 0x80
    //
    temp16 = (UINT16) (0x80 << 8);
  }
  //
  // Set to 400KHz, enable internal clock and wait for stability
  //
  SdHostWrite32 (SdHostData, MMIO_CLKCTL, (1 << 0) | temp16);

  STALL_US(5 * 1000);
  timeOut = 1000;
  do {
    tempD = SdHostRead32 (SdHostData, MMIO_CLKCTL);
    timeOut--;
  } while ((!(tempD & (1 << 1))) && (timeOut > 0));
  STALL_US(5 * 1000);
  //
  // Enable SD clock
  //
  tempD |= (1 << 2);
  SdHostWrite32 (SdHostData, MMIO_CLKCTL, tempD);
  STALL_US(5 * 1000);

  temp8 = SdHostRead8 (SdHostData, MMIO_PWRCTL);
  DEBUG ((EFI_D_ERROR, "==========%a, %d, offset=0x%x, PWRCTL = 0x%x================\n", __FUNCTION__, __LINE__, MMIO_PWRCTL, temp8));
  //
  // Apply 1.8V to the bus
  //
  temp8 = (0x5 << (1));
  SdHostWrite8 (SdHostData, MMIO_PWRCTL, temp8);
  STALL_US(5 * 1000);
  temp8 = SdHostRead8 (SdHostData, MMIO_PWRCTL);
  DEBUG ((EFI_D_ERROR, "==========%a, %d, set (0x5<<1):offset=0x%x, PWRCTL = 0x%x================\n", __FUNCTION__, __LINE__, MMIO_PWRCTL, temp8));
  //
  // Clear 1.8V signaling Enabled
  //
  temp16 = SdHostRead16 (SdHostData, MMIO_HOST_CTL2);
  temp16 &= (UINT16)~BIT3;
  SdHostWrite16 (SdHostData, MMIO_HOST_CTL2, temp16);
  DEBUG ((gSdHostDebugLevel, "Clear 1.8 V signaling Enable:0x%x \r\n", SdHostRead16 (SdHostData, MMIO_HOST_CTL2)));
  //
  // Apply power to SD
  //
  temp8 = SdHostRead8 (SdHostData, MMIO_PWRCTL);
  DEBUG ((EFI_D_ERROR, "==========%a, %d, read offset=0x%x, PWRCTL = 0x%x================\n", __FUNCTION__, __LINE__, MMIO_PWRCTL, temp8));

  temp8 |= (1 << (0));
  DEBUG ((EFI_D_ERROR, "==========%a, %d, set 1<<0:offset=0x%x, PWRCTL = 0x%x================\n", __FUNCTION__, __LINE__, MMIO_PWRCTL, temp8));

  SdHostWrite8 (SdHostData, MMIO_PWRCTL, temp8);
  STALL_US(5 * 1000);
  temp8 = SdHostRead8 (SdHostData, MMIO_PWRCTL);

  DEBUG ((EFI_D_ERROR, "==========%a, %d, read offset=0x%x, PWRCTL = 0x%x================\n", __FUNCTION__, __LINE__, MMIO_PWRCTL, temp8));

  do {
    temp8 = SdHostRead8 (SdHostData, MMIO_PWRCTL);
    STALL_US (1);
    timeOut--;
  } while ((!(temp8 & (1 << 0))) && (timeOut > 0));
  DEBUG ((EFI_D_ERROR, "==========%a, %d, read offset=0x%x, PWRCTL = 0x%x, timeOut =%d================\n", __FUNCTION__, __LINE__, MMIO_PWRCTL, temp8, timeOut));
  //
  // MAX out the DATA_TIMEOUT
  //
  SdHostWrite8 (SdHostData, MMIO_TOCTL, 0xE);
  STALL_US(5 * 1000);

  if (SdHostData->EnableVerboseDebug) {
    DEBUG ((EFI_D_ERROR, "==========%a, Start. RegMap================\n", __FUNCTION__));
    DEBUG ((EFI_D_INFO, "00 -10:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x00),SdHostRead32 (SdHostData, 0x04),SdHostRead32 (SdHostData, 0x08),SdHostRead32 (SdHostData, 0x0C)));
    DEBUG ((EFI_D_INFO, "10 -20:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x10),SdHostRead32 (SdHostData, 0x14),SdHostRead32 (SdHostData, 0x18),SdHostRead32 (SdHostData, 0x1C)));
    DEBUG ((EFI_D_INFO, "20 -30:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x20),SdHostRead32 (SdHostData, 0x24),SdHostRead32 (SdHostData, 0x28),SdHostRead32 (SdHostData, 0x2C)));
    DEBUG ((EFI_D_INFO, "30 -40:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x30),SdHostRead32 (SdHostData, 0x34),SdHostRead32 (SdHostData, 0x38),SdHostRead32 (SdHostData, 0x3C)));
    DEBUG ((EFI_D_INFO, "40 -50:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x40),SdHostRead32 (SdHostData, 0x44),SdHostRead32 (SdHostData, 0x48),SdHostRead32 (SdHostData, 0x4C)));
    DEBUG ((EFI_D_INFO, "50 -60:  %08x  %08x  %08x  %08x \n", SdHostRead32 (SdHostData, 0x50),SdHostRead32 (SdHostData, 0x54),SdHostRead32 (SdHostData, 0x58),SdHostRead32 (SdHostData, 0x5C)));
    DEBUG ((EFI_D_ERROR, "==========%a, END. RegMap================\n", __FUNCTION__));
  }
  return EFI_SUCCESS;
}

