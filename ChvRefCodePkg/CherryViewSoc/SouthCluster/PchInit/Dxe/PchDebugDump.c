/** @file
  Dump whole DXE_PCH_PLATFORM_POLICY_PROTOCOL and serial out.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchInit.h"

/**
  Dump whole DXE_PCH_PLATFORM_POLICY_PROTOCOL and serial out.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance

**/
VOID
PchDumpPlatformProtocol (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  )
{
#ifdef EFI_DEBUG
  UINT8 i;

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH Dump platform protocol Start -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH PLATFORM POLICY Revision= %x\n", PchPlatformPolicy->Revision));
  DEBUG ((EFI_D_INFO, " PCH PLATFORM POLICY BusNumber= %x\n", PchPlatformPolicy->BusNumber));
  DEBUG ((EFI_D_INFO, " PCH PLATFORM POLICY PnpSettings= %x\n", PchPlatformPolicy->PnpSettings));
  DEBUG ((EFI_D_INFO, " PCH PLATFORM POLICY S0ixSupport= %x\n", PchPlatformPolicy->S0ixSupport));
  DEBUG ((EFI_D_INFO, " PCH PLATFORM POLICY AcpiHwRed= %x\n", PchPlatformPolicy->AcpiHwRed));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_DEVICE_ENABLE -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE Azalia= %x\n", PchPlatformPolicy->DeviceEnabling->Azalia));
#ifdef SATA_SUPPORT
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE Sata= %x\n", PchPlatformPolicy->DeviceEnabling->Sata));
#endif
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE Smbus= %x\n", PchPlatformPolicy->DeviceEnabling->Smbus));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE LpeEnabled= %x\n", PchPlatformPolicy->DeviceEnabling->LpeEnabled));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE IshEnabled= %x\n", PchPlatformPolicy->DeviceEnabling->IshEnabled));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE PlatformClock[0]= %x\n", PchPlatformPolicy->DeviceEnabling->PlatformClock[0]));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE PlatformClock[1]= %x\n", PchPlatformPolicy->DeviceEnabling->PlatformClock[1]));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE PlatformClock[2]= %x\n", PchPlatformPolicy->DeviceEnabling->PlatformClock[2]));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE PlatformClock[3]= %x\n", PchPlatformPolicy->DeviceEnabling->PlatformClock[3]));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE PlatformClock[4]= %x\n", PchPlatformPolicy->DeviceEnabling->PlatformClock[4]));
  DEBUG ((EFI_D_INFO, " PCH_DEVICE_ENABLE PlatformClock[5]= %x\n", PchPlatformPolicy->DeviceEnabling->PlatformClock[5]));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_USB_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG SsicEnable = %x\n", PchPlatformPolicy->UsbConfig->SsicEnable));
  for (i = 0; i < PCH_USB_MAX_PHYSICAL_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG Usb20PortSettings[%d] Enabled= %x\n", i, PchPlatformPolicy->UsbConfig->Usb20PortSettings[i].Enable));
  }
  for (i = 0; i < PCH_XHCI_MAX_USB3_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG Usb30PortSettings[%d] Enabled= %x\n", i, PchPlatformPolicy->UsbConfig->Usb30PortSettings[i].Enable));
  }
  for (i = 0; i < PCH_HSIC_MAX_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG HsicPortSettings[%d] Enabled= %x\n", i, PchPlatformPolicy->UsbConfig->HsicPortSettings[i].Enable));
  }
  for (i = 0; i < PCH_SSIC_MAX_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG SsicPortSettings[%d] Enabled= %x\n", i, PchPlatformPolicy->UsbConfig->SsicPortSettings[i].Enable));
  }
  DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG Usb30Settings.Mode= %x\n", PchPlatformPolicy->UsbConfig->Usb30Settings.Mode));
  DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG UsbOtgSettings.Enable= %x\n", PchPlatformPolicy->UsbConfig->UsbOtgSettings.Enable));
  for (i = 0; i < PCH_USB_MAX_PHYSICAL_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG Usb20OverCurrentPins[%d]= OC%x\n", i, PchPlatformPolicy->UsbConfig->Usb20OverCurrentPins[i]));
  }
  for (i = 0; i < PCH_XHCI_MAX_USB3_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_USB_CONFIG Usb30OverCurrentPins[%d]= OC%x\n", i, PchPlatformPolicy->UsbConfig->Usb30OverCurrentPins[i]));
  }

