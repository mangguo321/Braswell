/** @file
  The Library for UEFIBootManager.

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _UEFI_BOOT_MANAGER_LIB_H_
#define _UEFI_BOOT_MANAGER_LIB_H_

//
// Boot Manager load option library functions.
//

//
// Load Option Type
//
typedef enum {
  LoadOptionTypeBoot,
  LoadOptionTypeDriver,
  LoadOptionTypeMax
} EFI_BOOT_MANAGER_LOAD_OPTION_TYPE;

typedef enum {
  LoadOptionNumberMax = 0x10000,
  LoadOptionNumberUnassigned = LoadOptionNumberMax
} EFI_BOOT_MANAGER_LOAD_OPTION_NUMBER;

//
// Common structure definition for DriverOption and BootOption
//
typedef struct {
  UINTN                             OptionNumber;       // #### numerical value, could be LoadOptionNumberUnassigned
  EFI_BOOT_MANAGER_LOAD_OPTION_TYPE OptionType;         // LoadOptionTypeBoot or LoadOptionTypeDriver
  UINT32                            Attributes;         // Load Option Attributes
  CHAR16                            *Description;       // Load Option Description
  EFI_DEVICE_PATH_PROTOCOL          *FilePath;          // Load Option Device Path
  UINT8                             *OptionalData;      // Load Option optional data to pass into image
  UINT32                            OptionalDataSize;   // Load Option size of OptionalData
  BOOLEAN                           BootNext;           // TRUE if this was a L"BootNext" Variable
  EFI_STATUS                        Status;             // Status returned from boot attempt gBS->StartImage ()
  CHAR16                            *ExitData;          // Exit data returned from gBS->StartImage () 
  UINTN                             ExitDataSize;       // Size of ExitData
} EFI_BOOT_MANAGER_LOAD_OPTION;

/**
  Returns an array of load options based on the EFI variable
  L"BootOrder"/L"DriverOrder" and the L"Boot####"/L"Driver####" variables impled by it.
  #### is the hex value of the UINT16 in each BootOrder/DriverOrder entry. 

  @param[out] LoadOptionCount   Returns number of entries in the array.
  @param[in]  LoadOptionType    The type of the load option.

  @retval   NULL                No load options exist.
  @retval   !NULL               Array of load option entries.

**/
EFI_BOOT_MANAGER_LOAD_OPTION *
EFIAPI
EfiBootManagerGetLoadOptions (
  OUT UINTN                            *LoadOptionCount,
  IN EFI_BOOT_MANAGER_LOAD_OPTION_TYPE LoadOptionType
  );

/**
  Free an array of load options which return from
  EfiBootManagerGetLoadOptions().

  @param[in]  LoadOptions       Pointer to the array of load options to free.
  @param[in]  LoadOptionCount   Number of array entries in LoadOptions.

  @retval EFI_SUCCESS           LoadOptions was freed.
  @retval EFI_INVALID_PARAMETER LoadOptions is NULL.

**/
EFI_STATUS
EFIAPI
EfiBootManagerFreeLoadOptions (
  IN  EFI_BOOT_MANAGER_LOAD_OPTION  *LoadOptions,
  IN  UINTN                         LoadOptionCount
  );

/**
  Get the option number that wasn't used.

  @param[in]  OptionType          Type of the load option.
  @param[out] FreeOptionNumber    Returns the minimal free option number.

  @retval EFI_SUCCESS             The option number is found.
  @retval EFI_NOT_FOUND           No free option number can be found.
                                  It happens when every number between 0000 and FFFF is used.
  @retval EFI_INVALID_PARAMETER   FreeOptionNumber is NULL.

**/
EFI_STATUS
EFIAPI
EfiBootManagerGetFreeOptionNumber (
  IN EFI_BOOT_MANAGER_LOAD_OPTION_TYPE  OptionType,
  OUT UINT16                            *FreeOptionNumber
  );

