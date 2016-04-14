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

Scope(\)
{
  //
  // Define ABASE I/O as an ACPI operating region. The base address
  // can be found in Device 31, Registers 40-43h.
  //
  OperationRegion(PMIO, SystemIo, \PMBS, 0x46)
  Field(PMIO, ByteAcc, NoLock, Preserve) {
      ,      8,
      PWBS,  1,    // Power Button Status
    Offset(0x20),
      ,      13,
      PMEB,  1,    // PME_B0_STS
    Offset(0x42),  // General Purpose Control
      ,      1,
      GPEC,  1
  }
  Field(PMIO, ByteAcc, NoLock, WriteAsZeros) {
    Offset(0x20),  // GPE0 Status
      ,      4,
      PSCI,  1,    // PUNIT SCI Status
      SCIS,  1,    // GUNIT SCI Status
      ,      17,
      GP23,  1     
  }

  //
  // Define a Memory Region that will allow access to the PMC
  // Register Block.  Note that in the Intel Reference Solution, the PMC
  // will get fixed up dynamically during POST.
  //
  OperationRegion(PMCR, SystemMemory, \PMCB, 0x100) // PMC Space
  Field(PMCR,DWordAcc,Lock,Preserve) {
    Offset(0x34),  //  Function Disable Register
    L10D,  1,      //  (00) LPIO1 DMA Disable
    L11D,  1,      //  (01) LPIO1 PWM #1 Disable
    L12D,  1,      //  (02) LPIO1 PWM #2 Disable
    L13D,  1,      //  (03) LPIO1 HS-UART #1 Disable
    L14D,  1,      //  (04) LPIO1 HS-UART #2 Disable
    L15D,  1,      //  (05) LPIO1 SPI Disable
    L16D,  1,      //  (06) LPIO1 SPI2 Disable
    L17D,  1,      //  (07) LPIO1 SPI3 Disable
    SD1D,  1,      //  (08) SCC SDIO #1 Disable
    SD2D,  1,      //  (09) SCC SDIO #2 Disable
    SD3D,  1,      //  (10) SCC SDIO #3 Disable
    ,      1,      //  (11)
    ,      1,      //  (12) Azalia Disable
    LPED,  1,      //  (13) LPE Disable
    OTGD,  1,      //  (14) USB OTG Disable
    ,      1,      //  (15)
    ,      1,      //  (16)
    ,      1,      //  (17) SATA Disable
    ,      1,      //  (18) USH Disable
    ,      1,      //  (19) SEC Disable
    RP1D,  1,      //  (20) Root Port 0 Disable
    RP2D,  1,      //  (21) Root Port 1 Disable
    RP3D,  1,      //  (22) Root Port 2 Disable
    RP4D,  1,      //  (23) Root Port 3 Disable
    L20D,  1,      //  (24) LPIO2 DMA Disable
    L21D,  1,      //  (25) LPIO2 I2C #1 Disable
    L22D,  1,      //  (26) LPIO2 I2C #2 Disable
    L23D,  1,      //  (27) LPIO2 I2C #3 Disable
    L24D,  1,      //  (28) LPIO2 I2C #4 Disable
    L25D,  1,      //  (29) LPIO2 I2C #5 Disable
    L26D,  1,      //  (30) LPIO2 I2C #6 Disable
    L27D,  1,      //  (31) LPIO2 I2C #7 Disable
    Offset(0x38),  //  Function Disable Register 2
    ,      1,      //  (00) SMBus Disable
    ,      1,      //  (01)
    ,      1,      //  (02)
    ,      1,      //  (03)
    ISHD,  1,      //  (04) ISH Disable
    ,      27,
    Offset(0x60),  //  PLT_CLK_CTL_0
    CKC0,  2,
    CKF0,  1,
    ,      29,
    Offset(0x64),  //  PLT_CLK_CTL_1
    CKC1,  2,
    CKF1,  1,
    ,      29,
    Offset(0x68),  //  PLT_CLK_CTL_2
    CKC2,  2,
    CKF2,  1,
    ,      29,
    Offset(0x6C),  //  PLT_CLK_CTL_3
    CKC3,  2,
    CKF3,  1,
    ,      29,
    Offset(0x70),  //  PLT_CLK_CTL_4
    CKC4,  2,
    CKF4,  1,
    ,      29,
    Offset(0x74),  //  PLT_CLK_CTL_5
    CKC5,  2,
    CKF5,  1,
    ,      29,
    Offset(0xA0),  //  D3 Status 0
    PMCD,  32,
    Offset(0xC0),  //  xHCI PME Status
    ,      14,
    XPME,  1,
    Offset(0xEC),  //  BIOS_COMM Register
    PMCC,  32
  }

  //
  // Support S0, S3, S4, and S5.  The proper bits to be set when
  // entering a given sleep state are found in the Power Management
  // 1 Control ( PM1_CNT ) register, located at PMC Base + 0xA0
  //
  OperationRegion(PPSC, SystemMemory, Add(\PMCB, 0xA0), 0x08) // PMC Base + 0xA0
  Field(PPSC,DWordAcc,Lock,Preserve) {
    DM1P,  1,  //BIT0
    PW1P,  1,  //BIT1
    PW2P,  1,  //2
    UR1P,  1,  //3
    UR2P,  1,  //4
    SP1P,  1,  //5
    SP2P,  1,  //6
    SP3P,  1,  //7
    EMMP,  1,  //8
    SDI1,  1,  //9
    SDI2,  1,  //10
    ,      2,  //11-mipi, 12-HDA
    LPEP,  1,  //13
    ,      1,  //14 -USB SIP Bridge
    ,      1,  //15
    ,      1,  //16
    SATP,  1,  //17, SATA Power State Current
    USBP,  1,  //18
    SECP,  1,  //19, SEC Power State Current
    PRP1,  1,  //20
    PRP2,  1,  //21
    PRP3,  1,  //22
    PRP4,  1,  //23, PCIE Rootports 1-4
    DM2P,  1,  //24, LPIO2 DMA
    IC1P,  1,  //25
    IC2P,  1,  //26
    IC3P,  1,  //27
    IC4P,  1,  //28
    IC5P,  1,  //29
    IC6P,  1,  //30
    IC7P,  1,  //31
    Offset(0x4), //reg_D3_STS_1_type
    ,      2,  //0-SMBus
    ISHP,  1,  //2
    ,      29
  }
} //end Scope(\)

