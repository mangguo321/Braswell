/** @file
  Library for GPIO Pin Programmings.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/GpioLib.h>

/**
  Function to get the Pin Pad0 MMIO offset

  @param[in] Community  NORTH-0x01
                        EAST-0x02
                        SOUTH_EAST-0x03
                        SOUTH_WEST-0x04
  @param[in] PinNum     Pin number.

  @retval    UINT32     Pin Mmio Offset
**/
UINT32
EFIAPI
GetPinMmioOffset (
  IN UINT32 Community,
  IN UINT32 PinNumber
  )
{
  UINT32 CommunityMmioOffset;

  switch (Community) {
    case CFIO_COMMUNITY_NORTH:
      CommunityMmioOffset = GPIO_MMIO_OFFSET_N;
      break;
    case CFIO_COMMUNITY_EAST:
      CommunityMmioOffset = GPIO_MMIO_OFFSET_E;
      break;
    case CFIO_COMMUNITY_SOUTH_EAST:
      CommunityMmioOffset = GPIO_MMIO_OFFSET_SE;
      break;
    case CFIO_COMMUNITY_SOUTH_WEST:
      CommunityMmioOffset = GPIO_MMIO_OFFSET_SW;
      break;
    default:
      return INVALID_COMMUNITY;
  }

  return CONF_MMIO_ADDRESS(CommunityMmioOffset, PinNumber, CHV_GPIO_PAD_CONF0_OFFSET);
}

/**
  Function to Read/Write Pad0 or Pad1 control register of given Pin from community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Padx          This can be either Pad0 or Pad1, 0 - PAD0
  @param[in] ReadOrWrite   0 - Read and 1 - Write of Padx
  @param[in] PadxValue     Pinter to hold the Read/Write value.

**/
VOID
ReadWritePadXConf (
  IN UINT32     Community,
  IN UINT32     PinNum,
  IN BOOLEAN    Padx,
  IN BOOLEAN    ReadOrWrite,
  IN OUT UINT32 *PadxValue
  )
{
  UINT32 Pad0MmioOffset = 0;

  //
  // Calculate the Pad0 MMIO Offset
  //
  Pad0MmioOffset = GetPinMmioOffset(Community, PinNum);

  if (!ReadOrWrite) {
    *PadxValue = MmioRead32(Pad0MmioOffset+ (Padx*4));
  } else {
    MmioWrite32((Pad0MmioOffset + (Padx*4)), *PadxValue);
  }
}

/**
  Function to Read the Pad Write Lock status
  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    Boolean       1-Locked,0-Unlocked
**/
BOOLEAN
EFIAPI
GetPinPadLockStatus (
  IN UINT32 Community,
  IN UINT32 PinNum
  )
{
  UINT32 PadConfigData = 0;
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_READ, &PadConfigData);

  if (PadConfigData & GPIO_PAD_LOCK)
    return LOCKED;
  else
    return UNLOCKED;
}

/**
  Function to lock the pad of specified Pin
  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Enable        1-Lock,0-UnLock

**/
VOID
LockUnlockGPIOPin (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN BOOLEAN Enable
  )
{
  UINT32 PadConfigData = 0;

  //
  // Read the Pad1 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_READ, &PadConfigData);

  if (Enable) {
    //
    //Lock the Pad Config Register's
    //
    PadConfigData |= GPIO_PAD_LOCK;
    ReadWritePadXConf(Community, PinNum, PAD1, PAD_WRITE, &PadConfigData);
  } else {
    //
    //UnLock the Pad Config Register's
    //
    PadConfigData &= (~GPIO_PAD_LOCK);
    ReadWritePadXConf(Community, PinNum, PAD1, PAD_WRITE, &PadConfigData);
  }
}

/**
  Function to configure the specified pin to any one of native mode

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Mode          Native Mode - M0, M1, .., M15

  @retval    Boolean       0 - successful native mode configuration,1 - Unsuccessful, Pad Could be locked
**/
BOOLEAN
EFIAPI
SetNativeMode (
  IN UINT32 Community,
  IN UINT32 PinNum,
  IN UINT8  Mode
  )
{
  UINT32 PadConfigData = 0;

  if (!GetPinPadLockStatus(Community, PinNum)) {
    //
    // Read Pad0 Configure Data
    //
    ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

    //
    // Disable GPIO and Keep Mode Field to default first.
    //
    PadConfigData &= (GPIO_EN_MASK | GPIO_PAD_MODE_MASK);

    //
    // Update the Mode in Pad config Data
    //
    PadConfigData = PadConfigData | Mode << 16;

    ReadWritePadXConf(Community,PinNum,PAD0,PAD_WRITE,&PadConfigData);

    return GPIO_RETURN_PASS;
  } else
    return LOCKED;
}

