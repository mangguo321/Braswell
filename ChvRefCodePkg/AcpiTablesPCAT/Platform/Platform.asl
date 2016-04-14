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

Name (PICM, 0)  // Global Name, returns current Interrupt controller mode; updated from _PIC control method

// Define the following External variables to prevent a WARNING when
// using ASL.EXE and an ERROR when using IASL.EXE.

External(PDC0)
External(PDC1)
External(PDC2)
External(PDC3)
External(CFGD)
External(\_PR.CPU0._PPC, IntObj)
External(\_SB.PCI0.LPCB.TPM.PTS, MethodObj)
External(\_SB.TCHG, DeviceObj)

//
// Create a Global MUTEX.
//
Mutex (MUTX, 0)

// Define Port 80 as an ACPI Operating Region to use for debugging.  Please
// note that the Intel CRBs have the ability to ouput an entire DWord to
// Port 80h for debugging purposes, so the model implemented here may not be
// able to be used on OEM Designs.

OperationRegion (PRT0, SystemIO, 0x80, 4)
Field (PRT0, DwordAcc, Lock, Preserve)
{
  P80H, 32
}

// Port 80h Update:
//    Update 8 bits of the 32-bit Port 80h.
//
//  Arguments:
//    Arg0:  0 = Write Port 80h, Bits 7:0 Only.
//           1 = Write Port 80h, Bits 15:8 Only.
//           2 = Write Port 80h, Bits 23:16 Only.
//           3 = Write Port 80h, Bits 31:24 Only.
//    Arg1:  8-bit Value to write
//
//  Return Value:
//    None

Method (P8XH, 2, Serialized)
{
  If (LEqual(Arg0, 0)) {                         // Write Port 80h, Bits 7:0.
    Store (Or(And(P80D, 0xFFFFFF00), Arg1), P80D)
  }  ElseIf (LEqual(Arg0, 1)) {                  // Write Port 80h, Bits 15:8.
    Store (Or (And (P80D, 0xFFFF00FF), ShiftLeft (Arg1, 8)), P80D)
  }  ElseIf (LEqual(Arg0, 2)) {                  // Write Port 80h, Bits 23:16.
    Store (Or (And (P80D, 0xFF00FFFF), ShiftLeft (Arg1, 16)), P80D)
  } ElseIf (LEqual(Arg0, 3)) {                   // Write Port 80h, Bits 31:24.
    Store (Or (And (P80D, 0x00FFFFFF), ShiftLeft (Arg1, 24)), P80D)
  }

  Store (P80D, P80H)
}

//
// Define SW SMI port as an ACPI Operating Region to use for generate SW SMI.
//
OperationRegion (SPRT, SystemIO, 0xB2, 2)
Field (SPRT, ByteAcc, Lock, Preserve) {
  SSMP, 8
}

// The _PIC Control Method is optional for ACPI design.  It allows the
// OS to inform the ASL code which interrupt controller is being used,
// the 8259 or APIC.  The reference code in this document will address
// PCI IRQ Routing and resource allocation for both cases.
//
// The values passed into _PIC are:
//   0 = 8259
//   1 = IOAPIC

Method (\_PIC, 1)
{
  Store(Arg0, GPIC)
  Store(Arg0, PICM)
}

OperationRegion(SWC0, SystemIO, 0x610, 0x0F)
Field(SWC0, ByteAcc, NoLock, Preserve)
{
  G1S, 8,      //SWC GPE1_STS
  Offset(0x4),
  G1E, 8,
  Offset(0xA),
  G1S2, 8,     //SWC GPE1_STS_2
  G1S3, 8      //SWC GPE1_STS_3
}

OperationRegion (SWC1, SystemIO, \PMBS, 0x2C)
Field(SWC1, DWordAcc, NoLock, Preserve)
{
  Offset(0x20),
  G0S, 32,      //GPE0_STS
  Offset(0x28),
  G0EN, 32      //GPE0_EN
}

OperationRegion(SCWS, SystemMemory, Or(\GBA1, 0x200), 4)
Field(SCWS, DWordAcc, Lock, Preserve) {
  ,     9,
  SOCW, 1                                        // SOC_WAKE_SCI_N Wake Status bit of GPIO N25: GPIO_SUS6
}

