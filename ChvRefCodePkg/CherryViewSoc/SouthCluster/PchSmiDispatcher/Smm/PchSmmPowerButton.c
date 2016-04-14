/** @file
  File to contain all the hardware specific stuff for the Smm Power Button dispatch protocol.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PchSmmHelpers.h>

CONST PCH_SMM_SOURCE_DESC POWER_BUTTON_SOURCE_DESC = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        R_PCH_ACPI_PM1_EN
      },
      S_PCH_ACPI_PM1_EN,
      N_PCH_ACPI_PM1_EN_PWRBTN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        R_PCH_ACPI_PM1_STS
      },
      S_PCH_ACPI_PM1_STS,
      N_PCH_ACPI_PM1_STS_PWRBTN
    }
  }
};

/**
  Get the power button status.

  @param[in] Record               The pointer to the DATABASE_RECORD.
  @param[out] Context             Calling context from the hardware, will be updated with the current power button status.

**/
VOID
EFIAPI
PowerButtonGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  )
{
  UINT16  GenPmCon2;
  UINT32  PmcBase;

  PmcBase = MmioRead32 (
              MmPciAddress (0,
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_LPC,
                PCI_FUNCTION_NUMBER_PCH_LPC,
                R_PCH_LPC_PMC_BASE
              )
            ) & B_PCH_LPC_PMC_BASE_BAR;

  GenPmCon2 = MmioRead16 (PmcBase + R_PCH_PMC_GEN_PMCON_2);

  if ((GenPmCon2 & B_PCH_PMC_GEN_PMCON_PWRBTN_LVL) == 0) {
    Context->PowerButton.Phase = EfiPowerButtonExit;
  } else {
    Context->PowerButton.Phase = EfiPowerButtonEntry; 
  }
}

/**
  Check whether Power Button status of two contexts match

  @param[in] Context1             Context 1 that includes Power Button status 1
  @param[in] Context2             Context 2 that includes Power Button status 2

  @retval FALSE                   Power Button status match
  @retval TRUE                    Power Button status don't match
**/
BOOLEAN
EFIAPI
PowerButtonCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  )
{
  return (BOOLEAN) (Context1->PowerButton.Phase == Context2->PowerButton.Phase);
}
