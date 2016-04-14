/** @file
  SampleCode for Intel PCH DXE Platform Policy initialization.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchPolicyInitDxe.h"
#ifndef FSP_FLAG
#include <Library/UefiBootServicesTableLib.h>
#endif
#include <Library/DebugLib.h>
#include <Protocol/PchPlatformPolicy.h>
#include <IndustryStandard/Pci22.h>
#include <BoardCfioTable.h>
#ifdef FSP_FLAG
#include <Library/MemoryAllocationLib.h>
#else
#endif
#include <Guid/SetupVariable.h>
#include <Library/BaseMemoryLib.h>

#define PCI_CLASS_NETWORK             0x02
#define PCI_CLASS_NETWORK_ETHERNET    0x00
#define PCI_CLASS_NETWORK_OTHER       0x80

DXE_PCH_PLATFORM_POLICY_PROTOCOL  mPchPolicyData        = { 0 };
PCH_DEVICE_ENABLING               mPchDeviceEnabling    = { 0 };
PCH_USB_CONFIG                    mPchUsbConfig         = { 0 };
#ifdef PCIESC_SUPPORT
PCH_PCI_EXPRESS_CONFIG            mPchPciExpressConfig  = { 0 };
#endif
#ifdef SATA_SUPPORT
PCH_SATA_CONFIG                   mPchSataConfig        = {{{ 0 }}};
#endif
PCH_AZALIA_CONFIG                 mPchAzaliaConfig      = { 0 };
PCH_SMBUS_CONFIG                  mPchSmbusConfig       = { 0 };
PCH_MISC_PM_CONFIG                mPchMiscPmConfig      = {{ 0 }};
PCH_DEFAULT_SVID_SID              mPchDefaultSvidSid    = { 0 };
PCH_LOCK_DOWN_CONFIG              mPchLockDownConfig    = { 0 };
PCH_LPC_SIRQ_CONFIG               mSerialIrqConfig      = { 0 };
#ifdef PCIESC_SUPPORT
PCH_PWR_OPT_CONFIG                mPchPwrOptConfig      = { 0 };
#endif
PCH_LPSS_CONFIG                   mLpssConfig           = { 0 };
PCH_SCC_CONFIG                    mSccConfig            = { 0 };

UINT8 mSmbusRsvdAddresses[4] = {
  0xA0,
  0xA2,
  0xA4,
  0xA6
};

PCH_PCIE_DEVICE_ASPM_OVERRIDE mDevAspmOverride[] = {

  //
  // Sample Dummy Device
  //
  {0x8086, 0xffff, 0xff, 0xff, 0xff, PchPcieAspmDisabled, PchPcieOverrideDisabled, 0, 0}
};

//
// Function implementations
//

/**
  Initilize Intel PCH DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
PchPlatformPolicyInit (
#ifndef FSP_FLAG
  IN EFI_HANDLE       ImageHandle
#else
  IN CONST EFI_PEI_SERVICES **PeiServices
#endif
  )
{
  EFI_STATUS              Status;
  UINT8                   PortIndex;
#ifdef FSP_FLAG
  EFI_PEI_PPI_DESCRIPTOR  *PchPlatformPolicyPpiDesc;
#endif
  SYSTEM_CONFIGURATION          SystemConfiguration;
  
  CopyMem (&SystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));
  
  ///
  /// General initialization
  ///
  PortIndex                             = 0;
  mPchPolicyData.Revision               = DXE_PCH_PLATFORM_POLICY_PROTOCOL_REVISION_1;
  mPchPolicyData.BusNumber              = 0;

  mPchPolicyData.DeviceEnabling         = &mPchDeviceEnabling;
  mPchPolicyData.UsbConfig              = &mPchUsbConfig;
#ifdef PCIESC_SUPPORT
  mPchPolicyData.PciExpressConfig       = &mPchPciExpressConfig;
#endif
#ifdef SATA_SUPPORT
  mPchPolicyData.SataConfig             = &mPchSataConfig;
#endif
  mPchPolicyData.AzaliaConfig           = &mPchAzaliaConfig;
  mPchPolicyData.SmbusConfig            = &mPchSmbusConfig;

  mPchPolicyData.MiscPmConfig           = &mPchMiscPmConfig;
  mPchPolicyData.DefaultSvidSid         = &mPchDefaultSvidSid;
  mPchPolicyData.LockDownConfig         = &mPchLockDownConfig;
  mPchPolicyData.SerialIrqConfig        = &mSerialIrqConfig;
#ifdef PCIESC_SUPPORT
  mPchPolicyData.PwrOptConfig           = &mPchPwrOptConfig;
  mPchPolicyData.PchPcieAspmSwSmiNumber = SW_SMI_PCIE_ASPM_OVERRIDE;
#endif
  mPchPolicyData.LpssConfig             = &mLpssConfig;
  mPchPolicyData.SccConfig              = &mSccConfig;

  ///
  /// CHV BIOS Spec, Section 10.6 Flash Security Recommendations,
  /// Intel strongly recommends that BIOS sets the BIOS Interface Lock Down bit. Enabling this bit
  /// will mitigate malicious software attempts to replace the system BIOS option ROM with its own code.
  /// We always enable this as a platform policy.
  ///
  mPchLockDownConfig.BiosInterface          = PCH_DEVICE_ENABLE;
  mPchLockDownConfig.GlobalSmi              = PCH_DEVICE_ENABLE;
  mPchLockDownConfig.RtcLock                = PCH_DEVICE_ENABLE;
  ///
  /// While BiosLock is enabled, BIOS can only be modified from SMM after ExitPmAuth.
  ///
  mPchLockDownConfig.BiosLock               = PCH_DEVICE_ENABLE;
  mPchLockDownConfig.PchBiosLockSwSmiNumber = SW_SMI_BIOS_LOCK;
  ///
  /// Initialize policy to default values when variable isn't found.
  ///
  mPchDeviceEnabling.Azalia             = PCH_DEVICE_ENABLE;
#ifdef SATA_SUPPORT
  mPchDeviceEnabling.Sata               = PCH_DEVICE_ENABLE;
#endif
  mPchDeviceEnabling.Smbus              = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.IshEnabled         = PCH_DEVICE_DISABLE;
  mPchDeviceEnabling.LpeEnabled         = SystemConfiguration.PchLpeEnabled;

  mPchUsbConfig.Usb20PortSettings[0].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb20PortSettings[1].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb20PortSettings[2].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb20PortSettings[3].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb20PortSettings[4].Enable  = PCH_DEVICE_ENABLE;

  mPchUsbConfig.Usb30PortSettings[0].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb30PortSettings[1].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb30PortSettings[2].Enable  = PCH_DEVICE_ENABLE;
  mPchUsbConfig.Usb30PortSettings[3].Enable  = PCH_DEVICE_ENABLE;

  mPchUsbConfig.SsicEnable                 = SystemConfiguration.PchSsicEnable;
  mPchUsbConfig.HsicPortSettings[0].Enable = SystemConfiguration.PchUsbHsicPort[0];
  mPchUsbConfig.HsicPortSettings[1].Enable = SystemConfiguration.PchUsbHsicPort[0];//SystemConfiguration.PchUsbHsicPort[0];

  mPchUsbConfig.SsicPortSettings[0].Enable = SystemConfiguration.PchUsbSsicPort[0];
  mPchUsbConfig.SsicPortSettings[1].Enable = SystemConfiguration.PchUsbSsicPort[0];//SystemConfiguration.PchUsbSsicPort[0];

  mPchUsbConfig.Usb30Settings.Mode = SystemConfiguration.PchUsb30Mode;

  mPchUsbConfig.UsbOtgSettings.Enable = PCH_DEVICE_DISABLE;

  mPchUsbConfig.Usb20OverCurrentPins[0]   = PchUsbOverCurrentPin0;
  mPchUsbConfig.Usb20OverCurrentPins[1]   = PchUsbOverCurrentPin0;
  mPchUsbConfig.Usb20OverCurrentPins[2]   = PchUsbOverCurrentPin1;
  mPchUsbConfig.Usb20OverCurrentPins[3]   = PchUsbOverCurrentPin1;
  mPchUsbConfig.Usb20OverCurrentPins[4]   = PchUsbOverCurrentPin1;

  mPchUsbConfig.Usb30OverCurrentPins[0]   = PchUsbOverCurrentPin0;
  mPchUsbConfig.Usb30OverCurrentPins[1]   = PchUsbOverCurrentPin0;
  mPchUsbConfig.Usb30OverCurrentPins[2]   = PchUsbOverCurrentPin1;
  mPchUsbConfig.Usb30OverCurrentPins[3]   = PchUsbOverCurrentPin1;

#ifdef PCIESC_SUPPORT
  ///
  /// PCI Express related settings
  ///
  mPchPciExpressConfig.RootPortClockGating    = PCH_DEVICE_ENABLE;
  mPchPciExpressConfig.TempRootPortBusNumMin  = 5;
  mPchPciExpressConfig.TempRootPortBusNumMax  = 7;

  for (PortIndex = 0; PortIndex < PCH_PCIE_MAX_ROOT_PORTS; PortIndex++) {
    mPchPciExpressConfig.RootPort[PortIndex].Enable                = PCH_DEVICE_ENABLE;
    mPchPciExpressConfig.RootPort[PortIndex].FunctionNumber        = PortIndex;
    mPchPciExpressConfig.RootPort[PortIndex].PhysicalSlotNumber    = PortIndex;
    mPchPciExpressConfig.RootPort[PortIndex].Aspm                  = 4;
    mPchPciExpressConfig.RootPort[PortIndex].SlotImplemented       = 1;
    mPchPciExpressConfig.RootPort[PortIndex].L1Substates           = 3;
    mPchPciExpressConfig.RootPort[PortIndex].NonCommonClockSscMode = 0;
    mPchPciExpressConfig.RootPort[PortIndex].TxEqdeEmphSelection   = 0;
  }

  mPchPciExpressConfig.NumOfDevAspmOverride = sizeof (mDevAspmOverride) / sizeof (PCH_PCIE_DEVICE_ASPM_OVERRIDE);
  mPchPciExpressConfig.DevAspmOverride      = mDevAspmOverride;
  ///
  /// Power Optimizer related settings
  ///
  mPchPwrOptConfig.NumOfDevLtrOverride      = 0;
  mPchPwrOptConfig.DevLtrOverride           = NULL;
  for (PortIndex = 0; PortIndex < PCH_PCIE_MAX_ROOT_PORTS; PortIndex++) {
    mPchPwrOptConfig.PchPwrOptPcie[PortIndex].LtrEnable             = PCH_DEVICE_ENABLE;
    mPchPwrOptConfig.PchPwrOptPcie[PortIndex].ObffEnable            = PCH_DEVICE_DISABLE;
  }
#endif

#ifdef SATA_SUPPORT
  for (PortIndex = 0; PortIndex < PCH_AHCI_MAX_PORTS; PortIndex++) {
    mPchSataConfig.PortSettings[PortIndex].Enable           = PCH_DEVICE_ENABLE;
    mPchSataConfig.PortSettings[PortIndex].HotPlug          = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].MechSw           = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].External         = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].DevSlp           = PCH_DEVICE_DISABLE;
  }

  mPchSataConfig.RaidAlternateId  = PCH_DEVICE_DISABLE;
  mPchSataConfig.Raid0            = PCH_DEVICE_DISABLE;
  mPchSataConfig.Raid1            = PCH_DEVICE_DISABLE;
  mPchSataConfig.Raid10           = PCH_DEVICE_DISABLE;
  mPchSataConfig.Raid5            = PCH_DEVICE_DISABLE;
  mPchSataConfig.Irrt             = PCH_DEVICE_DISABLE;
  mPchSataConfig.OromUiBanner     = PCH_DEVICE_DISABLE;
  mPchSataConfig.HddUnlock        = PCH_DEVICE_DISABLE;
  mPchSataConfig.LedLocate        = PCH_DEVICE_DISABLE;
  mPchSataConfig.IrrtOnly         = PCH_DEVICE_DISABLE;
  mPchSataConfig.TestMode         = PCH_DEVICE_DISABLE;
  if (SocStepping() <= SocA1) {
    mPchSataConfig.SpeedSupport     = PchSataSpeedSupportGen1;
  } else {
    mPchSataConfig.SpeedSupport     = PchSataSpeedSupportGen3;
  }
#endif
  ///
  /// AzaliaConfig
  ///
  mPchAzaliaConfig.Pme            = PCH_DEVICE_ENABLE;
  mPchAzaliaConfig.ResetWaitTimer = 300;
  mPchAzaliaConfig.AzaliaVCi      = PCH_DEVICE_ENABLE;
  mPchAzaliaConfig.HdmiCodec      = PCH_DEVICE_ENABLE;
  ///
  /// Reserved SMBus Address
  ///
  mPchSmbusConfig.NumRsvdSmbusAddresses = 4;
  mPchSmbusConfig.RsvdSmbusAddressTable = mSmbusRsvdAddresses;

  ///
  /// MiscPm Configuration
  ///
  mPchMiscPmConfig.PchSlpS3MinAssert                    = PchSlpS350ms;
  mPchMiscPmConfig.PchSlpS4MinAssert                    = PchSlpS44s;
  mPchMiscPmConfig.SlpStrchSusUp                        = PCH_DEVICE_DISABLE;
  mPchMiscPmConfig.ConfigureCfioOnSx                    = 0;
  mPchMiscPmConfig.CfioTable                            = (UINT8 *) (UINTN) &mBSW_CfioInitData;
  mPchMiscPmConfig.CfioEntries                          = (UINT8) (sizeof (mBSW_CfioInitData) / sizeof (mBSW_CfioInitData[0]));

  mPchAzaliaConfig.DA = 0;

  ///
  /// Initialize Serial IRQ Config
  ///
  mSerialIrqConfig.SirqEnable       = PCH_DEVICE_ENABLE;
  mSerialIrqConfig.SirqMode         = PchQuietMode;

  ///
  /// Initialize LPSS Config
  ///
  mLpssConfig.Dma1Enabled    = SystemConfiguration.LpssDma1Enabled;
  mLpssConfig.I2C0Enabled    = SystemConfiguration.LpssI2C0Enabled;
  mLpssConfig.I2C1Enabled    = SystemConfiguration.LpssI2C1Enabled;
  mLpssConfig.I2C2Enabled    = SystemConfiguration.LpssI2C2Enabled;
  mLpssConfig.I2C3Enabled    = SystemConfiguration.LpssI2C3Enabled;
  mLpssConfig.I2C4Enabled    = SystemConfiguration.LpssI2C4Enabled;
  mLpssConfig.I2C5Enabled    = SystemConfiguration.LpssI2C5Enabled;
  mLpssConfig.I2C6Enabled    = SystemConfiguration.LpssI2C6Enabled;

  mLpssConfig.Dma0Enabled    = SystemConfiguration.LpssDma0Enabled;
  mLpssConfig.Pwm0Enabled    = SystemConfiguration.LpssPwm0Enabled;
  mLpssConfig.Pwm1Enabled    = SystemConfiguration.LpssPwm1Enabled;
  mLpssConfig.Hsuart0Enabled = SystemConfiguration.LpssHsuart0Enabled;
  mLpssConfig.Hsuart1Enabled = SystemConfiguration.LpssHsuart1Enabled;
  mLpssConfig.Spi1Enabled    = SystemConfiguration.LpssSpi1Enabled;
  mLpssConfig.Spi2Enabled    = SystemConfiguration.LpssSpi2Enabled;
  mLpssConfig.Spi3Enabled    = SystemConfiguration.LpssSpi3Enabled;

  ///
  /// Initialize SCC Config
  ///
  mSccConfig.SdcardEnabled              = SystemConfiguration.SccSdcardEnabled;
  mSccConfig.SdioEnabled                = SystemConfiguration.SccSdioEnabled;
  mSccConfig.eMMCEnabled                = SystemConfiguration.ScceMMCEnabled;
  mSccConfig.EmmcDriverMode             = PchSccEmmcModeAuto;
  mSccConfig.SccEmmcDllTuningRequired   = TRUE;
  mSccConfig.SccEmmcRxDllTuningEnabled  = FALSE;
  mSccConfig.SccEmmcTxDllTuningEnabled  = FALSE;
  mSccConfig.SccEmmcRxDllDataValid      = FALSE;
  mSccConfig.SccEmmcTxDllDataValid      = FALSE;
  mSccConfig.SccEmmcRxStrobeDllValue    = 0;
  mSccConfig.SccEmmcTxDataDllValue      = 0;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxePchPlatformPolicyProtocolGuid,
                  &mPchPolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;

}
