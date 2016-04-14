/** @file
  Header file for the power management driver.
  This driver loads power management support designed to be similar to
  the mobile platform power management reference code.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _POWER_MANAGEMENT_H_
#define _POWER_MANAGEMENT_H_

//
// Statements that include other files
//
#include <PiDxe.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/PpmPlatformPolicy.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>
#include <Library/AslUpdateLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Guid/PowerManagementAcpiTableStorage.h>

#include <Ppm.h>
#include <CpuPpmLib.h>
#include <PchAccess.h>

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

//
// Callback function prototypes
//
/**
  PPM must restore runtime state of MSR.  This is not supported by the S3 boot script.
  In order to accomplish this, the ASL is modified to generate an SMI on S3 in the _WAK method.
  This SMI handler reponds to that SW SMI.
  
  @param[in]

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

  @retval

  None.

**/
EFI_STATUS
S3RestoreMsrCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  );

///VLV not support for Io Trap method
/**
  Initialize the power management support.
  This function will do boot time configuration:
    Install into SMRAM/SMM
    Detect HW capabilities and SW configuration
    Initialize HW and software state (primarily MSR and ACPI tables)
    Install SMI handlers for runtime interfacess

  @param[in]

  ImageHandle   - Pointer to the loaded image protocol for this driver
  SystemTable   - Pointer to the EFI System Table

  @retval

  EFI_SUCCESS   The driver installed/initialized correctly.
  Driver will ASSERT in debug builds on error.  PPM functionality is considered critical for mobile systems.

**/
EFI_STATUS
EFIAPI
InitializePowerManagement (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif
