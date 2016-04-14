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

Device(CHVC)
{
  Name(_ADR, 0x00000000)           // Device 0, Function 0
  //
  // Define various MCH Controller PCI Configuration Space
  // registers which will be used to dynamically produce all
  // resources in the Host Bus _CRS.
  //
  OperationRegion(HBUS, PCI_Config, 0x00, 0xFF)
  Field(HBUS, DWordAcc, NoLock, Preserve)
  {
    Offset(0xD0),
    SMCR,   32,             // Message Control Register (0xD0)
    Offset(0xD4),
    SMDR,   32,             // Message Data Register (0xD4)
    Offset(0xD8),
    MCRX,   32,             // Message Control Register Extension (0xD8)
    Offset(0xDC),
    MCXX,   32,             // Message Control Register Double Extension (0xDC)
  }
  //
  // Define a method to read a 32-bit register on the Message bus.
  //  Arg0 = Port
  //  Arg1 = Register
  //
  //  Returns 32-bit register value
  //
  Method(RMBR, 2, Serialized)
  {
    //
    // Initiate regsiter read message on Message Bus MCR
    //
    Or(ShiftLeft(Arg0, 16), ShiftLeft(Arg1, 8), Local0)
    Or(0x100000F0, Local0, SMCR)
    //
    // Read register value from Message Data Register
    //
    Return(SMDR)
  }

  //
  // Define a method to write a 32-bit register on the Message bus MDR.
  //  Arg0 = Port
  //  Arg1 = Register
  //  Arg2 = 32-bit value
  //
  Method(WMBR, 3, Serialized)
  {
    //
    // Write register value to Message Data Register
    //
    Store(Arg2, SMDR)
    //
    // Initiate register write message on Message Bus
    //
    Or(ShiftLeft(Arg0, 16), ShiftLeft(Arg1, 8), Local0)
    Or(0x110000F0, Local0, SMCR)
  }
  //
  // Define an extended IOSF-SB access method to read a 32-bit message bus register.
  //  Arg0 = Port
  //  Arg1 = Register
  //  Arg2 = ReadOpcode
  //  Arg3 = Bar
  //  Arg4 = Device
  //  Arg5 = Function
  //
  //  Returns 32-bit register value
  //
  Method(RMBX, 6, Serialized)
  {
    //
    // Initiate regsiter read message on Message Bus MCR
    // (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF))
    //
    And(Arg3, 0x07, Local4)               // (Bar & 0x07)
    Store(ShiftLeft(Local4, 8), Local4)   // ((Bar & 0x07) << 8)
    Or(ShiftLeft(Arg4, 3), Arg5, Local5)  // ((Device << 3) | (Function))
    And(Local5, 0xFF, Local5)             // (((Device << 3) | (Function)) & 0xFF)
    Or(Local4, Local5, MCXX)
    And(Arg1, 0xFFFFFF00, Local3)
    Store(Local3, MCRX)
    //
    // Perform the Operation:
    // (ReadOpCode << 24) | (PortId << 16) | ((Register & 0x000000FF) << 8) | 0xF0
    //
    And(Arg1, 0x000000FF, Local0)
    Or(ShiftLeft(Arg2, 24), ShiftLeft(Arg0, 16), Local1)
    And(Local1, 0xFFFF0000, Local1)
    Or(ShiftLeft(Local0, 8), 0xf0, Local2)
    And(Local2, 0x0000FFFF, Local2)
    Or(Local1, Local2, SMCR)
    //
    // Read register value from Message Data Register
    //
    Return(SMDR)
  }

  //
  // Define an extended IOSF-SB access method to write a 32-bit message bus register.
  //  Arg0 = Port
  //  Arg1 = Register
  //  Arg2 = 32-bit value
  //  Arg3 = WriteOpcode
  //  Arg4 = Bar
  //  Arg5 = Device
  //  Arg6 = Function
  //
  Method(WMBX, 7, Serialized)
  {
    //
    // Write register value to Message Data Register
    // (((Bar & 0x07) << 8) | (((Device << 3) | (Function)) & 0xFF))
    //
    And(Arg4, 0x07, Local4)               // (Bar & 0x07)
    Store(ShiftLeft(Local4, 8), Local4)   // ((Bar & 0x07) << 8)
    Or(ShiftLeft(Arg5, 3), Arg6, Local5)  // ((Device << 3) | (Function))
    And(Local5, 0xFF, Local5)             // (((Device << 3) | (Function)) & 0xFF)
    Or(Local4, Local5, MCXX)
    And(Arg1,0xFFFFFF00, Local3)
    Store(Local3, MCRX)
    Store(Arg2, SMDR)
    //
    // Initiate register write message on Message Bus
    // (WriteOpCode << 24) | (PortId << 16) | ((Register & 0x000000FF) << 8) | 0xF0
    //
    And(Arg1,0x000000FF, Local0)
    Or(ShiftLeft(Arg3, 24), ShiftLeft(Arg0, 16), Local1)
    And(Local1,0xFFFF0000, Local1)
    Or(ShiftLeft(Local0, 8), 0xf0, Local2)
    And(Local2, 0x0000FFFF, Local2)
    Or(Local1, Local2, SMCR)
  }
}

