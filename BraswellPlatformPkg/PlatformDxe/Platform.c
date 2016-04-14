/** @file
  Platform Initialization Driver.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformDxe.h"
#include "Platform.h"
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/ExitPmAuth.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PciLib.h>
#include <Guid/EventGroup.h>
#include <Guid/Chv2Variable.h>
#include <Library/PcdLib.h>
#include <Include/Library/BiosIdLib.h>
#include <Protocol/IgdOpRegion.h>
#include <KscLib.h>
#include <Library/GpioLib.h>
#include <SaveMemoryConfig.h>
#include <CpuRegs.h>

#define EFI_MEMORY_PORT_IO                0x4000000000000000ULL

#define WC_PMIC_READ_INSTRUCTION_DELAY 10
#define WC_PMIC_WRITE_INSTRUCTION_DELAY 100

SYSTEM_CONFIGURATION            mSystemConfiguration;
EFI_PLATFORM_INFO_HOB           mPlatformInfo;
EFI_EVENT                       mReadyToBootEvent;
UINTN                           SpiBase;

typedef struct {
  EFI_PHYSICAL_ADDRESS    BaseAddress;
  UINT64                  Length;
} MEMORY_MAP;

EFI_STATUS
EFIAPI
PolicyInitDxeEntryPoint (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  );

VOID
InitExI ();

VOID
PchInitBeforeBoot (
  );

VOID
UpdateDVMTSetup (
  );

VOID
InitRC6Policy (
  VOID
  );

UINT8
GetUsbIdDetectionStatus (
  );

VOID
InitPlatformResolution (
  VOID
  );

VOID
PlatformGcdInfoReport (
  VOID
  );

CHAR16
EFIAPI
InternalCharToUpper (
  IN      CHAR16                    Char
  );

BOOLEAN
EFIAPI
InternalIsHexaDecimalDigitCharacter (
  IN      CHAR16                    Char
  );

UINTN
EFIAPI
InternalHexCharToUintn (
  IN      CHAR16                    Char
  );

UINTN
EFIAPI
StrHex2Uintn (
  IN      CHAR16              *String
  )
{
  UINTN     Result;

  //
  // ASSERT String is less long than PcdMaximumUnicodeStringLength.
  // Length tests are performed inside StrLen().
  //
  ASSERT (StrSize (String) != 0);

  //
  // Ignore the pad spaces (space or tab)
  //
  while ((*String == L' ') || (*String == L'\t')) {
    String++;
  }

  //
  // Ignore leading Zeros after the spaces
  //
  while (*String == L'0') {
    String++;
  }

  if (InternalCharToUpper (*String) == L'X') {
    if (*(String - 1) != L'0') {
      return 0;
    }
    //
    // Skip the 'X'
    //
    String++;
  }

  Result = 0;

  while (InternalIsHexaDecimalDigitCharacter (*String)) {
    //
    // If the Hex Number represented by String overflows according
    // to the range defined by UINTN, then ASSERT().
    //
    ASSERT (Result <= ((((UINTN) ~0) - InternalHexCharToUintn (*String)) >> 4));

    Result = (Result << 4) + InternalHexCharToUintn (*String);
    String++;
  }

  return Result;
}

/**
  Read RTC content through its registers.

  @param  Address  Address offset of RTC. It is recommended to use macros such as
                   RTC_ADDRESS_SECONDS.

  @return The data of UINT8 type read from RTC.
**/
UINT8
RtcRead (
  IN  UINT8 Address
  )
{
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, (UINT8) (Address | (UINT8) (IoRead8 (PCAT_RTC_ADDRESS_REGISTER) & 0x80)));
  return IoRead8 (PCAT_RTC_DATA_REGISTER);
}