/**
  Function to get the native mode number

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    Mode          Native Mode - M0:0, M1:1, .., M15:15
                           Failure -  Value 0xFF.
**/
UINT8
EFIAPI
GetNativeMode (
  IN UINT32 Community,
  IN UINT32 PinNum
  )
{
  UINT32 PadConfigData = 0;
  UINT8  NativeMode = 0xFF;

  //
  // Read the Pad1 Config Data
  //
  ReadWritePadXConf(Community,PinNum,PAD0,PAD_READ,&PadConfigData);

  PadConfigData &= (~GPIO_PAD_MODE_MASK);

  NativeMode = (UINT8) (PadConfigData >> 16);

  DEBUG((EFI_D_INFO, " PAD Mode:   = M%d \n", NativeMode));

  return NativeMode;
}

/**
  Function to configure GPO Tx State for specified pin in the community.

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Value         1-HIGH, 0-LOW

**/
VOID
GPIOSetTXValue (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN BOOLEAN Value
  )
{
  UINT32 PadConfigData = 0;
  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  if (Value) {
    //
    // Set GPIO TX to Logic HIGH
    //
    PadConfigData |= GPIO_TX_STATE;
    ReadWritePadXConf(Community, PinNum, PAD0, PAD_WRITE, &PadConfigData);
  } else {
    //
    // Set GPIO TX to Logic LOW
    //
    PadConfigData &= (~GPIO_TX_STATE);
    ReadWritePadXConf(Community, PinNum, PAD0, PAD_WRITE, &PadConfigData);
  }
}

/**
  Function to get the GPO Tx state of specified pin in the community.

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    Boolean       1-HIGH,0-LOW
**/
BOOLEAN
EFIAPI
GPIOGetTXValue (
  IN UINT32 Community,
  IN UINT32 PinNum
  )
{
  UINT32 PadConfigData = 0;
  
  //
  // Read the Pad0 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  if (PadConfigData & GPIO_TX_STATE)
    return HIGH;
  else
    return LOW;
}

/**
  Function to configure pin to any one of GPIO functionalities for specified pin in the community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Direction     Gpio direction
                           GPIO - 0
                           GPO  - 1
                           GPI  - 2
                           HI-Z - 3
  @param[in] Value         Configure HIGH or LOW for GPO (Tx)
                           1-HIGH,0-Low
**/
VOID
SetGPIOMode (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN UINT8   Direction,
  IN BOOLEAN Value
  )
{
  UINT32 PadConfigData = 0;

  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  //
  // Enable the GPIO
  //
  PadConfigData |= GPIO_DIRECTION_EN;

  //
  // Set the GPIO direction
  //
  PadConfigData &= (~GPIO_DIRECTION);
  PadConfigData |= (UINT32)Direction << 8;

  //
  // Set the Tx HIGH / LOW if it is GPIO or GPO
  //
  if (Direction == 0 || Direction == 1) {
    if (Value)
      PadConfigData |= GPIO_TX_STATE;
    else
      PadConfigData &= (~GPIO_TX_STATE);
  }
  //
  // write the Pad1 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_WRITE, &PadConfigData);
}

/**
  Function to get the GPIO direction configuration for specified pin in the community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number

  @retval    UINT8         The value for GPIO direction
                           GPIO-0
                           GPO-1
                           GPI-2
                           HI-Z-3
**/
UINT8
EFIAPI
GetGPIOMode (
  IN UINT32 Community,
  IN UINT32 PinNum
  )
{
  UINT32 PadConfigData = 0;
  UINT8  GpioDirection = 0xFF;
  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);
  if (!(PadConfigData & GPIO_DIRECTION_EN)) {
    DEBUG ((EFI_D_INFO, "Pin is not configured for GPIO \n"));
    return GpioDirection;
  }
  GpioDirection = (UINT8)((PadConfigData & GPIO_DIRECTION) >> 8);
  if (GpioDirection < 0x04)
    return GpioDirection;
  else
    return 0xFF;
}

