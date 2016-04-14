/** @file
  PCH SPI SMM Driver implements the SPI Host Controller Compatibility Interface.

  Copyright (c) 2008 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSpi.h"

//
// Global variables
//
SPI_INSTANCE          *mSpiInstance;

/**
  Entry point for the SPI host controller driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
InstallPchSpi (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS  Status;

  ///
  /// Allocate pool for SPI protocol instance
  ///
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData, /// MemoryType don't care
                    sizeof (SPI_INSTANCE),
                    (VOID **) &mSpiInstance
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (mSpiInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem ((VOID *) mSpiInstance, sizeof (SPI_INSTANCE));
  ///
  /// Initialize the SPI protocol instance
  ///
  Status = SpiProtocolConstructor (mSpiInstance);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// Install the SMM EFI_SPI_PROTOCOL interface
  ///
  Status = gSmst->SmmInstallProtocolInterface (
                    &(mSpiInstance->Handle),
                    &gEfiSmmSpi2ProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &(mSpiInstance->SpiProtocol)
                    );
  if (EFI_ERROR (Status)) {
    gSmst->SmmFreePool (mSpiInstance);
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  This function is a a hook for Spi Smm phase specific initialization

**/
VOID
EFIAPI
SpiPhaseInit (
  VOID
  )
{
  UINTN       Index;
  static CONST UINT32 SpiRegister[] = {
    R_PCH_SPI_SSFCS,
    R_PCH_SPI_PREOP,
    R_PCH_SPI_OPMENU0,
    R_PCH_SPI_OPMENU1,
    R_PCH_SPI_VSCC0,
    R_PCH_SPI_VSCC1
  };

  ///
  /// Save SPI Registers for S3 resume usage
  ///
  for (Index = 0; Index < sizeof (SpiRegister) / sizeof (UINT32); Index++) {
    S3BootScriptSaveMemWrite (
      EfiBootScriptWidthUint32,
      (UINTN) (mSpiInstance->SpiBase + SpiRegister[Index]),
      1,
      (VOID *) (UINTN) (mSpiInstance->SpiBase + SpiRegister[Index])
      );
  }
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
  /// Set BIOSWE bit (SpiBase + 0xFC [0]) = 1b
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
  /// Clear BIOSWE bit (SpiBase + 0xFC [0]) = 0b
  ///
  MmioAnd8 ((UINTN) (SpiBase + R_PCH_SPI_BCR), (UINT8) ~B_PCH_SPI_BCR_BIOSWE);
}