/**
  Write RTC through its registers.

  @param  Address  Address offset of RTC. It is recommended to use macros such as
                   RTC_ADDRESS_SECONDS.
  @param  Data     The content you want to write into RTC.

**/
VOID
RtcWrite (
  IN  UINT8   Address,
  IN  UINT8   Data
  )
{
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, (UINT8) (Address | (UINT8) (IoRead8 (PCAT_RTC_ADDRESS_REGISTER) & 0x80)));
  IoWrite8 (PCAT_RTC_DATA_REGISTER, Data);
}

VOID
CheckValidDate (
  VOID
  )
{
  EFI_TIME       Time;
  UINT8          Century;
  EFI_STATUS     Status;
  BIOS_ID_IMAGE  BiosIdImage;
  CHAR16         *TempString;

  TempString = NULL;
  //
  // Get the Time/Date values.
  //
  Time.Second  = RtcRead (RTC_ADDRESS_SECONDS);
  Time.Minute  = RtcRead (RTC_ADDRESS_MINUTES);
  Time.Hour    = RtcRead (RTC_ADDRESS_HOURS);
  Time.Day     = RtcRead (RTC_ADDRESS_DAY_OF_THE_MONTH);
  Time.Month   = RtcRead (RTC_ADDRESS_MONTH);
  Century      = RtcRead (RTC_ADDRESS_CENTURY);
  Time.Year    = (UINT16) ((Century * 0x100) + RtcRead (RTC_ADDRESS_YEAR));
  //
  // Perform date check and correction
  // Mark any date less than year 2005 or more than year 2050 as invalid
  //
  if ((Time.Year < 0x2005) || (Time.Year > 0x2050)) {
    DEBUG ((EFI_D_ERROR, "Invalid date, set date and time to match BIOS ID\n"));
    Status = GetBiosId (&BiosIdImage);
    if (Status == EFI_SUCCESS) {
      TempString = AllocateZeroPool (5);
      if (TempString != NULL) {
        //
        // Year
        //
        *(TempString + 0) = '2';
        *(TempString + 1) = '0';
        *(TempString + 2) = BiosIdImage.BiosIdString.TimeStamp[0];
        *(TempString + 3) = BiosIdImage.BiosIdString.TimeStamp[1];
        *(TempString + 4) = '\0';
        Time.Year = (UINT16) StrHex2Uintn (TempString);
        //
        // Month
        //
        *(TempString + 0) = BiosIdImage.BiosIdString.TimeStamp[2];
        *(TempString + 1) = BiosIdImage.BiosIdString.TimeStamp[3];
        *(TempString + 2) = '\0';
        Time.Month = (UINT8) StrHex2Uintn (TempString);
        //
        // Day
        //
        *(TempString + 0) = BiosIdImage.BiosIdString.TimeStamp[4];
        *(TempString + 1) = BiosIdImage.BiosIdString.TimeStamp[5];
        *(TempString + 2) = '\0';
        Time.Day = (UINT8) StrHex2Uintn (TempString);
        //
        // Hour
        //
        *(TempString + 0) = BiosIdImage.BiosIdString.TimeStamp[6];
        *(TempString + 1) = BiosIdImage.BiosIdString.TimeStamp[7];
        *(TempString + 2) = '\0';
        Time.Hour = (UINT8) StrHex2Uintn (TempString);
        //
        // Minute
        //
        *(TempString + 0) = BiosIdImage.BiosIdString.TimeStamp[8];
        *(TempString + 1) = BiosIdImage.BiosIdString.TimeStamp[9];
        *(TempString + 2) = '\0';
        Time.Minute = (UINT8) StrHex2Uintn (TempString);
        //
        // Program the correct date and time
        //
        RtcWrite (RTC_ADDRESS_SECONDS, Time.Second);
        RtcWrite (RTC_ADDRESS_MINUTES, Time.Minute);
        RtcWrite (RTC_ADDRESS_HOURS, Time.Hour);
        RtcWrite (RTC_ADDRESS_DAY_OF_THE_MONTH, Time.Day);
        RtcWrite (RTC_ADDRESS_MONTH, Time.Month);
        RtcWrite (RTC_ADDRESS_YEAR, (UINT8) Time.Year);
        RtcWrite (RTC_ADDRESS_CENTURY, Century);

        //
        // W/A for Hardware issue in case of CHT platform when CMOS is reset and time goes to 12hr mode
        // BIOS needs only 24hr mode
        //

        RtcWrite(RTC_ADDRESS_REGISTER_B, (RtcRead(RTC_ADDRESS_REGISTER_B) | BIT1 ));

        //
        // Free allocated TempString
        //
        FreePool (TempString);
      }
    }
  }
  DEBUG ((EFI_D_ERROR, "Date and time is now: %X/%X/%X %X:%X:%X\n", Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second));
}

