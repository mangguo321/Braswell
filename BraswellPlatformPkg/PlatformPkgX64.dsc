## @file
#  Platform description.
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
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = BraswellPlatformPkg
  PLATFORM_GUID                       = 465B0A0B-7AC1-443b-8F67-7B8DEC145F90
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005

  #
  # Set platform specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at R9 DSC and FDF.
  #
  DEFINE      PLATFORM_PACKAGE                = BraswellPlatformPkg
  DEFINE      PLATFORM2_PACKAGE               = BraswellPlatformPkg
  DEFINE      PLATFORM_RC_PACKAGE             = ChvRefCodePkg
  DEFINE      CLIENT_COMMON_PACKAGE           = ClientCommonPkg
  DEFINE      PLATFORM_AMI_CSM_PACKAGE        = Vlv2DeviceAmiCsmPkg
  DEFINE      PLATFORM_INTEL_RESTRICT_PACKAGE = ChvTbltDeviceMiscPkg
  OUTPUT_DIRECTORY                    = Build/$(PLATFORM_PACKAGE)
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT
  VPD_TOOL_GUID                       = 8C3D856A-9BE6-468E-850A-24F7A8D38E08 

  DEFINE CPU_ARCH                  = ValleyView2
  DEFINE PROJECT_SC_FAMILY         = IntelPch

  DEFINE PROJECT_SC_ROOT           = ../$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster
  DEFINE PROJECT_VLV_ROOT          = ../$(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster
  DEFINE CHIPSET_MEMORY_CONTROLLER = ValleyView2

  !include BraswellPlatformPkg/AutoPlatformCFG.txt
  !include BraswellPlatformPkg/PlatformPkgConfig.dsc
  DEFINE SIGNED_CAPSULE_ENABLE = FALSE

  FLASH_DEFINITION                    = BraswellPlatformPkg/PlatformPkg.fdf
  FIX_LOAD_TOP_MEMORY_ADDRESS         = 0x0
  DEFINE   TOP_MEMORY_ADDRESS         = 0x0

  #
  # Do not use 0x prefix, pass prefix 0x or postfix h through macro for C or ASM
  #
  DEFINE   PLATFORM_PCIEXPRESS_BASE   = 0E0000000

  #
  # Set the global variables for EDK sytle module
  # EDK_GLOBAL only takes effect to R8 INF.
  #
  EDK_GLOBAL GG_SOURCE                 = $(WORKSPACE)/$(PLATFORM2_PACKAGE)/AmiCsm/Restricted/GreensGlacier
  EDK_GLOBAL AMI_RESTRICTED_SOURCE     = $(WORKSPACE)/$(PLATFORM2_PACKAGE)/AmiCsm/Restricted
  EDK_GLOBAL INTEL_RESTRICTED_SOURCE   = $(WORKSPACE)/$(PLATFORM_INTEL_RESTRICT_PACKAGE)
  EDK_GLOBAL PROJECT_VLV_ROOT          = ../$(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster

  EDK_GLOBAL PLATFORM_PACKAGE          = $(PLATFORM_PACKAGE)
  EDK_GLOBAL PLATFORM2_PACKAGE         = $(PLATFORM2_PACKAGE)
  EDK_GLOBAL PLATFORM_RC_PACKAGE       = $(PLATFORM_RC_PACKAGE)
  EDK_GLOBAL PLATFORM_ECP_PACKAGE      = R8ChvTbltDevicePkg
  EDK_GLOBAL CPU_ARCH                  = ValleyView2
  EDK_GLOBAL PROJECT_SC_FAMILY         = IntelPch
  EDK_GLOBAL PROJECT_SC_ROOT           = ../$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster
  EDK_GLOBAL PROJECT_VLV_ROOT          = ../$(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster
  EDK_GLOBAL CHIPSET_MEMORY_CONTROLLER = ValleyView2

  EDK_GLOBAL PROJECT_SC_ROOT           = ../$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster
  EDK_GLOBAL PROJECT_ACPI_ROOT         = ../$(PLATFORM_RC_PACKAGE)/AcpiTables
  EDK_GLOBAL PROJECT_PPM_ROOT          = ../$(PLATFORM_RC_PACKAGE)/PowerManagement
  EDK_GLOBAL PROJECT_DTS_ROOT          = ../$(PLATFORM_RC_PACKAGE)/DigitalThermalSensor
  EDK_GLOBAL PROJECT_SEC_ROOT          = ../$(PLATFORM_RC_PACKAGE)/SeC

  EDK_GLOBAL PROJECT_SC                = SouthCluster
  EDK_GLOBAL PROJECT_DTS               = DigitalThermalSensor
  EDK_GLOBAL CHIPSET_MEMORY_CONTROLLER = ValleyView2

  EDK_GLOBAL EFI_ACPI_TABLE_STORAGE_GUID = 7E374E25-8E01-4FEE-87F2-390C23C606CD
  EDK_GLOBAL EFI_ACPI_OEM_FADT_REVISION  = 0x0001

  EDK_GLOBAL UEFI_PREFIX = Uefi
  EDK_GLOBAL PI_PERFIX   =

  DEFINE PCH_INIT_ROOT          = ChvRefCodePkg/CherryViewSoc/SouthCluster/PchInit
  DEFINE DXE_ARCHITECTURE       = X64
  DEFINE EDK_DXE_ARCHITECTURE   = X64  
  
  !if $(ENABLE_FAST_BOOT)
    DEFINE PERFORMANCE_ENABLE = TRUE   
  !endif

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses.common]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  DxeSmmDriverEntryPoint|IntelFrameworkPkg/Library/DxeSmmDriverEntryPoint/DxeSmmDriverEntryPoint.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
#  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf

  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiDecompressLib|IntelFrameworkModulePkg/Library/BaseUefiTianoCustomDecompressLib/BaseUefiTianoCustomDecompressLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiCpuLib|UefiCpuPkg/Library/BaseUefiCpuLib/BaseUefiCpuLib.inf
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  GenericBdsLib|$(PLATFORM_PACKAGE)/Override/IntelFrameworkModulePkg/Library/GenericBdsLib/GenericBdsLib.inf
#  PlatformBdsLib|$(PLATFORM_PACKAGE)/Library/PlatformBdsLib/PlatformBdsLib_BB.inf
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  FlashDeviceLib|$(PLATFORM_PACKAGE)/Library/FlashDeviceLib/FlashDeviceLib.inf
  #
  # Framework
  #
!if $(S3_ENABLE) == TRUE
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
!else
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
!endif
  S3IoLib|MdePkg/Library/BaseS3IoLib/BaseS3IoLib.inf
  S3PciLib|MdePkg/Library/BaseS3PciLib/BaseS3PciLib.inf

  #
  # Generic Modules
  #
!if $(USB_ENABLE) == TRUE
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
!endif
!if $(SCSI_ENABLE) == TRUE
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
!endif
!if $(NETWORK_ENABLE) == TRUE
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib|MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib|MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  TcpIoLib|MdeModulePkg/Library/DxeTcpIoLib/DxeTcpIoLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
!endif
!if $(S3_ENABLE) == TRUE
  S3Lib|IntelFrameworkModulePkg/Library/PeiS3Lib/PeiS3Lib.inf
!endif
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
!if $(CAPSULE_ENABLE) == TRUE
  CapsuleLib|IntelFrameworkModulePkg/Library/DxeCapsuleLib/DxeCapsuleLib.inf
!else
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
!endif
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  IoApicLib|PcAtChipsetPkg/Library/BaseIoApicLib/BaseIoApicLib.inf

  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  #
  # CPU
  #
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  CpuConfigLib|BraswellPlatformPkg/IntelSiliconBasic/Library/CpuConfigLib/CpuConfigLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf

  #
  # ICH
  #
  SmbusLib|ChvRefCodePkg/CherryViewSoc/SouthCluster/Library/SmbusLib/SmbusLib.inf
  SmmLib|MdePkg/Library/SmmLibNull/SmmLibNull.inf
  SmmIoLib|$(PLATFORM_PACKAGE)/Library/SmmIo/SmmIoLib.inf

  #PchPlatformLib|ChvRefCodePkg/CherryViewSoc/SouthCluster/Library/PchPlatformLib/PchPlatformLib.inf

  #
  # Platform
  #
  ResetSystemLib|$(PLATFORM_PACKAGE)/Library/ResetSystemLib/ResetSystemLib.inf

  #
  # Misc
  #
!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
!endif
  PlatformHookLib|BraswellPlatformPkg/Library/PlatformHookLib/PlatformHookLib.inf 
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
!if $(TPM_ENABLED) == TRUE
  TpmCommLib|SecurityPkg/Library/TpmCommLib/TpmCommLib.inf
  Tpm12CommandLib|SecurityPkg/Library/Tpm12CommandLib/Tpm12CommandLib.inf
  Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf

!endif
  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf

  #
  # CryptLib
  #
!if $(TPM_ENABLED) == TRUE
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
!endif

#
# For R9 compilation
#
  CpuIA32Lib|$(PLATFORM_PACKAGE)/Library/CpuIA32Lib/CpuIA32Lib.inf

  StallSmmLib|$(PLATFORM_PACKAGE)/Library/StallSmmLib/StallSmmLib.inf

  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf

  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
!if ($(HYBRID_ENABLE) == TRUE)|| ($(FTPM_ENABLE) == TRUE)
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
!endif
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  TrEEPhysicalPresenceLib|SecurityPkg/Library/DxeTrEEPhysicalPresenceLib/DxeTrEEPhysicalPresenceLib.inf
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
  TrEEPpVendorLib|SecurityPkg/Library/TrEEPpVendorLibNull/TrEEPpVendorLibNull.inf
#
# RC Common Library
#
!include $(PLATFORM_RC_PACKAGE)/ClientRefCodePkgCommonLib.dsc


  # FSP lib
  FspApiLib|IntelFspWrapperPkg/Library/BaseFspApiLib/BaseFspApiLib.inf

  # FSP platform sample
  FspPlatformInfoLib|$(PLATFORM_PACKAGE)/FspSupport/Library/BaseFspPlatformInfoLibSample/BaseFspPlatformInfoLibSample.inf
  FspPlatformSecLib|$(PLATFORM_PACKAGE)/Override/IntelFspWrapperPkg/Library/SecPeiFspPlatformSecLibSample/SecPeiFspPlatformSecLibSample.inf
  FspHobProcessLib|$(PLATFORM_PACKAGE)/FspSupport/Library/PeiFspHobProcessLib/PeiFspHobProcessLib.inf
  BltLib|$(PLATFORM_PACKAGE)/FspSupport/Library/FrameBufferBltLib/FrameBufferBltLib.inf
  RecoveryOemHookLib|$(PLATFORM_PACKAGE)/Library/RecoveryOemHookLib/RecoveryOemHookLib.inf
  AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf

[LibraryClasses.IA32]
#
# RC Pei Library
#
!include $(PLATFORM_RC_PACKAGE)/ClientRefCodePkgPeiLib.dsc

  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/BaseAcpiTimerLib.inf
  GpioLib|ChvRefCodePkg/CherryViewSoc/SouthCluster/Library/GpioLib/GpioLib.inf

[LibraryClasses.IA32.SEC]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf  # RP_BDW_CorePkgDifference
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf


[LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE]
  #
  # PEI phase common
  #

  BiosIdLib|BraswellPlatformPkg/Library/BiosIdLib/BiosIdLib.inf
  I2CLibPei|ChvRefCodePkg/CherryViewSoc/SouthCluster/Library/I2CLibPei/I2CLibPei.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  MultiPlatformLib|$(PLATFORM_PACKAGE)/Library/MultiPlatformLib/MultiPlatformLib.inf
!if $(TPM_ENABLED) == TRUE
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterPei.inf
!endif
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf

!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
!endif

!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!else
  DebugLib|IntelFrameworkModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!endif
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxPeiLib.inf

PeiKscLib|$(PLATFORM_PACKAGE)/Library/Ksc/Pei/PeiKscLib.inf
HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterPei.inf

[LibraryClasses.X64]
  #
  # DXE phase common
  #
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf

  TcgPhysicalPresenceLib|SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf
  BiosIdLib|BraswellPlatformPkg/Library/BiosIdLib/BiosIdLib.inf
!if $(TPM_ENABLED) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
!endif
  DxeKscLib|$(PLATFORM_PACKAGE)/Library/Ksc/Dxe/DxeKscLib.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
  EfiRegTableLib|$(PLATFORM_PACKAGE)/Library/EfiRegTableLib/EfiRegTableLib.inf

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf

!if $(TPM_ENABLED) == TRUE
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
!endif
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterDxe.inf
#
# RC Dxe Library
#
!include $(PLATFORM_RC_PACKAGE)/ClientRefCodePkgDxeLib.dsc
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf  # RP_BDW_CorePkgDifference
CpuExceptionHandlerLib|MdeModulePkg/Library/CpuExceptionHandlerLibNull/CpuExceptionHandlerLibNull.inf  # RP_BDW_CorePkgDifference
  CustomizedDisplayLib|MdeModulePkg\Library\CustomizedDisplayLib\CustomizedDisplayLib.inf

[LibraryClasses.X64.PEIM]
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf


[LibraryClasses.X64.DXE_DRIVER]
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
!endif

[LibraryClasses.X64.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/DxeCorePerformanceLib/DxeCorePerformanceLib.inf
!endif
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf  # RP_BDW_CorePkgDifference

[LibraryClasses.X64.DXE_SMM_DRIVER]
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxSmmLib.inf
  SmmCpuPlatformHookLib|BraswellPlatformPkg/IntelSiliconBasic/Library/SmmCpuPlatformHookLibNull/SmmCpuPlatformHookLibNull.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/SmmPerformanceLib/SmmPerformanceLib.inf
!endif
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
#
# RC Smm Library
#
  !include $(PLATFORM_RC_PACKAGE)/ClientRefCodePkgSmmLib.dsc
  SmmIoLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/SampleCode/Library/SmmIo/SmmIo.inf
  SmmKscLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/SampleCode/Library/Ksc/Smm/SmmKscLib.inf
  DTSHookLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/SampleCode/Library/DTSHookLib/Smm/DTSHookLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf  # RP_BDW_CorePkgDifference

[LibraryClasses.X64.SMM_CORE]
  MemoryAllocationLib|MdeModulePkg/Library/PiSmmCoreMemoryAllocationLib/PiSmmCoreMemoryAllocationLib.inf
  SmmServicesTableLib|MdeModulePkg/Library/PiSmmCoreSmmServicesTableLib/PiSmmCoreSmmServicesTableLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  SmmCorePlatformHookLib|MdeModulePkg/Library/SmmCorePlatformHookLibNull/SmmCorePlatformHookLibNull.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf

!if $(TPM_ENABLED) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
!endif
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/SmmCorePerformanceLib/SmmCorePerformanceLib.inf
!endif

[LibraryClasses.X64.DXE_RUNTIME_DRIVER]
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/RuntimeCryptLib.inf
!if $(TPM_ENABLED) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/RuntimeCryptLib.inf
!endif

[LibraryClasses.X64.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag.common]

  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserGrayOutTextStatement|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdFrameworkCompatibilitySupport|FALSE
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdStatusCodeUseDataHub|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreImageLoaderSearchTeSectionFirst|FALSE
!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory|FALSE

  ## This PCD specifies whether PS2 keyboard does a extended verification during start.
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdPs2KbdExtendedVerification|FALSE

  ## This PCD specifies whether PS2 mouse does a extended verification during start.
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdPs2MouseExtendedVerification|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableCollectStatistics|FALSE

  gEfiCpuTokenSpaceGuid.PcdCpuSmmBlockStartupThisAp|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  gEfiCpuTokenSpaceGuid.PcdCpuSmmProfileEnable|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryOnIdeDisk|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryOnFatFloppyDisk|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryOnDataCD|FALSE
  
[PcdsFeatureFlag.X64]  
  gEfiCpuTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE
  
[PcdsFixedAtBuild.common]
  gFspWrapperTokenSpaceGuid.PcdTemporaryRamSize|0x00020000

  gChvRefCodePkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x10000

!if $(SECURE_BOOT_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x22000
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x7000
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00000800
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x400
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x200000
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeimPerFv|50
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPpiSupported|128
  gEfiCpuTokenSpaceGuid.PcdCpuSmmApSyncTimeout|1000
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif
!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
!else
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0
!endif

  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdEbdaReservedMemorySize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|$(TOP_MEMORY_ADDRESS)
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor|0x01
!if $(SECURE_BOOT_ENABLE) == TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmStackSize|0x20000

  ## Pcd for OptionRom.
  #  Image verification policy settings:
  #  ALWAYS_EXECUTE                         0x00000000
  #  NEVER_EXECUTE                          0x00000001
  #  ALLOW_EXECUTE_ON_SECURITY_VIOLATION    0x00000002
  #  DEFER_EXECUTE_ON_SECURITY_VIOLATION    0x00000003
  #  DENY_EXECUTE_ON_SECURITY_VIOLATION     0x00000004
  #  QUERY_USER_ON_SECURITY_VIOLATION       0x00000005
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy | 0x00000004
!endif

  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdBiosVideoCheckVbeEnable|TRUE
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdBiosVideoCheckVgaEnable|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x0
#[PcdsFixedAtBuild.IA32.PEIM, PcdsFixedAtBuild.IA32.PEI_CORE, PcdsFixedAtBuild.IA32.SEC]

  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber|0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber|31
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber|0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x40
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask|0x02
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset|0x40
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress|0x400
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset|0x08
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask|0xFF80

[PcdsFixedAtBuild.IA32]
#  gFspWrapperTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x2000
!if $(PERFORMANCE_ENABLE) == TRUE
  gFspWrapperTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x1600
!else
  gFspWrapperTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x2600
!endif
  gFspWrapperTokenSpaceGuid.PcdCpuMicrocodePatchAddress|0xFFC60000
  gFspWrapperTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0x00023000
  gFspWrapperTokenSpaceGuid.PcdFlashMicroCodeOffset|0x60
  gFspWrapperTokenSpaceGuid.PcdFlashCodeCacheAddress|0xFFC00000
  gFspWrapperTokenSpaceGuid.PcdFlashCodeCacheSize|0x00400000
  gFspWrapperTokenSpaceGuid.PcdFlashFvFspBase|0xFFF20000
  gFspWrapperTokenSpaceGuid.PcdFlashFvFspSize|0x0009E000 
  gFspWrapperTokenSpaceGuid.PcdMaxUpdRegionSize|0xB60
  
[PcdsFixedAtBuild.X64]
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif


[PcdsPatchableInModule.common]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80380546
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x$(PLATFORM_PCIEXPRESS_BASE)
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdLegacyBiosCacheLegacyRegion|FALSE

  ## This PCD specifies whether to use the optimized timing for best PS2 detection performance.
  #  Note this PCD could be set to TRUE for best boot performance and set to FALSE for best device compatibility.
  #gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdFastPS2Detection|TRUE|BOOLEAN|0x3000000b
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdFastPS2Detection|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|0x00010000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Intel Corporation"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSBiosVersion|"Cherryview Platform BIOS"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSSystemUuid|{0xa5, 0x00, 0x02, 0x88, 0x64, 0x62, 0x45, 0x24, 0x98, 0x6a, 0x9b, 0x77, 0x37, 0xe3, 0x15, 0xcf}
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSSystemManufacturer|"Intel Corporation"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSSystemVersion|"0.1"|VOID* |0x40
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSSystemSerialNumber|"112233445566"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSBoardManufacturer|"Intel Corp."|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSBoardVersion|"FAB"|VOID* |0x20
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSBoardSerialNumber|"1"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSChassisManufacturer|"Intel Corporation"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSChassisVersion|"0.1"|VOID* |0x20
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSChassisSerialNumber|"serial"|VOID* |0x80
  gPlatformModuleTokenSpaceGuid.PcdSMBIOSChassisAssetTag|"Asset Tag"|VOID* |0x20
  #
  # SIO HS-UART0 for Debug
  #
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate               |115200
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate              |115200
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseMmio               |TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase          |0x9000B000
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialLineControl           |0x03
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialFifoControl           |0x07
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialDetectCable           |FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterStride        |4
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialClockRate             |44236800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialPciDeviceInfo         |{0x1E, 0x03, 0x84, 0x00, 0xFF}|33

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|"Intel "
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x20202020324B4445
  gEfiEdkIIPlatformTokenSpaceGuid.PcdSsidSvid|0x12348086
  gPlatformModuleTokenSpaceGuid.PcdCustomizedVbtFile|{0xFF, 0xFF, 0xFF, 0xFF} |4  # Reserve n bytes for customized vbt
!if $(ENABLE_FAST_BOOT)  
  gPlatformModuleTokenSpaceGuid.PcdEnableFastBoot|TRUE
!else
  gPlatformModuleTokenSpaceGuid.PcdEnableFastBoot|FALSE
!endif

  gChvFspBinPkgTokenSpaceGuid.PcdMrcInitSpdAddr1|0xA0
  gChvFspBinPkgTokenSpaceGuid.PcdMrcInitSpdAddr2|0xA0
  
[PcdsDynamicHii.common.DEFAULT]
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"

  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdBootState|L"BootState"|gEfiBootStateGuid|0x0|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|L"ConOutConfig"|gClientCommonModuleTokenSpaceGuid|0x0|100
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|L"ConOutConfig"|gClientCommonModuleTokenSpaceGuid|0x4|31

[PcdsDynamicDefault.common.DEFAULT]
  gEfiEdkIIPlatformTokenSpaceGuid.PcdPlatformInfo|{0x0}|VOID*|0x100

  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0
  gEfiCHVTokenSpaceGuid.PcdBmpImageGuid |{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

  ## This PCD defines the video horizontal resolution.
  #  This PCD could be set to 0 then video resolution could be at highest resolution.
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0
  ## This PCD defines the video vertical resolution.
  #  This PCD could be set to 0 then video resolution could be at highest resolution.
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0

  ## This PCD defines the Console output column and the default value is 25 according to UEFI spec.
  #  This PCD could be set to 0 then console output could be at max column and max row.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|31|UINT32|0x40000006
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|31
  ## This PCD defines the Console output row and the default value is 80 according to UEFI spec.
  #  This PCD could be set to 0 then console output could be at max column and max row.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|100|UINT32|0x40000007
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|100

  ## The PCD is used to specify the video horizontal resolution of text setup.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|800|UINT32|0x50000001
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|800
  ## The PCD is used to specify the video vertical resolution of text setup.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|600|UINT32|0x50000002
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|600
  ## The PCD is used to specify the console output column of text setup.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|100|UINT32|0x50000003
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|100
  ## The PCD is used to specify the console output column of text setup.
  #gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|31|UINT32|0x50000004
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|31

  gPlatformModuleTokenSpaceGuid.PcdBootToFirmwareUserInterface|FALSE
!if $(TPM_ENABLED) == TRUE
  ## Put Ptt guid here: e.g. { 0xf9c6a62f, 0xc60f, 0x4b44, { 0xa6, 0x29, 0xed, 0x3d, 0x40, 0xae, 0xfa, 0x5f } }
  ## TPM1.2 { 0x8b01e5b6, 0x4f19, 0x46e8, { 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc } }
  ## TPM2.0Dtpm { 0x286bf25a, 0xc2c3, 0x408c, { 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17 } }
  ## TPM2.0Ptt { 0x72cd3a7b, 0xfea5, 0x4f5e, { 0x91, 0x65, 0x4d, 0xd1, 0x21, 0x87, 0xbb, 0x13 } }
  #gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0xb6, 0xe5, 0x01, 0x8b, 0x19, 0x4f, 0xe8, 0x46, 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc}
  #gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x7b, 0x3a, 0xcd, 0x72, 0xA5, 0xFE, 0x5e, 0x4f, 0x91, 0x65, 0x4d, 0xd1, 0x21, 0x87, 0xbb, 0x13}
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmAutoDetection|FALSE

  gEfiSecurityPkgTokenSpaceGuid.PcdTrEEAvailableEventLogs|0x00000001
  gEfiSecurityPkgTokenSpaceGuid.PcdTrEEHashAlgorithmBitmap|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTrEESupportedEventLogs|0xFFFFFFFF
  gEfiSecurityPkgTokenSpaceGuid.PcdTrEEProtocolVersion|0x0001

  gClientCommonModuleTokenSpaceGuid.PcdTpm2HierarchyChangeAuthPlatform|1
  gClientCommonModuleTokenSpaceGuid.PcdTpm2HierarchyControlPlatform|1
  gClientCommonModuleTokenSpaceGuid.PcdTpm2HierarchyControlEndorsement|1
  gClientCommonModuleTokenSpaceGuid.PcdTpm2HierarchyControlOwner|1
  gClientCommonModuleTokenSpaceGuid.PcdTpm2ChangeEps|0
  gClientCommonModuleTokenSpaceGuid.PcdTpm2ChangePps|0
  gClientCommonModuleTokenSpaceGuid.PcdTpm2Clear|0

  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2InitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2SelfTestPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2ScrtmPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmScrtmPolicy|1
!endif

[PcdsDynamicExDefault.common.DEFAULT]
  gEfiEdkIIPlatformTokenSpaceGuid.PcdSystemConfiguration|{0x0}|VOID*|0x3A0


[Components.IA32]

  IntelFspWrapperPkg/FspWrapperSecCore/FspWrapperSecCore.inf {
  <LibraryClasses>
    FspPlatformInfoLib|$(PLATFORM_PACKAGE)/FspSupport/Library/BaseFspPlatformInfoLibSample/BaseFspPlatformInfoLibSample.inf
    DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
    SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  <PcdsPatchableInModule>
    gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xF0000043
  <PcdsFixedAtBuild>
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
  <Defines>
    FILE_GUID = 1BA0062E-C779-4582-8566-336AE8F78F09
}
  IntelFspWrapperPkg/FspInitPei/FspInitPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }

  MdeModulePkg/Core/Pei/PeiMain.inf {
!if $(TARGET) == DEBUG
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2E
!endif
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
  }
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf 
#
# RC PEI Component
#
!include $(PLATFORM_RC_PACKAGE)/ClientRefCodePkgPei.dsc

  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf
  MdeModulePkg/Universal/Variable/Pei/VariablePei.inf

  $(PLATFORM_PACKAGE)/PlatformPei/PlatformPei.inf {
    <LibraryClasses>
#       NULL|BraswellPlatformPkg\Board\BraswellCR\BoardInit\BoardInit.inf
#       NULL|BraswellPlatformPkg\Board\BraswellCherryHill\BoardInit\BoardInit.inf      
       NULL|BraswellPlatformPkg\Board\BraswellWestDigital\BoardInit\BoardInit.inf
#      SeCUmaLib|ChvRefCodePkg/Txe/SeCUma/SeCUma.inf
!if ($(FTPM_ENABLED) == TRUE)
      fTPMInitLib|ChvRefCodePkg/Txe/fTPM/Pei/fTPMInitPei.inf
!endif
    !if $(TARGET) == DEBUG
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
    !else
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
    !endif
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf

!if $(ISH_ENABLE) == TRUE
      IshInitLib|ChvRefCodePkg/Txe/ISH/Pei/IshInit.inf
!endif
    <BuildOptions>
!if $(SeC_DEBUG_INFO_ENABLE) == TRUE
  *_*_IA32_CC_FLAGS      = /DSeC_DEBUG_INFO=1
!else
  *_*_IA32_CC_FLAGS      = /DSeC_DEBUG_INFO=0
!endif
!if $(FTPM_ENABLE)==TRUE
  *_*_IA32_CC_FLAGS = /D FTPM_ENABLE
!endif
!if $(ISH_ENABLE) == TRUE
  *_*_IA32_CC_FLAGS      = /DISH_ENABLE=1
!endif
  }

!if $(TPM_ENABLED) == TRUE
  SecurityPkg/Tcg/PhysicalPresencePei/PhysicalPresencePei.inf
  SecurityPkg/Tcg/TcgPei/TcgPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
}
!endif
  $(PLATFORM_PACKAGE)/PlatformInitPei/PlatformEarlyInit.inf {
    <LibraryClasses>
#      NULL|BraswellPlatformPkg\Board\BraswellCR\BoardInit\BoardInitLate.inf
#      NULL|BraswellPlatformPkg\Board\BraswellCherryHill\BoardInit\BoardInitLate.inf
      NULL|BraswellPlatformPkg\Board\BraswellWestDigital\BoardInit\BoardInitLate.inf
!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }

  MdeModulePkg/Universal/PCD/Pei/Pcd.inf

  UefiCpuPkg/CpuIoPei/CpuIoPei.inf
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf
  BraswellPlatformPkg/IntelSiliconBasic/PiSmmCommunication/PiSmmCommunicationPei.inf

!if $(CAPSULE_ENABLE) == TRUE
  MdeModulePkg/Universal/CapsulePei/CapsulePei.inf
!endif
  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
    NULL|IntelFrameworkModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

!if ($(TPM_ENABLED) == TRUE)
   SecurityPkg/Tcg/TrEEConfig/TrEEConfigPei.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|ChvRefCodePkg/Txe/Library/Tpm2DeviceLibSeC/Tpm2InstanceLibFtpm.inf
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }

   SecurityPkg/Tcg/TrEEPei/TrEEPei.inf {
    <LibraryClasses>
#      Tpm2DeviceLib|BraswellPlatformPkg/Restricted/Override/SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterPei.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
!if $(FTPM_ENABLE) == TRUE
      NULL|ChvRefCodePkg/Txe/Library/Tpm2DeviceLibSeC/Tpm2InstanceLibFtpm.inf
!endif
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }

!if $(FTPM_ENABLE) == TRUE
  ChvRefCodePkg/Txe/fTPM/Pei/fTPMInitPei.inf
!endif

!endif

!if $(ACPI50_ENABLE) == TRUE
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTablePei/FirmwarePerformancePei.inf{
    <LibraryClasses>
      TimerLib|PerformancePkg/Library/TscTimerLib/PeiTscTimerLib.inf
  }

!endif

  #
  # Recovery Module
  #
  ChvRefCodePkg/CherryViewSoc/SouthCluster/Usb/Pei/PchUsb.inf
  MdeModulePkg/Bus/Pci/XhciPei/XhciPei.inf
  MdeModulePkg/Bus/Usb/UsbBusPei/UsbBusPei.inf
  MdeModulePkg/Bus/Usb/UsbBotPei/UsbBotPei.inf
  FatPkg/FatPei/FatPei.inf

[Components.X64]
  #
  # EDK II Related Platform codes
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
    <LibraryClasses>
      NULL|IntelFrameworkModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }
  IntelFrameworkModulePkg/Universal/Acpi/AcpiS3SaveDxe/AcpiS3SaveDxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xF0000043
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
  }

  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  
  BraswellPlatformPkg/PcdConfigHook/DxePcdConfigHook.inf

  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf

  IntelFrameworkModulePkg/Universal/CpuIoDxe/CpuIoDxe.inf
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf
!if $(CAPSULE_ENABLE) == TRUE
  MdeModulePkg/Universal/CapsulePei/CapsuleX64.inf {
    <LibraryClasses>
      CpuExceptionHandlerLib|MdeModulePkg/Library/CpuExceptionHandlerLibNull/CpuExceptionHandlerLibNull.inf
  }
