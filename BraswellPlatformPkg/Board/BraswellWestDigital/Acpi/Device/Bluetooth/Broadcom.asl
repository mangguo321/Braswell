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

    //
    // Bluetooth controller using serial interface
    //
     Device(BTH0) // Bluetooth for Android
    {
      Name (_HID, "BCM2E1A")       // Vendor BRCM request to change to 2E1A for CHT, #4753211
       Method (_STA, 0x0, NotSerialized)
       {
         Return(0)
       } // _STA

       Method(_CRS, 0x0, NotSerialized)
       {
         Name(UBUF, ResourceTemplate () {
           // UARTSerial Bus Connection Descriptor
           UARTSerialBus(115200,   // InitialBaudRate: in bits ber second
           ,             // BitsPerByte: default to 8 bits
           ,             // StopBits: Defaults to one bit
           0xfc,                   // LinesInUse: 8 1-bit flags to declare line enabled
           ,             // IsBigEndian: default to LittleEndian
           ,             // Parity: Defaults to no parity
           ,             // FlowControl: Defaults to no flow control
           32,             // ReceiveBufferSize
           32,             // TransmitBufferSize
           "\\_SB.PCIO.URT1",       // ResourceSource: UART bus controller name
           ,)            // DescriptorName: creates name for offset of resource descriptor
         // Interrupt (ResourceConsumer, Edge, ActiveHigh, ExclusiveAndWake, , , ) {94}      // SE60 INT Line#2, IOAPIC 94
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO3", ) {62}  // GPIO SE62, COMBO_BT_WAKEUP
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO3", ) {64}  // GPIO SE64, COMBO_BT_RESET_N
         GpioInt(Level, ActiveHigh, Exclusive, PullNone, 0, "\\_SB.GPO3", ) {60}             // GPIO SE60, COMBO_UART_WAKE_R
         })
         Return(UBUF)
       }
     } // Device BTH0

     Device (BTH1) // Bluetooth for Windows OS
     {
       Name (_HID, "BCM2E64")

       Method (_STA, 0x0, NotSerialized)
       {
         Return (0)
       } // _STA

       Method(_CRS, 0x0, NotSerialized)
       {
         Name(BBUF, ResourceTemplate () { //For Cherrytrail CRB
           // UARTSerial Bus Connection Descriptor
           UARTSerialBus(115200,   // InitialBaudRate: in bits ber second
           ,             // BitsPerByte: default to 8 bits
           ,             // StopBits: Defaults to one bit
           0xfc,                   // LinesInUse: 8 1-bit flags to declare line enabled
           ,             // IsBigEndian: default to LittleEndian
           ,             // Parity: Defaults to no parity
           ,             // FlowControl: Defaults to no flow control
           32,             // ReceiveBufferSize
           32,             // TransmitBufferSize
           "\\_SB.PCI0.URT1",       // ResourceSource: UART bus controller name
           ,)            // DescriptorName: creates name for offset of resource descriptor
         //Interrupt (ResourceConsumer, Edge, ActiveHigh, ExclusiveAndWake, , , ) {94}       // SE60 INT Line#2, IOAPIC 94
         GpioInt(Level, ActiveLow, Exclusive, PullNone, 0, "\\_SB.GPO3", ) {60}              // GPIO SE60, COMBO_UART_WAKE_R
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO3", ) {62}  // GPIO SE62, COMBO_BT_WAKEUP
         GpioIo(Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.GPO3", ) {64}  // GPIO SE64, COMBO_BT_RESET_N
        })
        Return(BBUF)
      }
    } // Device BTH1

