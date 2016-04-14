/** @file
  Media Device Driver header

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

#include <Library/UefiDriverEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/TimerLib.h>
#include <Ppi/Sdhc.h>
#include <Ppi/PeiBlockIo.h>

#include <IndustryStandard/Mmc.h>
#include <IndustryStandard/SdCard.h>
//
// Driver Consumed Protocol Prototypes
//
#include <Protocol/DevicePath.h>
//
// Driver Produced Protocol Prototypes
//
#include <Ppi/BlockIo.h>

extern UINT32  gMediaDeviceDebugLevel;

#define PEI_CARD_DATA_SIGNATURE  SIGNATURE_32 ('p', 'c', 'r', 'd')
#define PEI_CARD_PARTITION_SIGNATURE  SIGNATURE_32 ('p', 'c', 'a', 'r')

#define MAX_NUMBER_OF_PARTITIONS 8

#define PEI_CARD_PARTITION_DATA_FROM_THIS(a) \
    CR(a, PEI_MMC_PARTITION_DATA, BlockIo, PEI_CARD_PARTITION_SIGNATURE)

#define PEI_CARD_DATA_FROM_THIS(a) \
    ((PEI_CARD_PARTITION_DATA_FROM_THIS(a))->PeiCardData)

#define PEI_CARD_DATA_PARTITION_NUM(p) \
    ((((UINTN) p) - ((UINTN) &(p->PeiCardData->Partitions))) / sizeof (*p))

  //
  // Used to check-call local functions inside this eMMC module
  //
  // Func - The function name which needs to be checked
  // Str   - Optional, it is the description of the output infomation
  // FailOrNot - When functional call is failed, it should stop or continue
  //
#define CHK_FUNC_CALL(Func, Str, FailOrNot) \
    do { \
      EFI_STATUS ret = Func; \
      if (EFI_SUCCESS!= ret && TRUE == FailOrNot) { \
        DEBUG((EFI_D_ERROR, "ERROR: %a,%d:", __FUNCTION__,__LINE__)); \
        if ( NULL != (void *)Str  ) {\
          DEBUG((EFI_D_ERROR, "%a:", Str)); \
        } \
        DEBUG((EFI_D_ERROR, " Status = 0x%x\n", ret)); \
        return ret; \
      } \
    } while (0)

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
#define  TIMEOUT_COMMAND     500
#define  TIMEOUT_DATA        5000

typedef enum{
  UnknownCard = 0,
  MMCCard,                // MMC card
  CEATACard,              // CE-ATA device
  SDMemoryCard,           // SD 1.1 card
  SDMemoryCard2,          // SD 2.0 or above standard card
  SDMemoryCard2High       // SD 2.0 or above high capacity card
}CARD_TYPE;

typedef struct _PEI_CARD_DATA PEI_CARD_DATA;

/**
  Writes the requested number of blocks to the specified block device.

  The function writes the requested number of blocks to the device. All the 
  blocks are write, or an error is returned. If there is no media in the device,
  the function returns EFI_NO_MEDIA.

  @param[in]  PeiServices   General-purpose services that are available to 
                            every PEIM.
  @param[in]  This          Indicates the EFI_PEI_RECOVERY_BLOCK_IO_PPI instance.
  @param[in]  DeviceIndex   Specifies the block device to which the function wants 
                            to talk. Because the driver that implements Block I/O 
                            PPIs will manage multiple block devices, PPIs that 
                            want to talk to a single device must specify the device 
                            index that was assigned during the enumeration process. 
                            This index is a number from one to NumberBlockDevices.
  @param[in]  StartLBA      The starting logical block address (LBA) to write to
                            on the device
  @param[in]  BufferSize    The size of the Buffer in bytes. This number must be
                            a multiple of the intrinsic block size of the device.
  @param[out] Buffer        A pointer to the destination buffer for the data.
                            The caller is responsible for the ownership of the 
                            buffer.
                         
  @retval EFI_SUCCESS             The data was read correctly from the device.
  @retval EFI_DEVICE_ERROR        The device reported an error while attempting 
                                  to perform the write operation.
  @retval EFI_INVALID_PARAMETER   The read request contains LBAs that are not 
                                  valid, or the buffer is not properly aligned.
  @retval EFI_NO_MEDIA            There is no media in the device.
  @retval EFI_BAD_BUFFER_SIZE     The BufferSize parameter is not a multiple of
                                  the intrinsic block size of the device.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_PEI_WRITE_BLOCKS) (
  IN  EFI_PEI_SERVICES               **PeiServices,
  IN  EFI_PEI_RECOVERY_BLOCK_IO_PPI  *This,
  IN  UINTN                          DeviceIndex,
  IN  EFI_PEI_LBA                    StartLBA,
  IN  UINTN                          BufferSize,
  OUT VOID                           *Buffer
  );

typedef struct {
  //
  //BlockIO
  //
  UINT32                    Signature;
  EFI_HANDLE                Handle;
  BOOLEAN                   Present;
  EFI_DEVICE_PATH_PROTOCOL  *DevPath;
  EFI_PEI_RECOVERY_BLOCK_IO_PPI   BlockIo;
  EFI_PEI_WRITE_BLOCKS            WriteBlocks;
  EFI_PEI_PPI_DESCRIPTOR          BlkIoPpiList;
  PEI_BLOCK_IO_MEDIA        BlockIoMedia;
  PEI_CARD_DATA             *PeiCardData;
} PEI_MMC_PARTITION_DATA;

struct _PEI_CARD_DATA {
  //
  //BlockIO
  //
  UINT32                    Signature;
  EFI_HANDLE                Handle;
  PEI_MMC_PARTITION_DATA    Partitions[MAX_NUMBER_OF_PARTITIONS];
  EFI_PEI_PPI_DESCRIPTOR    PpiList;
  PEI_SD_CONTROLLER_PPI     *SdControllerPpi;
  EFI_PEI_PCI_CFG2_PPI      *PciCfgPpi;
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
  //SD specific
  //
  SCR                       SCRRegister;
  SD_STATUS_REG             SDSattus;
  SWITCH_STATUS             SwitchStatus;
};

EFI_STATUS
EFIAPI
MediaDeviceDriverEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

EFI_STATUS
EFIAPI
BotGetNumberOfBlockDevices (
  IN  EFI_PEI_SERVICES                         **PeiServices,
  IN  EFI_PEI_RECOVERY_BLOCK_IO_PPI            *This,
  OUT UINTN                                    *NumberBlockDevices
  );

EFI_STATUS
EFIAPI
BotGetMediaInfo (
  IN  EFI_PEI_SERVICES                          **PeiServices,
  IN  EFI_PEI_RECOVERY_BLOCK_IO_PPI             *This,
  IN  UINTN                                     DeviceIndex,
  OUT EFI_PEI_BLOCK_IO_MEDIA                    *MediaInfo
  );

EFI_STATUS
EFIAPI
MMCSDBlockReadBlocks (
  IN  EFI_PEI_SERVICES                          **PeiServices,
  IN   EFI_PEI_RECOVERY_BLOCK_IO_PPI        *This,
  IN   UINT32                  MediaId,
  IN   EFI_LBA                 LBA,
  IN   UINTN                   BufferSize,
  OUT  VOID                    *Buffer
  );

EFI_STATUS
EFIAPI
MMCSDBlockWriteBlocks (
  IN  EFI_PEI_SERVICES                          **PeiServices,
  IN   EFI_PEI_RECOVERY_BLOCK_IO_PPI        *This,
  IN   UINT32                  MediaId,
  IN   EFI_LBA                 LBA,
  IN   UINTN                   BufferSize,
  IN   VOID                    *Buffer
  );

EFI_STATUS
MMCSDCardInit (
  IN  PEI_CARD_DATA              *PeiCardData
  );

EFI_STATUS
MMCSDBlockIoInit (
  IN  PEI_CARD_DATA    *PeiCardData
  );

EFI_STATUS
SendCommand (
  IN  PEI_SD_CONTROLLER_PPI       *This,
  IN   UINT16                     CommandIndex,
  IN   UINT32                     Argument,
  IN   TRANSFER_TYPE              DataType,
  IN   UINT8                      *Buffer, OPTIONAL
  IN   UINT32                     BufferSize,
  IN   RESPONSE_TYPE              ResponseType,
  IN   UINT32                     TimeOut,
  OUT  UINT32                     *ResponseData
  );

EFI_STATUS
FastIO (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN      UINT8       RegisterAddress,
  IN  OUT UINT8       *RegisterData,
  IN      BOOLEAN     Write
  );

EFI_STATUS
IndentifyDevice (
  IN  PEI_CARD_DATA              *PeiCardData
  );

EFI_STATUS
FlushCache (
  IN  PEI_CARD_DATA    *PeiCardData
  );

EFI_STATUS
StandByImmediate (
  IN  PEI_CARD_DATA           *PeiCardData
  );

EFI_STATUS
ReadDMAExt (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN  EFI_LBA     LBA,
  IN  UINT8       *Buffer,
  IN  UINT16      SectorCount
  );

EFI_STATUS
WriteDMAExt (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN  EFI_LBA     LBA,
  IN  UINT8       *Buffer,
  IN  UINT16      SectorCount
  );

EFI_STATUS
SoftwareReset (
  IN  PEI_CARD_DATA              *PeiCardData
  );

EFI_STATUS
SendAppCommand (
  IN  PEI_CARD_DATA              *PeiCardData,
  IN   UINT16                     CommandIndex,
  IN   UINT32                     Argument,
  IN   TRANSFER_TYPE              DataType,
  IN   UINT8                      *Buffer, OPTIONAL
  IN   UINT32                     BufferSize,
  IN   RESPONSE_TYPE              ResponseType,
  IN   UINT32                     TimeOut,
  OUT  UINT32                     *ResponseData
  );

UINT32
MmcGetExtCsd8 (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN UINTN                            Offset
  );

UINT32
MmcGetExtCsd24 (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN UINTN                            Offset
  );

UINT32
MmcGetExtCsd32 (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN UINTN                            Offset
  );

UINTN
MmcGetCurrentPartitionNum (
  IN  PEI_CARD_DATA    *PeiCardData
  );

EFI_STATUS
MmcSelectPartitionNum (
  IN  PEI_CARD_DATA    *PeiCardData,
  IN  UINT8                  Partition
  );

EFI_STATUS
MmcSelectPartition (
  IN  PEI_MMC_PARTITION_DATA     *Partition
  );

EFI_STATUS
MmcReadExtCsd (
  IN  PEI_CARD_DATA    *PeiCardData
              );

EFI_STATUS
MmcMoveToTranState (
  IN  PEI_CARD_DATA    *PeiCardData
                   );

EFI_STATUS
MmcSelect (
  IN  PEI_CARD_DATA    *PeiCardData,
          IN BOOLEAN       Select
          );

EFI_STATUS
MmcSendSwitch (
  IN  PEI_CARD_DATA        *PeiCardData,
  IN  SWITCH_ARGUMENT      *SwitchArgument
  );

EFI_STATUS
MmcGoIdleState (
  IN  PEI_CARD_DATA    *PeiCardData
          );
#endif

