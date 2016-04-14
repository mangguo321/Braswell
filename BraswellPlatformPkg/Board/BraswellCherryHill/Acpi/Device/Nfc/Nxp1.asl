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

  External(\NFCS, IntObj)

    //
    // NFC Device
    //
    Device (NFC1)
    {
      Name (_ADR, 0x00)
      Name (_HID, "NXP7471")
      Name (_CID, "NXP7471")
      Name (_DDN, "NXP NFC" )
      Name (_UID, 0x01)

      Method (_CRS, 0, NotSerialized) {
        Name(SBUF,ResourceTemplate ()
        {
          I2CSerialBus(
            0x29,               // SlaveAddress: bus address // liuzhi: for PinoleC, it's TI's fuel guage.
            ,                   // SlaveMode: default to ControllerInitiated
            400000,             // ConnectionSpeed: in Hz
            ,                   // Addressing Mode: default to 7 bit
            "\\_SB.PCI0.I2C7",  // ResourceSource: I2C bus controller name
            ,                   // Descriptor Name: creates name for offset of resource descriptor
          )  // VendorData

          GpioInt(Level, ActiveHigh, Exclusive, PullNone, 0, "\\_SB.GPO1", ) {23}           // N23, NFC_HOST_INT
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {27} // N27, NFC_RST_N
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {16} // N16, NFC_ENABLE_N
        })
        Return (SBUF)
      }
      Method (_STA)
      {
        Return (0x0)
      }
    } // End Device (NFC1)


