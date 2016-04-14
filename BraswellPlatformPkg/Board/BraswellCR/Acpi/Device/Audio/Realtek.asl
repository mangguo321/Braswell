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

  External(\CKC3, IntObj)
  External(\LPES, IntObj)

    //
    // Realtek Audio Codec
    //
    Device (RTEK)   //Audio Codec driver I2C
    {
      Name (_ADR, 0)
      Name (_HID, "10EC5670")
      Name (_CID, "10EC5670")
      Name (_DDN, "RTEK Codec Controller " )
      Name (_UID, 1)

      Method(_CRS, 0x0, NotSerialized)
      {
        Name(SBUF,ResourceTemplate ()
        {
          I2CSerialBus(
            0x1C,               // SlaveAddress: bus address
            ,                   // SlaveMode: default to ControllerInitiated
            400000,             // ConnectionSpeed: in Hz
            ,                   // Addressing Mode: default to 7 bit
            "\\_SB.PCI0.I2C2",  // ResourceSource: I2C bus controller name
            ,                   // Descriptor Name: creates name for offset of resource descriptor
          )  //VendorData
         ///Interrupt (ResourceConsumer, Edge, ActiveHigh, ExclusiveAndWake, , , ) {92}  // SE77 Line#0 route to IOAPIC IRQ 92
         GpioInt(Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0,"\\_SB.GPO3") {77} // SE77: GPIO_ALERT from AUDIO_CODEC_INT
        })
        Return (SBUF)
      }

      Method (_PS3, 0, NotSerialized)  // _PS3: Power State 3
      {
        Store (Zero, CKC3)
      }

      Method (_PS0, 0, NotSerialized)  // _PS0: Power State 0
      {
        Store (One, CKC3)
      }

      Method (_DIS, 0, NotSerialized)  // _DIS: Disable Device
      {
      }

      Method(_STA, 0x0, NotSerialized)
      {
        If (LEqual(LPES, 2))           // LPE Audio ACPI Mode = 2
        {
          Return (0xF)
        }
        Return (0x0)
      }

    } // Device (RTEK)
