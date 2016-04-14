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

DefinitionBlock (
    "Ssdt.aml",
    "SSDT",
    0x02,       // revision.
                // A Revision field value greater than or equal to 2 signifies that integers 
                // declared within the Definition Block are to be evaluated as 64-bit values
    "INTEL",    // OEM ID (6 byte string)
    "BSW_RC",   // OEM table ID  (8 byte string)
    0x0         // OEM version of DSDT table (4 byte Integer)
    )
{

  External(\_SB.GPO1, DeviceObj)
  External(\_SB.PCI0, DeviceObj)
  External(\_SB.PCI0.URT1, DeviceObj)
  External(\_SB.PCI0.URT2, DeviceObj)
  External(\_SB.PCI0.SPI1, DeviceObj)
  External(\_SB.PCI0.I2C1, DeviceObj)
  External(\_SB.PCI0.I2C2, DeviceObj)
  External(\_SB.PCI0.I2C3, DeviceObj)
  External(\_SB.PCI0.I2C4, DeviceObj)
  External(\_SB.PCI0.I2C5, DeviceObj)
  External(\_SB.PCI0.I2C6, DeviceObj)
  External(\_SB.PCI0.I2C7, DeviceObj)
  External(\_SB.PCI0.SDHB, DeviceObj)
  External(CDSM, MethodObj)

  Scope(\_SB.PCI0.URT1)
  {
    include("Device/Bluetooth/Broadcom.asl")
  }

  Scope(\_SB.PCI0.URT2)
  {
    include("Device/Gps/Broadcom.asl")
  }

  Scope(\_SB.PCI0.SPI1)
  {
    include("Device/FingerPrint/AuthenTec.asl")
  }

  Scope(\_SB.PCI0)
  {
    include("Device/Nfc/Nxp2.asl")
  }

  Scope(\_SB.PCI0.I2C1)
  {
    include("Device/SensorHub/STMicroelectronics.asl")
  }

  Scope(\_SB.PCI0.I2C2)
  {
    include("Device/Audio/Realtek.asl")
    include("Device/Rfid/Impinj.asl")
  }

  Scope(\_SB.PCI0.I2C3)
  {
    include("Device/Camera/Camera.asl")
  }

  Scope(\_SB.PCI0.I2C4)
  {
    include("Device/Camera/Camera2.asl")
  }

  Scope(\_SB.PCI0.I2C5)
  {
    include("Device/TouchPad/Synaptics.asl")
  }

  Scope(\_SB.PCI0.I2C6)
  {
    include("Device/TouchPad/Atmel.asl")
    include("Device/TouchPad/Synaptics2.asl")
  }

  Scope(\_SB.PCI0.I2C7)
  {
    include("Device/Nfc/Nxp1.asl")
  }
  
  Scope(\_SB.PCI0)
  {
    include("Device/Audio/Audio.asl")
  }

  Scope(\_SB.PCI0.SDHB)
  {
    include("Device/Wifi/Broadcom.asl")
  }
}