/**
  Function to configure termination of any specified pin in the community

  @param[in] Community     NORTH-0x01
                           EAST-0x02
                           SOUTH_EAST-0x03
                           SOUTH_WEST-0x04
  @param[in] PinNum        Pin number
  @param[in] Pullup        Enable pullup or pulldown
                           1-Pull Up
                           0-Pull Down
  @param[in] Value         Pull value
                           1-1K
                           2-5K
                           4-20K
  @retval    Boolean       0-Successful termination configured, 1-Unsuccessful could be due to unsupported value.
**/
BOOLEAN
EFIAPI
GPIOSetPadTerm (
  IN UINT32  Community,
  IN UINT32  PinNum,
  IN BOOLEAN PullUp,
  IN UINT8   Value
  )
{
  UINT32 PadConfigData = 0;

  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  //
  // Configure Pull Up or Down based on inputs.
  //
  if (PullUp)
    PadConfigData = (PadConfigData  | PULL_UP_EN);
  else
    PadConfigData = (PadConfigData  & (~PULL_UP_EN));

  //
  // Configure the Pull value based Value argument provided
  //
  switch (Value) {
    case 1:
      PadConfigData = (PadConfigData & PULLUP_OR_PULLDOWN_MASK) | PULL_1K;
      break;
    case 2:
      PadConfigData = (PadConfigData & PULLUP_OR_PULLDOWN_MASK) | PULL_5K;
      break;
    case 4:
      PadConfigData = (PadConfigData & PULLUP_OR_PULLDOWN_MASK) | PULL_20K;
      break;

    default:
      break;
  }
  if (Value == 1 || Value == 2 || Value == 4) {
    //
    // Write the Pad0 Config Data
    //
    ReadWritePadXConf(Community, PinNum, PAD0, PAD_WRITE, &PadConfigData);
    return GPIO_RETURN_PASS;
  } else {
    return GPIO_RETURN_FAIL;
  }
}

/**
  Function to current pin termination value of specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[out] *PullValue    Pull value
                            1 - 1K
                            2 - 5K
                            4 - 20K

  @retval    Boolean        1-Pull Up,0-Pull Down
**/
BOOLEAN
EFIAPI
GPIOGetPadTerm (
  IN UINT32 Community,
  IN UINT32 PinNum,
  IN UINT8  *PullValue
  )
{
  UINT32 PadConfigData = 0;

  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  if (PadConfigData & PULL_UP_EN) {
    if (PadConfigData & PULL_1K) {
      *PullValue = 1;
      DEBUG ((EFI_D_INFO, "Pin configured to 1K Pull Up \n"));
    } else if (PadConfigData & PULL_5K) {
      *PullValue = 2;
      DEBUG ((EFI_D_INFO, "Pin configured to 5K Pull Up \n"));
    } else if (PadConfigData & PULL_5K) {
      *PullValue = 4;
      DEBUG ((EFI_D_INFO, "Pin configured to 20K Pull Up \n"));
    } else if (PadConfigData & (~PULL_VALUE_MASK)) {
      *PullValue = 0;
      DEBUG ((EFI_D_INFO, "Pin configured to 0K Pull Up \n"));
    } else {
      *PullValue = 0xFF;
      DEBUG ((EFI_D_INFO, "UnSupported Pull Up Value \n"));
    }
    return PULL_UP;
  } else {
    if (PadConfigData & PULL_1K) {
      *PullValue = 1;
      DEBUG ((EFI_D_INFO, "Pin configured to 1K Pull Down \n"));
    } else if (PadConfigData & PULL_5K) {
      *PullValue = 2;
      DEBUG ((EFI_D_INFO, "Pin configured to 5K Pull Down \n"));
    } else if (PadConfigData & PULL_5K) {
      *PullValue = 4;
      DEBUG ((EFI_D_INFO, "Pin configured to 20K Pull Down \n"));
    } else if (PadConfigData & (~PULL_VALUE_MASK)) {
      *PullValue = 0;
      DEBUG ((EFI_D_INFO, "Pin configured to 0K Pull Down \n"));
    } else {
      *PullValue = 0xFF;
      DEBUG ((EFI_D_INFO, "UnSupported Pull Down Value \n"));
    }
    return PULL_DOWN;
  }
}

