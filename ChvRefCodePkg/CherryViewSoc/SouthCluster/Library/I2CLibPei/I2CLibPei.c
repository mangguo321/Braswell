/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "I2CDelayPei.h"
#include "I2CIoLibPei.h"
#include "I2CAccess.h"
#include "I2CLibPei.h"
#include <PlatformBaseAddresses.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/HobLib.h>
#include <PchRegs/PchRegsPcu.h>
#include <PchRegs/PchRegsLpss.h>

EFI_GUID  mI2CPeiInitGuid = {
  0x96DED71A, 0xB9E7, 0x4EAD, {0x96, 0x2C, 0x01, 0x69, 0x3C, 0xED, 0x2A, 0x64}
};

typedef struct _LPSS_PCI_DEVICE_INFO {
  UINTN        Segment;
  UINTN        BusNum;
  UINTN        DeviceNum;
  UINTN        FunctionNum;
  UINTN        Bar0;
  UINTN        Bar1;
} LPSS_PCI_DEVICE_INFO;

LPSS_PCI_DEVICE_INFO  mLpssPciDeviceList[] = {
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_DMAC1, PCI_FUNCTION_NUMBER_PCH_LPSS_DMAC, 0xFE900000, 0xFE908000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C0, 0xFE910000, 0xFE918000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C1, 0xFE920000, 0xFE928000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C2, 0xFE930000, 0xFE938000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C3, 0xFE940000, 0xFE948000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C4, 0xFE950000, 0xFE958000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C5, 0xFE960000, 0xFE968000},
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPSS_I2C,   PCI_FUNCTION_NUMBER_PCH_LPSS_I2C6, 0xFE970000, 0xFE978000}
};
#define LPSS_PCI_DEVICE_NUMBER  sizeof(mLpssPciDeviceList)/sizeof(LPSS_PCI_DEVICE_INFO)

#define R_PCH_LPIO_I2C_MEM_RESETS                 0x804 // Software Reset
#define B_PCH_LPIO_I2C_MEM_RESETS_FUNC            BIT1  // Function Clock Domain Reset
#define B_PCH_LPIO_I2C_MEM_RESETS_APB             BIT0  // APB Domain Reset

