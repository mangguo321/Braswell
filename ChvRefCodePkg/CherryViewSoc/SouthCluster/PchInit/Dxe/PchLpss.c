/** @file
  Initializes PCH LPSS Devices.

  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchInit.h"
#include <IndustryStandard/Acpi.h>

//
// CSRT Definitions
//
#define EFI_ACPI_CSRT_TABLE_REVISION 0x00
#define MAX_NO_CHANNEL1_SUPPORTED    7
#define MAX_NO_CHANNEL2_SUPPORTED    9
#define EFI_ACPI_OEM_ID              "INTEL "  // OEMID 6 bytes long
#define EFI_ACPI_OEM_TABLE_ID        SIGNATURE_64('E','D','K','2',' ',' ',' ',' ') // OEM table id 8 bytes long
#define EFI_ACPI_OEM_REVISION        0x00000005
#define EFI_ACPI_CREATOR_ID          SIGNATURE_32('I','N','T','L')
#define EFI_ACPI_CREATOR_REVISION    0x20120624
//
// Ensure proper structure formats for CSRT
//
#pragma pack (1)
//
// Resource  Share Info
//
typedef struct _SHARED_INFO_SECTION {
  UINT16 MajVersion;
  UINT16 MinVersion;
  UINT32 MMIOLowPart;
  UINT32 MMIOHighPart;
  UINT32 IntGSI;
  UINT8  IntPol;
  UINT8  IntMode;
  UINT8  NoOfCh;
  UINT8  DMAAddressWidth;
  UINT16 BaseReqLine;
  UINT16 NoOfHandSig;
  UINT32 MaxBlockTransferSize;
  } SHARED_INFO_SECTION;
//
// Resource Group Header
//
typedef struct _RESOURCE_GROUP_HEADER {
  UINT32 Length;
  UINT32 VendorId;
  UINT32 SubVendorId;
  UINT16 DeviceId;
  UINT16 SubDeviceId;
  UINT16 Revision;
  UINT16 Reserved;
  UINT32 SharedInfoLength;
  SHARED_INFO_SECTION SharedInfoSection;
} RESOURCE_GROUP_HEADER;
//
// Resource Descriptor Header
//
typedef struct _RESOURCE_DESCRIPTOR {
  UINT32 Length;
  UINT16 ResourceType;
  UINT16 ResourceSubType;
  UINT32 UUID;
  } RESOURCE_DESCRIPTOR;

typedef struct {
  RESOURCE_GROUP_HEADER          ResourceGroupHeaderInfo;
  RESOURCE_DESCRIPTOR            ResourceDescriptorInfo[MAX_NO_CHANNEL1_SUPPORTED];
  } RESOURCE_GROUP_INFO1;

typedef struct {
  RESOURCE_GROUP_HEADER          ResourceGroupHeaderInfo;
  RESOURCE_DESCRIPTOR            ResourceDescriptorInfo[MAX_NO_CHANNEL2_SUPPORTED];
  } RESOURCE_GROUP_INFO2;
//
// Core System Resources Table Structure (CSRT)
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER    Header;
  RESOURCE_GROUP_INFO1           ResourceGroupsInfo1;
  RESOURCE_GROUP_INFO2           ResourceGroupsInfo2;
} EFI_ACPI_CSRT_TABLE;
#pragma pack ()

/**
  Hide PCI config space of LPSS devices and do any final initialization.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] IsSetS3BootScript    Is this function called for S3 boot script save

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureLpssAtBoot (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL *PchPlatformPolicy,
  IN BOOLEAN                          IsSetS3BootScript
  )
{
  UINTN                         LpssPciMmBase;
  UINT32                        LpssMmioBase0;
  UINT32                        LpssMmioBase1;
  UINT32                        Data32And;
  UINT32                        Data32Or;
#ifndef FSP_FLAG
  EFI_STATUS                    AcpiTablePresent;
  EFI_STATUS                    Status;
  UINTN                         AcpiTableKey;
  UINT8                         Index;
  EFI_ACPI_CSRT_TABLE           *mCsrt;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
#endif
  DEBUG ((EFI_D_INFO, "ConfigureLpssAtBoot() Start\n"));

  //
  // Initialize Variables
  //
  LpssPciMmBase    = 0;
  LpssMmioBase0    = 0;
  LpssMmioBase1    = 0;
  Data32And        = 0;
  Data32Or         = 0;
#ifndef FSP_FLAG
  AcpiTablePresent = EFI_NOT_FOUND;
  AcpiTableKey     = 0;
  Index            = 0;
  Status           = EFI_SUCCESS;
  AcpiTable        = NULL;
  mCsrt            = NULL;
#endif

#ifndef FSP_FLAG
  //
  // Locate ACPI table
  //
  if (!IsSetS3BootScript) {
    AcpiTablePresent = InitializeAslUpdateLib ();
    if (!EFI_ERROR (AcpiTablePresent)) {
      if (PchPlatformPolicy->LpssConfig->Dma0Enabled != PchDisabled &&
          PchPlatformPolicy->LpssConfig->Dma1Enabled != PchDisabled) {
        //
        // Locate ACPI support protocol
        //
        DEBUG ((EFI_D_INFO, "Initialize CSRT Start\n"));
        Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
        if (EFI_ERROR (Status) || (AcpiTable == NULL)) {
          return Status;
        }
        //
        // Allocate Memory for Core System Resources Table
        //
        mCsrt = AllocateZeroPool (sizeof (EFI_ACPI_CSRT_TABLE));
        if (mCsrt == NULL) {
          return EFI_BUFFER_TOO_SMALL;
        }
        //
        // Constructing CSRT
        //
        mCsrt->Header.Signature        = EFI_ACPI_5_0_CORE_SYSTEM_RESOURCE_TABLE_SIGNATURE;
        mCsrt->Header.Length           = sizeof (EFI_ACPI_CSRT_TABLE);
        mCsrt->Header.Revision         = EFI_ACPI_CSRT_TABLE_REVISION;
        mCsrt->Header.Checksum         = 0;
        CopyMem(&mCsrt->Header.OemId, EFI_ACPI_OEM_ID, 6);
        mCsrt->Header.OemTableId       = EFI_ACPI_OEM_TABLE_ID;
        mCsrt->Header.OemRevision      = EFI_ACPI_OEM_REVISION;
        mCsrt->Header.CreatorId        = EFI_ACPI_CREATOR_ID;
        mCsrt->Header.CreatorRevision  = EFI_ACPI_CREATOR_REVISION;

        LpssPciMmBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, PCI_DEVICE_NUMBER_PCH_LPSS_DMAC0, PCI_FUNCTION_NUMBER_PCH_LPSS_DMAC, 0);
        LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR)) & B_PCH_LPSS_DMAC_BAR_BA;

        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.Length                                  = sizeof (RESOURCE_GROUP_INFO1);
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.VendorId                                = 0x4C544E49;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SubVendorId                             = 0x00000000;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.DeviceId                                = 0x9C60;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SubDeviceId                             = 0x0000;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.Revision                                = 0x0002;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.Reserved                                = 0x0000;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoLength                        = sizeof (SHARED_INFO_SECTION);
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.MajVersion            = 0x0001;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.MinVersion            = 0x0000;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.MMIOLowPart           = LpssMmioBase0;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.MMIOHighPart          = 0x00000000;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.IntGSI                = 0x0000002A;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.IntPol                = 0x02;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.IntMode               = 0x00;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.NoOfCh                = MAX_NO_CHANNEL1_SUPPORTED - 1;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.DMAAddressWidth       = 0x20;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.BaseReqLine           = 0x0000;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.NoOfHandSig           = 0x0010;
        mCsrt->ResourceGroupsInfo1.ResourceGroupHeaderInfo.SharedInfoSection.MaxBlockTransferSize  = 0x0000FFF;

        mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[0].Length           = 0x0000000C;
        mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[0].ResourceType     = 0x0003;
        mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[0].ResourceSubType  = 0x0001;
        mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[0].UUID             = 0x20495053; //SPI

        for (Index = 1; Index < MAX_NO_CHANNEL1_SUPPORTED; Index++) {
          mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[Index].Length           = 0x0000000C;
          mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[Index].ResourceType     = 0x0003;
          mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[Index].ResourceSubType  = 0x0000;
          mCsrt->ResourceGroupsInfo1.ResourceDescriptorInfo[Index].UUID             = 0x30414843 + ((Index - 1) << 24); //CHAn
        }

        LpssPciMmBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, PCI_DEVICE_NUMBER_PCH_LPSS_DMAC1, PCI_FUNCTION_NUMBER_PCH_LPSS_DMAC, 0);
        LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR)) & B_PCH_LPSS_DMAC_BAR_BA;

        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.Length                                  = sizeof (RESOURCE_GROUP_INFO2);
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.VendorId                                = 0x4C544E49;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SubVendorId                             = 0x00000000;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.DeviceId                                = 0x9C60;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SubDeviceId                             = 0x0000;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.Revision                                = 0x0003;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.Reserved                                = 0x0000;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoLength                        = sizeof (SHARED_INFO_SECTION);
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.MajVersion            = 0x0001;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.MinVersion            = 0x0000;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.MMIOLowPart           = LpssMmioBase0;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.MMIOHighPart          = 0x00000000;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.IntGSI                = 0x0000002B;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.IntPol                = 0x02;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.IntMode               = 0x00;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.NoOfCh                = MAX_NO_CHANNEL2_SUPPORTED - 1;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.DMAAddressWidth       = 0x20;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.BaseReqLine           = 0x0010;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.NoOfHandSig           = 0x0010;
        mCsrt->ResourceGroupsInfo2.ResourceGroupHeaderInfo.SharedInfoSection.MaxBlockTransferSize  = 0x0000FFF;

        mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[0].Length           = 0x0000000C;
        mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[0].ResourceType     = 0x0003;
        mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[0].ResourceSubType  = 0x0001;
        mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[0].UUID             = 0x20433249; //I2C

        for (Index = 1; Index < MAX_NO_CHANNEL2_SUPPORTED; Index++) {
          mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[Index].Length           = 0x0000000C;
          mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[Index].ResourceType     = 0x0003;
          mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[Index].ResourceSubType  = 0x0000;
          mCsrt->ResourceGroupsInfo2.ResourceDescriptorInfo[Index].UUID             = 0x30414843 + ((Index - 1) << 24); //CHAn
        }
        //
        // After including CSRT.asl in Acpitables.inf we are getting 2 CSRT tables. First is from the CRST.asl and the
        // second is by installing CSRT table below. So remove the bleow code to get only one CSRT table i.e. from CSRT.asl.
        //
        DEBUG ((EFI_D_INFO, "Initialize CSRT End\n"));
      }
    }
  }
  //
  // Update LPSS devices ACPI variables
  //
  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &GlobalNvsArea
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
#endif
  //
  // LPSS2 DMA
  //
  if (PchPlatformPolicy->LpssConfig->Dma1Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_DMAC1,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_DMAC,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR)) & B_PCH_LPSS_DMAC_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_DMAC_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR1)) & B_PCH_LPSS_DMAC_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_DMAC_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_DMAC_STSCMD_INTRDIS | B_PCH_LPSS_DMAC_STSCMD_BME | B_PCH_LPSS_DMAC_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->LDMA2Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->LDMA2Len   = V_PCH_LPSS_DMAC_BAR_SIZE;
      GlobalNvsArea->Area->LDMA21Addr = LpssMmioBase1;
      GlobalNvsArea->Area->LDMA21Len  = V_PCH_LPSS_DMAC_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xA0,
        0x280,
        0xFFFFFFFF,
        (BIT21 | BIT20),
        0x06,
        0x07
      );

      PchMsgBusAndThenOr32 (
        0xA0,
        0x154,
        (UINT32) ~(BIT3),
        (0x00),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_DMAC_STSCMD_INTRDIS | B_PCH_LPSS_DMAC_STSCMD_BME | B_PCH_LPSS_DMAC_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x280,
        0xFFFFFFFF,
        (BIT21 | BIT20),
        0x06,
        0x07
      );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x154,
        (UINT32) ~(BIT3),
        (0x00),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS2 I2C 0
  //
  if (PchPlatformPolicy->LpssConfig->I2C0Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C0,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C1Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C1Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C11Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C11Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x288,
        0xFFFFFFFF,
        (BIT21 | BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x288,
        0xFFFFFFFF,
        (BIT21 | BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS2 I2C 1
  //
  if (PchPlatformPolicy->LpssConfig->I2C1Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C1,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C2Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C2Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C21Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C21Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x290,
        0xFFFFFFFF,
        (BIT21 | BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xA0,
        0x290,
        0xFFFFFFFF,
        (BIT21 | BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS2 I2C 2
  //
  if (PchPlatformPolicy->LpssConfig->I2C2Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C2,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C3Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C3Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C31Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C31Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x298,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x298,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS2 I2C 3
  //
  if (PchPlatformPolicy->LpssConfig->I2C3Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C3,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C4Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C4Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C41Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C41Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x2a0,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x2a0,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS I2C 4
  //
  if (PchPlatformPolicy->LpssConfig->I2C4Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C4,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C5Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C5Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C51Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C51Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x2a8,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x2a8,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS2 I2C 5
  //
  if (PchPlatformPolicy->LpssConfig->I2C5Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C5,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C6Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C6Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C61Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C61Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x2b0,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x2b0,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS2 I2C 6
  //
  if (PchPlatformPolicy->LpssConfig->I2C6Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_I2C,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_I2C6,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR)) & B_PCH_LPSS_I2C_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_I2C_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1)) & B_PCH_LPSS_I2C_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_I2C_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->I2C7Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->I2C7Len   = V_PCH_LPSS_I2C_BAR_SIZE;
      GlobalNvsArea->Area->I2C71Addr = LpssMmioBase1;
      GlobalNvsArea->Area->I2C71Len  = V_PCH_LPSS_I2C_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x2b8,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_I2C_STSCMD_INTRDIS | B_PCH_LPSS_I2C_STSCMD_BME | B_PCH_LPSS_I2C_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_I2C_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x2b8,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 DMA
  //
  if (PchPlatformPolicy->LpssConfig->Dma0Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_DMAC0,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_DMAC,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR)) & B_PCH_LPSS_DMAC_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_DMAC_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR1)) & B_PCH_LPSS_DMAC_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_DMAC_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_DMAC_STSCMD_INTRDIS | B_PCH_LPSS_DMAC_STSCMD_BME | B_PCH_LPSS_DMAC_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->LDMA1Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->LDMA1Len   = V_PCH_LPSS_DMAC_BAR_SIZE;
      GlobalNvsArea->Area->LDMA11Addr = LpssMmioBase1;
      GlobalNvsArea->Area->LDMA11Len  = V_PCH_LPSS_DMAC_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x240,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );

      PchMsgBusAndThenOr32 (
        0xa0,
        0x154,
        (UINT32) ~(BIT2),
        (0x00),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_DMAC_STSCMD_INTRDIS | B_PCH_LPSS_DMAC_STSCMD_BME | B_PCH_LPSS_DMAC_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_DMAC_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x240,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x154,
        (UINT32) ~(BIT2),
        (0x00),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 PWM 0
  //
  if (PchPlatformPolicy->LpssConfig->Pwm0Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_PWM,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_PWM0,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR)) & B_PCH_LPSS_PWM_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_PWM_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR1)) & B_PCH_LPSS_PWM_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_PWM_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_PWM_STSCMD_INTRDIS | B_PCH_LPSS_PWM_STSCMD_BME | B_PCH_LPSS_PWM_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->PWM1Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->PWM1Len   = V_PCH_LPSS_PWM_BAR_SIZE;
      GlobalNvsArea->Area->PWM11Addr = LpssMmioBase1;
      GlobalNvsArea->Area->PWM11Len  = V_PCH_LPSS_PWM_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x248,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_PWM_STSCMD_INTRDIS | B_PCH_LPSS_PWM_STSCMD_BME | B_PCH_LPSS_PWM_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x248,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 PWM 1
  //
  if (PchPlatformPolicy->LpssConfig->Pwm1Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_PWM,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_PWM1,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR)) & B_PCH_LPSS_PWM_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_PWM_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR1)) & B_PCH_LPSS_PWM_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_PWM_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_PWM_STSCMD_INTRDIS | B_PCH_LPSS_PWM_STSCMD_BME | B_PCH_LPSS_PWM_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->PWM2Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->PWM2Len   = V_PCH_LPSS_PWM_BAR_SIZE;
      GlobalNvsArea->Area->PWM21Addr = LpssMmioBase1;
      GlobalNvsArea->Area->PWM21Len  = V_PCH_LPSS_PWM_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x250,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
       
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_PWM_STSCMD_INTRDIS | B_PCH_LPSS_PWM_STSCMD_BME | B_PCH_LPSS_PWM_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_PWM_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x250,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 HS-UART 0
  //
  if (PchPlatformPolicy->LpssConfig->Hsuart0Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_HSUART,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_HSUART0,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR)) & B_PCH_LPSS_HSUART_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_HSUART_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR1)) & B_PCH_LPSS_HSUART_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_HSUART_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_HSUART_STSCMD_INTRDIS | B_PCH_LPSS_HSUART_STSCMD_BME | B_PCH_LPSS_HSUART_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->UART1Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->UART1Len   = V_PCH_LPSS_HSUART_BAR_SIZE;
      GlobalNvsArea->Area->UART11Addr = LpssMmioBase1;
      GlobalNvsArea->Area->UART11Len  = V_PCH_LPSS_HSUART_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x258,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_HSUART_STSCMD_INTRDIS | B_PCH_LPSS_HSUART_STSCMD_BME | B_PCH_LPSS_HSUART_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x258,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 HS-UART 1
  //
  if (PchPlatformPolicy->LpssConfig->Hsuart1Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_HSUART,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_HSUART1,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR)) & B_PCH_LPSS_HSUART_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_HSUART_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR1)) & B_PCH_LPSS_HSUART_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_HSUART_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_HSUART_STSCMD_INTRDIS | B_PCH_LPSS_HSUART_STSCMD_BME | B_PCH_LPSS_HSUART_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_STSCMD),
        (UINT32) (B_PCH_LPSS_HSUART_STSCMD_INTRDIS | B_PCH_LPSS_HSUART_STSCMD_BME | B_PCH_LPSS_HSUART_STSCMD_MSE)
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->UART2Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->UART2Len   = V_PCH_LPSS_HSUART_BAR_SIZE;
      GlobalNvsArea->Area->UART21Addr = LpssMmioBase1;
      GlobalNvsArea->Area->UART21Len  = V_PCH_LPSS_HSUART_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x260,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_HSUART_STSCMD_INTRDIS | B_PCH_LPSS_HSUART_STSCMD_BME | B_PCH_LPSS_HSUART_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_HSUART_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x260,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 SPI
  //
  if (PchPlatformPolicy->LpssConfig->Spi1Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_SPI,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_SPI,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR)) & B_PCH_LPSS_SPI_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_SPI_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR1)) & B_PCH_LPSS_SPI_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_SPI_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_SPI_STSCMD_INTRDIS | B_PCH_LPSS_SPI_STSCMD_BME | B_PCH_LPSS_SPI_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->SPIAddr  = LpssMmioBase0;
      GlobalNvsArea->Area->SPILen   = V_PCH_LPSS_SPI_BAR_SIZE;
      GlobalNvsArea->Area->SPI1Addr = LpssMmioBase1;
      GlobalNvsArea->Area->SPI1Len  = V_PCH_LPSS_SPI_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x268,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_SPI_STSCMD_INTRDIS | B_PCH_LPSS_SPI_STSCMD_BME | B_PCH_LPSS_SPI_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x268,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 SPI 2
  //
  if (PchPlatformPolicy->LpssConfig->Spi2Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_SPI,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_SPI2,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR)) & B_PCH_LPSS_SPI_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_SPI_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR1)) & B_PCH_LPSS_SPI_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_SPI_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_SPI_STSCMD_INTRDIS | B_PCH_LPSS_SPI_STSCMD_BME | B_PCH_LPSS_SPI_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->SPI2Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->SPI2Len   = V_PCH_LPSS_SPI_BAR_SIZE;
      GlobalNvsArea->Area->SPI21Addr = LpssMmioBase1;
      GlobalNvsArea->Area->SPI21Len  = V_PCH_LPSS_SPI_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x270,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_SPI_STSCMD_INTRDIS | B_PCH_LPSS_SPI_STSCMD_BME | B_PCH_LPSS_SPI_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x270,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  //
  // LPSS1 SPI 3
  //
  if (PchPlatformPolicy->LpssConfig->Spi3Enabled == PchAcpiMode) {

    LpssPciMmBase = MmPciAddress (
                      0,
                      PchPlatformPolicy->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_LPSS_SPI,
                      PCI_FUNCTION_NUMBER_PCH_LPSS_SPI3,
                      0
                    );
    LpssMmioBase0 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR)) & B_PCH_LPSS_SPI_BAR_BA;
    ASSERT ((LpssMmioBase0 != 0) && (LpssMmioBase0 != B_PCH_LPSS_SPI_BAR_BA));
    LpssMmioBase1 = MmioRead32 ((UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR1)) & B_PCH_LPSS_SPI_BAR1_BA;
    ASSERT ((LpssMmioBase1 != 0) && (LpssMmioBase1 != B_PCH_LPSS_SPI_BAR1_BA));
    if (!IsSetS3BootScript) {

      Data32Or  = (UINT32) (B_PCH_LPSS_SPI_STSCMD_INTRDIS | B_PCH_LPSS_SPI_STSCMD_BME | B_PCH_LPSS_SPI_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      MmioOr32 (
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_STSCMD),
        Data32Or
      );
#ifndef FSP_FLAG
      //
      // Update BAR and length in ACPI table.
      //
      GlobalNvsArea->Area->SPI3Addr  = LpssMmioBase0;
      GlobalNvsArea->Area->SPI3Len   = V_PCH_LPSS_SPI_BAR_SIZE;
      GlobalNvsArea->Area->SPI31Addr = LpssMmioBase1;
      GlobalNvsArea->Area->SPI31Len  = V_PCH_LPSS_SPI_BAR1_SIZE;
#endif

      PchMsgBusAndThenOr32 (
        0xa0,
        0x278,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    } else {
      //
      // Set S3 Boot Script
      //
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR),
        1,
        &LpssMmioBase0
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_BAR1),
        1,
        &LpssMmioBase1
        );
      Data32Or  = (UINT32) (B_PCH_LPSS_SPI_STSCMD_INTRDIS | B_PCH_LPSS_SPI_STSCMD_BME | B_PCH_LPSS_SPI_STSCMD_MSE);
      Data32And = 0xFFFFFFFF;
      S3BootScriptSaveMemReadWrite (
        EfiBootScriptWidthUint32,
        (UINTN) (LpssPciMmBase + R_PCH_LPSS_SPI_STSCMD),
        &Data32Or, // OR mask
        &Data32And // AND mask
        );
      SetMsgBusAndThenOr32S3Item (
        0xa0,
        0x278,
        0xFFFFFFFF,
        (BIT21| BIT20),
        0x06,
        0x07
      );
    }
  }

  DEBUG ((EFI_D_INFO, "ConfigureLpssAtBoot() End\n"));

  return EFI_SUCCESS;
}
