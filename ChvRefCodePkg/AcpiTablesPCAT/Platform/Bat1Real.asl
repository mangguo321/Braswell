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

/* Already defined in GloblNvs.asl
Scope(\)
{
  // these fields come from the Global NVS area
  Field(GNVS,AnyAcc,Lock,Preserve)
  {
    Offset(30), // Battery Support Registers:
    BNUM, 8,    //   (30) Battery Number Present
    Offset(32),
    B1SC, 8,    //   (32) Battery 1 Stored Capacity
    Offset(35),
    B1SS, 8,    //   (35) Battery 1 Stored Status
  }
}
*/
  // Define the Real Battery 1 Control Method.

  Device(BAT1)
  {
    Name(_HID,EISAID("PNP0C0A"))

    Name(_UID,1)

    Method(_STA,0)
    {
      If(And(BNUM,1))    // Battery 1 present?
      {
        Return(0x001F)  // Yes.  Show it.
      }

      Return(0x000B)    // No.  Hide it.
    }

    Method(_BIF,0)
    {
      Name(BPKG, Package() {
        0x00000000, // Power Unit = mWh.
        0x00002710,  // 10000 mWh Design Capacity.
        0x00002710,  // 10000 mWh Last Full Charge.
        0x00000001, // Secondary Battery Technology.
        0xFFFFFFFF, // Unknown Design Voltage.
        0x000003E8, // 10% Warning Level.
        0x00000190, // 4% Low Level.
        0x00000064, // 1% Granularity Level 1.
        0x00000064, // 1% Granularity Level 2.
        "CRB Battery 1",
        "Battery 1",
        "Real",
        "-Real Battery 1-"
      })

      // Fix up the design capacity
      If (ECON)
      {
        Store(Multiply(\_SB.PCI0.LPCB.H_EC.B1DC,10), Index(BPKG,1))

        // Fix up the full charge capacity
        Store(Multiply(\_SB.PCI0.LPCB.H_EC.B1FC,10) , Index(BPKG,2))

        // Fix up the design voltage
        Store(\_SB.PCI0.LPCB.H_EC.B1FV, Index(BPKG,4))


  If(\_SB.PCI0.LPCB.H_EC.B1FC)
  {
    Store(Divide(Multiply(\_SB.PCI0.LPCB.H_EC.B1FC, 10),10), Index(BPKG,5))
    Store(Divide(Multiply(\_SB.PCI0.LPCB.H_EC.B1FC, 10),25), Index(BPKG,6))
    Store(Divide(Multiply(\_SB.PCI0.LPCB.H_EC.B1DC, 10),100), Index(BPKG,7))
  }
      }

      Return (BPKG)
    }

    Method(_BIX,0)
    {
      Name(BIXP, Package() {  //Data Package for _BIX
        0x00,       // Integer Revision
        0x00000000, // Power Unit 0 = mWh, 1=mAh.
        0xFFFFFFFF, // Unknown Design Capacity.
        0xFFFFFFFF, // Unknown Last Full Charge.
        0x00000001, // Battery Technology.(0x00 - Primary(Non-Rechargeable), 0x001 - Secondary(Rechargeable))
        0xFFFFFFFF, // Unknown Design Voltage.
        0x00000000, // 10% Warning Level.
        0x00000000, // 4% Low Level.
        0xFFFFFFFF, // Cycle Count, MUST not be 0xFFFFFFFF
        0xFFFFFFFF, // Measurement Accuracy 95%
        0xFFFFFFFF, // Max sampling time, MSFT No specific requirement.
        0xFFFFFFFF, // Min sampling time, MSFT No specific requirement.
        0x00000000, // Max averaging interval, MSFT No specific requirement.
        0x00000000, // Min averaging interval, MSFT No specific requirement.
        0x00000001, // Battery capacity granularity 1
        0x00000001, // Battery capacity granularity 2
        "Harris Beach", // Model number    //String (ASCIIZ)
        "123456789",  //Serial Number    //String (ASCIIZ)
        "LION",      // Battery type    //String (ASCIIZ)
        "Intel SR 1"  // OEM information  //String (ASCIIZ)
      })

      Store(Multiply(\_SB.PCI0.LPCB.H_EC.B1DC,10), Index(BIXP,2))

        // Fix up the full charge capacity
        Store(Multiply(\_SB.PCI0.LPCB.H_EC.B1FC,10) , Index(BIXP,3))

        // Fix up the design voltage
        Store(\_SB.PCI0.LPCB.H_EC.B1FV, Index(BIXP,5))


      If(\_SB.PCI0.LPCB.H_EC.B1FC)
      {
        Store(Divide(Multiply(\_SB.PCI0.LPCB.H_EC.B1FC, 10),10), Index(BIXP,6))
        Store(Divide(Multiply(\_SB.PCI0.LPCB.H_EC.B1FC, 10),25), Index(BIXP,7))
        Store(Divide(Multiply(\_SB.PCI0.LPCB.H_EC.B1DC, 10),100), Index(BIXP,8))
        //Store(0x100,  Index(BIXP,8))
        Store(0x40,   Index(BIXP,9))
        Store(0x320,  Index(BIXP,10))
        Store(0x251C, Index(BIXP,11))
      }

      Return (BIXP)
    }

    Method(_BST,0)
    {
      Name(PKG1,Package() {
        0xFFFFFFFF, // Battery State.
        0xFFFFFFFF, // Battery Present Rate. (in mWh)
        0xFFFFFFFF, // Battery Remaining Capacity. (in mWh)
        0xFFFFFFFF  // Battery Present Voltage. (in mV)
      })

      // Fix up the Battery Status.
      Store(And(B1ST, 0x07),Index(PKG1,0))
      If(And(B1ST, 0x01))
      {
        // Calculate discharge rate
        // Return Rate in mW since we report _BIF data in mW
        Store(Multiply(B1DI, B1FV), Local0)
        Store(Divide(Local0, 1000), Local0)
        Store(Local0, Index(PKG1,1))
      }
      Else
      {
        If(And(B1ST, 0x02))
        {
          // Calculate charge rate
          // Return Rate in mW since we report _BIF data in mW
          Store(Multiply(B1CI, B1FV), Local0)
          Store(Divide(Local0, 1000), Local0)
          Store(Local0, Index(PKG1,1))
        }
      }

      // Calculate Remaining Capacity in mWh =
      // (Remaininng Capacity (mAh) * Design Voltage (mV))/1000
      // Use Remaininng Capacity in mAh multiply with a fixed Design Voltage
      // for Remaininng Capacity in mWh
      Store(Multiply(\_SB.PCI0.LPCB.H_EC.B1RC,10),Index(PKG1,2))

      // Report Battery Present Voltage (mV)
      Store(B1FV, Index(PKG1,3))

      Return(PKG1)
    }

    Method(_BTP,1)
    {
      // arg0 = Trip Point, sent to EC as Threshold.
      // transfer input value from mWh to %
      // TODO
      Return()

    }

    // Return that everything runs off Battery.

    Method(_PCL,0)
    {
      Return(\_SB)
    }
  }

