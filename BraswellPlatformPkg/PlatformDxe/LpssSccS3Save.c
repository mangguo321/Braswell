/** @file
  Save PCI configuration registers of PCI mode LPSS and SCC devices for S3. 

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformDxe.h"
#include "Platform.h"
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/CpuIo2.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PciLib.h>
#include <Guid/EventGroup.h>
#include <Guid/Chv2Variable.h>
#include <Library/PcdLib.h>
#include <Library/GpioLib.h>
#include <SaveMemoryConfig.h>


typedef struct {
  UINT8     Register;
  UINT8     Function;
  UINT8     Device;
  UINT8     Bus;
  UINT32    ExtendedRegister;
} PCI_IO_ADDRESS;

/**
  This function saves PCI configuration registers of PCI mode LPSS and SCC devices for S3. 
  Both saving(normal boot path) and restoring(S3 resume path) must be done before these 
  devices are switched to ACPI mode. 

  @param[in] Event          
  @param[in] Context
  
  @retval EFI_SUCCESS             

**/
VOID
EFIAPI
SaveLpssSccPciMode (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  PCI_IO_ADDRESS        PciAddress;
  UINT32                Data32;
  UINT16                Data16;
  UINT8                 Data8;
  UINT8                 Mask;
  UINTN                 Index;
  UINTN                 Offset;
  UINT8                 ExtReg[] = { 0x9E, 0x9D };
  UINT8                 RegTable[] = {                  
                          //Bus  ,   Dev,  Func,    eMMC Device
                          0x00 ,  0x10,  0x00,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    SDIO Device
                          0x00 ,  0x11,  0x00,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    SDCard Device
                          0x00 ,  0x12,  0x00,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                                      
                          //Bus  ,   Dev,  Func,    LPE Device
                          0x00 ,  0x15,  0x00,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                         //Bus  ,   Dev,  Func,    LPIO2 DMA Device
                          0x00 ,  0x18,  0x00,
                         //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C0 Device
                          0x00 ,  0x18,  0x01,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C1 Device
                          0x00 ,  0x18,  0x02,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C2 Device
                          0x00 ,  0x18,  0x03,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C3 Device
                          0x00 ,  0x18,  0x04,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C4 Device
                          0x00 ,  0x18,  0x05,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C5 Device
                          0x00 ,  0x18,  0x06,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO2 I2C6 Device
                          0x00 ,  0x18,  0x07,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                                       
                          //Bus  ,   Dev,  Func,    LPIO1 DMA Device
                          0x00 ,  0x1E,  0x00,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO1 HSUART0 Device
                          0x00 ,  0x1E,  0x03,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //Bus  ,   Dev,  Func,    LPIO1 HSUART1 Device
                          0x00 ,  0x1E,  0x04,
                          //00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
                          0xF2 ,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
                    
                          //
                          //0xFF indicates the end of the table
                          //
                          0xFF
                      };

  //
  // Save PCI-Host bridge settings (0, 0, 0). 0x90, 94 and 9c are changed by CSM
  // and vital to S3 resume. That's why we put save code here
  //
  PciAddress.Bus      = 0;
  PciAddress.Device   = 0;
  PciAddress.Function = 0;
  PciAddress.ExtendedRegister = 0;

  for (Index = 0; Index < 2; Index++) {
    //
    // Read SRAM setting from Pci(0, 0, 0)
    //
    PciAddress.Register = ExtReg[Index];
    Data8 = MmioRead8 (
              MmPciAddress (0,
                PciAddress.Bus,
                PciAddress.Device,
                PciAddress.Function,
                PciAddress.Register
              )
            );

    //
    // Save latest settings to runtime script table
    //
    S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint8,
      *(UINT64 *) &PciAddress,
      1,
      &Data8
      );
  }

  //
  // Save PCI-Host bridge settings (0, 0, 0). 0x90, 94 and 9c are changed by CSM
  // and vital to S3 resume. That's why we put save code here
  //
  Index = 0;
  while (RegTable[Index] != 0xFF) {

    PciAddress.Bus      = RegTable[Index++];
    PciAddress.Device   = RegTable[Index++];
    PciAddress.Function = RegTable[Index++];
    PciAddress.Register = 0;
    PciAddress.ExtendedRegister = 0;

    Data16 = MmioRead16 (
              MmPciAddress (0,
                PciAddress.Bus,
                PciAddress.Device,
                PciAddress.Function,
                PciAddress.Register
              )
            );

    if (Data16 == 0xFFFF) {
      Index+=8;
      continue;
    }

    for (Offset = 0, Mask = 0x01; Offset < 256; Offset+=4, Mask<<=1) {

      if (Mask == 0x00) {
        Mask = 0x01;
      }

      if (RegTable[Index + Offset/32] & Mask ) {

        PciAddress.Register = (UINT8) Offset;
        Data32 = MmioRead32 (MmPciAddress (0, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register));

        //
        // Save latest settings to runtime script table
        //
        S3BootScriptSavePciCfgWrite (
          S3BootScriptWidthUint32,
          *(UINT64 *) &PciAddress,
          1,
          &Data32
        );
      }
    }

    Index += 8;

  }
}