/**
  @todo	add description

**/
EFI_STATUS
EFIAPI
IntelI2CPeiLibConstructor (
  VOID
  )
{
  //
  //  LPSS I2C0
  //  I2C0_SCL - IOBASE + 0x05428 [31:0] = 0x00C10300
  //  I2C0_SDA - IOBASE + 0x05408 [31:0] = 0x00C10300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05428, 0x00C10300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05408, 0x00C10300);
  //
  //  LPSS I2C1
  //  I2C1_SCL - IOBASE + 0x05418 [31:0] = 0x00910300
  //  I2C1_SDA - IOBASE + 0x05400 [31:0] = 0x00910300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05418, 0x00910300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05400, 0x00910300);
  //
  //  LPSS I2C2
  //  I2C2_SCL - IOBASE + 0x05430 [31:0] = 0x00910300
  //  I2C2_SDA - IOBASE + 0x05410 [31:0] = 0x00910300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05430, 0x00910300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05410, 0x00910300);
  //
  //  LPSS I2C3
  //  I2C3_SCL - IOBASE + 0x05438 [31:0] = 0x00910300
  //  I2C3_SDA - IOBASE + 0x05420 [31:0] = 0x00910300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05438, 0x00910300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05420, 0x00910300);
  //
  //  LPSS I2C4
  //  I2C4_SCL - IOBASE + 0x05028 [31:0] = 0x00910300
  //  I2C4_SDA - IOBASE + 0x05008 [31:0] = 0x00910300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05028, 0x00910300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05008, 0x00910300);
  //
  //  LPSS I2C5
  //  I2C5_SCL - IOBASE + 0x05018 [31:0] = 0x00910300
  //  I2C5_SDA - IOBASE + 0x05000 [31:0] = 0x00910300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05018, 0x00910300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05000, 0x00910300);
  //
  //  LPSS I2C6
  //  I2C6_SCL - IOBASE + 0x05030 [31:0] = 0x00910300
  //  I2C6_SDA - IOBASE + 0x05010 [31:0] = 0x00910300
  //
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05030, 0x00910300);
  I2CLibPeiMmioWrite32 (IO_BASE_ADDRESS + 0x05010, 0x00910300);

  return EFI_SUCCESS;
}
/**
@todo	add description

**/
EFI_STATUS
ProgramAllPciLpssI2C (
  VOID
  )
{
  UINT8 i;
  for (i =0; i<7; i++) {
    ProgramPciLpssI2C (i);
  }
  DEBUG ((EFI_D_INFO, "Pei ProgramPciLpssAllI2C() Done\n"));
  return EFI_SUCCESS;
}
/**
@todo	add description

**/
EFI_STATUS
ProgramPciLpssI2C (
  IN  UINT8 BusNo
  )
{
  UINT32       PmcBase;
  UINT32       DevID;
  UINTN        PciMmBase;
  
    UINT32 PMC_DIS[]= {
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC1,
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC2,
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC3,
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC4,
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC5,
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC6,
    B_PCH_PMC_FUNC_DIS_LPSS2_FUNC7
  };

  DEBUG ((EFI_D_INFO, "Pei ProgramPciLpssI2C() Start\n"));

  PciMmBase = 0;
  //
  // Set the CHV Function Disable Register to ZERO
  //
  PmcBase         = I2CLibPeiMmioRead32 (PciD31F0RegBase + R_PCH_LPC_PMC_BASE) & B_PCH_LPC_PMC_BASE_BAR;
  if (I2CLibPeiMmioRead32(PmcBase+R_PCH_PMC_FUNC_DIS)&PMC_DIS[BusNo]) {
    DEBUG ((EFI_D_INFO, "ProgramPciLpssI2C() enable:I2C[%x]\n",BusNo));
    I2CLibPeiMmioWrite32(
      PmcBase+R_PCH_PMC_FUNC_DIS,
      I2CLibPeiMmioRead32(PmcBase+R_PCH_PMC_FUNC_DIS)&(~PMC_DIS[BusNo])
      );
  }
    

  PciMmBase = MmPciAddress (
               mLpssPciDeviceList[BusNo+1].Segment,
               mLpssPciDeviceList[BusNo+1].BusNum,
               mLpssPciDeviceList[BusNo+1].DeviceNum,
               mLpssPciDeviceList[BusNo+1].FunctionNum,
               0
               );
   DEBUG((EFI_D_ERROR, "Program Pci Lpss I2C Device  %x %x %x PciMmBase:%x\n", \
        mLpssPciDeviceList[BusNo+1].BusNum, \
        mLpssPciDeviceList[BusNo+1].DeviceNum, \
        mLpssPciDeviceList[BusNo+1].FunctionNum, PciMmBase));

    //
    // Check if device present
    //
    
   DevID =  I2CLibPeiMmioRead32(PciMmBase);
    
   if (DevID != 0xFFFFFFFF) {
      if (!(I2CLibPeiMmioRead32 (PciMmBase+R_PCH_LPSS_I2C_STSCMD)& B_PCH_LPSS_I2C_STSCMD_MSE)) {
        //
        // Program BAR 0
        //
        I2CLibPeiMmioWrite32 ((UINTN) (PciMmBase + R_PCH_LPSS_I2C_BAR), (UINT32) (mLpssPciDeviceList[BusNo+1].Bar0 & B_PCH_LPSS_I2C_BAR_BA));
        //
        // Program BAR 1
        //
        I2CLibPeiMmioWrite32 ((UINTN) (PciMmBase + R_PCH_LPSS_I2C_BAR1), (UINT32) (mLpssPciDeviceList[BusNo+1].Bar1 & B_PCH_LPSS_I2C_BAR1_BA));
        //
        // Bus Master Enable & Memory Space Enable
        //
        I2CLibPeiMmioWrite32 ((UINTN) (PciMmBase + R_PCH_LPSS_I2C_STSCMD), (UINT32) (B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE));
        //
        // Release Resets
        //
        I2CLibPeiMmioWrite32 ((UINTN) (mLpssPciDeviceList[BusNo+1].Bar0 + R_PCH_LPIO_I2C_MEM_RESETS), (UINT32) (B_PCH_LPIO_I2C_MEM_RESETS_FUNC | B_PCH_LPIO_I2C_MEM_RESETS_APB));
      }
    }
  
  DEBUG ((EFI_D_INFO, "Pei ProgramPciLpssI2C() End\n"));

  return EFI_SUCCESS;
}
/**
@todo	add description

**/
EFI_STATUS
I2cDisable (
  IN UINT8 BusNo
  )
{
  UINTN   mI2CBaseAddress;
  UINT32 NumTries = 10000;  /* 0.1 seconds */
  mI2CBaseAddress = mLpssPciDeviceList[BusNo+1].Bar0;
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_ENABLE, 0 );
  while ( 0 != ( I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_ENABLE_STATUS ) & 1 )) {
    I2CMicroSecondDelay ( 10 );
    NumTries --;
    if (0 == NumTries) {
      return EFI_NOT_READY;
    }
  }
  return EFI_SUCCESS;
}
/**
@todo	add description

**/
EFI_STATUS
I2cEnable (
  IN UINT8 BusNo
  )
{
  UINTN   mI2CBaseAddress;
  UINT32 NumTries = 10000;  /* 0.1 seconds */
  mI2CBaseAddress = mLpssPciDeviceList[BusNo+1].Bar0;
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_ENABLE, 1 );
  while ( 0 == ( I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_ENABLE_STATUS ) & 1 )) {
    I2CMicroSecondDelay ( 10 );
    NumTries --;
    if (0 == NumTries) {
      return EFI_NOT_READY;
    }
  }
  return EFI_SUCCESS;
}