#ifdef PCIESC_SUPPORT
  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_PCI_EXPRESS_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG TempRootPortBusNumMin= %x\n", PchPlatformPolicy->PciExpressConfig->TempRootPortBusNumMin));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG TempRootPortBusNumMax= %x\n", PchPlatformPolicy->PciExpressConfig->TempRootPortBusNumMax));
  for (i = 0; i < PCH_PCIE_MAX_ROOT_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] Enabled= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].Enable));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] Hide= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].Hide));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] SlotImplemented= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].SlotImplemented));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] HotPlug= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].HotPlug));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] PmSci= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].PmSci));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] ExtSync= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].ExtSync));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] UnsupportedRequestReport= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].UnsupportedRequestReport));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] FatalErrorReport= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].FatalErrorReport));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] NoFatalErrorReport= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].NoFatalErrorReport));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] CorrectableErrorReport= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].CorrectableErrorReport));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] PmeInterrupt= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].PmeInterrupt));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] SystemErrorOnFatalError= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].SystemErrorOnFatalError));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] SystemErrorOnNonFatalError= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].SystemErrorOnNonFatalError));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] SystemErrorOnCorrectableError= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].SystemErrorOnCorrectableError));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] AdvancedErrorReporting= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].AdvancedErrorReporting));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] TransmitterHalfSwing= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].TransmitterHalfSwing));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] FunctionNumber= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].FunctionNumber));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] PhysicalSlotNumber= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].PhysicalSlotNumber));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] CompletionTimeout= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].CompletionTimeout));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] Aspm= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].Aspm));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] L1Substates= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].L1Substates));
    DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPort[%d] TxEqDeemphSelection= %x\n", i, PchPlatformPolicy->PciExpressConfig->RootPort[i].TxEqdeEmphSelection));
  }
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG NumOfDevAspmOverride= %x\n", PchPlatformPolicy->PciExpressConfig->NumOfDevAspmOverride));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG DevAspmOverride VendorId= %x\n", PchPlatformPolicy->PciExpressConfig->DevAspmOverride->VendorId));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG DevAspmOverride DeviceId= %x\n", PchPlatformPolicy->PciExpressConfig->DevAspmOverride->DeviceId));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG DevAspmOverride RevId= %x\n", PchPlatformPolicy->PciExpressConfig->DevAspmOverride->RevId));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG DevAspmOverride BaseClassCode= %x\n", PchPlatformPolicy->PciExpressConfig->DevAspmOverride->BaseClassCode));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG DevAspmOverride SubClassCode= %x\n", PchPlatformPolicy->PciExpressConfig->DevAspmOverride->SubClassCode));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG DevAspmOverride EndPointAspm= %x\n", PchPlatformPolicy->PciExpressConfig->DevAspmOverride->EndPointAspm));
  DEBUG ((EFI_D_INFO, " PCH_PCI_EXPRESS_CONFIG RootPortClockGating= %x\n", PchPlatformPolicy->PciExpressConfig->RootPortClockGating));
#endif

  DEBUG ((EFI_D_INFO, "\n"));
#ifdef SATA_SUPPORT
  DEBUG ((EFI_D_INFO, "------------------------ PCH_SATA_CONFIG -----------------\n"));
  for (i = 0; i < PCH_AHCI_MAX_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG PortSettings[%d] Enabled= %x\n", i, PchPlatformPolicy->SataConfig->PortSettings[i].Enable));
    DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG PortSettings[%d] HotPlug= %x\n", i, PchPlatformPolicy->SataConfig->PortSettings[i].HotPlug));
    DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG PortSettings[%d] MechSw= %x\n", i, PchPlatformPolicy->SataConfig->PortSettings[i].MechSw));
    DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG PortSettings[%d] External= %x\n", i, PchPlatformPolicy->SataConfig->PortSettings[i].External));
    DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG PortSettings[%d] DevSlp= %x\n", i, PchPlatformPolicy->SataConfig->PortSettings[i].DevSlp));
  }
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG RaidAlternateId= %x\n", PchPlatformPolicy->SataConfig->RaidAlternateId));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG Raid0= %x\n", PchPlatformPolicy->SataConfig->Raid0));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG Raid1= %x\n", PchPlatformPolicy->SataConfig->Raid1));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG Raid10= %x\n", PchPlatformPolicy->SataConfig->Raid10));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG Raid5= %x\n", PchPlatformPolicy->SataConfig->Raid5));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG Irrt= %x\n", PchPlatformPolicy->SataConfig->Irrt));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG OromUiBanner= %x\n", PchPlatformPolicy->SataConfig->OromUiBanner));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG HddUnlock= %x\n", PchPlatformPolicy->SataConfig->HddUnlock));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG LedLocate= %x\n", PchPlatformPolicy->SataConfig->LedLocate));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG IrrtOnly= %x\n", PchPlatformPolicy->SataConfig->IrrtOnly));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG TestMode= %x\n", PchPlatformPolicy->SataConfig->TestMode));
  DEBUG ((EFI_D_INFO, " PCH_SATA_CONFIG SpeedSupport= %x\n", PchPlatformPolicy->SataConfig->SpeedSupport));