#define PchLpcPciCfg32(Register)  MmioRead32 (MmPciAddress (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, Register))

/**
  This function read content of B:D:F 0:31:0, offset 44h (for PmcBase)

  @param[in] None

  @return  32 bit PmcBase

**/
UINT32
GetPmcBase (
  VOID
  )
{
  return (PchLpcPciCfg32 (R_PCH_LPC_PMC_BASE) & B_PCH_LPC_PMC_BASE_BAR);
}

/**
  Configure ExI. 

  @param    None

  @retval   EFI_SUCCESS        The driver initializes correctly.

**/
VOID
InitExI (
  )
{
  SYSTEM_CONFIGURATION          SystemConfiguration;
  
  CopyMem (&SystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));
  if (SystemConfiguration.ExISupport == 1) {
    MmioOr32 ((UINTN) (GetPmcBase() + R_PCH_PMC_MTPMC1), (UINT32) BIT0+BIT1+BIT2);
  } else if (SystemConfiguration.ExISupport == 0) {
    MmioAnd32 ((UINTN) (GetPmcBase() + R_PCH_PMC_MTPMC1), ~((UINT32) BIT0+BIT1+BIT2)); //clear bit 0,1,2
  }
}

VOID
SpiBiosProtectionFunction (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  UINTN                 mPciD31F0RegBase;
  UINTN                 BiosFlaLower = 0;
  UINTN                 BiosFlaLimit = 0x7fffff;

  gBS->CloseEvent (Event);

  BiosFlaLower = PcdGet32 (PcdFlashMicroCodeAddress)- PcdGet32 (PcdFlashAreaBaseAddress);

  mPciD31F0RegBase = MmPciAddress (0,
                       DEFAULT_PCI_BUS_NUMBER_PCH,
                       PCI_DEVICE_NUMBER_PCH_LPC,
                       PCI_FUNCTION_NUMBER_PCH_LPC,
                       0
                       );
  SpiBase          = MmioRead32(mPciD31F0RegBase + R_PCH_LPC_SPI_BASE) & B_PCH_LPC_SPI_BASE_BAR;

  //
  //Set SMM_BWP, WPD and LE bit
  //
  MmioOr32 ((UINTN) (SpiBase + R_PCH_SPI_BCR), (UINT8) B_PCH_SPI_BCR_SMM_BWP);
  MmioAnd32 ((UINTN) (SpiBase + R_PCH_SPI_BCR), (UINT8)(~B_PCH_SPI_BCR_BIOSWE));
  MmioOr32 ((UINTN) (SpiBase + R_PCH_SPI_BCR), (UINT8) B_PCH_SPI_BCR_BLE);

  //
  //First check if FLOCKDN or PR0FLOCKDN is set. No action if either of them set already.
  //
  if ((MmioRead16(SpiBase + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FLOCKDN) != 0 ||
      (MmioRead32(SpiBase + R_PCH_SPI_LOCK)& B_PCH_SPI_LOCK_PR0LOCKDN) != 0) {
    //
    // Already locked. we could take no action here
    //
    DEBUG((EFI_D_INFO, "PR0 already locked down. Stop configuring PR0.\n"));
    return;
  }

  //
  //Set PR0
  //
  MmioOr32((UINTN)(SpiBase + R_PCH_SPI_PR0),
    B_PCH_SPI_PR0_RPE|B_PCH_SPI_PR0_WPE|\
    (B_PCH_SPI_PR0_PRB_MASK&(BiosFlaLower>>12))|(B_PCH_SPI_PR0_PRL_MASK&(BiosFlaLimit>>12)<<16));

  DEBUG((EFI_D_ERROR, "SpiBiosProtectionFunction() R_PCH_SPI_FRAP [0x%x]\n", MmioRead32(SpiBase + R_PCH_SPI_FRAP)));
  
  MmioOr32 ((UINTN) (SpiBase + R_PCH_SPI_LOCK), 0x3FF);
  DEBUG((EFI_D_ERROR, "SpiBiosProtectionFunction() R_PCH_SPI_LOCK [0x%x]\n", MmioRead32(SpiBase + R_PCH_SPI_LOCK)));

  MmioOr16 ((UINTN) (SpiBase + R_PCH_SPI_HSFS), (UINT16) (B_PCH_SPI_HSFS_FLOCKDN));

  //
  // Verify if it's really locked.
  //
  if ((MmioRead16 (SpiBase + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FLOCKDN) == 0) {
    DEBUG((EFI_D_ERROR, "Failed to lock down PR0.\n"));
  }
  
  return;
}


STATIC
VOID
LockRegister (
  EFI_EVENT Event,
  VOID      *Context
  )
{

  DEBUG((EFI_D_ERROR, "LockRegister() Entry\n"));
  gBS->CloseEvent (Event);

  // Check if the Lock bit is 0x0
  if ((AsmReadMsr64(EFI_MSR_IA32_FEATURE_CONTROL) & B_EFI_MSR_IA32_FEATURE_CONTROL_LOCK) == 0) {
    AsmMsrOr64(EFI_MSR_IA32_FEATURE_CONTROL, B_EFI_MSR_IA32_FEATURE_CONTROL_LOCK);
    DEBUG((EFI_D_ERROR, "LockRegister() EFI_MSR_IA32_FEATURE_CONTROL [0x%x]\n", AsmReadMsr64(EFI_MSR_IA32_FEATURE_CONTROL)));
  }

  // Check if the bit0 is 0x0
  if ((AsmReadMsr64(0x13C) & BIT0) == 0) {
    AsmMsrOr64(0x13C, BIT0);
    DEBUG((EFI_D_ERROR, "LockRegister() [0x%x]\n", AsmReadMsr64(0x13C)));
  }

  return;
}


VOID
InitPciDevPME (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  gBS->CloseEvent (Event);

  CopyMem (&mSystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  //
  // Program HDA PME_EN
  //
  if (MmioRead32 (MmPciAddress (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_AZALIA, PCI_FUNCTION_NUMBER_PCH_AZALIA, R_PCH_HDA_DEVVENID)) != 0xFFFFFFFF) {
    PchAzaliaPciCfg32Or (R_PCH_HDA_PCS, B_PCH_HDA_PCS_PMEE);
  }

#ifdef SATA_SUPPORT
  //
  // Program SATA PME_EN
  //
  if (MmioRead32 (MmPciAddress (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA, R_PCH_SATA_ID)) != 0xFFFFFFFF) {
    PchSataPciCfg32Or (R_PCH_SATA_PMCS, B_PCH_SATA_PMCS_PMEE);
  }
#endif
}

VOID
TristateLpcGpioS0i3Config (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "TristateLpcGpioS0i3Config() - Start\n"));

  MmioWrite32 ((UINTN) (IO_BASE_ADDRESS + R_PCH_CFIO_SOUTHEAST + 0x5030), (UINT32) (0x00008300)); // MF_LPC_CLKOUT0_PAD_CFG0
  MmioWrite32 ((UINTN) (IO_BASE_ADDRESS + R_PCH_CFIO_SOUTHEAST + 0x5034), (UINT32) (0x05C00000)); // MF_LPC_CLKOUT0_PAD_CFG1

  DEBUG ((DEBUG_INFO, "TristateLpcGpioS0i3Config() - End\n"));

  return;
}

VOID
ConfigLpcTristate (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  DEBUG ((DEBUG_INFO, "ConfigLpcTristate() - Start\n"));

  //
  // CHV Sighting HSD# 4798180
  // VCCPADCF1SI0_1P8_3P3 Voltage Rail is consuming ~32mW during s0i3 state (functional baremetal s0i3 state)
  //
  TristateLpcGpioS0i3Config ();

  DEBUG ((DEBUG_INFO, "ConfigLpcTristate() - End\n"));

  return;
}

/**
  This is the standard EFI driver point for the Driver. This
  driver is responsible for setting up any platform specific policy or
  initialization information.

  @param[in]  ImageHandle     Handle for the image of this driver.
  @param[in]  SystemTable     Pointer to the EFI System Table.

  @retval   EFI_SUCCESS       Policy decisions set.

**/
EFI_STATUS
EFIAPI
InitializePlatform (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                          Status;
  EFI_HANDLE                          Handle;
  UINT8                               Buffer8;

  DEBUG ((EFI_D_INFO, "InitializePlatform\n"));

  Handle        = NULL;
  Buffer8       = 0;

  ReportStatusCodeEx (
    EFI_PROGRESS_CODE,
    EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_PLATFORM_DXE_INIT,
    0,
    &gEfiCallerIdGuid,
    NULL,
    NULL,
    0
    );

  CopyMem (&mPlatformInfo, PcdGetPtr (PcdPlatformInfo), sizeof(mPlatformInfo));
  //
  // PANEL0_BACKLIGHT_CTL (GPO) required to toggle to make HSIC Hub functional in Warm reset flow
  //
  if (mPlatformInfo.WarmResetOccured) {
    GPIOSetTXValue(CFIO_COMMUNITY_NORTH,65,LOW);
    DEBUG ((EFI_D_INFO, "HSIC: Programming Low \n"));
  }

  //
  // Check for valid date
  //
  CheckValidDate ();

  CopyMem (&mSystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  //
  // Detect the Manufacture Mode
  //
  DEBUG ((EFI_D_ERROR, "SystemConfiguration.MfgMode= %d \n",mSystemConfiguration.MfgMode));

  //
  // RC6 should be enabled if S0ix is enabled.
  //
  InitRC6Policy ();
  //
  // Create a ReadyToBoot Event to run the PME init process
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             InitPciDevPME,
             NULL,
             &mReadyToBootEvent
             );
  //
  // Create a ReadyToBoot Event to run enable PR0 and lock down
  //
  if (mSystemConfiguration.PchBiosLock == 1) {
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               SpiBiosProtectionFunction,
               NULL,
               &mReadyToBootEvent
               );
  }


  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             LockRegister,
             NULL,
             &mReadyToBootEvent
             );   
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Create ReadyToBoot event failed \n"));
  }


  InitPlatformResolution ();

  //
  // Init ExI
  //
  InitExI ();

  PlatformGcdInfoReport ();

  //
  // Process dynamic entries
  //

  PchInitBeforeBoot ();

  ReportStatusCodeEx (
    EFI_PROGRESS_CODE,
    EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_PLATFORM_DXE_INIT_DONE,
    0,
    &gEfiCallerIdGuid,
    NULL,
    NULL,
    0
    );

  //
  // PANEL0_BACKLIGHT_CTL (GPO) required to toggle to make HSIC Hub functional in Warm reset flow
  //
  if (mPlatformInfo.WarmResetOccured) {
    GPIOSetTXValue (CFIO_COMMUNITY_NORTH,65,HIGH);
    DEBUG ((EFI_D_INFO, "HSIC: Programming high \n"));
  }

  return EFI_SUCCESS;
}

