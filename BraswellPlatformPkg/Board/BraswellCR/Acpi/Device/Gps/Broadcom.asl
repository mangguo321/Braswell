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

     Device(GPS0)  // GPS for Android OS
     {
       Name(_HID, "BCM4752")
       Name(_HRV, 0)
       Name (UART, One)
       Name (ENAB, Zero)
       Name (HSTW, One)

       Method(_STA, 0x0, NotSerialized)
       {
         If (\_OSI("Android"))
         {
           Return (0x0F)
         }
         Return(0)
       }

       Method(_CRS, 0x0, NotSerialized)
       {
         // UARTSerial Bus Connection Descriptor

         Name(RBUF, ResourceTemplate () {   // Resource for BSW CR board
           UARTSerialBus(
           115200,   // InitialBaudRate: in bits ber second
           ,             // BitsPerByte: default to 8 bits
           ,             // StopBits: Defaults to one bit
           0xfc,                   // LinesInUse: 8 1-bit flags to declare line enabled
           ,             // IsBigEndian: default to LittleEndian
           ,             // Parity: Defaults to no parity
           FlowControlHardware,             // FlowControl: Defaults to no flow control
           32,             // ReceiveBufferSize
           32,             // TransmitBufferSize
           "\\_SB.PCI0.URT2",       // ResourceSource: UART bus controller name
           ,)            // DescriptorName: creates name for offset of resource descriptor

           GpioInt(Level, ActiveHigh, Exclusive, PullNone, 0, "\\_SB.GPO3", ) {7}              // GPS_HOSTREQ SE07
           GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {3}   // GPS_R_WAKEUP N03
           //GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {7} // COMBO_GPS_RESET_N N07
         })
         Return (RBUF)
       }
     } // Device GPS0
     Device(GPS1)//GPS for Windows OS.
     {
       Name(_HID, "BCM4752")      // Vendor: test GPS device for CHT
       Name(_HRV, 1)

       Method(_STA, 0x0, NotSerialized)
       {
         If (\_OSI("Android"))
         {
           Return (0)
         }
         Return(0xF)
       }

       Method(_CRS, 0x0, NotSerialized)
       {
         // UARTSerial Bus Connection Descriptor

         Name(CBUF, ResourceTemplate () {   // Resource for BSW CR board
           UARTSerialBus(
           115200,   // InitialBaudRate: in bits ber second
           ,             // BitsPerByte: default to 8 bits
           ,             // StopBits: Defaults to one bit
           0xfc,                   // LinesInUse: 8 1-bit flags to declare line enabled
           ,             // IsBigEndian: default to LittleEndian
           ,             // Parity: Defaults to no parity
           FlowControlHardware,             // FlowControl: Defaults to no flow control
           32,             // ReceiveBufferSize
           32,             // TransmitBufferSize
           "\\_SB.PCI0.URT2",       // ResourceSource: UART bus controller name
           ,)            // DescriptorName: creates name for offset of resource descriptor
         //GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {7}   // COMBO_GPS_RESET_N N07
         //Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {62}               // GPS_HOSTREQ SE07 Line1 maps to direct IRQ 62
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO1", ) {3}     // GPS_R_WAKEUP N03
         })
         Return (CBUF)
       }
     } // Device GPS1