#endif
  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_AZALIA_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG Pme= %x\n", PchPlatformPolicy->AzaliaConfig->Pme));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG DS= %x\n", PchPlatformPolicy->AzaliaConfig->DS));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG DA= %x\n", PchPlatformPolicy->AzaliaConfig->DA));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTableNum= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTableNum));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable Header VendorDeviceId= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableHeader.VendorDeviceId));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable Header SubSystemId= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableHeader.SubSystemId));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable Header RevisionId= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableHeader.RevisionId));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable Header FrontPanelSupport= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableHeader.FrontPanelSupport));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable Header NumberOfRearJacks= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableHeader.NumberOfRearJacks));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable Header NumberOfFrontJacks= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableHeader.NumberOfFrontJacks));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG AzaliaVerbTable VerbTableData= %x\n", PchPlatformPolicy->AzaliaConfig->AzaliaVerbTable->VerbTableData));
  DEBUG ((EFI_D_INFO, " PCH_AZALIA_CONFIG ResetWaitTimer= %x\n", PchPlatformPolicy->AzaliaConfig->ResetWaitTimer));
  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_SMBUS_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_SMBUS_CONFIG NumRsvdSmbusAddresses= %x\n", PchPlatformPolicy->SmbusConfig->NumRsvdSmbusAddresses));
  DEBUG ((EFI_D_INFO, " PCH_SMBUS_CONFIG RsvdSmbusAddressTable= %x\n", PchPlatformPolicy->SmbusConfig->RsvdSmbusAddressTable));
  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_MISC_PM_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_MISC_PM_CONFIG WakeConfig PmeB0S5Dis= %x\n", PchPlatformPolicy->MiscPmConfig->WakeConfig.PmeB0S5Dis));
  DEBUG ((EFI_D_INFO, " PCH_MISC_PM_CONFIG PchSlpS3MinAssert= %x\n", PchPlatformPolicy->MiscPmConfig->PchSlpS3MinAssert));
  DEBUG ((EFI_D_INFO, " PCH_MISC_PM_CONFIG PchSlpS4MinAssert= %x\n", PchPlatformPolicy->MiscPmConfig->PchSlpS4MinAssert));
  DEBUG ((EFI_D_INFO, " PCH_MISC_PM_CONFIG SlpStrchSusUp= %x\n", PchPlatformPolicy->MiscPmConfig->SlpStrchSusUp));
  DEBUG ((EFI_D_INFO, " PCH_MISC_PM_CONFIG CfioTable location = %x\n", PchPlatformPolicy->MiscPmConfig->CfioTable));
  DEBUG ((EFI_D_INFO, " PCH_MISC_PM_CONFIG Number of CFIO entries = %x\n", PchPlatformPolicy->MiscPmConfig->CfioEntries));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_DEFAULT_SVID_SID -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_DEFAULT_SVID_SID SubSystemVendorId= %x\n", PchPlatformPolicy->DefaultSvidSid->SubSystemVendorId));
  DEBUG ((EFI_D_INFO, " PCH_DEFAULT_SVID_SID SubSystemId= %x\n", PchPlatformPolicy->DefaultSvidSid->SubSystemId));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_LOCK_DOWN_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_LOCK_DOWN_CONFIG GlobalSmi= %x\n", PchPlatformPolicy->LockDownConfig->GlobalSmi));
  DEBUG ((EFI_D_INFO, " PCH_LOCK_DOWN_CONFIG BiosInterface= %x\n", PchPlatformPolicy->LockDownConfig->BiosInterface));
  DEBUG ((EFI_D_INFO, " PCH_LOCK_DOWN_CONFIG RtcLock= %x\n", PchPlatformPolicy->LockDownConfig->RtcLock));
  DEBUG ((EFI_D_INFO, " PCH_LOCK_DOWN_CONFIG BiosLock= %x\n", PchPlatformPolicy->LockDownConfig->BiosLock));
  DEBUG ((EFI_D_INFO, " PCH_LOCK_DOWN_CONFIG PchBiosLockSwSmiNumber= %x\n", PchPlatformPolicy->LockDownConfig->PchBiosLockSwSmiNumber));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_LPC_SIRQ_CONFIG -----------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_LPC_SIRQ_CONFIG SirqEnable= %x\n", PchPlatformPolicy->SerialIrqConfig->SirqEnable));
  DEBUG ((EFI_D_INFO, " PCH_LPC_SIRQ_CONFIG SirqMode= %x\n", PchPlatformPolicy->SerialIrqConfig->SirqMode));
