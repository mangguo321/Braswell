/** @file
  Boards identification for multiplatform.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <MultiPlatformLib.h>
#include <PeiKscLib.h>
#include <Library/TimerLib.h>
#include <Library/GpioLib.h>

/**
  This function Issues EC command (0x0D) to EC to get board ID and FAB ID details and returns to the
  calling function. Procedure to get Board ID & FAB ID is common to both Mobile and Desktop platforms.
  Bit position details
  when EC Command 0x0D is issue to EC
  Read value 1st byte = EC LSB [ 7:0]
  Read value 2nd byte = EC MSB [15:8]

  Bits [3:0] - Board ID (Range from 0x00 to 0x0F)
  Bit  6     - 0 = Mobile/ULT/Embedded;
             - 1 = Desktop
               (Note: Not required consider this bit when Bit 7 is set)
  Bit 7      - 0 = Mainstream
             - 1 =  Entry Notebook & Desktop (Essential Notebook)
  Bit 8      - Generation (Tick / Tock)
               (Note: Not required consider this bit when Bit 7 is set)
  Bits 11:9  - FAB ID (Range from 0x00 to 0x07)
  Bits 15:12 - Reserved

  @param[in]   BoardFabIds           Board ID & FabId ID as determined through the EC.

  @retval    EFI_SUCCESS             The function completed successfully.
                                     BoardFabIds word contains
                                     LSB - FAB ID   (0x00 - 0x07)
                                     MSB - Board ID (1, 2, 4, 8)
  @retval    EFI_DEVICE_ERROR        KSC fails to respond.

**/
EFI_STATUS
GetBoardFabIdsKsc (
  OUT UINT16                    *BoardFabIds
  )
{
  EFI_STATUS     Status = EFI_SUCCESS;
  UINT8          EcMSB;
  UINT8          EcLSB;

  //
  // Return Unknown Board ID & Fab ID in case of error.
  //
  *BoardFabIds = 0xFFFF;

  //
  // Send Board ID command to EC
  //
  Status = SendKscCommand (0x0D);
  if (!EFI_ERROR(Status)) {
    //
    // Read 1st Byte from EC (MSB)
    //
    Status = ReceiveKscData (&EcLSB);
    if (!EFI_ERROR(Status)) {
      //
      // Read 2nd Byte from EC (LSB)
      //
      Status = ReceiveKscData (&EcMSB);
      if (!EFI_ERROR(Status)) {
        EcMSB = EcMSB & 0x0E;
        EcMSB = EcMSB >> 1;
        EcLSB = EcLSB & 0x7; // Need new EC to support 4 bits for BSW CR board ID, or it would be wrong if and w/ 0xF
        *BoardFabIds = (UINT16) ((EcLSB << 8) | EcMSB );  //  MSB[11:8] - Board ID;  LSB[3:0] - FAB ID
        Status = EFI_SUCCESS;
      }
    }
  }

  return Status;
}

EFI_STATUS
GetBoradFabIdsGpio (
  OUT UINT8 *BoardId,
  OUT UINT8 *FabId
  )
{
  UINT32                          mmio_conf0;

  //
  // Assuming Braswell Cherry Hill/Cherry Island - Get Board ID/Fab ID from GPIO
  // Set Pad Config to GPIO for BoardId read
  // mmio_conf0 = (IO_BASE_ADDRESS + Community + Offset + 0x0);
  // PANEL1_BKLTCTL - BoardId Bit 0, North Community is 0x8000
  //
  mmio_conf0 = (IO_BASE_ADDRESS + 0x8000 + 0x5428) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  //PANEL1_BKLTEN - BoardId Bit 1
  mmio_conf0 = (IO_BASE_ADDRESS + 0x8000 + 0x5400) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  //PANEL1_VDDEN - BoardId Bit 2
  mmio_conf0 = (IO_BASE_ADDRESS + 0x8000 + 0x5460) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  //Get BoardId
  *BoardId = (UINT8)((MmioRead16(IO_BASE_ADDRESS + 0x8000 + 0x5428) & BIT0) | \
                    ((MmioRead16(IO_BASE_ADDRESS + 0x8000 + 0x5400) & BIT0) << 1) | \
                    ((MmioRead16(IO_BASE_ADDRESS + 0x8000 + 0x5460) & BIT0) << 2));

  //
  // Set Pad Config to GPIO for FabId read
  // mmio_conf0 = (IO_BASE_ADDRESS + Community + Offset + 0x0);
  // MF_ISH_GPIO_0 - FAB ID bit 0, East Community is 0x10000
  //
  mmio_conf0 = (IO_BASE_ADDRESS + 0x10000 + 0x4830) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  // MF_ISH_GPIO_1 - FAB ID bit 1
  mmio_conf0 = (IO_BASE_ADDRESS + 0x10000 + 0x4818) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  // MF_ISH_GPIO_2 - FAB ID bit 2
  mmio_conf0 = (IO_BASE_ADDRESS + 0x10000 + 0x4848) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  // MF_ISH_GPIO_3 - FAB ID bit 3
  mmio_conf0 = (IO_BASE_ADDRESS + 0x10000 + 0x4800) + CHV_GPIO_PAD_CONF0_OFFSET;
  MmioWrite32(mmio_conf0 ,0x00008200); //Set GPIOCfg [10:8] - GPI(0x02), GPIOEn -BIT15
  //Get FabId
  *FabId = (UINT8)((MmioRead16(IO_BASE_ADDRESS + 0x10000 + 0x4830) & BIT0) | \
                  ((MmioRead16(IO_BASE_ADDRESS + 0x10000 + 0x4818) & BIT0) << 1) | \
                  ((MmioRead16(IO_BASE_ADDRESS + 0x10000 + 0x4848) & BIT0) << 2) | \
                  ((MmioRead16(IO_BASE_ADDRESS + 0x10000 + 0x4800) & BIT0) << 3));

  return EFI_SUCCESS;
}
