/** @file
  Intel Corporation - ACPI Reference Code for the Cherryview
  Family of Customer Reference Boards.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


DefinitionBlock (
    "DSDT.aml",
    "DSDT",
    0x02,  // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
    "INTEL", // OEM ID (6 byte string)
    "CHV-SOC", // OEM table ID  (8 byte string)
    0x0 // OEM version of DSDT table (4 byte Integer)
    )

// BEGIN OF ASL SCOPE
{
  External(MDBG, MethodObj)

  Method(ADBG, 1, Serialized)
  {

    Return(0)
  }

// Miscellaneous services enabled in Project
    include ("token.asl")
    include ("GlobalNvs.asl")
    include ("Pch/PciTree.asl")
    include ("Pch/Pch.asl")
    include ("SystemAgent/Chv.asl")
    include ("Cpu/Cpu.asl")
    include ("Platform/Platform.asl")

Scope(\_SB)
  {
    Device(PTID)
    {
      Name(_HID, EISAID("INT340E"))
      Name(_CID, EISAID("PNP0C02"))
      Name(IVER, 0x00030000)      // Version 3
      Name(_STA, 0x00)

      Name(TSDL, Package()
      {  //DeviceClass type  Name of Temperature Value
        0x00000000, "CPU Core 0 DTS", //not from ec
        0x00000000, "CPU Core 1 DTS", //not from ec
        0x00000003, "AmbientTempLocal",
        0x00000003, "AmbientTempRemote1",
        0x00000003, "AmbientTempRemote2",
        0x00000003, "CpuTempLocal",
        0x00000003, "CpuTempRemote",
        0x00000003, "DDRTempLocal",
        0x00000003, "DDRTempRemote",
        0x00000003, "EC Critical temp",
        0x00000003, "OS Critical trip point", // not from EC
        0x00000003, "OS Hot trip point", // not from EC
        0x00000003, "OS Passive trip point", // not from EC
        0x00000003, "OS Active 0 trip point", // not from EC
        0x0000000E, "TZ01 _TMP" // not from EC
      })

      Method(TSDD, 0, Serialized)
      {
        Name (TSDX, Package() {
           0x80000000, 0x80000000, 0x80000000, 0x80000000,
           0x80000000, 0x80000000, 0x80000000, 0x80000000,
           0x80000000, 0x80000000, 0x80000000, 0x80000000,
           0x80000000, 0x80000000, 0x80000000  
        })
         Store(Add(Multiply(\DTS1, 10), 2732), Index(TSDX, 0))
         Store(Add(Multiply(\DTS2, 10), 2732), Index(TSDX, 1))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.ALT1, 10), 2732), Index(TSDX, 2))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.ART1, 10), 2732), Index(TSDX, 3))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.ART2, 10), 2732), Index(TSDX, 4))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.LTMP, 10), 2732), Index(TSDX, 5))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.TMPR, 10), 2732), Index(TSDX, 6))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.D0TL, 10), 2732), Index(TSDX, 7))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.DM0T, 10), 2732), Index(TSDX, 8))
         Store(Add(Multiply(\_SB.PCI0.LPCB.H_EC.CTMP, 10), 2732), Index(TSDX, 9))
         Store(\_TZ.TZ01._CRT, Index(TSDX, 10))
         Store(\_TZ.TZ01._HOT, Index(TSDX, 11))
         Store(\_TZ.TZ01._PSV, Index(TSDX, 12))
         Store(\_TZ.TZ01._AC0, Index(TSDX, 13))
         Store(\_TZ.TZ01._TMP, Index(TSDX, 14))
         Return(TSDX)
      }

      Name(PSDL, Package()
      {// DeviceClass type  Name of Power Value
        0x00000000,  "CPU Power"
      })

      Method(PSDD, 0, Serialized)
      {
        Name (PSDX, Package() {0x80000000})
        Store(100, Index(PSDX, 0))
        Return(PSDX)
      }

      Name(OSDL, Package()
      {  // DeviceClass type  Descriptive Name  Unit
        0x00000004, "CPU Fan Duty Cycle", "RAW",
        0x00000004, "CPU Fan #1 Speed", "RPM"
      })

      Method(OSDD, 0, Serialized)
      {
        Name (OSDX, Package () {0x80000000, 0x80000000})
        Store(\_SB.PCI0.LPCB.H_EC.FDCY, Index(OSDX, 0))
        Store(\_SB.PCI0.LPCB.H_EC.CFSP, Index(OSDX, 1))
        Return(OSDX)
      }

      Method (SDSP)
      {
        Return(10)      // Sampling period
      }
    }//end of Device(PTID)
  } // end \_SB scope

    include ("Platform/Thermal.asl")
    include ("Platform/PciDrc.asl")
    include ("SystemAgent/Video.asl")
    include ("Platform/Gpe.asl")

// Sleep states supported by Chipset/Board.
//----------------------------------------------------------------------
// SSx - BIOS setup controlled enabled _Sx Sleep state status
    Name(\_S0, Package(4){0x0,0x0,0,0}) // mandatory System state
    Name(\_S3, Package(4){0x5,0x0,0,0})
    //Name(\_S4, Package(4){0x6,0x0,0,0})
    Name(\_S5, Package(4){0x7,0x0,0,0}) // mandatory System state

    Method(PTS, 1) {        // METHOD CALLED FROM _PTS PRIOR TO ENTER ANY SLEEP STATE
        If(Arg0)            // entering any sleep state
        {
        }
    }
    Method(WAK, 1) {        // METHOD CALLED FROM _WAK RIGHT AFTER WAKE UP
    }
}// End of ASL File
