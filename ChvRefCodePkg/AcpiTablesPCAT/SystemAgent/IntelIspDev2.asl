/** @file
  ACPI DSDT table
  ISP Exist as B0D2F0 Software ACPI Reference Code for the CherryView Family.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
      //
      // Device ISP0
      //
      Device(ISP0) {
        Name(_ADR, 0x22B8)
        Name(_DDN, "CHV ISP - 808622B8")
        Name(_UID, 0x01)

        Method (_STA, 0, NotSerialized) {
          //
          // Dev2 need report ISP0 as GFX0 child
          //
          If (LAnd(LNotEqual(ISPA, 0), LEqual(ISPD,1)))
          {
            Return (0xF)
          }
          Else
          {
            Return (0x0)
          }
        }
        Name(SBUF,ResourceTemplate () {
          Memory32Fixed (ReadWrite, 0x00000000, 0x00400000, ISP0)
        })
        Method(_CRS, 0x0, NotSerialized) {
          CreateDwordField(^SBUF, ^ISP0._BAS, B0BA)
          Store(ISPA, B0BA)
          Return (SBUF)
        }
        Method (_SRS, 0x1, NotSerialized)
        {
        }
        Method (_DIS, 0x0, NotSerialized)
        {
        }
        Method(_DSM, 0x4, NotSerialized) {
          If (LEqual (Arg0, 0x01)) {
            // Switch ISP to D3H
            Return (0x01)
          }
          Elseif (LEqual (Arg0, 0x02)) {
            // Switch ISP to D0
            Return (0x02)
          }
          Else
          {
            //Do nothing
            Return (0x0F)
          }
        }
      } // End ISP0
