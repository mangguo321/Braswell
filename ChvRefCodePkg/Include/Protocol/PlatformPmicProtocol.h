/** @file
  Header file for Platform PMIC Protocol.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PLATFORM_PMIC_PROTOCOL_H
#define _PLATFORM_PMIC_PROTOCOL_H

//#define EFI_PLATFORM_PMIC_PROTOCOL_GUID {0x466cf304, 0x5dc3, 0x4ef5, 0x83, 0x5c, 0xc0, 0xae, 0xcc, 0xbc, 0x2f, 0xde}

typedef struct _EFI_PLATFORM_PMIC_PROTOCOL EFI_PLATFORM_PMIC_PROTOCOL;

extern EFI_GUID gDxePlatformPmicProtocolGuid;

/**
  @param[in]    This          Pointer to EFI_PLATFORM_PMIC_PROTOCOL
  @param[in]    Address		  PMIC register offset address to be read
  @param[in]    Data		  Data to be written to "Address"

  @retval  EFI_SUCCESS        Successfully read the value

**/
typedef
EFI_STATUS
(EFIAPI *EFI_PLATFORM_PMIC_PROTOCOL_WRITE) (
  IN   EFI_PLATFORM_PMIC_PROTOCOL    *This,
  IN   UINT8                     Address,
  IN   UINT8                     Data
  );

/**
  @param[in]  This           Pointer to EFI_PLATFORM_PMIC_PROTOCOL
  @param[in]  Address		 PMIC register offset address to be read

  @retval   Data		     Read value from the "Address"

**/
typedef
UINT8
(EFIAPI *EFI_PLATFORM_PMIC_PROTOCOL_READ) (
  IN   EFI_PLATFORM_PMIC_PROTOCOL    *This,
  IN   UINT8                     Address
  );

//
// Interface structure for the Platform PMIC Protocol
//
struct _EFI_PLATFORM_PMIC_PROTOCOL {
  EFI_PLATFORM_PMIC_PROTOCOL_WRITE               PmicWriteByte;
  EFI_PLATFORM_PMIC_PROTOCOL_READ				 PmicReadByte;
};

/**
  @todo	add description
**/
UINT8
EFIAPI
PmicReadByte (
  IN EFI_PLATFORM_PMIC_PROTOCOL  *This,
  IN UINT8					     Address
  );

/**
  @todo	add description
**/
EFI_STATUS
EFIAPI
PmicWriteByte (
  IN EFI_PLATFORM_PMIC_PROTOCOL  *This,
  IN UINT8					     Address,
  IN UINT8					     Data
  );

#endif