/**
  Function to Configure the Invert Rx/Tx Data and Rx/Tx Enable of specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[out] InvRxTxValue  0=No Inversion, 1=Inversion Enabled
                            [0] RX Enable
                            [1] TX Enable
                            [2] RX Data
                            [3] TX Data

**/
VOID
GPIOSetInvRxTx (
  IN UINT32 Community,
  IN UINT32 PinNum,
  IN UINT8  InvRxTxValue
  )
{
  UINT32 PadConfigData = 0;

  ReadWritePadXConf(Community, PinNum, PAD1, PAD_READ, &PadConfigData);

  PadConfigData = (PadConfigData & GPIO_INV_RXTX_MASK) | InvRxTxValue;

  //
  // Write the back the Pad1 configure value
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_WRITE, &PadConfigData);
}

/**
  Function to get current the Invert Rx/Tx Data and Rx/Tx Enable configuration of specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number

  @retval     InvRxTxValue  0-No Inversion, 1-Inversion Enabled
                            [0] RX Enable
                            [1] TX Enable
                            [2] RX Data
                            [3] TX Data
**/
UINT8
EFIAPI
GPIOGetInvRxTx (
  IN UINT32 Community,
  IN UINT32 PinNum
  )
{
  UINT32 PadConfigData = 0;
  UINT8  InvRxTxValue = 0;

  //
  // Read the Pad1 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_READ, &PadConfigData);

  //
  // Extract the Invert Rx TX value
  //
  InvRxTxValue = (UINT8 )(PadConfigData & (~GPIO_INV_RXTX_MASK)) >> 4;

  DEBUG ((EFI_D_INFO, "Pin Invert Rx Tx Value = %x \n", InvRxTxValue));

  return InvRxTxValue;
}

/**
  Function to configure the interrupt line and type for specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[in]  LineNum       0 - Interrupt Line 0
                            1 - Interrupt Line 1

                            15 = Interrupt Line 15
  @param[in]  Type          Type of interrupt
                            0 = Interrupt/Wake Disable (Disabled and no Pad State toggles trigger the Wake/Interrupt logic)
                            1 = Falling Edge Detect Interrupt/Wake
                            2 = Rising Edge Detect Interrupt/Wake
                            3 = Falling or Rising Edge Detect Interrupt/Wake
                            4 = Level Interrupt/Wake (Bypass edge detect logic and Pad state is directly toggling the Wake/Interrupt logic)
**/
VOID
ConfigureGpioInt (
  IN UINT32 Community,
  IN UINT32 PinNum,
  IN UINT32 LineNum,
  IN UINT8  Type
  )
{
  UINT32 PadConfigData = 0;

  //
  // Read the Pad0 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  //
  // Configure the Interrupt line
  //
  PadConfigData = (PadConfigData & GPIO_INT_MASK) | (LineNum << 28);

  //
  // Write the back the Pad0 configure value
  //
  ReadWritePadXConf(Community, PinNum, PAD0, PAD_WRITE, &PadConfigData);

  //
  // Read the Pad1 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_READ, &PadConfigData);

  //
  // Configure the Interrupt type
  //
  PadConfigData = (PadConfigData & GPIO_INT_TYPE) | Type;

  //
  // Write the back the Pad1 configure value
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_WRITE, &PadConfigData);
}

/**
  Function to Get the current interrupt configuration for specified pin in the community

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  PinNum        Pin number
  @param[out] LineNum       Interrupt line number refer Int_Select from PlatfromInfo.h
                            0 = Interrupt Line 0
                            1 = Interrupt Line 1

                                15 = Interrupt Line 15
  @param[out] Type          Type of interrupt
                            0 = Interrupt/Wake Disable (Disabled and no Pad State toggles trigger the Wake/Interrupt logic)
                            1 = Falling Edge Detect Interrupt/Wake
                            2 = Rising Edge Detect Interrupt/Wake
                            3 = Falling or Rising Edge Detect Interrupt/Wake
                            4 = Level Interrupt/Wake (Bypass edge detect logic and Pad state is directly toggling the Wake/Interrupt logic)
                            4>  Unsupported value

**/
VOID
GetGpioIntConfig (
  IN UINT32    Community,
  IN UINT32    PinNum,
  IN OUT UINT8 *LineNum,
  IN OUT UINT8 *IntType
  )
{
  UINT32 PadConfigData = 0;

  //
  // Read the Pad0 Config Data
  //
  ReadWritePadXConf(Community, PinNum, PAD0, PAD_READ, &PadConfigData);

  //
  // Configure the Interrupt line
  //
  *LineNum = (UINT8)((PadConfigData & (~GPIO_INT_MASK)) >> 28);

  //
  // Read the back the Pad1 configure value
  //
  ReadWritePadXConf(Community, PinNum, PAD1, PAD_READ, &PadConfigData);

  //
  // Extract the Interrupt type from Pad1 Control register
  //
  *IntType = (UINT8)(PadConfigData & (~GPIO_INT_TYPE));
}