/**
  Set the I2C controller bus clock frequency.

  @param[in] This           Address of the library's I2C context structure
  @param[in] PlatformData   Address of the platform configuration data
  @param[in] BusClockHertz  New I2C bus clock frequency in Hertz

  @retval RETURN_SUCCESS      The bus frequency was set successfully.
  @retval RETURN_UNSUPPORTED  The controller does not support this frequency.

**/
EFI_STATUS
I2cBusFrequencySet (
  IN UINTN   mI2CBaseAddress,
  IN UINTN   BusClockHertz,
  IN UINT16  *I2cMode
  )
{
  DEBUG ((EFI_D_INFO,"InputFreq BusClockHertz: %d\r\n",BusClockHertz));

  *I2cMode = B_IC_RESTART_EN | B_IC_SLAVE_DISABLE | B_MASTER_MODE;

  //
  //  Set the 100 KHz clock divider
  //
  //  From Table 10 of the I2C specification
  //
  //    High: 4.00 uS
  //    Low:  4.70 uS
  //
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_SS_SCL_HCNT, (UINT16)0x28F );
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_SS_SCL_LCNT, (UINT16)0x2BA );
  //
  //  Set the 400 KHz clock divider
  //
  //  From Table 10 of the I2C specification
  //
  //    High: 0.60 uS
  //    Low:  1.30 uS
  //
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_FS_SCL_HCNT, (UINT16)0x71 );
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_FS_SCL_LCNT, (UINT16)0xCE );

  switch ( BusClockHertz ) {
    case 100 * 1000:
      I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_SDA_HOLD, (UINT16)0x06);//100K
      I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_SDA_SETUP, (UINT16)0x06);
      *I2cMode |= V_SPEED_STANDARD;
      break;
    case 400 * 1000:
      I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_SDA_HOLD, (UINT16)0x06);//400K
      I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_SDA_SETUP, (UINT16)0x06);
      *I2cMode |= V_SPEED_FAST;
      break;
    default:
      I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_SDA_HOLD, (UINT16)0x06);//1.7M
      I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_SDA_SETUP, (UINT16)0x06);
      *I2cMode |= V_SPEED_HIGH;
  }

  return EFI_SUCCESS;
}
/**
@todo	add description

**/
EFI_STATUS
I2CInit (
  UINT8 BusNo,
  UINT16 SlaveAddress
  )
{
  EFI_STATUS Status;
  UINT32     NumTries;
  UINTN      mI2CBaseAddress;
  UINT16     I2cMode;
  UINTN      PciMmBase;

  NumTries  = 0;
  PciMmBase = 0;
  PciMmBase = MmPciAddress (
                0,
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                (BusNo + 1),
                0
                );
  if (I2CLibPeiMmioRead32 (PciMmBase) == 0xFFFFFFFF) {
    DEBUG ((EFI_D_INFO,"I2CInit() Exit with Status EFI_NOT_READY\r\n"));
    return EFI_NOT_READY;
  }
  
    mI2CBaseAddress = I2CLibPeiMmioRead32 (PciMmBase+R_PCH_LPSS_I2C_BAR) & B_PCH_LPSS_I2C_BAR_BA;
  //
  //  Verify the parameters
  //
  if ( 1023 < SlaveAddress ) {
    Status =  EFI_INVALID_PARAMETER;
    DEBUG ((EFI_D_INFO,"I2cStartRequest Exit with Status %r\r\n", Status));
    return Status;
  }
    I2CLibPeiMmioWrite32 ( mI2CBaseAddress + R_IC_TAR, (UINT16)SlaveAddress );

  //
  // Program I2C base address if needed
  //
  if (mI2CBaseAddress == mLpssPciDeviceList[BusNo+1].Bar0) {
    DEBUG((EFI_D_ERROR, "I2CInit() This device is already configured so ignore and return \n"));
    return EFI_SUCCESS;
  }
  ProgramPciLpssI2C(BusNo);
  
  mI2CBaseAddress = mLpssPciDeviceList[BusNo+1].Bar0;
  DEBUG ((EFI_D_ERROR, "mI2CBaseAddress = 0x%x \n",mI2CBaseAddress));
  NumTries = 10000; /* 1 seconds */
  while (( 1 == ( I2CLibPeiMmioRead32 ( mI2CBaseAddress + R_IC_STATUS) & STAT_MST_ACTIVITY ))) {
    I2CMicroSecondDelay (10);
    NumTries --;
    if (0 == NumTries) {
      return EFI_DEVICE_ERROR;
    }
  }

  Status = I2cDisable ( BusNo);
  DEBUG ((EFI_D_INFO, "I2cDisable Status = %r\r\n", Status));
  //
  // Set I2cMode
  //
  I2cBusFrequencySet (mI2CBaseAddress, 400 * 1000, &I2cMode);

  I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_INTR_MASK, 0x0);
  if ( 0x7F < SlaveAddress ) {
    SlaveAddress = ( SlaveAddress & 0x3ff ) | IC_TAR_10BITADDR_MASTER;
  }
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_TAR, (UINT16) SlaveAddress );
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_RX_TL, 0);
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_TX_TL, 0 );
  I2CLibPeiMmioWrite16 ( mI2CBaseAddress + R_IC_CON, I2cMode);

  Status = I2cEnable (BusNo);
  I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_CLR_TX_ABRT );
  return EFI_SUCCESS;
}
/**
  Reads a Byte from I2C Device

  @param[in]  BusNo             I2C Bus no to which the I2C device has been connected
  @param[in]  SlaveAddress      Device Address from which the byte value has to be read
  @param[in]  Offset            Offset from which the data has to be read
  @param[in]  *Byte             Address to which the value read has to be stored
  @param[out]	Readbuffer	@todo add description
  @param[in]	ReadBytes	@todo	add description

  @retval	EFI_SUCCESS       IF the byte value has been successfully read
  @retval	EFI_DEVICE_ERROR  Operation Failed, Device Error

**/
EFI_STATUS
ByteReadI2C_Basic (
  IN  UINT8 BusNo,
  IN  UINT8 SlaveAddress,
  IN  UINTN ReadBytes,
  OUT UINT8 *ReadBuffer,
  IN  UINT8 Start,
  IN  UINT8 End
)

