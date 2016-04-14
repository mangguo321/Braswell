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

  External(\PSDE, IntObj)

    //
    // PSS Device
    //
    Device (IMP2)   //Pss Driver I2c
    {
      Name (_ADR, 0)
      Name (_HID, "IMPJ0002")
      Name (_CID, "IMPJ0002")
      Name (_UID, 0x1)

      Method(_CRS, 0x0, NotSerialized)
      {
        Name (SBUF, ResourceTemplate ()
            {
                I2cSerialBus (
                    0x6F,                   //SlaveAddress
                    ControllerInitiated,    //SlaveMode
                    100000,                 //ConnectionSpeed
                    AddressingMode7Bit,     //Addressing Mode
                    "\\_SB.PCI0.I2C2",      //ResourceSource
                    0x00,                   //Descriptor Name
                    ResourceConsumer,
                    ,
                    )
            })
        Return (SBUF)
      }

      Method(_STA, 0x0, NotSerialized)
        {
            If (LEqual(PSDE, 1)) {
                Return(0xF)
            }
            Return(0)
        } //_STA
    } // Device (Pss Chip 1)

    //
    // Pss Chip (Monza 8k Dura)
    //
    Device (IMP3)
    {
        Name (_ADR, 0)                  // _ADR: Address
        Name (_HID, "IMPJ0003")         // _HID: Hardware ID
        Name (_CID, "IMPJ0003")         // _CID: Compatible ID
        Name (_UID, 1)                  // _UID: Unique ID

        Method (_CRS, 0, NotSerialized) // _CRS: Current Resource Settings
        {
            Name (SBUF, ResourceTemplate ()
            {
                I2cSerialBus (
                    0x6E,                   //SlaveAddress
                    ControllerInitiated,    //SlaveMode
                    100000,                 //ConnectionSpeed
                    AddressingMode7Bit,     //Addressing Mode
                    "\\_SB.PCI0.I2C2",      //ResourceSource
                    0x00,                   //Descriptor Name
                    ResourceConsumer,
                    ,
                    )
            })
            Return (SBUF)
        } //_CRS

        Method(_STA, 0x0, NotSerialized)
        {
            If (LEqual(PSDE, 2)) {
                Return(0xF)
            }
            Return(0)
         } //_STA
    } // End of Device IMP3


