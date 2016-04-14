/** @file
  This file includes a memory call back function notified when MRC is done,
  following action is performed in this file,
    1. ICH initialization after MRC.
    2. SIO initialization.
    3. Install ResetSystem and FinvFv PPI.
    4. Set MTRR for PEI
    5. Create FV HOB and Flash HOB

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "Platform.h"
#include <Ppi/Cache.h>
#include <Library/BaseCryptLib.h>
#include <Guid/PlatformInfo.h>
#include "ChvAccess.h"

/**
  This function will be called when MRC is done.

  @param[in]   PeiServices         General purpose services available to every PEIM.
  @param[in]   NotifyDescriptor    Information about the notify event..
  @param[in]   Ppi                 The notify context.

  @retval EFI_SUCCESS              If the function completed successfully.

**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS       Status;
  EFI_BOOT_MODE    BootMode;
  UINT32           Pages;
  VOID*            Memory;
  UINTN            Size;
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  UINT32                      FvAlignment;

  Status = (*PeiServices)->GetBootMode (
                             (const EFI_PEI_SERVICES **)PeiServices,
                             &BootMode
                             );

  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    //
    // Install Recovery PPI
    //
    Status = InitializeRecovery ((EFI_PEI_SERVICES **)PeiServices);
    ASSERT_EFI_ERROR (Status);

  }

  if (BootMode != BOOT_ON_S3_RESUME) {
    Size =  PcdGet32(PcdFlashFvRecovery2Size);
    if (Size > 0) {
      Pages=  (Size + 0xFFF)/0x1000;

      Memory = AllocatePages ( Pages );
      CopyMem(Memory , (VOID *) PcdGet32(PcdFlashFvRecovery2Base) , Size);

      //
      // We don't verify just load
      //
      PeiServicesInstallFvInfoPpi (
        NULL,
        (VOID *) Memory,
        PcdGet32 (PcdFlashFvRecovery2Size),
        NULL,
        NULL
      );
    }

    if (BootMode != BOOT_IN_RECOVERY_MODE){
      Size =  PcdGet32(PcdFlashFvMainSize);
      Pages=  (Size + 0xFFF)/0x1000;

      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)PcdGet32(PcdFlashFvMainBase);
    
      //
      // Check the FV alignment
      //
      if ((FvHeader->Attributes & EFI_FVB2_WEAK_ALIGNMENT) != EFI_FVB2_WEAK_ALIGNMENT) {
    	//
        // Get FvHeader alignment
        //
        FvAlignment = 1 << ((FvHeader->Attributes & EFI_FVB2_ALIGNMENT) >> 16);
        //
	    // FvAlignment must be greater than or equal to 8 bytes of the minimum FFS alignment value.
	    //
	    if (FvAlignment < 8) {
	      FvAlignment = 8;
	    }
        Memory = AllocateAlignedPages (Pages, FvAlignment);    	
      } else {
    	Memory = AllocatePages ( Pages );    	
      } 

      CopyMem(Memory , (VOID *) PcdGet32(PcdFlashFvMainBase) , Size);

      PeiServicesInstallFvInfoPpi (
        NULL,
        (VOID *) Memory,
        PcdGet32 (PcdFlashFvMainSize),
        NULL,
        NULL
        );
    }
  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    PeiServicesInstallFvInfoPpi (
      NULL,
      (VOID *) (UINTN) (PcdGet32 (PcdFlashFvRecovery2Base)),
      PcdGet32 (PcdFlashFvRecovery2Size),
      NULL,
      NULL
      );
  }

  return EFI_SUCCESS;
}

/**
  PEI termination callback.

  @param[in]  PeiServices          General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor     Not uesed.
  @param[in]  Ppi                  Not uesed.

  @retval  EFI_SUCCESS             If the interface could be successfully
                                   installed.

**/
EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                  Status;
  EFI_BOOT_MODE               BootMode;
  UINTN                       Instance;
  EFI_PEI_FV_HANDLE           VolumeHandle;
  EFI_FV_INFO                 VolumeInfo;
  EFI_PEI_FILE_HANDLE         FileHandle;
  EFI_FV_FILE_INFO            FileInfo;
  UINTN                       FfsCount;
  UINTN                       FvImageCount;

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);

  ASSERT_EFI_ERROR (Status);

  //
  // Check whether FVMAIN is compressed. If not, we will publish fv hob here in Normal Boot..
  //
  //
  // Determine if Main FV is only composed of FV Image files
  //
  if (BootMode != BOOT_ON_S3_RESUME && BootMode != BOOT_IN_RECOVERY_MODE) {
    Instance = 0;
    while (TRUE) {
      //
      // Traverse all firmware volume instances to find FV Main instance
      //
      Status = PeiServicesFfsFindNextVolume (Instance, &VolumeHandle);
      if (EFI_ERROR (Status)) {
        break;
      }
      Status = PeiServicesFfsGetVolumeInfo(VolumeHandle, &VolumeInfo);
      if (EFI_ERROR (Status)) {
        Instance++;
        continue;
      }
    
      if (VolumeInfo.FvSize != PcdGet32 (PcdFlashFvMainSize)) {
        Instance++;
        continue;
      }
    
      //
      // Count total number of FFS files and number of FV Image files in FV Main.
      //
      FileHandle   = NULL;
      FfsCount     = 0;
      FvImageCount = 0;
      while (TRUE) {
        Status = PeiServicesFfsFindNextFile (EFI_FV_FILETYPE_ALL, VolumeHandle, &FileHandle);
        if (EFI_ERROR (Status)) {
          break;
        }
        FfsCount++;
        Status = PeiServicesFfsGetFileInfo (FileHandle, &FileInfo);
        if (EFI_ERROR (Status)) {
          continue;
        }
        if (FileInfo.FileType == EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE) {
          FvImageCount++;
        }
      }
      //
      // If number if FFS files is not the same as the number of FV Image files, 
      // then publish HOB for FV Main so it is available for use in the DXE Phase.
      //
      if (FfsCount != FvImageCount) {
        BuildFvHob (
          (UINTN)VolumeInfo.FvStart,
          VolumeInfo.FvSize
          );
        DEBUG ((EFI_D_INFO, "FVMAIN Fv Hob Built, BaseAddress=0x%x, Size=%x\n", VolumeInfo.FvStart, VolumeInfo.FvSize));
      }
      break;
    }
  }

  return Status;
}
