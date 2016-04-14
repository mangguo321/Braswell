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

// General Purpose Events.  This Scope handles the Run-time and
// Wake-time SCIs.  The specific method called will be determined by
// the _Lxx value, where xx equals the bit location in the General
// Purpose Event register(s).
External(HGLS, MethodObj)
External(HGAS, MethodObj)

Scope(\_GPE)
{
  //
  // PCI Express Hot-Plug caused the wake event.
  //
  Method(_L01)
  {
    Add(L01C,1,L01C)  // Increment L01 Entry Count.

    P8XH(0,0x01)      // Output information to Port 80h.
    P8XH(1,L01C)

    // Check Root Port 1 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LEqual(RP1D,0),\_SB.PCI0.RP01.HPSX))
    {
      // Delay for 100ms to meet the timing requirements
      // of the PCI Express Base Specification, Revision
      // 1.0A, Section 6.6 ("...software must wait at
      // least 100ms from the end of reset of one or more
      // device before it is permitted to issue
      // Configuration Requests to those devices").

      Sleep(100)

      If(\_SB.PCI0.RP01.PDCX)
      {
        // Clear all status bits first.

        Store(1,\_SB.PCI0.RP01.PDCX)
        Store(1,\_SB.PCI0.RP01.HPSX)

        //
        // CHV BIOS Spec, Section 13.X PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If(LNot(\_SB.PCI0.RP01.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store(0,\_SB.PCI0.RP01.L0SE)

        }

        // Perform proper notification
        // to the OS.

        Notify(\_SB.PCI0.RP01,0)
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store(1,\_SB.PCI0.RP01.HPSX)
      }
    }

    // Check Root Port 2 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LEqual(RP2D,0),\_SB.PCI0.RP02.HPSX))
    {
      Sleep(100)

      If(\_SB.PCI0.RP02.PDCX)
      {
        Store(1,\_SB.PCI0.RP02.PDCX)
        Store(1,\_SB.PCI0.RP02.HPSX)

        If(LNot(\_SB.PCI0.RP02.PDSX)) {
          Store(0,\_SB.PCI0.RP02.L0SE)
        }

        Notify(\_SB.PCI0.RP02,0)
      }
      Else
      {
        Store(1,\_SB.PCI0.RP02.HPSX)
      }
    }

    // Check Root Port 3 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LEqual(RP3D,0),\_SB.PCI0.RP03.HPSX))
    {
      Sleep(100)

      If(\_SB.PCI0.RP03.PDCX)
      {
        Store(1,\_SB.PCI0.RP03.PDCX)
        Store(1,\_SB.PCI0.RP03.HPSX)

        If(LNot(\_SB.PCI0.RP03.PDSX)) {
          Store(0,\_SB.PCI0.RP03.L0SE)
        }

        Notify(\_SB.PCI0.RP03,0)
      }
      Else
      {
        Store(1,\_SB.PCI0.RP03.HPSX)
      }
    }

    // Check Root Port 4 for a Hot Plug Event if the Port is
    // enabled.

    If(LAnd(LEqual(RP4D,0),\_SB.PCI0.RP04.HPSX))
    {
      Sleep(100)

      If(\_SB.PCI0.RP04.PDCX)
      {
        Store(1,\_SB.PCI0.RP04.PDCX)
        Store(1,\_SB.PCI0.RP04.HPSX)

        If(LNot(\_SB.PCI0.RP04.PDSX)) {
          Store(0,\_SB.PCI0.RP04.L0SE)
        }

        Notify(\_SB.PCI0.RP04,0)
      }
      Else
      {
        Store(1,\_SB.PCI0.RP04.HPSX)
      }
    }
  }

  //
  // Software GPE caused the event.
  //
  Method(_L02)
  {
    // Clear the SWGPE Status Bit.
    Store(0,GPEC)

    // Handle the DTS Thermal Event.
    Notify(\_TZ.TZ01,0x80)
  }

  //
  // PUNIT SCI event.
  //
  Method(_L04)
  {
    // Clear the PUNIT Status Bit.
    Store(1, PSCI)
  }

  //
  // IGD OpRegion SCI event (see IGD OpRegion/Software SCI BIOS SPEC).
  //
  Method(_L05)
  {
    If(LAnd(\_SB.PCI0.GFX0.GSSE, LNot(GSMI))) // Graphics software SCI event?
    {
      \_SB.PCI0.GFX0.GSCI() // Handle the SWSCI
    }
  }

  //
  // This PME event (PCH's GPE #9) is received on one or more of the PCI Express* ports or
  // an assert PMEGPE message received via DMI
  //
  Method(_L09, 0) {
    //
    // If the Root Port is enabled, run PCI_EXP_STS handler
    //
    If(LEqual(RP1D,0))
    {
      \_SB.PCI0.RP01.HPME()
      Notify(\_SB.PCI0.RP01, 0x02)
    }

    If(LEqual(RP2D,0))
    {
      \_SB.PCI0.RP02.HPME()
      Notify(\_SB.PCI0.RP02, 0x02)
    }

    If(LEqual(RP3D,0))
    {
      \_SB.PCI0.RP03.HPME()
      Notify(\_SB.PCI0.RP03, 0x02)
    }

    If(LEqual(RP4D,0))
    {
      \_SB.PCI0.RP04.HPME()
      Notify(\_SB.PCI0.RP04, 0x02)
    }
  }

  //
  // This PME event (PCH's GPE #13) is received when any PCH internal device with PCI Power Management capabilities
  // on bus 0 asserts the equivalent of the PME# signal.
  //
  Method(_L0D, 0) {
    Notify(\_SB.PCI0.HDEF, 0x02)
    Notify(\_SB.PCI0.XHC1, 0x02)
  }
}