/**
  Set particular GPIO pin as SMI / SCI

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  IntSel        Selected interrupt number to rout
  @param[in]  Type          Configure SMI / SCI ?

**/
VOID
GpioGPEConfig (
  IN UINT32 community,
  IN UINT32 IntSel,
  IN UINT8  Type
  )
{
  GPIO_ROUT    gpio_rout;
  ALT_GPIO_SMI alt_gpio_smi;
  GPE0a_EN     gpe0a_en;

  alt_gpio_smi.smi_ = 0;
  gpe0a_en.gpe0a_   = 0;

  if (Type == SMI) {

    gpio_rout.rout_= (MmioRead32 (PMC_BASE_ADDRESS + GPIO_ROUT_REG));

    //
    // Program GPIO_ROUT set to 01 for SMI, bits [15-0] is for North Community and [31-16] for SouthWest Community INTR[15:8]
    // Program ALT_GPIO_SMI bits [7-0] for North Community and [15-8] bits for SouthWest Community  INTR[15:8]
    //
    if (community == GPIO_MMIO_OFFSET_N) {
      gpio_rout.rout_ |= (1 << ((IntSel-8) * 2));
      gpio_rout.rout_ &= ~(1 << ((IntSel-8) * 2 + 1));
      alt_gpio_smi.smi_ |= (1 << (IntSel - 8));
    } else if (community == GPIO_MMIO_OFFSET_SW) {
      gpio_rout.rout_ |= (1 << (IntSel * 2));
      gpio_rout.rout_ &= ~(1 << (IntSel * 2 + 1));
      alt_gpio_smi.smi_ |= (1 << (IntSel));
    }

    //
    // GPIO_ROUT Program
    //
    MmioWrite32 (PMC_BASE_ADDRESS + GPIO_ROUT_REG, gpio_rout.rout_);

    //
    // ALT_GPIO_SMI Program
    //
    alt_gpio_smi.smi_ = (IoRead32 (ACPI_BASE_ADDRESS + ALT_GPIO_SMI_REG) | (alt_gpio_smi.smi_ & 0x0000FFFF));
    IoWrite32 (ACPI_BASE_ADDRESS + ALT_GPIO_SMI_REG, alt_gpio_smi.smi_);

  } else if (Type == SCI) {
    gpio_rout.rout_= (MmioRead32 (PMC_BASE_ADDRESS + GPIO_ROUT_REG));

    //
    // Program GPIO_ROUT set to 10 for SCI, bits [15-0] is for North Community and [31-16] for SouthWest Community
    // Program GPE0a_EN bits [23-16] for North Community and bits [31-24]for SouthWest Community INTR[15:8]
    //
    if (community == GPIO_MMIO_OFFSET_N) {
      gpio_rout.rout_ |= (1 << (((IntSel-8) * 2) + 1));
      gpio_rout.rout_ &= ~(1 << ((IntSel-8) * 2));
      gpe0a_en.gpe0a_ |= (1 << (IntSel + 8));
    } else if (community == GPIO_MMIO_OFFSET_SW) {
      gpio_rout.rout_ |= (1 << ((IntSel * 2) + 1));
      gpio_rout.rout_ &= ~(1 << (IntSel * 2));
      gpe0a_en.gpe0a_ |= (1 << (IntSel+16));
    }

    //
    // GPIO_ROUT Program
    //
    MmioWrite32 (PMC_BASE_ADDRESS + GPIO_ROUT_REG, gpio_rout.rout_);

    //
    // GPE0a_EN Program
    //
    gpe0a_en.gpe0a_= (IoRead32 (ACPI_BASE_ADDRESS + GPE0A_EN_REG) | (gpe0a_en.gpe0a_ & 0xFFFF0000));
    IoWrite32 (ACPI_BASE_ADDRESS + GPE0A_EN_REG, gpe0a_en.gpe0a_);
  }
  return;
}

