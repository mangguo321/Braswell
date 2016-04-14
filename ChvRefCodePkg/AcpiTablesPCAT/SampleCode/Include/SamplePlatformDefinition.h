/** @file
  Header file for ACPI MADT and MCFG table

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// I/O APIC IDs, the code uses math to generate the numbers
// instead of using these defines.
//
#define ICH_IOAPIC                     (1 << 0)
#define ICH_IOAPIC_ID                   0x08

//
// CPU Equates
//
#define MAX_THREAD                      2
#define MAX_CORE                        1
#define MAX_DIE                         2
#define MAX_CPU_SOCKET                  1
#define MAX_CPU_NUM                     (MAX_THREAD * MAX_CORE * MAX_DIE * MAX_CPU_SOCKET)

//
// Platform Max Bus Number
//
#define PLATFORM_MAX_BUS_NUM 0x3f