{

  EFI_STATUS  Status;
  UINT32      I2cStatus;
  UINT16      ReceiveData;
  UINT8       *ReceiveDataEnd;
  UINT8       *ReceiveRequest;
  UINT16      raw_intr_stat;
  UINTN       mI2CBaseAddress;
  UINT32      Count;
  mI2CBaseAddress = (UINT32)  (0xFE910000+ BusNo *0x10000);

  Status = EFI_SUCCESS;
  Count  = 0;

  Status = I2CInit (BusNo, SlaveAddress);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  ReceiveDataEnd = &ReadBuffer [ ReadBytes ];
  if (ReadBytes) {
    ReceiveRequest = ReadBuffer;

    while ( (ReceiveDataEnd > ReceiveRequest) ||
              (ReceiveDataEnd > ReadBuffer)) {
      //
      // Check for NACK
      //
      raw_intr_stat = I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_RAW_INTR_STAT );
      if ( 0 != ( raw_intr_stat & I2C_INTR_TX_ABRT )) {
        I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_CLR_TX_ABRT );
        Status = RETURN_DEVICE_ERROR;
        break;
      }
      //
      //  Determine if another byte was received
      //
      I2cStatus = I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_STATUS );
      if ( 0 != ( I2cStatus & STAT_RFNE )) {
        ReceiveData = I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_DATA_CMD );
        *ReadBuffer++ = (UINT8)ReceiveData;
      }

      if (ReceiveDataEnd==ReceiveRequest) {
        I2CMicroSecondDelay ( FIFO_WRITE_DELAY );
        Count++;
        if (Count < 1024) {
          continue;
        } else {
          //
          // Waiting the last request to get data and make (ReceiveDataEnd > ReadBuffer) = TRUE.
          //
          break;
        }
      }
      //
      // Wait until a read request will fit
      //
      if ( 0 == ( I2cStatus & STAT_TFNF )) {
        I2CMicroSecondDelay ( 10 );
        continue;
      }
      //
      // Issue the next read request
      //
      if (End && Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, B_READ_CMD|B_CMD_RESTART|B_CMD_STOP);
      else if (!End && Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, B_READ_CMD|B_CMD_RESTART);
      else if (End && !Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, B_READ_CMD|B_CMD_STOP);
      else if (!End && !Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, B_READ_CMD);
      //
      // Wait after send cmd
      //
      I2CMicroSecondDelay (FIFO_WRITE_DELAY);
      ReceiveRequest += 1;
      }
    //local_irq_restore(eflags);
  }
  return Status;

}

