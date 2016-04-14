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

// LPC Bridge - Device 31, Function 0
// Define the needed LPC registers used by ASL.

scope(\_SB) {
  OperationRegion(ILBR, SystemMemory, \IBAS, 0x8C)
  Field(ILBR, AnyAcc, NoLock, Preserve) {
    Offset(0x08), // 0x08
    PARC,   8,
    PBRC,   8,
    PCRC,   8,
    PDRC,   8,
    PERC,   8,
    PFRC,   8,
    PGRC,   8,
    PHRC,   8,
    Offset(0x14),
    ULKM,   8,    // R_SB_ILB_ULKMC Trap enable
    Offset(0x88), // 0x88
    ,       4,
    UI4E,   1
  }

  Include ("Pch/98Link.asl")
}

  OperationRegion(LPC0, PCI_Config, 0x00, 0xC0)
  Field(LPC0, AnyAcc, NoLock, Preserve)
  {
    Offset(0x08), // 0x08
    SRID,   8,  // Revision ID
    Offset(0x080), // 0x80
    C1EN,   1, // COM1 Enable
    ,      31
  }

  Include ("Platform/Ec.asl")
  Include ("Pch/LpcDev.asl")

  // Define the KBC_COMMAND_REG-64, KBC_DATA_REG-60 Registers as an ACPI Operating
  // Region.  These registers will be used to skip kbd mouse
  // resource settings if not present.

  OperationRegion(PKBS, SystemIO, 0x60, 0x05)
  Field(PKBS, ByteAcc, Lock, Preserve)
  {
    PKBD, 8,
    ,     8,
    ,     8,
    ,     8,
    PKBC, 8
  }

Device(PS2K)  // PS2 Keyboard
{
  Name(_HID,"MSFT0001")
  Name(_CID,EISAID("PNP0303"))

  Method(_STA)
  {
  
  
    //If Port 60/64 trap is enabled, always return device present
    If (And(ULKM, 0x0F)) {
      Return(0x000F) 
    }
    
    // Only report resources to the OS if the Keyboard is present

    If(And(LEqual(PKBD,0xFF), LEqual(PKBC, 0xFF)))
    {
      Return(0x0000)
    }

    Return(0x000F)
  }

  Name(_CRS,ResourceTemplate()
  {
    IO(Decode16,0x60,0x60,0x01,0x01)
    IO(Decode16,0x64,0x64,0x01,0x01)
    IRQ(Edge,ActiveHigh,Exclusive){0x01}
  })

  Name(_PRS, ResourceTemplate() {
    StartDependentFn(0, 0) {
      FixedIO(0x60,0x01)
      FixedIO(0x64,0x01)
      IRQNoFlags(){1}
      }
    EndDependentFn()
  })

}

Device(PS2M)  // PS/2 Mouse
{
  Name(_HID,"MSFT0003")
  Name(_CID,EISAID("PNP0F13"))

  Method(_STA)
  {
  
    //If Port 60/64 trap is enabled, always return device present
    If (And(ULKM, 0x0F)) {
      Return(0x000F) 
    }
    
    // Only report resources to the OS if the Mouse is present

    If(And(LEqual(PKBD,0xFF), LEqual(PKBC, 0xFF)))
    {
      Return(0x0000)
    }

    Return(0x000F)
  }

  Name(_CRS,ResourceTemplate()
  {
    IRQ(Edge,ActiveHigh,Exclusive){0x0C}
  })

  Name(_PRS, ResourceTemplate() {
    StartDependentFn(0, 0) {
      IRQNoFlags(){12}
    }
    EndDependentFn()
  })
}
