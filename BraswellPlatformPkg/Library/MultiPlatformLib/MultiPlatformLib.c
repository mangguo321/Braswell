/** @file
  Multiplatform initialization.

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <MultiPlatformLib.h>


/**
  MultiPlatform initialization

  @param[in]  PeiServices      General purpose services available to every PEIM.
  @param[in]  PlatformInfoHob  The Hob of platform information 

  @retval  EFI_SUCCESS         Platform initialization completed successfully.
  @retval  Others              All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
MultiPlatformInfoInit (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN OUT EFI_PLATFORM_INFO_HOB          *PlatformInfoHob
  )
{
  UINT32          PcieLength;
  EFI_STATUS      Status;

  //
  // Calculate PCIe length based on user defined range
  //
  switch (PLATFORM_PCIE_BASE_SIZE) {
    case 64:
      //64MB
      PcieLength = 0x04000000;
      break;
    case 128:
      //128MB
      PcieLength = 0x08000000;
      break;
    default:
      // 256MB
      PcieLength = 0x10000000;
      break;
  }

  //
  // Don't support BASE above 4GB currently
  //
  PlatformInfoHob->PciData.PciExpressSize     = PcieLength;
  PlatformInfoHob->PciData.PciExpressBase     = PLATFORM_PCIE_BASE;

  PlatformInfoHob->PciData.PciResourceMem32Base  = (UINT32) (PlatformInfoHob->PciData.PciExpressBase - RES_MEM32_MIN_LEN);
  PlatformInfoHob->PciData.PciResourceMem32Limit = (UINT32) (PlatformInfoHob->PciData.PciExpressBase -1);

  PlatformInfoHob->PciData.PciResourceMem64Base   = RES_MEM64_36_BASE;
  PlatformInfoHob->PciData.PciResourceMem64Limit  = RES_MEM64_36_LIMIT;
  PlatformInfoHob->CpuData.CpuAddressWidth        = 36;

  PlatformInfoHob->MemData.MemMir0 = PlatformInfoHob->PciData.PciResourceMem64Base;
  PlatformInfoHob->MemData.MemMir1 = PlatformInfoHob->PciData.PciResourceMem64Limit + 1;

  PlatformInfoHob->PciData.PciResourceMinSecBus  = 1;  //can be changed by SystemConfiguration->PciMinSecondaryBus;

  //
  // Set MemMaxTolm to the lowest address between PCIe Base and PCI32 Base
  //
  if (PlatformInfoHob->PciData.PciExpressBase > PlatformInfoHob->PciData.PciResourceMem32Base ) {
    PlatformInfoHob->MemData.MemMaxTolm = (UINT32) PlatformInfoHob->PciData.PciResourceMem32Base;
  } else {
    PlatformInfoHob->MemData.MemMaxTolm = (UINT32) PlatformInfoHob->PciData.PciExpressBase;
  }
  PlatformInfoHob->MemData.MemTolm = PlatformInfoHob->MemData.MemMaxTolm;

  //
  // Platform PCI MMIO Size in unit of 1MB
  //
  PlatformInfoHob->MemData.MmioSize = 0x1000 - (UINT16)(PlatformInfoHob->MemData.MemMaxTolm >> 20);

  //
  // Update Memory Config HOB size which will be used by SaveMemoryConfig
  //
  // bugbug PlatformInfoHob->MemData.MemConfigSize = sizeof(MRC_PARAMS_SAVE_RESTORE);

  //
  // Enable ICH IOAPIC
  //
  PlatformInfoHob->SysData.SysIoApicEnable  = ICH_IOAPIC;

  DEBUG ((EFI_D_ERROR, "PlatformFlavor: %x (%x=tablet,%x=mobile,%x=desktop)\n", PlatformInfoHob->PlatformFlavor,FlavorTablet,FlavorMobile,FlavorDesktop));

  //
  // Get Platform Info and fill the Hob
  //
  PlatformInfoHob->RevisonId = PLATFORM_INFO_HOB_REVISION;

  //
  // Get GPIO table
  //
  //
  // Program GPIO
  //
  Status = MultiPlatformGpioProgram (PeiServices, PlatformInfoHob);

  return EFI_SUCCESS;
}
