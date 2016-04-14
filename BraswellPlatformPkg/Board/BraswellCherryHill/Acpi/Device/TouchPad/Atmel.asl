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

  External(\_SB.GPO0, DeviceObj)
  External(\_SB.GPO0.AVBL, IntObj)
  External(\_SB.GPO0.TSR2, IntObj)
  External(\_SB.GPO0.TSRS, IntObj)
  External(\FBID, IntObj)

    // Common _PS3 method for I2C Panel devices
      Method (TPS3, 0, Serialized) {
        If (LEqual (\_SB.GPO0.AVBL, 1)) {
          If (LEqual(FBID, 2)) {             // If it's BSW RVP FAB 2 board
            Store( 0, \_SB.GPO0.TSR2 )             // Reset BSW RVP FAB 2 TOUCH
          } Else {
            Store( 0, \_SB.GPO0.TSRS )             // Reset BSW RVP FAB 2 TOUCH
          }
        }
      }

    // Common _PS0 method for I2C Panel devices
      Method (TPS0, 0, Serialized) {
        If (LEqual (\_SB.GPO0.AVBL, 1)) {
          If (LEqual(FBID, 2)) {             // If it's BSW RVP FAB 2 board
            Store( 1, \_SB.GPO0.TSR2 )             // Reset BSW RVP FAB 2 TOUCH Done
          } Else {
            Store( 1, \_SB.GPO0.TSRS )             // Reset BSW RVP FAB 2 TOUCH Done
          }
        }
        Sleep(100)
      }

    // Common _CRS method for I2C Panel devices
    // Arg0 - Panel's Slave Address
      Method (TCRS, 0x1, Serialized)
      {
        Name (RSTL, ResourceTemplate ()          // Resource for BSW RVP FAB 1 2 & CR board
        {
          I2cSerialBus (
            0x4A,                                // SlaveAddress: bus address
            ControllerInitiated,                 // SlaveMode: Default to ControllerInitiated
            400000,                              // ConnectionSpeed: in Hz
            AddressingMode7Bit,                  // Addressing Mode: default to 7 bit
            "\\_SB.PCI0.I2C6",                   // ResourceSource: I2C bus controller name
            ,                                    // ResourceSourceIndex: defaults to 0
            ,                                    // ResourceUsage: Defaults to ResourceConsumer
            PI2C                                 // Descriptor Name: creates name for offset of resource descriptor
          )                                      // VendorData
          ///Interrupt(ResourceConsumer, Level, ActiveLow, Exclusive, , ,) {59}                    // SW76, Line0, IRQ 59
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO0",,, TRST) {75} // SW75, TOUCH_RST_N
          GpioInt(Level, ActiveLow, Shared, PullDefault, 0, "\\_SB.GPO0", ) {76}                   // SW76, TOUCH_INT_N
        })
        CreateByteField (RSTL, PI2C._ADR, VAL0)  // Panel's Slave Address Connection Descriptor
        Store (Arg0, VAL0)                       // Set Panel's Slave Address from Arg0
        CreateByteField (RSTL, TRST._PIN, VAL1)  // TOUCH_RST_N Connection Descriptor
        If (LEqual (FBID, 2)) {            // If it's BSW RVP FAB2 board
          Store (78, VAL1)                       // SW78 to BSW RVP FAB2 TOUCH_RST_N
        } Else {                                 // If it's BSW RVP FAB1 board
          Store (80, VAL1)                       // SW80 to BSW RVP FAB1 TOUCH_RST_N
        }
        Return (RSTL)
      }

    //------------------------
    //  Atmel Touch Panel using ATMEL7000 over SHARP display
    //  Model name currently not known
    //------------------------
    Device (TSC0)
    {
      Name (_HID, "ATML7000")
      Name (_CID, "PNP0C50")
      Name (HIDA, 0x00)                          // HID Descriptor Address
      Name (_S0W, 3)                             // Put device to 0=D0, 3=D3 Hot, 4=D3 Cold during S0 idle
      Name (_DEP, Package () {\_SB.GPO0})
      Method (_DSM, 0x4, Serialized) {
        Return (CDSM (Arg0, Arg1, Arg2, Arg3, HIDA))
      }
      Method(_STA, 0, NotSerialized) {
        Return (0x0)
      }
      Method (_PS3, 0, Serialized) {
        ^^TPS3 ()
      }
      Method (_PS0, 0, Serialized) {
        ^^TPS0 ()
      }
      Method (_CRS, 0, NotSerialized) {
        Return (^^TCRS (0x4A))                  // Use I2C Slave Adress 0x4A
      }
    }  // End Device (TSC0)

    //------------------------
    //  Atmel Touch Panel using ATMEL1000 over CHEMEI INNOLUX display
    //  Model name is N133HSE - EA1,  resolution 1920 x 1080
    //------------------------
    Device(TCS1)
    {
      Name (_ADR, Zero)
      Name (_HID, "ATML1000")
      Name (_CID, "PNP0C50")
      Name (HIDA, 0x00)                          // HID Descriptor Address
      Name (_UID, One)
      Name (_S0W, 3)                             // Put device to 0=D0, 3=D3 Hot, 4=D3 Cold during S0 idle
      Name (_DEP, Package() {\_SB.GPO0})
      Method (_DSM, 0x4, Serialized) {
        Return (CDSM (Arg0, Arg1, Arg2, Arg3, HIDA))
      }
      Method(_STA, 0, NotSerialized) {
        Return (0x0)
      }
      Method (_PS3, 0, Serialized) {
        ^^TPS3 ()
      }
      Method (_PS0, 0, Serialized) {
        ^^TPS0 ()
      }
      Method (_CRS, 0, NotSerialized) {
        Return (^^TCRS (0x4C))                  // Use I2C Slave Adress 0x4C
      }
    } // End Device(TSC1)

