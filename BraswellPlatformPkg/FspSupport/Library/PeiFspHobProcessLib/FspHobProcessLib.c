/** @file
  Null instance of Platform Sec Lib.

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/FspPlatformInfoLib.h>
#include <Guid/GuidHobFsp.h>
#include <Guid/MemoryTypeInformation.h>
#include <Ppi/Capsule.h>
#include <Guid/SmramMemoryReserve.h>
#include <Guid/GuidHobFspEas.h>
#include <Guid/AcpiS3Context.h>
#include <Guid/VariableFormat.h>
#include <Guid/GraphicsInfoHob.h>
#include "VpdHeader.h"
#include <ReservedAcpiS3Range.h>
#include <Guid/PlatformInfo.h>

extern EFI_GUID gFspSmbiosMemoryInfoHobGuid;

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

#include <CpuRegs.h>
#include <Guid/PlatformCpuInfo.h>
EFI_GUID gFspReservedMemoryResourceHobTsegGuid = {0xd038747c, 0xd00c, 0x4980, {0xb3, 0x19, 0x49, 0x01, 0x99, 0xa4, 0x7d, 0x55}};
EFI_GUID gEfiMemoryConfigDataGuid              = { 0x80dbd530, 0xb74c, 0x4f11, {0x8c, 0x03, 0x41, 0x86, 0x65, 0x53, 0x28, 0x31}};
EFI_GUID gDummyGuid              = { 0x00000000, 0xb74c, 0x4f11, {0x8c, 0x03, 0x41, 0x86, 0x65, 0x53, 0x28, 0x31}};

EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  { EfiACPIReclaimMemory,       0x2B  },    // ASL
  { EfiACPIMemoryNVS,           0xCF  },    // ACPI NVS
  { EfiReservedMemoryType,      0xC8  },    // BIOS Reserved
  { EfiRuntimeServicesCode,     0x39  },
  { EfiRuntimeServicesData,     0x50  },
  { EfiMaxMemoryType,           0     }
};

//
// Additional pages are used by DXE memory manager.
// It should be consistent between RetrieveRequiredMemorySize() and GetPeiMemSize()
//
#define PEI_ADDITIONAL_MEMORY_SIZE    (16 * EFI_PAGE_SIZE)

EFI_STATUS
PlatformHobCreateFromFsp (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  VOID                          *HobList
  )
{
  return EFI_SUCCESS;
}

/**
  Get the mem size in memory type infromation table.

  @param[in] PeiServices  PEI Services table.

  @return                 the mem size in memory type infromation table.
**/
UINT64
GetMemorySizeInMemoryTypeInformation (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_HOB_POINTERS        Hob;
  EFI_MEMORY_TYPE_INFORMATION *MemoryData;
  UINT8                       Index;
  UINTN                       TempPageNum;

  MemoryData = NULL;

  Status     = (*PeiServices)->GetHobList (PeiServices, (VOID **) &Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
      CompareGuid (&Hob.Guid->Name, &gEfiMemoryTypeInformationGuid)) {
      MemoryData = (EFI_MEMORY_TYPE_INFORMATION *) (Hob.Raw + sizeof (EFI_HOB_GENERIC_HEADER) + sizeof (EFI_GUID));
      break;
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  if (MemoryData == NULL) {
    return 0;
  }

  TempPageNum = 0;
  for (Index = 0; MemoryData[Index].Type != EfiMaxMemoryType; Index++) {
    //
    // Accumulate default memory size requirements
    //
    TempPageNum += MemoryData[Index].NumberOfPages;
  }

  return TempPageNum * EFI_PAGE_SIZE;
}

/**
  Get the mem size need to be reserved in PEI phase.

  @param[in] PeiServices  PEI Services table.

  @return                 the mem size need to be reserved in PEI phase.
**/
UINT64
RetrieveRequiredMemorySize (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  UINT64                      Size;

  Size = GetMemorySizeInMemoryTypeInformation (PeiServices);

  return Size + PEI_ADDITIONAL_MEMORY_SIZE;
}

/**
  Get the mem size need to be consumed and reserved in PEI phase.

  @param[in] PeiServices  PEI Services table.
  @param[in] BootMode     Current boot mode.

  @return                 the mem size need to be consumed and reserved in PEI phase.
**/
UINT64
GetPeiMemSize (
  IN EFI_PEI_SERVICES **PeiServices,
  IN UINT32           BootMode
  )
{
  UINT64                      Size;
  UINT64                      MinSize;

  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    return PcdGet32 (PcdPeiRecoveryMinMemSize);
  }

  Size = GetMemorySizeInMemoryTypeInformation (PeiServices);

  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    //
    // Maybe more size when in CapsuleUpdate phase ?
    //
    MinSize = PcdGet32 (PcdPeiMinMemSize);
  } else {
    MinSize = PcdGet32 (PcdPeiMinMemSize);
  }

  return MinSize + Size + PEI_ADDITIONAL_MEMORY_SIZE;
}

