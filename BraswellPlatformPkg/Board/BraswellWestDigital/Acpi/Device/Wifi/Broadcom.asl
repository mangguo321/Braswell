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

  External(\WIFD, IntObj)
  External(\_SB.GPO1.AVBL, IntObj)
  External(\_SB.GPO1.CWLE, IntObj)
  External(\_SB.PCI0.SDHB.PSTS, IntObj)

    Device (BRCM) // WIFI For Android
    {
      Name (_ADR, 0x01)                          // SlotNumber + Function
      Name (_DEP, Package() {\_SB.GPO1})
      // Added here for Android Support ++
      Name (_HID, "BCM43241") // _HID: Hardware ID
      Name (_CID, "BCM43241") // _CID: Compatible ID
      // Added here for Android Support --
      Method(_STA, 0x0, NotSerialized)
      {
        If (LEqual(WIFD, 1))
        {
          Return (0x0F)
        }
          Return(0x0)
      }

      Method (_RMV, 0, NotSerialized)
      {
        Return (0x0)
      }

      Name (_PRW, Package() {0, 0})
      Name (_S4W, 2)
      Name (_S0W, 2)

      Method (_CRS, 0, NotSerialized)
      {
        Name (RBUF, ResourceTemplate ()
        {
          Interrupt (ResourceConsumer, Edge, ActiveHigh, ExclusiveAndWake, , , ) {53}        // N21 Line#2 to IOAPIC IRQ 53
          GpioInt(Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0,"\\_SB.GPO1") {21}         // COMBO_WLAN_IRQ to GPIO N21
          GpioIo(Exclusive, PullNone, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {5}     // COMBO_WLAN _R_EN to GPIO N5
          })
          Return (RBUF)
      }

      Method (_PS3, 0, NotSerialized)
      {
        If(LEqual (\_SB.GPO1.AVBL, 1)) {
          Store( 0x00, \_SB.GPO1.CWLE )          // COMBO_WLAN_EN = 0 put WLAN to disabled state
        }
        Store( 0x00, \_SB.PCI0.SDHB.PSTS)        // Indicates that the device is powered off
      }
      Method (_PS0, 0, NotSerialized)
      {
        If(LEqual(\_SB.PCI0.SDHB.PSTS, 0x0))
        {
          If(LEqual (\_SB.GPO1.AVBL, 1)) {
            Store( 0x01, \_SB.GPO1.CWLE )        // COMBO_WLAN_EN = 1 put WLAN to enabled state
          }
          Store( 0x01, \_SB.PCI0.SDHB.PSTS)      // Indicates that the device is powered on
        }
      }

    } // Device (BRCM)
    Device (BRC1) // WIFI for Windows
    {
      Name (_ADR, 0x02)                          // SlotNumber + Function
      Name (_DEP, Package() {\_SB.GPO1})
      Method(_STA, 0x0, NotSerialized)
      {
        If (LEqual(WIFD, 0))
        {
          Return (0x0F)
        }
          Return(0)
      }
      Method (_RMV, 0, NotSerialized)
      {
        Return (0x0)
      }
      Name (_PRW, Package() {0, 0})
      Name (_S4W, 2)
      Name (_S0W, 2)

      Method (_CRS, 0, NotSerialized)
      {
        Name (RBUF, ResourceTemplate ()
        {
          Interrupt (ResourceConsumer, Edge, ActiveHigh, ExclusiveAndWake, , , ) {53}         // N21 Line#2 to IOAPIC IRQ 53
          //GpioInt(Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0,"\\_SB.GPO1") {21}        // COMBO_WLAN_IRQ to GPIO N21
          //GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {5} // COMBO_WLAN _R_EN to GPIO N5
          })
          Return (RBUF)
        }

        Method (_PS3, 0, NotSerialized)
        {
          If(LEqual (\_SB.GPO1.AVBL, 1)) {
            Store( 0x00, \_SB.GPO1.CWLE )        // COMBO_WLAN_EN = 0 put WLAN to disabled state
          }
          Store( 0x00, \_SB.PCI0.SDHB.PSTS)      // Indicates that the device is powered off
        }
        Method (_PS0, 0, NotSerialized)
        {
          If(LEqual(\_SB.PCI0.SDHB.PSTS, 0x0))
          {
            If(LEqual (\_SB.GPO1.AVBL, 1)) {
              Store( 0x01, \_SB.GPO1.CWLE )      // COMBO_WLAN_EN = 1 put WLAN to enabled state
            }
            Store( 0x01, \_SB.PCI0.SDHB.PSTS)    // Indicates that the device is powered on
          }
        }
    } // Device (BRC1)


   //
   // Secondary Broadcom WIFI function
   //
   Device(BRC2)
   {
       Name(_ADR, 0x2) // function 2
       Name(_STA, 0xf)
       //
       // The device is not removable. This must be a method.
       //
       Method(_RMV, 0x0, NotSerialized)
       {
           Return(0x0)
       }

       //
       // Describe a vendor-defined connection between this device and the
       // primary wifi device
       //

       Method(_CRS) {
           Name(NAM, Buffer() {"\\_SB.PCI0.SDHB.BRC1"})
           Name(SPB, Buffer() {
                0x8E,       // SPB Descriptor
                0x1D, 0x00, // Length including NAM above
                0x01,       // +0x00 SPB Descriptor Revision
                0x00,       // +0x01 Resource Source Index
                0xc0,       // +0x02 Bus type - vendor defined
                0x02,       // +0x03 Consumer + controller initiated
                0x00, 0x00, // +0x04 Type specific flags
                0x01,       // +0x06 Type specific revision
                0x00, 0x00  // +0x07 type specific data length
                            // +0x09 - 0xf bytes for NULL-terminated NAM
                            // Length = 0x18
           })

           Name(END, Buffer() {0x79, 0x00})
           Concatenate(SPB, NAM, Local0)
           Concatenate(Local0, END, Local1)
           Return(Local1)
       }
   }
