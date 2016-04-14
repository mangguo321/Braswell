/** @file
  This driver will determine memory configuration information from the chipset
  and memory and create SMBIOS memory structures appropriately.

  @par Revision Reference:
  SMBIOS Specification version 2.8.0 from DMTF: http://www.dmtf.org/standards/smbios
  Intel Framework Specifications, all available at: http://www.intel.com/technology/framework/spec.htm
   - Data Hub Specification
   - SMBUS Host Controller Protocol Specification
   - Human Interface Infrastructure Specification
  Unified Extensible Firmware Interface (UEFI) Specifications: http://www.uefi.org/specs/

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SmbiosMemory.h"
#include <Library/HobLib.h>

extern UINT8                    SmBiosMemoryStrings[];
MEM_INFO_PROTOCOL               *mMemInfoHob = NULL;
EFI_SMBIOS_PROTOCOL             *mSmbios = NULL;

EFI_GUID  gEfiMemorySubClassDriverGuid = EFI_MEMORY_SUBCLASS_DRIVER_GUID;
extern EFI_GUID gFspSmbiosMemoryInfoHobGuid;

EFI_STRING
EFIAPI
SmbiosMiscGetString (
  IN EFI_STRING_ID   StringId
  )
{
  EFI_STATUS  Status;
  UINTN       StringSize;
  CHAR16      TempString;
  EFI_STRING  String;
  String      = NULL;

  //
  // Retrieve the size of the string in the string package for the BestLanguage
  //
  StringSize = 0;
  Status = mHiiString->GetString (
                         mHiiString,
                         "en-US",
                         StringPackHandle,
                         StringId,
                         &TempString,
                         &StringSize,
                         NULL
                         );
  //
  // If GetString() returns EFI_SUCCESS for a zero size,
  // then there are no supported languages registered for HiiHandle.  If GetString()
  // returns an error other than EFI_BUFFER_TOO_SMALL, then HiiHandle is not present
  // in the HII Database
  //
  if (Status != EFI_BUFFER_TOO_SMALL) {
    goto Error;
  }

  //
  // Allocate a buffer for the return string
  //
  String = AllocateZeroPool (StringSize);
  if (String == NULL) {
    goto Error;
  }

  //
  // Retrieve the string from the string package
  //
  Status = mHiiString->GetString (
                         mHiiString,
                         "en-US",
                         StringPackHandle,
                         StringId,
                         String,
                         &StringSize,
                         NULL
                         );
  if (EFI_ERROR (Status)) {
    //
    // Free the buffer and return NULL if the supported languages can not be retrieved.
    //
    FreePool (String);
    String = NULL;
  }
Error:

  return String;
}

/**
  Initialize the module's protocols

  @retval EFI_SUCCESS          - if the protocols are successfully found.
  @retval EFI_NOT_FOUND        - if a protocol could not be located.

**/
EFI_STATUS
InitializeProtocols (
  EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**) &mSmbios);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Retrieve the pointer to the UEFI HII String Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiStringProtocolGuid,
                  NULL,
                  (VOID **) &mHiiString
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Create driver handle used by HII database
  //
  StringPackHandle = HiiAddPackages (&gEfiMemorySubClassDriverGuid,
                                     ImageHandle,
                                     SmBiosMemoryStrings,
                                     NULL);

  if (StringPackHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ASSERT (StringPackHandle != NULL);

  return Status;
}

/**
  This driver will determine memory configuration information from the chipset
  and memory and report the memory configuration info to the DataHub.

  @param[in]  ImageHandle     Handle for the image of this driver
  @param[in]  SystemTable     Pointer to the EFI System Table

  @retval   EFI_SUCCESS       if the data is successfully reported
  @retval   EFI_NOT_FOUND     if the HOB list could not be located.

**/
EFI_STATUS
SmbiosMemoryEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS      Status;
  VOID            *HobStart;

  HobStart = GetFirstGuidHob (&gFspSmbiosMemoryInfoHobGuid);

  if (HobStart == NULL) {
    return EFI_NOT_FOUND;
  }

  CopyMem (&gFspSmbiosMemoryInfo, (GET_GUID_HOB_DATA (HobStart)), sizeof(FSP_SMBIOS_MEMORY_INFO));
  
  Status = InitializeProtocols(ImageHandle);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallSmbiosType16();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallSmbiosType17();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallSmbiosType19();
  return Status;
}
