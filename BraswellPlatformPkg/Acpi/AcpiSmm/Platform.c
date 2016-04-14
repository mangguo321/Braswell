/** @file
  Generic template for a child of the IchSmm driver.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "AcpiSmm.h"
#include <Protocol/CpuIo2.h>

//
// Local variables
//
typedef struct {
  UINT8     Device;
  UINT8     Function;
} EFI_PCI_BUS_MASTER;

EFI_PCI_BUS_MASTER  mPciBm[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4 },
};

UINT16                                  mAcpiBaseAddr;
SYSTEM_CONFIGURATION                    mSystemConfiguration;
UINT16                                  mPM1_SaveState16;
UINT32                                  mGPE_SaveState32;

//
// Variables. Need to initialize this from Setup
//
BOOLEAN                                 mWakeOnLanS5Variable;
BOOLEAN                                 mWakeOnRtcVariable;
UINT8                                   mWakeupDay;
UINT8                                   mWakeupHour;
UINT8                                   mWakeupMinute;
UINT8                                   mWakeupSecond;

//
// Use an enum. 0 is Stay Off, 1 is Last State, 2 is Stay On
//
UINT8                                   mAcLossVariable;

static
UINT8 mTco1Sources[] = {
  IchnNmi
};

UINTN
DevicePathSize (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  );

VOID
S4S5ProgClock();

EFI_STATUS
InitRuntimeScriptTable (
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

VOID
EnableS5WakeOnRtc();

UINT8
HexToBcd (
  UINT8 HexValue
  );

UINT8
BcdToHex(
  IN UINT8 BcdValue
  );

VOID
CpuSmmSxWorkAround (
  );


/*++
  Set the AC Loss to turn on or off.

  @param[in]  Turn on or off.

  @retval     None

--*/

VOID
SetAfterG3On (
  BOOLEAN Enable
  )
{
  UINT8             PmCon1;

  //
  // ICH handling portion
  //
  PmCon1 = MmioRead8 ( PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1 );
  PmCon1 &= ~B_PCH_PMC_GEN_PMCON_AFTERG3_EN;
  if (Enable) {
    PmCon1 |= B_PCH_PMC_GEN_PMCON_AFTERG3_EN;
  }
  MmioWrite8 (PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1, PmCon1);

}

/**
  Allocate EfiACPIMemoryNVS below 4G memory address.

  This function allocates EfiACPIMemoryNVS below 4G memory address.

  @param  Size         Size of memory to allocate.

  @return Allocated address for output.

**/
VOID*
AllocateAcpiNvsMemoryBelow4G (
  IN   UINTN   Size
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;
  VOID*                 Buffer;

  Pages = EFI_SIZE_TO_PAGES (Size);
  Address = 0xffffffff;

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  Buffer = (VOID *) (UINTN) Address;
  ZeroMem (Buffer, Size);

  return Buffer;
}

