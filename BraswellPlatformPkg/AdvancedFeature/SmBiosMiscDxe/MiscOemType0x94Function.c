/** @file
  The function that processes the Smbios data type 0x94.

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
#include <Library/HiiLib.h>
#include <Protocol/CpuIo2.h>
#include <Library/PrintLib.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/IdeControllerInit.h>
#include <Protocol/MpService.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/I2cBus.h>
#include <Library/IoLib.h>
#include <Library/I2CLib.h>
#include <Library/CpuIA32.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Guid/Vlv2Variable.h>
#include "Valleyview.h"
#include "VlvAccess.h"
#include "PchAccess.h"
#include "PchCommonDefinitions.h"
#include <PlatformBaseAddresses.h>

typedef struct {
  UINT8 RevId;
  CHAR8 String[16];
} SB_REV;

SB_REV  SBRevisionTable[] = {
  {V_PCH_LPC_RID_A0, "(A0 Stepping)"},
  {V_PCH_LPC_RID_A1, "(A1 Stepping)"},
  {V_PCH_LPC_RID_A2, "(A2 Stepping)"},
  {V_PCH_LPC_RID_A3, "(A3 Stepping)"},
  {V_PCH_LPC_RID_A4, "(A4 Stepping)"},
  {V_PCH_LPC_RID_A5, "(A5 Stepping)"},
  {V_PCH_LPC_RID_A6, "(A6 Stepping)"},
  {V_PCH_LPC_RID_A7, "(A7 Stepping)"},
  {V_PCH_LPC_RID_B0, "(B0 Stepping)"},
  {V_PCH_LPC_RID_B1, "(B1 Stepping)"},
  {V_PCH_LPC_RID_B2, "(B2 Stepping)"},
  {V_PCH_LPC_RID_B3, "(B3 Stepping)"},
  {V_PCH_LPC_RID_B4, "(B4 Stepping)"},
  {V_PCH_LPC_RID_B5, "(B5 Stepping)"},
  {V_PCH_LPC_RID_B6, "(B6 Stepping)"},
  {V_PCH_LPC_RID_B7, "(B7 Stepping)"},
  {V_PCH_LPC_RID_C0, "(C0 Stepping)"},
  {V_PCH_LPC_RID_C1, "(C1 Stepping)"},
  {V_PCH_LPC_RID_C2, "(C2 Stepping)"},
  {V_PCH_LPC_RID_C3, "(C3 Stepping)"},
  {V_PCH_LPC_RID_C4, "(C4 Stepping)"},
  {V_PCH_LPC_RID_C5, "(C5 Stepping)"},
  {V_PCH_LPC_RID_C6, "(C6 Stepping)"},
  {V_PCH_LPC_RID_C7, "(C7 Stepping)"},
  {V_PCH_LPC_RID_D0, "(D0 Stepping)"},
  {V_PCH_LPC_RID_D1, "(D1 Stepping)"},
  {V_PCH_LPC_RID_D2, "(D2 Stepping)"},
  {V_PCH_LPC_RID_D3, "(D3 Stepping)"},
  {V_PCH_LPC_RID_D4, "(D4 Stepping)"},
  {V_PCH_LPC_RID_D5, "(D5 Stepping)"},
  {V_PCH_LPC_RID_D6, "(D6 Stepping)"},
  {V_PCH_LPC_RID_D7, "(D7 Stepping)"}
};

#define LEFT_JUSTIFY  0x01
#define PREFIX_SIGN   0x02
#define PREFIX_BLANK  0x04
#define COMMA_TYPE    0x08
#define LONG_TYPE     0x10
#define PREFIX_ZERO   0x20

#define ICH_REG_REV                 0x08
#define MSR_IA32_PLATFORM_ID        0x17
#define CHARACTER_NUMBER_FOR_VALUE  30

UINT8  ReadBuffer[20];  //Version report length
UINT8  WriteBuffer[22] = {0x40,0x01,0x14,0x00,0x06,0x51,0x02,0x07,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //Version request

UINTN
EfiValueToString (
  IN  OUT CHAR16  *Buffer,
  IN  INT64       Value,
  IN  UINTN       Flags,
  IN  UINTN       Width
  )
/*++

Routine Description:

  VSPrint worker function that prints a Value as a decimal number in Buffer

Arguments:

  Buffer - Location to place ascii decimal number string of Value.

  Value  - Decimal value to convert to a string in Buffer.

  Flags  - Flags to use in printing decimal string, see file header for details.

  Width  - Width of hex value.

Returns:

  Number of characters printed.

--*/
{
  CHAR16    TempBuffer[CHARACTER_NUMBER_FOR_VALUE];
  CHAR16    *TempStr;
  CHAR16    *BufferPtr;
  UINTN     Count;
  UINTN     ValueCharNum;
  UINTN     Remainder;
  CHAR16    Prefix;
  UINTN     Index;
  BOOLEAN   ValueIsNegative;
  UINT64    TempValue;

  TempStr         = TempBuffer;
  BufferPtr       = Buffer;
  Count           = 0;
  ValueCharNum    = 0;
  ValueIsNegative = FALSE;

  if (Width > CHARACTER_NUMBER_FOR_VALUE - 1) {
    Width = CHARACTER_NUMBER_FOR_VALUE - 1;
  }

  if (Value < 0) {
    Value           = -Value;
    ValueIsNegative = TRUE;
  }

  do {
    TempValue = Value;
    Value = (INT64)DivU64x32 ((UINT64)Value, 10);
    Remainder = (UINTN)((UINT64)TempValue - 10 * Value);
    *(TempStr++) = (CHAR16)(Remainder + '0');
    ValueCharNum++;
    Count++;
    if ((Flags & COMMA_TYPE) == COMMA_TYPE) {
      if (ValueCharNum % 3 == 0 && Value != 0) {
        *(TempStr++) = ',';
        Count++;
      }
    }
  } while (Value != 0);

  if (ValueIsNegative) {
    *(TempStr++)    = '-';
    Count++;
  }

  if ((Flags & PREFIX_ZERO) && !ValueIsNegative) {
    Prefix = '0';
  } else {
    Prefix = ' ';
  }

  Index = Count;
  if (!(Flags & LEFT_JUSTIFY)) {
    for (; Index < Width; Index++) {
      *(TempStr++) = Prefix;
    }
  }

  //
  // Reverse temp string into Buffer.
  //
  if (Width > 0 && (UINTN) (TempStr - TempBuffer) > Width) {
    TempStr = TempBuffer + Width;
  }
  Index = 0;
  while (TempStr != TempBuffer) {
    *(BufferPtr++) = *(--TempStr);
    Index++;
  }

  *BufferPtr = 0;

  return Index;
}