/**
  Initialize a load option.

  @param[in, out] Option           Pointer to the load option to be initialized.
  @param[in] OptionNumber          Option number of the load option.
  @param[in] OptionType            Type of the load option.
  @param[in] Attributes            Attributes of the load option.
  @param[in] Description           Description of the load option.
  @param[in] FilePath              Device path of the load option.
  @param[in] OptionalData          Optional data of the load option.
  @param[in] OptionalDataSize      Size of the optional data of the load option.

  @retval EFI_SUCCESS              The load option was initialized successfully.
  @retval EFI_NOT_FOUND            Cannot find a free option number to use.
  @retval EFI_INVALID_PARAMETER    Option, Description or FilePath is NULL.

**/
EFI_STATUS
EFIAPI
EfiBootManagerInitializeLoadOption (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION   *Option,
  IN  UINTN                             OptionNumber,
  IN  EFI_BOOT_MANAGER_LOAD_OPTION_TYPE OptionType,
  IN  UINT32                            Attributes,
  IN  CHAR16                            *Description,
  IN  EFI_DEVICE_PATH_PROTOCOL          *FilePath,
  IN  UINT8                             *OptionalData,
  IN  UINT32                            OptionalDataSize
  );

/**
  Free a load option that was created by the library.

  @param[in]  LoadOption        Pointer to the load option to free.
                                CONCERN: Check Boot#### instead of BootOrder, 
                                optimize, spec clarify

  @return EFI_SUCCESS           LoadOption was freed.
  @return EFI_INVALID_PARAMETER LoadOption is NULL.

**/
EFI_STATUS
EFIAPI
EfiBootManagerFreeLoadOption (
  IN  EFI_BOOT_MANAGER_LOAD_OPTION  *LoadOption
  );

/**
  Initialize the load option from the VariableName.

  @param[in]  VariableName          EFI Variable name which could be Boot#### or
                                    Driver####
  @param[in, out] LoadOption        Pointer to the load option to be initialized

  @retval EFI_SUCCESS               The option was created
  @retval EFI_INVALID_PARAMETER     VariableName or LoadOption is NULL.
  @retval EFI_NOT_FOUND             The variable specified by VariableName cannot be found.

**/
EFI_STATUS
EFIAPI
EfiBootManagerVariableToLoadOption (
  IN CHAR16                           *VariableName,
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *LoadOption
  );

/**
  Create the Boot#### or Driver#### variable from the load option.
  
  @param[in]  LoadOption      Pointer to the load option.

  @retval EFI_SUCCESS         The variable was created.
  @retval Others              Error status returned by RT->SetVariable.

**/
EFI_STATUS
EFIAPI
EfiBootManagerLoadOptionToVariable (
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION     *LoadOption
  );

/**
  This function will update the Boot####/Driver#### and the BootOrder/DriverOrder
  to add a new load option.

  @param[in]  Option        Pointer to load option to add.
  @param[in]  Position      Position of the new load option to put in the BootOrder/DriverOrder.

  @retval EFI_SUCCESS       The load option has been successfully added.
  @retval Others            Error status returned by RT->SetVariable.

**/
EFI_STATUS
EFIAPI
EfiBootManagerAddLoadOptionVariable (
  IN EFI_BOOT_MANAGER_LOAD_OPTION  *Option,
  IN UINTN                         Position
  );

/**
  Delete the load option according to the OptionNumber and OptionType.
  
  Only the BootOrder/DriverOrder is updated to remove the reference of the OptionNumber.
  
  @param[in]  OptionNumber        Option number of the load option.
  @param[in]  OptionType          Type of the load option.

  @retval EFI_NOT_FOUND           The load option cannot be found.
  @retval EFI_SUCCESS             The load option was deleted.

**/
EFI_STATUS
EFIAPI
EfiBootManagerDeleteLoadOptionVariable (
  IN UINTN                              OptionNumber,
  IN EFI_BOOT_MANAGER_LOAD_OPTION_TYPE  OptionType
  );

/**
  The comparator to be used by EfiBootManagerSortLoadOption.
  
  @param Left    Point to the EFI_BOOT_MANAGER_LOAD_OPTION.
  @param Right   Point to the EFI_BOOT_MANAGER_LOAD_OPTION.

  @retval TRUE   Left load option should be in front of the Right load option.
  @retval FALSE  Right load option should be in front of the Left load option.

**/
typedef
BOOLEAN
(EFIAPI *EFI_BOOT_MANAGER_LOAD_OPTION_COMPARATOR) (
  CONST EFI_BOOT_MANAGER_LOAD_OPTION  *Left,
  CONST EFI_BOOT_MANAGER_LOAD_OPTION  *Right
  );

