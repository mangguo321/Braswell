/** @file
  Create the device path for the Onboard device.
  The Onboard device information is Misc for subclass type 8, SMBIOS type 10.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "MiscSubclassDriver.h"
#include <IndustryStandard/Pci30.h>
#include <Protocol/PciRootBridgeIo.h>

EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *gPciRootBridgeIo;

/**
  Get the enabling/disabling setting through PciBridgeIo and return that.

  @param[out]   OnBoardDeviceType

**/
VOID
GetOnBoardDeviceType (
  UINT8         *OnBoardDeviceType
)
{
  EFI_STATUS    Status;
  UINT8         Value8;
  UINT32        Value32;
  UINT64        PciAddress;

  Value8        = 0;
  Value32       = 0;
  PciAddress    = 0;

  Status = gBS->LocateProtocol (
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &gPciRootBridgeIo
                  );
  if (EFI_ERROR(Status)) {
    //
    // Can't access PciRootBridgeIoProtocol, don't change the default setting.
    //
    DEBUG ((EFI_D_ERROR, "(SMBIOS TABLE Type10) Don't change the default setting for the OnBoardDeviceType.\n"));
    return;
  }

  //
  // Read PCI registers to set enabling/disabling onboard device
  //
  switch (*OnBoardDeviceType) {
    case OnBoardDeviceTypeVideo:
      //
      // Read command reg of device 0, function 0, reg 0x52 for onboard device enabled
      //
      PciAddress = EFI_PCI_ADDRESS (0, 0, 0, 0x52);
      gPciRootBridgeIo->Pci.Read (
                          gPciRootBridgeIo,
                          EfiPciWidthUint8,
                          PciAddress,
                          1,
                          &Value8
                          );
      if (Value8 & 0x02) {
        *OnBoardDeviceType &= 0x7F;
        DEBUG ((EFI_D_ERROR, "(SMBIOS TABLE Type10) OnBoardDevice Video is enabled.\n"));
      } else {
        *OnBoardDeviceType |= 0x80;
      }
      break;

    case OnBoardDeviceTypeEthernet:
      *OnBoardDeviceType &= 0x7F;
      break;

    case OnBoardDeviceTypeSound:
      //
      // Read command reg of HD Audio, reg 0x04 onboard device enabled
      //
      PciAddress = EFI_PCI_ADDRESS (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_AZALIA,
                     PCI_FUNCTION_NUMBER_PCH_AZALIA,
                     PCI_COMMAND_OFFSET
                     );
      gPciRootBridgeIo->Pci.Read (
                          gPciRootBridgeIo,
                          EfiPciWidthUint32,
                          PciAddress,
                          1,
                          &Value32
                          );
      if (Value32 != 0xffffffff) {
        if ((Value32 & 0x02) || (Value32 & 0x01)) {
          *OnBoardDeviceType |= 0x80;
          DEBUG ((EFI_D_ERROR, "(SMBIOS TABLE Type10) OnBoardDevice Sound is enabled.\n"));
        } else {
          *OnBoardDeviceType &= 0x7F;
        }
      }
      break;
  }

}
/**
  This function makes boot time changes to the contents of the
  MiscOnboardDevice (Type 10).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscOnBoardDevice)
{
  CHAR8                         *OptionalStrStart;
  UINTN                         StringNumber;
  UINTN                         StringLength;
  EFI_STATUS                    Status;
  EFI_SMBIOS_HANDLE             SmbiosHandle;
  SMBIOS_TABLE_TYPE10           *SmbiosRecord;
  SMBIOS_TABLE_TYPE10           *ForType10InputData;
  SMBIOS_TABLE_TYPE10_STRINGS   SmbiosTableType10Strings;
  CHAR16                        *StrBufferStart;
  CHAR16                        *StrBufferPtr;

  //
  // First check for invalid parameters.
  //
  if (RecordData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "(MiscOnBoardDevice) Entry.\n"));
  ZeroMem (&SmbiosTableType10Strings, sizeof(SMBIOS_TABLE_TYPE10_STRINGS));
  StringNumber        = 0;
  StringLength        = 0;
  ForType10InputData  = (SMBIOS_TABLE_TYPE10 *) RecordData;
  StrBufferStart      = AllocateZeroPool (1 * SMBIOS_STRING_MAX_LENGTH * sizeof(CHAR16));
  StrBufferPtr        = StrBufferStart;

  //
  // Initialize SMBIOS Tables Type10 strings
  //
  switch (ForType10InputData->Device[0].DeviceType & 0x7F) {
    case OnBoardDeviceTypeVideo:
      SmbiosStrInit (&SmbiosTableType10Strings.DescriptionString, SMBIOS_MISC_ONBOARD_DEVICE_VIDEO, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case OnBoardDeviceTypeEthernet:
      SmbiosStrInit (&SmbiosTableType10Strings.DescriptionString, SMBIOS_MISC_ONBOARD_DEVICE_ETHERNET, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    case OnBoardDeviceTypeSound:
      SmbiosStrInit (&SmbiosTableType10Strings.DescriptionString, SMBIOS_MISC_ONBOARD_DEVICE_SOUND, &StrBufferPtr, &StringNumber, &StringLength);
      break;
    default:
      break;
  }

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE10) + StringLength + StringNumber + 1);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem(SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE10) + StringLength + StringNumber + 1);
  CopyMem(SmbiosRecord, RecordData, sizeof(SMBIOS_TABLE_TYPE10));
  DEBUG((EFI_D_ERROR, "(Type10) SmbiosRecord->Hdr.Type is %d \n", SmbiosRecord->Hdr.Type));
  DEBUG((EFI_D_ERROR, "(Type10) SmbiosRecord->Hdr.Length is %d \n", SmbiosRecord->Hdr.Length));

  //
  // Get device status
  //
  GetOnBoardDeviceType (&SmbiosRecord->Device[0].DeviceType);

  //
  // Update SMBIOS Tables Type10 strings
  //
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  SmbiosStringsUpdate ((CHAR16 **) &SmbiosTableType10Strings, OptionalStrStart, StringNumber);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                     );
  FreePool(SmbiosRecord);
  FreePool(StrBufferStart);
  return Status;
}
