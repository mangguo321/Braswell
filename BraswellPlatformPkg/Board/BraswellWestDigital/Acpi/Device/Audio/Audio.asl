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

  //<<<<Additional Audio Modules --For Android
  Device (AMCR)
  {
    Name (_HID, "AMCR22A8")  // _HID: Hardware ID
    Name (_CID, "AMCR22A8")  // _CID: Compatible ID
    Name (_DDN, "Intel(R) Audio Machine Driver - AMCR0F28")  // _DDN: DOS Device Name
    Name (_UID, One)  // _UID: Unique ID
    Name (_DEP, Package (0x02)  {\_SB.GPO2, \_SB.PCI0.I2C2.RTEK})
    Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
    {
      Name (RBUF, ResourceTemplate ()
      {
        // Codec GPIO
        GpioInt (Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0x0000,
                 "\\_SB.GPO3", 0x00, ResourceConsumer, ,
                 )
                 {   // Pin list
                     77              // AUDIO_CODEC_INT to SoC GPIO_ALERT(SE77)
                 }
        // Dock GPIO
        GpioInt (Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0x0000,
                 "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                )
                {   // Pin list
                    0x0006           //
                }
        // Jack GPIO
        GpioInt (Edge, ActiveHigh, ExclusiveAndWake, PullNone, 0x0000,
                 "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                )
                {   // Pin list
                    0x0006           // JACK_DET_FB to SoC GPIO_DFX8(N06), un-staff R1H5 & staff R1H6
                }
        })
        Return (RBUF)
      }
      Method(_STA, 0x0, NotSerialized) {
        If (LEqual(LPES, 2))         // LPE Audio ACPI Mode = 2
        {
          Return (0xF)
        }
        Return (0x0)
      }
  }
 /////Additional Audio Modules --For Android >>>>
