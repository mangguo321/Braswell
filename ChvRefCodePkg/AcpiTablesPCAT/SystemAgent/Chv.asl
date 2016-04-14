/** @file
  ACPI DSDT table

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

Scope (\_SB.PCI0) {
  //
  // IUNIT - Device 3, Function 0
  //
  Device (ISP3)
  {
    Name (_ADR, 0x00030000)
    Method (_PR3, 0, NotSerialized)
    {
      Return (Package (0x01)
      {
        ID3C
      })
    }

    PowerResource (ID3C, 0x00, 0x0000)
    {
      Method (_STA, 0, NotSerialized)
      {
        If(LEqual(ISPD,2)) {
          Return (0x0F)
        } Else {
          Return (0x0)
        }
      }
      Method (_ON, 0, NotSerialized)
      {
      }
      Method (_OFF, 0, NotSerialized)
      {
      }
    }
  }

    Device(GFX0) { // Mobile I.G.D
      Name(_ADR, 0x00020000)
      Name (_S0W, 3)
      include("SystemAgent/IntelGraphics.asl")
      include("SystemAgent/IntelIspDev2.asl")
   } // end "Mobile I.G.D"
}//end scope