OperationRegion(TPIS, SystemMemory, Or(\GBA3, 0x204), 4)
Field(TPIS, DWordAcc, Lock, Preserve) {
  ,     14,
  PADW, 1                                        // TCH_PAD_INT_N Status bit of GPIO SE77: GPIO_ALERT
}

//
// Prepare to Sleep.  The hook is called when the OS is about to
// enter a sleep state.  The argument passed is the numeric value of
// the Sx state.
//
Method (_PTS, 1)
{
  Store (0, P80D)    // Zero out the entire Port 80h DWord.
  P8XH (0, Arg0)     // Output Sleep State to Port 80h, Byte 0.
  And (\XMMA, 0xFFFFFFF0, \XMIO) // Save xHCIBAR

  If (\_OSI("Android"))
  {
    If(LEqual(\_SB.PWRB.UPPS, 0x1))
    {
      Store (0, \_SB.PCI0.LPCB.H_EC.PB10)
    }
  }


  // Clear the 3 SWC status bits
  Store (Ones, G1S3)
  Store (Ones, G1S2)
  Store (1, G1S)

  // Set SWC GPE1_EN
  Store (1, G1E)

  // Clear GPE0_STS
  Store (Ones, G0S)
  
  If (LEqual (BDID, CRRB)) {                // If it's BSW CR board
    // Clear SOC_WAKE_SCI_N status
    Store (1, ^SOCW)
  }

  // Clear TCH_PAD_INT_N status
  Store (1, ^PADW)
  
  //
  // Check whether TPM Module support is included in the project or not
  //
  If(CondRefOf(TCGM)) // Check if TCGM defined
  {
    //
    // Call TPM PTS method
    //
    \_SB.PCI0.LPCB.TPM.PTS (Arg0)
  }
}

Name (XMIO, 0)
OperationRegion (XPEB, SystemMemory, Add (\PEBS, ShiftLeft(20, 15)), 0x78)
Field (XPEB, DWordAcc, NoLock, Preserve) { // XHCI PCIE MEM
  Offset (0x04),
   ,     1,
  XMSE,  1,
  Offset (0x10),
  XMMA, 32,
  Offset (0x74),
  XHDX,  2
}

///
/// Check if PmcBase + C0h[14]set, if set write xHCIBAR + 80A4h[28] 
/// WA for XHCI PME event status can't be cleared when S3 resuming
/// Arg0: Debug Port Output Byte Value if Workaround code executed
///
Method (XWAM, 1, Serialized)
{
  If (LEqual (^XPME, 1)) {                       // PmcBase + C0h[14] = 1
    If (LAnd (LNotEqual (\XMIO, 0), LNotEqual (\XMIO, 0xFFFFFFF0))) {
      Store (^XHDX, Local0)                      // Save xHCI Device State
      Store (^XMMA, Local1)                      // Save MMIO Base Address
      Store (\XMSE, Local2)                      // Save MMIO Sapce Enable
      Store (^XMIO, ^XMMA)                       // Load MMIO Base Address
      Store (0, ^XHDX)                           // xHCI Device State-> D0
      Store (1, ^XMSE)                           // Enable xHCI MMIO Space
      OperationRegion (XWAM, SystemMemory, Add (\XMIO, 0x80A4), 0x4)
      Field (XWAM, DWordAcc, NoLock, Preserve) { // xHCIBAR
        ,     28,
        PMEC,  1
      }
      Store (1, PMEC)                            // Clear xHCI BAR 0x80A4[28]
      Store (1, \PMEB)                           // Clear PMEB Status
      Store (Local2, ^XMSE)                      // Restore MMIO Eanble
      Store (Local1, ^XMMA)                      // Restore MMIO Base
      Store (Local0, ^XHDX)                      // Restore Device State
      P8XH (1, Arg0)                             // Output Debug Port
    }
  }
}

// Wake.  This hook is called when the OS is about to wake from a
// sleep state.  The argument passed is the numeric value of the
// sleep state the system is waking from.
Name (LLID, 0)
Method (_WAK, 1, Serialized)
{
  P8XH (1, 0xAB) // Beginning of _WAK.

  if (LEqual (Arg0, 3)) {
    ^XWAM (0xAE)
  }

  If (NEXP)
  {
    // Reinitialize the Native PCI Express after resume
    If (And(OSCC, 0x02))
    {
      \_SB.PCI0.NHPG ()
    }

    If (And(OSCC,0x04)) // PME control granted?
    {
      \_SB.PCI0.NPME ()
    }
  }

  Return (Package () {0, 0})
}

