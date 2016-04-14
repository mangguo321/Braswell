/** @file
  Entry and initialization module for platform browser.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformSetupDxe.h"
#include "Guid/SetupVariable.h"
#include <Protocol/FormBrowserEx2.h>

#define EFI_CALLBACK_INFO_SIGNATURE SIGNATURE_32 ('C', 'l', 'b', 'k')
#define EFI_CALLBACK_INFO_FROM_THIS(a)  CR (a, EFI_CALLBACK_INFO, ConfigAccess, EFI_CALLBACK_INFO_SIGNATURE)

typedef struct {
  UINTN                           Signature;
  EFI_HANDLE                      DriverHandle;
  EFI_HII_HANDLE                  RegisteredHandle;
  SYSTEM_CONFIGURATION            FakeNvData;
  SYSTEM_CONFIGURATION            BackupNvData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRouting;
  EFI_HII_CONFIG_ACCESS_PROTOCOL  ConfigAccess;
} EFI_CALLBACK_INFO;

#pragma pack(1)

//
// HII specific Vendor Device Path definition.
//
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

//
// uni string and Vfr Binary data.
//
extern UINT8  VfrBin[];
extern UINT8  PlatformSetupDxeStrings[];

//
// module global data
//
#define EFI_NORMAL_SETUP_GUID \
  { 0xec87d643, 0xeba4, 0x4bb5, 0xa1, 0xe5, 0x3f, 0x3e, 0x36, 0xb2, 0xd, 0xa9 }

EFI_GUID                     mNormalSetupGuid = EFI_NORMAL_SETUP_GUID;
EFI_GUID                     mSystemConfigGuid = SYSTEM_CONFIGURATION_GUID;
CHAR16                       mVariableName[] = L"Setup";
CHAR16                       mSetupName[] = L"Setup";
EFI_CALLBACK_INFO            *mCallbackInfo;
BOOLEAN                      GlobalReset=FALSE;

HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_CALLER_ID_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};


/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param[in]  This                Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Request             A null-terminated Unicode string in <ConfigRequest> format.
  @param[out] Progress            On return, points to a character in the Request string.
                                  Points to the string's null terminator if request was successful.
                                  Points to the most recent '&' before the first failing name/value
                                  pair (or the beginning of the string if the failure is in the
                                  first name/value pair) if the request was not successful.
  @param[out]  Results            A null-terminated Unicode string in <ConfigAltResp> format which
                                  has all values filled in for the names in the Request string.
                                  String to be allocated by the called function.

  @retval  EFI_SUCCESS            The Results is filled with the requested values.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval  EFI_INVALID_PARAMETER  Request is NULL, illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
SystemConfigExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                       Status;
  EFI_CALLBACK_INFO                *Private;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_STRING                       ConfigRequestHdr;
  EFI_STRING                       ConfigRequest;
  BOOLEAN                          AllocatedRequest;
  UINTN                            Size;
  UINTN                            BufferSize;
  VOID                             *SystemConfigPtr;

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Request;
  if ((Request != NULL) && !HiiIsConfigHdrMatch (Request, &mSystemConfigGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Size             = 0;
  AllocatedRequest = FALSE;

  Private          = EFI_CALLBACK_INFO_FROM_THIS (This);

  SetupInfo();

  HiiConfigRouting = Private->HiiConfigRouting;
  ConfigRequest = Request;
  if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&mSystemConfigGuid, mVariableName, Private->DriverHandle);
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);
    AllocatedRequest = TRUE;
    BufferSize = sizeof (SYSTEM_CONFIGURATION);
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
  }
  SystemConfigPtr = PcdGetPtr (PcdSystemConfiguration);

  if (SystemConfigPtr == NULL) {
    ZeroMem(&Private->FakeNvData, sizeof(SYSTEM_CONFIGURATION));
    ZeroMem(&Private->BackupNvData, sizeof(SYSTEM_CONFIGURATION));
  } else {
    CopyMem(&Private->FakeNvData, SystemConfigPtr, sizeof(SYSTEM_CONFIGURATION));
    CopyMem(&Private->BackupNvData, SystemConfigPtr, sizeof(SYSTEM_CONFIGURATION));
    FreePool(SystemConfigPtr);
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = HiiConfigRouting->BlockToConfig (
                               HiiConfigRouting,
                               ConfigRequest,
                               (UINT8 *) &Private->FakeNvData,
                               sizeof (SYSTEM_CONFIGURATION),
                               Results,
                               Progress
                               );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param[in]  This                Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Configuration       A null-terminated Unicode string in <ConfigRequest> format.
  @param[out] Progress            A pointer to a string filled in with the offset of the most
                                  recent '&' before the first failing name/value pair (or the
                                  beginning of the string if the failure is in the first
                                  name/value pair) or the terminating NULL if all was successful.

  @retval EFI_SUCCESS             The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER   Configuration is NULL.
  @retval EFI_NOT_FOUND           Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
SystemConfigRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_CALLBACK_INFO                         *Private;
  SYSTEM_CONFIGURATION                      *FakeNvData;
  UINTN                                     Size;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Progress = Configuration;

  if (!HiiIsConfigHdrMatch (Configuration, &mSystemConfigGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  *Progress = Configuration + StrLen (Configuration);
  Private    = EFI_CALLBACK_INFO_FROM_THIS (This);
  FakeNvData = &Private->FakeNvData;
  if (!HiiGetBrowserData (&mSystemConfigGuid, mVariableName, sizeof (SYSTEM_CONFIGURATION), (UINT8 *) FakeNvData)) {
    //
    // FakeNvData can't be got from SetupBrowser, which doesn't need to be set.
    //
    return EFI_SUCCESS;
  }

  Size = sizeof (SYSTEM_CONFIGURATION);
  PcdSetPtr (PcdSystemConfiguration, &Size, &Private->FakeNvData);

  return EFI_SUCCESS;
}

/**
  This is the function that is called to provide results data to the driver.  This data
  consists of a unique key which is used to identify what data is either being passed back
  or being asked for.

  @param[in]  This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action         A null-terminated Unicode string in <ConfigRequest> format.
  @param[in]  KeyValue       A unique Goto OpCode callback value which record user's selection.
                             0x100 <= KeyValue <0x500 : user select a controller item in the first page;
                             KeyValue == 0x1234       : user select 'Refresh' in first page, or user select 'Go to Previous Menu' in second page
                             KeyValue == 0x1235       : user select 'Pci device filter' in first page
                             KeyValue == 0x1500       : user select 'order ... priority' item in second page
                             KeyValue == 0x1800       : user select 'commint changes' in third page
                             KeyValue == 0x2000       : user select 'Go to Previous Menu' in third page
  @param[in]  Type           The type of value for the question.
  @param[in]  Value          A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest  On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS        Always returned.

**/
EFI_STATUS
EFIAPI
SystemConfigCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        KeyValue,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  if ((This == NULL) || (Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

/**
  The driver Entry Point. The funciton will export a disk device class formset and
  its callback function to hii database.

  @param[in]  ImageHandle    The firmware allocated handle for the EFI image.
  @param[in]  SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS        The entry point is executed successfully.
  @retval other              Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
PlatformSetupDxeInit (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                          Status;
  EFI_FORM_BROWSER2_PROTOCOL          *FormBrowser2;
  EFI_FORM_BROWSER_EXTENSION_PROTOCOL *FormBrowserEx;
  EFI_INPUT_KEY                       HotKey;
  EFI_STRING                          HelpString;
  
  Status = gBS->LocateProtocol (&gEfiFormBrowserExProtocolGuid, NULL, (VOID **) &FormBrowserEx);
  ASSERT_EFI_ERROR (Status);

  //
  // There should only be one Form Configuration protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiFormBrowser2ProtocolGuid,
                  NULL,
                  (VOID **) &FormBrowser2
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mCallbackInfo = AllocateZeroPool (sizeof (EFI_CALLBACK_INFO));
  if (mCallbackInfo == NULL) {
    return EFI_BAD_BUFFER_SIZE;
  }

  mCallbackInfo->Signature = EFI_CALLBACK_INFO_SIGNATURE;
  mCallbackInfo->ConfigAccess.ExtractConfig = SystemConfigExtractConfig;
  mCallbackInfo->ConfigAccess.RouteConfig   = SystemConfigRouteConfig;
  mCallbackInfo->ConfigAccess.Callback      = SystemConfigCallback;

  //
  // Install Device Path Protocol and Config Access protocol to driver handle
  // Install Platform Driver Override Protocol to driver handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mCallbackInfo->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mCallbackInfo->ConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto Finish;
  }

  //
  // Publish our HII data
  //
  mCallbackInfo->RegisteredHandle = HiiAddPackages (
                                      &mSystemConfigGuid,
                                      mCallbackInfo->DriverHandle,
                                      VfrBin,
                                      PlatformSetupDxeStrings,
                                      NULL
                                      );
  if (mCallbackInfo->RegisteredHandle == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Finish;
  }

  mHiiHandle = mCallbackInfo->RegisteredHandle;

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **) &mCallbackInfo->HiiConfigRouting
                  );
  if (EFI_ERROR (Status)) {
    goto Finish;
  }
  
  Status = FormBrowserEx->SetScope (SystemLevel);
  //
  // First unregister the default hot key F9 and F10.
  //
  HotKey.UnicodeChar = CHAR_NULL;
  HotKey.ScanCode    = SCAN_F9;
  Status = FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_UNREGISTER, 0, NULL);

  HotKey.ScanCode    = SCAN_F10;
  Status = FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_UNREGISTER, 0, NULL);
  
  //
  // Register the global HotKey F8, F9 and F10.
  //
  HotKey.ScanCode    = SCAN_F10;
  HelpString = HiiGetString (mHiiHandle, STRING_TOKEN (STR_COMMIT_CHANGE_F10), NULL);
  Status = FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_SUBMIT, 0, HelpString);
  FreePool (HelpString);

  HotKey.ScanCode    = SCAN_F9;
  HelpString = HiiGetString (mHiiHandle, STRING_TOKEN (STR_LOAD_DEFAULT_F9), NULL);
  Status = FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_DEFAULT, EFI_HII_DEFAULT_CLASS_STANDARD, HelpString);
  FreePool (HelpString);

  HotKey.ScanCode    = SCAN_F8;
  HelpString = HiiGetString (mHiiHandle, STRING_TOKEN (STR_DISCARD_CHANGE_F8), NULL);
  Status = FormBrowserEx->RegisterHotKey (&HotKey, BROWSER_ACTION_DISCARD, 0, HelpString);
  FreePool (HelpString);

  //
  // Clear all the globle variable
  //
  return EFI_SUCCESS;

Finish:
  if (mCallbackInfo->DriverHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mCallbackInfo->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiVendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mCallbackInfo->ConfigAccess,
           NULL
           );
  }

  if (mCallbackInfo->RegisteredHandle != NULL) {
    HiiRemovePackages (mCallbackInfo->RegisteredHandle);
  }

  if (mCallbackInfo != NULL) {
    FreePool (mCallbackInfo);
  }

  return Status;
}

/**
  Unload its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.

**/
EFI_STATUS
EFIAPI
PlatformSetupDxeUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  if (mCallbackInfo != NULL) {
    if (mCallbackInfo->DriverHandle != NULL) {
      gBS->UninstallMultipleProtocolInterfaces (
             mCallbackInfo->DriverHandle,
             &gEfiDevicePathProtocolGuid,
             &mHiiVendorDevicePath,
             &gEfiHiiConfigAccessProtocolGuid,
             &mCallbackInfo->ConfigAccess,
             NULL
             );
    }

    if (mCallbackInfo->RegisteredHandle != NULL) {
      HiiRemovePackages (mCallbackInfo->RegisteredHandle);
    }

    FreePool (mCallbackInfo);
  }

  return EFI_SUCCESS;
}

