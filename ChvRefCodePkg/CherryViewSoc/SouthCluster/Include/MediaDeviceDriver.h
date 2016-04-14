/** @file
  Header file for Media Device Driver

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MEDIA_DEVICE_DRIVER_H
#define _MEDIA_DEVICE_DRIVER_H

#include <Uefi.h>

#include <Guid/EventGroup.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include <IndustryStandard/Mmc.h>
#include <IndustryStandard/CeAta.h>
#include <IndustryStandard/SdCard.h>

//
// Driver Consumed Protocol Prototypes
//
#include <Protocol/DevicePath.h>
#include <Protocol/SdHostIo.h>

//
// Driver Produced Protocol Prototypes
//
#include <Protocol/DriverBinding.h>
#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/BlockIo.h>

extern EFI_COMPONENT_NAME_PROTOCOL  gMediaDeviceComponentName;
extern EFI_DRIVER_BINDING_PROTOCOL  gMediaDeviceDriverBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL  gSdMediaDeviceName2;

extern UINT32  gMediaDeviceDebugLevel;

#define CARD_DATA_SIGNATURE  SIGNATURE_32 ('c', 'a', 'r', 'd')
#define CARD_PARTITION_SIGNATURE  SIGNATURE_32 ('c', 'p', 'a', 'r')

#define CARD_PARTITION_DATA_FROM_THIS(a) \
    CR(a, MMC_PARTITION_DATA, BlockIo, CARD_PARTITION_SIGNATURE)

#define CARD_DATA_FROM_THIS(a) \
    ((CARD_PARTITION_DATA_FROM_THIS(a))->CardData)

#define CARD_DATA_PARTITION_NUM(p) \
    ((((UINTN) p) - ((UINTN) &(p->CardData->Partitions))) / sizeof (*p))

//
// Define RPMB Data Frame strucutre(512 Bytes). Byte order is MSB first.
// Below is the memory layout of RPMBDataFrame.
// When performing data exchange, some swap operation may need
// +------------------------+
// +        Stuff[0]        +
// +         ......         +
// +        Stuff[511]      +
// +        RPMBKey[32]     +
// +        Data[0]         +
// +        ......          +
// +        Data[31]        +
// +        Nonce[16]       +
// +        WriteCounter4]  +
// +        Address[0]      +
// +        Address[1]      +
// +        BlkCnt[0]       +
// +        BlkCnt[1]       +
// +        Result[0]       +
// +        Result[1]       +
// +        ReqResp[0]      +
// +        ReqResp[1]      +
// +------------------------+
#pragma pack(1)    
typedef struct {
    UINT8  Stuff[196];
    UINT8  RPMBKey[32];
    UINT8  Data[256];
    UINT8  Nonce[16];
    UINT8  WriteCounter[4];
    UINT16 Address; 
    UINT16 BlkCnt;
    UINT16 Result;
    UINT16 ReqResp;
} RPMBDataFrame;   
#pragma pack()

#define RPMB_REQUEST_KEY_WRITE   0x0100
#define RPMB_REQUEST_CTR_READ    0x0200
#define RPMB_REQUEST_AUTH_WRITE  0x0300
#define RPMB_REQUEST_AUTH_READ   0x0400
#define RPMB_REQUEST_READ_RESULT 0x0500

#define RPMB_RESPONSE_KEY_WRITE   0x0001
#define RPMB_RESPONSE_CTR_READ    0x0002
#define RPMB_RESPONSE_AUTH_WRITE  0x0003
#define RPMB_RESPONSE_AUTH_READ   0x0004

#define RPMB_RESULT_OP_OK         0x0000
#define RPMB_RESULT_GENERAL_FAIL  0x0100
#define RPMB_RESULT_AUTH_FAIL     0x0200
#define RPMB_RESULT_COUNTER_FAIL  0x0300
#define RPMB_RESULT_ADDRESS_FAIL  0x0400
#define RPMB_RESULT_WRITE_FAIL    0x0500
#define RPMB_RESULT_READ_FAIL     0x0600
#define RPMB_RESULT_KEY_NOT_PROGRAMMED 0x0700

#define RPMB_PARTITION             3
#define RPMB_DATA_FRAME_SIZE       512

#define CARD_DATA_FROM_RPMB_THIS(a) \
    CR(a, CARD_DATA, RPMBIo, CARD_DATA_SIGNATURE)

//
// Define a protocol for RPMB operation
//
// {2E7EF202-552A-4b1f-A0BD-B644216FA5C5}
#define EFI_EMMC_RPMB_OP_PROTOCOL_GUID \
  { \
    0x2e7ef202, 0x552a, 0x4b1f, 0xa0, 0xbd, 0xb6, 0x44, 0x21, 0x6f, 0xa5, 0xc5 } \
  }

typedef struct _EFI_EMMC_RPMB_OP_PROTOCOL  EFI_EMMC_RPMB_OP_PROTOCOL;

typedef BOOLEAN (EFIAPI *IS_RPMBKEY_PROGRAMMED)(IN EFI_EMMC_RPMB_OP_PROTOCOL *This);

typedef EFI_STATUS (EFIAPI *EMMC_PROGRAM_RPMBKEY)(IN EFI_EMMC_RPMB_OP_PROTOCOL *This, IN UINT8 * KeyString);

//
// EMMC Card info Structures
//
struct _EFI_EMMC_RPMB_OP_PROTOCOL {
  IS_RPMBKEY_PROGRAMMED EmmcIsRPMBProgrammed;
  EMMC_PROGRAM_RPMBKEY  EmmcProgramRPMBKey;
};

extern EFI_GUID gEfiEmmcRpmbOpProtocolGuid;

//
// Partition Number
//
#define NO_ACCESS_TO_BOOT_PARTITION                0x00
#define BOOT_PARTITION_1                           0x01
#define BOOT_PARTITION_2                           0x02
#define REPLAY_PROTECTED_MEMORY_BLOCK              0x03
#define GENERAL_PURPOSE_PARTITION_1                0x04
#define GENERAL_PURPOSE_PARTITION_2                0x05
#define GENERAL_PURPOSE_PARTITION_3                0x06
#define GENERAL_PURPOSE_PARTITION_4                0x07

//
// Command timeout will be max 100 ms
//
#define  TIMEOUT_COMMAND     100
#define  TIMEOUT_DATA        5000

typedef enum{
  UnknownCard = 0,
  MMCCard,                // MMC card
  CEATACard,              // CE-ATA device
  SDMemoryCard,           // SD 1.1 card
  SDMemoryCard2,          // SD 2.0 or above standard card
  SDMemoryCard2High       // SD 2.0 or above high capacity card
} CARD_TYPE;

typedef struct _CARD_DATA CARD_DATA;

typedef struct {
  //
  //BlockIO
  //
  UINT32                    Signature;

  EFI_HANDLE                Handle;

  BOOLEAN                   Present;

  EFI_DEVICE_PATH_PROTOCOL  *DevPath;

  EFI_BLOCK_IO_PROTOCOL     BlockIo;

  EFI_BLOCK_IO_MEDIA        BlockIoMedia;

  CARD_DATA                 *CardData;

} MMC_PARTITION_DATA;

#define MAX_NUMBER_OF_PARTITIONS 8

struct _CARD_DATA {
  //
  //BlockIO
  //
  UINT32                    Signature;

  EFI_HANDLE                Handle;

  MMC_PARTITION_DATA        Partitions[MAX_NUMBER_OF_PARTITIONS];

  EFI_SD_HOST_IO_PROTOCOL   *SdHostIo;
  EFI_EMMC_RPMB_OP_PROTOCOL RPMBIo;
  EFI_UNICODE_STRING_TABLE  *ControllerNameTable;
  CARD_TYPE                 CardType;

  UINT8                     CurrentBusWidth;
  BOOLEAN                   DualVoltage;
  BOOLEAN                   NeedFlush;
  UINT8                     Reserved[3];

  UINT16                    Address;
  UINT32                    BlockLen;
  UINT32                    MaxFrequency;
  UINT64                    BlockNumber;
  //
  //Common used
  //
  CARD_STATUS               CardStatus;
  OCR                       OCRRegister;
  CID                       CIDRegister;
  CSD                       CSDRegister;
  EXT_CSD                   ExtCSDRegister;
  UINT8                     *RawBufferPointer;
  UINT8                     *AlignedBuffer;
  //
  //CE-ATA specific
  //
  TASK_FILE                 TaskFile;
  IDENTIFY_DEVICE_DATA      IndentifyDeviceData;
  //
  //SD specific
  //
  SCR                       SCRRegister;
  SD_STATUS_REG             SDSattus;
  SWITCH_STATUS             SwitchStatus;
};

/**
  Entry point for Media Device EFI drivers.

  @param[in] ImageHandle          EFI_HANDLE
  @param[in] SystemTable          EFI_SYSTEM_TABLE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        The function failed to complete
**/
EFI_STATUS
EFIAPI
MediaDeviceDriverEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