EFI_STATUS
EFIAPI
AllocateS3AcpiReservedMemory (
  VOID
)
{
  EFI_PEI_HOB_POINTERS                      Hob;
  UINT64                                    MemoryLength;
  RESERVED_ACPI_S3_RANGE                    *AcpiS3Range;
  EFI_SMRAM_DESCRIPTOR                      *SmramDescriptor;
  VOID                                      *GuidHob;
  VOID                                      *AcpiReservedBase;
  
  //
  // Calculate the system memory length by memory hobs
  //
  MemoryLength  = 0x100000;

  Hob.Raw = GetFirstHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR);
  if (Hob.Raw == NULL) {
    return EFI_NOT_FOUND;
  }

  while ((Hob.Raw != NULL) && (!END_OF_HOB_LIST (Hob))) {
      if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
        //
        // Skip the memory region below 1MB
        //
        if (Hob.ResourceDescriptor->PhysicalStart >= 0x100000) {
          MemoryLength += Hob.ResourceDescriptor->ResourceLength;
        }
      }

    Hob.Raw = GET_NEXT_HOB (Hob);
    Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw);
  }
  
  //
  // Find the location of reserved page in TSEG.  This was reserved by the MRC.
  //
  GuidHob = GetFirstGuidHob (&gEfiAcpiVariableGuid);
  if (GuidHob != NULL) {
    SmramDescriptor = (EFI_SMRAM_DESCRIPTOR*) GET_GUID_HOB_DATA (GuidHob);

    //
    // Now find the location of the data structure that is used to store the address
    // of the S3 reserved memory.
    //
    AcpiS3Range = (RESERVED_ACPI_S3_RANGE*) (UINTN) (SmramDescriptor->PhysicalStart +
      RESERVED_ACPI_S3_RANGE_OFFSET);

    //
    // Allocate reserved ACPI memory for S3 resume.  Pointer to this region is
    // stored in SMRAM in the first page of TSEG.
    //
    AcpiReservedBase = AllocateAcpiNvsMemoryBelow4G (
      PcdGet32 (PcdS3AcpiReservedMemorySize)
      );
    if (AcpiReservedBase != NULL) {
      AcpiS3Range->AcpiReservedMemoryBase = (UINT32)(UINTN) AcpiReservedBase;
      AcpiS3Range->AcpiReservedMemorySize = PcdGet32 (PcdS3AcpiReservedMemorySize);
    }
    
    AcpiS3Range->SystemMemoryLength = (UINT32)MemoryLength;
    
    DEBUG ((EFI_D_ERROR, "S3 Memory  Base:    %08X\n", AcpiS3Range->AcpiReservedMemoryBase));
    DEBUG ((EFI_D_ERROR, "S3 Memory  Size:    %08X\n", AcpiS3Range->AcpiReservedMemorySize));
    DEBUG ((EFI_D_ERROR, "S3 SysMemoryLength: %08X\n", AcpiS3Range->SystemMemoryLength));  
  }
  
  return EFI_SUCCESS;

}


