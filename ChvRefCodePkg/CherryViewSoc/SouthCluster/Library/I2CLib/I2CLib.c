/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <PchRegs.h>
#include <PlatformBaseAddresses.h>
#include <Library/I2CLib.h>
#include <Protocol/GlobalNvsArea.h>
#include <Library/UefiBootServicesTableLib.h>
#include "ChvAccess.h"

#define GLOBAL_NVS_OFFSET(Field)    (UINTN)((CHAR8*)&((EFI_GLOBAL_NVS_AREA*)0)->Field - (CHAR8*)0)

#define PciD31F0RegBase             PCIEX_BASE_ADDRESS + (UINT32) (31 << 15)

typedef struct _LPSS_PCI_DEVICE_INFO {
  UINTN        Segment;
  UINTN        BusNum;
  UINTN        DeviceNum;
  UINTN        FunctionNum;
  UINTN        Bar0;
  UINTN        Bar1;
} LPSS_PCI_DEVICE_INFO;

//
// Initial temporary base addresses to use before PCI enumeration.
//
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
//
// LPIO I2C Module
// Memory Space Registers
//
#define R_PCH_LPIO_I2C_MEM_RESETS                 0x804 // Software Reset
#define B_PCH_LPIO_I2C_MEM_RESETS_FUNC            BIT1  // Function Clock Domain Reset
#define B_PCH_LPIO_I2C_MEM_RESETS_APB             BIT0  // APB Domain Reset
#define R_PCH_LPSS_I2C_MEM_PCP                    0x800 // Private Clock Parameters

#define I2C_DEBUG_INFO_ON  1

#if I2C_DEBUG_INFO_ON
#define I2C_DEBUG_INFO(arg)   DEBUG(arg)
#else
#define I2C_DEBUG_INFO(arg)
#endif

STATIC UINTN mI2CBaseAddress = 0;

UINT16      I2cMode = (B_IC_RESTART_EN | B_IC_SLAVE_DISABLE | B_MASTER_MODE);

UINTN i2cNvsBaseAddress[] = {
  GLOBAL_NVS_OFFSET(LDMA2Addr),
  GLOBAL_NVS_OFFSET(I2C1Addr),
  GLOBAL_NVS_OFFSET(I2C2Addr),
  GLOBAL_NVS_OFFSET(I2C3Addr),
  GLOBAL_NVS_OFFSET(I2C4Addr),
  GLOBAL_NVS_OFFSET(I2C5Addr),
  GLOBAL_NVS_OFFSET(I2C6Addr),
  GLOBAL_NVS_OFFSET(I2C7Addr)
};