!endif
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf  {
    <LibraryClasses>
!if $(TARGET) != RELEASE
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
  }
  MdeModulePkg/Universal/StatusCodeHandler/Smm/StatusCodeHandlerSmm.inf

  MdeModulePkg/Universal/ReportStatusCodeRouter/Smm/ReportStatusCodeRouterSmm.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf{
    <LibraryClasses>
!if $(SECURE_BOOT_ENABLE) == TRUE
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
!endif
#!if $(TPM_ENABLED) == TRUE
     # NULL|SecurityPkg/Library/DxeTpmMeasureBootLib/DxeTpmMeasureBootLib.inf
#!endif
!if ($(FTPM_ENABLE) == TRUE) || ($(TPM_ENABLED) == TRUE)
      NULL|SecurityPkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
!endif
  }

MdeModulePkg/Universal/Metronome/Metronome.inf

#
# RC DXE Component
#
!include $(PLATFORM_RC_PACKAGE)/ClientRefCodePkgDxe.dsc

BraswellPlatformPkg\IntelSiliconBasic\CpuInit\MpCpu.inf
BraswellPlatformPkg\IntelSiliconBasic\PciHostBridge\PciHostBridge.inf

IntelFspWrapperPkg/FspNotifyDxe/FspNotifyDxe.inf