// JYAO1 - move to S3
VOID
PchInitBeforeBoot()
{
  //
  // Saved SPI Opcode menu to fix EFI variable unable to write after S3 resume.
  //
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (SPI_BASE_ADDRESS + (R_PCH_SPI_OPMENU0)),
    1,
    (VOID *) (UINTN) (SPI_BASE_ADDRESS + (R_PCH_SPI_OPMENU0))
    );

  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN) (SPI_BASE_ADDRESS + (R_PCH_SPI_OPMENU1)),
    1,
    (VOID *) (UINTN) (SPI_BASE_ADDRESS + (R_PCH_SPI_OPMENU1))
    );

  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint16,
    (UINTN) (SPI_BASE_ADDRESS + R_PCH_SPI_OPTYPE),
    1,
    (VOID *) (UINTN) (SPI_BASE_ADDRESS + R_PCH_SPI_OPTYPE)
    );

  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint16,
    (UINTN) (SPI_BASE_ADDRESS + R_PCH_SPI_PREOP),
    1,
    (VOID *) (UINTN) (SPI_BASE_ADDRESS + R_PCH_SPI_PREOP)
    );

  //
  // Saved MTPMC_1 for S3 resume.
  //
  S3BootScriptSaveMemWrite (
    EfiBootScriptWidthUint32,
    (UINTN)(PMC_BASE_ADDRESS + R_PCH_PMC_MTPMC1),
    1,
    (VOID *)(UINTN)(PMC_BASE_ADDRESS + R_PCH_PMC_MTPMC1));

  return;
}