/**
  Initializes the SMM Handler Driver

  @param[in]   ImageHandle
  @param[in]   SystemTable

  @retval    None

**/
EFI_STATUS
EFIAPI
InitializePlatformSmm (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;
  EFI_HANDLE                                Handle;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT     PowerButtonContext;
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL   *PowerButtonDispatch;
  EFI_SMM_ICHN_DISPATCH_PROTOCOL            *IchnDispatch;
  EFI_SMM_SX_DISPATCH2_PROTOCOL             *SxDispatch;
  EFI_SMM_SX_REGISTER_CONTEXT               EntryDispatchContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL             *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT               SwContext;
  EFI_BOOT_MODE                             BootMode;

  Handle = NULL;

  //
  // Get the ACPI Base Address
  //

  mAcpiBaseAddr = PchLpcPciCfg16( R_PCH_LPC_ACPI_BASE ) & B_PCH_LPC_ACPI_BASE_BAR;

  CopyMem (&mSystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof (SYSTEM_CONFIGURATION));
  mAcLossVariable = mSystemConfiguration.StateAfterG3;
  mWakeOnRtcVariable = mSystemConfiguration.WakeOnRtcS5;

  BootMode = GetBootModeHob ();

  //
  // Get the Power Button protocol
  //
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                  NULL,
                  (VOID **) &PowerButtonDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  if (BootMode != BOOT_ON_FLASH_UPDATE) {
    //
    // Register for the power button event
    //
    PowerButtonContext.Phase = EfiPowerButtonEntry;
    Status = PowerButtonDispatch->Register (
                                    PowerButtonDispatch,
                                    PowerButtonCallback,
                                    &PowerButtonContext,
                                    &Handle
                                    );
    ASSERT_EFI_ERROR (Status);
  }
  //
  // Get the Sx dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSxDispatch2ProtocolGuid,
                  NULL,
                  (VOID **) &SxDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register entry phase call back function
  //
  EntryDispatchContext.Type  = SxS3;
  EntryDispatchContext.Phase = SxEntry;

  Status = SxDispatch->Register (
                         SxDispatch,
                         SxSleepEntryCallBack,
                         &EntryDispatchContext,
                         &Handle
                         );

  EntryDispatchContext.Type  = SxS4;

  Status = SxDispatch->Register (
                         SxDispatch,
                         S4S5CallBack,
                         &EntryDispatchContext,
                         &Handle
                         );
  ASSERT_EFI_ERROR (Status);

  EntryDispatchContext.Type  = SxS5;

  Status = SxDispatch->Register (
                         SxDispatch,
                         S4S5CallBack,
                         &EntryDispatchContext,
                         &Handle
                         );
  ASSERT_EFI_ERROR (Status);

  Status = SxDispatch->Register (
                         SxDispatch,
                         S5SleepAcLossCallBack,
                         &EntryDispatchContext,
                         &Handle
                         );
  ASSERT_EFI_ERROR (Status);

  //
  //  Get the Sw dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSwDispatch2ProtocolGuid,
                  NULL,
                  (VOID **) &SwDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register ACPI enable handler
  //
  SwContext.SwSmiInputValue = ACPI_ENABLE;
  Status = SwDispatch->Register (
                         SwDispatch,
                         EnableAcpiCallback,
                         &SwContext,
                         &Handle
                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Register ACPI disable handler
  //
  SwContext.SwSmiInputValue = ACPI_DISABLE;
  Status = SwDispatch->Register (
                         SwDispatch,
                         DisableAcpiCallback,
                         &SwContext,
                         &Handle
                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Get the ICHn protocol
  //
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmIchnDispatchProtocolGuid,
                  NULL,
                  (VOID **) &IchnDispatch
                  );
  ASSERT_EFI_ERROR (Status); 

  SetAfterG3On (TRUE);

  AllocateS3AcpiReservedMemory ();
  
  return EFI_SUCCESS;
}

/**
  @param[in]  DispatchHandle      The handle of this callback, obtained when registering
  @param[in]  DispatchContext     The predefined context which contained sleep type and phase

  @retval  EFI_SUCCESS            Operation successfully performed

**/
EFI_STATUS
SxSleepEntryCallBack (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  )

{
//
//  EFI_STATUS              Status;
//
//  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PcdGet32 (PcdProgressCodeS3SuspendStart));
//  Status = SaveRuntimeScriptTable ();
//  if (EFI_ERROR (Status)) {
//    return Status;
//  }
//
  return EFI_SUCCESS;
}

/**
  When a power button event happens, it shuts off the machine.
  
  @param[in]  DispatchHandle      The handle of this callback, obtained when registering
  @param[in]  DispatchContext     The predefined context which contained sleep type and phase
  
  @retval

**/
EFI_STATUS
PowerButtonCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{

  UINT16  data16;
  
  //
  // Set what the state to return to after AC Loss. If Last State, then
  // set it to Off.
  //
  SetAfterG3On (TRUE);
  
  if (mWakeOnRtcVariable) {
    EnableS5WakeOnRtc();
  }

  data16 = (UINT16) (IoRead16 (mAcpiBaseAddr + R_PCH_ACPI_GPE0a_EN));
  data16 &= B_PCH_ACPI_GPE0a_EN_PCI_EXP;

  //
  // Clear Sleep SMI Status
  //
  IoWrite16 (mAcpiBaseAddr + R_PCH_SMI_STS,
                (UINT16) (IoRead16 (mAcpiBaseAddr + R_PCH_SMI_STS) | B_PCH_SMI_STS_ON_SLP_EN));
  //
  // Clear Sleep Type Enable
  //
  IoWrite16 (mAcpiBaseAddr + R_PCH_SMI_EN,
                (UINT16) (IoRead16 (mAcpiBaseAddr + R_PCH_SMI_EN) & (~B_PCH_SMI_EN_ON_SLP_EN)));

  //
  // Clear Power Button Status
  //
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_PWRBTN);

  //
  // Shut it off now!
  //
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5);
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT, B_PCH_ACPI_PM1_CNT_SLP_EN | V_PCH_ACPI_PM1_CNT_S5);

  //
  // Should not return
  //
  return EFI_SUCCESS;
}