IntelFrameworkModulePkg/Universal/BdsDxe/BdsDxe.inf{
  <LibraryClasses>
   OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
   IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
   PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
   BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
   PlatformBdsLib|$(PLATFORM_PACKAGE)/Library/PlatformBdsLib/PlatformBdsLib.inf
   Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
   #TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf
!if $(TPM_ENABLED) == TRUE
   NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
!endif
!if $(FTPM_ENABLE) == TRUE
   NULL|ChvRefCodePkg/Txe/Library/Tpm2DeviceLibSeC/Tpm2InstanceLibFtpm.inf
!endif
}

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  
  BraswellPlatformPkg/Flash/SpiFlashParts/MX25/MX25.inf
  
!if $(SMM_VARIABLE_ENABLE) == TRUE
  BraswellPlatformPkg/Flash/SpiDeviceDxe/SpiDeviceSmm.inf
  BraswellPlatformPkg/Flash/SpiDeviceDxe/SpiDeviceSmmDxe.inf
  BraswellPlatformPkg/Flash/FvbRuntimeDxe/FvbSmm.inf
  BraswellPlatformPkg/Flash/FvbRuntimeDxe/FvbSmmDxe.inf 
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteSmm.inf 
!else
  BraswellPlatformPkg/SpiDeviceDxe/SpiDeviceDxe.inf
  BraswellPlatformPkg/FvbRuntimeDxe/FvbRuntimeDxe.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf  

