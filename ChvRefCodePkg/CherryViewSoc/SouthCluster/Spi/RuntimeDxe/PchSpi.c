/** @file
  PCH SPI Runtime Driver implements the SPI Host Controller Compatibility Interface.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSpi.h"

///
/// Global variables
///
SPI_INSTANCE  *mSpiInstance;
EFI_EVENT     mVirtualAddressChangeEvent = NULL;

static CONST UINT32 mSpiRegister[] = {
  R_PCH_SPI_SSFCS,
  R_PCH_SPI_PREOP,
  R_PCH_SPI_OPMENU0,
  R_PCH_SPI_OPMENU1,
  R_PCH_SPI_VSCC0,
  R_PCH_SPI_VSCC1
  };

//
// Function implementations
//

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.

**/
VOID
EFIAPI
PchSpiVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSpiInstance->SpiBase));
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSpiInstance->SpiProtocol.Init));
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSpiInstance->SpiProtocol.Lock));
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSpiInstance->SpiProtocol.Execute));
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSpiInstance));
}

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
  EFI_STATUS                      Status;
  UINT64                          BaseAddress;
  UINT64                          Length;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR GcdMemorySpaceDescriptor;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR LpcMemorySpaceDescriptor;
  UINT64                          Attributes;

  DEBUG ((EFI_D_INFO, "InstallPchSpi() Start\n"));

  Status = PciLibConstructor ();
  ASSERT_EFI_ERROR (Status);

  ///
  /// Allocate Runtime memory for the SPI protocol instance.
  ///
  mSpiInstance = AllocateRuntimeZeroPool (sizeof (SPI_INSTANCE));
  if (mSpiInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Initialize the SPI protocol instance
  ///
  Status = SpiProtocolConstructor (mSpiInstance);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// Install the EFI_SPI_PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(mSpiInstance->Handle),
                  &gEfiSpiProtocolGuid,
                  &(mSpiInstance->SpiProtocol),
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    FreePool (mSpiInstance);
    return EFI_DEVICE_ERROR;
  }
  ///
  /// Set SPI memory space in GCD to be RUNTIME so that the range will be supported in
  /// virtual address mode in EFI aware OS runtime.
  /// It will assert if SPI memory space is not allocated
  /// The caller is responsible for the existence and allocation of the SPI memory spaces
  ///
  BaseAddress = (EFI_PHYSICAL_ADDRESS) (mSpiInstance->SpiBase);
  Length      = 0x1000;

  Status      = gDS->GetMemorySpaceDescriptor (BaseAddress, &GcdMemorySpaceDescriptor);
  ASSERT_EFI_ERROR (Status);

  Attributes = GcdMemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME;

  Status = gDS->SetMemorySpaceAttributes (
                  BaseAddress,
                  Length,
                  Attributes
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  ///  LPC memory space
  ///
  BaseAddress = MmPciAddress(0,
                             DEFAULT_PCI_BUS_NUMBER_PCH,
                             PCI_DEVICE_NUMBER_PCH_LPC,
                             PCI_FUNCTION_NUMBER_PCH_LPC,
                             0
                             );
  Length  = 4096;

  Status  = gDS->GetMemorySpaceDescriptor (BaseAddress, &LpcMemorySpaceDescriptor);
  ASSERT_EFI_ERROR (Status);

  Attributes = LpcMemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME;

  Status = gDS->SetMemorySpaceAttributes (
                  BaseAddress,
                  Length,
                  Attributes
                  );
  ASSERT_EFI_ERROR (Status);

  Status = PciLibRegisterMemory (
             PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
             PCI_DEVICE_NUMBER_PCH_LPC,
             PCI_FUNCTION_NUMBER_PCH_LPC,
             0),
             (UINTN) Length
             );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PchSpiVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mVirtualAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "InstallPchSpi() End\n"));

  return EFI_SUCCESS;
}

/**
  This function is a hook for Spi Dxe phase specific initialization
**/
VOID
EFIAPI
SpiPhaseInit (
  VOID
  )
{
  UINTN                         Index;

  //
  // Disable SMM BIOS write protect if it's not a SMM protocol
  //
  MmioAnd8 (
    mSpiInstance->SpiBase + R_PCH_SPI_BCR,
    (UINT8) (~B_PCH_SPI_BCR_SMM_BWP)
    );

  //
  // Save SPI Registers for S3 resume usage
  //
  for (Index = 0; Index < sizeof (mSpiRegister) / sizeof (UINT32); Index++) {
    S3BootScriptSaveMemWrite (
      EfiBootScriptWidthUint32,
      (UINTN) (mSpiInstance->SpiBase + mSpiRegister[Index]),
      1,
      (VOID *) (UINTN) (mSpiInstance->SpiBase + mSpiRegister[Index])
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