/**
  Sort the load options. The DriverOrder/BootOrder variables will be re-created to 
  reflect the new order.

  @param OptionType        The type of the load option.
  @param Comparator        The comparator function pointer.

**/
VOID
EFIAPI
EfiBootManagerSortLoadOptionVariable (
  EFI_BOOT_MANAGER_LOAD_OPTION_TYPE        OptionType,
  EFI_BOOT_MANAGER_LOAD_OPTION_COMPARATOR  Comparator
  );

/**
  Return the index of the load option in the load option array.

  The function consider two load options are equal when the 
  OptionType, Attributes, Description, FilePath and OptionalData are equal.

  @param[in] Key      Pointer to the load option to be found.
  @param[in] Array    Pointer to the array of load options to be found.
  @param[in] Count    Number of entries in the Array.

  @retval -1          Key wasn't found in the Array.
  @retval 0 ~ Count-1 The index of the Key in the Array.

**/
INTN
EFIAPI
EfiBootManagerFindLoadOption (
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION *Key,
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION *Array,
  IN UINTN                              Count
  );

//
// Boot Manager hot key library functions.
//

#pragma pack(1)
///
/// EFI Key Option.
///
typedef struct {
  ///
  /// Specifies options about how the key will be processed.
  ///
  EFI_BOOT_KEY_DATA  KeyData;
  ///
  /// The CRC-32 which should match the CRC-32 of the entire EFI_LOAD_OPTION to
  /// which BootOption refers. If the CRC-32s do not match this value, then this key
  /// option is ignored.
  ///
  UINT32             BootOptionCrc;
  ///
  /// The Boot#### option which will be invoked if this key is pressed and the boot option
  /// is active (LOAD_OPTION_ACTIVE is set).
  ///
  UINT16             BootOption;
  ///
  /// The key codes to compare against those returned by the
  /// EFI_SIMPLE_TEXT_INPUT and EFI_SIMPLE_TEXT_INPUT_EX protocols.
  /// The number of key codes (0-3) is specified by the EFI_KEY_CODE_COUNT field in KeyOptions.
  ///
  EFI_INPUT_KEY      Keys[3];
  UINT16             OptionNumber;
} EFI_BOOT_MANAGER_KEY_OPTION;
#pragma pack()

/**
  Start the hot key service so that the key press can trigger the boot option.

  @param[in] HotkeyTriggered  Return the waitable event and it will be signaled 
                              when a valid hot key is pressed.

  @retval EFI_SUCCESS         The hot key service is started.

**/
  
EFI_STATUS
EFIAPI
EfiBootManagerStartHotkeyService (
  IN EFI_EVENT      *HotkeyTriggered
  );

//
// Modifier for EfiBootManagerAddKeyOptionVariable and EfiBootManagerDeleteKeyOptionVariable
//
#define EFI_BOOT_MANAGER_SHIFT_PRESSED    0x00000001
#define EFI_BOOT_MANAGER_CONTROL_PRESSED  0x00000002
#define EFI_BOOT_MANAGER_ALT_PRESSED      0x00000004
#define EFI_BOOT_MANAGER_LOGO_PRESSED     0x00000008
#define EFI_BOOT_MANAGER_MENU_KEY_PRESSED 0x00000010
#define EFI_BOOT_MANAGER_SYS_REQ_PRESSED  0x00000020

/**
  Add the key option.
  It adds the key option variable and the key option takes affect immediately.

  @param[out] AddedOption      Return the added key option.
  @param[in] BootOptionNumber  The boot option number for the key option.
  @param[in] Modifier          Key shift state.
  @param ...                   Parameter list of pointer of EFI_INPUT_KEY.

  @retval EFI_SUCCESS          The key option is added.
  @retval EFI_ALREADY_STARTED  The hot key is already used by certain key option.

**/
EFI_STATUS
EFIAPI
EfiBootManagerAddKeyOptionVariable (
  OUT EFI_BOOT_MANAGER_KEY_OPTION *AddedOption,   OPTIONAL
  IN UINT16                       BootOptionNumber,
  IN UINT32                       Modifier,
  ...
  );

