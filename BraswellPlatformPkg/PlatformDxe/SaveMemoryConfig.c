/** @file
  This is the driver that locates the MemoryConfigurationData HOB, if it
  exists, and saves the data to NVRAM.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SaveMemoryConfig.h"
#include "PolicyInitDxe.h"

CHAR16    EfiMemoryConfigVariable[] = L"MemoryConfig";

/**
  This is the standard EFI driver that detects whether there is a
  MemoryConfigurationData HOB and, if so, saves its data to nvRAM.

  @return  EFI_SUCCESS     The data is successfully saved or there was no data
  @return  EFI_NOT_FOUND   The HOB list could not be located.
  @return  EFI_UNLOAD_IMAGE It is not success.

**/
EFI_STATUS
EFIAPI
SaveMemoryConfigEntryPoint (
    VOID
  )
{
  EFI_STATUS                     Status = EFI_SUCCESS;
  VOID                           *HobList;
  VOID                           *HobData=NULL;
  VOID                           *VariableData;
  UINTN                          DataSize = 0;
  UINTN                          BufferSize = 0;
  BOOLEAN                        SaveConfig;
  EFI_PLATFORM_INFO_HOB          *PlatformInfoHobPtr = NULL;
  EFI_HANDLE                     Handle;

  DataSize      = 0;
  VariableData  = NULL;
  SaveConfig    = FALSE;
  Handle        = NULL;

  //
  // Get Platform Info HOB
  //
  HobList = GetHobList ();
  if (HobList == NULL) {
    Status = EFI_NOT_FOUND;
    ASSERT_EFI_ERROR (Status);
    return RETURN_LOAD_ERROR;
  }
  
  if ((HobList = GetNextGuidHob (&gEfiPlatformInfoGuid, HobList)) != NULL) {
    PlatformInfoHobPtr = GET_GUID_HOB_DATA (HobList);
  }

  //
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);  //EfiLibGetSystemConfigurationTable
  ASSERT_EFI_ERROR (Status);

  if ((HobList = GetNextGuidHob (&gFspNonVolatileStorageHobGuid, HobList)) != NULL) {
    HobData = GET_GUID_HOB_DATA (HobList);
    DataSize = GET_GUID_HOB_DATA_SIZE(HobList);
  }

  if (HobData == NULL) {
    Status = EFI_NOT_FOUND;
    DEBUG((EFI_D_ERROR,"HobData == NULL\n"));
  }


  if (!EFI_ERROR (Status) && NULL != HobData ) {

    //
    // TODO: Get MRC Parmater data sturcture size from HOB.
    //
    BufferSize = 0x6C58;

    Status = gRT->GetVariable (
                    EfiMemoryConfigVariable,
                    &gEfiChvVariableGuid,
                    NULL,
                    &BufferSize,
                    VariableData
                    );
    //
    // The Memory Configuration will not exist during first boot or after bios updated
    // with MRC/memory config changes
    //
    if (EFI_ERROR(Status)) {
      SaveConfig = TRUE;
    } else {
      //
      // Do a compare to see if there is a need to save the configuration
      //
      if (CompareMem (HobData, VariableData, BufferSize)) {  //EfiCompareMem
        SaveConfig = TRUE;
      }
    } 
    
    if (SaveConfig) {
      DEBUG((EFI_D_ERROR, "Saving Memory Config to NV RAM\n"));
      Status = gRT->SetVariable (
                      EfiMemoryConfigVariable,
                      &gEfiChvVariableGuid,
                      (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
                      DataSize,
                      HobData
                      );
    } 

    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR,"Save Memory Config to NV RAM failed\n"));
    }
    
  }

  //
  // This driver does not produce any protocol services, so always unload it.
  //
  return RETURN_LOAD_ERROR;
}