/**
  @param[in]  DispatchHandle      The handle of this callback, obtained when registering
  @param[in]  DispatchContext     The predefined context which contained sleep type and phase

**/
EFI_STATUS
EFIAPI
S5SleepAcLossCallBack (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
  return EFI_SUCCESS;
}

/**
  @param[in]  DispatchHandle      The handle of this callback, obtained when registering
  @param[in]  DispatchContext     The predefined context which contained sleep type and phase

  @retval                         Clears the Save State bit in the clock.

**/
EFI_STATUS
EFIAPI
S4S5CallBack (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
  return EFI_SUCCESS;
}

VOID
S4S5ProgClock()
{
}

/**
  SMI handler to enable ACPI mode

  Dispatched on reads from APM port with value 0xA0

  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then enabled.

   Disable SW SMI Timer

   Clear all ACPI event status and disable all ACPI events
   Disable PM sources except power button
   Clear status bits

   Disable GPE0 sources
   Clear status bits

   Disable GPE1 sources
   Clear status bits

   Guarantee day-of-month alarm is invalid (ACPI 5.0 Section 4.8.2.4 "Real Time Clock Alarm")

   Enable SCI

  @param[in]  DispatchHandle       EFI Handle
  @param[in]  DispatchContext      Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

  @retval Nothing

**/
EFI_STATUS
EFIAPI
EnableAcpiCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
  UINT16     Pm1Cnt;

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT);
  Pm1Cnt |= B_PCH_ACPI_PM1_CNT_SCI_EN;
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  
  return EFI_SUCCESS;
}

/**
  SMI handler to disable ACPI mode

  Dispatched on reads from APM port with value 0xA1

  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then disabled.
   Clear all ACPI event status and disable all ACPI events
   Disable PM sources except power button
   Clear status bits
   Disable GPE0 sources
   Clear status bits
   Disable GPE1 sources
   Clear status bits
   Disable SCI

  @param[in] DispatchHandle      EFI Handle
  @param[in] DispatchContext     Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

  @retval Nothing

**/
EFI_STATUS
EFIAPI
DisableAcpiCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
  UINT16     Pm1Cnt;

  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_STS, 0xffff);
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_EN, mPM1_SaveState16);

  IoWrite32 (mAcpiBaseAddr + R_PCH_ACPI_GPE0a_STS, 0xffffffff);
  IoWrite32 (mAcpiBaseAddr + R_PCH_ACPI_GPE0a_EN, mGPE_SaveState32);

  //
  // Disable SCI
  //
  Pm1Cnt = IoRead16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT);
  Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SCI_EN;
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  
  return EFI_SUCCESS;
}

/**
  When an unknown event happen.

  @param[in]  DispatchHandle      The handle of this callback, obtained when registering
  @param[in]  DispatchContext     The predefined context which contained sleep type and phase

  @retval None

**/
EFI_STATUS
EFIAPI
DummyTco1Callback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
   return EFI_SUCCESS;
}

UINTN
DevicePathSize (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL     *Start;

  if (DevicePath == NULL) {
    return 0;
  }

  //
  // Search for the end of the device path structure
  //
  Start = DevicePath;
  while (!IsDevicePathEnd (DevicePath)) {
    DevicePath = NextDevicePathNode (DevicePath);
  }

  //
  // Compute the size and add back in the size of the end device path structure
  //
  return ((UINTN) DevicePath - (UINTN) Start) + sizeof (EFI_DEVICE_PATH_PROTOCOL);
}

/**
  @param[in]  DispatchHandle       The handle of this callback, obtained when registering
  @param[in]  DispatchContext      The predefined context which contained sleep type and phase

**/
EFI_STATUS
S5SleepWakeOnRtcCallBack (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
  EnableS5WakeOnRtc();
  
  return EFI_SUCCESS;
}

