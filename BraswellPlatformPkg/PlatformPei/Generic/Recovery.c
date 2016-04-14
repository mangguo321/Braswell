/** @file
  This file provides the implementatin of EFI_PEI_DEVICE_RECOVERY_MODULE_PPI,
  it does following
    1. Determine the number of DXE recovery capsules found by each device
    2. Determine capsule information
    3. Load a specific DXE recovery capsule from the indicated device
    4. Determine the device load order
  The capsule is security verified and decomposed and the HOB table is updated 
  with the DXE recovery firmware volume.

  The recovery capsule is determined by 2 factors,
    1. The device search order, if more than one Device Recovery Module PPI 
    was discovered
    2. The individual search order, if the device reported more than one recovery 
    DXE capsule was found generating a search order list.
  
  The 2 orders are decided by the RecoveryOemHook library function OemRecoveryRankCapsule().  

  The security check and error handling is done by RecoveryOemHook library function 
  OemRecoverySecurityCheck()

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include "CommonHeader.h"

EFI_STATUS
EFIAPI
PlatformRecoveryModule (
  IN EFI_PEI_SERVICES                     **PeiServices,
  IN EFI_PEI_RECOVERY_MODULE_PPI          *This
  );

STATIC EFI_PEI_RECOVERY_MODULE_PPI mRecoveryPpi = {
  PlatformRecoveryModule
};

STATIC EFI_PEI_PPI_DESCRIPTOR mRecoveryPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiRecoveryModulePpiGuid,
    &mRecoveryPpi
  }
};

/**
  This function initialize recovery functionality by installing the recovery PPI.
  
  @param  PeiServices General purpose services available to every PEIM. 

  @retval EFI_SUCCESS if the interface could be successfully installed.
**/
EFI_STATUS
EFIAPI
InitializeRecovery (
  IN EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  Status = PeiServicesInstallPpi (mRecoveryPpiList);

  return Status;
}

