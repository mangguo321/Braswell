/** @file
  This library includes the recovery function that can be customized by OEM, 
  including how to select the recovery capsule if more than one capsule found, 
  and security check.
  
  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#ifndef __RECOVERY_OEM_HOOK_LIB_H__
#define __RECOVERY_OEM_HOOK_LIB_H__

/**
  The recovery capsule is determined by 2 factors,
    1. The device search order, if more than one Device Recovery Module PPI 
    was discovered
    2. The individual search order, if the device reported more than one recovery 
    DXE capsule was found generating a search order list.
  
  The 2 orders are decided by the RecoveryOemHook library function OemRecoveryRankCapsule().  
  
  @param[in, out]  DeviceInstance  Specifies which EFI_PEI_DEVICE_RECOVERY_MODULE_PPI instance
                   to retrieve when passed out.  Specifies which EFI_PEI_DEVICE_RECOVERY_MODULE_PPI 
                   instance retrieved last time when passed in, start from zero.
  @param[in, out]  CapsuleInstance  Specifies which recovery capsule to retrieve when passed out.  
                   Specifies which recovery capsule retrieved last time when passed in, start from zero.   
  @param[out]      DeviceRecoveryModule  If it's not NULL, it's EFI_PEI_DEVICE_RECOVERY_MODULE_PPI.
                   instance specified by DeviceInstance,  caller can use this instance directly.  The 
                   function will change DeviceRecoveryModule to NULL, if it doesn't return a PPI instance.                                                      
  @param[out]      CapsuleBuffer  If it's not NULL, it contains loaded capsule, caller can use it directly.
                   The function will change Buffer to NULL, if it doesn't load a capsule.
                   
  @retval TRUE  If a recovery capsule is found.
  @retval FALSE If  no recovery capsule found.
                     
**/
BOOLEAN
EFIAPI
OemRecoveryRankCapsule (
  IN OUT UINTN                               *DeviceInstance,
  IN OUT UINTN                               *CapsuleInstance,
     OUT EFI_PEI_DEVICE_RECOVERY_MODULE_PPI  **DeviceRecoveryModule,  OPTIONAL
     OUT VOID                                **CapsuleBuffer OPTIONAL    
  );
  
/**
  This function performe security check and check failure handling,
  
  @param  Buffer The caller allocated buffer that contains recovery capsule to be checked.                                 

  @retval TRUE  If it's secure.
  @retval FALSE If it's not secure.
                     
**/
BOOLEAN
EFIAPI
OemRecoverySecurityCheck (
  VOID    *Buffer
  );  

#endif
