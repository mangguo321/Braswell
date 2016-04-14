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

External(LLID)
External(OTGM)
External(HGAS, MethodObj)
External(CHPS, MethodObj)
External(HPFS, MethodObj)
External(HBRT, MethodObj)
External(\_SB.PCI0.B0DB.CLPO, PkgObj)

#define POWER_BUTTON         0
#define WINDOWS_BUTTON       1
#define VOLUME_UP_BUTTON     2
#define VOLUME_DOWN_BUTTON   3
#define ROTATION_LOCK_BUTTON 4
#define CONVERTIBLE_BUTTON   6
#define DOCK_INDICATOR       7

Device (ECRD)
{
    Name (_ADR, Zero)  // _ADR: Address
    Name (_HID, "BSWEC001")  // _HID: Hardware ID
    Name (_CID, "BSWEC001")  // _CID: Compatible ID
    Name (_DDN, "EC Resource Controller")  // _DDN: DOS Device Name
    Method (_CRS, 0x0, Serialized)  // _CRS: Current Resource Settings
    {
        Name (SBUF, ResourceTemplate ()
        {
            GpioInt (Edge, ActiveLow, Exclusive, PullDefault, 0x1770,
                "\\_SB.GPO0", 0x00, ResourceConsumer, ,
                )
                {   // Pin list
                    0x0000
                }
        })
        Return (SBUF)
    }
    Method (_STA, 0x0, NotSerialized)
    {
        If (LEqual(OSID, 1))
        {
            Return (0xF)
        }
        Return (0x0)
    }
}