!endif  
  
  #
  # Authenticated variable modules
  #
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
      NULL|MdeModulePkg/Library/VarCheckPcdLib/VarCheckPcdLib.inf
  }
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmmRuntimeDxe.inf  {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
      NULL|MdeModulePkg/Library/VarCheckPcdLib/VarCheckPcdLib.inf
  }
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf{
  <LibraryClasses>
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
  !if $(SECURE_BOOT_ENABLE) == TRUE
      AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
!else
      AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
!endif
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
      NULL|MdeModulePkg/Library/VarCheckPcdLib/VarCheckPcdLib.inf
}
  SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf 
  
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf{
     <LibraryClasses>
          FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  }
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  PcAtChipsetPkg/PcatRealTimeClockRuntimeDxe/PcatRealTimeClockRuntimeDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  $(PLATFORM_PACKAGE)/Setup/PlatformSetupDxe.inf

!if $(DATAHUB_ENABLE) == TRUE
  IntelFrameworkModulePkg/Universal/DataHubDxe/DataHubDxe.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|0
  }
!endif
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

  PcAtChipsetPkg/HpetTimerDxe/HpetTimerDxe.inf

!if ($(TPM_ENABLED) == TRUE) || ($(FTPM_ENABLE) == TRUE)
  SecurityPkg/Tcg/MemoryOverwriteControl/TcgMor.inf
  SecurityPkg/Tcg/TrEEDxe/TrEEDxe.inf {
    <LibraryClasses>
#      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|ChvRefCodePkg/Txe/Library/Tpm2DeviceLibSeC/Tpm2InstanceLibFtpm.inf
  }
