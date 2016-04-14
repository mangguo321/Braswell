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

// Thermal.asl represents a Thermal Zone to be used for testing on the
// Customer Reference Boards.

External(\_SB.DPTF.CTOK, MethodObj)

Scope(\_TZ)
{
  // Notes:
  //  1) WIN2K strictly uses interrupt driven thermal events.
  //  2) Temperature values are stored in tenths of Kelvin to
  //    eliminate the decimal place.
  //  3) Kelvin = Celsius + 273.2.
  //  4) All temperature must be >= 289K.

  // Flag to indicate Legacy thermal control.
  // 1 = Legacy thermal control
  // 0 = other thermal control, like DPTF
  Name(LEGA, 1) // for Active Policy
  Name(LEGP, 1) // for Passive Policy
  Name(LEGC, 1) // for Critical Policy

  // Fan 0 = Package Processor Fan.
  PowerResource(FN00,0,0)
  {
    Method(_STA,0,Serialized)
    {
      If(LEqual(FNSP,0)) {
        Return(0x0)
      }
      // Return Virtual Fan 0 status.
      Return(VFN0)
    }

    Method(_ON,0,Serialized)
    {
      // Set Virtual Fan 0 On.
      Store(1,VFN0)
  
      // If EC access is enabled.
      If(LAnd(\ECON, \_TZ.LEGA))
      {
        Store(1, \_SB.PCI0.LPCB.H_EC.FNSL)      // Set bit-0 to select CPU Fan.
        Store(100, \_SB.PCI0.LPCB.H_EC.FDCY)     // Fan set to 100% duty cycle.
        \_SB.PCI0.LPCB.H_EC.ECMD(0x1A)          // Fan command.
      }
    }

    Method(_OFF,0,Serialized)
    {
      // Set Virtual Fan 0 Off.
      Store(0,VFN0)
  
      // If EC access is enabled.
      If(LAnd(\ECON, \_TZ.LEGA))
      {
        Store(1, \_SB.PCI0.LPCB.H_EC.FNSL)      // Set bit-0 to select CPU Fan.
        Store(0, \_SB.PCI0.LPCB.H_EC.FDCY)      // Fan set to 0% duty cycle.
        \_SB.PCI0.LPCB.H_EC.ECMD(0x1A)          // Fan command.
      }
    }
  }

  // Associate Virtual Fan 0 Power Resource with the FAN0 Device.
  Device(FAN0)
  {
    Name(_HID, EISAID("PNP0C0B"))
    Name(_UID,0)
    Name(_PR0, Package(1){FN00})
  }

  // Thermal Zone 1 = Package Thermal Zone.
  // Package Thermal Zone is used for Active and Critical Policy Control
  // Package Thermal Zone returns the maximum temperature
  // of all components within the package
  ThermalZone(TZ01)
  {
    // Temporary variable for holding the current temprature reading
    Name(PTMP, 3000)
    Name(SINI, 0)
    Name(ADDR, 0x96) // SMBUS Address of temperature sensor
    Name(LTV0, 0)    // Local Temperature Value
    Name(RTV1, 1)    // Remote 1 Temperature Value
    Name(RTV2, 0x30) // Remote 2 Temperature Value
    Name(HITP, 0x55) // Temp High Limit

    // Notifies ASL Code the current cooling mode.
    //  0 - Active cooling
    //  1 - Passive cooling
    Method(_SCP, 1, Serialized)
    {
      Store(Arg0, CTYP)
    }

    // Return the temperature at which the OS must perform a Critcal Shutdown.
    Method(_CRT,0,Serialized)
    {
      If (LEqual(\_TZ.LEGC,0)){ Return(Add(2732,Multiply(210,10)))} // 210 degree C
      Return(Add(2732,Multiply(\CRTT,10)))
    }

    // We are not using Active Cooling in legacy mode so hard code some values for WHQL.
    Method(_AC0,0,Serialized)
    {
      Return(Add(2732,Multiply(110,10))) // 110 degree C
    }

    // Returns the Device to turn on when _ACx is exceeded.
    Name(_AL0, Package(){FAN0})

    // _TMP (Temperature)
    //
    // Return the highest of the CPU temperatures to the OS.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing the current temperature of the thermal zone (in tenths of degrees Kelvin)
    //
    Method(_TMP,0,Serialized)
    {
      //
      // If DTS enabled, return higher temperature of the 2 cores
      //
      If (DTSE)
      {
        If (LGreater(DTS2, DTS1))
        {
          Store(DTS2,Local0)
        } Else {
          Store(DTS1,Local0)
        }
        Return (Add(2732,Multiply(Local0,10)))
      }
      //
      // Return the higher of EC temperature sensors.
      //
      If (\ECON)
      {
        If (LGreater(\_SB.PCI0.LPCB.H_EC.LTMP, \_SB.PCI0.LPCB.H_EC.TMPR))
        {
          Return (Add(2732,Multiply(\_SB.PCI0.LPCB.H_EC.LTMP,10)))
        } Else {
          Return (Add(2732,Multiply(\_SB.PCI0.LPCB.H_EC.TMPR,10)))
        }
      }
      //
      // Return a static value if both EC and DTS are unavailable.
      //
      Return (3000) // (3000-2732)/10 = 26.8 degree C
    } // End of_TMP scope

    // Return the Processor(s) used for Passive Cooling.
    Method(_PSL,0,Serialized)
    {
      If(LEqual(MPEN, 4))
      {
        //  CMP - Throttling controls all four logical CPUs.
        Return(Package(){\_PR.CPU0,\_PR.CPU1,\_PR.CPU2,\_PR.CPU3})
      }

      If(MPEN)
      {
        //  CMP - Throttling controls both CPUs.
        Return(Package(){\_PR.CPU0,\_PR.CPU1})
      }

      Return(Package(){\_PR.CPU0})
    }

    // Returns the temperature at which the OS initiates CPU throttling.
    Method(_PSV,0,Serialized)
    {
      If (LEqual(\_TZ.LEGP,0)){ Return(Add(2732,Multiply(110,10)))} // 110 degree C
      Return(Add(2732,Multiply(\PSVT,10)))
    }

    // Returns TC1 value used in the passive cooling formula.
    Method(_TC1,0,Serialized)
    {
      Return(TC1V)
    }

    // Returns TC2 value used in the passive cooling formula.
    Method(_TC2,0,Serialized)
    {
      Return(TC2V)
    }

    // Returns the sampling period used in the passive cooling formula.
    Method(_TSP,0,Serialized)
    {
      Return(TSPV)
    }

    // Returns Hot Temperature
    Method(_HOT,0,Serialized)
    {
      If (LEqual(\_TZ.LEGC,0)){ Return(Add(2732,Multiply(205,10)))} // 205 degree C
      Subtract(\CRTT, 5, Local0)
      Return(Add(2732,Multiply(Local0,10)))
    }
  }  // End of thermal zone
}  // End of scope