/**
  get I2Cx controller base address (bar0)
  
  @todo	add parameter and return value description

**/
UINTN
GetI2cBarAddr (
  IN    UINT8 BusNo
  )
{
  EFI_STATUS           Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
  UINTN  AcpiBaseAddr;
  UINTN  PciMmBase=0;

  if (gBS == NULL) {
    ASSERT(gBS != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &GlobalNvsArea
                  );
  //
  //PCI mode from PEI ( Global NVS is not ready)
  //
  if (EFI_ERROR(Status)) {
    I2C_DEBUG_INFO ((EFI_D_INFO, "GetI2cBarAddr() gEfiGlobalNvsAreaProtocolGuid:%r\n", Status));
    return 0;
  }

  AcpiBaseAddr =  *(UINTN*)((CHAR8*)GlobalNvsArea->Area + i2cNvsBaseAddress[BusNo+1]);
  //
  //PCI mode from DXE (global NVS protocal) to LPSS OnReadytoBoot(swith to ACPI)
  //
  if ( AcpiBaseAddr == 0) {
    PciMmBase = MmPciAddress (
                  mLpssPciDeviceList[BusNo+1].Segment,
                  mLpssPciDeviceList[BusNo+1].BusNum,
                  mLpssPciDeviceList[BusNo+1].DeviceNum,
                  mLpssPciDeviceList[BusNo+1].FunctionNum,
                  0
                  );
    I2C_DEBUG_INFO((EFI_D_INFO, "\nGetI2cBarAddr() I2C Device %x %x %x PciMmBase:%x\n", \
           mLpssPciDeviceList[BusNo+1].BusNum, \
           mLpssPciDeviceList[BusNo+1].DeviceNum, \
           mLpssPciDeviceList[BusNo+1].FunctionNum, PciMmBase));

    if (MmioRead32 (PciMmBase) != 0xFFFFFFFF) {
      if ((MmioRead32 (PciMmBase+R_PCH_LPSS_I2C_STSCMD)& B_PCH_LPSS_I2C_STSCMD_MSE)) {
        mLpssPciDeviceList[BusNo+1].Bar0 = MmioRead32 (PciMmBase+R_PCH_LPSS_I2C_BAR);     //get the address allocted.
        mLpssPciDeviceList[BusNo+1].Bar1 = MmioRead32 (PciMmBase+R_PCH_LPSS_I2C_BAR1);
        I2C_DEBUG_INFO((EFI_D_INFO, "GetI2cBarAddr() bar0:0x%x bar1:0x%x\n",mLpssPciDeviceList[BusNo+1].Bar0, mLpssPciDeviceList[BusNo+1].Bar1));
      }
    }
    AcpiBaseAddr =mLpssPciDeviceList[BusNo+1].Bar0;
  }
  //
  //ACPI mode from BDS: LPSS OnReadytoBoot
  //
  else {
    I2C_DEBUG_INFO ((EFI_D_INFO, "GetI2cBarAddr() NVS Varialable is updated by this LIB or LPSS  \n"));
  }
  I2C_DEBUG_INFO ((EFI_D_INFO, "GetI2cBarAddr() BusNo+1 0x%x AcpiBaseAddr:0x%x \n", BusNo+1, AcpiBaseAddr));
  return AcpiBaseAddr;
}
/**
@todo	add description

**/
EFI_STATUS
ProgramPciLpssI2C (
  IN  UINT8 BusNo
  )
{
  UINT32 PmcBase;
  UINTN  PciMmBase=0;
  EFI_STATUS           Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;

  UINT32 PMC_DIS[]= {
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC1,
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC2,
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC3,
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC4,
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC5,
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC6,
      B_PCH_PMC_FUNC_DIS_LPSS2_FUNC7
    };

  I2C_DEBUG_INFO ((EFI_D_INFO, "ProgramPciLpssI2C() Start\n"));

  //
  // Set the CHT Function Disable Register to ZERO
  //
  PmcBase = MmioRead32 (PciD31F0RegBase + R_PCH_LPC_PMC_BASE) & B_PCH_LPC_PMC_BASE_BAR;
  if (MmioRead32(PmcBase+R_PCH_PMC_FUNC_DIS)&PMC_DIS[BusNo]) {
    DEBUG ((EFI_D_ERROR, "ProgramPciLpssI2C() End:I2C[%x] is disabled\n",BusNo));
    return EFI_NOT_READY;
  }
  I2C_DEBUG_INFO ((EFI_D_INFO, "ProgramPciLpssI2C()------------BusNo=%x,PMC=%x\n",BusNo,MmioRead32(PmcBase+R_PCH_PMC_FUNC_DIS)));

  //for(Index = 0; Index < LPSS_PCI_DEVICE_NUMBER; Index ++)
  {

    PciMmBase = MmPciAddress (
                  mLpssPciDeviceList[BusNo+1].Segment,
                  mLpssPciDeviceList[BusNo+1].BusNum,
                  mLpssPciDeviceList[BusNo+1].DeviceNum,
                  mLpssPciDeviceList[BusNo+1].FunctionNum,
                  0
                  );
    I2C_DEBUG_INFO((EFI_D_INFO, "Program Pci Lpss I2C Device  %x %x %x PciMmBase:%x\n", \
           mLpssPciDeviceList[BusNo+1].BusNum, \
           mLpssPciDeviceList[BusNo+1].DeviceNum, \
           mLpssPciDeviceList[BusNo+1].FunctionNum, PciMmBase));

    //
    // Check if device present
    //
    if (MmioRead32 (PciMmBase) != 0xFFFFFFFF) {
      if ((MmioRead32 (PciMmBase+R_PCH_LPSS_I2C_STSCMD)& B_PCH_LPSS_I2C_STSCMD_MSE)) {
        mLpssPciDeviceList[BusNo+1].Bar0 = MmioRead32 (PciMmBase+R_PCH_LPSS_I2C_BAR);     //get the address allocted.
        mLpssPciDeviceList[BusNo+1].Bar1 = MmioRead32 (PciMmBase+R_PCH_LPSS_I2C_BAR1);
        I2C_DEBUG_INFO((EFI_D_INFO, "ProgramPciLpssI2C() bar0:0x%x bar1:0x%x\n",mLpssPciDeviceList[BusNo+1].Bar0, mLpssPciDeviceList[BusNo+1].Bar1));
      } else {
        //
        // Program BAR 0
        //
        ASSERT (((mLpssPciDeviceList[BusNo+1].Bar0 & B_PCH_LPSS_I2C_BAR_BA) == mLpssPciDeviceList[BusNo+1].Bar0) && (mLpssPciDeviceList[BusNo+1].Bar0 != 0));
        MmioWrite32 ((UINTN) (PciMmBase + R_PCH_LPSS_I2C_BAR), (UINT32) (mLpssPciDeviceList[BusNo+1].Bar0 & B_PCH_LPSS_I2C_BAR_BA));
        //
        // Program BAR 1
        //
        ASSERT (((mLpssPciDeviceList[BusNo+1].Bar1 & B_PCH_LPSS_I2C_BAR1_BA) == mLpssPciDeviceList[BusNo+1].Bar1) && (mLpssPciDeviceList[BusNo+1].Bar1 != 0));
        MmioWrite32 ((UINTN) (PciMmBase + R_PCH_LPSS_I2C_BAR1), (UINT32) (mLpssPciDeviceList[BusNo+1].Bar1 & B_PCH_LPSS_I2C_BAR1_BA));
        //
        // Bus Master Enable & Memory Space Enable
        //

        MmioOr32 ((UINTN) (PciMmBase + R_PCH_LPSS_I2C_STSCMD), (UINT32) (B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE));
        ASSERT (MmioRead32 (mLpssPciDeviceList[BusNo+1].Bar0) != 0xFFFFFFFF);
      }
      //
      // Release Resets
      //
      MmioWrite32 (mLpssPciDeviceList[BusNo+1].Bar0 + R_PCH_LPIO_I2C_MEM_RESETS,(B_PCH_LPIO_I2C_MEM_RESETS_FUNC | B_PCH_LPIO_I2C_MEM_RESETS_APB));

      I2C_DEBUG_INFO ((EFI_D_INFO, "ProgramPciLpssI2C() Programmed()\n"));
    }
    //
    //BDS: already switched to ACPI mode
    //
    else {
      if (gBS == NULL) {
        ASSERT(gBS != NULL);
        return EFI_OUT_OF_RESOURCES;
      }
      Status = gBS->LocateProtocol (
                      &gEfiGlobalNvsAreaProtocolGuid,
                      NULL,
                      (VOID **) &GlobalNvsArea
                      );
      if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "GetI2cBarAddr() gEfiGlobalNvsAreaProtocolGuid:%r\n", Status));
        return 0;
      }
      mLpssPciDeviceList[BusNo+1].Bar0 = *(UINT32*)((CHAR8*)GlobalNvsArea->Area + i2cNvsBaseAddress[BusNo+1]);
      I2C_DEBUG_INFO ((EFI_D_INFO, "ProgramPciLpssI2C(): is switched to ACPI 0x:%x \n",mLpssPciDeviceList[BusNo+1].Bar0));
    }
  }
  I2C_DEBUG_INFO ((EFI_D_INFO, "ProgramPciLpssI2C() End\n"));

  return EFI_SUCCESS;
}
/**
  @todo	add description

**/
RETURN_STATUS
I2cDisable (
  VOID
  )
{
  UINT32 NumTries = 10000;  /* 0.1 seconds */
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_ENABLE), 0);
  while (0 != (MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_ENABLE_STATUS)) & 1)) {
    MicroSecondDelay (10);
    NumTries --;
    if (0 == NumTries) {
      return RETURN_NOT_READY;
    }
  }
  return RETURN_SUCCESS;
}
/**
  @todo	add description

**/
RETURN_STATUS
I2cEnable (
  VOID
  )
{
  UINT32 NumTries = 10000;  /* 0.1 seconds */
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_ENABLE), 1);
  while (0 == (MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_ENABLE_STATUS)) & 1)) {
    MicroSecondDelay (10);
    NumTries --;
    if (0 == NumTries) {
      return RETURN_NOT_READY;
    }
  }
  return RETURN_SUCCESS;
}