!endif

!if $(TPM_ENABLED) == TRUE
    SecurityPkg/Tcg/TrEESmm/TrEESmm.inf
    SecurityPkg/Tcg/TrEEConfig/TrEEConfigDxe.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      NULL|ChvRefCodePkg/Txe/Library/Tpm2DeviceLibSeC/Tpm2InstanceLibFtpm.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
!endif

  #
  # EDK II Related Platform codes
  #
  $(PLATFORM_PACKAGE)/Acpi/AcpiSmm/AcpiSmm.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xF0000043
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x3F
  
  }
  $(PLATFORM_PACKAGE)/PlatformDxe/PlatformDxe.inf {
    <LibraryClasses>
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
!if $(TPM_ENABLED) == TRUE
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
!endif
!if $(FTPM_ENABLE) == TRUE
      NULL|ChvRefCodePkg/Txe/Library/Tpm2DeviceLibSeC/Tpm2InstanceLibFtpm.inf
!endif
    <BuildOptions>
     !if $(HYBRID_ENABLE) == TRUE
       *_*_X64_CC_FLAGS = /D _PSSD_FIX_=1
     !else
       *_*_X64_CC_FLAGS = /D _PSSD_FIX_=0
     !endif
     !if $(EMMC_DRIVER_ENABLE) == TRUE
       *_*_X64_CC_FLAGS = /D EMMC_ENABLE=1
     !else
       *_*_X64_CC_FLAGS = /D EMMC_ENABLE=0
     !endif
     !if $(FSP_BOOT_ENABLE) == TRUE
       *_*_X64_CC_FLAGS       = -U FSP_FLAG  
     !endif
  }

  $(PLATFORM_PACKAGE)/AdvancedFeature/PciPlatform/PciPlatform.inf