/*
// Get Buffer:
//    This method will take a buffer passed into the method and
//    create then return a smaller buffer based on the pointer
//    and size parameters passed in.
//
//  Arguments:
//    Arg0:  Pointer to start of new Buffer in passed in Buffer.
//    Arg1:  Size of Buffer to create.
//    Arg2:  Original Buffer
//
//  Return Value:
//    Newly created buffer.

Method (GETB, 3, Serialized)
{
  Multiply (Arg0, 8, Local0)                     // Convert Index.
  Multiply (Arg1, 8, Local1)                     // Convert Size.
  CreateField (Arg2, Local0, Local1, TBF3)       // Create Buffer.

  Return (TBF3)                                  // Return Buffer.
}
*/
// Power Notification:
//    Perform all needed OS notifications during a
//    Power Switch.
//
//  Arguments:
//    None
//
//  Return Value:
//    None

Method (PNOT, 0, Serialized)
{
  // If MP enabled and driver support is present, notify both
  // processors.

  If (MPEN)
  {
    If (And (PDC0, 0x0008))
    {
      Notify (\_PR.CPU0,0x80)      // Eval CPU0 _PPC.

      If (And (PDC0, 0x0010))
      {
        Sleep (100)
        Notify (\_PR.CPU0, 0x81)   // Eval _CST.
      }
    }

    If (And (PDC1, 0x0008))
    {
      Notify (\_PR.CPU1, 0x80)     // Eval CPU1 _PPC.

      If (And (PDC1, 0x0010))
      {
        Sleep (100)
        Notify (\_PR.CPU1, 0x81)   // Eval _CST.
      }
    }

    If (And (PDC2, 0x0008))
    {
      Notify (\_PR.CPU2, 0x80)     // Eval CPU2 _PPC.

      If (And (PDC2, 0x0010))
      {
        Sleep (100)
        Notify (\_PR.CPU2, 0x81)   // Eval _CST.
      }
    }

    If (And (PDC3, 0x0008))
    {
      Notify (\_PR.CPU3, 0x80)     // Eval CPU3 _PPC.

      If (And (PDC3, 0x0010))
      {
        Sleep (100)
        Notify (\_PR.CPU3, 0x81)   // Eval _CST.
      }
    }
  }
  Else
  {
    Notify (\_PR.CPU0, 0x80)       // Eval _PPC.
    Sleep (100)
    Notify (\_PR.CPU0, 0x81)       // Eval _CST
  }
  // Update the Battery 1 Stored Capacity and Stored Status.
  // Battery 0 information is always accurrate.

  If (LEqual (PFLV, FMBL))
  {
    If (LNotEqual (BTSP, 0))
    {
      Store (\_SB.PCI0.LPCB.H_EC.B1CC, B1SC)
      Store (\_SB.PCI0.LPCB.H_EC.B1ST, B1SS)

      // Perform update to all Batteries in the System.
      If (LGreaterEqual (OSYS, 2006))            // Vista and Win7 later on OS
      {
        Notify (\_SB.PCI0.LPCB.H_EC.BAT0, 0x81)  // Eval BAT0 _BIF.
        Notify (\_SB.PCI0.LPCB.H_EC.BAT1, 0x81)  // Eval BAT1 _BIF.
      }
      Else
      {
        Notify (\_SB.PCI0.LPCB.H_EC.BAT0, 0x80)  // Eval BAT0 _BST.
        Notify (\_SB.PCI0.LPCB.H_EC.BAT1, 0x80)  // Eval BAT1 _BST.
      }
    }
  }
  If (LEqual (\DPTE, 1)){
    If (LEqual (\CHGR, 1)){
      Notify (\_SB.TCHG, 0x80) // PPPC/PPDL reevaluation after AC/DC transtion has occurred.
    }
  }
}