/**
  @todo	add description

**/
RETURN_STATUS
I2cBusFrequencySet (
  IN UINTN BusClockHertz
  )
{
  I2C_DEBUG_INFO((EFI_D_INFO,"InputFreq BusClockHertz: %d\r\n",BusClockHertz));
  //
  // Set the 100 KHz clock divider according to SV result and I2C spec
  //
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SS_SCL_HCNT), (UINT16) 0x28F);
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SS_SCL_LCNT), (UINT16) 0x2BA);
  //
  // Set the 400 KHz clock divider according to SV result and I2C spec
  //
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_FS_SCL_HCNT), (UINT16) 0x71);
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_FS_SCL_LCNT), (UINT16) 0xCE);

  switch (BusClockHertz) {
    case 100 * 1000:
      //
      // 100 KHz
      //
      MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SDA_HOLD), (UINT16) 0x06);
      MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SDA_SETUP), (UINT16) 0x06);
      I2cMode = V_SPEED_STANDARD;
      break;
    case 400 * 1000:
      //
      // 400 KHz
      //
      MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SDA_HOLD), (UINT16) 0x06);
      MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SDA_SETUP), (UINT16) 0x06);
      I2cMode = V_SPEED_FAST;
      break;
    default:
      //
      // 1.7 MHz
      //
      MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SDA_HOLD), (UINT16) 0x06);
      MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_SDA_SETUP), (UINT16) 0x06);
      I2cMode = V_SPEED_HIGH;
  }
  //
  // Select the frequency counter
  // Enable restart condition,
  // Enable master FSM, disable slave FSM
  //
  I2cMode |= B_IC_RESTART_EN | B_IC_SLAVE_DISABLE | B_MASTER_MODE;

  return EFI_SUCCESS;
}
/**
  Initializes the host controller to execute I2C commands.

  @todo	add parameter description

  @retval  EFI_SUCCESS             Opcode initialization on the I2C host controller completed.
  @retval  EFI_DEVICE_ERROR        Device error, operation failed.

**/
EFI_STATUS
I2CInit (
  IN  UINT8  BusNo,
  IN  UINT16 SlaveAddress
  )

