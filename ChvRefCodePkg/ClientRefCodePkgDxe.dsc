## @file
#  Component description file for the Braswell RC DXE drivers.
#
#  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php.
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
##

#
# CPU
#
#  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/CpuInit/Dxe/MpCpu.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/PowerManagement/Smm/PowerManagement.inf
#  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/Dts/Smm/DigitalThermalSensor.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/AcpiTables/CpuAcpiTables.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/SampleCode/SmramSaveInfoHandlerSmm/SmramSaveInfoHandlerSmm.inf

#
# SouthCluster
#
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Spi/Smm/PchSpiSmm.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/SmmControl/RuntimeDxe/SmmControl.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Reset/RuntimeDxe/PchReset.inf

  $(PCH_INIT_ROOT)/Dxe/PchInitDxe.inf {
    <BuildOptions>
    !if $(TARGET) == DEBUG
      *_*_*_CC_FLAGS = -DEFI_DEBUG
    !endif
	!if $(FSP_BOOT_ENABLE) == TRUE
    *_*_IA32_CC_FLAGS      = -U FSP_FLAG 
    *_*_X64_CC_FLAGS       = -U FSP_FLAG  
    !endif
  }


  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/PchSmiDispatcher/Smm/PchSmiDispatcher.inf

  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Spi/RuntimeDxe/PchSpiRuntime.inf {
    <LibraryClasses>
      PciLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/DxeRuntimePciLibPciExpress/DxeRuntimePciLibPciExpress.inf
  }

  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/SDControllerDxe/MmcHostDxe.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/SDMediaDeviceDxe/MmcMediaDeviceDxe.inf
!if $(SATA_ENABLE) == TRUE
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/SataController/Dxe/SataController.inf
!endif

#
# NorthCluster
#
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster/SmmAccess/Dxe/SmmAccess.inf

  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster/ChvInit/Dxe/ChvInit.inf {
   <BuildOptions>
   !if $(FSP_BOOT_ENABLE) == TRUE
    *_*_IA32_CC_FLAGS      = -U FSP_FLAG 
    *_*_X64_CC_FLAGS       = -U FSP_FLAG  
   !endif
  }

  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster/ISPDxe/ISPDxe.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster/SmBiosMemory/Dxe/SmBiosMemory.inf


#
# Txe
#
!if $(SEC_ENABLE) == TRUE

  
!endif

!if $(FTPM_ENABLE) == TRUE

!endif

#
# eMMC DLL Tuning
#
  $(PLATFORM_RC_PACKAGE)/Platform/PlatformEmmc/Dxe/PlatformEmmcDxe.inf

#
# AcpiTables
#
  $(PLATFORM_RC_PACKAGE)/AcpiTablesPCAT/AcpiTables.inf


