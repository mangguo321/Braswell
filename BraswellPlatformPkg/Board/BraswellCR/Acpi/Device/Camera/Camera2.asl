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

  External(CLK0, PowerResObj)
  External(CLK1, PowerResObj)

  //
  // Device CAM1-----Back Camera
  //
  Device (CAM1)
  {
    Name(_ADR, 0x00)
    Name(_HID, "INTCF1A")
    Name(_CID, "INTCF1A")
    Name(_SUB, "INTL0000")
    Name(_DDN, "Sony IMX175")
    Name(_UID, 0x01)

    Name (_PR0, Package (1)                // _PR0: Power Resources for D0
    {
        CLK0
    })

    Name (PLDB, Package(1)
    {
      Buffer(0x14)
      {
        0x82,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x69,0x0c,0x00,0x00,
        0x03,0x00,0x00,0x00,
        0xFF,0xFF,0xFF,0xFF
      }
    })

    Method(_PLD,0,Serialized)
    {
      Return (PLDB)
    }

    Method (_STA, 0, NotSerialized) {
      If (LEqual (RCAM, 0)) {                          // Cynthiana_A IMX175 + OV2722
        Return (0x0F)
      }
      Return (0)
    }

    Method (_CRS, 0x0, Serialized)
    {
      Name(SBUF,ResourceTemplate ()
      {
        //GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {49}   // MUX_CAM1_PWRDWN
        GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {52}     // CAM_1_RST_N
        //GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {48}   // MUX_CAM_ACT_LED

        I2CSerialBus(0x10,                 //SlaveAddress: bus address
                     ,                       //SlaveMode: default to ControllerInitiated
                     400000,                 //ConnectionSpeed: in Hz
                     ,                       //Addressing Mode: default to 7 bit
                     "\\_SB.PCI0.I2C4",      //ResourceSource: I2C bus controller name
                     ,                       //ResourceSourceIndex: defaults to 0
                     ,                       //ResourceUsage: Defaults to ResourceConsumer
                     ,                       //Descriptor Name: creates name for offset of resource descriptor
                    )                        //VendorData
        I2CSerialBus(0xC,                  //SlaveAddress: bus address
                     ,                       //SlaveMode: default to ControllerInitiated
                     400000,                 //ConnectionSpeed: in Hz
                     ,                       //Addressing Mode: default to 7 bit
                     "\\_SB.PCI0.I2C4",      //ResourceSource: I2C bus controller name
                     ,                       //ResourceSourceIndex: defaults to 0
                     ,                       //ResourceUsage: Defaults to ResourceConsumer
                     ,                       //Descriptor Name: creates name for offset of resource descriptor
                    )                        //VendorData
        I2CSerialBus(0x54,                 //SlaveAddress: bus address
                     ,                       //SlaveMode: default to ControllerInitiated
                     400000,                 //ConnectionSpeed: in Hz
                     ,                       //Addressing Mode: default to 7 bit
                     "\\_SB.PCI0.I2C4",      //ResourceSource: I2C bus controller name
                     ,                       //ResourceSourceIndex: defaults to 0
                     ,                       //ResourceUsage: Defaults to ResourceConsumer
                     ,                       //Descriptor Name: creates name for offset of resource descriptor
                    )                        //VendorData
      })
      Return (SBUF)
    }

    Method(_DSM, 0x4, NotSerialized)
    {
      If(LEqual(Arg0, ToUUID("377BA76A-F390-4AFF-AB38-9B1BF33A3015")))   // HWID
      {
        Return("INTCF1A");
      }
      If(LEqual(Arg0, ToUUID("3C62AAAA-D8E0-401A-84C3-FC05656FA28C")))   // Sensor CMOS Name
      {
        Return("IMX175")
      }
      If(LEqual(Arg0, ToUUID("822ACE8F-2814-4174-A56B-5F029FE079EE")))   // module name
      {
        Return("13P2BA807")
      }
      If(LEqual(Arg0, ToUUID("2959512A-028C-4646-B73D-4D1B5672FAD8")))   // Customer/platform info string
      {
        Return("INTEL_RVP")
      }
      If(LEqual(Arg0, ToUUID("918AB242-C37C-450A-9D0F-F47AB97C3DEA")))   // MIPI lanes count
      {
        Return(0x0104) // 4 lanes
      }
      If(LEqual(Arg0, ToUUID("EA3B7BD8-E09B-4239-AD6E-ED525F3F26AB")))   //MIPI Port
      {
        Return(0x01)  // csi portx4
      }
      If(LEqual(Arg0, ToUUID("B65AC492-9E30-4D60-B5B2-F497C790D9CF")))   //DIR
      {
        Return(0x0)  // degree 0
      }
      If(LEqual(Arg0, ToUUID("E770AB0F-2644-4BAB-8628-D62F1683FB9D")))   //ROM
      {
        Return(0x2)  // eeprom
      }
      If(LEqual(Arg0, ToUUID("1EA54AB2-CD84-48CC-9DD4-7F594EC3B015")))   // old power option
      {
        Return(0x00)
      }
      If(LEqual(Arg0, ToUUID("8DBE2651-70C1-4C6F-AC87-A37CB46E4AF6")))   // old mclk option
      {
        Return(0x00)
      }
      If(LEqual(Arg0, ToUUID("75C9A639-5C8A-4A00-9F48-A9C3B5DA789F")))   //Reserved, default return 0x0
      {
        Return(0x0)
      }
      If(LEqual(Arg0, ToUUID("26257549-9271-4CA4-BB43-C4899D5A4881")))   //I2c
      {
        if(LEqual(Arg2, 1))   // Count
        {
          Return(0x3);
        }
        if(LEqual(Arg2, 2))   // Function 1 - general
        {
          Return(0x04001000);
        }
        if(LEqual(Arg2, 3))   // Function 2 - vcm
        {
          Return(0x04000c01);
        }
        if(LEqual(Arg2, 4))   // Function 3 - eeprom
        {
          Return(0x04005402);
        }
      }
      If(LEqual(Arg0, ToUUID("79234640-9E10-4FEA-A5C1-B5AA8B19756F")))   //GPIO
      {
        if(LEqual(Arg2, 1))   // Count
        {
          Return(0x1);
        }
        if(LEqual(Arg2, 2))   // Function 1 - RST
        {
          Return(0x01001800);
        }
      }
      Return(0x00)
    }
  }  // End CAM1

  //
  // Device CAM2---Front Camera
  //
  Device (CAM2)
  {
    Name(_ADR, 0x00)
    Name(_HID, "INT33FB")
    Name(_CID, "INT33FB")
    Name(_SUB, "INTL0000")
    Name(_DDN, "OV2722")
    Name(_UID, 0x01)
    Name (_PR0, Package (1)                // _PR0: Power Resources for D0
    {
        CLK1
    })

    Name (PLDB, Package(1)
    {
      Buffer(0x14)
      {
        0x82,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x61,0x0c,0x00,0x00,
        0x03,0x00,0x00,0x00,
        0xFF,0xFF,0xFF,0xFF
      }
    })

    Method(_PLD,0,Serialized)
    {
      Return (PLDB)
    }

    Method (_STA, 0, NotSerialized)
    {
      If (LEqual (RCAM, 0)) {                          // Cynthiana_A IMX175 + OV2722
        Return (0x0F)
      }
      Return (0)
    }

    Method (_CRS, 0x0, Serialized)
    {
      Name(SBUF,ResourceTemplate ()
      {
        GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {54}    //MUX_CAM2_PWRDWN
        GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {50}    //CAM_2_RST_N
        I2CSerialBus(0x36,                 //SlaveAddress: bus address
                     ,                       //SlaveMode: default to ControllerInitiated
                     400000,                 //ConnectionSpeed: in Hz
                     ,                       //Addressing Mode: default to 7 bit
                     "\\_SB.PCI0.I2C4",      //ResourceSource: I2C bus controller name
                     ,                       //ResourceSourceIndex: defaults to 0
                     ,                       //ResourceUsage: Defaults to ResourceConsumer
                     ,                       //Descriptor Name: creates name for offset of resource descriptor
                    )                        //VendorData
      })
      Return (SBUF)
    }

    Method(_DSM, 0x4, NotSerialized)
    {
      If(LEqual(Arg0, ToUUID("377BA76A-F390-4AFF-AB38-9B1BF33A3015")))     // HWID
      {
        Return("INT33FB");
      }
      If(LEqual(Arg0, ToUUID("3C62AAAA-D8E0-401A-84C3-FC05656FA28C")))     // Sensor CMOS Name
      {
        Return("OV2722")
      }
      If(LEqual(Arg0, ToUUID("822ACE8F-2814-4174-A56B-5F029FE079EE")))     // module name
      {
        Return("12P2SF220")
      }
      If(LEqual(Arg0, ToUUID("2959512A-028C-4646-B73D-4D1B5672FAD8")))     // Customer/platform info string
      {
        Return("INTEL_RVP")
      }
      If(LEqual(Arg0, ToUUID("918AB242-C37C-450A-9D0F-F47AB97C3DEA")))     // MIPI lanes count
      {
        Return(0x0101)  // 1 lanes
      }
      If(LEqual(Arg0, ToUUID("EA3B7BD8-E09B-4239-AD6E-ED525F3F26AB")))     // MIPI Port
      {
        Return(0x00)    // csi portx1
      }
      If(LEqual(Arg0, ToUUID("B65AC492-9E30-4D60-B5B2-F497C790D9CF")))     // DIR
      {
        Return(0x0)     // degree 0
      }
      If(LEqual(Arg0, ToUUID("E770AB0F-2644-4BAB-8628-D62F1683FB9D")))     // ROM
      {
        Return(0x0)     // none
      }
      If(LEqual(Arg0, ToUUID("1EA54AB2-CD84-48CC-9DD4-7F594EC3B015")))     // old power option
      {
        Return(0x00)
      }
      If(LEqual(Arg0, ToUUID("8DBE2651-70C1-4C6F-AC87-A37CB46E4AF6")))     // old mclk option
      {
        Return(0x01)
      }
      If(LEqual(Arg0, ToUUID("75C9A639-5C8A-4A00-9F48-A9C3B5DA789F")))     // Reserved, default return 0x0
      {
        Return(0x0)
      }
      If(LEqual(Arg0, ToUUID("26257549-9271-4CA4-BB43-C4899D5A4881")))     // I2C
      {
        if(LEqual(Arg2, 1))   // Count
        {
          Return(0x1);
        }
        if(LEqual(Arg2, 2))   // Function 1 - general
        {
          Return(0x04003600);
        }
      }
      If(LEqual(Arg0, ToUUID("79234640-9E10-4FEA-A5C1-B5AA8B19756F")))     // GPIO
      {
        if(LEqual(Arg2, 1))   // Count
        {
          Return(0x2);
        }
        if(LEqual(Arg2, 2))   // Function 1 - RST
        {
          Return(0x01001600);
        }
        if(LEqual(Arg2, 3))   // Function 2 - PWDN
        {
          Return(0x01001901);
        }
      }
      Return(0x00)
    }
  } // End CAM2
  
    //
    // Device STRA
    //
    Device (STRA)
    {
      Name(_ADR, 0x00)
      Name(_HID, "INTCF1C")
      Name(_CID, "INTCF1C")
      Name(_SUB, "INTL0000")
      Name(_DDN, "Flash LM3554")
      Name(_UID, 0x01)
      //Return the PowerResource of D0 & D3, used for OS
      //Name(_PR0,Package(){P28X, P18X})

      Method (_STA, 0, NotSerialized) {
        If (LEqual (RCAM, 0)) {                          // Flash for Cynthiana_A IMX175 + OV2722
          Return (0x0F)
        }
        Return (0)
      }

      Method (_CRS, 0x0, Serialized)
      {
        Name(SBUF,ResourceTemplate ()
        {
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {56}       // MUX_FLASH_TRIG
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {47}       // MUX_FLASH_TORCH
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {53}       // MUX_FLASH_RESET_N
          GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {46}       // MUX_GP_CAMERASB02
          I2CSerialBus(0x53,                   //SlaveAddress: bus address
                       ,                     //SlaveMode: default to ControllerInitiated
                       400000,                 //ConnectionSpeed: in Hz
                       ,                       //Addressing Mode: default to 7 bit
                       "\\_SB.PCI0.I2C4",      //ResourceSource: I2C bus controller name
                       ,                       //ResourceSourceIndex: defaults to 0
                       ,                       //ResourceUsage: Defaults to ResourceConsumer
                       ,                       //Descriptor Name: creates name for offset of resource descriptor
                      )                        //VendorData
        })
        Return (SBUF)
      }

      Method(_DSM, 0x4, NotSerialized)
      {
        If(LEqual(Arg0, ToUUID("377BA76A-F390-4AFF-AB38-9B1BF33A3015")))   //HWID
        {
          Return("INTCF1C");
        }
        If(LEqual(Arg0, ToUUID("822ACE8F-2814-4174-A56B-5F029FE079EE")))   //module name
        {
          Return("LM3554")
        }
        If(LEqual(Arg0, ToUUID("2959512A-028C-4646-B73D-4D1B5672FAD8")))   //Customer/platform info string
        {
          Return("INTEL_RVP")
        }
        If(LEqual(Arg0, ToUUID("75C9A639-5C8A-4A00-9F48-A9C3B5DA789F")))   //Reserved, default return 0x0
        {
          Return(0x0)
        }
        If(LEqual(Arg0, ToUUID("26257549-9271-4CA4-BB43-C4899D5A4881")))   //I2c
        {
          if(LEqual(Arg2, 1))   // Count
          {
            Return(0x1);
          }
          if(LEqual(Arg2, 2))   // Function 1 - general
          {
            Return(0x04005300);
          }
        }
        If(LEqual(Arg0, ToUUID("79234640-9E10-4FEA-A5C1-B5AA8B19756F")))   //GPIO
        {
          if(LEqual(Arg2, 1))   // Count
          {
            Return(0x4);
          }
          if(LEqual(Arg2, 2))   // Function 1 - Strobe
          {
            Return(0x01001302);
          }
          if(LEqual(Arg2, 3))   // Function 2 - Torch
          {
            Return(0x01001403);
          }
          if(LEqual(Arg2, 4))   // Function 3 - Strobe
          {
            Return(0x01001001);
          }
          if(LEqual(Arg2, 5))   // Function 4 - Torch
          {
            Return(0x01001104);
          }
        }
        Return(0x00)
      }
    } //End STRA

  //
  // Device CAM3-----Rear Camera
  //
            Device (CAM3)
            {
                Name (_ADR, Zero)  // _ADR: Address
                Name (_HID, "INT33BE")  // _HID: Hardware ID
                Name (_CID, "INT33BE")  // _CID: Compatible ID
                Name (_SUB, "INTL0000")  // _SUB: Subsystem ID
                Name (_DDN, "OV5693")  // _DDN: DOS Device Name
                Name (_UID, One)  // _UID: Unique ID
                Name (_PR0, Package (1)                // _PR0: Power Resources for D0
                {
                  CLK1
                })
                Name (PLDB, Package (0x01)
                {
                    Buffer (0x14)
                    {
                        /* 0000 */   0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        /* 0008 */   0x69, 0x0C, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
                        /* 0010 */   0xFF, 0xFF, 0xFF, 0xFF
                    }
                })
                Method (_PLD, 0, Serialized)           // _PLD: Physical Location of Device
                {
                    Return (PLDB)
                }

                Method (_STA, 0, NotSerialized)        // _STA: Status
                {
                  If (LEqual (RCAM, 1)) {              // CRD_B v2.1 OV5693 + OV2724
                    Return (0x0F)
                  }
                  Return (0)
                }

                Method (_CRS, 0, NotSerialized)        // _CRS: Current Resource Settings
                {
                    Name (SBUF, ResourceTemplate ()    // I2C Resource define
                    {
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x0032		       // GPIO N"50"
                            }
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x0033		       // GPIO dvdd12 camerasb03 N"51"
                            }
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x0038		       // GPIO dvdd18 camerasb04 N"56"
                            }
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x002D		       // GPIO avdd28 camerasb08 N"45"
                            }
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x0031                 // GPIO vcm camerasb06 N"49"
                            }
                        I2cSerialBus (0x0010, ControllerInitiated, 0x00061A80,
                            AddressingMode7Bit, "\\_SB.PCI0.I2C4",
                            0x00, ResourceConsumer, ,
                            )
                        I2cSerialBus (0x000C, ControllerInitiated, 0x00061A80,
                            AddressingMode7Bit, "\\_SB.PCI0.I2C4",
                            0x00, ResourceConsumer, ,
                            )
                        I2cSerialBus (0x0054, ControllerInitiated, 0x00061A80,
                            AddressingMode7Bit, "\\_SB.PCI0.I2C4",
                            0x00, ResourceConsumer, ,
                            )
                    })
                    Return (SBUF)
                }

                Method (_DSM, 4, NotSerialized)  // _DSM: Device-Specific Method
                {
                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x6A, 0xA7, 0x7B, 0x37, 0x90, 0xF3, 0xFF, 0x4A,
                                /* 0008 */   0xAB, 0x38, 0x9B, 0x1B, 0xF3, 0x3A, 0x30, 0x15
                            }))
                    {
                        Return ("INT33BE")
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0xAA, 0xAA, 0x62, 0x3C, 0xE0, 0xD8, 0x1A, 0x40,
                                /* 0008 */   0x84, 0xC3, 0xFC, 0x05, 0x65, 0x6F, 0xA2, 0x8C
                            }))
                    {
                        Return ("OV5693")
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x8F, 0xCE, 0x2A, 0x82, 0x14, 0x28, 0x74, 0x41,
                                /* 0008 */   0xA5, 0x6B, 0x5F, 0x02, 0x9F, 0xE0, 0x79, 0xEE
                            }))
                    {
                        Return ("13P2BA540")           // IHV Part Numbe
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x2A, 0x51, 0x59, 0x29, 0x8C, 0x02, 0x46, 0x46,
                                /* 0008 */   0xB7, 0x3D, 0x4D, 0x1B, 0x56, 0x72, 0xFA, 0xD8
                            }))
                    {
                        Return ("BSW_ITL_RVP")
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0xD8, 0x7B, 0x3B, 0xEA, 0x9B, 0xE0, 0x39, 0x42,
                                /* 0008 */   0xAD, 0x6E, 0xED, 0x52, 0x5F, 0x3F, 0x26, 0xAB
                            }))
                    {
                        Return (0x1020)
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x92, 0xC4, 0x5A, 0xB6, 0x30, 0x9E, 0x60, 0x4D,
                                /* 0008 */   0xB5, 0xB2, 0xF4, 0x97, 0xC7, 0x90, 0xD9, 0xCF
                            }))
                    {
                        Return (Zero)
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x0F, 0xAB, 0x70, 0xE7, 0x44, 0x26, 0xAB, 0x4B,
                                /* 0008 */   0x86, 0x28, 0xD6, 0x2F, 0x16, 0x83, 0xFB, 0x9D
                            }))
                    {
                        Return (One)
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0xB2, 0x4A, 0xA5, 0x1E, 0x84, 0xCD, 0xCC, 0x48,
                                /* 0008 */   0x9D, 0xD4, 0x7F, 0x59, 0x4E, 0xC3, 0xB0, 0x15
                            }))
                    {
                        Return (0x2)
                    }

                    If (LEqual (Arg0, Buffer (0x10)    // mclk
                            {
                                /* 0000 */   0x39, 0xA6, 0xC9, 0x75, 0x8A, 0x5C, 0x00, 0x4A,
                                /* 0008 */   0x9F, 0x48, 0xA9, 0xC3, 0xB5, 0xDA, 0x78, 0x9F
                            }))
                    {
                        Return ("AD5823")
                    }

                    //Dsm2PlatformStr
                    If(LEqual(Arg0, ToUUID("2FA9BB94-9C5D-4AEB-8E6E-27434F81E3D3")))
                    {
                        Return ("BSW")
                    }
                    
                    //Dsm2PlatformSubStr
                    If(LEqual(Arg0, ToUUID("647A6CA2-8B29-49AC-8806-D58B3D2D3EF5")))
                    {
                        Return ("FFD")
                    }
                    
                    //Dsm2SiliconStr
                    If(LEqual(Arg0, ToUUID("A6E922A1-F7B3-4399-B56A-406AE416843B")))
                    {
                        Return ("BSW")
                    }
                    
                    //Dsm2PlatformStr
                    If(LEqual(Arg0, ToUUID("5960313B-0AB0-4940-8840-2CAFA420C015")))
                    {
                        Return ("INTEL")
                    }
                    
                    //Dsm2Info
                    If(LEqual(Arg0, ToUUID("F486D39F-D657-484B-84A6-42A565712B92")))
                    {
                        Return (Buffer (32)
                        {
                            0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02,
                            0x07, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        })
                    }

                    If (LEqual (Arg0, Buffer (0x10)    // I2C
                            {
                                /* 0000 */   0x49, 0x75, 0x25, 0x26, 0x71, 0x92, 0xA4, 0x4C,
                                /* 0008 */   0xBB, 0x43, 0xC4, 0x89, 0x9D, 0x5A, 0x48, 0x81
                            }))
                    {
                        If (LEqual (Arg2, One))
                        {
                            Return (0x03)
                        }

                        If (LEqual (Arg2, 0x02))
                        {
                            Return (0x04001000)
                        }

                        If (LEqual (Arg2, 0x03))
                        {
                            Return (0x04000C01)
                        }

                        If (LEqual (Arg2, 0x04))
                        {
                            Return (0x04005402)
                        }
                    }

                    If (LEqual (Arg0, Buffer (0x10)    // GPIO
                            {
                                /* 0000 */   0x40, 0x46, 0x23, 0x79, 0x10, 0x9E, 0xEA, 0x4F,
                                /* 0008 */   0xA5, 0xC1, 0xB5, 0xAA, 0x8B, 0x19, 0x75, 0x6F
                            }))
                    {
                        If (LEqual (Arg2, One))
                        {
                            Return (0x5)               // total number
                        }

                        If (LEqual (Arg2, 0x02))
                        {
                            Return (0x01003200)        // GPIO N"50"  HIGH POWER EFFECITVE
                        }
                        If (LEqual (Arg2, 0x03))
                        {
                            Return (0x01003301)        // GPIO N"51"  HIGH POWER 12 EFFECITVE
                        }
                        If (LEqual (Arg2, 0x04))
                        {
                            Return (0x01003807)        // GPIO N"56"  HIGH POWER 18 EFFECITVE
                        }
                        If (LEqual (Arg2, 0x05))
                        {
                            Return (0x01002D08)        // GPIO N"45"  HIGH POWER 28 EFFECITVE
                        }
                        If (LEqual (Arg2, 0x06))
                        {
                            Return (0x01003109)        // GPIO N"49"  HIGH POWER 28 EFFECITVE
                        }
                    }
                    Return (Zero)
                }
            }

            //
            // Device STRB
            //
            Device (STRB)
            {
                Name (_ADR, Zero)                      // _ADR: Address
                Name (_HID, "INTCF1C")                 // _HID: Hardware ID
                Name (_CID, "INTCF1C")                 // _CID: Compatible ID
                Name (_SUB, "INTL0000")                // _SUB: Subsystem ID
                Name (_DDN, "Flash LM3554")            // _DDN: DOS Device Name
                Name (_UID, One)                       // _UID: Unique ID
                Method (_STA, 0, NotSerialized)        // _STA: Status
                {
                  If (LEqual (RCAM, 1)) {              // Flash for CRD_B v2.1 OV5693 + OV2724
                    Return (0x0F)
                  }
                  Return (0)
                }

                Method (_CRS, 0, Serialized)           // _CRS: Current Resource Settings
                {
                    Name (SBUF, ResourceTemplate ()
                    {
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x0036
                            }
                        GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,
                            )
                            {   // Pin list
                                0x002F
                            }

			GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                            "\\_SB.GPO1", 0x00, ResourceConsumer, ,    
                            )
                            {   // Pin list
                                0x0037   //for camera led camerasb11
                            }

                        I2cSerialBus (0x0053, ControllerInitiated, 0x00061A80,
                            AddressingMode7Bit, "\\_SB.PCI0.I2C4",
                            0x00, ResourceConsumer, ,
                            )
                    })
                    Return (SBUF)
                }

                Method (_DSM, 4, NotSerialized)  // _DSM: Device-Specific Method
                {
                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x6A, 0xA7, 0x7B, 0x37, 0x90, 0xF3, 0xFF, 0x4A,
                                /* 0008 */   0xAB, 0x38, 0x9B, 0x1B, 0xF3, 0x3A, 0x30, 0x15
                            }))
                    {
                        Return ("INTCF1C")
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x8F, 0xCE, 0x2A, 0x82, 0x14, 0x28, 0x74, 0x41,
                                /* 0008 */   0xA5, 0x6B, 0x5F, 0x02, 0x9F, 0xE0, 0x79, 0xEE
                            }))
                    {
                        Return ("LM3554")
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x2A, 0x51, 0x59, 0x29, 0x8C, 0x02, 0x46, 0x46,
                                /* 0008 */   0xB7, 0x3D, 0x4D, 0x1B, 0x56, 0x72, 0xFA, 0xD8
                            }))
                    {
                        Return ("BSW_ITL_RVP")
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x39, 0xA6, 0xC9, 0x75, 0x8A, 0x5C, 0x00, 0x4A,
                                /* 0008 */   0x9F, 0x48, 0xA9, 0xC3, 0xB5, 0xDA, 0x78, 0x9F
                            }))
                    {
                        Return (Zero)
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x49, 0x75, 0x25, 0x26, 0x71, 0x92, 0xA4, 0x4C,
                                /* 0008 */   0xBB, 0x43, 0xC4, 0x89, 0x9D, 0x5A, 0x48, 0x81
                            }))
                    {
                        If (LEqual (Arg2, One))
                        {
                            Return (One)
                        }

                        If (LEqual (Arg2, 0x02))
                        {
                            Return (0x04005300)
                        }
                    }

                    If (LEqual (Arg0, Buffer (0x10)
                            {
                                /* 0000 */   0x40, 0x46, 0x23, 0x79, 0x10, 0x9E, 0xEA, 0x4F,
                                /* 0008 */   0xA5, 0xC1, 0xB5, 0xAA, 0x8B, 0x19, 0x75, 0x6F
                            }))
                    {
                        If (LEqual (Arg2, One))
                        {
                            Return (0x03)
                        }

                        If (LEqual (Arg2, 0x02))
                        {
                            Return (0x01003601)
                        }

                        If (LEqual (Arg2, 0x03))
                        {
                            Return (0x01002F02)
                        }
                        If (LEqual (Arg2, 0x04))
                        {
                            Return (0x01003705)
                        }
                    }

                    Return (Zero)
                }
            } // End STRB