/**
  Set particular GPIO Wake Cable

  @param[in]  Community     NORTH-0x01
                            EAST-0x02
                            SOUTH_EAST-0x03
                            SOUTH_WEST-0x04
  @param[in]  CHV_PAD_MISC  Wake capable mask details
**/
VOID
ConfigureGPIOWake (
  IN UINT32       community,
  IN CHV_PAD_MISC Pad_misc
  )
{
  GPIO_WAKE gpio_wake;
  gpio_wake.wake_     = 0;

  if (Pad_misc.r.Wake_Mask_Position <= CHV_GPIO_WAKE_REG0_BITS) {
    gpio_wake.wake_= MmioRead32(IO_BASE_ADDRESS + community + GPIO_WAKE_MASK_REG0);
    gpio_wake.wake_ |= (1<<Pad_misc.r.Wake_Mask_Position);
    MmioWrite32 (IO_BASE_ADDRESS + community + GPIO_WAKE_MASK_REG0, gpio_wake.wake_);
    DEBUG ((EFI_D_INFO, " Wake unmask programming: GPIO_WAKE_MASK_REG0 = 0x%08x\n ", MmioRead32(IO_BASE_ADDRESS + community + GPIO_WAKE_MASK_REG0)));
  } else {
    gpio_wake.wake_= MmioRead32(IO_BASE_ADDRESS + community + GPIO_WAKE_MASK_REG1);
    gpio_wake.wake_ |= (1<< (Pad_misc.r.Wake_Mask_Position - CHV_GPIO_WAKE_REG0_BITS));
    MmioWrite32 (IO_BASE_ADDRESS + community + GPIO_WAKE_MASK_REG1, gpio_wake.wake_);
    DEBUG ((EFI_D_INFO, " Wake unmask programming: GPIO_WAKE_MASK_REG1 = 0x%08x\n ", MmioRead32(IO_BASE_ADDRESS + community + GPIO_WAKE_MASK_REG1)));
  }
}

