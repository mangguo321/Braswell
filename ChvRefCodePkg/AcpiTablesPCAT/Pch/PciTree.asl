/** @file
  ACPI DSDT table

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

Scope(\_SB) {

  Name(PR00, Package(){
// Host Bridge
// Mobile IGFX
    Package(){0x0002FFFF, 0, LNKA, 0 },
// IUNIT
    Package(){0x0003FFFF, 0, LNKB, 0 },
// ISH
    Package(){0x000AFFFF, 0, LNKE, 0 },
// PUNIT
    Package(){0x000BFFFF, 0, LNKF, 0 },
// SD Host #0 - eMMC
    Package(){0x0010FFFF, 0, LNKA, 0 },
// SD Host #1 - SDIO
    Package(){0x0011FFFF, 0, LNKB, 0 },
// SD Host #2 - SD Card
    Package(){0x0012FFFF, 0, LNKC, 0 },
// SATA Controller
    Package(){0x0013FFFF, 0, LNKD, 0 },
// xHCI Host
    Package(){0x0014FFFF, 0, LNKE, 0 },
// Low Power Audio Engine
    Package(){0x0015FFFF, 0, LNKF, 0 },
// USB OTG
    Package(){0x0016FFFF, 0, LNKH, 0 },
// LPIO2 DMA
// LPIO2 I2C #4
   Package(){0x0018FFFF, 0, LNKB, 0 },
// LPIO2 I2C #1
// LPIO2 I2C #5
   Package(){0x0018FFFF, 2, LNKD, 0 },
// LPIO2 I2C #2
// LPIO2 I2C #6
   Package(){0x0018FFFF, 3, LNKC, 0 },
// LPIO2 I2C #3
// LPIO2 I2C #7
   Package(){0x0018FFFF, 1, LNKA, 0 },
// SeC
    Package(){0x001AFFFF, 0, LNKF, 0 },
// High Definition Audio Controller
    Package(){0x001BFFFF, 0, LNKG, 0 },
// LPIO1 DMA
// LPIO1 HS-UART #2
    Package(){0x001EFFFF, 0, LNKD, 0 },
// LPIO1 PWM #1
// LPIO1 SPI #1
    Package(){0x001EFFFF, 3, LNKA, 0 },
// LPIO1 PWM #2
// LPIO1 SPI #2
    Package(){0x001EFFFF, 1, LNKB, 0 },
// LPIO1 HS-UART #1
// LPIO1 SPI #3
    Package(){0x001EFFFF, 2, LNKC, 0 },
// LPC Bridge
// SMBus Controller
    Package(){0x001FFFFF, 1, LNKC, 0 },
// PCIE Root Port #1
    Package(){0x001CFFFF, 0, LNKA, 0 },
// PCIE Root Port #2
    Package(){0x001CFFFF, 1, LNKB, 0 },
// PCIE Root Port #3
    Package(){0x001CFFFF, 2, LNKC, 0 },
// PCIE Root Port #4
    Package(){0x001CFFFF, 3, LNKD, 0 },
  })

  Name(AR00, Package(){
// Host Bridge
// Mobile IGFX
    Package(){0x0002FFFF, 0, 0, 16 },
// IUNIT
    Package(){0x0003FFFF, 0, 0, 17 },
// ISH
    Package(){0x000AFFFF, 0, 0, 20 },
// PUNIT
    Package(){0x000BFFFF, 0, 0, 21 },
// SD Host #0 - eMMC
    Package(){0x0010FFFF, 0, 0, 16 },
// SD Host #1 - SDIO
    Package(){0x0011FFFF, 0, 0, 17 },
// SD Host #2 - SD Card
    Package(){0x0012FFFF, 0, 0, 18 },
// SATA Controller
    Package(){0x0013FFFF, 0, 0, 19 },
// xHCI Host
    Package(){0x0014FFFF, 0, 0, 20 },
// Low Power Audio Engine
    Package(){0x0015FFFF, 0, 0, 21 },
// USB OTG
    Package(){0x0016FFFF, 0, 0, 23 },
// LPIO2 DMA
// LPIO2 I2C #4
    Package(){0x0018FFFF, 0, 0, 17 },
// LPIO2 I2C #1
// LPIO2 I2C #5
    Package(){0x0018FFFF, 2, 0, 19 },
// LPIO2 I2C #2
// LPIO2 I2C #6
    Package(){0x0018FFFF, 3, 0, 18 },
// LPIO2 I2C #3
// LPIO2 I2C #7
    Package(){0x0018FFFF, 1, 0, 16 },
// SeC
    Package(){0x001AFFFF, 0, 0, 21 },
// High Definition Audio Controller
    Package(){0x001BFFFF, 0, 0, 22 },
// LPIO1 DMA
// LPIO1 HS-UART #2
    Package(){0x001EFFFF, 0, 0, 19 },
// LPIO1 PWM #1
// LPIO1 SPI #1
    Package(){0x001EFFFF, 3, 0, 16 },
// LPIO1 PWM #2
// LPIO1 SPI #2
    Package(){0x001EFFFF, 1, 0, 17 },
// LPIO1 HS-UART #1
// LPIO1 SPI #3
    Package(){0x001EFFFF, 2, 0, 18 },
// LPC Bridge
// SMBus Controller
    Package(){0x001FFFFF, 1, 0, 18 },
// PCIE Root Port #1
    Package(){0x001CFFFF, 0, 0, 16 },
// PCIE Root Port #2
    Package(){0x001CFFFF, 1, 0, 17 },
// PCIE Root Port #3
    Package(){0x001CFFFF, 2, 0, 18 },
// PCIE Root Port #4
    Package(){0x001CFFFF, 3, 0, 19 },
  })

  Name(PR04, Package(){
// PCIE Port #1 Slot
    Package(){0x0000FFFF, 0, LNKA, 0 },
    Package(){0x0000FFFF, 1, LNKB, 0 },
    Package(){0x0000FFFF, 2, LNKC, 0 },
    Package(){0x0000FFFF, 3, LNKD, 0 },
  })

  Name(AR04, Package(){
// PCIE Port #1 Slot
    Package(){0x0000FFFF, 0, 0, 16 },
    Package(){0x0000FFFF, 1, 0, 17 },
    Package(){0x0000FFFF, 2, 0, 18 },
    Package(){0x0000FFFF, 3, 0, 19 },
  })

  Name(PR05, Package(){
// PCIE Port #2 Slot
    Package(){0x0000FFFF, 0, LNKB, 0 },
    Package(){0x0000FFFF, 1, LNKC, 0 },
    Package(){0x0000FFFF, 2, LNKD, 0 },
    Package(){0x0000FFFF, 3, LNKA, 0 },
  })

  Name(AR05, Package(){
// PCIE Port #2 Slot
    Package(){0x0000FFFF, 0, 0, 17 },
    Package(){0x0000FFFF, 1, 0, 18 },
    Package(){0x0000FFFF, 2, 0, 19 },
    Package(){0x0000FFFF, 3, 0, 16 },
  })

  Name(PR06, Package(){
// PCIE Port #3 Slot
    Package(){0x0000FFFF, 0, LNKC, 0 },
    Package(){0x0000FFFF, 1, LNKD, 0 },
    Package(){0x0000FFFF, 2, LNKA, 0 },
    Package(){0x0000FFFF, 3, LNKB, 0 },
  })

  Name(AR06, Package(){
// PCIE Port #3 Slot
    Package(){0x0000FFFF, 0, 0, 18 },
    Package(){0x0000FFFF, 1, 0, 19 },
    Package(){0x0000FFFF, 2, 0, 16 },
    Package(){0x0000FFFF, 3, 0, 17 },
  })

  Name(PR07, Package(){
// PCIE Port #4 Slot
    Package(){0x0000FFFF, 0, LNKD, 0 },
    Package(){0x0000FFFF, 1, LNKA, 0 },
    Package(){0x0000FFFF, 2, LNKB, 0 },
    Package(){0x0000FFFF, 3, LNKC, 0 },
  })

  Name(AR07, Package(){
// PCIE Port #4 Slot
    Package(){0x0000FFFF, 0, 0, 19 },
    Package(){0x0000FFFF, 1, 0, 16 },
    Package(){0x0000FFFF, 2, 0, 17 },
    Package(){0x0000FFFF, 3, 0, 18 },
  })

//---------------------------------------------------------------------------
// List of IRQ resource buffers compatible with _PRS return format.
//---------------------------------------------------------------------------
// Naming legend:
// RSxy, PRSy - name of the IRQ resource buffer to be returned by _PRS, "xy" - last two characters of IRQ Link name.
// Note. PRSy name is generated if IRQ Link name starts from "LNK".
// HLxy , LLxy - reference names, can be used to access bit mask of available IRQs. HL and LL stand for active High(Low) Level triggered Irq model.
//---------------------------------------------------------------------------
  Name(PRSA, ResourceTemplate(){  // Link name: LNKA
    IRQ(Level, ActiveLow, Shared, LLKA) {3,4,5,6,10,11,12,14,15}
  })
  Alias(PRSA,PRSB)  // Link name: LNKB
  Alias(PRSA,PRSC)  // Link name: LNKC
  Alias(PRSA,PRSD)  // Link name: LNKD
  Alias(PRSA,PRSE)  // Link name: LNKE
  Alias(PRSA,PRSF)  // Link name: LNKF
  Alias(PRSA,PRSG)  // Link name: LNKG
  Alias(PRSA,PRSH)  // Link name: LNKH
//---------------------------------------------------------------------------
// Begin PCI tree object scope
//---------------------------------------------------------------------------

  Device(PCI0) { // PCI Bridge "Host Bridge"
    Name(_HID, EISAID("PNP0A08"))  // Indicates PCI Express/PCI-X Mode2 host hierarchy
    Name(_CID, EISAID("PNP0A03"))  // To support legacy OS that doesn't understand the new HID
    Name(_ADR, 0x00000000)
    Method(^BN00, 0) { return(0x0000) }  // Returns default Bus number for Peer PCI busses. Name can be overriden with control method placed directly under Device scope
    Method(_BBN, 0) { return(BN00()) }  // Bus number, optional for the Root PCI Bus
    Name(_UID, 0x0000)  // Unique Bus ID, optional

    Method(_PRT,0) {
      If(PICM) {Return(AR00)} // APIC mode
      Return (PR00) // PIC Mode
    } // end _PRT

    include("SystemAgent/HostBus.asl")
    Device(LPCB) { // LPC Bridge
      Name(_ADR, 0x001F0000)
      include("Pch/LpcB.asl")
    } // end "LPC Bridge"

  } // end PCI0 Bridge "Host Bridge"
} // end _SB scope
