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

    Device(FPNT)
    {
      Name(_HID, "AUTH2750") // AuthenTec AES2750
      Name(_DDN, "AuthenTec AES2750")
      Method(_CRS, 0x0, NotSerialized)
      {
        // SpiSerial Bus Connection Descriptor
        Name(UBUF, ResourceTemplate () {
          SPISerialBus(
            1,                    // Device selection
            PolarityLow,          // Device selection polarity
            FourWireMode,         // wiremode
            8,                    // databit len
            ControllerInitiated,  // slave mode
            8000000,              // Connection speed
            ClockPolarityLow,     // Clock polarity
            ClockPhaseSecond,     // clock phase
            "\\_SB.PCI0.SPI1",    // ResourceSource: SPI bus controller name
            0,                    // ResourceSourceIndex
            ResourceConsumer,     // Resource usage
            ,                     // DescriptorName: creates name for offset of resource descriptor
          )                       // Vendor Data
          Interrupt (ResourceConsumer, Edge, ActiveHigh, Exclusive, , , ) {72}
          //GpioInt(Edge, ActiveHigh, Exclusive, PullNone, 0, "\\_SB.GPO2", ) {1}
        })
        Return (UBUF)
      }
      Method (_STA, 0x0, NotSerialized)
      {
        Return (0x0)
      }
    } // Device(FPNT)