/**
  Install Media Device BlockIo Protocol

  @param[in] CardData             Pointer to CARD_DATA

  @retval EFI_SUCCESS             BlockIo Protocol installed succesfully
  @retval EFI_OUT_OF_RESOURCES    The request could not be completed due to a lack of resources.
**/
EFI_STATUS
MediaDeviceDriverInstallBlockIo (
  IN  CARD_DATA    *CardData
  );

/**
  Uninstall Media Device BlockIo Protocol

  @param[in] CardData             Pointer to CARD_DATA

  @retval EFI_SUCCESS             BlockIo Protocol uninstalled succesfully
**/
EFI_STATUS
MediaDeviceDriverUninstallBlockIo (
  IN  CARD_DATA    *CardData
  );

/**
  Retrieves a Unicode string that is the user readable name of the EFI Driver.

  @param[in] This                 A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param[in] Language             A pointer to a three character ISO 639-2 language identifier.
                                  This is the language of the driver name that that the caller
                                  is requesting, and it must match one of the languages specified
                                  in SupportedLanguages.  The number of languages supported by a
                                  driver is up to the driver writer.
  @param[in] DriverName           A pointer to the Unicode string to return.  This Unicode string
                                  is the name of the driver specified by This in the language
                                  specified by Language.
   
  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Language is NULL
  @retval EFI_INVALID_PARAMETER   DriverName is NULL
  @retval EFI_UNSUPPORTED         The driver specified by This does not support the
                                  language specified by Language.
**/
EFI_STATUS
EFIAPI
MediaDeviceComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL     *This,
  IN  CHAR8                           *Language,
  OUT CHAR16                          **DriverName
  );

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by an EFI Driver.

  @param[in] This                 A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param[in] ControllerHandle     The handle of a controller that the driver specified by
                                  This is managing.  This handle specifies the controller
                                  whose name is to be returned.
  @param[in] ChildHandle          The handle of the child controller to retrieve the name
                                  of. This is an optional parameter that may be NULL.  It
                                  will be NULL for device drivers.  It will also be NULL
                                  for a bus drivers that wish to retrieve the name of the
                                  bus controller.  It will not be NULL for a bus driver
                                  that wishes to retrieve the name of a child controller.
  @param[in] Language             A pointer to a three character ISO 639-2 language
                                  identifier.  This is the language of the controller name
                                  that that the caller is requesting, and it must match one
                                  of the languages specified in SupportedLanguages.  The
                                  number of languages supported by a driver is up to the
                                  driver writer.
  @param[in] ControllerName       A pointer to the Unicode string to return.  This Unicode
                                  string is the name of the controller specified by
                                  ControllerHandle and ChildHandle in the language
                                  specified by Language from the point of view of the
                                  driver specified by This.

  @retval EFI_SUCCESS             The Unicode string for the user readable name in the
                                  language specified by Language for the driver
                                  specified by This was returned in DriverName.
  @retval EFI_INVALID_PARAMETER   ControllerHandle is not a valid EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER   ChildHandle is not NULL and it is not a valid
                                  EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER   Language is NULL
  @retval EFI_INVALID_PARAMETER   ControllerName is NULL.
  @retval EFI_UNSUPPORTED         The driver specified by This is not currently
                                  managing the controller specified by
                                  ControllerHandle and ChildHandle.
  @retval EFI_UNSUPPORTED         The driver specified by This does not support the
                                  language specified by Language.