/**
  @retval 1. Check Alarm interrupt is not set.
         2. Clear Alarm interrupt.
         2. Set RTC wake up date and time.
         2. Enable RTC wake up alarm.
         3. Enable ICH PM1 EN Bit 10(RTC_EN)

**/
VOID
EnableS5WakeOnRtc()
{
  UINT8             CmosData;
  UINTN             i;

  if (!mSystemConfiguration.WakeOnRtcS5) {
    return;
  }
  mWakeupDay = HexToBcd((UINT8) mSystemConfiguration.RTCWakeupDate);
  mWakeupHour = HexToBcd((UINT8) mSystemConfiguration.RTCWakeupTimeHour);
  mWakeupMinute = HexToBcd((UINT8) mSystemConfiguration.RTCWakeupTimeMinute);
  mWakeupSecond = HexToBcd((UINT8) mSystemConfiguration.RTCWakeupTimeSecond);

  //
  // Check RTC alarm interrupt is enabled.  If enabled, someone already
  // grabbed RTC alarm.  Just return.
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_B);
  if (IoRead8 (PCAT_RTC_DATA_REGISTER) & B_RTC_ALARM_INT_ENABLE) {
    return;
  }

  //
  // Set Date
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_D);
  CmosData = IoRead8 (PCAT_RTC_DATA_REGISTER);
  CmosData &= ~(B_RTC_DATE_ALARM_MASK);
  CmosData |= mWakeupDay;
  for (i = 0; i < 0xffff; i++) {
    IoWrite8 (PCAT_RTC_DATA_REGISTER, CmosData);
    SmmStall (1);
    if(((CmosData = IoRead8 (PCAT_RTC_DATA_REGISTER)) & B_RTC_DATE_ALARM_MASK)
         == mWakeupDay) {
      break;
    }
  }

  //
  // Set Second
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_SECOND_ALARM);
  for (i = 0; i < 0xffff; i++) {
    IoWrite8 (PCAT_RTC_DATA_REGISTER, mWakeupSecond);
    SmmStall (1);
    if (IoRead8 (PCAT_RTC_DATA_REGISTER) == mWakeupSecond) {
      break;
    }
  }

  //
  // Set Minute
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_MINUTE_ALARM);
  for (i = 0; i < 0xffff; i++) {
    IoWrite8 (PCAT_RTC_DATA_REGISTER, mWakeupMinute);
    SmmStall (1);
    if (IoRead8 (PCAT_RTC_DATA_REGISTER) == mWakeupMinute) {
      break;
    }
  }

  //
  // Set Hour
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_HOUR_ALARM);
  for (i = 0; i < 0xffff; i++) {
    IoWrite8 (PCAT_RTC_DATA_REGISTER, mWakeupHour);
    SmmStall (1);
    if (IoRead8 (PCAT_RTC_DATA_REGISTER) == mWakeupHour) {
      break;
    }
  }

  //
  // Wait for UIP to arm RTC alarm
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_A);
  while (IoRead8 (PCAT_RTC_DATA_REGISTER) & 0x80);

  //
  // Read RTC register 0C to clear pending RTC interrupts
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_C);
  IoRead8 (PCAT_RTC_DATA_REGISTER);

  //
  // Enable RTC Alarm Interrupt
  //
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_B);
  IoWrite8 (PCAT_RTC_DATA_REGISTER, IoRead8 (PCAT_RTC_DATA_REGISTER) | B_RTC_ALARM_INT_ENABLE);

  //
  // Clear ICH RTC Status
  //
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_RTC);

  //
  // Enable ICH RTC event
  //
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_EN,
              (UINT16) (IoRead16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_EN) | B_PCH_ACPI_PM1_EN_RTC));
}

UINT8
HexToBcd (
  IN UINT8 HexValue
  )
{
  UINTN   HighByte;
  UINTN   LowByte;

  HighByte    = (UINTN) HexValue / 10;
  LowByte     = (UINTN) HexValue % 10;

  return ((UINT8) (LowByte + (HighByte << 4)));
}

UINT8
BcdToHex (
  IN UINT8 BcdValue
  )
{
  UINTN   HighByte;
  UINTN   LowByte;

  HighByte    = (UINTN) ((BcdValue >> 4) * 10);
  LowByte     = (UINTN) (BcdValue & 0x0F);

  return ((UINT8) (LowByte + HighByte));
}