VOID
UpdateDVMTSetup (
  )
{
  // Workaround to support IIA bug.
  // IIA request to change option value to 4, 5 and 7 relatively
  // instead of 1, 2, and 3 which follow Lakeport Specs.
  // Check option value, temporary hardcode GraphicsDriverMemorySize
  // Option value to fulfill IIA requirment. So that user no need to
  // load default and update setupvariable after update BIOS.
  //   Option value hardcoded as: 1 to 4, 2 to 5, 3 to 7.
  // *This is for broadwater and above product only.
  //

  SYSTEM_CONFIGURATION        SystemConfiguration;
  UINTN                       Size;
  EFI_STATUS                  Status;

  CopyMem (&SystemConfiguration, PcdGetPtr (PcdSystemConfiguration), sizeof(SYSTEM_CONFIGURATION));

  if ((SystemConfiguration.GraphicsDriverMemorySize < 4) && !EFI_ERROR (Status)) {
    switch (SystemConfiguration.GraphicsDriverMemorySize) {
      case 1:
        SystemConfiguration.GraphicsDriverMemorySize = 4;
        break;
      case 2:
        SystemConfiguration.GraphicsDriverMemorySize = 5;
        break;
      case 3:
        SystemConfiguration.GraphicsDriverMemorySize = 7;
        break;
      default:
        break;
     }

    Size = sizeof(SYSTEM_CONFIGURATION);
    PcdSetPtr (PcdSystemConfiguration, &Size, &SystemConfiguration);
  }
}