//
// BUS, I/O, and MMIO resources
//
Method(_CRS,0,Serialized) {

  Name(FTSZ,0x100000)                     // FTPM size

  //
  // Update LPEA reserved memory
  //
  CreateDwordField(RES0, ^LPA0._MIN,LPMN)
  CreateDwordField(RES0, ^LPA0._MAX,LPMX)
  CreateDwordField(RES0, ^LPA0._LEN,LPLN)
  If (LAnd(LNotEqual(LPE2, 0), LEqual(LPED, 0))) {
    Store (LPE2, LPMN)
    Add (LPMN, LPLN, LPMX)
    Subtract(LPMX, 1, LPMX)
  } Else {
    Store (0, LPMN)
    Store (0, LPMX)
    Store (0, LPLN)
  }

  //Update ISP0 reserved memory
  CreateDwordField(RES0, ^ISP0._MIN,ISMN)
  CreateDwordField(RES0, ^ISP0._MAX,ISMX)
  CreateDwordField(RES0, ^ISP0._LEN,ISLN)
  If (LEqual(ISPD,1)){
    Store (ISPA, ISMN)
    Add (ISMN, ISLN, ISMX)
    Subtract(ISMX, 1, ISMX)
  } Else {
    Store (0, ISMN)
    Store (0, ISMX)
    Store (0, ISLN)
  }

  ////PCI MMIO SPACE
  CreateDwordField(RES0, ^PM01._MIN,M1MN)
  CreateDwordField(RES0, ^PM01._MAX,M1MX)
  CreateDwordField(RES0, ^PM01._LEN,M1LN)

  ///Get dBMBOUND Base
  And(BMBD, 0xFF000000, M1MN)

  // Calculate Length
  Add(Subtract(M1MX, M1MN), 1, M1LN)


  Return(RES0)
}

