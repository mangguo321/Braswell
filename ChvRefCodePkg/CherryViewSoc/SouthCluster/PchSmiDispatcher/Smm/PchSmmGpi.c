/** @file
  File to contain all the hardware specific stuff for the Smm Gpi dispatch protocol.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSmmHelpers.h"

#define GPI_INIT_ELEMENT(num) { \
    PCH_SMM_NO_FLAGS, \
    { \
      { \
        { \
          ACPI_ADDR_TYPE, R_PCH_ALT_GP_SMI_EN \
        }, \
        S_PCH_ALT_GP_SMI_EN, num, \
      }, \
      NULL_BIT_DESC_INITIALIZER \
    }, \
    { \
      { \
        { \
          ACPI_ADDR_TYPE, R_PCH_ALT_GP_SMI_STS \
        }, \
        S_PCH_ALT_GP_SMI_STS, (num), \
      }, \
    } \
  }

CONST PCH_SMM_SOURCE_DESC GPI_SOURCE_DESC[NUM_SUPPORTED_GPIS] = {
  GPI_INIT_ELEMENT(0),
  GPI_INIT_ELEMENT(1),
  GPI_INIT_ELEMENT(2),
  GPI_INIT_ELEMENT(3),
  GPI_INIT_ELEMENT(4),
  GPI_INIT_ELEMENT(5),
  GPI_INIT_ELEMENT(6),
  GPI_INIT_ELEMENT(7),
  GPI_INIT_ELEMENT(8),
  GPI_INIT_ELEMENT(9),
  GPI_INIT_ELEMENT(10),
  GPI_INIT_ELEMENT(11),
  GPI_INIT_ELEMENT(12),
  GPI_INIT_ELEMENT(13),
  GPI_INIT_ELEMENT(14),
  GPI_INIT_ELEMENT(15),
};