/**
  Set GPIO PAD control 0 and 1 registers for N/E/SW/SE GPIO communities

  @param[in] GPIOTable_Index          Begin Pin Number to start configuring in GPIO_Conf_Data table
  @param[in] GPIOTable_NumberofPins   Number of Pins to configure in GPIO_Conf_Data table
  @param[in] Gpio_Conf_Data           GPIO_CONF_PAD_INIT data array

**/
VOID
InternalGpioPADConfig (
  IN UINT32         GPIOTable_Index,
  IN UINT32         GPIOTable_NumberofPins,
  CHV_GPIO_PAD_INIT *Gpio_Conf_Data
  )
{

  UINT32         index,community;
  UINT32         pad_conf0, pad_conf1;
  UINT32         mmio_conf0, mmio_conf1;
  GPIO_INTERRUPT gpio_int_mask;
  GPE0a_EN       gpe0a_en;
  ALT_GPIO_SMI   alt_gpio_smi;

  gpio_int_mask.intr_ = 0;
  gpe0a_en.gpe0a_     = 0;
  alt_gpio_smi.smi_   = 0;
  community           = 0;

  DEBUG ((EFI_D_INFO, "FamilyName               Mode_Sel  Mode#  IntType  Pull Up/Down    MMIO_CONF0    PAD_CONF0    MMIO_CONF1    PAD_CONF1\n"));
  DEBUG ((EFI_D_INFO, "-------------------------------------------------------------------------------------------------------------------------\n"));
  for (index = GPIOTable_Index; index < (GPIOTable_Index + GPIOTable_NumberofPins); index++) {
    //
    // Calculate the MMIO Address for specific GPIO pin CONF0 register pointed by index.
    //
    mmio_conf0 = Gpio_Conf_Data[index].MMIO_ADDRESS + CHV_GPIO_PAD_CONF0_OFFSET;
    mmio_conf1 = Gpio_Conf_Data[index].MMIO_ADDRESS + CHV_GPIO_PAD_CONF1_OFFSET;
    community  = Gpio_Conf_Data[index].Community;

#ifdef EFI_DEBUG
    if (Gpio_Conf_Data[index].pad_name != NULL) {
    DEBUG ((EFI_D_INFO, " %s >>", Gpio_Conf_Data[index].pad_name));
    }
#endif
    DEBUG ((EFI_D_INFO, "  %d        %d        %d         %02d      ", Gpio_Conf_Data[index].padConfg0.r.GPIOEn, Gpio_Conf_Data[index].padConfg0.r.Pmode, Gpio_Conf_Data[index].padConfg0.r.IntSel,     Gpio_Conf_Data[index].padConfg0.r.Term));

    pad_conf0 = MmioRead32(mmio_conf0);
    pad_conf0 = (pad_conf0 & (~Gpio_Conf_Data[index].padConfg0_changes.padCnf0)) | Gpio_Conf_Data[index].padConfg0.padCnf0;
    MmioWrite32(mmio_conf0,pad_conf0 );

    pad_conf1 = MmioRead32(mmio_conf1);
    pad_conf1 = (pad_conf1 & (~Gpio_Conf_Data[index].padConfg1_changes.padCnf1)) | Gpio_Conf_Data[index].padConfg1.padCnf1;
    MmioWrite32(mmio_conf1,pad_conf1 );

    DEBUG ((EFI_D_INFO, "    0x%08x    %08x     0x%08x    %08x\n", mmio_conf0, MmioRead32(mmio_conf0), mmio_conf1, MmioRead32(mmio_conf1)));

    //
    // GPIO_Interrupt_mask:  Unmask interrupts
    //

    if (Gpio_Conf_Data[index].PAD_MISC.r.intr_mask == NonMaskable) {
      gpio_int_mask.intr_ = MmioRead32(IO_BASE_ADDRESS + community + GPIO_INTERRUPT_MASK);
      gpio_int_mask.r.GPIO_ |= 1 << Gpio_Conf_Data[index].padConfg0.r.IntSel;
      MmioWrite32 (IO_BASE_ADDRESS + community + GPIO_INTERRUPT_MASK, gpio_int_mask.intr_);
      DEBUG ((EFI_D_INFO, " Interrupt unmask programming:  GPIO_INTERRUPT_MASK = 0x%08x \n ", MmioRead32(IO_BASE_ADDRESS + community + GPIO_INTERRUPT_MASK)));
    }

    //
    // Unmask wakes based on the wake_mask_position
    //
    if (Gpio_Conf_Data[index].PAD_MISC.r.wake_able == UNMASK_WAKE && Gpio_Conf_Data[index].PAD_MISC.r.Wake_Mask_Position != NA) {
      ConfigureGPIOWake(community,Gpio_Conf_Data[index].PAD_MISC);
    }

    //
    // Program the GPE based on Type SMI / SCI
    //
    if (Gpio_Conf_Data[index].PAD_MISC.r.GPE_ENABLE != NA) {
      GpioGPEConfig(community,Gpio_Conf_Data[index].padConfg0.r.IntSel,(UINT8)Gpio_Conf_Data[index].PAD_MISC.r.GPE_ENABLE);
    }
  }
  DEBUG ((EFI_D_INFO, "GPE Programming: GPIO_ROUT_REG  = 0x%08x, \n GPE0A_EN_REG = 0x%08x,\n ALT_GPIO_SMI_REG = 0x%08x \n", MmioRead32 (PMC_BASE_ADDRESS + GPIO_ROUT_REG),IoRead32 (ACPI_BASE_ADDRESS + GPE0A_EN_REG),IoRead32 (ACPI_BASE_ADDRESS + ALT_GPIO_SMI_REG) ));
}

