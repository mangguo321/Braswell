/** @file
  The function that processes the Smbios data type 0x88 before they are submitted to Data Hub.

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
#include <Library/PrintLib.h>
#include <Library/CpuIA32.h>
#include <Protocol/DxeSmmReadyToLock.h>

VOID
GetCPUStepping (
  CHAR16 *Stepping
)
{
  CHAR16    Buffer[40];

  UINT16                                FamilyId;
  UINT8                                 Model;
  UINT8                                 SteppingId;
  UINT8                                 ProcessorType;


  EfiCpuVersion (&FamilyId, &Model, &SteppingId, &ProcessorType);
  //we need raw Model data
  Model = Model & 0xf;
  //
  //Family/Model/Step
  UnicodeSPrint (Buffer, sizeof (Buffer), L"%d/%d/%d", FamilyId,  Model, SteppingId);
  StrCpy (Stepping, Buffer);
  return;
}

EFI_STATUS
SearchChildHandle (
  EFI_HANDLE Father,
  EFI_HANDLE *Child
  )
{
  EFI_STATUS                                                 Status;
  UINTN                                                      HandleIndex;
  EFI_GUID                                                   **ProtocolGuidArray = NULL;
  UINTN                                                      ArrayCount;
  UINTN                                                      ProtocolIndex;
  UINTN                                                      OpenInfoCount;
  UINTN                                                      OpenInfoIndex;
  EFI_OPEN_PROTOCOL_INFORMATION_ENTRY                        *OpenInfo = NULL;
  UINTN                                                      mHandleCount;
  EFI_HANDLE                                                 *mHandleBuffer= NULL;

  //
  // Retrieve the list of all handles from the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &mHandleCount,
                  &mHandleBuffer
                  );

  for (HandleIndex = 0; HandleIndex < mHandleCount; HandleIndex++) {
    //
    // Retrieve the list of all the protocols on each handle
    //
    Status = gBS->ProtocolsPerHandle (
                    mHandleBuffer[HandleIndex],
                    &ProtocolGuidArray,
                    &ArrayCount
                    );
    if (!EFI_ERROR (Status)) {
      for (ProtocolIndex = 0; ProtocolIndex < ArrayCount; ProtocolIndex++) {
        Status = gBS->OpenProtocolInformation (
                        mHandleBuffer[HandleIndex],
                        ProtocolGuidArray[ProtocolIndex],
                        &OpenInfo,
                        &OpenInfoCount
                        );

        if (!EFI_ERROR (Status)) {
          for (OpenInfoIndex = 0; OpenInfoIndex < OpenInfoCount; OpenInfoIndex++) {
            if (OpenInfo[OpenInfoIndex].AgentHandle == Father) {
              if ((OpenInfo[OpenInfoIndex].Attributes & EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) == EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) {
                *Child = mHandleBuffer[HandleIndex];
                Status = EFI_SUCCESS;
                goto TryReturn;
              }
            }
          }
          Status = EFI_NOT_FOUND;
        }
      }
      if (OpenInfo != NULL) {
        FreePool(OpenInfo);
        OpenInfo = NULL;
      }
    }
    if (ProtocolGuidArray != NULL) {
      FreePool (ProtocolGuidArray);
      ProtocolGuidArray = NULL;
    }
  }
TryReturn:
  if (OpenInfo != NULL) {
    FreePool (OpenInfo);
    OpenInfo = NULL;
  }
  if (ProtocolGuidArray != NULL) {
    FreePool(ProtocolGuidArray);
    ProtocolGuidArray = NULL;
  }
  if (mHandleBuffer != NULL) {
    FreePool (mHandleBuffer);
    mHandleBuffer = NULL;
  }

  return Status;
}

EFI_STATUS
JudgeHandleIsPCIDevice (
  EFI_HANDLE       Handle,
  UINT8            Device,
  UINT8            Funs
  )
{
  EFI_STATUS        Status;
  EFI_DEVICE_PATH   *DPath;
  EFI_DEVICE_PATH   *DevicePath;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DPath
                  );
  if (!EFI_ERROR (Status)) {
    DevicePath = DPath;
    while (!IsDevicePathEnd(DPath)) {
      if ((DPath->Type == HARDWARE_DEVICE_PATH) && (DPath->SubType == HW_PCI_DP)) {
        PCI_DEVICE_PATH   *PCIPath;
        PCIPath = (PCI_DEVICE_PATH*) DPath;
        DPath = NextDevicePathNode(DPath);

        if (IsDevicePathEnd(DPath) && (PCIPath->Device == Device) && (PCIPath->Function == Funs)) {
          return EFI_SUCCESS;
        }
      } else {
        DPath = NextDevicePathNode(DPath);
      }
    }
  }

  return EFI_UNSUPPORTED;
}

VOID
GetDriverName(
  EFI_HANDLE   Handle,
  CHAR16*      GOPVer
)
{
  EFI_DRIVER_BINDING_PROTOCOL        *BindHandle = NULL;	
  EFI_STATUS                         Status;
  UINT32                             Version;                                                 
  UINT16                             *Ptr;
  CHAR16                             Buffer[40];
  Status = gBS->OpenProtocol(
                                   Handle,
                                   &gEfiDriverBindingProtocolGuid,
                                   (VOID**)&BindHandle,
                                   NULL,
                                   NULL,
                                   EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                   );  
  
  if (EFI_ERROR(Status)) 
  {
  	StrCpy (GOPVer, L"NA");
    return;
  }
    
  Version = BindHandle->Version;
  Ptr = (UINT16*)&Version;
  UnicodeSPrint(Buffer, sizeof (Buffer), L"8.0.%04d", *(Ptr));
  StrCpy (GOPVer, Buffer);

  return;
}

VOID
GetGOPDriverName(
  CHAR16* GOPVer
)
{
  UINTN                     HandleCount;
  EFI_HANDLE                *Handles= NULL;
  UINTN                     Index;
  EFI_STATUS                Status;
  EFI_HANDLE                Child = 0;

  Status = gBS->LocateHandleBuffer(
		        ByProtocol, 
		        &gEfiDriverBindingProtocolGuid,  		        
		        NULL,
		        &HandleCount,
		        &Handles
                      );  

  for (Index = 0; Index < HandleCount ; Index++)
  {
    Status = SearchChildHandle(Handles[Index], &Child);
    if(!EFI_ERROR(Status))
    {
      Status = JudgeHandleIsPCIDevice(Child, 0x02, 0x00);
      if(!EFI_ERROR(Status))
      {
        // DEBUG ((EFI_D_ERROR, "JudgeHandleIsPCIDevice ok \n"));
        GetDriverName(Handles[Index], GOPVer);
        return;
      }
    }
  }
  StrCpy (GOPVer, L"NA");
  return;
}

VOID
GetUcodeVersion(
  CHAR16 *uCodeVer
)
{
  UINT32                   MicroCodeVersion;
  CHAR16                   Buffer[40];
  //
  // Microcode Revision
  //
  EfiWriteMsr (EFI_MSR_IA32_BIOS_SIGN_ID, 0);
  EfiCpuid (EFI_CPUID_VERSION_INFO, NULL);
  MicroCodeVersion = (UINT32) RShiftU64 (EfiReadMsr (EFI_MSR_IA32_BIOS_SIGN_ID), 32);
  UnicodeSPrint (Buffer, sizeof (Buffer), L"%x", MicroCodeVersion);
  StrCpy (uCodeVer, Buffer);
  return;
}

VOID
GetSeCVersion(
  CHAR16 *SECVer
)
{
  StrCpy (SECVer, L"NA");
  return;
}

EFI_STATUS
EFIAPI
AddSmbiosT0x90Callback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
  /*++

Routine Description:

  Publish the smbios OEM type 0x90.

Arguments:

  Event   - Event whose notification function is being invoked (gEfiDxeSmmReadyToLockProtocolGuid).
  Context - Pointer to the notification functions context, which is implementation dependent.

Returns:

  None

--*/
{
  EFI_STATUS                  Status;
  SMBIOS_TABLE_TYPE90         *SmbiosRecord;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  SMBIOS_TABLE_TYPE90_STRINGS SmbiosTableType90Strings;
  CHAR16                      *SECVer;
  CHAR16                      *uCodeVer;
  CHAR16                      *GOPVer;
  CHAR16                      *Stepping;
  CHAR8                       *OptionalStrStart;
  UINTN                       StringNumber;
  UINTN                       StringLength;
  CHAR16                      *StrBufferStart;
  CHAR16                      *StrBufferPtr;
  EFI_SMBIOS_PROTOCOL         *SmbiosProtocol;

  DEBUG ((EFI_D_INFO, "Executing SMBIOS T0x90 callback.\n"));

  gBS->CloseEvent (Event);    // Unload this event.

  //
  // First check for invalid parameters.
  //
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&SmbiosTableType90Strings, sizeof(SMBIOS_TABLE_TYPE90_STRINGS));
  StringNumber   = 0;
  StringLength   = 0;
  SECVer         = AllocateZeroPool (0x20);
  uCodeVer       = AllocateZeroPool (0x10);
  GOPVer         = AllocateZeroPool (0x18);
  Stepping       = AllocateZeroPool (0x18);
  ASSERT(SECVer != NULL);
  ASSERT(uCodeVer != NULL);
  ASSERT(GOPVer != NULL);
  ASSERT(Stepping != NULL);

  StrBufferStart = AllocateZeroPool (4 * SMBIOS_STRING_MAX_LENGTH * sizeof(CHAR16));
  StrBufferPtr   = StrBufferStart;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID *) &SmbiosProtocol);
  ASSERT_EFI_ERROR (Status);
  
  GetSeCVersion (SECVer);
  GetUcodeVersion (uCodeVer);
  GetGOPDriverName (GOPVer);
  GetCPUStepping (Stepping);

  DEBUG((EFI_D_INFO, "SECVerStr = %s, uCodeVerStr = %s, GOPStr = %s, SteppingStr = %s\n", SECVer, uCodeVer, GOPVer, Stepping));
  
  SmbiosStrInit (&SmbiosTableType90Strings.SECVersion, SECVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType90Strings.uCodeVersion, uCodeVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType90Strings.GOPVersion, GOPVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType90Strings.CpuStepping, Stepping, &StrBufferPtr, &StringNumber, &StringLength);
  
  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool (sizeof(SMBIOS_TABLE_TYPE90) + StringLength + StringNumber + 1);
  if (SmbiosRecord == NULL) {
    FreePool(SECVer);
    FreePool(uCodeVer);
    FreePool(GOPVer);
    FreePool(Stepping);
    FreePool(StrBufferStart);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem(SmbiosRecord, sizeof(SMBIOS_TABLE_TYPE90) + StringLength + StringNumber + 1);
  CopyMem(SmbiosRecord, Context, sizeof(SMBIOS_TABLE_TYPE90));
  
  //
  // Update SMBIOS Tables type90 strings
  //
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  SmbiosStringsUpdate ((CHAR16 **) &SmbiosTableType90Strings, OptionalStrStart, StringNumber);
  
  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = SmbiosProtocol->Add (
                             SmbiosProtocol,
                             NULL,
                             &SmbiosHandle,
                             (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                             );

  FreePool(SECVer);
  FreePool(uCodeVer);
  FreePool(GOPVer);
  FreePool(Stepping);
  FreePool(SmbiosRecord);
  FreePool(StrBufferStart);
  return Status;
}

/**
  This function makes boot time changes to the contents of the
  MiscOemType0x90 (Type 0x90).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscOemType0x90)
{
  EFI_STATUS                    Status;
  static BOOLEAN                CallbackIsInstalledT0x90 = FALSE;
  VOID                          *AddSmbiosT0x90CallbackNotifyReg;
  EFI_EVENT                     AddSmbiosT0x90CallbackEvent;

  //
  // This callback will create a OEM Type 0x90 record.
  //
  if (CallbackIsInstalledT0x90 == FALSE) {
    CallbackIsInstalledT0x90 = TRUE;      // Prevent more than 1 callback.
    DEBUG ((EFI_D_INFO, "Create Smbios T0x90 callback.\n"));

    //
    // gEfiDxeSmmReadyToLockProtocolGuid is ready
    //
    Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AddSmbiosT0x90Callback,
                  RecordData,
                  &AddSmbiosT0x90CallbackEvent
                  );

    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->RegisterProtocolNotify (
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  AddSmbiosT0x90CallbackEvent,
                  &AddSmbiosT0x90CallbackNotifyReg
                  );

    return Status;
  }

  return EFI_SUCCESS;
}