**/
EFI_STATUS
EFIAPI
MediaDeviceComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL     *This,
  IN  EFI_HANDLE                      ControllerHandle,
  IN  EFI_HANDLE                      ChildHandle, OPTIONAL
  IN  CHAR8                           *Language,
  OUT CHAR16                          **ControllerName
  );

/**
  Test to see if this Media Device driver supports ControllerHandle. 
  Any ControllerHandle that has installed will be supported.

  @param[in] This                 Protocol instance pointer
  @param[in] Controller           Handle of device to test
  @param[in] RemainingDevicePath  Not used 

  @retval EFI_SUCCESS             This driver supports this device
  @retval EFI_UNSUPPORTED         This driver does not support this device
**/
EFI_STATUS
EFIAPI
MediaDeviceDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

/**
  Starting the Media Device Driver

  @param[in] This                 Protocol instance pointer
  @param[in] Controller           Handle of device to start
  @param[in] RemainingDevicePath  Not used 

  @retval EFI_SUCCESS             This driver start this device
  @retval EFI_UNSUPPORTED         This driver does not support this device 
  @retval EFI_DEVICE_ERROR        This driver cannot be started due to device Error
  @retval EFI_OUT_OF_RESOURCES    This driver cannot allocate resources
**/
EFI_STATUS
EFIAPI
MediaDeviceDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN EFI_HANDLE                      Controller,
  IN EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath
  );

