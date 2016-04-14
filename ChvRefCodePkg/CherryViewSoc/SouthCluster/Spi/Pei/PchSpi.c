/** @file
  PCH SPI PEIM implements the SPI Host Controller Compatibility Interface.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSpi.h"

/**
  Installs PCH SPI PPI.

  @param[in] FfsHeader            Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             PCH SPI PPI is installed successfully.
  @retval EFI_OUT_OF_RESOURCES    Can't allocate pool.

**/
EFI_STATUS
EFIAPI
InstallPchSpi (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS        Status;
  PEI_SPI_INSTANCE  *PeiSpiInstance;
  SPI_INSTANCE      *SpiInstance;

  DEBUG ((EFI_D_INFO, "InstallPchSpi() Start\n"));

  PeiSpiInstance = (PEI_SPI_INSTANCE *) AllocateZeroPool (sizeof (PEI_SPI_INSTANCE));
  if (NULL == PeiSpiInstance) {
    return EFI_OUT_OF_RESOURCES;
  }

  SpiInstance = &(PeiSpiInstance->SpiInstance);
  SpiProtocolConstructor (SpiInstance);

  PeiSpiInstance->PpiDescriptor.Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PeiSpiInstance->PpiDescriptor.Guid  = &gPeiSpiPpiGuid;
  PeiSpiInstance->PpiDescriptor.Ppi   = &(SpiInstance->SpiProtocol);

  ///
  /// Install the SPI PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, &PeiSpiInstance->PpiDescriptor);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "SPI PPI Installed\n"));

  DEBUG ((EFI_D_INFO, "InstallPchSpi() End\n"));

  return Status;
}

/**
  This function is a a hook for Spi Pei phase specific initialization

**/
VOID
EFIAPI
SpiPhaseInit (
  VOID
  )
{
  UINT32 PciD31F0RegBase;
  UINT32 SpiBase;

  PciD31F0RegBase = MmPciAddress (0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC,
                      0
                    );
  SpiBase = MmioRead32 (PciD31F0RegBase + R_PCH_LPC_SPI_BASE) & B_PCH_LPC_SPI_BASE_BAR;
  //
  // Disable SMM BIOS write protect if it's not a SMM protocol
  //
  MmioAnd8 (SpiBase + R_PCH_SPI_BCR,
    (UINT8) (~B_PCH_SPI_BCR_SMM_BWP)
    );
}

/**
  This function is a hook for Spi to disable BIOS Write Protect

**/
VOID
EFIAPI
DisableBiosWriteProtect (
  IN UINTN        SpiBase
  )
{
  ///
  /// Enable the access to the BIOS space for both read and write cycles
  ///
  MmioOr8 ((UINTN) (SpiBase + R_PCH_SPI_BCR), (UINT8) B_PCH_SPI_BCR_BIOSWE);
}

/**
  This function is a hook for Spi to enable BIOS Write Protect

**/
VOID
EFIAPI
EnableBiosWriteProtect (
  IN UINTN        SpiBase
  )
{
  ///
  /// Disable the access to the BIOS space for write cycles
  ///
  MmioAnd8 ((UINTN) (SpiBase + R_PCH_SPI_BCR), (UINT8) ~B_PCH_SPI_BCR_BIOSWE);
}