Device(H_EC)  // Nuvoton Embedded Controller
{
  Name(_HID, EISAID("PNP0C09"))

  Name(_UID,1)

  Method(_CRS, 0, Serialized)
  {
    Name(BFFR, ResourceTemplate()
    {
      IO(Decode16,0x62,0x62,0,1)  // DIN/DOUT
      IO(Decode16,0x66,0x66,0,1)  // CMD/STS
    })

    Return(BFFR)
  }

  Method(_STA)
  {
    If(LEqual(ECSP,0)) {
      Store(0,ECON)
      Return(0x00)
    } Else { 
      Store(1,ECON)
      Return(0x0F)
    }
  }

    Name(_GPE,22)    // Core GPI0_SUS6 - GPE22 - INT14 - Runtime SCI

  OperationRegion(ECF2,EmbeddedControl,0,0xFF)
  Field(ECF2,ByteAcc,Lock,Preserve)
  {
    Offset(1),
    TMPR, 8,      // 1       CPU temp remote, U1H5,ADT7421, Celsius temperature
    LTMP, 8,      // 2       CPU temp local, U1H5,ADT7421, Celsius temperature
    RPWR, 1,      // 3.0     Real AC Power (AC Present = 1)
        , 2,      // 3.2:1   Reserved
    CFAN, 1,      // 3.3     CPU Fan (Fan On = 1)
        , 2,      // 3.5:4   Reserved
    LSTE, 1,      // 3.6     Lid State (Lid Open = 1)
        , 1,      // 3.7     Reserved
    SPTR, 8,      // 4       SMBus Protocol Register
    SSTS, 8,      // 5       SMBus Status Register
    SADR, 8,      // 6       SMBus Address Register
    SCMD, 8,      // 7       SMBus Command Register
    SBFR, 256,    // 8       SMBus Block Buffer
    SCNT, 8,      // 40      SMBus Block Count

    Offset(47),
    CTMP,  8,  // 47      EC Critical Temperature
        ,  3,  // 48.2:0, Reserved
    PBNS,  1,  // 48.3    Power Button State (0 = Pressed)
    VPWR,  1,  // 48.4    Virtual AC Power (AC Present = 1)
        ,  3,  // 48.7:5  Reserved
    SCAN,  8,  // 49      Hot-Key Scan Code
    B1ST,  8,  // 50      Battery 1 Status
    B1CR,  8,  // 51      Battery 1 Current Rate
    B1CC,  8,  // 52      Battery 1 Current Capacity
    B1VT,  8,  // 53      Battery 1 Voltage
    B2ST,  8,  // 54      Battery 2 Status
    B2CR,  8,  // 55      Battery 2 Current Rate
    B2CC,  8,  // 56      Battery 2 Current Capacity
    B2VT,  8,  // 57      Battery 2 Voltage
    CMDR,  8,  // 58,     KSC command register
    LUXL,  8,  // 59      LUX - Low Byte
    LUXH,  8,  // 60      LUX - High Byte
    ACH0,  8,  // 61,     Channel 0 RAW Value
    ACH1,  8,  // 62,     Channel 1 RAW VAlue

    Offset(65),
    FNSL, 8,      // 65   bitwise PWM port select (0-7), BIT0 is set for the CPU Fan

    Offset(68),
    FDCY, 8,      // 68   final PWM value, output duty cycle %, range from 1 to 100

    Offset(80),
    ART1,  8,   // 80,     Ambient temp remote 1, U3A1, ADT7481

    Offset(82),   // 82-85   DPTF fields
    TSI,  4,      //         Hysteresis selection is global and meant for all four sensors.
                  // ADT7421Remote 0 ACPIRemoteTemp[1]
                  // ADT7421Local 1 ACPILocalTemp[2]
                  // ADT7481Remote 2 NBAmbientTemp[80/50]
                  // ADT7481Local 3 NBAmbientTempLocal[186/BA]
                  // ADT7481Remote2 4 NBAmbientTempRemote2[187/BB]
                  // ADM1032Remote 5 ACPIOBDIMM0Temp[126/7E]
                  // ADM1032Local 6 ACPIOBDIMM0TempLocal[185/B9]
    HYST, 4,      // 82.4-7  Hysteresis in deg C
    TSHT, 8,      // 83      Thermal Sensor (N) high trip point
    TSLT, 8,      // 84      Thermal Sensor (N) low trip point
    TSSR, 8,      // 85      TSSR- thermal sensor status register:
                  //         TSSR bits defined:
                  // BIT0- ADT7421Remote
                  // BIT1- ADT7421Local 1
                  // BIT2- ADT7481Remote
                  // BIT3- ADT7481Local 3
                  // BIT4- ADT7481Remote2 4
                  // BIT5- ADM1032Remote 5
                  // BIT6- ADM1032Local 6

    Offset(86),
    B1CM,  8,  // 86      Battery 1 Command (Place holder for now)
    B1DC,  16, // 87      Battery 1 Design Capacity (mWh)
    B1RC,  16, // 89      Battery 1 Remaining Capacity (mWh)
    B1FC,  16, // 91      Battery 1 Full Charge Capacity (mWh)
    B1FV,  16, // 93      Battery 1 Full Resolution Voltage (mV)
    B1DI,  16, // 95      Battery 1 Full Resolution Discharge Current (mA)
    B1CI,  16, // 97      Battery 1 Full Resolution Charge Current (mA)
    Offset(115),
    CFSP,  16, // 115     CPU Fan speed in rpm 
    Offset(120),
        ,   1,
    PB10,   1,  //120.1      10s power button enable bit(0=4s, 1=10s)
    Offset(126),
    DM0T,  8, // 126      DDR3 On-board TS temp remote, U2F1,ADM1032

    Offset(177),
    ISTS,   8,      // 177  ISCT Status      (Reserved for BIOS update)
    IWRS,  8,       // 178  ISCT Wake Reason (Reserved for BIOS update)
    S3T0,  8,       // 179     ISCT S3 Time-Out value Bit 7-0
    S3T1,  8,       // 180     ISCT S3 Time-Out value Bit 15-8
    S3T2,  8,       // 181     ISCT S3 Time-Out value Bit 23-16
    S0IS,  8,       // 182  S0-ISCT Status,  bit 0 -> S0-ISCT Mode (1 - S0-ISCT Enable Mode; 0 = S0-ISCT Disable mode)
                    //       bit 1 -> AC Status (1 - On AC, 0 - On Battery); bit 2- 7 -> Reserved
    S3WR,   8,      // 183 Get Platform Wake Reason         (Provided by EC)
    IPNC,  8,       // 184 ISCT Power Enable to Network Card
                    //          184.0   Reserved, Always 0
                    //          184.1   WLAN module powered in S3 (0=Disabled, 1=Enabled)
                    //          184.2   WLAN module powered in S4 (0=Disabled, 1=Enabled)
                    //          184.3   WLAN module powered in S5 (0=Disabled, 1=Enabled)
                    //          184.4   WWAN module powered in S3 (0=Disabled, 1=Enabled)
                    //          184.5   WWAN module powered in S4 (0=Disabled, 1=Enabled)
                    //          184.6   WWAN module powered in S5 (0=Disabled, 1=Enabled)
                    //          184.7   Reserved

    Offset(185),
    D0TL,  8,  // 185     DDR3 local temp, U2F1,ADM1032
    ALT1,  8,  // 186     Ambient local temp, U3A1,ADT7481
    ART2,  8,  // 187     Ambient temp remote 2, U3A1,ADT7481

    Offset(190),
    B1DV, 16,     // 190     Battery 1 Design Voltage (mV)

    Offset(194),
    B1ML, 8,      // 194   BattAPmaxL - Battery Pack A maximum low byte
    B1MH, 8,      // 195   BattAPmaxH - Battery Pack A maximum high byte

    Offset(198),
    BCC1, 16,      // 198   Battery charge rate value
    BTHV, 8,        // 200   Battery critical threshold value
    Offset(206),
    BATL, 8,       //206  Battery Temperature Low
    BATH, 8,       //207  Battery Temperature High
    Offset(214),
    BTPL, 8,       // 214   Battery 1 Trip Point low byte
    BTPH, 8,       // 215   Battery 1 Trip Point high byte
    Offset(217),
    BCLM, 16,      // 217   Battery charge limit
    offset(221),
    OVBS, 2,        // 221.0   OTDID and VBUSSNS.
  }

  // ECMD (Embedded Controller Command)
  //
  // Handle all commands sent to EC by BIOS
  //
  //  Arguments: (1)
  //    Arg0 - EC command
  //  Return Value:
  //    0x00 = Success
  //    0xFF = Failure
  //
  Method(ECMD,1,Serialized)
  {
    If(\ECON)
    {
      While(\_SB.PCI0.LPCB.H_EC.CMDR){Stall(20)}
      Store(Arg0, \_SB.PCI0.LPCB.H_EC.CMDR)
      Return (0x00)
    } Else {
      Return (0xFF)
    }
  }

//  Include("SystemAgent/Als.asl")    //will be enabled later

  // Virtual battery code
  //
  Include("Platform/Bat0Virt.asl")

  // Real battery code
  //
  Include("Platform/Bat1Real.asl")

  // The _REG Method is needed because communication with the EC
  // before the driver is loaded is prohibited in WIN2000/WINXP.

  Name( IGDF, 0 ) // Internal graphics display flag, set externally if display is IGD

  Method(_REG, 2 )
  {
    If(LAnd(LEqual(Arg0,3),LEqual(Arg1,1)))
    {
      // Must be running NT 5.0 OS.

      If(LEqual(ECSP,0))
      {
        Store(0,ECON)
      }
      Else
      { 
        Store(1,ECON)
      }

      // Turn off the CPU Fan if Active Cooling
      //   is disabled.

      If(LEqual(0,ACTT))
      {
        Store(0,CFAN)
      }

      // Update the Ambiant Light Illuminence Values.

//      Store(LUXH,LHIH)
//      Store(LUXL,LLOW)

      // Fix up the Lid State if needed.

      // Save the Lid State in global NVS and IGD OpRegion.
          If (LEqual(LSTE, 0))
          {
            Store(0,\_SB.PCI0.GFX0.CLID)
          }
          If (LEqual(LSTE, 1))
          {
            Store(3,\_SB.PCI0.GFX0.CLID)
          }
          Store(LSTE,LIDS)

      // Unconditionally fix up the Battery and Power State.

      // Initialize the Number of Present Batteries.
      //  0 = Virtual Support
      //  1 = Real Battery 1 is present

      Store(0,BNUM)
      Or(BNUM,ShiftRight(And(B1ST,0x08),3),BNUM)

      // Save the current Power State for later.

      // Initialize the Power State.
      //  BNUM = 0 = Virtual Power State
      //  BNUM > 0 = Real Power State

      If(LEqual(BNUM,0))
      {
        Store(VPWR,PWRS)
      }
      Else
      {
        Store(RPWR,PWRS)
      }
      Store (5, BTHV)
      ECMD (0x34)
      // Perform needed ACPI Notifications.

      PNOT()
    }

   // Perform OTG changes based on EC OpRegion
      OTID()
      VBUT()
  }

  Method(_Q30)    // Real Battery AC Insertion Event.
  {
    P8XH(0,0x30)

    // Set Global Power State = AC Mode.

    Store(1,PWRS)

    // Notify Android BSP charger driver
    If (LEqual(OSID, 1))
    {
      Notify(\_SB.ADP1, 0x80)
    } 
    

    // Perform needed ACPI Notifications.

    PNOT()
  }

  Method(_Q31)    // Real Battery AC Removal Event.
  {
    P8XH(0,0x31)

    // Set Global Power State = Battery Mode.

    Store(0,PWRS)

    // Notify Android BSP charger driver
    If (LEqual(OSID, 1))
    {
      Notify(\_SB.ADP1, 0x80)
    } 
    
    // Perform needed ACPI Notifications.

    PNOT()
  }

  Method(_Q32)    // Real Battery Capacity Change.
  {
    P8XH(0,0x32)

    // Perform needed ACPI Notifications.

    PNOT()
  }

  Method(_Q33)    // Real Battery Insertion/Removal Event.
  {
    P8XH(0,0x33)

    // Initialize the Number of Present Batteries.
    //  0 = Virtual Support
    //  1 = Real Battery 1 is present

    Store(0,BNUM)
    Or(BNUM,ShiftRight(And(B1ST,0x08),3),BNUM)

    // Perform needed ACPI Notifications.

    PNOT()
  }


  Method(_Q51)    // Lid Switch Event.
  {
    P8XH(0,0x51)

    // Update Lid NVS State.
    Store(LSTE,LIDS)

    Store (1, Local0)
    Store (LLID, Local1)
    And(LLID, 0x80, LLID)
    If (LEqual(LLID, 0x80))   // resume from S3
    {
      And (Local1, 0x7F, LLID)
      If (LEqual(LLID, LIDS))
      {
        Store (0, Local0)
      }
    }
    If (LNotEqual(Local0, 0))
    {
      \_SB.PCI0.GFX0.GLID(LIDS)

      Notify(\_SB.LID0,0x80)
    }
  }

  Method(_Q52)    // Hot-Key Event.
  {
    P8XH(0,0x52)

    // This event will be generated whenever a
    // CRTL+ALT+SHIFT+"Next Key" is hit.  This event handler
    // will base code execution on the "Next Key" Scan Code
    // stored in the EC Memory Space.

    P8XH(1,SCAN)


    // "D" Scan Code = 0x20 (=Narrator settings screen)

    If(LEqual(SCAN,0x20))
    {
      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        //
        // Ctrl + Alt + Shift + D test emulation mode
        // 1. Windows press notify 0xC2
        // 2. Volume up press notify 0xC4
        // 3. Volume up release notify 0xC5
        // 4. Windows release notify 0xC3
        //

        Sleep(1000) // 1sec delay is needed for Powerbutton and Windows Home button.

        ^VGBI.UPBT(WINDOWS_BUTTON, One) // Update VBDS
        Notify(^VGBI, 0xC2) // Notify Virtual GPIO driver that Windows Home button is pressed.

        ^VGBI.UPBT(VOLUME_UP_BUTTON, One)
        Notify(^VGBI, 0xC4) // Notify Virtual GPIO driver that Vol up button is pressed.

        ^VGBI.UPBT(VOLUME_UP_BUTTON, Zero)
        Notify(^VGBI, 0xC5) // Notify Virtual GPIO driver that Vol up button is released.

        ^VGBI.UPBT(WINDOWS_BUTTON, Zero)
        Notify(^VGBI, 0xC3) // Notify Virtual GPIO driver that Windows Home button is released.
      }
    }

    // "F" Scan Code = 0x21 (=PrintScreen/ScreenShot)

    ElseIf(LEqual(SCAN,0x21))
    {
      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        //
        // Ctrl + Alt + Shift + F test emulation mode
        // 1. Windows press notify 0xC2
        // 2. Volume Down press notify 0xC6
        // 3. Volume Down release notify 0xC7
        // 4. Windows release notify 0xC3
        //

        Sleep(1000) // 1sec delay is needed for Powerbutton and Windows Home button.

        ^VGBI.UPBT(WINDOWS_BUTTON, One) // Update VBDS
        Notify(^VGBI, 0xC2) // Notify Virtual GPIO driver that Windows Home button is pressed.

        ^VGBI.UPBT(VOLUME_DOWN_BUTTON, One)
        Notify(^VGBI, 0xC6) // Notify Virtual GPIO driver that Vol down button is pressed.

        ^VGBI.UPBT(VOLUME_DOWN_BUTTON, Zero)
        Notify(^VGBI, 0xC7) // Notify Virtual GPIO driver that Vol down button is released.

        ^VGBI.UPBT(WINDOWS_BUTTON, Zero)
        Notify(^VGBI, 0xC3) // Notify Virtual GPIO driver that Windows Home button is released.
      }
    }


    // "G" Scan Code = 0x22 (=Ctrl+Alt+Del)

    ElseIf(LEqual(SCAN,0x22))
    {
      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        //
        // CAS+G test emulation mode
        // 1. Windows press notify 0xC2
        // 2. Powerbutton press notify 0xC0
        // 3. Powerbutton release notify 0xC1
        // 4. Windows release notify 0xC3
        //

        Sleep(1000) // 1sec delay is needed for Powerbutton and Windows Home button
        
        ^VGBI.UPBT(WINDOWS_BUTTON, One)
        Notify(^VGBI, 0xC2) // Notify Virtual GPIO driver that Windows Home button is pressed.
        
        ^VGBI.UPBT(POWER_BUTTON, One)
        Notify(^VGBI, 0xC0) // Notify Virtual GPIO driver that Power button is pressed.
        
        ^VGBI.UPBT(POWER_BUTTON, Zero)
        Notify(^VGBI, 0xC1) // Notify Virtual GPIO driver that Power button is released.
        
        ^VGBI.UPBT(WINDOWS_BUTTON, Zero)
        Notify(^VGBI, 0xC3) // Notify Virtual GPIO driver that Windows Home button is released.
      }

    }

    // "H" Scan Code = 0x23 (=Windows Home)

    ElseIf(LEqual(SCAN,0x23))
    {
      // Windows Button.  If HID Event Driver loaded, use it.  Otherwise, use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        Sleep(1000)
        
        ^VGBI.UPBT(WINDOWS_BUTTON, One)
        Notify(^VGBI, 0xC2) // Notify Virtual GPIO driver that Windows Home button is pressed.
        
        ^VGBI.UPBT(WINDOWS_BUTTON, Zero)
        Notify(^VGBI, 0xC3) // Notify Virtual GPIO driver that Windows Home button is released.
      }
    }

    // "J" Scan Code = 0x24 (=Volume Up)
    
    ElseIf(LEqual(SCAN,0x24))
    {
      // Volume Up.  If HID Event Driver loaded, use it.  Otherwise, use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        Sleep(1000)
        
        ^VGBI.UPBT(VOLUME_UP_BUTTON, One)
        Notify(^VGBI, 0xC4) // Notify Virtual GPIO driver that Volume UP button is pressed.
        
        ^VGBI.UPBT(VOLUME_UP_BUTTON, Zero)
        Notify(^VGBI, 0xC5) // Notify Virtual GPIO driver that Volume UP button is released.
      }
    }

    // "K" Scan Code = 0x25 (Volume Down)

    ElseIf(LEqual(SCAN,0x25))
    {
      // Volume Down.  If HID Event Driver loaded, use it. Otherwise, use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        Sleep(1000)
        
        ^VGBI.UPBT(VOLUME_DOWN_BUTTON, One)
        Notify(^VGBI, 0xC6) // Notify Virtual GPIO driver that Volue Down button is pressed.
        
        ^VGBI.UPBT(VOLUME_DOWN_BUTTON, Zero)
        Notify(^VGBI, 0xC7) // Notify Virtual GPIO driver that Volue Down button is released.
      }
    }

    // "L" Scan Code = 0x26 (Rotation Lock)

    ElseIf(LEqual(SCAN,0x26))
    {
       // Rotation Lock.  If HID Event Driver loaded, use it. Otherwise, use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        Sleep(1000)
        
        ^VGBI.UPBT(ROTATION_LOCK_BUTTON, One)
        Notify(^VGBI, 0xC8)   // Notify Virtual GPIO driver that Rotation Lock button is pressed.

        ^VGBI.UPBT(ROTATION_LOCK_BUTTON, Zero)
        Notify(^VGBI, 0xC9)   // Notify Virtual GPIO driver that Rotation Lock button is released.
      }
    }


    // ":;" Scan Code = 0x27 (switch Slate/Clamshell mode)

    ElseIf(LEqual(SCAN,0x27))
    {
      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One))) // VirtualButton driver is loaded(means it is Winblue)
      {
        Sleep(1000)
        
        If(And(PB1E, 0x08))   // Slave/Laptop Mode changed 0 -> 1 Laptop mode
        {
          P8XH(0,0x1)
          ^VGBI.UPBT(CONVERTIBLE_BUTTON, Zero)
          Notify(^VGBI, 0xCC) 	// Notify Virtual GPIO driver that the system is now in Laptop mode.
        } 
        Else 
        { // Slave/Laptop Mode Changed 1 -> 0 Slate/Tablet Mode                              
          P8XH(0,0x0)
          ^VGBI.UPBT(CONVERTIBLE_BUTTON, One)
          Notify(^VGBI, 0xCD) 	// Notify Virtual GPIO driver that the system is now in Slate/Tablet mode.
        }
        Xor(PB1E, 0x08, PB1E)   // Toggle Slate/Laptop Lock Status
      }

    }

    // "'"" Scan Code = 0x28 (switch Dock/UnDock mode)
    
    ElseIf(LEqual(SCAN,0x28))
    {
      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One)))
      {
        Sleep(1000)
        
        If(And(PB1E, 0x10))    // Dock/Undock status changed 0 -> 1 Dock mode
        {
          P8XH(0,0x1)
          ^VGBI.UPBT(DOCK_INDICATOR, Zero)
          Notify(^VGBI, 0xCB)		// Notify Virtual GPIO driver that the system is now in Dock mode.
        } 
        Else
        {  // Dock/Undock status Changed 1 -> 0 Undock mode                              
          P8XH(0,0x0)
          ^VGBI.UPBT(DOCK_INDICATOR, One)
          Notify(^VGBI, 0xCA)		// Notify Virtual GPIO driver that the system is now in Undock mode.
        }
        Xor(PB1E, 0x10, PB1E)    // Toggle Dock/Undock Status
      }
    }    


    // F1 Scan Code = 0x3B

    If(LEqual(SCAN,0x3B))   // Hot Key Display Switch.
    {
      If(IGDS)
      {
        \_SB.PCI0.GFX0.GHDS(0) // GMCH SCI hotkey display switch, table index 0
      }
    }

    // F2 Scan Code = 0x3C

    If(LEqual(SCAN,0x3C))   // Hot Key Display Switch.
    {
      If(IGDS)
      {
        \_SB.PCI0.GFX0.GHDS(1) // GMCH SCI hotkey display switch, table index 1
      }
    }

    // F3 Scan Code = 0x3D

    If(LEqual(SCAN,0x3D))   // Hot Key Display Switch.
    {
      If(IGDS)
      {
        \_SB.PCI0.GFX0.GHDS(2) // GMCH SCI hotkey display switch, table index 2
      }
    }

    // F4 Scan Code = 0x3E

    If(LEqual(SCAN,0x3E))   // Hot Key Display Switch.
    {
      If(IGDS)
      {
        \_SB.PCI0.GFX0.GHDS(3) // GMCH SCI hotkey display switch, table index 3
      }
    }

    // F5 Scan Code = 0x3F

    // Virtual Battery Hot Keys(VBHK) - VBHK uses keys 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, -, + and F5.

    If(LEqual(BNUM,0))  // Check for Virtual Battery Mode before testing the hot-keys specific to Virtual Battery Support.
    {
      VBHK()  //  remove this call if no virtual battery is needed.
    }


    // F6 Scan Code = 0x40

    If(LEqual(SCAN,0x40))   // Cycle Brightness
    {
      If(IGDS)
      {
        If(CondRefOf(HGAS))
        {
          HGAS()
        }
        Else
        {
          If(And(4,DSEN))
          {
            BRTN(0x85)
          }
        }
      }
    }

    // F7 Scan Code = 0x41

    If(LEqual(SCAN,0x41))   // Zero Brightness
    {
      If(IGDS)
      {
        If(CondRefOf(CHPS))
        {
          CHPS()      //Display the current hybrid policy on Port 80 header
        }
        Else
        {
          If(And(4,DSEN))
          {
            BRTN(0x88)
          }
        }
      }
    }

    // F8 Scan Code = 0x42

    If(LEqual(SCAN,0x42))   // Panel Fitting Hot Key.
    {
      If(IGDS)
      {
        If(CondRefOf(HPFS))
        {
          HPFS()
        }
        Else
        {
          \_SB.PCI0.GFX0.AINT(2, 0)
        }
      }
    }

    // F9 Scan Code = 0x43

    If(LEqual(SCAN,0x43))   // Decrease Brightness Level.
    {
      If(CondRefOf(HBRT))
      {
        HBRT(4)
      }

      If(IGDS)
      {
        If(And(4,DSEN))
        {
          BRTN(0x87)
        }
        Else
        {
          Store(BRTL, Local0)

          // Current brightness is a percentage.
          // This must be done if ALS is enabled,
          // as the driver will change the
          // brightness based on ambient light.
          Store(\_SB.PCI0.GFX0.CBLV, BRTL)

          // 1 % will be lost in the conversion
          // so need to make sure we account for it.
          AND(Add(BRTL, 1),0xFE, BRTL)


          If(LGreaterEqual(BRTL, 10))
          {
            Subtract(BRTL, 10, BRTL)
          }

          \_SB.PCI0.GFX0.AINT(1, BRTL)
        }
      }

    }

    // F10 Scan Code = 0x44

    If(LEqual(SCAN,0x44))   // Increase Brightness Level.
    {
      If(CondRefOf(HBRT))
      {
        HBRT(3)
      }

      If(IGDS)
      {
        If(And(4,DSEN))
        {
          BRTN(0x86)
        }
        Else
        {
          Store(BRTL, Local0)

          // Current brightness is a percentage.
          // This must be done if ALS is enabled,
          // as the driver will change the
          // brightness based on ambient light.
          Store(\_SB.PCI0.GFX0.CBLV, BRTL)

          // 1 % will be lost in the conversion
          // so need to make sure we account for it.
          AND(Add(BRTL, 1),0xFE, BRTL)

          If(LLessEqual(BRTL, 90))
          {
            Add(BRTL, 10, BRTL)
          }

          \_SB.PCI0.GFX0.AINT(1, BRTL)
        }
      }

    }

  } // end Method(_Q52) hot key event