/**
  Delete the Key Option variable and unregister the hot key

  @param[in] DeletedOption  Return the deleted key options.
  @param[in] Modifier       Key shift state.
  @param ...                Parameter list of pointer of EFI_INPUT_KEY.

  @retval EFI_SUCCESS       The key option is deleted.
  @retval EFI_NOT_FOUND     The key option cannot be found.

**/
EFI_STATUS
EFIAPI
EfiBootManagerDeleteKeyOptionVariable (
  IN EFI_BOOT_MANAGER_KEY_OPTION *DeletedOption, OPTIONAL
  IN UINT32                      Modifier,
  ...
  );

/**
  Return the array of key options.

  @param[out] Count  Return the number of key options.

  @retval NULL       No key option.
  @retval Other      Pointer to the key options.

**/
EFI_BOOT_MANAGER_KEY_OPTION *
EFIAPI
EfiBootManagerGetKeyOptions (
  OUT UINTN     *Count
  );

/**
  Free the key options returned from EfiBootManagerGetKeyOptions.

  @param[in] KeyOptions     Pointer to the key options.
  @param[in] KeyOptionCount Number of the key options.

  @retval EFI_SUCCESS       The key options are freed.
  @retval EFI_NOT_FOUND     KeyOptions is NULL.

**/
EFI_STATUS
EFIAPI
EfiBootManagerFreeKeyOptions (
  IN EFI_BOOT_MANAGER_KEY_OPTION    *KeyOptions,
  IN UINTN                          KeyOptionCount
  );

/**
  Register the key option to exit the waiting of the Boot Manager timeout.
  Platform should ensure that the continue key option isn't conflict with
  other boot key options.

  @param[in] Modifier         Key shift state.
  @param  ...                 Parameter list of pointer of EFI_INPUT_KEY.

  @retval EFI_SUCCESS         Successfully register the continue key option.
  @retval EFI_ALREADY_STARTED The continue key option is already registered.

**/
EFI_STATUS
EFIAPI
EfiBootManagerRegisterContinueKeyOption (
  IN UINT32           Modifier,
  ...
  );

/**
  Try to boot the boot option triggered by hot key.
**/
VOID
EFIAPI
EfiBootManagerHotkeyBoot (
  VOID
  );
//
// Boot Manager boot library functions.
//

/**
  The function enumerates all boot options, creates them and registers them in the BootOrder variable.
**/
VOID
EFIAPI
EfiBootManagerRefreshAllBootOption (
  VOID
  );

/**
  Attempt to boot the EFI boot option. This routine sets L"BootCurent", clears L"BootNext",
  and signals the EFI ready to boot event. If the device path for the option
  starts with a BBS device path a legacy boot is attempted. Short form device paths are 
  also supported via this rountine. A device path starting with 
  MEDIA_HARDDRIVE_DP, MSG_USB_WWID_DP, MSG_USB_CLASS_DP gets expaned out
  to find the first device that matches. If the BootOption Device Path 
  fails the removable media boot algorithm is attempted (\EFI\BOOTIA32.EFI,
  \EFI\BOOTX64.EFI,... only one file type is tried per processor type)

  @param[in] BootOption Boot Option to try and boot.
                        On return, BootOption->Status contains the boot status:
                        EFI_SUCCESS     BootOption was booted
                        EFI_UNSUPPORTED BootOption isn't supported.
                        EFI_NOT_FOUND   The BootOption was not found on the system
                        Others          BootOption failed with this error status

**/
VOID
EFIAPI
EfiBootManagerBoot (
  IN  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption
  );

VOID
EfiBootManagerDefaultBootBehavior (
  VOID
  );

/**
  This function is called by platform to register a new boot option for the Boot Manager Menu.

  The Boot Manager Menu is shown after successfully booting a boot option.
  
  @param[in] FilePath         Pointer to the Boot Manager Menu
  @param[in] Description      The optional description for the Boot Manager Menu
                              L"Boot Manager Menu" is used if the Description is NULL
  @param[out] OptionNumber    Return the boot option number of the Boot Manager Menu

  @retval EFI_SUCCESS         Successfully register the Boot Manager Menu.
  @retval EFI_ALREADY_STARTED The Boot Manager Menu is already registered.

**/
EFI_STATUS
EFIAPI
EfiBootManagerRegisterBootManagerMenu (
  IN EFI_DEVICE_PATH_PROTOCOL     *FilePath,
  IN CHAR16                       *Description,  OPTIONAL
  OUT UINT16                      *OptionNumber  OPTIONAL
  );

