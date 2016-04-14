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

   //------------------------
   //  Synaptics Precision touchpad for CR board
   //------------------------
   Device (TPDC)
   {
      Name (_ADR, One)
      Name (_HID, "MSFT0002")
      Name (_CID, "PNP0C50")
      Name (_UID, One)
      Name (_S0W, 3)                                 // Required to put the device to D3 Hot during S0 idle
      Method (_DSM, 0x4, Serialized) {
        Return (CDSM (Arg0, Arg1, Arg2, Arg3, 0x20)) // HID Descriptor Address = 0x20 (IHV Specific)
      }

      Method (_STA, 0, NotSerialized)
      {
        Return (0xF)
      }

      Method (_CRS, 0, Serialized)
      {
        Name (SBFI, ResourceTemplate ()
        {
          I2cSerialBus (
            0x20,                  //SlaveAddress: bus address
            ControllerInitiated,   //SlaveMode: Default to ControllerInitiated
            400000,                //ConnectionSpeed: in Hz
            AddressingMode7Bit,    //Addressing Mode: default to 7 bit
            "\\_SB.PCI0.I2C5",     //ResourceSource: I2C bus controller name
            ,                      //ResourceSourceIndex: defaults to 0
            ,                      //ResourceUsage: Defaults to ResourceConsumer
            ,                      //Descriptor Name: creates name for offset of resource descriptor
          )  //VendorData

          //Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {93} //  SE77, Line1, IRQ93
          GpioInt(Level, ActiveLow, Shared, PullDefault, 0,"\\_SB.GPO3",) {77}   //  SE77, TCH_PAD_INT_N to GPIO_ALERT
        })
        Return (SBFI)
      }
    } // End Device (TPDC)


