/** @file
  This driver is responsible for the registration of child drivers
  and the abstraction of the PCH SMI sources.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSmm.h"
#include "PchSmmHelpers.h"

#include <Protocol/SmmBase2.h>
#include <Protocol/SmmControl2.h>
//
// MODULE / GLOBAL DATA
//
// Module variables used by the both the main dispatcher and the source dispatchers
// Declared in PchSmmSources.h
//

UINT32                AcpiBase;
PRIVATE_DATA          mPrivateData = {  ///< for the structure
  {
    NULL
  },                                    ///< CallbackDataBase linked list head
  NULL,                                 ///< EFI handle returned when calling InstallMultipleProtocolInterfaces
  NULL,                                 ///< EFI handle returned when calling InstallMultipleProtocolInterfaces
  {                                     ///< protocol arrays
    ///
    /// elements within the array
    ///
    {
      PROTOCOL_SIGNATURE,
      UsbType,
      &gEfiSmmUsbDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      SxType,
      &gEfiSmmSxDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      SwType,
      &gEfiSmmSwDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister,
        (UINTN) MAXIMUM_SWI_VALUE
      }
    },
    {
      PROTOCOL_SIGNATURE,
      GpiType,
      &gEfiSmmGpiDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister,
        (UINTN) NUM_SUPPORTED_GPIS
      }
    },
    {
      PROTOCOL_SIGNATURE,
      IchnType,
      &gEfiSmmIchnDispatchProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      IchnExType,
      &gEfiSmmIchnDispatchExProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      PowerButtonType,
      &gEfiSmmPowerButtonDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      PeriodicTimerType,
      &gEfiSmmPeriodicTimerDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister,
        (UINTN) PchSmmPeriodicTimerDispatchGetNextShorterInterval
      }
    },
  }
};

CONTEXT_FUNCTIONS     mContextFunctions[PchSmmProtocolTypeMax] = {
  {
    NULL,
    NULL,
    NULL
  },
  {
    SxGetContext,
    SxCmpContext,
    NULL
  },
  {
    SwGetContext,
    SwCmpContext,
    SwGetCommBuffer
  },
  {
    NULL,
    NULL,
    NULL
  },
  {
    NULL,
    NULL,
    NULL
  },
  {
    NULL,
    NULL,
    NULL
  },
  {
    PowerButtonGetContext,
    PowerButtonCmpContext,
    NULL
  },
  {
    PeriodicTimerGetContext,
    PeriodicTimerCmpContext,
    PeriodicTimerGetCommBuffer
  },
};

//
// PROTOTYPES
//
// Functions use only in this file
//
EFI_STATUS
EFIAPI
PchSmmCoreDispatcher (
  IN EFI_HANDLE             SmmImageHandle,
  IN CONST VOID             *ContextData,         OPTIONAL
  IN OUT VOID               *CommunicationBuffer, OPTIONAL
  IN OUT UINTN              *SourceSize           OPTIONAL
  );

//
// FUNCTIONS
//
/**
  Initializes the PCH SMM Dispatcher

  @param[in] ImageHandle          Pointer to the loaded image protocol for this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             PchSmmDispatcher Initialization completed.
**/
EFI_STATUS
EFIAPI
InitializePchSmmDispatcher (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  //
  // Access ACPI Base Addresses Register
  //
  AcpiBase = (UINT32) (MmioRead16 (
                         MmPciAddress (0,
                           DEFAULT_PCI_BUS_NUMBER_PCH,
                           PCI_DEVICE_NUMBER_PCH_LPC,
                           PCI_FUNCTION_NUMBER_PCH_LPC,
                           R_PCH_LPC_ACPI_BASE
                         )
                       ) & B_PCH_LPC_ACPI_BASE_BAR);
  ASSERT (AcpiBase != 0);

  ///
  /// Init required protocol for Pch Sw Dispatch protocol.
  ///
  PchSwDispatchInit ();

  ///
  /// Register a callback function to handle subsequent SMIs.  This callback
  /// will be called by SmmCoreDispatcher.
  ///
  Status = gSmst->SmiHandlerRegister (PchSmmCoreDispatcher, NULL, &mPrivateData.SmiHandle);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize Callback DataBase
  ///
  InitializeListHead (&mPrivateData.CallbackDataBase);
  PchSmmPublishDispatchProtocols ();

  ///
  /// Enable SMIs on the PCH now that we have a callback
  ///
  PchSmmInitHardware ();

  return EFI_SUCCESS;
}