/**
  The function enumerates all the legacy boot options, creates them and registers them in the BootOrder variable.
**/
typedef
VOID
(EFIAPI *EFI_BOOT_MANAGER_REFRESH_LEGACY_BOOT_OPTION) (
  VOID
  );

/**
  The function boots a legacy boot option.
**/
typedef
VOID
(EFIAPI *EFI_BOOT_MANAGER_LEGACY_BOOT) (
  IN  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption
  );

/**
  The function registers the legacy boot support capabilities.

  @param RefreshLegacyBootOption The function pointer to create all the legacy boot options.
  @param LegacyBoot              The function pointer to boot the legacy boot option.
**/
VOID
EFIAPI
EfiBootManagerRegisterLegacyBootSupport (
  EFI_BOOT_MANAGER_REFRESH_LEGACY_BOOT_OPTION   RefreshLegacyBootOption,
  EFI_BOOT_MANAGER_LEGACY_BOOT                  LegacyBoot
  );

//
// Boot Manager connect and disconnect library functions
//

/**
  This function creates all handles associated with the given device
  path node. If the handle associated with one device path node cannot
  be created, then it tries to execute the dispatch to load the missing drivers.  

  @param[in]  DevicePath        The device path to be connected. Can be
                                a multi-instance device path.

  @retval EFI_SUCCESS           All handles associates with every device path node
                                were created.
  @retval EFI_OUT_OF_RESOURCES  Not enough resources to create new handles.
  @retval EFI_NOT_FOUND         The controller specified by the device path cannot be created.

**/
EFI_STATUS
EFIAPI
EfiBootManagerConnectDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePathToConnect,
  OUT EFI_HANDLE               *MatchingHandle          OPTIONAL
  );

/**
  This function will disconnect all current system handles. 
  
  gBS->DisconnectController() is invoked for each handle exists in system handle buffer.
  If handle is a bus type handle, all childrens also are disconnected recursively by
  gBS->DisconnectController().
**/
VOID
EFIAPI
EfiBootManagerDisconnectAll (
  VOID
  );

/**
  Connect the specific Usb device which match the short form device path.

  @param[in]  DevicePath         A short-form device path that starts with the first
                                 element being a USB WWID or a USB Class device
                                 path

  @return EFI_INVALID_PARAMETER  DevicePath is NULL pointer.
                                 DevicePath is not a USB device path.

  @return EFI_SUCCESS            Success to connect USB device
  @return EFI_NOT_FOUND          Fail to find handle for USB controller to connect.

**/
EFI_STATUS
EFIAPI
EfiBootManagerConnectUsbShortFormDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL   *DevicePath
  );

//
// Boot Manager console library functions
//

typedef enum {
  ConIn,
  ConOut,
  ErrOut,
  ConInDev,
  ConOutDev,
  ErrOutDev,
  ConsoleTypeMax
} CONSOLE_TYPE;

/**
  This function will search every input/output device in current system,
  and make every input/output device as potential console device.
**/
VOID
EFIAPI
EfiBootManagerConnectAllConsoles (
  VOID
  );

/**
  This function will connect all the console devices base on the console
  device variable ConIn, ConOut and ErrOut.
**/
VOID
EFIAPI
EfiBootManagerConnectAllDefaultConsoles (
  VOID
  );

/**
  This function updates the console variable based on ConVarName. It can
  add or remove one specific console device path from the variable

  @param[in]  ConsoleType              ConIn, ConOut, ErrOut, ConInDev, ConOutDev or ErrOutDev.
  @param[in]  CustomizedConDevicePath  The console device path to be added to
                                       the console variable. Cannot be multi-instance.
  @param[in]  ExclusiveDevicePath      The console device path to be removed
                                       from the console variable. Cannot be multi-instance.

  @retval EFI_UNSUPPORTED              The added device path is the same as a removed one.
  @retval EFI_SUCCESS                  Successfully added or removed the device path from the
                                       console variable.

**/
EFI_STATUS
EFIAPI
EfiBootManagerUpdateConsoleVariable (
  IN  CONSOLE_TYPE              ConsoleType,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
  );