!if $(GOP_DRIVER_ENABLE) == TRUE
  $(PLATFORM_PACKAGE)/FspSupport/GraphicsOutputDxe/GraphicsOutputDxe.inf  {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  }
!endif

  #
  # SMM
  #
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf
  MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf
  #
  #Override: Add CedarView and CherryView CPU information for smm
  #
  BraswellPlatformPkg/IntelSiliconBasic/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x3F
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
  BraswellPlatformPkg/IntelSiliconBasic/PiSmmCommunication/PiSmmCommunicationSmm.inf

  #
  # ACPI
  #

   MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xF0000043
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }

  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf

  $(PLATFORM_PACKAGE)/Acpi/AcpiPlatform/AcpiPlatform.inf {
    <BuildOptions>
       *_*_X64_CC_FLAGS = /D SUPPORT_64BIT_D=1
  }
#  $(PLATFORM_PACKAGE)/Board/BraswellCR/Acpi/Acpi.inf
#  $(PLATFORM_PACKAGE)/Board/BraswellCherryHill/Acpi/Acpi.inf
  $(PLATFORM_PACKAGE)/Board/BraswellWestDigital/Acpi/Acpi.inf
  
  #
  # PCI
  #
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

#
# ISA
#
  $(PLATFORM_PACKAGE)/Sio/Wpce791/Wpce791.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaBusDxe/IsaBusDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaIoDxe/IsaIoDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaSerialDxe/IsaSerialDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/Ps2MouseDxe/Ps2MouseDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2keyboardDxe.inf

