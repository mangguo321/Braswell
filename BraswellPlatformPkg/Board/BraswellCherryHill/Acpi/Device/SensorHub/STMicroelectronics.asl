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

  External(\GBA2, IntObj)
  External(\_SB.GPO2.AVBL, IntObj)
  External(\_SB.GPO2.SHD3, IntObj)

    //
    // Sensor hub
    //
    Device(SHUB)
    {
      Name (_ADR, 0x0)
      Name (_HID, "SMO91D0")
      Name (_CID, "PNP0C50")
      Name (_DDN, "STM32F103RD")
      Name (_UID, 0x1)
      Name (_S0W, 3)                             // Put device to 0=D0, 3=D3 Hot, 4=D3 Cold during S0 idle
      Name (_DEP, Package() {\_SB.GPO2})
      Method (_DSM, 0x4, Serialized) {
        Return (CDSM (Arg0, Arg1, Arg2, Arg3, 1)) // HID Descriptor Address = 1 (IHV Specific)
      }
      Method (_STA, 0x0, NotSerialized)
      {
        Return (0x0)
      }

      OperationRegion (GPM2, SystemMemory, Or(\GBA2, 0x4820), 4)
      Field (GPM2, DWordAcc, Lock, Preserve) {
        ,     1,
        SHD3, 1                                  // [1] GPIO Tx State of  GPIO E19: SENSOR_HUB_RESET
      }

      Method (_PS3, 0, Serialized) {
        If (\_OSI ("Android")) {
          If (LEqual (\_SB.GPO2.AVBL, 1)) {
            Store (0x00, \_SB.GPO2.SHD3 )        // SENSOR_HUB_RESET = 0 puts the device in reset state
          }
        } Else {
          Store (0, ^SHD3)                       // SENSOR_HUB_RESET = 0 for WOS
        }
      }
      Method (_PS0, 0, Serialized) {
        If (\_OSI ("Android")) {
          If (LEqual (\_SB.GPO2.AVBL, 1)) {
            Store (0x01, \_SB.GPO2.SHD3 )        // SENSOR_HUB_RESET = 1 put the device to normal state
          }
        } Else {
          Store (1, ^SHD3)                       // SENSOR_HUB_RESET = 1 for WOS
        }
        Sleep (100)
      }

      Method(_CRS, 0x0, NotSerialized)
      {
        Name(SBUF,ResourceTemplate () {
        I2CSerialBus (0x40,                      // SlaveAddress: bus address
             ,                                   // SlaveMode: default to ControllerInitiated
             400000,                             // ConnectionSpeed: in Hz
             ,                                   // Addressing Mode: default to 7 bit
             "\\_SB.PCI0.I2C1",                  // ResourceSource: I2C bus controller name
             ,                                   // Descriptor Name: creates name for offset of resource descriptor
             )                                   // VendorData
         //Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {67}
         GpioInt(Level, ActiveLow, Exclusive, PullUp, 0, "\\_SB.GPO2", ) {22}               // E22, SENSOR_HUB_INT
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO2", ) {19} // E19, SENSOR_HUB_RESET
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO2", ) {16} // E16, SENSOR_HUB_WAKE
        })
        Return (SBUF)
      }
    } // Device SHUB


