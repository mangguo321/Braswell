/** @file
  File to contain all the hardware specific stuff for the Smm Ichn dispatch protocol.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSmmHelpers.h"
#include "PlatformBaseAddresses.h"
#include <Library/PchPlatformLib.h>

PCH_SMM_SOURCE_DESC ICHN_SOURCE_DESCS[NUM_ICHN_TYPES] = {
  ///
  /// IchnMch
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnPme
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnRtcAlarm
  ///
  {
    PCH_SMM_SCI_EN_DEPENDENT,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_PM1_EN
        },
        S_PCH_ACPI_PM1_EN,
        N_PCH_ACPI_PM1_EN_RTC
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
        N_PCH_ACPI_PM1_STS_RTC
      }
    }
  },
  ///
  /// IchnRingIndicate
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnAc97Wake
  /// Not supported,
  /// we just fill in invalid initializer and not use it.
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnSerialIrq
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_STS
        },
        S_PCH_SMI_STS,
        N_PCH_SMI_STS_ILB
      }
    }
  },
  ///
  /// IchnY2KRollover
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnTcoTimeout
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_EN
        },
        S_PCH_ACPI_GPE0a_EN,
        N_PCH_ACPI_GPE0a_EN_TCO
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_TCO_STS
        },
        S_PCH_TCO_STS,
        N_PCH_TCO_STS_TIMEOUT
      }
    }
  },
  ///
  /// IchnOsTco
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnNmi
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_EN
        },
        S_PCH_ACPI_GPE0a_EN,
        N_PCH_ACPI_GPE0a_EN_TCO
      },
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (ILB_BASE_ADDRESS + R_PCH_ILB_GNMI)
        },
        S_PCH_ILB_GNMI,
        N_PCH_ILB_GNMI_NMI2SMIEN
      }
    },
    {
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (ILB_BASE_ADDRESS + R_PCH_ILB_GNMI)
        },
        S_PCH_ILB_GNMI,
        N_PCH_ILB_GNMI_NMI2SMIST
      }
    }
  },
  ///
  /// IchnIntruderDetect
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnBiosWp
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_EN
        },
        S_PCH_ACPI_GPE0a_EN,
        N_PCH_ACPI_GPE0a_EN_TCO
      },
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (SPI_BASE_ADDRESS + R_PCH_SPI_SCS)
        },
        S_PCH_SPI_SCS,
        N_PCH_SPI_SCS_SMIWPEN
      }
    },
    {
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (SPI_BASE_ADDRESS + R_PCH_SPI_SCS)
        },
        S_PCH_SPI_SCS,
        N_PCH_SPI_SCS_SMIWPST
      }
    }
  },
  ///
  /// IchnMcSmi
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnPmeB0
  ///
  {
    PCH_SMM_SCI_EN_DEPENDENT,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_EN
        },
        S_PCH_ACPI_GPE0a_EN,
        N_PCH_ACPI_GPE0a_EN_PME_B0
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_STS
        },
        S_PCH_ACPI_GPE0a_STS,
        N_PCH_ACPI_GPE0a_STS_PME_B0
      }
    }
  },
  ///
  /// IchnThrmSts
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnSmBus
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_STS
        },
        S_PCH_SMI_STS,
        N_PCH_SMI_STS_SMBUS
      }
    }
  },
  ///
  /// IchnIntelUsb2
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_EN
        },
        S_PCH_SMI_EN,
        N_PCH_SMI_EN_INTEL_USB2
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_STS
        },
        S_PCH_SMI_STS,
        N_PCH_SMI_STS_INTEL_USB2
      }
    }
  },
  ///
  /// IchnMonSmi7
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnMonSmi6
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnMonSmi5
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnMonSmi4
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap13
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap12, KBC_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap11
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap10
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap9, PIRQDH_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap8, PIRQCG_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap7, PIRQBF_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap6, PIRQAE_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap5
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap3
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap2
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap1
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap0, IDE_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 3 monitor,
  /// The "PCH_RCRB_BASE_NEED_FIX" should be fixed since the RCRB base should get from the RCBA register filled by platform module.
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 2 monitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 1 monitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 0 monitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
};

PCH_SMM_SOURCE_DESC ICHN_SOURCE_DESCS_AX[NUM_ICHN_TYPES] = {
  ///
  /// IchnMch
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnPme
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnRtcAlarm
  ///
  {
    PCH_SMM_SCI_EN_DEPENDENT,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_PM1_EN
        },
        S_PCH_ACPI_PM1_EN,
        N_PCH_ACPI_PM1_EN_RTC
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
        N_PCH_ACPI_PM1_STS_RTC
      }
    }
  },
  ///
  /// IchnRingIndicate
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnAc97Wake
  /// Not supported,
  /// we just fill in invalid initializer and not use it.
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnSerialIrq
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_STS
        },
        S_PCH_SMI_STS,
        N_PCH_SMI_STS_ILB
      }
    }
  },
  ///
  /// IchnY2KRollover
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnTcoTimeout
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_EN
        },
        S_PCH_SMI_EN,
        N_PCH_SMI_EN_TCO
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_TCO_STS
        },
        S_PCH_TCO_STS,
        N_PCH_TCO_STS_TIMEOUT
      }
    }
  },
  ///
  /// IchnOsTco
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnNmi
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_EN
        },
        S_PCH_SMI_EN,
        N_PCH_SMI_EN_TCO
      },
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (ILB_BASE_ADDRESS + R_PCH_ILB_GNMI)
        },
        S_PCH_ILB_GNMI,
        N_PCH_ILB_GNMI_NMI2SMIEN
      }
    },
    {
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (ILB_BASE_ADDRESS + R_PCH_ILB_GNMI)
        },
        S_PCH_ILB_GNMI,
        N_PCH_ILB_GNMI_NMI2SMIST
      }
    }
  },
  ///
  /// IchnIntruderDetect
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnBiosWp
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_EN
        },
        S_PCH_SMI_EN,
        N_PCH_SMI_EN_TCO
      },
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (SPI_BASE_ADDRESS + R_PCH_SPI_SCS)
        },
        S_PCH_SPI_SCS,
        N_PCH_SPI_SCS_SMIWPEN
      }
    },
    {
      {
        {
          MEMORY_MAPPED_IO_ADDRESS_TYPE,
          (SPI_BASE_ADDRESS + R_PCH_SPI_SCS)
        },
        S_PCH_SPI_SCS,
        N_PCH_SPI_SCS_SMIWPST
      }
    }
  },
  ///
  /// IchnMcSmi
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnPmeB0
  ///
  {
    PCH_SMM_SCI_EN_DEPENDENT,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_EN
        },
        S_PCH_ACPI_GPE0a_EN,
        N_PCH_ACPI_GPE0a_EN_PME_B0
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_GPE0a_STS
        },
        S_PCH_ACPI_GPE0a_STS,
        N_PCH_ACPI_GPE0a_STS_PME_B0
      }
    }
  },
  ///
  /// IchnThrmSts
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnSmBus
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_STS
        },
        S_PCH_SMI_STS,
        N_PCH_SMI_STS_SMBUS
      }
    }
  },
  ///
  /// IchnIntelUsb2
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_EN
        },
        S_PCH_SMI_EN,
        N_PCH_SMI_EN_INTEL_USB2
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_SMI_STS
        },
        S_PCH_SMI_STS,
        N_PCH_SMI_STS_INTEL_USB2
      }
    }
  },
  ///
  /// IchnMonSmi7
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnMonSmi6
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnMonSmi5
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnMonSmi4
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap13
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap12, KBC_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap11
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap10
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap9, PIRQDH_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap8, PIRQCG_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap7, PIRQBF_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap6, PIRQAE_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap5
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap3
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap2
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap1
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnDevTrap0, IDE_ACT_STS
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 3 monitor,
  /// The "PCH_RCRB_BASE_NEED_FIX" should be fixed since the RCRB base should get from the RCBA register filled by platform module.
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 2 monitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 1 monitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// PCH I/O Trap register 0 monitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
};

PCH_SMM_SOURCE_DESC ICHN_EX_SOURCE_DESCS[IchnExTypeMAX - IchnExPciExpress] = {
  ///
  /// IchnExPciExpress
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnExMonitor
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnExSpi
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnExQRT
  ///
  NULL_SOURCE_DESC_INITIALIZER,
  ///
  /// IchnExGpioUnlock
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      NULL_BIT_DESC_INITIALIZER,
      NULL_BIT_DESC_INITIALIZER
    },
    {
      NULL_BIT_DESC_INITIALIZER
    }
  },
  ///
  /// IchnExTmrOverflow
  ///
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          ACPI_ADDR_TYPE,
          R_PCH_ACPI_PM1_EN
        },
        S_PCH_ACPI_PM1_EN,
        N_PCH_ACPI_PM1_EN_TMROF
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
        N_PCH_ACPI_PM1_STS_TMROF
      }
    }
  },

};

///
/// TCO_STS bit that needs to be cleared
///
PCH_SMM_SOURCE_DESC TCO_STS = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        R_PCH_ACPI_GPE0a_STS
      },
      S_PCH_ACPI_GPE0a_STS,
      N_PCH_ACPI_GPE0a_STS_TCO
    }
  }
};

PCH_SMM_SOURCE_DESC TCO_STS_AX = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        R_PCH_SMI_STS
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_TCO
    }
  }
};

/**
  Clear the SMI status bit after the SMI handling is done

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchSmmIchnClearSource (
  PCH_SMM_SOURCE_DESC   *SrcDesc
  )
{
  PchSmmClearSource (SrcDesc);
  ///
  /// Any TCO-based status bits require special handling.
  /// SMI_STS.TCO_STS must be cleared in addition to the status bit in the TCO registers
  ///
  if (SocStepping () < SocB0) {
    PchSmmClearSource (&TCO_STS_AX);
  } else {
    PchSmmClearSource (&TCO_STS);
  }
}
