/** @file
  SmmAccess PPI
  This code abstracts the PEI core to provide SmmAccess services.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PEI_SMM_ACCESS_PPI_H_
#define _PEI_SMM_ACCESS_PPI_H_

typedef struct _PEI_SMM_ACCESS_PPI PEI_SMM_ACCESS_PPI;

/**
  This routine accepts a request to "open" a region of SMRAM.  The
  region could be legacy ABSEG, HSEG, or TSEG near top of physical memory.
  The use of "open" means that the memory is visible from all PEIM
  and SMM agents.

  @param[in]
  PeiServices           - General purpose services available to every PEIM.
  This                  - Pointer to the SMM Access Interface.
  DescriptorIndex       - Region of SMRAM to Open.

  @retval
  EFI_SUCCESS           - The region was successfully opened.
  EFI_DEVICE_ERROR      - The region could not be opened because locked by
                            chipset.
  EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
  
**/
typedef
EFI_STATUS
(EFIAPI *PEI_SMM_OPEN) (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  );

/**
  This routine accepts a request to "close" a region of SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "close" means that the memory is only visible from SMM agents,
  not from PEIM.

  @param[in]
  PeiServices           - General purpose services available to every PEIM.
  This                  - Pointer to the SMM Access Interface.
  DescriptorIndex       - Region of SMRAM to Close.

  @retval
  EFI_SUCCESS           - The region was successfully closed.
  EFI_DEVICE_ERROR      - The region could not be closed because locked by
                          chipset.
  EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
  
**/
typedef
EFI_STATUS
(EFIAPI *PEI_SMM_CLOSE) (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  );

/**
  This routine accepts a request to "lock" SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "lock" means that the memory can no longer be opened
  to PEIM.

  @param[in]
  PeiServices           - General purpose services available to every PEIM.
  This                  - Pointer to the SMM Access Interface.
  DescriptorIndex       - Region of SMRAM to Lock.

  @retval
  EFI_SUCCESS           - The region was successfully locked.
  EFI_DEVICE_ERROR      - The region could not be locked because at least
                            one range is still open.
  EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
  
**/
typedef
EFI_STATUS
(EFIAPI *PEI_SMM_LOCK) (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  );

/**
  This routine services a user request to discover the SMRAM
  capabilities of this platform.  This will report the possible
  ranges that are possible for SMRAM access, based upon the 
  memory controller capabilities.

  @param[in]
  PeiServices           - General purpose services available to every PEIM.
  This                  - Pointer to the SMRAM Access Interface.
  SmramMapSize          - Pointer to the variable containing size of the
                          buffer to contain the description information.
  SmramMap              - Buffer containing the data describing the Smram
                          region descriptors.
  @retval
  EFI_BUFFER_TOO_SMALL  - The user did not provide a sufficient buffer.
  EFI_SUCCESS           - The user provided a sufficiently-sized buffer.
  
**/
typedef
EFI_STATUS
(EFIAPI *PEI_SMM_CAPABILITIES) (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  );

struct _PEI_SMM_ACCESS_PPI {
  PEI_SMM_OPEN          Open;
  PEI_SMM_CLOSE         Close;
  PEI_SMM_LOCK          Lock;
  PEI_SMM_CAPABILITIES  GetCapabilities;
  BOOLEAN               LockState;
  BOOLEAN               OpenState;
};

extern EFI_GUID gPeiSmmAccessPpiGuid;

#endif
