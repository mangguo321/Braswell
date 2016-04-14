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
  // eMMC (ACPI Mode)
  //
  Device(SDHA) {
    Name (_HID, "80860F14")
    Name (_CID, "PNP0D40")
    Name (_DDN, "Intel(R) eMMC Controller - 80862294")
    Name (_UID, 1)

    Name (RBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {45}  // eMMC IRQ
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^RBUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^RBUF, ^BAR0._LEN, B0LN)
      Store(eM0A, B0BA)
      Store(eM0L, B0LN)
      Return (RBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(eM0A, 0), LEqual(SD1D, 1)))
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
    OperationRegion (KEYS, SystemMemory, eM1A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,  32
    }

    Method (_DIS, 0x0, NotSerialized)
    {
      // Adding dummy disable methods for device EMM0
    }

    Device (EMMD)
    {
      Name (_ADR, 0x00000008) // Slot 0, Function 8
      Method (_RMV, 0, NotSerialized)
      {
        Return (0x0)
      }
    }
  } // Device(SDHA)

  //
  // eMMC (PCI Mode)
  //
  Device(PEMC) {
    Name(_ADR, 0x00100000)
    OperationRegion(SDIO, PCI_Config, 0x84, 0x4)
    Field(SDIO,WordAcc,NoLock,Preserve)
    {
      Offset(0x00), // 0x84, PMCR
      ,   8,
      PMEE,   1,    //PME_EN
      ,   6,
      PMES,   1     //PME_STS
    }

    Method (_STA, 0x0, NotSerialized)
    {
      If (LAnd(LEqual(eM0A, 0), LEqual(SD1D, 0)))
      {
        Return(0xF)
      }
      Else
      {
        Return(0x0)
      }
    }

    Method(_DSW, 3)
    {
    } // End _DSW

    Device (CARD)
    {
      Name (_ADR, 0x00000008)
      Method(_RMV, 0x0, NotSerialized)
      {
        Return (0)
      } // End _RMV
    }
  } // Device(PEMC)

  //
  // SDIO
  //
  Device(SDHB) {
    Name(_ADR, 0x00110000)
    Name (_HID, "INT33BB")
    Name (_CID, "PNP0D40")
    Name (_DDN, "Intel(R) SDIO Controller - 80862295")
    Name (_UID, 2)
    Name (_HRV, 2)
    Name (_DEP, Package() {\_SB.GPO1})
    Name (PSTS, 0x0)

    Name (ABUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {46}  // SDIO IRQ
      // Added here for Android Support ++
      GpioIo(Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {5}     // COMBO_WLAN _R_EN to GPIO N5
      // Added here for Android Support --
    })

    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^ABUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^ABUF, ^BAR0._LEN, B0LN)
      Store(SI0A, B0BA)
      Store(SI0L, B0LN)
      Return(ABUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(SI0A, 0), LEqual(SD2D, 1)))
      {
        Return (0x0)
      }
      Return (0xF)
    }
    Method (_DIS, 0x0, NotSerialized)
    {
      // Adding dummy disable methods for device EMM0
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
        If(LEqual(\_SB.PCI0.SDHB.PSTS, 0x0))
        {
          If(LEqual (\_SB.GPO1.AVBL, 1)) {
            Store( 0x01, \_SB.GPO1.CWLE )        // COMBO_WLAN_EN = 1 put WLAN to enabled state
          }
          Store( 0x01, \_SB.PCI0.SDHB.PSTS)      // Indicates that the device is powered on
        }
    }
    OperationRegion (KEYS, SystemMemory, SI1A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

  } // Device(SDHB)

  //
  // SD Card
  //
  Device(SDHC) {
    Name(_ADR, 0x00120000)
    Name (_HID, "80860F14")
    Name (_CID, "PNP0D40")
    Name (_HRV, 0x0001)
    Name (_DDN, "Intel(R) SD Card Controller - 80862296")
    Name (_UID, 3)
    Name (ABUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BAR0)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {47}  // SD Card IRQ
      GpioInt(Edge, ActiveBoth, SharedAndWake, PullNone, 10000, "\\_SB.GPO3", 0, ResourceConsumer, , ) {81}  // SE81
      GpioIO(Shared, PullDefault, 0, 0, IoRestrictionInputOnly, "\\_SB.GPO3", 0, ResourceConsumer, , ) {81}  // SE81
      GpioIO(Shared, PullDefault, 0, 0, IoRestrictionInputOnly, "\\_SB.GPO3", 0, ResourceConsumer, , ) {85}  // SE85
      GpioIO(Shared, PullDefault, 0, 0, IoRestrictionInputOnly, "\\_SB.GPO3", 0, ResourceConsumer, , ) {78}  // SE78
    })
    Name (WBUF, ResourceTemplate ()
    {
      Memory32Fixed (ReadWrite, 0x00000000, 0x00001000, BARA)
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {47}  // SD Card IRQ
      GpioInt(Edge, ActiveBoth, SharedAndWake, PullNone, 10000, "\\_SB.GPO3", 0, ResourceConsumer, , ) {81}  // SE81
      GpioIO(Shared, PullDefault, 0, 0, IoRestrictionInputOnly, "\\_SB.GPO3", 0, ResourceConsumer, , ) {81}  // SE81
    })
    Method (_CRS, 0x0, NotSerialized)
    {
      CreateDwordField(^ABUF, ^BAR0._BAS, B0BA)
      CreateDwordField(^ABUF, ^BAR0._LEN, B0LN)
      Store(SD0A, B0BA)
      Store(SD0L, B0LN)
      CreateDwordField(^WBUF, ^BARA._BAS, B01A)
      CreateDwordField(^WBUF, ^BARA._LEN, B01N)
      Store(SD0A, B01A)
      Store(SD0L, B01N)
       //CreateDwordField(^RBUF, BAR1._BAS, B1BA)
       //CreateDwordField(^RBUF, BAR1._LEN, B1LN)
       //Store(SD1A, B1BA)
       //Store(SD1L, B1LN)
       If (LEqual(OSID,2))
       {
         Return (ABUF)
       }
       Return(WBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
      If (LOr(LEqual(SD0A, 0), LEqual(SD3D, 1)))
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
      Sleep (1000)	
    }
    OperationRegion (KEYS, SystemMemory, SD1A, 0x100)
    Field (KEYS, DWordAcc, NoLock, WriteAsZeros)
    {
      Offset (0x84),
      PSAT,   32
    }

    Method(_DSM, 0x4, NotSerialized)
    {
      If(LEqual(Arg0, Buffer(0x10) {
        0xa5, 0x3e, 0xc1, 0xf6, 0xcd, 0x65, 0x1f, 0x46, 0xab, 0x7a, 0x29, 0xf7,
        0xe8, 0xd5, 0xbd, 0x61
      })) {
        If (LEqual (Arg1, Zero)) {
          If (LEqual (Arg2, Zero)) {
            Return (Buffer (0x4)
            {
              0x1F,0x0,0x0,0x0
            })
          }
          If (LEqual (Arg2, One)) {
            //
            // Clear Power Control Register
            //
            Sleep (5)                            // Delay for Clear +VSDIO Power Control
            Return (One)
          }
          If (LEqual (Arg2, 2)) {
            //
            // Set Power Control Register
            //
            Return (0x2)
          }
          If (LEqual (Arg2, 3)) {
            //
            // Set 1.8v signalling
            //
            Sleep (VS18)                         // Delay for 1.8 Volt +VSDIO switching
            Return (0x3)
          }
          If (LEqual (Arg2, 4)) {
            //
            // Set 3.3v signalling
            //
            Sleep (VD33)                         // Delay for 3.3 Volt +VSDIO switching
            Return (0x4)
          }
        } Else {
          Return (Zero)
        }
        Return (Zero)
      }
    }
  } // Device (SDHC)