/**
  Check the Fed SwSmiInputValue to see if there is a duplicated one in the database

  @param[in] FedSwSmiInputValue   Fed SwSmiInputValue

  @retval EFI_SUCCESS             There is no duplicated SwSmiInputValue
  @retval EFI_INVALID_PARAMETER   There is a duplicated SwSmiInputValue
**/
EFI_STATUS
SmiInputValueDuplicateCheck (
  UINTN           FedSwSmiInputValue
  )
{

  DATABASE_RECORD *RecordInDb;
  LIST_ENTRY      *LinkInDb;

  LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);
  while (!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
    RecordInDb = DATABASE_RECORD_FROM_LINK (LinkInDb);

    if (RecordInDb->ProtocolType == SwType) {
      if (RecordInDb->ChildContext.Sw.SwSmiInputValue == FedSwSmiInputValue) {
        return EFI_INVALID_PARAMETER;
      }
    }

    LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);
  }

  return EFI_SUCCESS;
}

/**
  Register a child SMI dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for this SMI source.
  @param[in] DispatchContext      Pointer to the dispatch function's context.
  @param[out] DispatchHandle      Handle of dispatch function, for when interfacing
                                  with the parent SMM driver, will be the address of linked
                                  list link in the call back record.

  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database record
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid
  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
**/
EFI_STATUS
PchSmmCoreRegister (
  IN  PCH_SMM_GENERIC_PROTOCOL                          *This,
  IN  EFI_SMM_HANDLER_ENTRY_POINT2                      DispatchFunction,
  IN  PCH_SMM_CONTEXT                                   *DispatchContext,
  OUT EFI_HANDLE                                        *DispatchHandle
  )
{
  EFI_STATUS                  Status;
  DATABASE_RECORD             *Record;
  PCH_SMM_QUALIFIED_PROTOCOL  *Qualified;
  PCH_SMM_SOURCE_DESC         NullSourceDesc = NULL_SOURCE_DESC_INITIALIZER;
  UINTN                       Index;

  Index     = 0;
  ///
  /// Create database record and add to database
  ///

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (DATABASE_RECORD), (VOID **) &Record);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Gather information about the registration request
  ///
  Record->Callback          = DispatchFunction;

  Record->ChildContext      = *DispatchContext;

  Qualified                 = QUALIFIED_PROTOCOL_FROM_GENERIC (This);

  Record->ProtocolType      = Qualified->Type;

  Record->ContextFunctions  = mContextFunctions[Qualified->Type];
  ///
  /// Perform linked list housekeeping
  ///
  Record->Signature = DATABASE_RECORD_SIGNATURE;

  switch (Qualified->Type) {
    ///
    /// By the end of this switch statement, we'll know the
    /// source description the child is registering for
    ///
    case UsbType:
      ///
      /// Check the validity of Context Type
      ///
      if ((Record->ChildContext.Usb.Type < UsbLegacy) || (Record->ChildContext.Usb.Type > UsbWake)) {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      MapUsbToSrcDesc (DispatchContext, &(Record->SrcDesc));
      Record->ClearSource = NULL;
      ///
      /// use default clear source function
      ///
      break;

    case SxType:
      ///
      /// Check the validity of Context Type and Phase
      ///
      if ((Record->ChildContext.Sx.Type < SxS0) ||
          (Record->ChildContext.Sx.Type >= EfiMaximumSleepType) ||
          (Record->ChildContext.Sx.Phase < SxEntry) ||
          (Record->ChildContext.Sx.Phase >= EfiMaximumPhase)
          ) {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) (&SX_SOURCE_DESC), sizeof (PCH_SMM_SOURCE_DESC));
      Record->ClearSource = NULL;
      ///
      /// use default clear source function
      ///
      break;

    case SwType:
      ///
      /// Check the validity of Context Value
      ///
    if (Record->ChildContext.Sw.SwSmiInputValue  == (UINTN)-1) {
      for (Index = 1; Index < MAXIMUM_SWI_VALUE; Index++) {
        if (!EFI_ERROR (SmiInputValueDuplicateCheck (Index))) {
          Record->ChildContext.Sw.SwSmiInputValue = Index;
          break;
        }
      }
      if (Record->ChildContext.Sw.SwSmiInputValue == (UINTN)-1) {
        goto Error;
      }
    }
      if (Record->ChildContext.Sw.SwSmiInputValue > MAXIMUM_SWI_VALUE) {
        goto Error;
      }

      if (EFI_ERROR (SmiInputValueDuplicateCheck (Record->ChildContext.Sw.SwSmiInputValue))) {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) (&SW_SOURCE_DESC), sizeof (PCH_SMM_SOURCE_DESC));
      Record->ClearSource = NULL;
      ///
      /// use default clear source function
      ///
      break;

    case GpiType:
      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      CopyMem (
        (VOID *) &(Record->SrcDesc),
        (VOID *) &(GPI_SOURCE_DESC[Record->ChildContext.Gpi.GpiNum]),
        sizeof (PCH_SMM_SOURCE_DESC)
        );
      Record->ClearSource = NULL;
      ///
      /// use default clear source function
      ///
      break;

    case IchnType:
      ///
      /// Check the validity of Context Type
      ///
      if (Record->ChildContext.Ichn.Type >= NUM_ICHN_TYPES) {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      if (SocStepping () < SocB0) {
        CopyMem (
          (VOID *) &(Record->SrcDesc),
          (VOID *) &(ICHN_SOURCE_DESCS_AX[Record->ChildContext.Ichn.Type]),
          sizeof (PCH_SMM_SOURCE_DESC)
          );
      } else {
        CopyMem (
          (VOID *) &(Record->SrcDesc),
          (VOID *) &(ICHN_SOURCE_DESCS[Record->ChildContext.Ichn.Type]),
          sizeof (PCH_SMM_SOURCE_DESC)
          );
      }
      Record->ClearSource = PchSmmIchnClearSource;
      break;

    case IchnExType:
      ///
      /// Check the validity of Context Type
      ///
      if ((Record->ChildContext.IchnEx.Type < IchnExPciExpress) || (Record->ChildContext.IchnEx.Type >= IchnExTypeMAX)) {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      CopyMem (
        (VOID *) &(Record->SrcDesc),
        (VOID *) &(ICHN_EX_SOURCE_DESCS[Record->ChildContext.IchnEx.Type - IchnExPciExpress]),
        sizeof (PCH_SMM_SOURCE_DESC)
        );
      Record->ClearSource = NULL;
      break;

    case PowerButtonType:
      ///
      /// Check the validity of Context Phase
      ///
      if ((Record->ChildContext.PowerButton.Phase < EfiPowerButtonEntry) ||
          (Record->ChildContext.PowerButton.Phase > EfiPowerButtonExit)
         )
      {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) &POWER_BUTTON_SOURCE_DESC, sizeof (PCH_SMM_SOURCE_DESC));
      Record->ClearSource = NULL;
      ///
      /// use default clear source function
      ///
      break;

    case PeriodicTimerType:
      ///
      /// Check the validity of timer value
      ///
      if (DispatchContext->PeriodicTimer.SmiTickInterval <= 0) {
        goto Error;
      }

      InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
      MapPeriodicTimerToSrcDesc (DispatchContext, &(Record->SrcDesc));
      Record->ClearSource = PchSmmPeriodicTimerClearSource;
      break;

    default:
      goto Error;
      break;
    }

  if (CompareSources (&Record->SrcDesc, &NullSourceDesc)) {
    goto Error;
  }

  if (Record->ClearSource == NULL) {
    ///
    /// Clear the SMI associated w/ the source using the default function
    ///
    PchSmmClearSource (&Record->SrcDesc);
  } else {
    ///
    /// This source requires special handling to clear
    ///
    Record->ClearSource (&Record->SrcDesc);
  }

  PchSmmEnableSource (&Record->SrcDesc);

  ///
  /// Child's handle will be the address linked list link in the record
  ///
  *DispatchHandle = (EFI_HANDLE) (&Record->Link);
  *DispatchContext = Record->ChildContext;

  return EFI_SUCCESS;

