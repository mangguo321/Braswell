/** @file
  SmmIchnDispatch Extended Protocol

  Copyright (c) 2008 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _EFI_SMM_ICHN_DISPATCH_EX_H_
#define _EFI_SMM_ICHN_DISPATCH_EX_H_

#include <Protocol/SmmIchnDispatch.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                                   gEfiSmmIchnDispatchExProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_SMM_ICHN_DISPATCH_EX_PROTOCOL EFI_SMM_ICHN_DISPATCH_EX_PROTOCOL;

//
// Related Definitions
//
///
/// Ichn Dispatch Extended Types
///
typedef enum {
  IchnExPciExpress = NUM_ICHN_TYPES + 1,
  IchnExMonitor,
  IchnExSpi,
  IchnExQRT,
  IchnExGpioUnlock,
  IchnExTmrOverflow,
  IchnExPcie0Hotplug,
  IchnExPcie1Hotplug,
  IchnExPcie2Hotplug,
  IchnExPcie3Hotplug,
  IchnExPcie0LinkActive,
  IchnExPcie1LinkActive,
  IchnExPcie2LinkActive,
  IchnExPcie3LinkActive,
  //
  // INSERT NEW ITEMS JUST BEFORE THIS LINE
  //
  IchnExTypeMAX ///< the maximum number of items in this enumeration
} EFI_SMM_ICHN_EX_SMI_TYPE;

typedef struct {
  EFI_SMM_ICHN_EX_SMI_TYPE  Type;
} EFI_SMM_ICHN_DISPATCH_EX_CONTEXT;

//
// Member functions
//

/**
  Dispatch function for a ICH n Extended specific SMI handler.

  @param[in] DispatchHandle       Handle of this dispatch function.
  @param[in] DispatchContext      Pointer to the dispatch function's context.
                                  The DispatchContext fields are filled in
                                  by the dispatching driver prior to
                                  invoking this dispatch function.

  
**/
typedef
VOID
(EFIAPI *EFI_SMM_ICHN_DISPATCH_EX) (
  IN  EFI_HANDLE                                DispatchHandle,
  IN  EFI_SMM_ICHN_DISPATCH_EX_CONTEXT          * DispatchContext
  );

/**
  Register a child SMI source dispatch function with a parent SMM driver

  @param[in] This                 Protocol instance pointer.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for
                                  this SMI source
  @param[in] DispatchContext      Pointer to the dispatch function's context.
                                  The caller fills this context in before calling
                                  the register function to indicate to the register
                                  function the ICHN SMI source for which the dispatch
                                  function should be invoked.
  @param[out] DispatchHandle      Handle of dispatch function, for when interfacing
                                  with the parent SMM driver.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR        The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES    Not enough memory (system or SMM) to manage this
                                  child.
  @retval EFI_INVALID_PARAMETER   DispatchContext is invalid. The ICHN input value
                                  is not within valid range.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_ICHN_EX_REGISTER) (
  IN  EFI_SMM_ICHN_DISPATCH_EX_PROTOCOL   * This,
  IN  EFI_SMM_ICHN_DISPATCH_EX            DispatchFunction,
  IN  EFI_SMM_ICHN_DISPATCH_EX_CONTEXT    * DispatchContext,
  OUT EFI_HANDLE                          * DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                 Protocol instance pointer.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
  @retval Others                  TBD
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_ICHN_EX_UNREGISTER) (
  IN  EFI_SMM_ICHN_DISPATCH_EX_PROTOCOL         * This,
  IN  EFI_HANDLE                                DispatchHandle
  );

///
/// Interface structure for the SMM Ich n specific SMI Dispatch Protocol
///
/// This protocol provides the ability to dispatch function for a ICHn specific SMI.
/// This protocol acts as an extension to the EFI_SMM_ICHN_DISPATCH_PROTOCOL capabilities
/// by defining several new SMI types: IchnExPciExpress, IchnExMonitor, IchnExSpi,
/// IchnExQRT, IchnGpioUnlockSmi, IchnExTmrOverflow, IchnExPcieXHotplug, IchnExPcieXLinkActive.
///
struct _EFI_SMM_ICHN_DISPATCH_EX_PROTOCOL {
  EFI_SMM_ICHN_EX_REGISTER    Register;   ///< Register a child SMI source dispatch function with a parent SMM driver.
  EFI_SMM_ICHN_EX_UNREGISTER  UnRegister; ///< Un-register a child SMI source dispatch function with a parent SMM driver.
};

#endif