#
# SDIO
#
!if $(ACPI50_ENABLE) == TRUE
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf {
    <LibraryClasses>
      TimerLib|PerformancePkg/Library/TscTimerLib/DxeTscTimerLib.inf
  }
  MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableSmm/FirmwarePerformanceSmm.inf {
    <LibraryClasses>
      TimerLib|PerformancePkg/Library/TscTimerLib/DxeTscTimerLib.inf
  }
  MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
!endif

#
# IDE/SCSI/AHCI
#
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
!if $(SCSI_ENABLE) == TRUE
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
!endif
#
# Console
#
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  #
  # USB
  #
!if $(USB_ENABLE) == TRUE
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Pci/UhciDxe/UhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf {
    <BuildOptions>
      *_*_*_CC_FLAGS = $(C_PROJ_FLAGS) /Od /D EFI_DEBUG /GL-
  }
!endif

  #
  # SMBIOS
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  MdeModulePkg/Universal/SectionExtractionDxe/SectionExtractionDxe.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
      NULL|SecurityPkg/Library/DxeRsa2048Sha256GuidedSectionExtractLib/DxeRsa2048Sha256GuidedSectionExtractLib.inf
  }
  $(PLATFORM_PACKAGE)/AdvancedFeature/SmBiosMiscDxe/SmbiosMiscDxe.inf


  #
  # CPU/FW Microde
  #
  $(PLATFORM2_PACKAGE)/Microcode/MicrocodeUpdatesFsp.inf

  #
  # Legacy Modules
  #
  PcAtChipsetPkg/8259InterruptControllerDxe/8259.inf

  #
  # Network Modules
  #
!if $(NETWORK_ENABLE) == TRUE
  !if $(NETWORK_ISCSI_ENABLE) == TRUE
    !if $(NETWORK_IP6_ENABLE) == TRUE
      NetworkPkg/IScsiDxe/IScsiDxe.inf
    !else
      MdeModulePkg/Universal/Network/IScsiDxe/IScsiDxe.inf
    !endif
  !endif
  !if $(NETWORK_VLAN_ENABLE) == TRUE
    MdeModulePkg/Universal/Network/VlanConfigDxe/VlanConfigDxe.inf
  !endif
  !if $(CSM_ENABLE) == TRUE
    IntelFrameworkModulePkg/Csm/BiosThunk/Snp16Dxe/Snp16Dxe.inf
  !endif
!endif