//
// System Bus
//
Scope (\_SB)
{

  // Define a (Control Method) Power Button.
  Device (PWRB)
  {
    Name (_HID,EISAID ("PNP0C0C"))

    // GPI13 = GPE29 = Waketime SCI.  The PRW isn't working when
    // placed in any of the logical locations ( PS2K, PS2M),
    // so a Power Button Device was created specifically
    // for the WAKETIME_SCI PRW.

    Name(_PRW, Package () {29, 4})

    //
    // Power button status flag
    //
    Name (PBST, 1)

    //
    // Up Press Register flag. Set when OS register to recieve the up press of the power button
    //
    Name (UPPS, 0)

    //
    // Status of Power Button Level when EC is in mode where SCI is sent for both press and release of power button
    //
    Name (PBLV, 0)

    Method (_STA, 0)
    {
      If (LAnd (LEqual (ECON, 1), PBST)){ 
        Return (0x0F)
      }
      Return (0x00)
    }

    Method (PBUP, 0) {
      If (UPPS) {
        Notify (\_SB.PWRB, 0xC0) // Send release notification to Power Button device
      }
    }

    Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
      // Compare passed in UUID to supported UUID.
      If (LEqual (Arg0, ToUUID ("9C355BCB-35FA-44f7-8A67-447359C36A03")))
      {
        If (LEqual (0, ToInteger (Arg1)))        // Revision 0.
        {
          Switch (ToInteger (Arg2))              // Switch to Function Index.
          {
             //
             // Function 0, Query of supported functions.
             //

             Case (0)
             {
               Return (Buffer() {0x07}) // Supports Function 1,2.
             }

             //
             // Function 1, Windows Compatible Button Array Power Button Properties.
             //

             Case (1)
             {
               Store (0x1, UPPS) // OS has registered to recieve notify on release of power button
              //
              // Place EC into mode where SCI is sent for both press and release of power button
              //
              \_SB.PCI0.LPCB.H_EC.ECMD (0x74)
              Store (1, \_SB.PCI0.LPCB.H_EC.PB10)
              Return (0x00)

             }

             //
             // Function 2, Power Button Level.
             //
             Case (2) 
             {
               If (UPPS) { // If OS has registered to recieve notify on release of power button
                 If (LNot (PBLV)) {
                   Return (0x00)   // Power button not pressed  
                 } Else {
                  Return (0x01)    // Power button pressed 
                 }
               }
             }
           } // End Switch statement
        } // End Revision check
      } // End UUID check

      //
      // If the code falls through to this point, just return a buffer of 0.
      //
      Return (Buffer() {0x00})
    } // End _DSM Method

  }

  Device (SLPB)
  {
    Name (_HID, EISAID ("PNP0C0E"))
  } // END SLPB

  Scope (PCI0)
  {
    Method (_INI, 0)
    {
      // Determine the OS and store the value, where:
      //
      //   OSYS = 2000 = WIN2000.
      //   OSYS = 2001 = WINXP, RTM or SP1.
      //   OSYS = 2002 = WINXP SP2.
      //   OSYS = 2006 = Vista.
      //   OSYS = 2009 = Windows 7 and Windows Server 2008 R2.
      //   OSYS = 2012 = Windows 8 and Windows Server 2012.
      //   OSYS = 2013 = Windows Blue
      //   OSYS = 2015 = Windows Threshold
      //
      // Assume Windows 2000 at a minimum.

      Store(2000, OSYS)

      // Check for a specific OS which supports _OSI.

      If (CondRefOf (\_OSI, Local0))
      {
        // Linux returns _OSI = TRUE for numerous Windows
        // strings so that it is fully compatible with
        // BIOSes available in the market today.  There are
        // currently 2 known exceptions to this model:
        //  1) Video Repost - Linux supports S3 without
        //    requireing a Driver, meaning a Video
        //    Repost will be required.
        //  2) On-Screen Branding - a full CMT Logo
        //    is limited to the WIN2K and WINXP
        //    Operating Systems only.

        // Use OSYS for Windows Compatibility.

        If (\_OSI ("Windows 2001"))     // Windows XP
        {
          Store (2001, OSYS)
        }

        If (\_OSI ("Windows 2001 SP1")) // Windows XP SP1
        {
          Store (2001, OSYS)
        }

        If (\_OSI ("Windows 2001 SP2")) // Windows XP SP2
        {
          Store (2002, OSYS)
        }

        If (\_OSI ("Windows 2006"))     // Windows Vista
        {
          Store (2006, OSYS)
        }

        If (\_OSI ("Windows 2009"))     // Windows 7 or Windows Server 2008 R2
        {
          Store (2009, OSYS)
        }

        If (\_OSI ("Windows 2012"))     // Windows 8 or Windows Server 2012
        {
          Store (2012, OSYS)
        }

        If (\_OSI ("Windows 2013"))     //Windows Blue
        {
          Store (2013, OSYS)
        }

        If(\_OSI("Windows 2015"))      //Windows Threshold
        {
          Store(2015,OSYS)
        }
        //
        // If CMP is enabled, enable SMM C-State
        // coordination.  SMM C-State coordination
        // will be disabled in _PDC if driver support
        // for independent C-States deeper than C1
        // is indicated.
      }
    }

    Method (NHPG, 0, Serialized)
    {

    }

    Method (NPME, 0, Serialized)
    {

    }
  } // end Scope (PCI0)

 Device (GPED)  // virtual GPIO device for ASL based AC/Battery/Expection notification
 {
   Name (_ADR, 0)
   Name (_HID, "INT0002")
   Name (_CID, "INT0002")
   Name (_DDN, "Virtual GPIO controller" )
   Name (_UID, 1)
   Name (INST, One)
   Name (XTMP, Zero)

   //
   // Declare an op-region for accessing the PMC PME_STS register.
   // L02 ASL will use these bits to identify the cause of the wake.
   // This register is valid for B1 and later steppings only.
   //
   OperationRegion (SCIS, SystemMemory, 0xFED030C0, 0x4)
   Field (SCIS, DWordAcc, NoLock, Preserve)
   {
           , 1,      // Bit 0
       LHAD, 1,
       LATA, 1,
       LDIO, 1,
       LARD, 1,
       LIO1, 1,
       LCEP, 1,      // Bit 6 SEC0
       LANB, 1,
       LHCI, 1,      // Bit 8 XHCI
       LOTG, 1,
       LECI, 1,
       LHSI, 1
   }


   Method (_DSM, 0x4, NotSerialized)
   {
     // DSM UUID .
     If (LEqual (Arg0, ToUUID ("8B38B469-6F95-4B08-9B02-2DEFCC2D2C35")))
     {
       // Function 0 : Query Function
       If (LEqual (Arg2, Zero))
       {
         // Revision 1
         If (LEqual (Arg1, One))
         {
           Return (Buffer (One) { 0x03 })
         }
       }
       // Function 1 : return PMBASE+0x20
       If (LEqual (Arg2, One))
       {
         Return (Add (PMBS,0x20))
       }

       Return (0x00)
     }
     Else
     {
       Return (Buffer (One) { 0x00 })
     }

   }

     Method (_CRS, 0x0, Serialized)
     {
       Name (RBUF, ResourceTemplate ()
       {
         Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) {9} // should be 108
       })
       Return (RBUF)
     }

     Method (_STA, 0x0, NotSerialized)
     {
       If (LGreaterEqual (OSYS,2013))            // Virtual Button applicable for Winblue and higher version of WinOS
       {
         If (And (PB1E, 1))                      // Virtual Power Button is enabled?
         {
           Return (0x00)                         // Device present,enabled and should be shown in UI.
         } 
       }
       Return (0x00)                             // Device NOT present
     }

     Method (_AEI, 0x0, Serialized)
     {
       Name (RBUF, ResourceTemplate()
       {
         GpioInt(Edge, ActiveHigh, ExclusiveAndWake, PullDown,,"\\_SB.GPED",) {2} //pin 2
       })
       Return (RBUF)
     }

     Method (_E02) { // _Exx method will be called when interrupt is raised
       If (LEqual (PWBS, 1)) {
         Store (1, PWBS)                         // Clear PowerButton Status
         Notify (\_SB.PWRB, 0x80)
       }
       If (LEqual (PMEB, 1)) {
         Store (1, PMEB)      //Clear PME_B0_STS
       }

     If (LEqual (\_SB.PCI0.SATA.PMES, 1)) {
       Store (1, \_SB.PCI0.SATA.PMES)
       Notify (\_SB.PCI0.SATA, 0x02)
     }


     If (LEqual (\_SB.PCI0.HDEF.PMES, 1)) {
       Store (1, \_SB.PCI0.HDEF.PMES)
       Notify (\_SB.PCI0.HDEF, 0x02)
     }

     If (LEqual (\_SB.PCI0.XHC1.PMES, 1))
     {
       Store (1, \_SB.PCI0.XHC1.PMES)
       Notify (\_SB.PCI0.XHC1, 0x02)
     }
   }
 } //  Device (GPED)

  //--------------------
  //  GPIO
  //--------------------
  Device (GPO0)
  {
     Name (_ADR, 0)
     Name (_HID, "INT33FF")
     Name (_CID, "INT33FF")
     Name (_DDN, "General Purpose Input/Output (GPIO) controller - SOUTHWEST")
     Name (_UID, 1)

     Method (_CRS, 0x0, NotSerialized)
     {
       Name (RBUF, ResourceTemplate ()
       {
          //IO(Decode16,0x0500,0x0500,0x01,0xFF)
          Memory32Fixed (ReadWrite, 0x0FED80000, 0x00008000)
          Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , ) {49}
       })
        Return (RBUF)
      }

    OperationRegion (GPM0, SystemMemory, Or(\GBA0, 0x5800), 0x30)
    Field (GPM0, DWordAcc, Lock, Preserve) {
      ,     1,
      TSRR, 1,                                   // GPIO Tx State of GPIO SW75: TOUCH_RST_N for BSW CR
      Offset(0x18),
      ,     1,
      TSR2, 1,                                   // GPIO Tx State of GPIO SW78: TOUCH_RST_N for BSW RVP FAB2
      Offset(0x28),
      ,     1,
      TSRS, 1                                    // GPIO Tx State of GPIO SW80: TOUCH_RST_N for BSW RVP
    }

    Name (AVBL, 1)
    Method (_REG, 2) {
      If (Lequal (Arg0, 0)) {
        Store (Arg1, ^AVBL)
      }
    }

    Method (_STA, 0x0, NotSerialized)
    {
       If (\_OSI ("Android")) {                  // Android would parse GPIO _REG but Win8.1 won't
         Store (0, ^AVBL)
       }
       if (LEqual (GOAE, 1)) {                   // If LPSS with GPIO Devices Support Enabled
         Return (0xF)
       }
       Return (0)
    }
  }   //  Device (GPO0)

  Device (GPO1)
  {
    Name (_ADR, 0)
    Name (_HID, "INT33FF")
    Name (_CID, "INT33FF")
    Name (_DDN, "General Purpose Input/Output (GPIO) controller - NORTH" )
    Name (_UID, 2)

    Method (_CRS, 0x0, NotSerialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite, 0x0FED88000, 0x00008000)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , ) {48}
      })
      Return (RBUF)
    }

    OperationRegion (GPM1, SystemMemory, Or (\GBA1, 0x4428), 4)
    Field (GPM1, DWordAcc, Lock, Preserve) {
      ,     1,
      CWLE, 1                                    // Tx bit of SoC GPIO N05: GPIO_DFX4 to COMBO_WLAN_EN
    }

    Name (AVBL, 1)
    Method (_REG, 2) {
      If (Lequal (Arg0, 0)) {
        Store (Arg1, ^AVBL)
      }
    }

    Method (_STA, 0x0, NotSerialized)
    {
       If (\_OSI ("Android")) {                  // Android would parse GPIO _REG but Win8.1 won't
         Store (0, ^AVBL)
       }
       if (LEqual (GOAE, 1)) {                   // If LPSS with GPIO Devices Support Enabled
         Return (0xF)
       }
       Return (0)
    }
  }   //  Device (GPO1)

  Device (GPO2)
  {
    Name (_ADR, 0)
    Name (_HID, "INT33FF")
    Name (_CID, "INT33FF")
    Name (_DDN, "General Purpose Input/Output (GPIO) controller - EAST" )
    Name (_UID, 3)

    Method (_CRS, 0x0, NotSerialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite, 0x0FED90000, 0x00008000)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , ) {50}
      })
      Return (RBUF)
    }

    OperationRegion (GPM2, SystemMemory, Or(\GBA2, 0x4820), 4)
    Field (GPM2, DWordAcc, Lock, Preserve) {
      ,     1,
      SHD3, 1                                    // [1] GPIO Tx State of  GPIO E19: SENSOR_HUB_RESET
    }

    Name (AVBL, 1)
    Method (_REG, 2) {
      If (Lequal (Arg0, 0)) {
        Store (Arg1, ^AVBL)
      }
    }

    Method (_STA, 0x0, NotSerialized)
    {
       If (\_OSI ("Android")) {                  // Android would parse GPIO _REG but Win8.1 won't
         Store (0, ^AVBL)
       }
       if (LEqual (GOAE, 1)) {                   // If LPSS with GPIO Devices Support Enabled
         Return (0xF)
       }
       Return (0)
    }
  }   //  Device (GPO2)

  Device (GPO3)
  {
    Name (_ADR, 0)
    Name (_HID, "INT33FF")
    Name (_CID, "INT33FF")
    Name (_DDN, "General Purpose Input/Output (GPIO) controller - SOUTHEAST" )
    Name (_UID, 4)

    Method (_CRS, 0x0, NotSerialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        Memory32Fixed (ReadWrite, 0x0FED98000, 0x00008000)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , ) {91}
      })
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
       if (LEqual (GOAE, 1)) {                   // If LPSS with GPIO Devices Support Enabled
         Return (0xF)
       }
       Return (0)
    }
  }   //  Device (GPO3)


  Device (TBAD) // Button device
  {
    Name (_HID, "INTCFD9")
    Name (_CID, "PNP0C40")
    Name (_DDN, "Keyboard less system - 5 Button Array Device" )

    Method (_CRS, 0x0, NotSerialized)
    {
      Name (RBUF, ResourceTemplate ()
      {
        // Resources have to appear in order (power, windows, volup, voldown, rotationlock)
        GpioInt (Edge, ActiveBoth, Exclusive, PullDefault, 6000, "\\_SB.GPO0", ) {6}   // Home
        GpioInt (Edge, ActiveBoth, Exclusive, PullDefault, 6000, "\\_SB.GPO0", ) {5}   // Volume up
        GpioInt (Edge, ActiveBoth, Exclusive, PullDefault, 6000, "\\_SB.GPO0", ) {4}   // Volume down
        GpioInt (Edge, ActiveBoth, Exclusive, PullDefault, 6000, "\\_SB.GPO0", ) {2}   // rotationlock
      })
      Return (RBUF)
    }

    Method(_STA)
    {
      Return (0x00) // Used for non-EC system
    }

    Method(_DSM, 0x4, NotSerialized)
    {
      Store ("Method _DSM for Button Device", Debug)
      // Switch based on which unique function identifier was passed in
      // Windows Button Array device UUID: {DFBCF3C5-E7A5-44E6-9C1F-29C76F6E059C}
      //
      If (LEqual (Arg0, ToUUID ("DFBCF3C5-E7A5-44E6-9C1F-29C76F6E059C"))) {
        switch (Arg2) {
          // Function 0: Return supported functions, based on revision
          Case (0)
          {
            // revision 0: function 0 & 1 are supported.
            return (Buffer () {0x3})
          } // Case (0)
          // Function 1: For Windows Button Array device, returns an integer with the bits set for
          // ACPI 10 seconds reset timer and level-detection on power button.
          Case (1)
          {
            //An integer (DWORD) with the following bit-wise definition:
            //Bits 31 - 2: Reserved (must be 0).
            //Bit 1: This bit should be set if power button is wired to an interrupt controller (GPIO or otherwise)
            // that supports level-detection.
            //Bit 0: This bit should be set if the platform supports ACPI reset time of 10 seconds or greater.
            //Otherwise it should be clear.
            return (0x3)
          } //Case (1)
        } // end of switch
      }// if
      else {
      // If not one of the function identifiers we recognize, then return a buffer
      // with bit 0 set to 0 indicating no functions supported.
      return (Buffer () {0})
      }
    } //End of _DSM Method
  } // end of TBAD device

  //
  // Device for Message Bus Interface
  //
  Device (MBID) {
    Name (_HID, "INT33BD")
    Name (_CID, "INT33BD")
    Name (_HRV, 2)//different from CLT's
    Name (_UID, 1)

    Method (_CRS, 0, Serialized){
      Name (RBUF, ResourceTemplate (){
                    Memory32Fixed (
                      ReadWrite,
                      0xE00000D0,  // Address Base
                      0xC,         // Address Length (MCR/MDR/MCRX)
                    )
                  })
      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized)
    {
       Return (0x00)               // Device not present, disabled and hided in UI.
    }

    //
    // custom opregion for MBI access
    //
    OperationRegion (REGS, 0x87, 0x0, 0x30)
    Field (REGS, DWordAcc, NoLock, Preserve)
    {
      PORT, 32,    // Message Port
      REG,  32,    // Message Target Register Address
      DATA, 32,    // Message Data
      MASK, 32,    // Mask bits for modify operation
      BE,   32,    // Message Write Byte enables: 0 - BYTE; 1 - WORD; 2 - DWORD
      OP,   32     // Operations: 0 - read; 1 - write; 2 - modify
    }

    //
    // availability of the custom opregion
    //
    Name (AVBL, 0)
    Method (_REG, 2) {
      If (Lequal(Arg0, 0x87)) {
        Store (Arg1, ^AVBL)
      }
    }

    //
    //  Method Name: READ
    //  Arguments:
    //    Arg0:   PORT
    //    Arg1:   REG
    //    Arg2:   BE
    //  Return Value:
    //  DATA
    //
    Method(READ, 3, Serialized) {
      Store (0xFFFFFFFF , Local0)
      If (Lequal (AVBL, 1)) {
        Store (0, OP)  // must be set at first, do not change!
        Store (Arg0, PORT)
        Store (Arg1, REG)
        Store (Arg2, BE)
        Store (DATA, Local0)
      }
      return (Local0)
    }

    //
    //  Method Name: WRIT
    //  Arguments:
    //    Arg0:   PORT
    //    Arg1:   REG
    //    Arg2:   BE
    //    Arg3:   DATA
    //  Return Value:
    //  NONE
    //
    Method (WRIT, 4, Serialized) {
      If (Lequal (AVBL, 1)) {
        Store (1, OP)  // must be set at first, do not change!
        Store (Arg0, PORT)
        Store (Arg1, REG)
        Store (Arg2, BE)
        Store (Arg3, DATA)
      }
    }

    //
    //  Method Name: MODI
    //  Arguments:
    //    Arg0:   PORT
    //    Arg1:   REG
    //    Arg2:   BE
    //    Arg3:   DATA
    //    Arg4:   MASK
    //  Return Value:
    //  NONE
    //
    Method (MODI, 5, Serialized) {
      If (Lequal (AVBL, 1)) {
        Store (2, OP)  // must be set at first, do not change!
        Store (Arg0, PORT)
        Store (Arg1, REG)
        Store (Arg2, BE)
        Store (Arg3, DATA)
        Store (Arg4, MASK)
      }
    }
  }