/**
  Writes a Byte to I2C Device

@param[in]  BusNo             I2C Bus no to which the I2C device has been connected
@param[in]  SlaveAddress      Device Address from which the byte value has to be read
@param[in]	WriteBytes		@todo	add description  
@param[in]	WriteBuffer		@todo	add description
@param[in]	Start			@todo	add description
@param[in]  End	            @todo	add description

@retval  EFI_SUCCESS       IF the byte value written successfully
@retval  EFI_DEVICE_ERROR  Operation Failed, Device Error

**/
EFI_STATUS
ByteWriteI2C_Basic (
  IN  UINT8 BusNo,
  IN  UINT8 SlaveAddress,
  IN  UINTN WriteBytes,
  IN  UINT8 *WriteBuffer,
  IN  UINT8 Start,
  IN  UINT8 End
)
{
  EFI_STATUS  Status;
  UINT32      I2cStatus;
  UINT8       *TransmitEnd;
  UINT16      raw_intr_stat;
  UINTN       mI2CBaseAddress;
  UINT32     Count;
  mI2CBaseAddress = (UINT32)  0xFE910000 + BusNo * 0x10000;

  Status = EFI_SUCCESS;
  Count  = 0;
  //
  // Initialise I2C Device
  //
  Status = I2CInit(BusNo, SlaveAddress);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  TransmitEnd = &WriteBuffer [ WriteBytes ];
  if ( WriteBytes ) {
    while ( TransmitEnd > WriteBuffer ) {
      I2cStatus = I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_STATUS );
      raw_intr_stat = I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_RAW_INTR_STAT );
      if ( 0 != ( raw_intr_stat & I2C_INTR_TX_ABRT )) {
        I2CLibPeiMmioRead16 ( mI2CBaseAddress + R_IC_CLR_TX_ABRT );
        Status = RETURN_DEVICE_ERROR;
        DEBUG ((EFI_D_ERROR,"TX ABRT TransmitEnd:0x%x WriteBuffer:0x%x\r\n", TransmitEnd, WriteBuffer));
        break;
      }
      //
      // If tx not full , will send cmd  or continue to wait
      //
      if ( 0 == ( I2cStatus & STAT_TFNF )) {
        I2CMicroSecondDelay (FIFO_WRITE_DELAY);
        continue;
      }
      if (End && Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, (*WriteBuffer++)|B_CMD_RESTART|B_CMD_STOP);
      else if (!End && Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, (*WriteBuffer++)|B_CMD_RESTART);
      else if (End && !Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, (*WriteBuffer++)|B_CMD_STOP);
      else if (!End && !Start )
        I2CLibPeiMmioWrite16 (mI2CBaseAddress + R_IC_DATA_CMD, (*WriteBuffer++));
      //
      // Add a small delay to work around some odd behavior being seen.
      // Without this delay bytes get dropped.
      //
      I2CMicroSecondDelay (FIFO_WRITE_DELAY);
      //
      // Time out
      //
      while (1) {
        raw_intr_stat = I2CLibPeiMmioRead16 ((UINTN) (mI2CBaseAddress + R_IC_RAW_INTR_STAT));
        if (0 != (raw_intr_stat & I2C_INTR_TX_ABRT)) {
          I2CLibPeiMmioRead16 ((UINTN) (mI2CBaseAddress + R_IC_CLR_TX_ABRT));
          Status = RETURN_DEVICE_ERROR;
          DEBUG((EFI_D_ERROR,"TX ABRT TransmitEnd:0x%x WriteBuffer:0x%x\r\n", TransmitEnd, WriteBuffer));
        }
        if (0 == I2CLibPeiMmioRead16 ((UINTN) (mI2CBaseAddress + R_IC_TXFLR))) {
          break;
        }
        I2CMicroSecondDelay ( FIFO_WRITE_DELAY );
        Count++;
        if (Count < 1024) {
          //
          // Waiting the last request to get data and make (ReceiveDataEnd > ReadBuffer) = TRUE.
          //
          continue;
        } else {
          break;
        }
      } // while(1)
    }
  }
  if (EFI_ERROR(Status))
  DEBUG((EFI_D_INFO,"I2cStartRequest Exit with Status %r\r\n",Status));
  return Status;
}
/**
@todo	add description

**/
EFI_STATUS
ByteReadI2C (
  IN  UINT8 BusNo,
  IN  UINT8 SlaveAddress,
  IN  UINT8 Offset,
  IN  UINTN ReadBytes,
  OUT UINT8 *ReadBuffer
  )
{
  EFI_STATUS          Status;

  Status = ByteWriteI2C_Basic (BusNo, SlaveAddress, 1, &Offset, TRUE, FALSE);
  if (!EFI_ERROR(Status)) {
    Status = ByteReadI2C_Basic(BusNo, SlaveAddress,ReadBytes,ReadBuffer,TRUE,TRUE);
  }
  return Status;
}
/**
@todo	add description

**/
EFI_STATUS
ByteWriteI2C (
  IN  UINT8 BusNo,
  IN  UINT8 SlaveAddress,
  IN  UINT8 Offset,
  IN  UINTN WriteBytes,
  IN  UINT8 *WriteBuffer
  )
{
  EFI_STATUS          Status;

  Status = ByteWriteI2C_Basic (BusNo, SlaveAddress, 1, &Offset, TRUE, FALSE);
  if (!EFI_ERROR(Status)) {
    Status = ByteWriteI2C_Basic(BusNo, SlaveAddress,WriteBytes,WriteBuffer,FALSE,TRUE);
  }
  return Status;
}
/**
  This function initiates an I2C operation on the controller.

  The operation is performed by selecting the I2C device with its slave
  address and then sending all write data to the I2C device.  If read data
  is requested, a restart is sent followed by the slave address and then
  the read data is clocked into the I2C controller and placed in the read
  buffer.  When the operation completes, the status value is returned and
  then the event is set.

  @param[in]  BusNo         I2C Bus no to which the I2C device has been connected
  @param[in]  SlaveAddress  Device Address from which the byte value has to be read
  @param[in]  WriteBytes    Number of bytes to send
  @param[in]  WriteBuffer   Address of buffer containing data to send
  @param[in]  ReadBytes     Number of bytes to read
  @param[out] ReadBuffer    Address of buffer to receive data

  @retval RETURN_SUCCESS            The operation completed successfully.
  @retval RETURN_DEVICE_ERROR       There was an I2C error (NACK) during the operation.
                                    This could indicate the slave device is not present

**/
EFI_STATUS
I2cRequest (
  IN  UINT8 BusNo,
  IN  UINT16 SlaveAddress,
  IN  UINTN WriteBytes,
  IN  UINT8 *WriteBuffer,
  IN  UINTN ReadBytes,
  OUT UINT8 *ReadBuffer
  )
{
  UINT32        I2cStatus;
  UINT16        ReceiveData;
  UINT8         *ReceiveDataEnd;
  UINT8         *ReceiveRequest;
  RETURN_STATUS Status;
  UINT8         *TransmitEnd;
  UINT16        RawInitStart;
  UINTN         BaseAddr;

  DEBUG((EFI_D_INFO, "I2cRequest Entered\r\n"));

  ReceiveRequest  = NULL;
  RawInitStart    = 0;
  BaseAddr        = (UINT32)(0xFE910000+ BusNo *0x10000);
  ReceiveDataEnd  = &ReadBuffer[ReadBytes];
  TransmitEnd     = &WriteBuffer[WriteBytes];

  Status = I2CInit (BusNo, SlaveAddress);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Status = EFI_SUCCESS;
  if ( WriteBytes ) {
    while ( TransmitEnd > WriteBuffer ) {
      I2cStatus = I2CLibPeiMmioRead16 ( BaseAddr + R_IC_STATUS );
      RawInitStart = I2CLibPeiMmioRead16 ( BaseAddr + R_IC_RAW_INTR_STAT );
      if ( 0 != ( RawInitStart & I2C_INTR_TX_ABRT )) {
        I2CLibPeiMmioRead16 ( BaseAddr + R_IC_CLR_TX_ABRT );
        Status = RETURN_DEVICE_ERROR;
        DEBUG((EFI_D_INFO, "TX ABRT TransmitEnd:0x%016Lx WriteBuffer:0x%016Lx\r\n", (UINT64)(UINTN)TransmitEnd, (UINT64)(UINTN)WriteBuffer));
        break;
      }      
      if ( 0 == ( I2cStatus & STAT_TFNF )) {
        continue;
      }
      if ( TransmitEnd == (WriteBuffer + 1) ) {
        //
        //  For WRITE only transaction, write B_CMD_STOP to issue bus stop signal
        //  For WRITE-READ sequence, READ loop will issue bus stop signal
        //
        if ( ReadBytes == 0 ) {
          I2CLibPeiMmioWrite16 ( BaseAddr + R_IC_DATA_CMD, *WriteBuffer | B_CMD_STOP );
        } else {
          I2CLibPeiMmioWrite16 ( BaseAddr + R_IC_DATA_CMD, *WriteBuffer );
        }
        WriteBuffer ++;
      } else {
        I2CLibPeiMmioWrite16 ( BaseAddr + R_IC_DATA_CMD, *WriteBuffer++ );
      }
      //
      //  Add a small delay to work around some odd behavior being seen.  Without
      //  this delay bytes get dropped.
      //
      I2CMicroSecondDelay ( FIFO_WRITE_DELAY );
    }

    if ( EFI_ERROR(Status) ) {
      goto ExirI2cRequest;
    }
    //
    //  If this transaction is WRITE only, rather than WRITE-READ sequence,wait for bytes to go
    //  Write into Tx fifo doesn't mean the dat will go correctly on the SDA data line
    //
    if ( ReadBytes == 0) {
      while( 1 ) {
        RawInitStart = I2CLibPeiMmioRead16 ( BaseAddr + R_IC_RAW_INTR_STAT );
        if ( 0 != ( RawInitStart & I2C_INTR_TX_ABRT )) {
            I2CLibPeiMmioRead16 ( BaseAddr + R_IC_CLR_TX_ABRT );
            Status = RETURN_DEVICE_ERROR;
            DEBUG((EFI_D_INFO, "TX ABRT TransmitEnd:0x%x WriteBuffer:0x%x\r\n", TransmitEnd, WriteBuffer));
            goto ExirI2cRequest;
        }
        if ( 0 == I2CLibPeiMmioRead16(BaseAddr + R_IC_TXFLR ) ) {
          break;
        }
      }
    }
  }

  if(EFI_ERROR(Status)) {
    goto ExirI2cRequest;
  }

  if (ReadBytes) {
    ReceiveRequest = ReadBuffer;
    while ( (ReceiveDataEnd > ReceiveRequest) ||
            (ReceiveDataEnd > ReadBuffer)) {
      RawInitStart = I2CLibPeiMmioRead16 ( BaseAddr + R_IC_RAW_INTR_STAT );
      if ( 0 != ( RawInitStart & I2C_INTR_TX_ABRT )) {
        I2CLibPeiMmioRead16 ( BaseAddr + R_IC_CLR_TX_ABRT );
        Status = RETURN_DEVICE_ERROR;
        DEBUG((EFI_D_INFO, "TX ABRT ,%d bytes hasn't been transferred\r\n", ReceiveDataEnd - ReceiveRequest));
        break;
      }
      //
      //  Determine if another byte was received
      //
      I2cStatus = I2CLibPeiMmioRead16 ( BaseAddr + R_IC_STATUS );
      if ( 0 != ( I2cStatus & STAT_RFNE )) {
        ReceiveData = I2CLibPeiMmioRead16 ( BaseAddr + R_IC_DATA_CMD );
        *ReadBuffer++ = (UINT8)ReceiveData;
      }

      if (ReceiveDataEnd == ReceiveRequest) {
        continue;
      }
      //
      //  Wait until a read request will fit
      //
      if ( 0 == ( I2cStatus & STAT_TFNF )) {
        I2CMicroSecondDelay ( 10 );
        continue;
      }
      //
      //  Issue the next read request
      //
      if (ReceiveDataEnd == ( ReceiveRequest + 1 ) ) {
        I2CLibPeiMmioWrite16 ( BaseAddr + R_IC_DATA_CMD, B_READ_CMD | B_CMD_STOP );
      } else {
        I2CLibPeiMmioWrite16 ( BaseAddr + R_IC_DATA_CMD, B_READ_CMD );
      }
      ReceiveRequest += 1;
    }
  }

ExirI2cRequest:
  DEBUG((EFI_D_INFO, "I2cRequest Exit with Status %r\r\n",Status));
  return Status;
}