/**
  Connect the console device base on the variable ConVarName. If
  ConVarName is a multi-instance device path, and at least one
  instance connects successfully, then this function
  will return success.

  @param[in]  ConVarName           The console related variable name: ConIn, ConOut,
                                   ErrOut.

  @retval EFI_NOT_FOUND            No console devices were connected successfully
  @retval EFI_SUCCESS              Connected at least one instance of the console
                                   device path based on the variable ConVarName.

**/
EFI_STATUS
EFIAPI
EfiBootManagerConnectConsoleVariable (
  IN  CONSOLE_TYPE              ConsoleType
  );

//
// Boot Manager device path library functions
//
/**
  Delete the instance in Multi that overlaps with Single. 

  @param[in]  Multi             A pointer to a multi-instance device path data
                                structure.
  @param[in]  Single            A pointer to a single-instance device path data
                                structure.

  @return This function removes the device path instances in Multi that overlap
          Single, and returns the resulting device path. If there is no
          remaining device path as a result, this function will return NULL.

**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiBootManagerDelPartMatchInstance (
  IN     EFI_DEVICE_PATH_PROTOCOL  *Multi,
  IN     EFI_DEVICE_PATH_PROTOCOL  *Single
  );

/**
  This function compares a device path data structure to that of all the nodes of a
  second device path instance.

  @param[in]  Multi             A pointer to a multi-instance device path data
                                structure.
  @param[in]  Single            A pointer to a single-instance device path data
                                structure.

  @retval TRUE                  If the Single device path is contained within a 
                                Multi device path.
  @retval FALSE                 The Single device path is not contained within a 
                                Multi device path.

**/
BOOLEAN
EFIAPI
EfiBootManagerMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  *Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  *Single
  );

/**
  The function expands a short-form device path to a full device path.

  It supports to expand the short-form hard drive media device path and 
  the short-form USB WWID or USB Class device path.

  @param[in] DevicePath  The short-form device path to expand.

  @retval FullDevicePath The full device path after expanding.
  @retval NULL           There is no corresponding full device path.
  @retval DevicePath     The device path is not a short-form device path.
                         Caller can compare the returning pointer value directly 
                         against the input DevicePath to determin whether the 
                         DevicePath is a short-form device path.

**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiBootManagerExpandShortFormDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL       *DevicePath
  );

/**
  Checks whether the Device path points to a bootable device, and if the device
  is ready to boot now.

  @param[in]  DevicePath  The device path to be checked.
  @param[in]  CheckMedia  If TRUE, check whether the device is ready to boot now.

  @retval TRUE            The device path is bootable.
  @retval FALSE           The device path is not bootable.

**/
BOOLEAN
EFIAPI
EfiBootManagerIsBootableDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN BOOLEAN                      CheckMedia
  );

//
// Boot Manager misc library functions.
//

/**
  The function will go through the driver option array, and then load and start
  every driver to which the driver option device path points.

  @param[in]  DriverOption        Pointer to driver option array to load.
  @param[in]  DriverOptionCount   Number of array entries in DriverOption

**/
VOID
EFIAPI
EfiBootManagerLoadDrivers (
  IN EFI_BOOT_MANAGER_LOAD_OPTION       *DriverOption,
  IN UINTN                              DriverOptionCount
  );

/**
  This function reads the EFI variable (VendorGuid/Name) and returns a dynamically allocated
  buffer and the size of the buffer. If it fails, return NULL.

  @param[in]  Name                   The string part of the EFI variable name.
  @param[in]  VendorGuid             The GUID part of the EFI variable name.
  @param[out]  VariableSize          Returns the size of the EFI variable that was read.

  @retval !NULL                      Dynamically allocated memory that contains a copy 
                                     of the EFI variable. The caller is responsible to 
                                     free the buffer.
  @retval NULL                       The variable was not found.

**/
VOID *
EFIAPI
EfiBootManagerGetVariableAndSize (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize
  );

#endif