Error:
  Status = gSmst->SmmFreePool (Record);
  ///
  /// DEBUG((EFI_D_ERROR,"Free pool status %d\n", Status ));
  ///
  return EFI_INVALID_PARAMETER;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the  EFI_SMM_IO_TRAP_DISPATCH_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
PchSmmCoreUnRegister (
  IN PCH_SMM_GENERIC_PROTOCOL                           *This,
  IN EFI_HANDLE                                         *DispatchHandle
  )
{
  ///
  /// BOOLEAN SafeToDisable;
  ///
  DATABASE_RECORD *RecordToDelete;

  ///
  /// DATABASE_RECORD *RecordInDb;
  /// EFI_LIST_NODE   *LinkInDb;
  ///
  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RecordToDelete = DATABASE_RECORD_FROM_LINK (DispatchHandle);

  ///
  /// Take the entry out of the linked list
  ///
  if (RecordToDelete->Link.ForwardLink == (LIST_ENTRY *) EFI_BAD_POINTER) {
    return EFI_INVALID_PARAMETER;
  }

  RemoveEntryList (&RecordToDelete->Link);
  
  return EFI_SUCCESS;
}

/**
  The callback function to handle subsequent SMIs.  This callback will be called by SmmCoreDispatcher.

  @param[in] SmmImageHandle             Not used
  @param[in,out] CommunicationBuffer   Not used
  @param[in,out] SourceSize            Not used

  @retval EFI_SUCCESS                   Function successfully completed
**/
EFI_STATUS
EFIAPI
PchSmmCoreDispatcher (
  IN EFI_HANDLE             SmmImageHandle,
  IN CONST VOID             *ContextData,          OPTIONAL
  IN OUT VOID               *CommunicationBuffer,  OPTIONAL
  IN OUT UINTN              *SourceSize            OPTIONAL
  )
{
  ///
  /// Used to prevent infinite loops
  ///
  UINTN               EscapeCount;

  BOOLEAN             ContextsMatch;
  BOOLEAN             EosSet;
  BOOLEAN             SxChildWasDispatched;

  DATABASE_RECORD     *RecordInDb;
  LIST_ENTRY          *LinkInDb;
  DATABASE_RECORD     *RecordToExhaust;
  LIST_ENTRY          *LinkToExhaust;

  PCH_SMM_CONTEXT     Context;
  VOID                *CommBuffer;
  UINTN               CommBufferSize;

  EFI_STATUS          Status;

  PCH_SMM_SOURCE_DESC ActiveSource = NULL_SOURCE_DESC_INITIALIZER;

  EscapeCount           = 100;
  ContextsMatch         = FALSE;
  EosSet                = FALSE;
  SxChildWasDispatched  = FALSE;
  Status                = EFI_SUCCESS;

  if (!IsListEmpty (&mPrivateData.CallbackDataBase)) {
    ///
    /// We have children registered w/ us -- continue
    ///
    while ((!EosSet) && (EscapeCount > 0)) {
      EscapeCount--;

      LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);

      while (!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
        RecordInDb = DATABASE_RECORD_FROM_LINK (LinkInDb);

        ///
        /// look for the first active source
        ///
        if (!SourceIsActive (&RecordInDb->SrcDesc)) {
          ///
          /// Didn't find the source yet, keep looking
          ///
          LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);

          ///
          /// if it's the last one, try to clear EOS
          ///
          if (IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
            EosSet = PchSmmSetAndCheckEos ();
          }
        } else {
          ///
          /// We found a source. If this is a sleep type, we have to go to
          /// appropriate sleep state anyway.No matter there is sleep child or not
          ///
          if (RecordInDb->ProtocolType == SxType) {
            SxChildWasDispatched = TRUE;
          }
          ///
          /// "cache" the source description and don't query I/O anymore
          ///
          CopyMem ((VOID *) &ActiveSource, (VOID *) &(RecordInDb->SrcDesc), sizeof (PCH_SMM_SOURCE_DESC));
          LinkToExhaust = LinkInDb;

          ///
          /// exhaust the rest of the queue looking for the same source
          ///
          while (!IsNull (&mPrivateData.CallbackDataBase, LinkToExhaust)) {
            RecordToExhaust = DATABASE_RECORD_FROM_LINK (LinkToExhaust);
            ///
            /// RecordToExhaust->Link might be removed (unregistered) by Callback function, and then the
            /// system will hang in ASSERT() while calling GetNextNode().
            /// To prevent the issue, we need to get next record in DB here (before Callback function).
            ///
            LinkToExhaust = GetNextNode (&mPrivateData.CallbackDataBase, &RecordToExhaust->Link);

            if (CompareSources (&RecordToExhaust->SrcDesc, &ActiveSource)) {
              ///
              /// These source descriptions are equal, so this callback should be
              /// dispatched.
              ///
              if (RecordToExhaust->ContextFunctions.GetContext != NULL) {
                ///
                /// This child requires that we get a calling context from
                /// hardware and compare that context to the one supplied
                /// by the child.
                ///
                ASSERT (RecordToExhaust->ContextFunctions.CmpContext != NULL);

                ///
                /// Make sure contexts match before dispatching event to child
                ///
                RecordToExhaust->ContextFunctions.GetContext (RecordToExhaust, &Context);
                ContextsMatch = RecordToExhaust->ContextFunctions.CmpContext (&Context, &RecordToExhaust->ChildContext);

              } else {
                ///
                /// This child doesn't require any more calling context beyond what
                /// it supplied in registration.  Simply pass back what it gave us.
                ///
                ASSERT (RecordToExhaust->Callback != NULL);
                Context       = RecordToExhaust->ChildContext;
                ContextsMatch = TRUE;
              }

              if (ContextsMatch) {

                if (RecordToExhaust->Callback != NULL) {
                  if (RecordToExhaust->ContextFunctions.GetCommBuffer != NULL) {
                    ///
                    /// This callback function needs CommBuffer and CommBufferSize.
                    /// Get those from child and then pass to callback function.
                    ///
                    RecordToExhaust->ContextFunctions.GetCommBuffer (RecordToExhaust, &CommBuffer, &CommBufferSize);
                  } else {
                    ///
                    /// Child doesn't support the CommBuffer and CommBufferSize.
                    /// Just pass NULL value to callback function.
                    ///
                    CommBuffer     = NULL;
                    CommBufferSize = 0;
                  }

                  PERF_START_EX (NULL, "SmmFunction", NULL, AsmReadTsc(), RecordToExhaust->ProtocolType);
                  RecordToExhaust->Callback ((EFI_HANDLE) & RecordToExhaust->Link, &Context, CommBuffer, &CommBufferSize);
                  PERF_END_EX (NULL, "SmmFunction", NULL, AsmReadTsc(), RecordToExhaust->ProtocolType);
                  if (RecordToExhaust->ProtocolType == SxType) {
                    SxChildWasDispatched = TRUE;
                  }
                } else {
                  ASSERT (FALSE);
                }
              }
            }
          }

          if (RecordInDb->ClearSource == NULL) {
            ///
            /// Clear the SMI associated w/ the source using the default function
            ///
            PchSmmClearSource (&ActiveSource);
          } else {
            ///
            /// This source requires special handling to clear
            ///
            RecordInDb->ClearSource (&ActiveSource);
          }
          ///
          /// Also, try to clear EOS
          ///
          EosSet = PchSmmSetAndCheckEos ();
          ///
          /// Queue is empty, reset the search
          ///
          break;
        }
      }
    }
  }
  ///
  /// If you arrive here, there are two possible reasons:
  /// (1) you've got problems with clearing the SMI status bits in the
  /// ACPI table.  If you don't properly clear the SMI bits, then you won't be able to set the
  /// EOS bit.  If this happens too many times, the loop exits.
  /// (2) there was a SMM communicate for callback messages that was received prior
  /// to this driver.
  /// If there is an asynchronous SMI that occurs while processing the Callback, let
  /// all of the drivers (including this one) have an opportunity to scan for the SMI
  /// and handle it.
  /// If not, we don't want to exit and have the foreground app. clear EOS without letting
  /// these other sources get serviced.
  ///
  /// This assert is not valid with CSM legacy solution because it generates software SMI
  /// to test for legacy USB support presence.
  /// This may not be illegal, so we cannot assert at this time.
  ///
  ///  ASSERT (EscapeCount > 0);
  ///
  if (SxChildWasDispatched) {
    ///
    /// A child of the SmmSxDispatch protocol was dispatched during this call;
    /// put the system to sleep.
    ///
    PchSmmSxGoToSleep ();
  }

  return Status;
}