/**
  Loads a DXE capsule from some media into memory and updates the HOB table
  with the DXE firmware volume information.

  @param  PeiServices   General-purpose services that are available to every PEIM.
  @param  This          Indicates the EFI_PEI_RECOVERY_MODULE_PPI instance.

  @retval EFI_SUCCESS        The capsule was loaded correctly.
  @retval EFI_DEVICE_ERROR   A device error occurred.
  @retval EFI_NOT_FOUND      A recovery DXE capsule cannot be found.

**/
EFI_STATUS
EFIAPI
PlatformRecoveryModule (
  IN EFI_PEI_SERVICES                     **PeiServices,
  IN EFI_PEI_RECOVERY_MODULE_PPI          *This
  )
{
  EFI_STATUS                             Status;
  EFI_PEI_DEVICE_RECOVERY_MODULE_PPI     *DeviceRecoveryModule;
  UINTN                                  RecoveryCapsuleSize;
  EFI_GUID                               DeviceId;
  EFI_PHYSICAL_ADDRESS                   Address;
  VOID                                   *Buffer;
  EFI_PEI_HOB_POINTERS                   Hob;
  EFI_PEI_HOB_POINTERS                   HobOld;
  BOOLEAN                                HobUpdate;
  EFI_FIRMWARE_VOLUME_HEADER             *FvHeader;
  UINTN                                  DeviceInstance;
  UINTN                                  CapsuleInstance;
  
  DeviceInstance = 0;
  CapsuleInstance = 1;
  RecoveryCapsuleSize = 0;  
  Status = EFI_NOT_FOUND;
  HobUpdate = FALSE;

  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Entry\n"));
  
  //
  // Search the platform for some recovery capsule if the DXE IPL
  // discovered a recovery condition and has requested a load.
  //
  while (OemRecoveryRankCapsule(
           &DeviceInstance,
           &CapsuleInstance,
           &DeviceRecoveryModule,
           &Buffer
           )) {
        
    DEBUG ((
      EFI_D_INFO | EFI_D_LOAD, 
      "Recovery Capsule ranked DeviceInstance = %x CapsuleInstance = %x\n", 
      DeviceInstance, 
      CapsuleInstance
      ));        

    if (Buffer == NULL) {
    
      //
      // The OemRecoveryRankCapsule() does not reurn a capsule, load it by CapsuleInstance.
      //  
      
      if (DeviceRecoveryModule == NULL) {      
        
        //
        // The OemRecoveryRankCapsule() does not reurn a devide PPI, load it by DeviceInstance.
        //
        Status = PeiServicesLocatePpi (
                   &gEfiPeiDeviceRecoveryModulePpiGuid,
                   DeviceInstance,
                   NULL,
                   (VOID **) &DeviceRecoveryModule
                   );
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Randked Device Recovery PPI not located\n"));
          DeviceInstance ++;
          continue;
        }
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Device Recovery PPI located\n"));
      }

      Status = DeviceRecoveryModule->GetRecoveryCapsuleInfo (
                                       PeiServices,
                                       DeviceRecoveryModule,
                                       CapsuleInstance,
                                       &RecoveryCapsuleSize,
                                       &DeviceId
                                       );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Unknown Recovery Capsule Size\n"));
        CapsuleInstance ++;
        continue;
      }
      DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Capsule Size: %d\n", RecoveryCapsuleSize));

      DEBUG((EFI_D_INFO, 
        "PcdRecoveryOnIdeDisk = %x \n", 
        FeaturePcdGet (PcdRecoveryOnIdeDisk)
        ));
      DEBUG((EFI_D_INFO, 
        "PcdRecoveryOnFatFloppyDisk = %x \n", 
        FeaturePcdGet (PcdRecoveryOnFatFloppyDisk)
        ));
      DEBUG((EFI_D_INFO, 
        "PcdRecoveryOnDataCD = %x \n", 
        FeaturePcdGet (PcdRecoveryOnDataCD)
        ));
      DEBUG((EFI_D_INFO, 
        "PcdRecoveryOnFatUsbDisk = %x \n", 
        FeaturePcdGet (PcdRecoveryOnFatUsbDisk)
        ));
      //
      // Detect whether it's a supported media
      //
      if ((!FeaturePcdGet (PcdRecoveryOnIdeDisk) || 
           !CompareGuid (&DeviceId, &gRecoveryOnFatIdeDiskGuid)) &&
          (!FeaturePcdGet (PcdRecoveryOnFatFloppyDisk) || 
           !CompareGuid (&DeviceId, &gRecoveryOnFatFloppyDiskGuid)) &&
          (!FeaturePcdGet (PcdRecoveryOnDataCD) || 
           !CompareGuid (&DeviceId, &gRecoveryOnDataCdGuid)) &&
          (!FeaturePcdGet (PcdRecoveryOnFatUsbDisk) || 
           !CompareGuid (&DeviceId, &gRecoveryOnFatUsbDiskGuid))
          ) { 
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery media not supported\n")); 
        CapsuleInstance ++;           
        continue;
      }
           
      Buffer = NULL;
      
      DEBUG ((EFI_D_INFO | EFI_D_LOAD, "AllocatePage Pagess: %x\n", EFI_SIZE_TO_PAGES (RecoveryCapsuleSize)));        
      
      Status = PeiServicesAllocatePages (
                 EfiBootServicesCode,
                 EFI_SIZE_TO_PAGES (RecoveryCapsuleSize),
                 &Address
                 );
      DEBUG ((EFI_D_INFO | EFI_D_LOAD, "AllocatePage Returns: %r\n", Status));                 
      if (EFI_ERROR (Status)) {       
        CapsuleInstance ++;        
        continue;
      }
 
      
      Buffer = (UINT8*)(UINTN)Address;
      Status = DeviceRecoveryModule->LoadRecoveryCapsule (
                                       PeiServices,
                                       DeviceRecoveryModule,
                                       CapsuleInstance,
                                       Buffer
                                       );
      DEBUG ((EFI_D_INFO | EFI_D_LOAD, "LoadRecoveryCapsule Returns: %r\n", Status));
      if (EFI_ERROR (Status)) {
        CapsuleInstance ++;        
        continue;
      } 
    } // end of (Buffer == NULL)

    if (!OemRecoverySecurityCheck (Buffer)) {
      CapsuleInstance ++;      
      continue;
    }

    //
    // Update FV Hob if found
    //
    Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
    HobOld.Raw = Hob.Raw;
    while (!END_OF_HOB_LIST (Hob)) {
      if (Hob.Header->HobType == EFI_HOB_TYPE_FV) {
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Hob FV Length: %x\n", Hob.FirmwareVolume->Length));    

        if (Hob.FirmwareVolume->BaseAddress == (UINTN) PcdGet32 (PcdFlashFvMainBase)) {
          HobUpdate = TRUE;
          //
          // This looks like the Hob we are interested in
          //
          DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Hob Updated\n"));  
          Hob.FirmwareVolume->BaseAddress = (UINTN)Buffer;
          Hob.FirmwareVolume->Length = RecoveryCapsuleSize;
         
        }
      }  
      Hob.Raw = GET_NEXT_HOB (Hob);
    }
    
    //
    // Check if the top of the file is a firmware volume header
    //
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)Buffer;
    if (FvHeader->Signature== EFI_FVH_SIGNATURE) {
      //
      // build FV Hob if it is not built before
      //
      if (!HobUpdate) {
        DEBUG ((EFI_D_INFO | EFI_D_LOAD, "FV Hob is not found, Build FV Hob then..\n" ));

        BuildFvHob (
          (EFI_PHYSICAL_ADDRESS) (UINTN) Buffer,
          (UINT32) FvHeader->FvLength
          );
      }

      PeiServicesInstallFvInfoPpi (
        NULL,
        Buffer,
        (UINT32) FvHeader->FvLength,
        NULL,
        NULL
        );
    }

    //
    // Capsule is loaded and HOB list is updated, done.
    //
    DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Capsule is loaded: %r\n", Status));    
    
    break;
  } // end of while
  
  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Recovery Module Returning: %r\n", Status));
  return Status;
}



