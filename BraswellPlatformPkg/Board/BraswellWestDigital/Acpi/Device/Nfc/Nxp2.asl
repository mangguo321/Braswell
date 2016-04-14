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

  Device (NFC2)
  {
    Name (_ADR, Zero)
    Name (_HID, "NXP1002")
    Name (_CID, "NXP1002")
    Name (_DDN, "NXP NFC")
    Name (_UID, One)
    Method (_STA, 0, NotSerialized)
    {
      If (LEqual(NFCS, 2))
      {
        Return (0xF)
      }
      Return (0x0)
    }
  }