/**
  Stop Media Device driver on ControllerHandle. Support stoping any child handles
  created by this driver.

  @param[in] This                 Protocol instance pointer
  @param[in] Controller           Handle of device to stop driver on
  @param[in] NumberOfChildren     Number of Children in the ChildHandleBuffer
  @param[in] ChildHandleBuffer    List of handles for the children we need to stop
   
  @retval EFI_SUCCESS             This driver stop this device
  @retval EFI_DEVICE_ERROR        This driver cannot be stop due to device Error
**/
EFI_STATUS
EFIAPI
MediaDeviceDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN  EFI_HANDLE                      Controller,
  IN  UINTN                           NumberOfChildren,
  IN  EFI_HANDLE                      *ChildHandleBuffer
  );

/**
  MMC/SD card init function
   
  @param[in] CardData             Pointer to CARD_DATA

  @retval EFI_SUCCESS             The function completed successfully 
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_UNSUPPORTED         The operation is not supported
  @retval EFI_BAD_BUFFER_SIZE     BufferSize is smaller than the size indicated
**/
EFI_STATUS
MMCSDCardInit (
  IN  CARD_DATA    *CardData
  );

/**
  MMC/SD card BlockIo init function
   
  @param[in] CardData             Pointer to CARD_DATA

  @retval EFI_SUCCESS             Success 
**/
EFI_STATUS
MMCSDBlockIoInit (
  IN  CARD_DATA    *CardData
  );

/**
  Send command by using Host IO protocol

  @param[in] This                 Pointer to EFI_SD_HOST_IO_PROTOCOL
  @param[in] CommandIndex         The command index to set the command index field of command register
  @param[in] Argument             Command argument to set the argument field of command register
  @param[in] DataType             TRANSFER_TYPE, indicates no data, data in or data out
  @param[in] Buffer               Contains the data read from / write to the device
  @param[in] BufferSize           The size of the buffer
  @param[in] ResponseType         RESPONSE_TYPE
  @param[in] TimeOut              Time out value in 1 ms unit 
  @param[in] ResponseData         Depending on the ResponseType, such as CSD or card status 

  @retval EFI_SUCCESS             The function completed successfully 
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_OUT_OF_RESOURCES    A resource has run out.
  @retval EFI_TIMEOUT             The timeout time expired. 
  @retval EFI_UNSUPPORTED         The operation is not supported
  @retval EFI_DEVICE_ERROR        The physical device reported an error while attempting the operation
**/
EFI_STATUS
SendCommand (
  IN   EFI_SD_HOST_IO_PROTOCOL    *This,
  IN   UINT16                     CommandIndex,
  IN   UINT32                     Argument,
  IN   TRANSFER_TYPE              DataType,
  IN   UINT8                      *Buffer, OPTIONAL
  IN   UINT32                     BufferSize,
  IN   RESPONSE_TYPE              ResponseType,
  IN   UINT32                     TimeOut,
  OUT  UINT32                     *ResponseData
  );

/**
  Send the card FAST_IO command

  @param[in] CardData             Pointer to CARD_DATA
  @param[in] RegisterAddress      Register Address 
  @param[in] RegisterData         Pointer to register Data
  @param[in] Write                TRUE for write, FALSE for read 
   
  @retval EFI_SUCCESS             The function completed successfully 
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_UNSUPPORTED         The operation is not supported
  @retval EFI_DEVICE_ERROR        The function failed with device error
**/
EFI_STATUS
FastIO (
  IN      CARD_DATA   *CardData,
  IN      UINT8       RegisterAddress,
  IN  OUT UINT8       *RegisterData,
  IN      BOOLEAN     Write
  );

