/** @file
  ACPI DSDT table

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

  Scope(\_SB)
  {
    //
    // Dummy power resource for USB D3 cold support
    //
    PowerResource(USBC, 0, 0)
    {
      Method(_STA) { Return (0xF) }
      Method(_ON) {}
      Method(_OFF) {}
    }
  }

  //
  // xHCI Controller - Device 20, Function 0
  //
  Device(XHC1) {
    Name(_ADR, 0x00140000)
    Name(_PRW, Package() {0x0D, 4})
    Name (_S3D, 2)
    Name (_S4D, 2)
    Name (XFLT, 0x0)

    Method (_PS3, 0, NotSerialized)
    {
    }

    Method (_PS0, 0, NotSerialized)
    {
      Store (1, PMEB)
    }

    Method(_DSM, 0x4, Serialized)
    {
      //
      // GUID Match
      //
      If(LEqual(Arg0, Buffer(0x10)
      {
        0xb7, 0x0c, 0x34, 0xac, 0x01, 0xe9, 0xbf, 0x45, 0xb7, 0xe6, 0x2b, 0x34,
        0xec, 0x93, 0x1e, 0x23
      }))
      {
        //
        // Only Supported value is "0x3"
        //
        If(LEqual(Arg1, 0x3))
        {
          Store(Arg1,XFLT)
        }
      }
    }

    Method(_S0W, 0x0, NotSerialized)
    {
      If (\_OSI("Android"))        // FOR AOS case Return D3
      {
        Return (0x3)
      }
      If (LEqual(XFLT, Zero))      // Filter Driver will set XFLT for WOS
      {
        Return (0x0)
      } Else {
        Return (0x3)
      }
    }

    OperationRegion (USBR, PCI_Config, 0x00, 0x98)
    Field (USBR, WordAcc, NoLock, Preserve)
    {
      Offset (0x10),
      BAR0,    32,
      Offset (0x74),
        ,      8,
        PMEE,  1,
            ,  6,
        PMES,  1
    }
    Method (_RMV, 0, NotSerialized)  // _RMV: Removal Status
    {
      Return (Zero)
    }

    Method (_PR3, 0, NotSerialized)  // _PR3: Power Resources for D3hot
    {
      Return (Package (0x01)
      {
        USBC
      })
    }

    Device(RHUB)
    {
      Name(_ADR, Zero)

      //
      // High Speed Ports
      //
      Device(HS01) // Pair with SSP1
      {
        Name(_ADR, 0x01)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0x03,  // USB 3 Standard-A connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x61, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS01

      Device(HS02) // Pair with SSP2
      {
        Name(_ADR, 0x02)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0x03,  // USB 3 Standard-A connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x61, 0x0C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS02

      Device(HS03) // Pair with SSP3
      {
        Name(_ADR, 0x03)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0xFF,  // Proprietary connector, also set _PLD bit64 User Visible to 0.
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS03

      Device(HS04) // Pair with SSP4
      {
        Name(_ADR, 0x04)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0xFF,  // Proprietary connector, also set _PLD bit64 User Visible to 0.
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS04

      Device(HS05)
      {
        Name(_ADR, 0x05)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0x00,  // Type 'A' connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x61, 0x0C, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS05

      Device(HS06) // HSIC Port, pair with SSP5
      {
        Name(_ADR, 0x06)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0xFF,  // Proprietary connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS06

      Device(HS07) // HSIC Port, pair with SSP6
      {
        Name(_ADR, 0x07)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero
            0xFF,  // Proprietary connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of HS07

      //
      // Super Speed Ports - must match _UPC declarations of the coresponding High Speed Ports.
      //
      Device(SSP1) // Pair with HS01
      {
        Name(_ADR, 0x08)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero.
            0x03,  // USB 3 Standard-A connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x61, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of SSP1

      Device(SSP2) // Pair with HS02
      {
        Name(_ADR, 0x09)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero.
            0x03,  // USB 3 Standard-A connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x61, 0x0C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of SSP2

      Device(SSP3) // Pair with HS03
      {
        Name(_ADR, 0x0A)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero.
            0xFF,  // Proprietary connector, also set _PLD bit64 User Visible to 0.
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of SSP3

      Device(SSP4) // Pair with HS04
      {
        Name(_ADR, 0x0B)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero.
            0xFF,  // Proprietary connector, also set _PLD bit64 User Visible to 0.
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of SSP4

      Device(SSP5) // SSIC Port, pair with HS06
      {
        Name(_ADR, 0x0C)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero.
            0xFF,  // Proprietary connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of SSP5

      Device(SSP6) // SSIC Port, pair with HS07
      {
        Name(_ADR, 0x0D)
        Method(_UPC,0,Serialized) {
          Name(UPCP, Package() {
            0xFF,  // Port is connectable if the value is non-zero.
            0xFF,  // Proprietary connector
            0x00,
            0x00
          })
          Return(UPCP)
        }

        Method(_PLD,0,Serialized) {
          Name(PLDP, Package() {
            Buffer(0x10)
            {
              0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x60, 0x0C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00
            }
          })
          Return (PLDP)
        }
      } // end of SSP6

    } // end of RHUB
  }  // end of XHC1