/**
  Set GPIO families register configuration for N/E/SW/SE GPIO communities

  @param[in] Gpio_Families_Mmio_Offset        GPIO_MMIO_OFFSET_SW / GPIO_MMIO_OFFSET_N / GPIO_MMIO_OFFSET_E / GPIO_MMIO_OFFSET_SE.
  @param[in] Gpio_Communities_Num             Families numbers to config for GPIO communities.
  @param[in] Gpio_Families_Data               GPIO_CONF_FAMILY_INIT data array for each GPIO family.

**/
VOID
InternalGpioFamiliesConfig (
  IN UINT32             Gpio_Families_Mmio_Offset,
  IN UINT32             Gpio_Communities_Num,
  GPIO_CONF_FAMILY_INIT *Gpio_Families_Data
  )
{
  UINT32                index;
  UINT32                gpio_family_conf;
  CHV_RCOMP_CTRL        rcomp_ctrl;
  CHV_RCOMP_VALUE       rcomp_value;
  CHV_FAMILY_RCOMP_CNFG rcomp_family;

  for (index=0; index < Gpio_Communities_Num; index++) {

#ifdef EFI_DEBUG
    if (Gpio_Families_Data[index].family_name != NULL) {
    DEBUG ((EFI_D_ERROR, " %S, ", Gpio_Families_Data[index].family_name));
    }
#endif
    DEBUG ((EFI_D_INFO, "    HYSCTL = %d, CurSource = %d, HSMode = %d, odpuEn = %d, MMIO = 0x%08x,",
            Gpio_Families_Data[index].confg.r.HYSCTL,
            Gpio_Families_Data[index].confg.r.CurrSrcStr,
            Gpio_Families_Data[index].confg.r.hsMode,
            Gpio_Families_Data[index].confg.r.odpullEn,
            Gpio_Families_Data[index].mmioAddr));

    //
    // If want to changes in GPIO setting please do in GPIO table in BoardGpios.h
    //
    gpio_family_conf = Gpio_Families_Data[index].confg.famCnf & Gpio_Families_Data[index].confg_change.famCnf;
    MmioWrite32(Gpio_Families_Data[index].mmioAddr+FAMILY_CONFIG_REG,gpio_family_conf );

    DEBUG ((EFI_D_INFO, "gpio_family_conf value = 0x%08x \n", gpio_family_conf));

    //
    // RCOMP routine here for each family
    //
    //
    // To Run an Initial RCOMP cycle:
    //   1.      Set RCOMP FSM Enable
    //   2.      Wait 10uS for binary calibration
    //   3.      Copy the P/N strength and slew values from RCOMP value register to RCOMP Reg for all families that need the RCOMP value.
    // To Run a periodic RCOMP cycle:
    //   1.      Clear RCOMP Start
    //   2.      Set RCOMP Start
    //   3.      Wait 10uS for thermometer calibration
    //   4.      Copy the P/N strength and slew values from RCOMP value register to RCOMP Reg for all families that need the RCOMP value. Only the lower 2 bits will change.
    //

    if (Gpio_Families_Data[index].family_misc.r.RCOMP_ENABLE) {
      rcomp_ctrl.rcomCtrl = MmioRead32 (Gpio_Families_Data[index].mmioAddr+RCOMP_CONTROl_REG);

      rcomp_ctrl.r.RFSMEn = 0x1;
      rcomp_ctrl.r.InitCalValN = 0x1D;
      rcomp_ctrl.r.InitCalValP = 0x1D;
      rcomp_ctrl.r.RFSMStopCyc = 4;
      rcomp_ctrl.r.RFSMStop = 1;
      rcomp_ctrl.r.ircintclkperiod = 3;

      MmioWrite32(Gpio_Families_Data[index].mmioAddr+RCOMP_CONTROl_REG,rcomp_ctrl.rcomCtrl);

      NanoSecondDelay(10000); // 10 micro second delay

      rcomp_value.rcompValue = MmioRead32 (Gpio_Families_Data[index].mmioAddr+RCOMP_VALUE_REG);
      rcomp_family.familyRcompCnfg = MmioRead32 (Gpio_Families_Data[index].mmioAddr+FAMILY_RCOMP_CONFIG_REG);
      rcomp_family.r.NStrVal = rcomp_value.r.NStrVal;
      rcomp_family.r.PStrVal = rcomp_value.r.PStrVal;
      rcomp_family.r.NSlewVal = rcomp_value.r.NSlewVal;
      rcomp_family.r.PSlewVal = rcomp_value.r.PSlewVal;
      MmioWrite32(Gpio_Families_Data[index].mmioAddr+FAMILY_RCOMP_CONFIG_REG,rcomp_family.familyRcompCnfg);
    }
  }
}

/**
  Set GPIO SAI registers for N/E/SW/SE GPIO communities

  @param[in] SAI_Conf_Data    GPIO_SAI_INIT data array for each GPIO communities.
  @param[in] UINT32           Size of the table
**/
VOID
SaiSettingOfGpioFamilies (
  GPIO_SAI_INIT *SAI_Conf_Data,
  UINT32        familySize
  )
{

  UINT32 Index = 0;
  DEBUG ((EFI_D_ERROR, "SAI Setting of GPIO families Start\n"));
  //
  // Community SAI programming
  //
  for(Index = 0;Index < familySize;Index++){
    MmioWrite32 (IO_BASE_ADDRESS + SAI_Conf_Data[Index].Community + SAI_Conf_Data[Index].Offset , SAI_Conf_Data[Index].val_.secSAI);
  }
   DEBUG ((EFI_D_ERROR, "SAI Setting of GPIO families End\n "));
}