static CHAR16 mHexStr[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
                            L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };

UINTN
EfiValueToHexStr (
  IN  OUT CHAR16  *Buffer,
  IN  UINT64      Value,
  IN  UINTN       Flags,
  IN  UINTN       Width
  )
/*++

Routine Description:

  VSPrint worker function that prints a Value as a hex number in Buffer

Arguments:

  Buffer - Location to place ascii hex string of Value.

  Value  - Hex value to convert to a string in Buffer.

  Flags  - Flags to use in printing Hex string, see file header for details.

  Width  - Width of hex value.

Returns:

  Number of characters printed.

--*/
{
  CHAR16  TempBuffer[CHARACTER_NUMBER_FOR_VALUE];
  CHAR16  *TempStr;
  CHAR16  Prefix;
  CHAR16  *BufferPtr;
  UINTN   Count;
  UINTN   Index;

  TempStr   = TempBuffer;
  BufferPtr = Buffer;

  //
  // Count starts at one since we will null terminate. Each iteration of the
  // loop picks off one nibble. Oh yea TempStr ends up backwards
  //
  Count = 0;

  if (Width > CHARACTER_NUMBER_FOR_VALUE - 1) {
    Width = CHARACTER_NUMBER_FOR_VALUE - 1;
  }

  do {
    Index = ((UINTN)Value & 0xf);
    *(TempStr++) = mHexStr[Index];
    Value = RShiftU64 (Value, 4);
    Count++;
  } while (Value != 0);

  if (Flags & PREFIX_ZERO) {
    Prefix = '0';
  } else {
    Prefix = ' ';
  }

  Index = Count;
  if (!(Flags & LEFT_JUSTIFY)) {
    for (; Index < Width; Index++) {
      *(TempStr++) = Prefix;
    }
  }

  //
  // Reverse temp string into Buffer.
  //
  if (Width > 0 && (UINTN) (TempStr - TempBuffer) > Width) {
    TempStr = TempBuffer + Width;
  }
  Index = 0;
  while (TempStr != TempBuffer) {
    *(BufferPtr++) = *(--TempStr);
    Index++;
  }

  *BufferPtr = 0;
  return Index;
}