/**
  Send the card APP_CMD command with the following command indicated
  by CommandIndex

  @param[in] CardData             Pointer to CARD_DATA
  @param[in] CommandIndex         The command index to set the command index field of command register
  @param[in] Argument             Command argument to set the argument field of command register
  @param[in] DataType             TRANSFER_TYPE, indicates no data, data in or data out
  @param[in] Buffer               Contains the data read from / write to the device
  @param[in] BufferSize           The size of the buffer
  @param[in] ResponseType         RESPONSE_TYPE
  @param[in] TimeOut              Time out value in 1 ms unit 
  @param[in] ResponseData         Depending on the ResponseType, such as CSD or card status 

  @retval EFI_SUCCESS             The function completed successfully 
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_OUT_OF_RESOURCES    A resource has run out.
  @retval EFI_TIMEOUT             The timeout time expired. 
  @retval EFI_UNSUPPORTED         The operation is not supported
  @retval EFI_DEVICE_ERROR        The physical device reported an error while attempting the operation
**/
EFI_STATUS
SendAppCommand (
  IN   CARD_DATA                  *CardData,
  IN   UINT16                     CommandIndex,
  IN   UINT32                     Argument,
  IN   TRANSFER_TYPE              DataType,
  IN   UINT8                      *Buffer, OPTIONAL
  IN   UINT32                     BufferSize,
  IN   RESPONSE_TYPE              ResponseType,
  IN   UINT32                     TimeOut,
  OUT  UINT32                     *ResponseData
  );

/**
  Function to get MMC EXT_CSD in 8 bits

  @param[in] CardData             Pointer to CARD_DATA
  @param[in] Offset               Register offset
   
  @retval ExtCSDRegister          Ext Csd register in 8 bits
**/
UINT32
MmcGetExtCsd8 (
  IN CARD_DATA                        *CardData,
  IN UINTN                            Offset
  );

/**
  Function to get MMC EXT_CSD in 24 bits

  @param[in] CardData             Pointer to CARD_DATA
  @param[in] Offset               Register offset
   
  @retval ExtCSDRegister          Ext Csd register in 24 bits
**/
UINT32
MmcGetExtCsd24 (
  IN CARD_DATA                        *CardData,
  IN UINTN                            Offset
  );

/**
  Function to get MMC EXT_CSD in 32 bits

  @param[in] CardData             Pointer to CARD_DATA
  @param[in] Offset               Register offset
   
  @retval ExtCSDRegister          Ext Csd register in 32 bits
**/
UINT32
MmcGetExtCsd32 (
  IN CARD_DATA                        *CardData,
  IN UINTN                            Offset
  );

/**
  Function to get MMC current partition

  @param[in] CardData             Pointer to CARD_DATA
   
  @retval CurrentPartition        MMC card current partition
**/
UINTN
MmcGetCurrentPartitionNum (
  IN  CARD_DATA              *CardData
  );

/**
  Function to set Write Protection on the MMC

  @param[in] Event                EFI_EVENT
  @param[in] Context              Pointer to the context

  @retval none
**/
VOID
EFIAPI
SetEmmcWpOnEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
);

VOID
SecureEraseEvent(
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

/**
  Function to select MMC partition

  @param[in] CardData             Pointer to CARD_DATA
  @param[in] Partition            Partition to select 

  @retval EFI_SUCCESS             The function completed successfully 
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_UNSUPPORTED         The operation is not supported 
  @retval EFI_DEVICE_ERROR        The function failed with device error
**/
EFI_STATUS
MmcSelectPartitionNum (
  IN  CARD_DATA              *CardData,
  IN  UINT8                  Partition
  );

/**
  Function to select MMC partition

  @param[in] Partition            Pointer to MMC Partition data

  @retval EFI_SUCCESS             The function completed successfully 
  @retval EFI_INVALID_PARAMETER   A parameter was incorrect.
  @retval EFI_UNSUPPORTED         The operation is not supported 
  @retval EFI_DEVICE_ERROR        The function failed with device error
**/
EFI_STATUS
MmcSelectPartition (
  IN  MMC_PARTITION_DATA     *Partition
  );

VOID
SecureErase(
  CARD_DATA              *CardData
  );

EFI_STATUS
EFIAPI
EmmcInstallRPMBProtocol (
IN CARD_DATA *CardData
);

#endif