#ifdef PCIESC_SUPPORT
  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH_PWR_OPT_CONFIG -----------------\n"));
  for (i = 0; i < PCH_PCIE_MAX_ROOT_PORTS; i++) {
    DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG RootPort[%d] LtrEnable= %x\n", i, PchPlatformPolicy->PwrOptConfig->PchPwrOptPcie[i].LtrEnable));
    DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG RootPort[%d] ObffEnable= %x\n", i, PchPlatformPolicy->PwrOptConfig->PchPwrOptPcie[i].ObffEnable));
  }
  DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG NumOfDevLtrOverride= %x\n", PchPlatformPolicy->PwrOptConfig->NumOfDevLtrOverride));
  DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG DevLtrOverride VendorId= %x\n", PchPlatformPolicy->PwrOptConfig->DevLtrOverride->VendorId));
  DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG DevLtrOverride DeviceId= %x\n", PchPlatformPolicy->PwrOptConfig->DevLtrOverride->DeviceId));
  DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG DevLtrOverride RevId= %x\n", PchPlatformPolicy->PwrOptConfig->DevLtrOverride->RevId));
  DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG DevLtrOverride SnoopLatency= %x\n", PchPlatformPolicy->PwrOptConfig->DevLtrOverride->SnoopLatency));
  DEBUG ((EFI_D_INFO, " PCH_PWR_OPT_CONFIG DevLtrOverride NonSnoopLatency= %x\n", PchPlatformPolicy->PwrOptConfig->DevLtrOverride->NonSnoopLatency));
#endif
  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------- PCH_LPSS_CONFIG --------------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Dma1Enabled= %x\n", PchPlatformPolicy->LpssConfig->Dma1Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C0Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C0Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C1Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C1Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C2Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C2Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C3Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C3Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C4Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C4Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C5Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C5Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG I2C6Enabled= %x\n", PchPlatformPolicy->LpssConfig->I2C6Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Dma0Enabled= %x\n", PchPlatformPolicy->LpssConfig->Dma0Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Pwm0Enabled= %x\n", PchPlatformPolicy->LpssConfig->Pwm0Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Pwm1Enabled= %x\n", PchPlatformPolicy->LpssConfig->Pwm1Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Hsuart0Enabled= %x\n", PchPlatformPolicy->LpssConfig->Hsuart0Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Hsuart1Enabled= %x\n", PchPlatformPolicy->LpssConfig->Hsuart1Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Spi1Enabled= %x\n", PchPlatformPolicy->LpssConfig->Spi1Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Spi2Enabled= %x\n", PchPlatformPolicy->LpssConfig->Spi2Enabled));
  DEBUG ((EFI_D_INFO, " PCH_LPSS_CONFIG Spi3Enabled= %x\n", PchPlatformPolicy->LpssConfig->Spi3Enabled));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------- PCH_SCC_CONFIG --------------------\n"));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG eMMCEnabled= %x\n", PchPlatformPolicy->SccConfig->eMMCEnabled));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SdioEnabled= %x\n", PchPlatformPolicy->SccConfig->SdioEnabled));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SdcardEnabled= %x\n", PchPlatformPolicy->SccConfig->SdcardEnabled));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG EmmcDriverMode= %x\n", PchPlatformPolicy->SccConfig->EmmcDriverMode));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcDllTuningRequired= %x\n", PchPlatformPolicy->SccConfig->SccEmmcDllTuningRequired));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcRxDllTuningEnabled= %x\n", PchPlatformPolicy->SccConfig->SccEmmcRxDllTuningEnabled));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcTxDllTuningEnabled= %x\n", PchPlatformPolicy->SccConfig->SccEmmcTxDllTuningEnabled));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcRxDllDataValid= %x\n", PchPlatformPolicy->SccConfig->SccEmmcRxDllDataValid));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcTxDllDataValid= %x\n", PchPlatformPolicy->SccConfig->SccEmmcTxDllDataValid));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcRxStrobeDllValue= %x\n", PchPlatformPolicy->SccConfig->SccEmmcRxStrobeDllValue));
  DEBUG ((EFI_D_INFO, " PCH_SCC_CONFIG SccEmmcTxDataDllValue= %x\n", PchPlatformPolicy->SccConfig->SccEmmcTxDataDllValue));

  DEBUG ((EFI_D_INFO, "\n"));
  DEBUG ((EFI_D_INFO, "------------------------ PCH Dump platform protocol End -----------------\n"));
  DEBUG ((EFI_D_INFO, "\n"));
#endif
}