VOID
InitRC6Policy (
  VOID
  )
{
  UINTN  Size;

  if ((mSystemConfiguration.S0ix==1) && (mSystemConfiguration.EnableRenderStandby==0)) {
    mSystemConfiguration.EnableRenderStandby=1;
    Size = sizeof(SYSTEM_CONFIGURATION);
    PcdSetPtr (PcdSystemConfiguration, &Size, &mSystemConfiguration);
  }
}

VOID
InitPlatformResolution (
  VOID
  )
{
  PANEL_RESOLUTION PanelResolution[] = {
    0, 0,     0,     //Natvie Mode, Find the highest resolution which GOP supports.
    1, 640,   480,
    2, 800,   600,
    3, 1024,  768,
    4, 1280,  1024,
    5, 1366,  768,
    6, 1680,  1050,
    7, 1920,  1200,
    8, 1280,  800,
  };

  //
  // Set the Panel Resolution.
  //
  PcdSet32 (PcdVideoHorizontalResolution, PanelResolution[mSystemConfiguration.IgdFlatPanel].HorizontalResolution);
  PcdSet32 (PcdVideoVerticalResolution, PanelResolution[mSystemConfiguration.IgdFlatPanel].VerticalResolution);

}

VOID
PlatformGcdInfoReport (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PHYSICAL_ADDRESS            BaseAddress;
  UINT64                          Length;
  UINT8                           Index;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR GcdMemorySpaceDescriptor;
  UINT64                          Attributes;

  //
  // Base address and length must exist and match
  // the entries in MemoryCallback.c
  //
  MEMORY_MAP                      MmioMap[] = {
    {PCIEX_BASE_ADDRESS,      0x4000000},
    {IO_APIC_ADDRESS,         0x1000},
    {SPI_BASE_ADDRESS,        0x1000},
    {PMC_BASE_ADDRESS,        0x1000},
    {IO_BASE_ADDRESS,         0x40000},
    {ILB_BASE_ADDRESS,        0x2000},
    {MPHY_BASE_ADDRESS,       0x100000},
    {PUNIT_BASE_ADDRESS,      0x1000},
    {RCBA_BASE_ADDRESS,       0x1000},
    {LOCAL_APIC_ADDRESS,      0x1000}
  };

  for (Index = 0; Index < sizeof (MmioMap) / (sizeof (MEMORY_MAP)); Index++) {
    BaseAddress = MmioMap[Index].BaseAddress;
    Length      = MmioMap[Index].Length;
    DEBUG ((EFI_D_ERROR, "BaseAddress = 0x%X\n", BaseAddress));
    Status      = gDS->GetMemorySpaceDescriptor (BaseAddress, &GcdMemorySpaceDescriptor);
    ASSERT_EFI_ERROR (Status);

    Attributes = GcdMemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME;
    if (MmioMap[Index].BaseAddress == PCIEX_BASE_ADDRESS) {
      Attributes |= EFI_MEMORY_PORT_IO;
    }

    if (MmioMap[Index].BaseAddress == PMC_BASE_ADDRESS) {
      Attributes |= EFI_MEMORY_UC;
    }

    Status = gDS->SetMemorySpaceAttributes (
                    BaseAddress,
                    Length,
                    Attributes
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
EFIAPI
SiliconInitDoneCallback (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Protocol;
  
  Status = gBS->LocateProtocol (
                  &gIgdOpRegionProtocolGuid,
                  NULL,
                  &Protocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  InitializePlatform (gImageHandle, gST);
}

/**
  This is the standard EFI driver point for the Driver. This
  driver is responsible for setting up any platform specific policy or
  initialization information.

  @param[in]  ImageHandle     Handle for the image of this driver.
  @param[in]  SystemTable     Pointer to the EFI System Table.

  @retval   EFI_SUCCESS       Policy decisions set.

**/
EFI_STATUS
EFIAPI
PlatformDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID  *Registration;

  PolicyInitDxeEntryPoint (ImageHandle, SystemTable);

  //
  // gIgdOpRegionProtocolGuid is installed by ChvInit
  //
  EfiCreateProtocolNotifyEvent (&gIgdOpRegionProtocolGuid, TPL_CALLBACK, SiliconInitDoneCallback, NULL, &Registration);
  
  SaveMemoryConfigEntryPoint ();
  
  EfiCreateProtocolNotifyEvent (&gExitPmAuthProtocolGuid, TPL_CALLBACK, SaveLpssSccPciMode, NULL, &Registration);
    
  return EFI_SUCCESS;
}