//
// Hardware Buttons.
//
  Method(_Q80)    // Volume Up
  {
      // Volume Up.
      // If VirtualButton driver loaded
      //   use VirtualButton driver
      // Else If HID Event Driver loaded
      //   use HID Event Driver
      // Else
      //   use GFX Driver.
      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One)))
      {
    P8XH(0,0x80)
        ^VGBI.UPBT(VOLUME_UP_BUTTON, One)
        Notify(^VGBI, 0xC4) // Notify Virtual GPIO driver that Volume UP button is pressed.
        ^VGBI.UPBT(VOLUME_UP_BUTTON, Zero)
        Notify(^VGBI, 0xC5) // Notify Virtual GPIO driver that Volume UP button is released.
      } //EndIf

      //Notify Android PIND driver
      If (\_OSI("Android"))
      {
        Notify(\_SB.PIND, 0x80) // VOL_UP event index 
      }
  }

  Method(_Q81)    // Volume Down
  {
      // Volume Down.  
      // If VirtualButton driver loaded
      //   use VirtualButton driver
      // Else If HID Event Driver loaded
      //   use HID Event Driver
      // Else
      //   use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One)))
      {
        P8XH(0,0x81)
        ^VGBI.UPBT(VOLUME_DOWN_BUTTON, One)
        Notify(^VGBI, 0xC6) // Notify Virtual GPIO driver that Volue Down button is pressed.
        ^VGBI.UPBT(VOLUME_DOWN_BUTTON, Zero)
        Notify(^VGBI, 0xC7) // Notify Virtual GPIO driver that Volue Down button is released.
      }
      //Notify Android PIND driver
      If (\_OSI("Android"))
      {
        Notify(\_SB.PIND, 0x81) // VOL_DOWN event index
      }
  }

  Method(_Q85)    // Windows Home button
  {
      // Windows Home Button
      // If VirtualButton driver loaded
      //   use VirtualButton driver
      // Else If HID Event Driver loaded
      //   use HID Event Driver
      // Else
      //   use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One)))
      {
    P8XH(0,0x85)
        ^VGBI.UPBT(WINDOWS_BUTTON, One)
        Notify(^VGBI, 0xC2) // Notify Virtual GPIO driver that Windows Home button is pressed.
        ^VGBI.UPBT(WINDOWS_BUTTON, Zero)
        Notify(^VGBI, 0xC3) // Notify Virtual GPIO driver that Windows Home button is released.
      }

      //Notify Android PIND driver
      If (\_OSI("Android"))
      {
        Notify(\_SB.PIND, 0x82) // HOME_BUTTON event index
      }
  }

  Method(_Q86)    // Power button
  {
      // Power button
      // If VirtualButton driver loaded
      //   use VirtualButton driver
      // Else If HID Event Driver loaded
      //   use HID Event Driver
      // Else
      //   use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One)))
      {
    P8XH(0,0x86)
        ^VGBI.UPBT(POWER_BUTTON, One)
        Notify(^VGBI, 0xC0) // Notify Virtual GPIO driver that Power button is pressed.
        ^VGBI.UPBT(POWER_BUTTON, Zero)
        Notify(^VGBI, 0xC1) // Notify Virtual GPIO driver that Power button is released.
      }
  }

  Method(_Q87)    // Rotation Lock button
  {
      // Rotation Lock button
      // If VirtualButton driver loaded
      //   use VirtualButton driver
      // Else If HID Event Driver loaded
      //   use HID Event Driver
      // Else
      //   use GFX Driver.

      If(LAnd(CondRefOf(^VGBI._STA),LEqual(And(^VGBI._STA(),One),One)))
      {
    P8XH(0,0x87)
        ^VGBI.UPBT(ROTATION_LOCK_BUTTON, One)
        Notify(^VGBI, 0xC8) // Notify Virtual GPIO driver that Rotation Lock button is pressed.
        ^VGBI.UPBT(ROTATION_LOCK_BUTTON, Zero)
        Notify(^VGBI, 0xC9) // Notify Virtual GPIO driver that Rotation Lock button is released.
      }
  }

  Method(_QD5)    // 10 second power button press.
  {
    P8XH(0,0xD5)
    If(LEqual(\_SB.PWRB.PBST, 0x1)) { // Using Control Method Power Button
      Notify(\_SB.PWRB, 0x80) // Send press notification to Power Button device
      Store(1, \_SB.PWRB.PBLV) // Keep track of Power Button Level
    }
    
  }// End of Method

  Method(_QD6)    // 10 second power button de-press.
  {
    P8XH(0,0xD6)
    If(LEqual(\_SB.PWRB.PBST, 0x1)) { // Using Control Method Power Button
      \_SB.PWRB.PBUP() // Send release notification to Power Button device if requested
      Store(0, \_SB.PWRB.PBLV) //// Keep track of Power Button Level
    }
    
  }// End of Method

  Method(_QD7)    // OTGID Toggle
  {
    P8XH(0,0xD7)
    OTID()
  }// End of Method


  Method(_QD8)    // VBUSSNS Toggle
  {
    P8XH(0,0xD8)
    VBUT()
  }// End of Method


  Method(_QF0)    // Thermal Event.
  {
    ADBG("_QF0 SCI")
    Notify(\_TZ.TZ01,0x80)
  }


  Method(OTID,0,Serialized)
  {
    ADBG ( OTGM )
    If(LNotEqual(OTGM, 0))
    {
      AND(\_SB.PCI0.XHC1.BAR0, 0xFFFFFFF0, Local0)
      If(LAnd(LNotEqual(Local0, 0), LNotEqual(Local0, 0xFFFFFFFF)))
      {
        OperationRegion(XBAR, SystemMemory, Local0, 0x80E0)
        Field(XBAR, DWordAcc, NoLock, Preserve)
        {
          Offset(0x80D8),
          OTGR, 32,         // OTG MUX to set Host/Device Mode
          OTGS, 32          // OTG MUX to verify complete transition
         }
            
        OperationRegion(PBKL, SystemMemory, 0x0FED88000, 0x8000)
        Field(PBKL, DWordAcc, Lock, Preserve) {
          Offset(0x5400),                 // USB3_BP0_PWR_EN  to GPIO N60: PANEL0_BKLTEN
          ,     1,
          USBP, 1
        }
      
        AND(OVBS, 0x1, Local1)
        If(LEqual(Local1,0x0))   // OTGID-0 Host Mode
        {
          AND(OTGR, 0xFEEFFFFF, OTGR)  // Clear Bit 20 Host Mode and Bit 24 VBUSSNS
          Store(200, Local2)    // 200 * 50us = 10ms.
          While( Local2 ) 
          {
            If(And(OTGS,0x20000000))    //Activate VBUS after  Host mode is set
            {
              Store(0,Local2)
            } else {
              Decrement(Local2)
            }
          }
          Store(1,USBP)
          Notify(\_SB.PCI0.UOTG, 0x80)
        }
      
        If(LEqual(Local1,0x1))   //Cable Disconnet
        {
          Store(0,USBP)
          AND(OTGR, 0xFEFFFFFF, OTGR)
          OR(OTGR, 0x00100000, OTGR)
        }
      }
    }
  }
  
  Method(VBUT,0,Serialized)
  {
    If(LNotEqual(OTGM, 0))
    {
      AND(\_SB.PCI0.XHC1.BAR0, 0xFFFFFFF0, Local0)
      If(LAnd(LNotEqual(Local0, 0), LNotEqual(Local0, 0xFFFFFFFF)))
      {
        OperationRegion(XBAR, SystemMemory, Local0, 0x80E0)
        Field(XBAR, DWordAcc, NoLock, Preserve)
        {
          Offset(0x80D8),
          OTGR, 32,         // OTG MUX to set Host/Device Mode
          OTGS, 32          // OTG MUX to verify complete transition
        }
        
        OperationRegion(PBKL, SystemMemory, 0x0FED88000, 0x8000)
        Field(PBKL, DWordAcc, Lock, Preserve) {
          Offset(0x5400),                 // USB3_BP0_PWR_EN  to GPIO N60: PANEL0_BKLTEN
          ,     1,
          USBP, 1
        }
      
        AND(OVBS, 0x1, Local1)  // OTG ID
        AND(OVBS, 0x2, Local2)  // VBUSSNS
        
        If(LEqual(Local1, 0x1)) {               //Cable Disconnect
          If(LEqual(Local2, 0x0)) { 
            Store(0,USBP)
            AND(OTGR, 0xFEFFFFFF, OTGR)
            OR(OTGR, 0x00100000, OTGR)
            Notify(\_SB.PCI0.UOTG, 0x83)
          }
          If(LEqual(Local2, 0x2)) {             //Device Mode
            Store(0,USBP)
            OR(OTGR, 0x01000000, OTGR)
            OR(OTGR, 0x00100000, OTGR)
            Notify(\_SB.PCI0.UOTG, 0x82)
          }
        }
      }
    }
  }
  
  //
  // Hardware Button Array support
  //
  Device(BIND)  // Button Indicators.
  {
     Name(_HID, "INT33D2")
     Name(_CID, "PNP0C40")

     Method(_STA, 0,Serialized)
     {
       If(LGreaterEqual(OSYS, 2013))
       {
         If(And(PB1E, 1)) // Virtual Power Button is enabled?
         {
           Return(0x000F)
           
         }
         Else
         {
           Return(0x000B) // Not visible in UI
         }
       }
       Return(0x00)
     }
     //
     // _DSM : Device Specific Method for the Windows Compatible Button Array.
     //
     // Arg0: UUID Unique function identifier
     // Arg1: Integer Revision Level
     // Arg2: Integer Function Index
     // Arg3: Package Parameters
     //
     Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj})
     {
       // Compare passed in UUID to supported UUID.

       If (LEqual(Arg0, ToUUID ("DFBCF3C5-E7A5-44E6-9C1F-29C76F6E059C")))
       {
         If (LEqual(0,ToInteger(Arg1)))        // Revision 0.
         {
           Switch (ToInteger(Arg2))            // Switch to Function Index.
           {
             //
             // Function 0, Query of supported functions.
             //

             Case (0)
             {
               Return (Buffer() {0x03})
             }

             //
             // Function 1, Windows Compatible Button Array Power Button Properties.
             //

             Case (1)
             {
               // Only return support if platform enabled via setup.

               If(LEqual(And(PB1E, 1), 1))
               {
                 Return (0x07)
               }

               Return(0x00)

             }
           } // End Switch statement
         }  // End Revision check
       }  // End UUID check

       // If the code falls through to this point, just return a buffer of 0.

       Return (Buffer() {0x00})

    }  // End _DSM Method
  }
  
  Device(CIND)  // Convertible Indicators.
  {
     Name(_HID, "INT33D3")
     Name(_CID, "PNP0C60")
    
     Method(_STA, 0,Serialized)
     {
       //If(LAnd(And(IUCE,1), LGreaterEqual(OSYS, 2012)))
       If(LGreaterEqual(OSYS, 2012))
       {
         If(And(PB1E, 1)) // Virtual Power Button is enabled?
         {
           Return(0x000F)
         }
       }
       Return(0x00)
     }
  }

  Device(DOCK)  // docking/undocking.
  {
    Name(_HID, "INT33D4")
    Name(_CID, "PNP0C70")	        	// define for dock
    
    Method(_STA, 0,Serialized)
    {
      If(LGreaterEqual(OSYS, 2012))
      {
        Return(0x000F)
       }
      Return(0x00)
    }
  }

  Device(VGBI)                                  // Virtual GPIO Button "Interface."
  {
  
    Name(_HID, EISAID("INT33D6"))               // HID for Intel Virtual GPIO Button Interface.
    Name(VBDS,0)
    Name(ONTM,0)                                // Temporary variable to initialise VBDS only once in the boot
    
    Method(_STA,0,Serialized)
    {
      If(LGreaterEqual(OSYS,2013))              // Virtual Button applicable for Winblue and higher version of WinOS
      {
        If(And(PB1E, 1))                        // Virtual Power Button is enabled?
        {
          Return(0x0F)                          // Device present,enabled and should be shown in UI.
        } 
      }
      Return(0x00)                              // Device NOT present
    }// End of _STA

    Method(VBDL,0,Serialized) // Virtual Button Driver Load - Refer Bios Arch Spec
    {
     //10s power button is not supported, so simply ignore it
     
        //
        // Here we know V-GPIO driver is loaded but 10s PB is disabled.
        // Set PBST so that the default power button can handle the request. 
        //
//        If(CondRefOf(\_SB.PWRB.PBST))
//      {
//          Store(1, \_SB.PWRB.PBST)
//          Notify(\_SB.PWRB, 1) // Device check
//      }
    }

    Method(VGBS,0,Serialized)  // Virtual GPIO Button Status - Refer Bios Arch Spec
    {
      // Bit[7] : Docking Indicatory Status  -- 0 (No Docking support on BBAY)
      // Bit[6] : Convertible/Slate Indicator Status  -- 0 (Slate Mode)
      // Bit[5] : Reserved  -- 0
      // Bit[4] : Rotation Lock Button Status 
      // Bit[3] : Volume Down Button Status
      // Bit[2] : Volume Up Button Status
      // Bit[1] : Windows Home Button Status
      // Bit[0] : Power Button Status

      If(LEqual(ONTM,0))
      {
        // Initial setup option on VBDS and thereafter hotkeys should be updating the VBDS
        if(LEqual(And(PB1E,0x08),0x08)) // Slate/Laptop(CalmShell)
        {
          UPBT(CONVERTIBLE_BUTTON,One)
        }
        Else
        {
          UPBT(CONVERTIBLE_BUTTON,Zero)
        }

        if(LEqual(And(PB1E,0x10),0x10))  // Undock/Dock
        {
          UPBT(DOCK_INDICATOR,One)
        }
        Else
        {
          UPBT(DOCK_INDICATOR,Zero)
        }
        Store(One,ONTM)
      } 
      Return(VBDS)
    }// End of Method

    //
    // UPBT Update Button Status
    //
    // Arg0: Bit location of the target button
    //       0: Power Button
    //       1: Windows Button
    //       2: Volume up Button
    //       3: Volume down Button
    //       4: Rotation Lock Button
    //       5: Reserved
    //       6: Convertible state 0 - Slate, 1 - Notebook
    //       7: Dock Indicator 0 - Undock, 1 - Dock
    //       
    // Arg1: On/Off state, 0 - Clear the target bit, 1 - Set the target bit.
    // 
    Method(UPBT,2,Serialized)  // Update Button Status 
    {
      ShiftLeft(One, Arg0, Local0)
      if(Arg1){                // Button Press/ON
        Or(VBDS, Local0, VBDS)
      } Else {                 // Button Press/OFF
        And(VBDS, Not(Local0),VBDS) 
      }
    } // End of UPBT

  } // End of VGBI


}       // Device H_EC

// System Bus
Scope(\_SB)
{
  // Define AC Device
  Device(ADP1)
  {
    Name(_HID,"ACPI0003")    //only one ACPI0003 is allowed in system

    Method(_STA)
    {
      If (LEqual(ECON,1)){
        Return(0x0F)
      }
      Return(0x00)
    }

    // Return the value that determines if running
    // from AC or not.

    Method(_PSR,0)
    {
      Return(PWRS)
    }

    // Return that everything runs off of AC.
    Method(_PCL,0)
    {
      Return(\_SB)
    }

  } //Device(ADP1)


  // Define a Lid Switch.
  Device(LID0)
  {
    Name(_HID,EISAID("PNP0C0D"))

    Method(_STA)
    {
      If(LEqual(ECON,1)){
        Return(0x0F)
      }
      Return(0x00)
    }

    Method(_LID,0)
    {
      // 0 = Closed, 1 = Open.

      Return(\_SB.PCI0.LPCB.H_EC.LSTE)
    }
  }


}//end scope _SB