EFI_STATUS
GetS3AcpiReservedMemory (
  IN  EFI_PHYSICAL_ADDRESS  TsegBase,
  OUT EFI_PHYSICAL_ADDRESS  *S3PeiMemBase,
  OUT UINT64                *S3PeiMemSize 
)
{
  RESERVED_ACPI_S3_RANGE  *AcpiS3Range;

  AcpiS3Range = (RESERVED_ACPI_S3_RANGE*) (UINTN) (TsegBase + RESERVED_ACPI_S3_RANGE_OFFSET);
  *S3PeiMemBase = (EFI_PHYSICAL_ADDRESS)(AcpiS3Range->AcpiReservedMemoryBase);  
  *S3PeiMemSize = (UINT64) (AcpiS3Range->AcpiReservedMemorySize);

  return EFI_SUCCESS;
}

/**
  BIOS process FspBobList.

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return                If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
FspHobProcessForMemoryResource (
  IN VOID                 *FspHobList
  )
{
  EFI_PEI_HOB_POINTERS Hob;
  EFI_PEI_HOB_POINTERS PeiMemoryHob;
  UINT64               FspMemorySize;
  EFI_PHYSICAL_ADDRESS FspMemoryBase;
  UINT64               RequiredMemSize;
  BOOLEAN              FoundFspMemHob;
  EFI_STATUS           Status;
  EFI_BOOT_MODE        BootMode;
  EFI_PEI_SERVICES     **PeiServices;
  UINT64               TsegSize;
  EFI_PHYSICAL_ADDRESS TsegBase;
  BOOLEAN              FoundTsegHob;
  UINT64               PeiMemSize;
  EFI_PHYSICAL_ADDRESS PeiMemBase;
  PEI_CAPSULE_PPI      *Capsule;
  VOID                 *CapsuleBuffer;
  UINTN                CapsuleBufferLength;


  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  PeiServicesGetBootMode (&BootMode);

  PeiMemoryHob.ResourceDescriptor = NULL;
  PeiMemBase          = BASE_512MB;
  PeiMemSize          = BASE_256MB;
  RequiredMemSize     = BASE_128MB;   // initial DXE required memory
  FspMemorySize       = 0;
  FspMemoryBase       = 0;
  FoundFspMemHob      = FALSE;
  TsegSize            = 0;
  TsegBase            = 0;
  FoundTsegHob        = FALSE;
  CapsuleBufferLength = 0;
  CapsuleBuffer       = NULL;
  Capsule             = NULL;

  if (BootMode == BOOT_ON_S3_RESUME) {
   Status = PeiServicesLocatePpi (
              &gPeiCapsulePpiGuid,
              0,
              NULL,
              (VOID **) &Capsule
              );
   if (!EFI_ERROR(Status)) {
     Status = Capsule->CheckCapsuleUpdate(PeiServices);
     //
     // Capsule exists, switch boot mode to Flash Update
     //
     if (Status == EFI_SUCCESS) {
       BootMode = BOOT_ON_FLASH_UPDATE;
       PeiServicesSetBootMode(BootMode);
     }
   }
  }

  //
  // Parse the hob list from fsp
  // Report all the memory resource hob.
  //
  Hob.Raw = (UINT8 *)(UINTN)FspHobList;
  DEBUG((DEBUG_INFO, "FspHobList - 0x%x\n", FspHobList));

  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    DEBUG((DEBUG_INFO, "\nResourceType: 0x%x\n", Hob.ResourceDescriptor->ResourceType));
    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) ||
        (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)) {
      DEBUG((DEBUG_INFO, "ResourceAttribute: 0x%x\n", Hob.ResourceDescriptor->ResourceAttribute));
      DEBUG((DEBUG_INFO, "PhysicalStart: 0x%lx\n", Hob.ResourceDescriptor->PhysicalStart));
      DEBUG((DEBUG_INFO, "ResourceLength: 0x%lx\n", Hob.ResourceDescriptor->ResourceLength));
      DEBUG((DEBUG_INFO, "Owner: %g\n\n", &Hob.ResourceDescriptor->Owner));
    }

    if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
      BuildResourceDescriptorHob (
        Hob.ResourceDescriptor->ResourceType,
        Hob.ResourceDescriptor->ResourceAttribute,
        Hob.ResourceDescriptor->PhysicalStart,
        Hob.ResourceDescriptor->ResourceLength
        );

      if ((Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB)
           && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
           && (Hob.ResourceDescriptor->ResourceLength) >= PeiMemSize + RequiredMemSize) {
        PeiMemBase = Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength - PeiMemSize - RequiredMemSize;
        //
        // Record Pei Memory Hob
        //
        PeiMemoryHob = Hob;
      }
    }

    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)  // Found the low memory length below 4G
        && (Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
        && (CompareGuid (&Hob.ResourceDescriptor->Owner, &gFspReservedMemoryResourceHobGuid))) {
      FoundFspMemHob = TRUE;
      FspMemoryBase = Hob.ResourceDescriptor->PhysicalStart;
      FspMemorySize = Hob.ResourceDescriptor->ResourceLength;
      DEBUG((DEBUG_INFO, "Find fsp mem hob, base 0x%x, len 0x%x\n", FspMemoryBase, FspMemorySize));
    }

    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)  // Found Tseg
      && (Hob.ResourceDescriptor->PhysicalStart >= 0x100000)
      && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= 0x100000000)
      && (CompareGuid (&Hob.ResourceDescriptor->Owner, &gFspReservedMemoryResourceHobTsegGuid))) {
        FoundTsegHob = TRUE;
        TsegBase = Hob.ResourceDescriptor->PhysicalStart;

        if ((Hob.ResourceDescriptor->ResourceLength == 0  ) || (Hob.ResourceDescriptor->ResourceLength > 0x800000)){
          Hob.ResourceDescriptor->ResourceLength = 0x800000;
        }

        TsegSize = Hob.ResourceDescriptor->ResourceLength;
        DEBUG((EFI_D_ERROR, "Find Tseg mem hob, base 0x%lx, len 0x%lx\n", TsegBase, TsegSize));
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  if (!FoundFspMemHob) {
    DEBUG((DEBUG_INFO, "Didn't find the fsp used memory information.\n"));
  }

  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    Hob.Raw = (UINT8 *)(UINTN)FspHobList;
    //
    // Find the largest memory range under 4GB excluding that given to Capsule.
    //
    while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {

      if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY
         && (Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB) 
         && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
        ) {
        if (PeiMemoryHob.ResourceDescriptor != Hob.ResourceDescriptor) {
          if (Hob.ResourceDescriptor->ResourceLength > CapsuleBufferLength) {
            CapsuleBuffer       = (VOID *) ((UINTN) Hob.ResourceDescriptor->PhysicalStart);
            CapsuleBufferLength = (UINTN)Hob.ResourceDescriptor->ResourceLength;
          }
        } else {
          if ((Hob.ResourceDescriptor->ResourceLength - PeiMemSize- RequiredMemSize) >= CapsuleBufferLength) {
            CapsuleBuffer       = (VOID *) ((UINTN) Hob.ResourceDescriptor->PhysicalStart);
            CapsuleBufferLength = (UINTN)(Hob.ResourceDescriptor->ResourceLength - PeiMemSize - RequiredMemSize);
          }
        }
      }

      Hob.Raw = GET_NEXT_HOB (Hob);
    }

    //
    // Call the Capsule PPI Coalesce function to coalesce the capsule data.
    //
    Status = Capsule->Coalesce ((EFI_PEI_SERVICES **)PeiServices, &CapsuleBuffer, &CapsuleBufferLength);

    DEBUG((DEBUG_INFO, "Capsule Buffer: 0x%x, Length : 0x%x\n", CapsuleBuffer, CapsuleBufferLength));
  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    DEBUG ((EFI_D_INFO, "Install S3 Memory. \n"));
    Status = GetS3AcpiReservedMemory(TsegBase, &PeiMemBase, &PeiMemSize);
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((EFI_D_INFO, "Install Normal Boot Mmeory. \n"));
  }
  DEBUG ((EFI_D_INFO, "PeiMemBase = %0x\n", PeiMemBase));
  DEBUG ((EFI_D_INFO, "PeiMemSize = %0x\n", PeiMemSize));

  Status = PeiServicesInstallPeiMemory (PeiMemBase, PeiMemSize);
  ASSERT_EFI_ERROR (Status);

  if (BootMode == BOOT_ON_FLASH_UPDATE && Capsule != NULL) {
    Status = Capsule->CreateState ((EFI_PEI_SERVICES **)PeiServices, CapsuleBuffer, CapsuleBufferLength);
    DEBUG((EFI_D_INFO, "Capsule CreateState :%d\n", Status));
  }

  return EFI_SUCCESS;
}

typedef struct {
  EFI_GUID    *Guid;
  BOOLEAN     NeedTransfer;
} FSP_TRANSFER_HOB_LIST;

FSP_TRANSFER_HOB_LIST  mFspTransferHobList[] = {
  {&gFspNonVolatileStorageHobGuid,     TRUE}, // This HOB stores MRC parameters for S3 resume.
  {&gFspBootLoaderTemporaryMemoryGuid, TRUE},
  {&gEfiPlatformCpuInfoGuid,           TRUE},
  {&gEfiMemoryConfigDataGuid,          FALSE}, // This HOB stores MRC parameters for S3 resume. 
  {&gEfiVariableGuid,                  TRUE},
  {&gEfiSmmPeiSmramMemoryReserveGuid,  TRUE}, // Handled by FspHobProcessForMemoryResource().
  {&gEfiAcpiVariableGuid,              TRUE}, // For S3 save/resume. 
  {&gFspSmbiosMemoryInfoHobGuid,       TRUE},
  {&gEfiGraphicsInfoHobGuid,           TRUE},
  {&gEfiPlatformInfoGuid,              TRUE},
};

BOOLEAN
GetFspGuidHobTransferFlag (
  IN EFI_GUID  *Guid
  )
{
  UINTN  Index;
  for (Index = 0; Index < sizeof(mFspTransferHobList)/sizeof(mFspTransferHobList[0]); Index++) {
    if (CompareGuid(Guid, mFspTransferHobList[Index].Guid)) {
      return mFspTransferHobList[Index].NeedTransfer;
    }
  }
  return FALSE;
}

VOID
TransferFspHobs (
  IN VOID                 *FspHobList
)
{
  EFI_PEI_HOB_POINTERS   FspHob;
  EFI_PEI_HOB_POINTERS   DxeHob;
  BOOLEAN                TransferFlag;

  //
  // Get the HOB list for processing.
  //
  FspHob.Raw = (VOID *)FspHobList;

  //
  // Go through HOBs produced by FSP and pass them to UEFI BIOS.
  //
  while (!END_OF_HOB_LIST (FspHob)) {
    
    TransferFlag = FALSE;

    if (FspHob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
        
      //
      // GUID HOB.
      //
      TransferFlag = GetFspGuidHobTransferFlag (&FspHob.Guid->Name);
      
      if (TransferFlag) {
        DEBUG ((DEBUG_INFO, "FSP  Extended    GUID HOB: {%g}\n", &(FspHob.Guid->Name)));
        DEBUG ((DEBUG_INFO, "                 HOB Length: %x\n", (UINT32)(FspHob.Header->HobLength)));
      }
      
    } else if ((FspHob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) && 
       (FspHob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)) {
       
      //
      // Reserved Memory Resource HOB.
      //
      DEBUG ((DEBUG_INFO, "FSP Reserved Memory Resource HOB: %016lX ~ %016lX\n", \
              FspHob.ResourceDescriptor->PhysicalStart, FspHob.ResourceDescriptor->PhysicalStart \
              + FspHob.ResourceDescriptor->ResourceLength));
      
      TransferFlag = TRUE;
    }

    //
    // Pass FSP HOB to UEFI BIOS.
    //
    if (TransferFlag) {        
      PeiServicesCreateHob (EFI_HOB_TYPE_UNUSED, FspHob.Header->HobLength, &(DxeHob.Raw));
      CopyMem (DxeHob.Raw, FspHob.Raw, FspHob.Header->HobLength);
    }
    
    FspHob.Raw = GET_NEXT_HOB (FspHob);
  }

  return;
} 

/**
  BIOS process FspBobList for other data (not Memory Resource Descriptor).

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return                If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
FspHobProcessForOtherData (
  IN VOID                 *FspHobList
  )
{
  EFI_PEI_SERVICES     **PeiServices;

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();
  
  //
  // Other hob for platform
  //
  PlatformHobCreateFromFsp ( PeiServices,  FspHobList);
  
  TransferFspHobs (FspHobList);
  
  return EFI_SUCCESS;
}

/**
  BIOS process FspBobList.

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return                If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
FspHobProcess (
  IN VOID                 *FspHobList
  )
{
  EFI_STATUS  Status;
  
  BuildGuidDataHob (&gEfiMemoryTypeInformationGuid, mDefaultMemoryTypeInformation, sizeof (mDefaultMemoryTypeInformation) );
    
  Status = FspHobProcessForMemoryResource (FspHobList);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = FspHobProcessForOtherData (FspHobList);

  return Status;
}