CHAR16 *
StrMacToString (
  OUT CHAR16              *String,
  IN  EFI_MAC_ADDRESS     *MacAddr,
  IN  UINT32              AddrSize
  )
/*++

Routine Description:
  Converts MAC address to Unicode string.
  The value is 64-bit and the resulting string will be 12
  digit hex number in pairs of digits separated by dashes.

Arguments:
  String - string that will contain the value
  Val    - value to convert

--*/
// GC_TODO: function comment is missing 'Returns:'
// GC_TODO:    MacAddr - add argument and description to function comment
// GC_TODO:    AddrSize - add argument and description to function comment
{
  UINT32  i;

  for (i = 0; i < AddrSize; i++) {

    EfiValueToHexStr (
      &String[2 * i],
      MacAddr->Addr[i] & 0xFF,
      PREFIX_ZERO,
      2
      );
  }

  //
  // Terminate the string.
  //
  String[2 * AddrSize] = L'\0';

  return String;
}

EFI_STATUS
TJudgeHandleIsPCIDevice (
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
    while (!IsDevicePathEnd (DPath)) {
      if ((DPath->Type == HARDWARE_DEVICE_PATH) && (DPath->SubType == HW_PCI_DP)) {
        PCI_DEVICE_PATH   *PCIPath;
        PCIPath = (PCI_DEVICE_PATH*) DPath;
        DPath = NextDevicePathNode(DPath);
        if (IsDevicePathEnd(DPath) && (PCIPath->Device == Device) && (PCIPath->Function == Funs)) {
          return EFI_SUCCESS;
        }
      }
      else {
        DPath = NextDevicePathNode(DPath);
      }
    }
  }

  return EFI_UNSUPPORTED;
}

