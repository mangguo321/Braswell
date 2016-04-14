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

  //
  // LPIO1 DMA#1 (Synopsis GP DMA) - Windows
  //
  Device (GDM1) {
    Name (_HID, "INTL9C60")
    Name (_DDN, "Intel(R) DMA Controller #1 - INTL9C60")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00004000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {42}  // DMA #1 IRQ
    })

    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(D10A, B0BA)
      Store(D10L, B0LN)
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized) {
      If (LOr(LEqual(D10A, 0), LEqual(L10D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      Or (PSAT, 3, PSAT)
      Or (PSAT, 0, PSAT)
    }
    Method (_PS0, 0, NotSerialized)
    {
      And (PSAT, 0xFFFFFFFC, PSAT)
      Or (PSAT, 0, PSAT)
    }
    OperationRegion (KEYS, SystemMemory, Or (D10A, 0x84), 4)
    Field (KEYS, DWordAcc, NoLock, Preserve)
    {
      PSAT,   2         // Device Power State, 00 = D0, 01 = D3 Hot
    }
  }

 
  //
  // LPIO1 DMA#2 (Synopsis GP DMA) - Windows
  //
  Device (GDM3) {
    Name (_HID, "INTL9C60")
    Name (_DDN, "Intel(R) DMA Controller #2 - INTL9C60")
    Name (_UID, 2)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00004000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {43}  // DMA #2 IRQ
    })

    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(D20A, B0BA)
      Store(D20L, B0LN)
      Return (RBUF)
    }
 
    Method (_STA, 0x0, NotSerialized) {
      If (LOr(LEqual(D20A, 0), LEqual(L20D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      Or (PSAT, 3, PSAT)
      Or (PSAT, 0, PSAT)
    }
    Method (_PS0, 0, NotSerialized)
    {
      And (PSAT, 0xFFFFFFFC, PSAT)
      Or (PSAT, 0, PSAT)
    }
    OperationRegion (KEYS, SystemMemory, Or (D20A, 0x84), 4)
    Field (KEYS, DWordAcc, NoLock, Preserve)
    {
      PSAT,   2         // Device Power State, 00 = D0, 01 = D3 Hot
    }
  }

  
  //
  // LPIO1 PWM #1
  //
  Device(PWM1) {
    Name (_HID, "80862288")
    Name (_CID, "80862288")
    Name (_DDN, "Intel(R) PWM Controller #1 - 80862288")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(P10A, B0BA)
      Store(P10L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(P10A, 0), LEqual(L11D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, P11A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
  }

  //
  // LPIO1 PWM #2
  //
  Device(PWM2) {
    Name (_HID, "80862288")
    Name (_CID, "80862288")
    Name (_DDN, "Intel(R) PWM Controller #2 - 80862289")
    Name (_UID, 2)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(P20A, B0BA)
      Store(P20L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(P20A, 0), LEqual(L12D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, P21A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
  }

  //
  // LPIO1 HS-UART #1
  //
  Device(URT1) {
    Name (_HID, "8086228A")
    Name (_CID, "8086228A")
    Name (_DDN, "Intel(R) HS-UART Controller #1 - 8086228A")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {39}  // HS-UART #1 IRQ

      FixedDMA(0x2, 0x2, Width32Bit, )
      FixedDMA(0x3, 0x3, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(U10A, B0BA)
      Store(U10L, B0LN)
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(U10A, 0), LEqual(L13D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, U11A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

  } // Device (URT1)

  //
  // LPIO1 HS-UART #2
  //
  Device(URT2) {
    Name (_HID, "8086228A")
    Name (_CID, "8086228A")
    Name (_DDN, "Intel(R) HS-UART Controller #2 - 8086228C")
    Name (_UID, 2)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {40}  // HS-UART #2 IRQ

      FixedDMA(0x4, 0x4, Width32Bit, )
      FixedDMA(0x5, 0x5, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(U20A, B0BA)
      Store(U20L, B0LN)
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(U20A, 0), LEqual(L14D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, U21A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
    
  }//  Device (URT2)

  //
  // LPIO1 SPI
  //
  Device(SPI1) {
    Name (_HID, "8086228E")
    Name (_CID, "8086228E")
    Name (_DDN, "Intel(R) SPI Controller #1 - 8086228E")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {41}  // SPI IRQ

      FixedDMA(0x0, 0x0, Width32Bit, )
      FixedDMA(0x1, 0x1, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(SP0A, B0BA)
      Store(SP0L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(SP0A, 0), LEqual(L15D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, SP1A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
    
  } // Device (SPI1)

  //
  // LPIO1 SPI #2
  //
  Device(SPI2) {
    Name (_HID, "8086228E")
    Name (_CID, "8086228E")
    Name (_DDN, "Intel(R) SPI Controller #2 - 80862290")
    Name (_UID, 2)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {89}  // SPI IRQ

      FixedDMA(0x6, 0x6, Width32Bit, )
      FixedDMA(0x7, 0x7, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(S20A, B0BA)
      Store(S20L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(S20A, 0), LEqual(L16D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, S21A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
  } // Device (SPI2)

  //
  // LPIO1 SPI #3
  //
  Device(SPI3) {
    Name (_HID, "8086228E")
    Name (_CID, "8086228E")
    Name (_DDN, "Intel(R) SPI Controller #3 - 808622AC")
    Name (_UID, 3)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {90}  // SPI IRQ

      FixedDMA(0x8, 0x8, Width32Bit, )
      FixedDMA(0x9, 0x9, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(S30A, B0BA)
      Store(S30L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(S30A, 0), LEqual(L16D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
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
    OperationRegion (KEYS, SystemMemory, S31A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
  } // Device (SPI3)

    // Common _DSM method for HID I2C devices
    // Arg0..3 - original arguments to _DSM method, as passed by OS
    // Arg4 - HID descriptor address to be returned from Function1
    Method (CDSM, 5, Serialized) {
      If (LEqual (Arg0, ToUUID ("3CDFF6F7-4267-4555-AD05-B30A3D8938DE"))) {
        // Function 0 : Query Function
        If (LEqual (Arg2, Zero)) {
          // Revision 1
          If (LEqual (Arg1, One)) {
            Return(Buffer(One) { 3 })
          }
        } ElseIf (LEqual (Arg2, One)) {
          // Function 1 : HID Function
          Return (Arg4) // HID Descriptor Address (IHV Specific)
        }
      }
      Return (Buffer (One) { 0x00 })
    }

  //
  // LPIO2 I2C #1
  //
  Device(I2C1) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #1 - 808622C1")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {32}  // I2C #1 IRQ

      FixedDMA(0x10, 0x0, Width32Bit, )
      FixedDMA(0x11, 0x1, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I10A, B0BA)
      Store(I10L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I10A, 0), LEqual(L21D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I0D3, 1)) {  // If I2C 1 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I0D3, 1)) {  // If I2C 1 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I11A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
    
  } // Device(I2C1)

  //
  // LPIO2 I2C #2
  //
  Device(I2C2) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #2 - 808622C2")
    Name (_UID, 2)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {33}  // I2C #2 IRQ

      FixedDMA(0x12, 0x2, Width32Bit, )
      FixedDMA(0x13, 0x3, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I20A, B0BA)
      Store(I20L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I20A, 0), LEqual(L22D, 1)))
      {
        Return (0x0)
      }
      If (LEqual (BDID, CRRB)) {                // If it's BSW CR board
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I1D3, 1)) {  // If I2C 2 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I1D3, 1)) {  // If I2C 2 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I21A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }
    
  } // Device (I2C2)

  //
  // LPIO2 I2C #3
  //
  Device(I2C3) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #3 - 808622C3")
    Name (_UID, 3)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {34}  // I2C #3 IRQ

      FixedDMA(0x14, 0x4, Width32Bit, )
      FixedDMA(0x15, 0x5, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I30A, B0BA)
      Store(I30L, B0LN)
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I30A, 0), LEqual(L23D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I2D3, 1)) {  // If I2C 3 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I2D3, 1)) {  // If I2C 3 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I31A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

    PowerResource (CLK0, 0x00, 0x0000)
    {
      Method (_STA, 0, NotSerialized)  // _STA: Status
      {
        Return (CKC0)
      }

      Method (_ON, 0, NotSerialized)  // _ON_: Power On
      {
        Store (One, CKC0)
        Sleep(5)
      }

      Method (_OFF, 0, NotSerialized)  // _OFF: Power Off
      {
        Store (0x02, CKC0)
      }
    }

  } // Device(I2C3)

  //
  // LPIO2 I2C #4
  //
  Device(I2C4) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #4 - 808622C4")
    Name (_UID, 4)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {35}  // I2C #4 IRQ

      FixedDMA(0x16, 0x6, Width32Bit, )
      FixedDMA(0x17, 0x7, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I40A, B0BA)
      Store(I40L, B0LN)
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I40A, 0), LEqual(L24D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I3D3, 1)) {  // If I2C 4 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I3D3, 1)) {  // If I2C 4 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I41A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
       Offset (0x84),
       PSAT,   32
    }

    PowerResource (CLK0, 0x00, 0x0000)
    {
      Method (_STA, 0, NotSerialized)  // _STA: Status
      {
        Return (CKC0)
      }

      Method (_ON, 0, NotSerialized)  // _ON_: Power On
      {
        Store (One, CKC0)
        Sleep(5)
      }

      Method (_OFF, 0, NotSerialized)  // _OFF: Power Off
      {
        Store (0x02, CKC0)
      }
    }
    PowerResource (CLK1, 0x00, 0x0000)
    {
      Method (_STA, 0, NotSerialized)  // _STA: Status
      {
        Return (CKC1)
      }

      Method (_ON, 0, NotSerialized)  // _ON_: Power On
      {
        Store (One, CKC1)
        Sleep(5)
      }

      Method (_OFF, 0, NotSerialized)  // _OFF: Power Off
      {
        Store (0x02, CKC1)
      }
    }

  } // End LPIO2 I2C #4

  //
  // LPIO2 I2C #5
  //
  Device(I2C5) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #5 - 808622C5")
    Name (_UID, 5)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {36}  // I2C #5 IRQ

      FixedDMA(0x18, 0x0, Width32Bit, )
      FixedDMA(0x19, 0x1, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I50A, B0BA)
      Store(I50L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I50A, 0), LEqual(L25D, 1)))
      {
        Return (0x0)
      }
      If (LEqual(BDID, CRRB)) {    // If it's BSW CR board
        Return (0xF)
      }
      Return (0)                   // RVP uses this I2C wires as DDI2 DDC SCL & SDA
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I4D3, 1)) {  // If I2C 5 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I4D3, 1)) {  // If I2C 5 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I51A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

  } // Device(I2C5)

  //
  // LPIO2 I2C #6
  //
  Device(I2C6) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #6 - 808622C6")
    Name (_UID, 6)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {37}  // I2C #6 IRQ

      FixedDMA(0x1A, 0x02, Width32Bit, )
      FixedDMA(0x1B, 0x03, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I60A, B0BA)
      Store(I60L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I60A, 0), LEqual(L26D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I5D3, 1)) {  // If I2C 6 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I5D3, 1)) {  // If I2C 6 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I61A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

  } // End Device(I2C6)

  //
  // LPIO2 I2C #7
  //
  Device(I2C7) {
    Name (_HID, "808622C1")
    Name (_CID, "808622C1")
    Name (_DDN, "Intel(R) I2C Controller #7 - 808622C7")
    Name (_UID, 7)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {38}  // I2C #7 IRQ

      FixedDMA(0x1C, 0x4, Width32Bit, )
      FixedDMA(0x1D, 0x5, Width32Bit, )
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(I70A, B0BA)
      Store(I70L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(I70A, 0), LEqual(L27D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }

    Method (_PS3, 0, NotSerialized)
    {
      If (LEqual(I6D3, 1)) {  // If I2C 7 Runtime D3 Enabled
        Or (PSAT, 0x00000003, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    Method (_PS0, 0, NotSerialized)
    {
      If (LEqual(I6D3, 1)) {  // If I2C 7 Runtime D3 Enabled
        And(PSAT, 0xFFFFFFFC, PSAT)
        Or (PSAT, 0x00000000, PSAT)
      }
    }
    OperationRegion (KEYS, SystemMemory, I71A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

  } // End Device(I2C7)