!if $(NETWORK_ENABLE) == TRUE
  #
  # UEFI network modules
  #
    MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
    MdeModulePkg/Universal/Network/SnpDxe/SnpDxe.inf

    MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
    MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
    MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
    MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
    MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
    MdeModulePkg/Universal/Network/Tcp4Dxe/Tcp4Dxe.inf {
      <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
    }
    MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
    !if $(NETWORK_IP6_ENABLE) == TRUE
      NetworkPkg/Ip6Dxe/Ip6Dxe.inf
      NetworkPkg/Dhcp6Dxe/Dhcp6Dxe.inf
      NetworkPkg/IpSecDxe/IpSecDxe.inf
      NetworkPkg/TcpDxe/TcpDxe.inf
      NetworkPkg/Udp6Dxe/Udp6Dxe.inf
      NetworkPkg/Mtftp6Dxe/Mtftp6Dxe.inf
    !endif
    !if $(NETWORK_IP6_ENABLE) == TRUE
      NetworkPkg/UefiPxeBcDxe/UefiPxeBcDxe.inf
    !else
      MdeModulePkg/Universal/Network/UefiPxeBcDxe/UefiPxeBcDxe.inf
    !endif
!endif
  
!if $(CAPSULE_ENABLE) == TRUE
  # capsule related drivers and Application.
  IntelFrameworkModulePkg/Universal/FirmwareVolume/FwVolDxe/FwVolDxe.inf
  IntelFrameworkModulePkg/Universal/FirmwareVolume/UpdateDriverDxe/UpdateDriverDxe.inf
!endif

  FatPkg/EnhancedFatDxe/Fat.inf
!if $(PERFORMANCE_ENABLE) == TRUE 
	PerformancePkg/Dp_App/Dp.inf {
    <LibraryClasses>
      SortLib|ShellPkg/Library/UefiSortLib/UefiSortLib.inf
      ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
      FileHandleLib|ShellPkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
      PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  }
!endif

[BuildOptions]
#
# Define Build Options both for EDK and EDKII drivers.
#

#
# Define token for different Platform
#

!if $(S3_ENABLE) == TRUE
  DEFINE DSC_S3_BUILD_OPTIONS = /DEFI_S3_RESUME=1
!else
  DEFINE DSC_S3_BUILD_OPTIONS =
!endif

!if $(HYBRID_ENABLE) == TRUE
  DEFINE HYBRID_BUILD_OPTIONS = /D_SLE_HYB_=1 /D_PSSD_FIX_=1
!else
  DEFINE HYBRID_BUILD_OPTIONS = /D_SLE_HYB_=0 /D_PSSD_FIX_=0
!endif

!if $(CLKGEN_CONFIG_EXTRA_ENABLE) == TRUE
  DEFINE CLKGEN_CONFIG_EXTRA_BUILD_OPTION = /DCLKGEN_CONFIG_EXTRA=1
!else
  DEFINE CLKGEN_CONFIG_EXTRA_BUILD_OPTION =
!endif

!if $(PCIESC_ENABLE) == TRUE
  DEFINE PCIESC_SUPPORT_BUILD_OPTION = /DPCIESC_SUPPORT=1
!else
  DEFINE PCIESC_SUPPORT_BUILD_OPTION =
!endif

!if $(SATA_ENABLE) == TRUE
  DEFINE SATA_SUPPORT_BUILD_OPTION = /DSATA_SUPPORT=1
!else
  DEFINE SATA_SUPPORT_BUILD_OPTION =
!endif

!if $(FTPM_ENABLE) == TRUE
  DEFINE DSC_FTPM_BUILD_OPTIONS = /DFTPM_ENABLE
!else
  DEFINE DSC_FTPM_BUILD_OPTIONS =
!endif

!if $(TPM_ENABLED) == TRUE
  DEFINE DSC_TPM_BUILD_OPTIONS = /DTPM_ENABLED
!else
  DEFINE DSC_TPM_BUILD_OPTIONS =
!endif

!if $(FSP_BOOT_ENABLE) == TRUE
  DEFINE FSP_DSC_BUILD_OPTIONS = /DFSP_FLAG
!else
  DEFINE FSP_DSC_BUILD_OPTIONS = 
!endif

  DEFINE EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS = $(HYBRID_BUILD_OPTIONS) $(CLKGEN_CONFIG_EXTRA_BUILD_OPTION) $(SATA_SUPPORT_BUILD_OPTION) $(PCIESC_SUPPORT_BUILD_OPTION) $(DSC_FTPM_BUILD_OPTIONS) $(DSC_TPM_BUILD_OPTIONS) $(DSC_HG_BUILD_OPTIONS) $(DSC_S3_BUILD_OPTIONS) $(FSP_DSC_BUILD_OPTIONS)

!if $(PERFORMANCE_ENABLE) == TRUE
  DEFINE PDB_BUILD_OPTION = /Zi
!endif

[BuildOptions.Common.EDKII]
#!if $(PERFORMANCE_ENABLE) == TRUE
#  RELEASE_*_*_DLINK_FLAGS = /DEBUG
#!endif
  DEFINE DEBUG_BUILD_OPTIONS = /D EFI_DEBUG /D DEBUG_MODE=1 /DEDKII_GLUE_DebugPrintErrorLevel=(EFI_D_ERROR)

  *_*_IA32_ASM_FLAGS         = /D EDKII_GLUE_PciExpressBaseAddress=$(PLATFORM_PCIEXPRESS_BASE)h /DEFI_S3_RESUME

  DEBUG_*_IA32_CC_FLAGS      = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_CC_FLAGS    = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  DEBUG_*_IA32_VFRPP_FLAGS   = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_VFRPP_FLAGS = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  DEBUG_*_IA32_APP_FLAGS     = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_APP_FLAGS   = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  DEBUG_*_IA32_PP_FLAGS      = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_PP_FLAGS    = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_IA32_ASLPP_FLAGS       = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)

  DEBUG_*_X64_CC_FLAGS       = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_CC_FLAGS     = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  DEBUG_*_X64_VFRPP_FLAGS    = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_VFRPP_FLAGS  = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  DEBUG_*_X64_APP_FLAGS      = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_APP_FLAGS    = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  DEBUG_*_X64_PP_FLAGS       = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_PP_FLAGS     = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_X64_ASLPP_FLAGS        = $(EDK_EDKII_DSC_FEATURE_BUILD_OPTIONS)
  
  MSFT:*_*_*_CC_FLAGS        = /wd4200