{
  EFI_STATUS Status;
  UINT32     NumTries;
  UINT32     GnvsI2cBarAddr;

  Status     = RETURN_SUCCESS;
  NumTries   = 0;
  GnvsI2cBarAddr = 0;

  //
  // Verify the parameters
  //
  if (( 1023 < SlaveAddress) || ( LPSS_PCI_DEVICE_NUMBER-2 < BusNo)) {//start from 0 and  have extra DMA device in array.
    Status =  RETURN_INVALID_PARAMETER;
    DEBUG((EFI_D_ERROR,"I2CInit Exit with RETURN_INVALID_PARAMETER\r\n"));
    return Status;
  }
  MmioWrite32 ( mI2CBaseAddress + R_IC_TAR, (UINT16)SlaveAddress );
  //
  //1.PEI: program and init ( before pci enumeration).
  //2.DXE:update address and re-init ( after pci enumeration).
  //3.BDS:update ACPI address and re-init ( after acpi mode is enabled).
  //
  if (mI2CBaseAddress == mLpssPciDeviceList[BusNo+1].Bar0) {
    // I2CInit is already  called
    GnvsI2cBarAddr = (UINT32)GetI2cBarAddr(BusNo);
    if (
      (GnvsI2cBarAddr == 0)||//PEI: Gnvs not ready.
      (GnvsI2cBarAddr == mI2CBaseAddress)//DXE and BDS
    ) {
      I2C_DEBUG_INFO((EFI_D_INFO,"I2CInit Exit with mI2CBaseAddress:%x == [%x].Bar0\r\n",mI2CBaseAddress,BusNo+1));
      return RETURN_SUCCESS;
    }
  }
  Status = ProgramPciLpssI2C (BusNo);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "ProgramPciLpssI2C Status = %r\r\n", Status));
    return Status;
  }

  mI2CBaseAddress = (UINT32) mLpssPciDeviceList[BusNo+1].Bar0;
  I2C_DEBUG_INFO ((EFI_D_INFO, "mI2CBaseAddress = 0x%x \n",mI2CBaseAddress));

  NumTries = 10000; /* 1 seconds */
  while (( STAT_MST_ACTIVITY == ( MmioRead32 ((UINTN)(mI2CBaseAddress + R_IC_STATUS)) & STAT_MST_ACTIVITY ))) {
    MicroSecondDelay(10);
    NumTries --;
    if (0 == NumTries) {
      DEBUG((EFI_D_ERROR, "Try timeout\r\n"));
      return RETURN_DEVICE_ERROR;
    }
  }

  Status = I2cDisable ();
  I2C_DEBUG_INFO((EFI_D_INFO, "I2cDisable Status = %r\r\n", Status));
  //
  // Set I2cMode
  //
  I2cBusFrequencySet (400 * 1000);
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_INTR_MASK), 0x0);
  if (0x7f < SlaveAddress) {
    SlaveAddress = (SlaveAddress & 0x3ff) | IC_TAR_10BITADDR_MASTER;
  }
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_TAR), (UINT16) SlaveAddress);
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_RX_TL), 0);
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_TX_TL), 0);
  MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_CON), I2cMode);
  Status = I2cEnable ();

  I2C_DEBUG_INFO((EFI_D_INFO, "I2cEnable Status = %r\r\n", Status));
  MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_CLR_TX_ABRT));
  return EFI_SUCCESS;
}
/**
  Reads a Byte from I2C Device

  @param[in]  BusNo             I2C Bus no to which the I2C device has been connected
  @param[in]	SlaveAddress      Device Address from which the byte value has to be read
  @param[in]  Offset            Offset from which the data has to be read
  @param[in]  *Byte             Address to which the value read has to be stored
  @param[out]	ReadBytes	  @todo	add description

  @retval  EFI_SUCCESS       IF the byte value has been successfully read
  @retval  EFI_DEVICE_ERROR  Operation Failed, Device Error

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
  EFI_STATUS Status;
  UINT32     I2cStatus;
  UINT16     ReceiveData;
  UINT8      *ReceiveDataEnd;
  UINT8      *ReceiveRequest;
  UINT16     raw_intr_stat;
  UINT32     Count;

  Status = EFI_SUCCESS;
  Count  = 0;

  ReceiveDataEnd = &ReadBuffer [ ReadBytes ];
  if (ReadBytes) {
    ReceiveRequest = ReadBuffer;
    //DEBUG((EFI_D_INFO,"Read: ---------------%d bytes to RX\r\n",ReceiveDataEnd - ReceiveRequest));

    while ((ReceiveDataEnd > ReceiveRequest) ||
            (ReceiveDataEnd > ReadBuffer)) {
      //
      // Check for NACK
      //
      raw_intr_stat = (UINT16) MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_RAW_INTR_STAT));
      if (0 != (raw_intr_stat & I2C_INTR_TX_ABRT)) {
        MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_CLR_TX_ABRT));
        Status = RETURN_DEVICE_ERROR;
        DEBUG((EFI_D_INFO,"TX ABRT ,%d bytes hasn't been transferred\r\n", ReceiveDataEnd - ReceiveRequest));
        break;
      }
      //
      // Determine if another byte was received
      //
      I2cStatus = (UINT16) MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_STATUS));
      if (0 != (I2cStatus & STAT_RFNE)) {
        ReceiveData = (UINT16) MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD));
        *ReadBuffer++ = (UINT8) ReceiveData;
        I2C_DEBUG_INFO((EFI_D_INFO,"MmioRead32 ,1 byte 0x:%x is received\r\n",ReceiveData));

      }

      if (ReceiveDataEnd==ReceiveRequest) {
        MicroSecondDelay ( FIFO_WRITE_DELAY );
        I2C_DEBUG_INFO((EFI_D_INFO,"ReceiveDataEnd==ReceiveRequest------------%x\r\n",I2cStatus & STAT_RFNE));
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
      if (0 == (I2cStatus & STAT_TFNF)) {
        I2C_DEBUG_INFO((EFI_D_INFO,"Wait until a read request will fit\r\n"));
        MicroSecondDelay (10);
        continue;
      }
      //
      //  Issue the next read request
      //
      if (End && Start)
          MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), B_READ_CMD | B_CMD_RESTART | B_CMD_STOP);
      else if (!End && Start)
          MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), B_READ_CMD | B_CMD_RESTART);
      else if (End && !Start)
          MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), B_READ_CMD | B_CMD_STOP);
      else if (!End && !Start)
          MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), B_READ_CMD);
      //
      // Wait after send cmd
      //
      MicroSecondDelay (FIFO_WRITE_DELAY);

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
  @param[in]  Offset            Offset from which the data has to be read
  @param[in]  *Byte             Address to which the byte value has to be written

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
  EFI_STATUS Status;
  UINT32     I2cStatus;
  UINT8      *TransmitEnd;
  UINT16     raw_intr_stat;
  UINT32     Count;

  Status = EFI_SUCCESS;
  Count  = 0;
  //
  // Initialise I2C Device
  //
  Status = I2CInit (BusNo, SlaveAddress);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  TransmitEnd = &WriteBuffer [ WriteBytes ];
  if (WriteBytes) {
    while (TransmitEnd > WriteBuffer) {
      I2cStatus = MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_STATUS));
      raw_intr_stat = (UINT16) MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_RAW_INTR_STAT));
      if (0 != (raw_intr_stat & I2C_INTR_TX_ABRT)) {
        MmioRead32 ((UINTN) (mI2CBaseAddress + R_IC_CLR_TX_ABRT));
        Status = RETURN_DEVICE_ERROR;
        DEBUG ((EFI_D_ERROR,"TX ABRT TransmitEnd:0x%x WriteBuffer:0x%x\r\n", TransmitEnd, WriteBuffer));
        break;
      }
      //
      // If tx not full , will send cmd  or continue to wait
      //
      if (0 == (I2cStatus & STAT_TFNF)) {
        MicroSecondDelay (FIFO_WRITE_DELAY);
        continue;
      }

      if (End && Start)
        MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), (*WriteBuffer++) | B_CMD_RESTART | B_CMD_STOP);
      else if (!End && Start)
        MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), (*WriteBuffer++) | B_CMD_RESTART);
      else if (End && !Start)
        MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), (*WriteBuffer++) | B_CMD_STOP);
      else if (!End && !Start)
        MmioWrite32 ((UINTN) (mI2CBaseAddress + R_IC_DATA_CMD), (*WriteBuffer++));
      //
      // Add a small delay to work around some odd behavior being seen.
      // Without this delay bytes get dropped.
      //
      MicroSecondDelay (FIFO_WRITE_DELAY);
      //
      // Time out
      //
      while (1) {
        raw_intr_stat = MmioRead16 ((UINTN) (mI2CBaseAddress + R_IC_RAW_INTR_STAT));
        if (0 != (raw_intr_stat & I2C_INTR_TX_ABRT)) {
          MmioRead16 ((UINTN) (mI2CBaseAddress + R_IC_CLR_TX_ABRT));
          Status = RETURN_DEVICE_ERROR;
          DEBUG((EFI_D_ERROR,"TX ABRT TransmitEnd:0x%x WriteBuffer:0x%x\r\n", TransmitEnd, WriteBuffer));
        }
        if (0 == MmioRead16 ((UINTN) (mI2CBaseAddress + R_IC_TXFLR))) {
          break;
        }

        MicroSecondDelay (FIFO_WRITE_DELAY);
        Count++;
        //
        // To avoid sys hung without ul-pmc device on RVP
        //
        if (Count < 1024) {
          //
          // Waiting the last request to get data and make (ReceiveDataEnd > ReadBuffer) = TRUE.
          //
          continue;
        } else {
          break;
        }
      } // while(1)
    } // while (TransmitEnd > WriteBuffer)
  } // if (WriteBytes)
  if(EFI_ERROR(Status))
    DEBUG((EFI_D_ERROR,"ByteWriteI2C_Basic BusNo:0x%x, SlaveAddress:0x%x,\
           WriteBytes:0x%x, WriteBuffer:0x%x, Start:0x%x, End:0x%x \
           Exit with Status %r\r\n",BusNo, SlaveAddress, WriteBytes, \
           *WriteBuffer, Start, End, Status));
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