//
//  Platform Indicator device PIND
//  handles platform events, buttons and indicators such as: 
//  VOLUME_UP, VOLUME_DOWN, BRIGHTNESS, ROTATION_LOCK, MUTE, LID, DOCK, POWER_BUTTON
//  SLEEP_BUTTON, WIFI, TABLET_MODE
//
Device (PIND) {
  Name (_HID, "INT3497")
  Name (_UID, 0)

  Method (_STA) {
    If (\_OSI ("Android"))
    {
      Return (0xF)
    }
    Return (0x0)   
  }

   

  //
  //  Supported TYPES:
  //  0x00  - Undefined
  //  0x01  - Interrupt source (GpioInt() or Interrupt()) defined in the _CRS method, 
  //          Descriptor field returns type, zero based index of interrupt sources found in _CRS method.
  //  0x02  - interrupt event that is not listed in CRS table. This interrupt is delivered 
  //          through a notify () event where the notify event value is a pointer 
  //          to the package indexed within the package returned by the _IND method. Descriptor field returns type only.
  //  0x03  - Level, descriptor field returnd type, current value (1 or 0).
  //  0x04  - Numerical, descriptor field returnd type, an integer value
  //  0x05  - Percentage, descriptor field returnd type, value between 0 - 100
  //  0x06  - Range, descriptor field returnd type, Minimum Value, Max Value, and current value
  //  0x07  - 0xNN  - Reserved for future types
  //
  Name (PDAT, Package () {
                // Name,          Descriptor:  Type,       value/Index        
    Package (2) {"VOLUME_UP",   Package (1) {0x02 }},  // Pulse event
    Package (2) {"VOLUME_DOWN", Package (1) {0x02 }},  // Pulse event
    Package (2) {"HOME_BUTTON", Package (1) {0x02 }}   // Pulse event
  })

  Method (_IND, 0, Serialized) {
   // If required read any items that need to be updated before returning package PDAT.
   return (PDAT)
  }
} // end device PIND



} // end Scope(\_SB)

