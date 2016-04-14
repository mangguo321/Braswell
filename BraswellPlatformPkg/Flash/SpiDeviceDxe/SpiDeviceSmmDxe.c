/** @file
  Provides an interface to the SMM SPI Device driver.

      gSpiDeviceProtocolGuid (DXE: SpiDeviceSmmDxe)
          |
          |  via gEfiSmmCommunicationProtocolGuid 
          V
      gSmmSpiDeviceProtocolGuid (SMM: SpiDeviceSmm)
          |
          |
          V
      gEfiSmmSpi2ProtocolGuid (SMM: SpiSmm)
  
  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Protocol/SpiDevice.h>
#include <Protocol/SmmSpiDevice.h>
#include <Protocol/SmmCommunication.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include "SpiDevice.h"
#include "SpiDeviceSmmComm.h"

EFI_SMM_COMMUNICATION_PROTOCOL  *mSmmComm = NULL;

SPI_DEVICE_PROTOCOL mSpiDevProtocol = {
  SpiRead,
  SpiWrite,
  SpiErase,
  SpiLock,
  SpiSetRange,
  SpiLockRanges
};

VOID
EFIAPI
SmmSpiDeviceReady (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  );

EFI_STATUS
CreateCommBuffer (
  OUT   VOID      **CommBuffer,
  OUT   VOID      **DataArea,
  IN    UINTN     DataSize,
  IN    UINTN     Function
  )
{
  EFI_SMM_COMMUNICATE_HEADER                *SmmCommunicateHeader;
  SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER   *SmmSpiDevFunctionHeader;

  //
  // Allocate communication buffer.
  //
  SmmCommunicateHeader = AllocatePool (DataSize + SMM_COMMUNICATE_HEADER_SIZE + SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER_SIZE);
  if (SmmCommunicateHeader == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Fill in new structure will data from caller.
  //
  CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gSmmSpiDeviceProtocolGuid);
  SmmCommunicateHeader->MessageLength = DataSize + SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER_SIZE;
  SmmSpiDevFunctionHeader = (SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER*) SmmCommunicateHeader->Data;
  SmmSpiDevFunctionHeader->Function = Function;

  //
  // Assign return values.
  //
  *CommBuffer = SmmCommunicateHeader;
  if (DataArea != NULL) {
    *DataArea = SmmSpiDevFunctionHeader->Data;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SendCommBuffer (
  IN OUT    EFI_SMM_COMMUNICATE_HEADER    *SmmCommunicateHeader,
  IN        UINTN                         DataSize
  )
{
  EFI_STATUS                                Status;
  UINTN                                     CommSize;
  SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER   *SmmSpiDevFunctionHeader;

  //
  // Compute actual size of communication data.
  //
  CommSize = DataSize + SMM_COMMUNICATE_HEADER_SIZE + SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER_SIZE;

  //
  // Send the message to be processed in SMM.
  //
  Status = mSmmComm->Communicate (mSmmComm, SmmCommunicateHeader, &CommSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the return value from the SMM function.
  //
  SmmSpiDevFunctionHeader = (SMM_SPI_DEV_COMMUNICATE_FUNCTION_HEADER*) SmmCommunicateHeader->Data;

  return SmmSpiDevFunctionHeader->ReturnStatus;
}

EFI_STATUS
EFIAPI
SpiRead (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       *Size,
  OUT     UINT8       *Buffer
  )
{
  EFI_STATUS                            Status;
  UINTN                                 DataSize;
  EFI_SMM_COMMUNICATE_HEADER            *SmmCommunicateHeader;
  SMM_SPI_DEV_READ_WRITE_ERASE_HEADER   *SpiDevReadHeader;

  //
  // Validate input parameters.
  //
  if (Size == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Determine the actual data size required for the transaction.
  //
  DataSize = *Size + sizeof(SMM_SPI_DEV_READ_WRITE_ERASE_HEADER);

  //
  // Create the communication buffer.
  //
  Status = CreateCommBuffer ((VOID**) &SmmCommunicateHeader, (VOID**) &SpiDevReadHeader, DataSize, SPI_DEV_FUNCTION_READ);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Fill in communication buffer parameters.
  //
  SpiDevReadHeader->Offset = SpiOffset;
  SpiDevReadHeader->Size = *Size;

  //
  // Communicate request to SMM driver and fill in return values.
  //
  Status = SendCommBuffer (SmmCommunicateHeader, DataSize);
  *Size = SpiDevReadHeader->Size;
  if (!EFI_ERROR (Status)) {
    CopyMem (Buffer, (UINT8*)(SpiDevReadHeader + 1), *Size);
  }

  return Status;
}

EFI_STATUS
EFIAPI
SpiWrite (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       *Size,
  IN      UINT8       *Buffer
  )
{
  EFI_STATUS                            Status;
  UINTN                                 DataSize;
  EFI_SMM_COMMUNICATE_HEADER            *SmmCommunicateHeader;
  SMM_SPI_DEV_READ_WRITE_ERASE_HEADER   *SpiDevWriteHeader;

  //
  // Validate input parameters.
  //
  if (Size == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Determine the actual data size required for the transaction.
  //
  DataSize = *Size + sizeof(SMM_SPI_DEV_READ_WRITE_ERASE_HEADER);

  //
  // Create the communication buffer.
  //
  Status = CreateCommBuffer ((VOID**) &SmmCommunicateHeader, (VOID**) &SpiDevWriteHeader, DataSize, SPI_DEV_FUNCTION_WRITE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Fill in communication buffer parameters.
  //
  SpiDevWriteHeader->Offset = SpiOffset;
  SpiDevWriteHeader->Size = *Size;
  CopyMem ((UINT8*)(SpiDevWriteHeader + 1), Buffer, *Size);

  //
  // Communicate request to SMM driver and fill in return values.
  //
  Status = SendCommBuffer (SmmCommunicateHeader, DataSize);
  *Size = SpiDevWriteHeader->Size;

  return Status;
}

EFI_STATUS
EFIAPI
SpiErase (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       Size
  )
{
  EFI_STATUS                            Status;
  UINTN                                 DataSize;
  EFI_SMM_COMMUNICATE_HEADER            *SmmCommunicateHeader;
  SMM_SPI_DEV_READ_WRITE_ERASE_HEADER   *SpiDevEraseHeader;

  //
  // Determine the actual data size required for the transaction.
  //
  DataSize = sizeof(SMM_SPI_DEV_READ_WRITE_ERASE_HEADER);

  //
  // Create the communication buffer.
  //
  Status = CreateCommBuffer ((VOID**) &SmmCommunicateHeader, (VOID**) &SpiDevEraseHeader, DataSize, SPI_DEV_FUNCTION_ERASE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Fill in communication buffer parameters.
  //
  SpiDevEraseHeader->Offset = SpiOffset;
  SpiDevEraseHeader->Size = Size;

  //
  // Communicate request to SMM driver and fill in return values.
  //
  Status = SendCommBuffer (SmmCommunicateHeader, DataSize);

  return Status;
}

EFI_STATUS
EFIAPI
SpiLock (
  IN      UINTN       SpiOffset,
  IN OUT  UINTN       Size,
  IN      BOOLEAN     Lock
  )
{
  EFI_STATUS                            Status;
  UINTN                                 DataSize;
  EFI_SMM_COMMUNICATE_HEADER            *SmmCommunicateHeader;
  SMM_SPI_DEV_LOCK_HEADER               *SmmSpiDevLockHeader;

  //
  // Compute data size required for the transaction.
  //
  DataSize = sizeof(SMM_SPI_DEV_LOCK_HEADER);

  //
  // Create the communication buffer.
  //
  Status = CreateCommBuffer ((VOID**) &SmmCommunicateHeader, (VOID**) &SmmSpiDevLockHeader, DataSize, SPI_DEV_FUNCTION_LOCK);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Fill in communication buffer parameters.
  //
  SmmSpiDevLockHeader->Offset = SpiOffset;
  SmmSpiDevLockHeader->Size = Size;
  SmmSpiDevLockHeader->Lock = Lock;

  //
  // Communicate request to SMM driver and fill in return values.
  //
  Status = SendCommBuffer (SmmCommunicateHeader, DataSize);

  return Status;
}

EFI_STATUS
EFIAPI
SpiSetRange (
  IN      UINTN       SpiOffset,
  IN      UINTN       Size,
  IN      BOOLEAN     ReadLock,
  IN      BOOLEAN     WriteLock
  )
{
  EFI_STATUS                            Status;
  UINTN                                 DataSize;
  EFI_SMM_COMMUNICATE_HEADER            *SmmCommunicateHeader;
  SMM_SPI_DEV_SET_RANGE_HEADER          *SmmSpiDevSetRangeHeader;

  //
  // Compute data size required for the transaction.
  //
  DataSize = sizeof(SMM_SPI_DEV_SET_RANGE_HEADER);

  //
  // Create the communication buffer.
  //
  Status = CreateCommBuffer ((VOID**) &SmmCommunicateHeader, (VOID**) &SmmSpiDevSetRangeHeader, DataSize, SPI_DEV_FUNCTION_SET_RANGE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Fill in communication buffer parameters.
  //
  SmmSpiDevSetRangeHeader->Offset = SpiOffset;
  SmmSpiDevSetRangeHeader->Size = Size;
  SmmSpiDevSetRangeHeader->ReadLock = ReadLock;
  SmmSpiDevSetRangeHeader->WriteLock = WriteLock;

  //
  // Communicate request to SMM driver and fill in return values.
  //
  Status = SendCommBuffer (SmmCommunicateHeader, DataSize);

  return Status;
}

EFI_STATUS
EFIAPI
SpiLockRanges (
  )
{
  EFI_STATUS                            Status;
  EFI_SMM_COMMUNICATE_HEADER            *SmmCommunicateHeader;

  //
  // Create the communication buffer.
  //
  Status = CreateCommBuffer ((VOID**) &SmmCommunicateHeader, NULL, 0, SPI_DEV_FUNCTION_LOCK_RANGES);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Communicate request to SMM driver and fill in return values.
  //
  Status = SendCommBuffer (SmmCommunicateHeader, 0);

  return Status;
}

EFI_STATUS
EFIAPI
InitSpiDevice (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  VOID      *SmmSpiDeviceReg;

  //
  // Register for a callback when the SMM version of the SPI Device protocol
  // is installed.
  //
  EfiCreateProtocolNotifyEvent (
    &gSmmSpiDeviceProtocolGuid,
    TPL_CALLBACK,
    SmmSpiDeviceReady,
    NULL,
    &SmmSpiDeviceReg
    );

  return EFI_SUCCESS;
}

VOID
EFIAPI
SmmSpiDeviceReady (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_HANDLE                    Handle;
  SPI_DEVICE_PROTOCOL           *SmmSpiDevice;
  EFI_STATUS                    Status;

  //
  // Locate the protocol first just to make sure it was actually installed.
  //
  Status = gBS->LocateProtocol (
                  &gSmmSpiDeviceProtocolGuid,
                  NULL,
                  (VOID **) &SmmSpiDevice
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // SMM Service installed so get communication link to SMM
  //
  Status = gBS->LocateProtocol (
                  &gEfiSmmCommunicationProtocolGuid,
                  NULL,
                  (VOID **) &mSmmComm
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Install DXE protocol so it can be used by drivers.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gSpiDeviceProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mSpiDevProtocol
                  );
  ASSERT_EFI_ERROR (Status);
}
