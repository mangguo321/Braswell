/** @file
  IST/PPM reference code definitions.

  Acronyms:
    PPM   Platform Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CPU_PPM_LIB_H_
#define _CPU_PPM_LIB_H_

//
// Statements that include other files
//
#include <PiDxe.h>

#include <PpmCommon.h>
#include <Protocol/PpmProcessorSupport2.h>

//
// CPUID definitions
//
#define CPUID_EXT_FAMILY                  0x0FF00000
#define CPUID_EXT_MODEL                   0x000F0000
#define CPUID_FAMILY                      0x00000F00
#define CPUID_MODEL                       0x000000F0
#define CPUID_STEPPING                    0x0000000F
#define CPUID_FAMILY_MODEL                0x00000FF0
#define CPUID_FAMILY_MODEL_STEPPING       0x00000FFF
#define CPUID_FULL_FAMILY_MODEL           0x0FFF0FF0
#define CPUID_FULL_FAMILY_MODEL_STEPPING  0x0FFF0FFF
#define CPUID_FULL_FAMILY_MODEL_VALLEVIEW 0x00030670  // Valleyview
//
// Structure declarations
//

//
// UINT64 workaround
//
// The MS compiler doesn't handle QWORDs very well.  I'm breaking
// them into DWORDs to circumvent the problems.  Converting back
// shouldn't be a big deal.
//
#pragma pack(1)
typedef union _MSR_REGISTER {
  UINT64  Qword;

  struct _DWORDS {
    UINT32  Low;
    UINT32  High;
  } Dwords;

  struct _BYTES {
    UINT8 FirstByte;
    UINT8 SecondByte;
    UINT8 ThirdByte;
    UINT8 FouthByte;
    UINT8 FifthByte;
    UINT8 SixthByte;
    UINT8 SeventhByte;
    UINT8 EighthByte;
  } Bytes;

} MSR_REGISTER;
#pragma pack()

typedef struct _EFI_MSR_VALUES{
  UINT16  Index;
  UINT64  Value;
} EFI_MSR_VALUES;

//
// Function prototypes
//
/**
  Initializes the processor power management library.  This must be called
  prior to any of the library functions being used.

  At this time, we don't properly publish the PPM processor support protocol,
  we simply return it if this library implements the protocol.

  If the processor is not supported, the input will not be modified.

  @param[in, out]  This        Pointer to the PPM support protocol instance

  @retval  EFI_SUCCESS         Library was initialized successfully
  @retval  EFI_NOT_SUPPORTED   The library does not support the current processor

**/
EFI_STATUS
InitializePenrynPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  );
/**
  Initializes the processor power management library.  This must be called
  prior to any of the library functions being used.

  At this time, we don't properly publish the PPM processor support protocol,
  we simply return it if this library implements the protocol.

  If the processor is not supported, the input will not be modified.

  @param[in, out]  This        Pointer to the PPM support protocol instance

  @retval  EFI_SUCCESS         Library was initialized successfully
  @retval  EFI_NOT_SUPPORTED   The library does not support the current processor

**/
EFI_STATUS
InitializeMeromPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  );

/**
  Initializes the processor power management library.  This must be called
  prior to any of the library functions being used.

  At this time, we don't properly publish the PPM processor support protocol,
  we simply return it if this library implements the protocol.

  If the processor is not supported, the input will not be modified.

  @param[in, out]  This        Pointer to the PPM support protocol instance

  @retval  EFI_SUCCESS         Library was initialized successfully
  @retval  EFI_NOT_SUPPORTED   The library does not support the current processor

**/
EFI_STATUS
InitializeYonahPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  );

/**
  Initializes the processor power management library.  This must be called
  prior to any of the library functions being used.

  At this time, we don't properly publish the PPM processor support protocol,
  we simply return it if this library implements the protocol.

  If the processor is not supported, the input will not be modified.

  @param[in, out] This         Pointer to the PPM support protocol instance

  @retval  EFI_SUCCESS         Library was initialized successfully
  @retval  EFI_NOT_SUPPORTED   The library does not support the current processor

**/
EFI_STATUS
InitializeDothanPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  );

/**
  Initializes the processor power management library for Cherryview processors.
  This must be called  prior to any of the library functions being used.

  At this time, we don't properly publish the PPM processor support protocol,
  we simply return it if this library implements the protocol.

  If the processor is not supported, the input will not be modified.

  @param[in, out]  This        Pointer to the PPM support protocol instance

  @retval  EFI_SUCCESS         Library was initialized successfully
  @retval  EFI_NOT_SUPPORTED   The library does not support the current processor

**/
EFI_STATUS
InitializeCherryviewPowerManagementLib (
  IN OUT PPM_PROCESSOR_SUPPORT_PROTOCOL_2   **This
  );

#endif