EFI_STATUS
TSearchChildHandle (
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
    FreePool (ProtocolGuidArray);
    ProtocolGuidArray = NULL;
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
TGetDriverName (
  EFI_HANDLE   Handle,
  CHAR16         *Name
)
{
  EFI_DRIVER_BINDING_PROTOCOL        *BindHandle = NULL;
  EFI_STATUS                         Status;
  UINT32                             Version;
  UINT16                             *Ptr;
  Status = gBS->OpenProtocol (
                  Handle,
                  &gEfiDriverBindingProtocolGuid,
                  (VOID **) &BindHandle,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Version = BindHandle->Version;
  Ptr = (UINT16*)&Version;
  UnicodeSPrint(Name, 40, L"%d.%d.%d", Version >> 24 , (Version >>16)& 0x0f ,*(Ptr));

  return EFI_SUCCESS;
}

EFI_STATUS
TGetGOPDriverName (
  CHAR16 *Name
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
  for (Index = 0; Index < HandleCount ; Index++) {
    Status = TSearchChildHandle(Handles[Index], &Child);
    if (!EFI_ERROR (Status)) {
      Status = TJudgeHandleIsPCIDevice(Child, 0x02, 0x00);
      if (!EFI_ERROR (Status)) {
        return TGetDriverName(Handles[Index], Name);
      }
    }
  }

  return EFI_UNSUPPORTED;
}

EFI_STATUS
TGetTouchFirmwareVersion(
 CHAR16 *TouchVerString
)
{
  CHAR16     Buffer[40];

  UnicodeSPrint (Buffer, sizeof (Buffer), L"NA");
  StrCpy (TouchVerString, Buffer);

  return EFI_SUCCESS;
}

EFI_STATUS
UpdatePlatformInformation (
  CHAR16 *SECVer, CHAR16 *uCodeVer, CHAR16 *GOPVer, CHAR16 *MrcVer,
  CHAR16 *PmcVer, CHAR16 *UlpmcVer, CHAR16 *PunitVer, CHAR16 *SocVer,
  CHAR16 *BoardVer, CHAR16 *FabVer, CHAR16 *CpuFlavorString, CHAR16 *BiosVer,
  CHAR16 *PmicVer, CHAR16 *TouchVer, CHAR16 *SecureBootMode, CHAR16 *BootModeString,
  CHAR16 *SpeedStepMode, CHAR16 *MaxCState, CHAR16 *CpuTurbo, CHAR16 *GfxTurbo,
  CHAR16 *S0ix, CHAR16 *RC6
)
{
  UINT32                   MicroCodeVersion;
  CHAR16                   Buffer[SMBIOS_STRING_MAX_LENGTH + 2];
  EFI_STATUS               Status;
  UINT8                    CpuFlavor=0;
  EFI_PEI_HOB_POINTERS     GuidHob;
  EFI_PLATFORM_INFO_HOB    *mPlatformInfo=NULL;
  UINTN                    NumHandles;
  EFI_HANDLE                        *HandleBuffer;
  UINTN                             Index;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;
  UINTN                             PciD31F0RegBase;
  UINT8                             count;
  UINT8                             Data8;
  UINT8                             Data8_1;

  CHAR16                            Name[40];
  UINT32                            MrcVersion;
  CHAR16                            *Version;
  SYSTEM_CONFIGURATION              SystemConfiguration;
  EFI_BOOT_MODE                     BootMode;
  UINTN                             VarSize;
  EFI_PLATFORM_INFO_HOB             *mPlatformInfo;
  
  Version                           = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH + 2);

  //
  // Get the System configuration
  //
  CopyMem (&SystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  //
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  mPlatformInfo = PcdGetPtr (PcdPlatformInfo);

  GetSeCVersion (SECVer);
  Status = GetBiosVersionDateTime (Version, NULL, NULL);
  UnicodeSPrint (Buffer, sizeof (Buffer), L"%s", Version);
  FreePool(Version);
  StrCpy (BiosVer, Buffer);

  Status = TGetGOPDriverName(Name);

  if(!EFI_ERROR(Status))
  {
    StrCpy (GOPVer, Name);
  }

  //
  //CpuFlavor
  //
  //CHV
  //CHV-DC Tablet        000
  //CHV-QC Notebook      001
  //CHV-QC Desktop       010

  //CPU flavor
  CpuFlavor = RShiftU64 (EfiReadMsr (MSR_IA32_PLATFORM_ID), 50) & 0x07;

  switch(CpuFlavor){
    case 0x0:
        UnicodeSPrint (Buffer, sizeof (Buffer), L"%s (%01x)", L"CHV-DC Tablet", CpuFlavor);
        StrCpy (CpuFlavorString, Buffer);
        break;
    case 0x01:
#if (TABLET_PF_ENABLE == 1)
        UnicodeSPrint (Buffer, sizeof (Buffer), L"%s (%01x)", L"CHV-QC Tablet", CpuFlavor);
#else
        UnicodeSPrint (Buffer, sizeof (Buffer), L"%s (%01x)", L"CHV-QC Notebook", CpuFlavor);
#endif
        StrCpy (CpuFlavorString, Buffer);
        break;
    case 0x02:
        UnicodeSPrint (Buffer, sizeof (Buffer), L"%s (%01x)", L"CHV-QC Desktop", CpuFlavor);
        StrCpy (CpuFlavorString, Buffer);
        break;
    case 0x03:
        UnicodeSPrint (Buffer, sizeof (Buffer), L"%s (%01x)", L"CHV-QC Notebook", CpuFlavor);
        StrCpy (CpuFlavorString, Buffer);
        break;
    default:
        UnicodeSPrint (Buffer, sizeof (Buffer), L"%s (%01x)", L"Unknown CPU", CpuFlavor);
        StrCpy (CpuFlavorString, Buffer);
        break;
  }

  if ( NULL != mPlatformInfo) {
    //
    // Board Id
    //
    UnicodeSPrint (Buffer, sizeof (Buffer), L"%x", mPlatformInfo->BoardId);
    StrCpy (BoardVer, Buffer);

    //
    // FAB ID
    //
    UnicodeSPrint (Buffer, sizeof (Buffer), L"%x", mPlatformInfo->FABID);
    StrCpy (FabVer, Buffer);
  }

  //
  //Update MRC Version
  //
  MrcVersion = MmioRead32 (MmPciAddress (0, 0, 0, 0, 0xF0));
  MrcVersion &= 0xffff;
  Index = EfiValueToString (Buffer, MrcVersion/100, PREFIX_ZERO, 0);
  StrCat (Buffer, L".");
  EfiValueToString (Buffer + Index + 1, (MrcVersion%100)/10, PREFIX_ZERO, 0);
  EfiValueToString (Buffer + Index + 2, (MrcVersion%100)%10, PREFIX_ZERO, 0);
  StrCpy (MrcVer, Buffer);

  //
  //Update Soc Version
  //

  //
  // Retrieve all instances of PCH Platform Policy protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDxePchPlatformPolicyProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Find the matching PCH Policy protocol
    //
    for (Index = 0; Index < NumHandles; Index++) {
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gDxePchPlatformPolicyProtocolGuid,
                      &PchPlatformPolicy
                      );
      if (!EFI_ERROR (Status)) {
        PciD31F0RegBase = MmPciAddress (
                            0,
                            PchPlatformPolicy->BusNumber,
                            PCI_DEVICE_NUMBER_PCH_LPC,
                            PCI_FUNCTION_NUMBER_PCH_LPC,
                            0
                          );

         Data8 = MmioRead8 (PciD31F0RegBase + R_PCH_LPC_RID_CC) & B_PCH_LPC_RID_STEPPING_MASK;
         count = sizeof (SBRevisionTable) / sizeof (SBRevisionTable[0]);
         for (Index = 0; Index < count; Index++) {
           if(Data8 == SBRevisionTable[Index].RevId) {
              UnicodeSPrint (Buffer, sizeof (Buffer), L"%02x %a", Data8, SBRevisionTable[Index].String);
              StrCpy (SocVer, Buffer);
             break;
           }
         }
        break;
      }
    }
  }

  //
  // Microcode Revision
  //
  EfiWriteMsr (EFI_MSR_IA32_BIOS_SIGN_ID, 0);
  EfiCpuid (EFI_CPUID_VERSION_INFO, NULL);
  MicroCodeVersion = (UINT32) RShiftU64 (EfiReadMsr (EFI_MSR_IA32_BIOS_SIGN_ID), 32);
  UnicodeSPrint (Buffer, sizeof (Buffer), L"%x", MicroCodeVersion);
  StrCpy (uCodeVer, Buffer);

  //
  //Secure boot
  //
  Data8 = SystemConfiguration.SecureBoot;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (SecureBootMode, Buffer);

  //
  //Bootmode
  //
  BootMode = GetBootModeHob();
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", BootMode);
  StrCpy (BootModeString, Buffer);

  //
  //SpeedStep
  //
  Data8 = SystemConfiguration.EnableGv;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (SpeedStepMode, Buffer);

  //
  //CPU Turbo
  //
  Data8 = SystemConfiguration.TurboModeEnable;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (CpuTurbo, Buffer);

  //
  //CState
  //
  Data8 = SystemConfiguration.MaxCState;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (MaxCState, Buffer);

  //
  //GFX Turbo
  //
  Data8 = SystemConfiguration.IgdTurboEnabled;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (GfxTurbo, Buffer);

  //
  //S0ix
  //
  Data8 = SystemConfiguration.S0ix;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (S0ix, Buffer);

  //
  //RC6
  //
  Data8 = SystemConfiguration.EnableRenderStandby;
  UnicodeSPrint (Buffer, sizeof(Buffer), L"%x", Data8);
  StrCpy (RC6, Buffer);

  //
  // Punit Version
  //
  Data8 = (UINT8) EfiReadMsr (0x667);
  UnicodeSPrint (Buffer, sizeof (Buffer), L"0x%x", Data8);
  StrCpy (PunitVer, Buffer);

  //
  // PMC Version
  //
  Data8 = (UINT8)((MmioRead32 (PMC_BASE_ADDRESS + R_PCH_PMC_PRSTS)>>16)&0x00FF);
  Data8_1 = (UINT8)((MmioRead32 (PMC_BASE_ADDRESS + R_PCH_PMC_PRSTS)>>24)&0x00FF);
  UnicodeSPrint (Buffer, sizeof (Buffer), L"0x%X_%X", Data8_1, Data8);
  StrCpy (PmcVer, Buffer);

  TGetTouchFirmwareVersion(TouchVer);

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
AddSmbiosT0x94Callback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
  /*++

Routine Description:

  Smbios OEM type 0x94 callback.

Arguments:

  Event   - Event whose notification function is being invoked.
  Context - Pointer to the notification functions context, which is implementation dependent.

Returns:

  None

--*/
{
  EFI_STATUS                  Status;
  SMBIOS_TABLE_TYPE94         *SmbiosRecord;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  SMBIOS_TABLE_TYPE94_STRINGS SmbiosTableType94Strings;
  CHAR16                      *SECVer;
  CHAR16                      *uCodeVer;
  CHAR16                      *GOPVer;
  CHAR16                      *MrcVer;
  CHAR16                      *PmcVer;
  CHAR16                      *UlpmcVer;
  CHAR16                      *PunitVer;
  CHAR16                      *SocVer;
  CHAR16                      *BoardVer;
  CHAR16                      *FabVer;
  CHAR16                      *CpuFlavor;
  CHAR16                      *BiosVer;
  CHAR16                      *PmicVer;
  CHAR16                      *TouchVer;
  CHAR16                      *SecureBootMode;
  CHAR16                      *BootMode;
  CHAR16                      *SpeedStepMode;
  CHAR16                      *MaxCState;
  CHAR16                      *CpuTurbo;
  CHAR16                      *GfxTurbo;
  CHAR16                      *S0ix;
  CHAR16                      *RC6;

  CHAR8                       *OptionalStrStart;
  UINTN                       StringNumber;
  UINTN                       StringLength;
  CHAR16                      *StrBufferStart;
  CHAR16                      *StrBufferPtr;
  EFI_SMBIOS_PROTOCOL         *SmbiosProtocol;

  DEBUG ((EFI_D_INFO, "Executing SMBIOS T0x94 callback.\n"));

  gBS->CloseEvent (Event);    // Unload this event.

  //
  // First check for invalid parameters.
  //
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&SmbiosTableType94Strings, sizeof(SMBIOS_TABLE_TYPE94_STRINGS));
  StringNumber   = 0;
  StringLength   = 0;

  SECVer         = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  uCodeVer       = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  GOPVer         = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  MrcVer         = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  PmcVer         = AllocateZeroPool (0x10);
  UlpmcVer       = AllocateZeroPool (0x10);
  PunitVer       = AllocateZeroPool (0x10);
  SocVer         = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  BoardVer       = AllocateZeroPool (0x10);
  FabVer         = AllocateZeroPool (0x10);
  CpuFlavor      = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH);
  BiosVer        = AllocateZeroPool (SMBIOS_STRING_MAX_LENGTH + 2);
  PmicVer        = AllocateZeroPool (0x10);
  TouchVer       = AllocateZeroPool (0x10);
  SecureBootMode = AllocateZeroPool (0x10);
  BootMode       = AllocateZeroPool (0x10);
  SpeedStepMode  = AllocateZeroPool (0x10);
  MaxCState      = AllocateZeroPool (0x10);
  CpuTurbo       = AllocateZeroPool (0x10);
  GfxTurbo       = AllocateZeroPool (0x10);
  S0ix           = AllocateZeroPool (0x10);
  RC6            = AllocateZeroPool (0x10);

  ASSERT(SECVer != NULL);
  ASSERT(uCodeVer != NULL);
  ASSERT(GOPVer != NULL);
  ASSERT(MrcVer != NULL);
  ASSERT(PmcVer != NULL);
  ASSERT(UlpmcVer != NULL);
  ASSERT(PunitVer != NULL);
  ASSERT(SocVer != NULL);
  ASSERT(BoardVer != NULL);
  ASSERT(FabVer != NULL);
  ASSERT(CpuFlavor != NULL);
  ASSERT(BiosVer != NULL);
  ASSERT(SecureBootMode != NULL);
  ASSERT(BootMode != NULL);
  ASSERT(SpeedStepMode != NULL);
  ASSERT(MaxCState != NULL);
  ASSERT(CpuTurbo != NULL);
  ASSERT(GfxTurbo != NULL);
  ASSERT(S0ix != NULL);
  ASSERT(RC6 != NULL);

  StrBufferStart = AllocateZeroPool (22 * SMBIOS_STRING_MAX_LENGTH * sizeof(CHAR16));
  StrBufferPtr   = StrBufferStart;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID *) &SmbiosProtocol);
  ASSERT_EFI_ERROR (Status);

  UpdatePlatformInformation (
    SECVer, uCodeVer, GOPVer, MrcVer,
    PmcVer, UlpmcVer, PunitVer, SocVer,
    BoardVer, FabVer, CpuFlavor, BiosVer,
    PmicVer, TouchVer, SecureBootMode, BootMode,
    SpeedStepMode, MaxCState, CpuTurbo, GfxTurbo,
    S0ix, RC6
  );

  SmbiosStrInit (&SmbiosTableType94Strings.SECVersion, SECVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.uCodeVersion, uCodeVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.GopVersion, GOPVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.MRCVersion, MrcVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.PMCVersion, PmcVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.ULPMCVersion, UlpmcVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.PUnitVersion, PunitVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.SoCVersion, SocVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.BoardVersion, BoardVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.FabVersion, FabVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.CPUFlavor, CpuFlavor, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.BiosVersion, BiosVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.PmicVersion, PmicVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.TouchVersion, TouchVer, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.SecureBoot, SecureBootMode, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.BootMode, BootMode, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.SpeedStepMode, SpeedStepMode, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.MaxCState, MaxCState, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.CPUTurboMode, CpuTurbo, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.GfxTurbo, GfxTurbo, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.S0ix, S0ix, &StrBufferPtr, &StringNumber, &StringLength);
  SmbiosStrInit (&SmbiosTableType94Strings.RC6, RC6, &StrBufferPtr, &StringNumber, &StringLength);

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool (sizeof(SMBIOS_TABLE_TYPE94) + StringLength + StringNumber + 1);
  if (SmbiosRecord == NULL) {
    FreePool(SECVer);
    FreePool(uCodeVer);
    FreePool(GOPVer);
    FreePool(MrcVer);
    FreePool(PmcVer);
    FreePool(UlpmcVer);
    FreePool(PunitVer);
    FreePool(SocVer);
    FreePool(BoardVer);
    FreePool(FabVer);
    FreePool(CpuFlavor);
    FreePool(BiosVer);
    FreePool(PmicVer);
    FreePool(TouchVer);
    FreePool(SecureBootMode);
    FreePool(BootMode);
    FreePool(SpeedStepMode);
    FreePool(MaxCState);
    FreePool(CpuTurbo);
    FreePool(GfxTurbo);
    FreePool(S0ix);
    FreePool(RC6);
    FreePool(SmbiosRecord);
    FreePool(StrBufferStart);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem(SmbiosRecord, sizeof(SMBIOS_TABLE_TYPE94) + StringLength + StringNumber + 1);
  CopyMem(SmbiosRecord, Context, sizeof(SMBIOS_TABLE_TYPE94));

  //
  // Update SMBIOS Tables type94 strings
  //
  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  SmbiosStringsUpdate ((CHAR16 **) &SmbiosTableType94Strings, OptionalStrStart, StringNumber);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = SmbiosProtocol-> Add(
                      SmbiosProtocol,
                      NULL,
                      &SmbiosHandle,
                      (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                      );
  FreePool(SECVer);
  FreePool(uCodeVer);
  FreePool(GOPVer);
  FreePool(MrcVer);
  FreePool(PmcVer);
  FreePool(UlpmcVer);
  FreePool(PunitVer);
  FreePool(SocVer);
  FreePool(BoardVer);
  FreePool(FabVer);
  FreePool(CpuFlavor);
  FreePool(BiosVer);
  FreePool(PmicVer);
  FreePool(TouchVer);
  FreePool(SecureBootMode);
  FreePool(BootMode);
  FreePool(SpeedStepMode);
  FreePool(MaxCState);
  FreePool(CpuTurbo);
  FreePool(GfxTurbo);
  FreePool(S0ix);
  FreePool(RC6);
  FreePool(SmbiosRecord);
  FreePool(StrBufferStart);
  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the contents of the
  MiscOemType0x94 (Type 0x94).

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscOemType0x94)
{
  EFI_STATUS                    Status;
  EFI_EVENT                     AddSmbiosT0x94CallbackEvent;

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             AddSmbiosT0x94Callback,
             RecordData,
             &AddSmbiosT0x94CallbackEvent
             );

  ASSERT_EFI_ERROR (Status);

  return Status;
}