Name( RES0,ResourceTemplate() {
  WORDBusNumber (          // Bus number resource (0); the bridge produces bus numbers for its subsequent buses
          ResourceProducer,      // bit 0 of general flags is 1
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          PosDecode,             // PosDecode
          0x0000,                // Granularity
          0x0000,                // Min
          0x00FF,                // Max
          0x0000,                // Translation
          0x0100                 // Range Length = Max-Min+1
          )

  IO (Decode16, 0x70, 0x77, 0x01, 0x08)         //Consumed resource (0xCF8-0xCFF)
  IO (Decode16, 0xCF8, 0xCF8, 0x01, 0x08)       //Consumed resource (0xCF8-0xCFF)

  WORDIO (                 // Consumed-and-produced resource (all I/O below CF8)
          ResourceProducer,      // bit 0 of general flags is 0
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          PosDecode,
          EntireRange,
          0x0000,                // Granularity
          0x0000,                // Min
          0x006F,                // Max
          0x0000,                // Translation
          0x0070                 // Range Length
          )

  WORDIO (                 // Consumed-and-produced resource
          ResourceProducer,      // bit 0 of general flags is 0
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          PosDecode,
          EntireRange,
          0x0000,                // Granularity
          0x0078,                // Min
          0x0CF7,                // Max
          0x0000,                // Translation
          0x0C80                 // Range Length
          )

  WORDIO (                 // Consumed-and-produced resource (all I/O above CFF)
          ResourceProducer,      // bit 0 of general flags is 0
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          PosDecode,
          EntireRange,
          0x0000,                // Granularity
          0x0D00,                // Min
          0xFFFF,                // Max
          0x0000,                // Translation
          0xF300                 // Range Length
          )

  DWORDMEMORY (            // Descriptor for legacy VGA video RAM
          ResourceProducer,      // bit 0 of general flags is 0
          PosDecode,
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          Cacheable,
          ReadWrite,
          0x00000000,            // Granularity
          0x000A0000,            // Min
          0x000BFFFF,            // Max
          0x00000000,            // Translation
          0x00020000             // Range Length
          )

  DWORDMEMORY (            // Descriptor for legacy OptionRom
          ResourceProducer,      // bit 0 of general flags is 0
          PosDecode,
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          Cacheable,
          ReadWrite,
          0x00000000,            // Granularity
          0x000C0000,            // Min
          0x000DFFFF,            // Max
          0x00000000,            // Translation
          0x00020000             // Range Length
          )

  DWORDMEMORY (            // Descriptor for BIOS Area
          ResourceProducer,      // bit 0 of general flags is 0
          PosDecode,
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          Cacheable,
          ReadWrite,
          0x00000000,            // Granularity
          0x000E0000,            // Min
          0x000FFFFF,            // Max
          0x00000000,            // Translation
          0x00020000             // Range Length
          )

  DWORDMEMORY (            // Descriptor for LPEA Area
          ResourceProducer,      // bit 0 of general flags is 0
          PosDecode,
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          Cacheable,
          ReadWrite,
          0x00000000,            // Granularity
          0x20000000,            // Min
          0x201FFFFF,            // Max
          0x00000000,            // Translation
          0x00200000             // Range Length
          ,,,
          LPA0
          )

  DWORDMEMORY (            // Descriptor for ISP0 reserved Mem
          ResourceProducer,      // bit 0 of general flags is 0
          PosDecode,
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          Cacheable,
          ReadWrite,
          0x00000000,            // Granularity
          0x7A000000,            // Min
          0x7A3FFFFF,            // Max
          0x00000000,            // Translation
          0x00400000             // Range Length
          ,,,
          ISP0
          )

  DWORDMEMORY (            // Descriptor for PCI MMIO
          ResourceProducer,      // bit 0 of general flags is 0
          PosDecode,
          MinFixed,              // Range is fixed
          MaxFixed,              // Range is fixed
          Cacheable,
          ReadWrite,
          0x00000000,            // Granularity
          0x80000000,            // Min
          0xDFFFFFFF,            // Max
          0x00000000,            // Translation
          0x60000000             // Range Length
          ,,,
          PM01
          )

})

//Name(GUID,UUID("33DB4D5B-1FF7-401C-9657-7441C03DD766"))
Name(GUID,Buffer(){0x5b, 0x4d, 0xdb, 0x33,
          0xf7, 0x1f,
          0x1c, 0x40,
          0x96, 0x57,
          0x74, 0x41, 0xc0, 0x3d, 0xd7, 0x66})


Name(SUPP,0)  // PCI _OSC Support Field value
Name(CTRL,0)  // PCI _OSC Control Field value

Method(_OSC,4,Serialized)
{  // Check for proper UUID
  // Save the capabilities buffer
  Store(Arg3,Local0)

  // Create DWord-adressable fields from the Capabilties Buffer
  CreateDWordField(Local0,0,CDW1)
  CreateDWordField(Local0,4,CDW2)
  CreateDWordField(Local0,8,CDW3)

  // Check for proper UUID
  If(LAnd(LEqual(Arg0,GUID),NEXP))
  {
    // Save Capabilities DWord2 & 3
    Store(CDW2,SUPP)
    Store(CDW3,CTRL)

    // You can clear bits in CTRL here if you don't want OS to take
    // control

//    And(CTRL, 0xFFFFFFF8, CTRL)        // disable Native hot plug, PME

    If(Not(And(CDW1,1)))  // Query flag clear?
    {  // Disable GPEs for features granted native control.
      If(And(CTRL,0x02))
      {
        NHPG()
      }
      If(And(CTRL,0x04))  // PME control granted?
      {
        NPME()
      }
    }

    If(LNotEqual(Arg1,One))
    {  // Unknown revision
      Or(CDW1,0x08,CDW1)
    }

    If(LNotEqual(CDW3,CTRL))
    {  // Capabilities bits were masked
      Or(CDW1,0x10,CDW1)
    }
    // Update DWORD3 in the buffer
    Store(CTRL,CDW3)
    Store(CTRL,OSCC)
    Return(Local0)
  } Else {
    Or(CDW1,4,CDW1)    // Unrecognized UUID
    Return(Local0)
  }
}  // End _OSC