scope (\_SB.PCI0) {


  //
  // Serial ATA Host Controller - Device 19, Function 0
  //

  Device(SATA) {
    Name(_ADR,0x00130000)
    //
    // SATA Methods pulled in via SSDT.
    //

    OperationRegion(SATR, PCI_Config, 0x74,0x4)
    Field(SATR,WordAcc,NoLock,Preserve) {
      Offset(0x00), // 0x74, PMCR
          ,   8,
      PMEE,   1,    //PME_EN
          ,   6,
      PMES,   1     //PME_STS
    }
    Method(_DSW, 3)
    {
    } // End _DSW
  }


  include("Pch/PchXhci.asl")

  include ("Pch/PchScc.asl")
  include ("Pch/PchLpss.asl")

  //
  // High Definition Audio Controller - Device 27, Function 0
  //
  Device(HDEF) {
    Name(_ADR, 0x001B0000)
    include("Pch/PchAudio.asl")
    Name(_PRW, Package() {0x0D, 4})

    Method(_DSW, 3)
    {
    } // End _DSW
  } // end "High Definition Audio Controller"


  //
  // PCIE Root Port #1
  //
  Device(RP01) {
    Name(_ADR, 0x001C0000)
    include("Pch/PchPcie.asl")
    Name(_PRW, Package() {0x09, 4})

    Method(_PRT,0) {
      If(PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #1"

  //
  // PCIE Root Port #2
  //
  Device(RP02) {
    Name(_ADR, 0x001C0001)
    include("Pch/PchPcie.asl")
    Name(_PRW, Package() {0x09, 4})

    Method(_PRT,0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT

  } // end "PCIE Root Port #2"

  //
  // PCIE Root Port #3
  //
  Device(RP03) {
    Name(_ADR, 0x001C0002)
    include("Pch/PchPcie.asl")
    Name(_PRW, Package() {0x09, 4})

    Method(_PRT,0) {
      If(PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT

  } // end "PCIE Root Port #3"

  //
  // PCIE Root Port #4
  //
  Device(RP04) {
    Name(_ADR, 0x001C0003)
    include("Pch/PchPcie.asl")
    Name(_PRW, Package() {0x09, 4})

    Method(_PRT,0) {
      If(PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT

  } // end "PCIE Root Port #4"

  //
  // SMBus Controller - Device 31, Function 3
  //
  Device(SBUS) {
    Name(_ADR,0x001F0003)
    Include("Pch/PchSmb.asl")
  }

  //
  // Low Power Engine Audio - Device 21, Function 0
  //
  Device(LPEA) {
    Name (_HID, "808622A8")
    Name (_CID, "808622A8")
    Name (_DDN, "Intel(R) Low Power Audio Controller - 808622A8")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00200000, BAR0)  // MMIO 1 - LPE MMIO
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR1)  // MMIO 2 - Shadowed PCI Config Space
      Memory32Fixed (ReadWrite, 0x00000000, 0x00200000, BAR2)  // MMIO 3 - LPE Reserved Memory
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {24}  // LPE IRQ
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {25}  // LPE IRQ
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {26}  // LPE IRQ
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {27}  // LPE IRQ
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {28}  // LPE IRQ
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {29}  // LPE IRQ
      GpioInt(Edge, ActiveBoth, ExclusiveAndWake, PullNone, 0,"\\_SB.GPO3") {77}  // Audio jack interrupt
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      Store(LPE0, B0BA)
      CreateDwordField(^RBUF, ^BAR1._BAS, B1BA)
      Store(LPE1, B1BA)
      CreateDwordField(^RBUF, ^BAR2._BAS, B2BA)
      Store(LPE2, B2BA)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LAnd(LNotEqual(LPE0, 0), LEqual(LPED, 0)))
      {
        Return (0xF)  // Enabled 1111
      }
      Return (0x0)  // Disabled 0000
    }
    Method (_DIS, 0x0, NotSerialized)
    {
      // Add a dummy disable function
    }
    Method (_PS3, 0, NotSerialized)
    {
      OR(PSAT, 0x00000003, PSAT)
      OR(PSAT, 0x00000000, PSAT)
    }

    Method (_PS0, 0, NotSerialized)
    {
      And(PSAT, 0xFFFFFFFC, PSAT)
      OR(PSAT, 0x00000000, PSAT)
    }
    OperationRegion (KEYS, SystemMemory, LPE1, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
    //
    // For Andriod +++<<
    //
    Device (ADMA)
    {
      Name (_HID, "ADMA22A8")  // _HID: Hardware ID
      Name (_CID, "ADMA22A8")  // _CID: Compatible ID
      Name (_DDN, "Intel(R) Audio  DMA0 - DMA0F28")  // _DDN: DOS Device Name
      Name (_UID, One)  // _UID: Unique ID

      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite, 0x00000000, 0x00200000, BAR0)  // MMIO 1 - LPE MMIO
        Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, ,, ) {24}
      })

      Method (_CRS, 0x0, NotSerialized) {
        CreateDwordField(^RBUF, ^BAR0._BAS, B3BA)
        Store(LPE0, B3BA)
        Return (RBUF)
      }

      Method(_STA, 0x0, NotSerialized) {
          If (Lor(LEqual(OSID,2), LEqual(OSID,3))) {
            Return(0xF)
          }
        Return(0x0)
      }
    } // End of Device (ADMA)
  } // End "Low Power Engine Audio"
  
  Device (HAD0)
  {
    Name (_HID, "HAD022A8")
    Name (_CID, "HAD022A8")
    Name (_DDN, "Intel(R) HDMI Audio Driver - HAD")
    Name (_UID, One)
    Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite, // HDMI Audio register offset
                       0x00065800,         // Address Base
                       0x00000140,         // Address Length
                       )
      })
      Return (RBUF)
    }
    Method(_STA, 0x0, NotSerialized) {
      If (LEqual(ADED,1)){
        If (Lor(LEqual(OSID,2), LEqual(OSID,3))) {
          Return(0xF)
        }
      }
      Return(0x0)
    }
  } ///HAD0 end

  //
  // USB3 OTG - Device 22, Function 0
  //
  Device(UOTG) {
    Name (_HID, "808622B7")
    Name (_CID, "808622B7")
    Name (_DDN, "Intel(R) USB On-The-Go Controller - 808622B7")

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00200000, BAR0)  // MMIO 1 - OTG MMIO
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR1)  // MMIO 2 - Shadowed PCI Config Space
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {30}  // OTG IRQ
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {31}  // OTG IRQ
    })

    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      Store(OTG0, B0BA)
      CreateDwordField(^RBUF, ^BAR1._BAS, B1BA)
      Store(OTG1, B1BA)
      If (LNotEqual(OTG0, 0))
      {
        Return (RBUF) 
      }
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(OTGD, 1), LOr(LEqual(OTGM, 1), LEqual(OTG0, 0))))
      {
        Return (0x0)  // Disabled 0000
      }
      Return (0xF)  // Enabled 1111
    }
    Method (_DIS, 0x0, NotSerialized)
    {
      // Add a dummy disable function
    }
    Method (_PS3, 0, NotSerialized)
    {
      OR(PSAT, 0x00000003, PSAT)
      OR(PSAT, 0x00000000, PSAT)
    }
    Method (_PS0, 0, NotSerialized)
    {
      And(PSAT, 0xFFFFFFFC, PSAT)
      OR(PSAT, 0x00000000, PSAT)
    }
    OperationRegion (KEYS, SystemMemory, OTG1, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
  } // End "USB3 OTG"

  //
  // Shadowed PCI Config Space Resource Consumption
  //
  Device(SPRC) {
    Name(_HID,EISAID("PNP0C02"))
    Name(_UID,3)

    Name(RBUF,ResourceTemplate()
    {
      //
      // BIOS specific change.
      // Provide shadowed PCI config base addresses
      // for all the devices that support ACPI mode.
      // BIOS to update AddressBase field dynamically after PCI enumeration.
      //
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, SCB1)  // SCC eMMC BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, SCB2)  // SCC SDIO BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, SCB3)  // SCC SDCard BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L10B)  // LPSS 1 DMA BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L11B)  // LPSS 1 PWM #1 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L12B)  // LPSS 1 PWM #2 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L13B)  // LPSS 1 HSUART #1 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L14B)  // LPSS 1 HSUART #2 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L15B)  // LPSS 1 SPI #1 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L16B)  // LPSS 1 SPI #2 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L17B)  // LPSS 1 SPI #3 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L20B)  // LPSS 2 DMA BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L21B)  // LPSS 2 I2C #1 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L22B)  // LPSS 2 I2C #2 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L23B)  // LPSS 2 I2C #3 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L24B)  // LPSS 2 I2C #4 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L25B)  // LPSS 2 I2C #5 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L26B)  // LPSS 2 I2C #6 BAR1
      Memory32Fixed (ReadWrite, 0x00000000, 0x00000000, L27B)  // LPSS 2 I2C #7 BAR1
    })

    Method (_CRS, 0x0, NotSerialized)
    {
      //
      // Populate BAR Lengths if the BAR addresses are non-zero
      //
      CreateDWordField(^RBUF, ^SCB1._BAS, BR01)
      Store(eM1A, BR01)
      If(LAnd(LNotEqual(BR01, 0), LNotEqual(BR01, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^SCB1._LEN, LN01)
        Store(eM1L, LN01)
      }

      CreateDWordField(^RBUF, ^SCB2._BAS, BR02)
      Store(SI1A, BR02)
      If(LAnd(LNotEqual(BR02, 0), LNotEqual(BR02, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^SCB2._LEN, LN02)
        Store(SI1L, LN02)
      }

      CreateDWordField(^RBUF, ^SCB3._BAS, BR03)
      Store(SD1A, BR03)
      If(LAnd(LNotEqual(BR03, 0), LNotEqual(BR03, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^SCB3._LEN, LN03)
        Store(SD1L, LN03)
      }

      CreateDWordField(^RBUF, ^L10B._BAS, BR04)
      Store(D11A, BR04)
      If(LAnd(LNotEqual(BR04, 0), LNotEqual(BR04, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L10B._LEN, LN04)
        Store(D11L, LN04)
      }

      CreateDWordField(^RBUF, ^L11B._BAS, BR05)
      Store(P11A, BR05)
      If(LAnd(LNotEqual(BR05, 0), LNotEqual(BR05, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L11B._LEN, LN05)
        Store(P11L, LN05)
      }

      CreateDWordField(^RBUF, ^L12B._BAS, BR06)
      Store(P21A, BR06)
      If(LAnd(LNotEqual(BR06, 0), LNotEqual(BR06, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L12B._LEN, LN06)
        Store(P21L, LN06)
      }

      CreateDWordField(^RBUF, ^L13B._BAS, BR07)
      Store(U11A, BR07)
      If(LAnd(LNotEqual(BR07, 0), LNotEqual(BR07, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L13B._LEN, LN07)
        Store(U11L, LN07)
      }

      CreateDWordField(^RBUF, ^L14B._BAS, BR08)
      Store(U21A, BR08)
      If(LAnd(LNotEqual(BR08, 0), LNotEqual(BR08, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L14B._LEN, LN08)
        Store(U21L, LN08)
      }

      CreateDWordField(^RBUF, ^L15B._BAS, BR09)
      Store(SP1A, BR09)
      If(Land(LNotEqual(BR09, 0), LNotEqual(BR09, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L15B._LEN, LN09)
        Store(SP1L, LN09)
      }

      CreateDWordField(^RBUF, ^L16B._BAS, BR10)
      Store(S21A, BR10)
      If(LAnd(LNotEqual(BR10, 0), LNotEqual(BR10, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L16B._LEN, LN10)
        Store(S21L, LN10)
      }

      CreateDWordField(^RBUF, ^L17B._BAS, BR11)
      Store(S31A, BR11)
      If(LAnd(LNotEqual(BR11, 0), LNotEqual(BR11, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L17B._LEN, LN11)
        Store(S31L, LN11)
      }

      CreateDWordField(^RBUF, ^L20B._BAS, BR12)
      Store(D21A, BR12)
      If(LAnd(LNotEqual(BR12, 0), LNotEqual(BR12, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L20B._LEN, LN12)
        Store(D21L, LN12)
      }

      CreateDWordField(^RBUF, ^L21B._BAS, BR13)
      Store(I11A, BR13)
      If(LAnd(LNotEqual(BR13, 0), LNotEqual(BR13, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L21B._LEN, LN13)
        Store(I11L, LN13)
      }

      CreateDWordField(^RBUF, ^L22B._BAS, BR14)
      Store(I21A, BR14)
      If(LAnd(LNotEqual(BR14, 0), LNotEqual(BR14, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L22B._LEN, LN14)
        Store(I21L, LN14)
      }

      CreateDWordField(^RBUF, ^L23B._BAS, BR15)
      Store(I31A, BR15)
      If(LAnd(LNotEqual(BR15, 0), LNotEqual(BR15, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L23B._LEN, LN15)
        Store(I31L, LN15)
      }

      CreateDWordField(^RBUF, ^L24B._BAS, BR16)
      Store(I41A, BR16)
      If(LAnd(LNotEqual(BR16, 0), LNotEqual(BR16, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L24B._LEN, LN16)
        Store(I41L, LN16)
      }

      CreateDWordField(^RBUF, ^L25B._BAS, BR17)
      Store(I51A, BR17)
      If(LAnd(LNotEqual(BR17, 0), LNotEqual(BR17, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L25B._LEN, LN17)
        Store(I51L, LN17)
      }

      CreateDWordField(^RBUF, ^L26B._BAS, BR18)
      Store(I61A, BR18)
      If(LAnd(LNotEqual(BR18, 0), LNotEqual(BR18, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L26B._LEN, LN18)
        Store(I61L, LN18)
      }

      CreateDWordField(^RBUF, ^L27B._BAS, BR19)
      Store(I71A, BR19)
      If(LAnd(LNotEqual(BR19, 0), LNotEqual(BR19, 0xFFFFF000)))
      {
        CreateDWordField(^RBUF, ^L27B._LEN, LN19)
        Store(I71L, LN19)
      }

      Return (RBUF)
    }
  } // Device(SPRC)

} // End scope (\_SB.PCI0)
