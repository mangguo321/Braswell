/** @file
  The implementation of functions by Pci Platform Driver

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PciPlatform.h"
#include "PchRegs.h"
#include "PchAccess.h"
#include "ChvCommonDefinitions.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/CpuIo.h>
#include <Protocol/PciIo.h>
#include <Guid/SetupVariable.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/FirmwareVolume.h>
#include <Library/HobLib.h>
#include <IndustryStandard/Pci22.h>

extern  PCI_OPTION_ROM_TABLE  mPciOptionRomTable[];
extern  UINTN                 mSizeOptionRomTable;

EFI_PCI_PLATFORM_PROTOCOL mPciPlatform = {
  PhaseNotify,
  PlatformPrepController,
  GetPlatformPolicy,
  GetPciRom
};

EFI_HANDLE mPciPlatformHandle = NULL;

SYSTEM_CONFIGURATION          mSystemConfiguration;

VOID       *mPciRegistration;

/**
  Standard Pci Bus event notification 
  @param[in]   Event         the event that is signaled.
  @param[in]   Context       not used here.

**/
VOID
EFIAPI
PciBusEvent (
  IN EFI_EVENT    Event,
  IN VOID*        Context
  );

EFI_STATUS
PciBusDriverHook (
  )
{
  EFI_STATUS                Status;
  EFI_EVENT                 FilterEvent;

  //
  // Register for callback to PCI I/O protocol
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PciBusEvent,
                  NULL,
                  &FilterEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register for protocol notifications on this event
  //
  Status = gBS->RegisterProtocolNotify (
                  &gEfiPciIoProtocolGuid,
                  FilterEvent,
                  &mPciRegistration
                  );
  ASSERT_EFI_ERROR (Status);

  return  EFI_SUCCESS;
}

VOID
ProgramPciLatency (
  IN    EFI_PCI_IO_PROTOCOL           *PciIo
  )
{
  EFI_STATUS                          Status;

  //
  // Program Master Latency Timer
  //  
  if (mSystemConfiguration.PciLatency != 0) {
     Status = PciIo->Pci.Write (
                           PciIo,
                           EfiPciIoWidthUint8,
                           PCI_LATENCY_TIMER_OFFSET,
                           1,
                           &mSystemConfiguration.PciLatency
                           );
  }

  return;
}

/**
  Standard Pci Bus event notification 
  @param[in]   Event         the event that is signaled.
  @param[in]   Context       not used here.

**/
VOID
EFIAPI
PciBusEvent (
  IN EFI_EVENT    Event,
  IN VOID*        Context
  )
{

  EFI_STATUS                    Status;
  UINTN                         BufferSize;
  EFI_HANDLE                    Handle;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT64                        Supports;  
  UINT8                         mCacheLineSize = 0x10;
  UINT16                        VendorId;

  while (TRUE) {
    BufferSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mPciRegistration,
                    &BufferSize,
                    &Handle
                    );
    if (EFI_ERROR (Status)) {
      //
      // If no more notification events exist
      //
      return;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiPciIoProtocolGuid,
                    &PciIo
                    );

    PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, 0, 1, &VendorId);
    //
    // Enable I/O for bridge so port 0x80 codes will come out
    //
    if (VendorId == V_PCH_INTEL_VENDOR_ID) {
      Status = PciIo->Attributes(
                        PciIo,
                        EfiPciIoAttributeOperationSupported,
                        0,
                        &Supports
                        );
      Supports &= EFI_PCI_DEVICE_ENABLE;
      Status = PciIo->Attributes (
                        PciIo, 
                        EfiPciIoAttributeOperationEnable,
                        Supports,
                        NULL
                        );
      break;
    }

    //
    // Program PCI Latency Timer
    //
    ProgramPciLatency(PciIo);

    //
    // Program Cache Line Size to 64 bytes (0x10 DWORDs)
    // 
    Status = PciIo->Pci.Write (
                          PciIo,
                          EfiPciIoWidthUint8,
                          PCI_CACHELINE_SIZE_OFFSET,
                          1,
                          &mCacheLineSize
                          );
  }

  return;
}

EFI_STATUS
EFIAPI    
PhaseNotify (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE  Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL       *This,
  OUT EFI_PCI_PLATFORM_POLICY               *PciPolicy
  )
{
  *PciPolicy = EFI_RESERVE_VGA_IO_ALIAS;

  return EFI_SUCCESS;
}

/**
  GetPciRom from platform specific location for specific PCI device

  @param[in] This           Protocol instance
  @param[in] PciHandle      Identify the specific PCI devic
  @param[out] RomImage      Returns the ROM Image memory location
  @param[out] RomSize       Returns Rom Image size

  @retval EFI_SUCCESS
  @retval EFI_NOT_FOUND
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI    
GetPciRom (
  IN   CONST EFI_PCI_PLATFORM_PROTOCOL    *This,
  IN   EFI_HANDLE                         PciHandle,
  OUT  VOID                               **RomImage,
  OUT  UINTN                              *RomSize
  )
{
  return EFI_NOT_FOUND;
}

/**
  The Entry Point for Pci Platform driver.
  
  @param[in] ImageHandle       A handle for the image that is initializing this driver
  @param[in] SystemTable       A pointer to the EFI system table
  
  @retval    EFI_SUCCESS       The driver is executed successfully.
  @retval    Other value       Some error occurs when executing this driver.

**/
EFI_STATUS
EFIAPI
PciPlatformDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  
  CopyMem (&mSystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  //
  // Install on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &mPciPlatformHandle,
                  &gEfiPciPlatformProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPciPlatform
                  );
  
  //
  // Install PCI Bus Driver Hook
  //
  PciBusDriverHook ();

  return Status;
}

