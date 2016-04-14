/** @file
  PCH SPI Common Driver implements the SPI Host Controller Compatibility Interface.

  Copyright (c) 2008 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PchSpi.h"

/**
  Initialize an SPI protocol instance.
  The function will assert in debug if SpiBase has not been initialized.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval EFI_SUCCESS             The protocol instance was properly initialized
  @exception EFI_UNSUPPORTED      The PCH is not supported by this module

**/
EFI_STATUS
SpiProtocolConstructor (
  SPI_INSTANCE          *SpiInstance
  )
{
  UINT32 SpiBar;

  //
  // Check if the current SoC is known and supported by this code
  //
  if (!IsPchSupported ()) {
    DEBUG ((EFI_D_ERROR, "This SPI Protocol not supported due to no proper SoC found!\n"));
    return EFI_UNSUPPORTED;
  }
  //
  // Initialize the SPI protocol instance
  //
  SpiInstance->Signature            = PCH_SPI_PRIVATE_DATA_SIGNATURE;
  SpiInstance->Handle               = NULL;
  SpiInstance->SpiProtocol.ReadId   = SpiProtocolReadId;
  SpiInstance->SpiProtocol.Init     = SpiProtocolInit;
  SpiInstance->SpiProtocol.Lock     = SpiProtocolLock;
  SpiInstance->SpiProtocol.Execute  = SpiProtocolExecute;

  SpiBar                            = MmioRead32 (
                                        MmPciAddress (0,
                                          DEFAULT_PCI_BUS_NUMBER_PCH,
                                          PCI_DEVICE_NUMBER_PCH_LPC,
                                          PCI_FUNCTION_NUMBER_PCH_LPC,
                                          R_PCH_LPC_SPI_BASE
                                        )
                                      );
  SpiInstance->SpiBase              = SpiBar & B_PCH_LPC_SPI_BASE_BAR;
  //
  // Let's perform a check if SpiBase is not programmed or is disabled.
  //
  ASSERT ((SpiInstance->SpiBase != 0) && (SpiInstance->SpiBase != B_PCH_LPC_SPI_BASE_BAR));

  //
  // Let's perform another check to see if SpiBase is enabled.
  //
  ASSERT ((SpiBar & B_PCH_LPC_SPI_BASE_EN) == B_PCH_LPC_SPI_BASE_EN);

  return EFI_SUCCESS;
}

/**
  JEDEC Read IDs from SPI flash part, this function will return 1-byte Vendor ID and 2-byte Device ID.

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] Address              This value is for determines the command is sent to SPI Component 1 or 2
  @param[in, out] Buffer          Pointer to caller-allocated buffer containing the data received or sent during the SPI cycle.

  @retval EFI_SUCCESS             Read Jedec Id completed.
  @retval EFI_DEVICE_ERROR        Device error, operation failed.
  @exception EFI_UNSUPPORTED      This function is unsupport after SpiProtocolInit is called

**/
EFI_STATUS
EFIAPI
SpiProtocolReadId (
  IN EFI_SPI_PROTOCOL     *This,
  IN     UINTN            Address,
  IN OUT UINT8            *Buffer
  )
{
  EFI_STATUS                Status;
  SPI_INSTANCE              *SpiInstance;
  UINTN                     SpiBase;
  UINT16                    OpcodeType;
  UINT8                     Code;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  if (SpiInstance->SpiIdTable[0].VendorId != 0) {
    DEBUG ((EFI_D_ERROR, "This function is unsupport after SpiProtocolInit is called, please use SpiProtocolExecute to get Jedec ID!\n"));
    return EFI_UNSUPPORTED;
  }

  SpiBase = SpiInstance->SpiBase;
  SpiInstance->SpiInitTable.SpiCmdConfig[0].Operation = EnumSpiOperationJedecId;
  SpiInstance->SpiInitTable.SpiCmdConfig[0].Frequency = EnumSpiCycle33MHz;
  OpcodeType = (UINT16) (V_PCH_SPI_OPTYPE_RDNOADDR);
  Code       = PCH_SPI_COMMAND_READ_ID;

  ///
  /// Set Opcode Menu Configuration registers.
  /// Need to be done before sending any SPI command
  ///
  MmioWrite8 (SpiBase + R_PCH_SPI_OPMENU0, Code);
  MmioRead8 (SpiBase + R_PCH_SPI_OPMENU0);
  ///
  /// Set Opcode Type Configuration registers.
  ///
  MmioWrite16 (SpiBase + R_PCH_SPI_OPTYPE, OpcodeType);
  MmioRead16 (SpiBase + R_PCH_SPI_OPTYPE);

  Status = SpiProtocolExecute (
            This,
            0,
            0,
            TRUE,
            TRUE,
            FALSE,
            Address,
            3,
            Buffer,
            EnumSpiRegionAll
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Get Descriptor values from the Descriptor Region.

  @param[in] This                 A pointer to "EFI_SPI_PROTOCOL" for issuing commands

**/
VOID
EFIAPI
GetDescriptorValues (
  IN      EFI_SPI_PROTOCOL      *This
  )
{
  SPI_INSTANCE  *SpiInstance;
  UINTN         SpiBase;

  SpiInstance  = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase      = SpiInstance->SpiBase;

  ///
  /// Select to Flash Map 0 Register to get the number of flash Component
  ///
  MmioAndThenOr32 (
    SpiBase + R_PCH_SPI_FDOC,
    (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
    (UINT32) (V_PCH_SPI_FDOC_FDSS_FSDM | R_PCH_SPI_FDBAR_FLASH_MAP0)
    );

  ///
  /// Copy Zero based Number Of Components
  ///
  SpiInstance->SpiDescriptor.NumberComponents = (UINT8) ((MmioRead16 (SpiBase + R_PCH_SPI_FDOD) & B_PCH_SPI_FDBAR_NC) >> N_PCH_SPI_FDBAR_NC);

  ///
  /// Select to Flash Components Register to get the Component 1 Density
  ///
  MmioAndThenOr32 (
    SpiBase + R_PCH_SPI_FDOC,
    (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
    (UINT32) (V_PCH_SPI_FDOC_FDSS_COMP | R_PCH_SPI_FCBA_FLCOMP)
    );

  ///
  /// Copy Component 1 Density
  ///
  SpiInstance->SpiDescriptor.Comp1Density = (UINT8) MmioRead32 (SpiBase + R_PCH_SPI_FDOD) & B_PCH_SPI_FLCOMP_COMP1_MASK;
}

/**
  Get VSCC values from the Descriptor Region (VSCC Table).

  @param[in] This                   A pointer to "EFI_SPI_PROTOCOL" for issuing commands
  @param[in] ReadDataCmdOpcodeIndex The index of the opcode - "PCH_SPI_COMMAND_READ_DATA"
  @param[in, out] Vscc0Value        VSCC0 (Vendor Specific Component Capabilities) Value
  @param[in, out] Vscc1Value        VSCC1 (Vendor Specific Component Capabilities) Value

  @retval EFI_SUCCESS               Found the VSCC values on Descriptor Region
  @retval EFI_NOT_FOUND             Couldn't find the VSCC values on Descriptor Region
  @retval EFI_UNSUPPORTED           ReadDataCmdOpcodeIndex is out of range
  
**/
EFI_STATUS
EFIAPI
GetDescriptorVsccValues (
  IN      EFI_SPI_PROTOCOL      *This,
  IN      UINT8                 ReadDataCmdOpcodeIndex,
  IN OUT  UINT32                *Vscc0Value,
  IN OUT  UINT32                *Vscc1Value
  )
{
  UINT32        SpiDescFlashUpperMap1;
  UINT32        VsccTableBaseAddr;
  UINT32        VsccTableLength;
  UINT32        JedecIdRegIndex;
  EFI_STATUS    Status;
  UINT32        FlashDescriptor;
  SPI_INSTANCE  *SpiInstance;
  BOOLEAN       MatchedVtbEntryFound;
  UINT8         SpiIndex;
  UINT32        Data32;

  Data32 = 0;

  if (ReadDataCmdOpcodeIndex >= SPI_NUM_OPCODE) {
    return EFI_UNSUPPORTED;
  }

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);

  Status = SpiProtocolExecute (
             This,
             ReadDataCmdOpcodeIndex,
             0,
             TRUE,
             TRUE,
             FALSE,
             (UINTN) R_PCH_SPI_FLASH_UMAP1,
             sizeof (SpiDescFlashUpperMap1),
             (UINT8 *) &SpiDescFlashUpperMap1,
             EnumSpiRegionDescriptor
             );
  if ((EFI_ERROR (Status)) || (SpiDescFlashUpperMap1 == 0xFFFFFFFF)) {
    return EFI_NOT_FOUND;
  }
  ///
  /// B_PCH_SPI_FLASH_UMAP1_VTBA represents address bits [11:4]
  ///
  VsccTableBaseAddr = ((SpiDescFlashUpperMap1 & B_PCH_SPI_FLASH_UMAP1_VTBA) << 4);
  ///
  /// Multiplied by 4? B_PCH_SPI_FDBAR_VTL is the 1-based number of DWORDs.
  ///
  VsccTableLength = (((SpiDescFlashUpperMap1 & B_PCH_SPI_FLASH_UMAP1_VTL) >> 8) << 2);
  if (VsccTableLength < SIZE_OF_SPI_VTBA_ENTRY) {
    ///
    /// Non-existent or invalid Vscc Table
    ///
    return EFI_NOT_FOUND;
  }

  for (SpiIndex = 0; SpiIndex <= SpiInstance->SpiDescriptor.NumberComponents; SpiIndex++) {
    JedecIdRegIndex       = 0;
    MatchedVtbEntryFound  = FALSE;
    while (JedecIdRegIndex <= (VsccTableLength - SIZE_OF_SPI_VTBA_ENTRY)) {
      Status = SpiProtocolExecute (
                 This,
                 ReadDataCmdOpcodeIndex,
                 0,
                 TRUE,
                 TRUE,
                 FALSE,
                 (UINTN) (VsccTableBaseAddr + JedecIdRegIndex),
                 sizeof (UINT32),
                 (UINT8 *) &FlashDescriptor,
                 EnumSpiRegionDescriptor
                 );

      if ((EFI_ERROR (Status)) || (FlashDescriptor == 0xFFFFFFFF)) {
        break;
      }

      if (((FlashDescriptor & B_PCH_SPI_VTBA_JID0_VID) != SpiInstance->SpiIdTable[SpiIndex].VendorId) ||
         (((FlashDescriptor & B_PCH_SPI_VTBA_JID0_DID0) >> N_PCH_SPI_VTBA_JID0_DID0)
        != SpiInstance->SpiIdTable[SpiIndex].DeviceId0) ||
         (((FlashDescriptor & B_PCH_SPI_VTBA_JID0_DID1) >> N_PCH_SPI_VTBA_JID0_DID1)
        != SpiInstance->SpiIdTable[SpiIndex].DeviceId1)) {
        JedecIdRegIndex += SIZE_OF_SPI_VTBA_ENTRY;
      } else {
        MatchedVtbEntryFound = TRUE;
        break;
      }
    }

    if (!MatchedVtbEntryFound) {
      return EFI_NOT_FOUND;
    }

    Status = SpiProtocolExecute (
               This,
               ReadDataCmdOpcodeIndex,
               0,
               TRUE,
               TRUE,
               FALSE,
               (UINTN) (VsccTableBaseAddr + JedecIdRegIndex + R_PCH_SPI_VTBA_VSCC0),
               sizeof (UINT32),
               (UINT8 *) &Data32,
               EnumSpiRegionDescriptor
               );
    if ((EFI_ERROR (Status)) || (Data32 == 0xFFFFFFFF)) {
      return EFI_NOT_FOUND;
    }
    ///
    /// Copy correct VSCCn value
    ///
    if (SpiIndex == 0) {
      *Vscc0Value = Data32;
    } else {
      *Vscc1Value = Data32;
      return EFI_SUCCESS;
    }
  }

  return EFI_SUCCESS;
}

/**
  Discover if the flash parts supports 4KB Block/Sector erase size.

  @param[in] This                 A pointer to "EFI_SPI_PROTOCOL" for issuing commands

  @retval EFI_SUCCESS             The flash part supports 4KB erase size.
  @retval EFI_UNSUPPORTED         The flash part does not support 4KB erase size.

**/
EFI_STATUS
EFIAPI
SpiDiscoveryParameters (
  IN      EFI_SPI_PROTOCOL      *This
  )
{
  UINT16        ParameterTableIndex;
  UINT32        Data32;
  SPI_INSTANCE  *SpiInstance;
  UINT8         SpiIndex;
  UINTN         SpiBase;

  SpiInstance  = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase      = SpiInstance->SpiBase;

  ///
  /// Check if valid SFDP table is present for SPI0
  ///
  Data32 = MmioRead32 ((UINTN) (SpiBase + R_PCH_SPI_VSCC0)) & B_PCH_SPI_VSCC0_CPPTV;
  if (Data32 == 0) {
    return EFI_UNSUPPORTED;
  }

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);

  ///
  /// Check if valid SFDP table is present for SPI1
  ///
  if (SpiInstance->SpiDescriptor.NumberComponents) {
    Data32 = MmioRead32 ((UINTN) (SpiBase + R_PCH_SPI_VSCC1)) & B_PCH_SPI_VSCC1_CPPTV;
    if (Data32 == 0) {
      ///
      /// Program VSCCn values from Flash Descriptor or internal BIOS table
      /// if both parts do not support SFDP
      ///
      return EFI_UNSUPPORTED;
    }
  }

  for (SpiIndex = 0; SpiIndex <= SpiInstance->SpiDescriptor.NumberComponents; SpiIndex++) {
    ///
    /// Read Block/Sector Erase Size in 1st dword in the Flash Parameter Table.
    ///
    ParameterTableIndex = (SpiIndex << N_PCH_SPI_PTINX_SPT) | (V_PCH_SPI_PTINX_HORD_DATA << N_PCH_SPI_PTINX_HORD);
    MmioWrite32 (SpiBase + R_PCH_SPI_PTINX, ParameterTableIndex);
    Data32 = MmioRead32 (SpiBase + R_PCH_SPI_PTDATA) & B_PCH_SPI_VSCC0_BSES_MASK;

    ///
    /// Program VSCCn.EO from Flash Descriptor or internal BIOS table
    /// if erase size other than 4KB.
    ///
    if (Data32 != V_PCH_SPI_VSCC0_BSES_4K) {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialize the host controller to execute SPI command.

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] InitData             Initialization data to be programmed into the SPI host controller.

  @retval EFI_SUCCESS             Initialization completed.
  @retval EFI_ACCESS_DENIED       The SPI static configuration interface has been locked-down.
  @retval EFI_INVALID_PARAMETER   Bad input parameters.
  @retval EFI_UNSUPPORTED         Can't get Descriptor mode VSCC values.

**/
EFI_STATUS
EFIAPI
SpiProtocolInit (
  IN EFI_SPI_PROTOCOL       *This,
  IN SPI_INIT_DATA          *InitData
  )
{
  EFI_STATUS                Status;
  UINT8                     Index;
  UINT16                    OpcodeType;
  SPI_INSTANCE              *SpiInstance;
  SPI_SPECIAL_OPCODE_ENTRY  *SpecialOpcodeEntry;
  UINT32                    Vscc0Value;
  UINT32                    Vscc1Value;
  UINTN                     SpiBase;
  UINT8                     UnlockCmdOpcodeIndex;
  UINT8                     ReadDataCmdOpcodeIndex;
  UINT8                     JedecIdCmdOpcodeIndex;
  UINT8                     Code;
  UINT8                     FlashPartId[3];
  UINT32                    Data32;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase     = SpiInstance->SpiBase;
  Vscc0Value  = 0;
  Vscc1Value  = 0;

  if (InitData == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Check if the SPI interface has been locked-down.
  ///
  if ((MmioRead16 (SpiBase + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FLOCKDN) != 0) {
    ASSERT_EFI_ERROR (EFI_ACCESS_DENIED);
    return EFI_ACCESS_DENIED;
  }
  ///
  /// Copy Flash Descriptor Values into SPI driver Private data structure
  ///
  SpiInstance->DescriptorMode = PchIsSpiDescriptorMode (SpiBase);
  if (SpiInstance->DescriptorMode == TRUE) {
    GetDescriptorValues (This);
  }
  ///
  /// Clear all the status bits for hardware regs.
  ///
  MmioOr16 (
    (UINTN) (SpiBase + R_PCH_SPI_HSFS),
    (UINT16) ((B_PCH_SPI_HSFS_AEL | B_PCH_SPI_HSFS_FCERR | B_PCH_SPI_HSFS_FDONE))
    );
  MmioRead16 (SpiBase + R_PCH_SPI_HSFS);

  ///
  /// Clear all the status bits for software registers.
  ///
  MmioOr8 (
    (UINTN) (SpiBase + R_PCH_SPI_SSFCS),
    (UINT8) ((B_PCH_SPI_SSFCS_FCERR | B_PCH_SPI_SSFCS_CDS))
    );
  MmioRead8 (SpiBase + R_PCH_SPI_SSFCS);

  ReadDataCmdOpcodeIndex  = SPI_NUM_OPCODE;
  UnlockCmdOpcodeIndex    = SPI_NUM_OPCODE;
  JedecIdCmdOpcodeIndex   = SPI_NUM_OPCODE;
  ///
  /// Set Opcode Type Configuration registers.
  /// Need to be done before sending any SPI command
  ///
  for (Index = 0, OpcodeType = 0; Index < SPI_NUM_OPCODE; Index++) {
    ///
    /// Copy Operation and Frequency into SPI driver Private data structure
    ///
    SpiInstance->SpiInitTable.SpiCmdConfig[Index].Operation = InitData->SpiCmdConfig[Index].Operation;
    SpiInstance->SpiInitTable.SpiCmdConfig[Index].Frequency = InitData->SpiCmdConfig[Index].Frequency;

    switch (SpiInstance->SpiInitTable.SpiCmdConfig[Index].Operation) {
      case EnumSpiOperationReadData:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_RDADDR << (Index * 2));
        Code                    = PCH_SPI_COMMAND_READ_DATA;
        ReadDataCmdOpcodeIndex  = Index;
        break;

      case EnumSpiOperationFastRead:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_RDADDR << (Index * 2));
        Code                    = PCH_SPI_COMMAND_FAST_READ;
        ReadDataCmdOpcodeIndex  = Index;
        break;

      case EnumSpiOperationDualOutputFastRead:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_RDADDR << (Index * 2));
        Code                    = PCH_SPI_COMMAND_DUAL_FAST_READ;
        ReadDataCmdOpcodeIndex  = Index;
        break;

      case EnumSpiOperationDiscoveryParameters:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_RDADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_DISCOVERY_PARAMETERS;
        break;

      case EnumSpiOperationProgramData_1_Byte:
      case EnumSpiOperationProgramData_64_Byte:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_PROGRAM_BYTE;
        break;

      case EnumSpiOperationErase_256_Byte:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_256B_ERASE;
        break;

      case EnumSpiOperationErase_4K_Byte:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_4KB_ERASE;
        break;

      case EnumSpiOperationErase_64K_Byte:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_64KB_ERASE;
        break;

      case EnumSpiOperationWriteStatus:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRNOADDR << (Index * 2));
        Code                  = PCH_SPI_COMMAND_WRITE_STATUS;
        UnlockCmdOpcodeIndex  = Index;
        break;

      case EnumSpiOperationWriteDisable:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRNOADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_WRITE_DISABLE;
        break;

      case EnumSpiOperationWriteEnable:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRNOADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_WRITE_ENABLE;
        break;

      case EnumSpiOperationEnableWriteStatus:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRNOADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_ENABLE_WRITE_STATUS;
        break;

      case EnumSpiOperationFullChipErase:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_WRNOADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_FULL_CHIP_ERASE;
        break;

      case EnumSpiOperationReadStatus:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_RDNOADDR << (Index * 2));
        Code = PCH_SPI_COMMAND_READ_STATUS;
        break;

      case EnumSpiOperationJedecId:
        OpcodeType |= (UINT16) (V_PCH_SPI_OPTYPE_RDNOADDR << (Index * 2));
        Code                  = PCH_SPI_COMMAND_READ_ID;
        JedecIdCmdOpcodeIndex = Index;
        break;

      case EnumSpiOperationErase_8K_Byte:
      case EnumSpiOperationOther:
        Code = 0;
        break;

      default:
        Code = 0;
        ASSERT (FALSE);
        break;
    }
    ///
    /// Overrided Opcode Type and Menu Configuration registers per SpecialOpcodeEntry
    ///
    if (InitData->SpecialOpcodeEntry != NULL) {
      SpecialOpcodeEntry = InitData->SpecialOpcodeEntry;

      while (SpecialOpcodeEntry->OpcodeIndex != 0xFF) {
        if (SpecialOpcodeEntry->OpcodeIndex == Index) {
          OpcodeType &= (UINT16)~(B_PCH_SPI_OPTYPE0_MASK << (Index * 2));
          OpcodeType |= (UINT16) (SpecialOpcodeEntry->Type << (Index * 2));
          Code = SpecialOpcodeEntry->Code;
        }

        SpecialOpcodeEntry++;
      }
    }
    ///
    /// Set Opcode Menu Configuration registers.
    /// Need to be done before sending any SPI command
    ///
    MmioWrite8 (
      SpiBase + R_PCH_SPI_OPMENU0 + Index,
      Code
      );
    MmioRead8 (SpiBase + R_PCH_SPI_OPMENU0 + Index);
  }
  ///
  /// Set Opcode Type Configuration registers.
  ///
  MmioWrite16 (SpiBase + R_PCH_SPI_OPTYPE, OpcodeType);
  MmioRead16 (SpiBase + R_PCH_SPI_OPTYPE);

  if (JedecIdCmdOpcodeIndex >= SPI_NUM_OPCODE) {
    return EFI_UNSUPPORTED;
  } else {
    ///
    /// Read VendorId/DeviceId from SPI Component 1
    ///
    Status = SpiProtocolExecute (
               This,
               JedecIdCmdOpcodeIndex,
               0,
               TRUE,
               TRUE,
               FALSE,
               (UINTN) 0,
               3,
               FlashPartId,
               EnumSpiRegionDescriptor
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    ///
    /// Copy VendorId, DeviceId0, DeviceId1 and BiosStartOffset into SPI
    /// driver Private data structure
    ///
    SpiInstance->SpiIdTable[0].VendorId       = FlashPartId[0];
    SpiInstance->SpiIdTable[0].DeviceId0      = FlashPartId[1];
    SpiInstance->SpiIdTable[0].DeviceId1      = FlashPartId[2];
    SpiInstance->SpiInitTable.BiosStartOffset = InitData->BiosStartOffset;
    if (SpiInstance->SpiDescriptor.NumberComponents == 0x01) {
      ///
      /// If SPI Descriptor indicates two SPI components then
      /// read VendorId/DeviceId from SPI Component 2.
      /// Calculate SPI Component 2 address. The secondary SPI's address is equal to the first SPI's size
      /// Note: 512KB (BIT19) is the minimum Component Density.
      ///
      Data32 = (UINT32) (UINTN) (V_PCH_SPI_FLCOMP_COMP_512KB << SpiInstance->SpiDescriptor.Comp1Density);
      Status = SpiProtocolExecute (
                 This,
                 JedecIdCmdOpcodeIndex,
                 0,
                 TRUE,
                 TRUE,
                 FALSE,
                 (UINTN) Data32,
                 3,
                 FlashPartId,
                 EnumSpiRegionAll
                 );
      if (!EFI_ERROR (Status)) {
        ///
        /// Copy VendorId, DeviceId0, DeviceId1 into SPI
        /// driver Private data structure
        ///
        SpiInstance->SpiIdTable[1].VendorId   = FlashPartId[0];
        SpiInstance->SpiIdTable[1].DeviceId0  = FlashPartId[1];
        SpiInstance->SpiIdTable[1].DeviceId1  = FlashPartId[2];
      }
    }
    ///
    /// Set the Prefix Opcode registers.
    ///
    MmioWrite16 (
      SpiBase + R_PCH_SPI_PREOP,
      (InitData->PrefixOpcode[1] << 8) | InitData->PrefixOpcode[0]
      );
    MmioRead16 (SpiBase + R_PCH_SPI_PREOP);

    ///
    /// Copy PrefixOpcode into SPI driver Private data structure
    ///
    for (Index = 0; Index < SPI_NUM_PREFIX_OPCODE; Index++) {
      SpiInstance->SpiInitTable.PrefixOpcode[Index] = InitData->PrefixOpcode[Index];
    }
    ///
    /// Copy BiosSize into SPI driver Private data structure
    ///
    SpiInstance->SpiInitTable.BiosSize = InitData->BiosSize;
  }
  ///
  /// Get VSCC values from VTBA table in the Descriptor.
  ///
  if (SpiInstance->DescriptorMode == TRUE) {
    Status = GetDescriptorVsccValues (
               This,
               ReadDataCmdOpcodeIndex,
               &Vscc0Value,
               &Vscc1Value
               );

    if (EFI_ERROR (Status)) {
      ///
      /// Program the VSCC0 & VSCC1 registers by getting the data from SpiInitTable
      ///
      for (Index = 0; Index < SPI_NUM_OPCODE; Index++) {
        ///
        /// For every platform that supports ME, only 4 KB erase is supported
        /// Get the opcode from SpiInitTable if the operation is 4 KB erase
        ///
        if (SpiInstance->SpiInitTable.SpiCmdConfig[Index].Operation == EnumSpiOperationErase_4K_Byte) {
          Vscc0Value  = Vscc0Value | (UINT32) (V_PCH_SPI_VSCC0_BSES_4K);
          Vscc0Value  = Vscc0Value | (UINT32) (PCH_SPI_COMMAND_4KB_ERASE << 8);
        } else if (SpiInstance->SpiInitTable.SpiCmdConfig[Index].Operation == EnumSpiOperationProgramData_64_Byte) {
          Vscc0Value = Vscc0Value | (UINT32) (B_PCH_SPI_VSCC0_WG_64B);
        }
      }
      ///
      /// Bit WSR and WEWS should NOT be both set to 1, so we check if there is any "Write enable on Write status" prefix opcode
      /// from SpiInitTable at first, then check "Write Status Enable" prefix opcode
      ///
      if ((SpiInstance->SpiInitTable.PrefixOpcode[0] == PCH_SPI_COMMAND_WRITE_ENABLE) ||
          (SpiInstance->SpiInitTable.PrefixOpcode[1] == PCH_SPI_COMMAND_WRITE_ENABLE)) {
        Vscc0Value = Vscc0Value | (UINT32) (B_PCH_SPI_VSCC0_WEWS);
      } else if ((SpiInstance->SpiInitTable.PrefixOpcode[0] == PCH_SPI_COMMAND_WRITE_STATUS_EN) ||
                 (SpiInstance->SpiInitTable.PrefixOpcode[1] == PCH_SPI_COMMAND_WRITE_STATUS_EN)) {
        Vscc0Value = Vscc0Value | (UINT32) (B_PCH_SPI_VSCC0_WSR);
      }

      Vscc1Value = Vscc0Value;
    }
    ///
    /// The VCL locks itself when set, it will assert because we have no way to update VSCC value
    ///
    if ((MmioRead32 ((UINTN) (SpiBase + R_PCH_SPI_VSCC0)) & B_PCH_SPI_VSCC0_VCL) != 0) {
      ASSERT_EFI_ERROR (EFI_ACCESS_DENIED);
      return EFI_ACCESS_DENIED;
    }
    ASSERT (Vscc0Value != 0);
    MmioWrite32 ((UINTN) (SpiBase + R_PCH_SPI_VSCC0), (UINT32) (Vscc0Value & B_PCH_SPI_VSCC0_CAP_MASK));
    if (SpiInstance->SpiDescriptor.NumberComponents == 0) {
      Vscc1Value = Vscc0Value;
    }
    MmioWrite32 ((UINTN) (SpiBase + R_PCH_SPI_VSCC1), (UINT32) (Vscc1Value & B_PCH_SPI_VSCC1_CAP_MASK));
  }

  SpiPhaseInit ();

  return EFI_SUCCESS;
}

/**
  Lock the SPI Static Configuration Interface.
  Once locked, the interface can not be changed and can only be clear by system reset.

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.

  @retval EFI_SUCCESS             Initialization completed.
  @retval EFI_ACCESS_DENIED       The interface has already been locked.
  @retval EFI_DEVICE_ERROR        Device error, operation failed.

**/
EFI_STATUS
EFIAPI
SpiProtocolLock (
  IN EFI_SPI_PROTOCOL     *This
  )
{
  SPI_INSTANCE  *SpiInstance;
  UINTN         SpiBase;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase     = SpiInstance->SpiBase;

  //
  // Check if the interface has already been locked-down.
  //
  if ((MmioRead16 (SpiBase + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FLOCKDN) != 0) {
    return EFI_ACCESS_DENIED;
  }
  //
  // Lock-down the configuration interface.
  //
  MmioOr16 ((UINTN) (SpiBase + R_PCH_SPI_HSFS), (UINT16) (B_PCH_SPI_HSFS_FLOCKDN));
  MmioRead16 (SpiBase + R_PCH_SPI_HSFS);

  //
  // Verify if it's really locked.
  //
  if ((MmioRead16 (SpiBase + R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FLOCKDN) == 0) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Execute SPI commands from the host controller.
  This function would be called by runtime driver, please do not use any MMIO marco here

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] OpcodeIndex          Index of the command in the OpCode Menu.
  @param[in] PrefixOpcodeIndex    Index of the first command to run when in an atomic cycle sequence.
  @param[in] DataCycle            TRUE if the SPI cycle contains data
  @param[in] Atomic               TRUE if the SPI cycle is atomic and interleave cycles are not allowed.
  @param[in] ShiftOut             If DataByteCount is not zero, TRUE to shift data out and FALSE to shift data in.
  @param[in] Address              In Descriptor Mode, for Descriptor Region, SeC Region and Platform
                                  Region, this value specifies the offset from the Region Base; for BIOS Region,
                                  this value specifies the offset from the start of the BIOS Image. In Non
                                  Descriptor Mode, this value specifies the offset from the start of the BIOS Image.
                                  Please note BIOS Image size may be smaller than BIOS Region size (in Descriptor
                                  Mode) or the flash size (in Non Descriptor Mode), and in this case, BIOS Image is
                                  supposed to be placed at the top end of the BIOS Region (in Descriptor Mode) or
                                  the flash (in Non Descriptor Mode)
  @param[in] DataByteCount        Number of bytes in the data portion of the SPI cycle. This function may break the
                                  data transfer into multiple operations. This function ensures each operation does
                                  not cross 256 byte flash address boundary.
                                  *NOTE: if there is some SPI chip that has a stricter address boundary requirement
                                  (e.g., its write page size is < 256 byte), then the caller cannot rely on this
                                  function to cut the data transfer at proper address boundaries, and it's the
                                  caller's responsibility to pass in a properly cut DataByteCount parameter.
  @param[in,out] Buffer          Pointer to caller-allocated buffer containing the data received or sent during the
                                  SPI cycle.
  @param[in] SpiRegionType        SPI Region type. Values EnumSpiRegionBios, EnumSpiRegionSeC,
                                  EnumSpiRegionDescriptor, and EnumSpiRegionPlatformData are only applicable in
                                  Descriptor mode. Value EnumSpiRegionAll is applicable to both Descriptor Mode
                                  and Non Descriptor Mode, which indicates "SpiRegionOffset" is actually relative
                                  to base of the 1st flash device (i.e., it is a Flash Linear Address).

  @retval EFI_SUCCESS             Command succeed.
  @retval EFI_INVALID_PARAMETER   The parameters specified are not valid.
  @exception EFI_UNSUPPORTED      Command not supported.
  @retval EFI_DEVICE_ERROR        Device error, command aborts abnormally.

**/
EFI_STATUS
EFIAPI
SpiProtocolExecute (
  IN     EFI_SPI_PROTOCOL   *This,
  IN     UINT8              OpcodeIndex,
  IN     UINT8              PrefixOpcodeIndex,
  IN     BOOLEAN            DataCycle,
  IN     BOOLEAN            Atomic,
  IN     BOOLEAN            ShiftOut,
  IN     UINTN              Address,
  IN     UINT32             DataByteCount,
  IN OUT UINT8              *Buffer,
  IN     SPI_REGION_TYPE    SpiRegionType
  )
{
  EFI_STATUS   Status;
  UINT8        BiosCtlSave;
  UINT32       Data32;
  UINT32       AcpiBase;
  UINT32       SmiEnSave;
  SPI_INSTANCE *SpiInstance;
  UINTN        SpiBase;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase     = SpiInstance->SpiBase;

  BiosCtlSave = 0;
  SmiEnSave   = 0;

  ///
  /// Check if the parameters are valid.
  ///
  if ((OpcodeIndex >= SPI_NUM_OPCODE) || (PrefixOpcodeIndex >= SPI_NUM_PREFIX_OPCODE)) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Make sure it's safe to program the command.
  /// Poll both Hardware Sequencing and Software Sequencing Status
  ///
  if (!WaitForSpiCycleComplete (This, TRUE, FALSE)) {
    return EFI_DEVICE_ERROR;
  }

  if (!WaitForSpiCycleComplete (This, FALSE, FALSE)) {
    return EFI_DEVICE_ERROR;
  }
  ///
  /// Acquire access to the SPI interface is not required any more.
  ///
  ///
  /// Disable SMIs to make sure normal mode flash access is not interrupted by an SMI
  /// whose SMI handler accesses flash (e.g. for error logging)
  ///
  /// *** NOTE: if the SMI_LOCK bit is set (i.e., ABASE + 0x30 [0] = '1'),
  /// clearing B_GBL_SMI_EN will not have effect. In this situation, some other
  /// synchronization methods must be applied either here or in the consumer of the
  /// EFI_SPI_PROTOCOl.Execute(). An example method is disabling the specific SMI sources
  /// whose SMI handlers access flash before calling Execute() and re-enabling the SMI
  /// sources after the call.
  ///
  AcpiBase = PciRead32 (
               PCI_LIB_ADDRESS ( DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_LPC,
               PCI_FUNCTION_NUMBER_PCH_LPC,
               R_PCH_LPC_ACPI_BASE)
               ) & B_PCH_LPC_ACPI_BASE_BAR;
  SmiEnSave = IoRead32 ((UINTN) (AcpiBase + R_PCH_SMI_EN));
  Data32    = SmiEnSave &~B_PCH_SMI_EN_GBL_SMI;
  IoWrite32 ((UINTN) (AcpiBase + R_PCH_SMI_EN), Data32);

  ///
  /// If shifts the data out, disable Prefetching and Caching.
  ///
  if (ShiftOut) {
    BiosCtlSave = MmioRead8 (SpiBase + R_PCH_SPI_BCR) & B_PCH_SPI_BCR_SRC;

    MmioAndThenOr8 (SpiBase + R_PCH_SPI_BCR,
      (UINT8) (~B_PCH_SPI_BCR_SRC),
      (UINT8) (V_PCH_SPI_BCR_SRC_PREF_DIS_CACHE_DIS)
      );
  }
  ///
  /// Sends the command to the SPI interface to execute.
  ///
  Status = SendSpiCmd (
             This,
             OpcodeIndex,
             PrefixOpcodeIndex,
             DataCycle,
             Atomic,
             ShiftOut,
             Address,
             DataByteCount,
             Buffer,
             SpiRegionType
             );
  ///
  /// Restore the settings for SPI Prefetching and Caching.
  ///
  if (ShiftOut) {
    MmioAndThenOr8 (SpiBase + R_PCH_SPI_BCR,
      (UINT8) (~B_PCH_SPI_BCR_SRC),
      (UINT8) (BiosCtlSave)
      );
  }
  ///
  /// Restore SMIs.
  ///
  IoWrite32 ((UINTN) (AcpiBase + R_PCH_SMI_EN), SmiEnSave);

  return Status;
}

/**
  Convert SPI offset to Physical address of SPI hardware

  @param[in] This                       Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] SpiRegionOffset            In Descriptor Mode, for Descriptor Region, SeC Region and Platform
                                        Region, this value specifies the offset from the Region Base; for BIOS Region,
                                        this value specifies the offset from the start of the BIOS Image. In Non
                                        Descriptor Mode, this value specifies the offset from the start of the BIOS Image.
                                        Please note BIOS Image size may be smaller than BIOS Region size (in Descriptor
                                        Mode) or the flash size (in Non Descriptor Mode), and in this case, BIOS Image is
                                        supposed to be placed at the top end of the BIOS Region (in Descriptor Mode) or
                                        the flash (in Non Descriptor Mode)
  @param[in] BaseAddress                Base Address of the region.
  @param[in] SpiRegionType              SPI Region type. Values EnumSpiRegionBios, EnumSpiRegionSeC,
                                        EnumSpiRegionDescriptor, and EnumSpiRegionPlatformData are only applicable in
                                        Descriptor mode. Value EnumSpiRegionAll is applicable to both Descriptor Mode
                                        and Non Descriptor Mode, which indicates "SpiRegionOffset" is actually relative
                                        to base of the 1st flash device (i.e., it is a Flash Linear Address).
  @param[in, out] HardwareSpiAddress    Return absolution SPI address (i.e., Flash Linear Address)
  @param[in, out] BaseAddress           Return base address of the region type
  @param[in, out] LimitAddress          Return limit address of the region type

  @retval  EFI_SUCCESS                  Command succeed.

**/
VOID
SpiOffset2Physical (
  IN      EFI_SPI_PROTOCOL  *This,
  IN      UINTN             SpiRegionOffset,
  IN      SPI_REGION_TYPE   SpiRegionType,
  OUT     UINTN             *HardwareSpiAddress,
  OUT     UINTN             *BaseAddress,
  OUT     UINTN             *LimitAddress
  )
{
  SPI_INSTANCE  *SpiInstance;
  UINTN         SpiBase;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase     = SpiInstance->SpiBase;

  if (SpiInstance->DescriptorMode == TRUE) {
    switch (SpiRegionType) {

      case EnumSpiRegionBios:
        *LimitAddress = (((MmioRead32 (SpiBase + R_PCH_SPI_FREG1_BIOS)
                          & B_PCH_SPI_FREG1_LIMIT_MASK) >> 16) + 1) << 12;
        //
        // Adjust BaseAddress because of FTOOLC
        // FTOOLC will create a bigger region than BIOS actual size at the end of whole SPI
        // and Base Address reported by PCH will not the same with what BIOS assumes.
        // For example, FTOOLC create a 3MB size of 4MB for BIOS region and the Base Address
        // will be 0x100000, and BIOS will be placed at 0x300000 to 0x3fffff.
        // PCH_SPI_FREG1_BIOS will report BIOS address at 0x100000 and BIOS will fail to read
        // Variable from 0x100000.
        //
        // *BaseAddress = (MmioRead32 (SpiBase + PCH_SPI_FREG1_BIOS)
        //                & B_PCH_SPI_FREG1_BASE_MASK) << 12;
        //
        *BaseAddress = *LimitAddress - SpiInstance->SpiInitTable.BiosSize;
        break;

      case EnumSpiRegionSeC:
        *BaseAddress = (MmioRead32 (SpiBase + R_PCH_SPI_FREG2_SEC) & B_PCH_SPI_FREG2_BASE_MASK) << 12;
        *LimitAddress = (((MmioRead32 (SpiBase + R_PCH_SPI_FREG2_SEC)
                          & B_PCH_SPI_FREG2_LIMIT_MASK) >> 16) + 1) << 12;
        break;

      case EnumSpiRegionDescriptor:
        *BaseAddress = (MmioRead32 (SpiBase + R_PCH_SPI_FREG0_FLASHD) & B_PCH_SPI_FREG0_BASE_MASK) << 12;
        *LimitAddress = (((MmioRead32 (SpiBase + R_PCH_SPI_FREG0_FLASHD)
                          & B_PCH_SPI_FREG0_LIMIT_MASK) >> 16) + 1) << 12;
        break;

      case EnumSpiRegionPlatformData:
        *BaseAddress = (MmioRead32 (SpiBase + R_PCH_SPI_FREG4_PLATFORM_DATA) & B_PCH_SPI_FREG4_BASE_MASK) << 12;
        *LimitAddress = (((MmioRead32 (SpiBase + R_PCH_SPI_FREG4_PLATFORM_DATA)
                          & B_PCH_SPI_FREG4_LIMIT_MASK) >> 16) + 1) << 12;
        break;

      default:
        ///
        /// EnumSpiRegionAll indicates address is relative to flash device (i.e., address is Flash
        /// Linear Address)
        ///
        *BaseAddress  = 0;
        *LimitAddress = 0;
        break;
    }

    *HardwareSpiAddress = SpiRegionOffset +*BaseAddress;
  } else {
    if (SpiRegionType == EnumSpiRegionAll) {
      ///
      /// EnumSpiRegionAll indicates address is relative to flash device (i.e., address is Flash
      /// Linear Address)
      ///
      *HardwareSpiAddress = SpiRegionOffset;
    } else {
      ///
      /// Otherwise address is relative to BIOS image
      ///
      *HardwareSpiAddress = SpiRegionOffset + SpiInstance->SpiInitTable.BiosStartOffset;
    }
  }
}

/**
  This function sends the programmed SPI command to the slave device.

  @param[in] OpcodeIndex          Index of the command in the OpCode Menu.
  @param[in] PrefixOpcodeIndex    Index of the first command to run when in an atomic cycle sequence.
  @param[in] DataCycle            TRUE if the SPI cycle contains data
  @param[in] Atomic               TRUE if the SPI cycle is atomic and interleave cycles are not allowed.
  @param[in] ShiftOut             If DataByteCount is not zero, TRUE to shift data out and FALSE to shift data in.
  @param[in] Address              In Descriptor Mode, for Descriptor Region, SeC Region and Platform
                                  Region, this value specifies the offset from the Region Base; for BIOS Region,
                                  this value specifies the offset from the start of the BIOS Image. In Non
                                  Descriptor Mode, this value specifies the offset from the start of the BIOS Image.
                                  Please note BIOS Image size may be smaller than BIOS Region size (in Descriptor
                                  Mode) or the flash size (in Non Descriptor Mode), and in this case, BIOS Image is
                                  supposed to be placed at the top end of the BIOS Region (in Descriptor Mode) or
                                  the flash (in Non Descriptor Mode)
  @param[in] DataByteCount        Number of bytes in the data portion of the SPI cycle. This function may break the
                                  data transfer into multiple operations. This function ensures each operation does
                                  not cross 256 byte flash address boundary.
                                  *NOTE: if there is some SPI chip that has a stricter address boundary requirement
                                  (e.g., its write page size is < 256 byte), then the caller cannot rely on this
                                  function to cut the data transfer at proper address boundaries, and it's the
                                  caller's responsibility to pass in a properly cut DataByteCount parameter.
  @param[in, out] Buffer          Data received or sent during the SPI cycle.
  @param[in] SpiRegionType        SPI Region type. Values EnumSpiRegionBios, EnumSpiRegionSeC,
                                  EnumSpiRegionDescriptor, and EnumSpiRegionPlatformData are only applicable in
                                  Descriptor mode. Value EnumSpiRegionAll is applicable to both Descriptor Mode
                                  and Non Descriptor Mode, which indicates "SpiRegionOffset" is actually relative
                                  to base of the 1st flash device (i.e., it is a Flash Linear Address).

  @retval EFI_SUCCESS             SPI command completes successfully.
  @retval EFI_DEVICE_ERROR        Device error, the command aborts abnormally.
  @retval EFI_ACCESS_DENIED       Some unrecognized command encountered in hardware sequencing mode
  @retval EFI_INVALID_PARAMETER   The parameters specified are not valid.

**/
EFI_STATUS
SendSpiCmd (
  IN     EFI_SPI_PROTOCOL   *This,
  IN     UINT8              OpcodeIndex,
  IN     UINT8              PrefixOpcodeIndex,
  IN     BOOLEAN            DataCycle,
  IN     BOOLEAN            Atomic,
  IN     BOOLEAN            ShiftOut,
  IN     UINTN              Address,
  IN     UINT32             DataByteCount,
  IN OUT UINT8              *Buffer,
  IN     SPI_REGION_TYPE    SpiRegionType
  )
{
  UINT32        Index;
  SPI_INSTANCE  *SpiInstance;
  UINTN         HardwareSpiAddr;
  UINTN         SpiBiosSize;
  BOOLEAN       UseSoftwareSequence;
  UINTN         BaseAddress;
  UINTN         LimitAddress;
  UINT32        SpiDataCount;
  UINT8         OpCode;
  UINT16        OpType;
  SPI_OPERATION Operation;
  UINTN         SpiBase;
  UINT32        SpiSoftFreq;
  UINT16        FlashCycle;
  BOOLEAN       BiosWriteProtect;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase     = SpiInstance->SpiBase;
  SpiBiosSize = SpiInstance->SpiInitTable.BiosSize;
  Operation   = SpiInstance->SpiInitTable.SpiCmdConfig[OpcodeIndex].Operation;
  OpCode      = MmioRead8 (SpiBase + R_PCH_SPI_OPMENU0 + OpcodeIndex);
  OpType      = (MmioRead16 (SpiBase + R_PCH_SPI_OPTYPE) >> OpcodeIndex * 2) & (UINT16) (B_PCH_SPI_OPTYPE0_MASK);
  ///
  /// Check if the value of opcode register is 0 or the BIOS Size of SpiInitTable is 0 while SpiRegionType is EnumSpiRegionBios.
  ///
  if (OpCode == 0 || (SpiBiosSize == 0 && SpiRegionType == EnumSpiRegionBios)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Check if need to disable BIOS Write Protect
  ///
  if ((Operation == EnumSpiOperationProgramData_1_Byte) ||
      (Operation == EnumSpiOperationProgramData_64_Byte) ||
      (Operation == EnumSpiOperationErase_256_Byte) ||
      (Operation == EnumSpiOperationErase_4K_Byte) ||
      (Operation == EnumSpiOperationErase_8K_Byte) ||
      (Operation == EnumSpiOperationErase_64K_Byte) ||
      (Operation == EnumSpiOperationFullChipErase)) {
    DisableBiosWriteProtect (SpiBase);
    BiosWriteProtect = FALSE;
  } else {
    BiosWriteProtect = TRUE;
  }
  ///
  /// When current code is read id OR current is not descriptor mode, we will use compatible mode
  ///
  UseSoftwareSequence = FALSE;
  if ((Operation == EnumSpiOperationJedecId) ||
      (Operation == EnumSpiOperationReadStatus) ||
      (Operation == EnumSpiOperationWriteStatus) ||
      (Operation == EnumSpiOperationWriteDisable) ||
      (Operation == EnumSpiOperationWriteEnable) ||
      (Operation == EnumSpiOperationEnableWriteStatus) ||
      (Operation == EnumSpiOperationOther) ||
      (Operation == EnumSpiOperationDiscoveryParameters) ||
      (SpiInstance->DescriptorMode == FALSE)
      ) {
    UseSoftwareSequence = TRUE;
  }

  SpiOffset2Physical (This, Address, SpiRegionType, &HardwareSpiAddr, &BaseAddress, &LimitAddress);
  ///
  /// Have direct access to BIOS region in Descriptor mode,
  ///
  if (OpType == EnumSpiOpcodeRead && SpiRegionType == EnumSpiRegionBios) {
    CopyMem (
      Buffer,
      (UINT8 *) ((HardwareSpiAddr - BaseAddress) + (UINT32) (~(SpiBiosSize - 1))),
      DataByteCount
      );
    return EFI_SUCCESS;
  }
  ///
  /// DEBUG((EFI_D_ERROR, "SPIADDR %x, %x, %x, %x\n", Address, HardwareSpiAddr, BaseAddress,
  /// LimitAddress));
  ///
  if ((DataCycle == FALSE) && (DataByteCount > 0)) {
    DataByteCount = 0;
  }

  do {
    ///
    /// Trim at 256 byte boundary per operation,
    /// - PCH SPI controller requires trimming at 4KB boundary
    /// - Some SPI chips require trimming at 256 byte boundary for write operation
    /// - Trimming has limited performance impact as we can read / write at most 64 byte
    ///   per operation
    ///
    if (HardwareSpiAddr + DataByteCount > ((HardwareSpiAddr + BIT8) &~(BIT8 - 1))) {
      SpiDataCount = (((UINT32) (HardwareSpiAddr) + BIT8) &~(BIT8 - 1)) - (UINT32) (HardwareSpiAddr);
    } else {
      SpiDataCount = DataByteCount;
    }
    ///
    /// Calculate the number of bytes to shift in/out during the SPI data cycle.
    /// Valid settings for the number of bytes duing each data portion of the
    /// PCH SPI cycles are: 0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 24, 32, 40, 48, 56, 64
    ///
    if ((SpiInstance->DescriptorMode == FALSE) &&
        (OpCode == PCH_SPI_COMMAND_PROGRAM_BYTE || Operation == EnumSpiOperationProgramData_1_Byte)) {
      SpiDataCount = 1;
    } else if (SpiDataCount >= 64) {
      SpiDataCount = 64;
    } else if ((SpiDataCount &~0x07) != 0) {
      SpiDataCount = SpiDataCount &~0x07;
    }
    ///
    /// If shifts data out, load data into the SPI data buffer.
    ///
    if (ShiftOut) {
      for (Index = 0; Index < SpiDataCount; Index++) {
        MmioWrite8 (SpiBase + R_PCH_SPI_FDATA00 + Index, Buffer[Index]);
        MmioRead8 (SpiBase + R_PCH_SPI_FDATA00 + Index);
      }
    }

    MmioWrite32 (
      (SpiBase + R_PCH_SPI_FADDR),
      (UINT32) (HardwareSpiAddr & B_PCH_SPI_FADDR_MASK)
      );
    MmioRead32 (SpiBase + R_PCH_SPI_FADDR);

    ///
    /// Execute the command on the SPI compatible mode
    ///
    if (UseSoftwareSequence) {
      ///
      /// Software sequencing ...
      ///
      ///
      /// Clear error flags
      ///
      MmioWrite16 (SpiBase + R_PCH_SPI_HSFS, (UINT16) B_PCH_SPI_HSFS_AEL);
      MmioWrite8 (SpiBase + R_PCH_SPI_SSFCS, (UINT8) B_PCH_SPI_SSFCS_FCERR);

      switch (SpiInstance->SpiInitTable.SpiCmdConfig[OpcodeIndex].Frequency) {
        case EnumSpiCycle20MHz:
          SpiSoftFreq = V_PCH_SPI_SSFCS_SCF_20MHZ;
          break;

        case EnumSpiCycle33MHz:
          SpiSoftFreq = V_PCH_SPI_SSFCS_SCF_33MHZ;
          break;

        case EnumSpiCycle50MHz:
          SpiSoftFreq = V_PCH_SPI_SSFCS_SCF_50MHZ;
          break;

        default:
          ///
          /// This is an invalid use of the protocol
          /// See definition, but caller must call with valid value
          ///
          SpiSoftFreq = 0;
          ASSERT (!EFI_UNSUPPORTED);
          break;
      }
      ///
      /// SSFCS BIT 31:27 are reserved, BIOS must set this field to '11111'b
      /// To change the offset to the right DWORD boundary, so use offset 0x90 as the operation address
      ///
      if (DataCycle) {
        MmioWrite32 (
          (SpiBase + R_PCH_SPI_SSFCS),
          ( (UINT32) (BIT31 | BIT30 | BIT29 | BIT28 | BIT27) |
            (UINT32) ((SpiSoftFreq << 24) & B_PCH_SPI_SSFCS_SCF_MASK) |
            (UINT32) (B_PCH_SPI_SSFCS_DC) | (UINT32) (((SpiDataCount - 1) << 16) & B_PCH_SPI_SSFCS_DBC_MASK) |
            (UINT32) ((OpcodeIndex << 12) & B_PCH_SPI_SSFCS_COP) |
            (UINT32) ((PrefixOpcodeIndex << 11) & B_PCH_SPI_SSFCS_SPOP) |
            (UINT32) (Atomic ? B_PCH_SPI_SSFCS_ACS : 0) |
            (UINT32) (B_PCH_SPI_SSFCS_SCGO)));
      } else {
        MmioWrite32 (
          (SpiBase + R_PCH_SPI_SSFCS),
          ( (UINT32) (BIT31 | BIT30 | BIT29 | BIT28 | BIT27) |
            (UINT32) ((SpiSoftFreq << 24) & B_PCH_SPI_SSFCS_SCF_MASK) |
            (UINT32) ((OpcodeIndex << 12) & B_PCH_SPI_SSFCS_COP) |
            (UINT32) ((PrefixOpcodeIndex << 11) & B_PCH_SPI_SSFCS_SPOP) |
            (UINT32) (Atomic ? B_PCH_SPI_SSFCS_ACS : 0) |
            (UINT32) (B_PCH_SPI_SSFCS_SCGO)));
      }
      MmioRead32 (SpiBase + R_PCH_SPI_SSFCS);
    } else {
      ///
      /// Hardware sequencing ...
      ///
      ///
      /// check for PCH SPI hardware sequencing required commands
      ///
      if (Operation == EnumSpiOperationReadData ||
          Operation == EnumSpiOperationFastRead ||
          Operation == EnumSpiOperationDualOutputFastRead) {
        ///
        /// Read Cycle
        ///
        FlashCycle = (UINT16) (V_PCH_SPI_HSFC_FCYCLE_READ << 1);
      } else if (Operation == EnumSpiOperationProgramData_1_Byte ||
                 Operation == EnumSpiOperationProgramData_64_Byte) {
        ///
        /// Write Cycle
        ///
        FlashCycle = (UINT16) (V_PCH_SPI_HSFC_FCYCLE_WRITE << 1);
      } else if (Operation == EnumSpiOperationErase_256_Byte ||
               Operation == EnumSpiOperationErase_4K_Byte ||
               Operation == EnumSpiOperationErase_8K_Byte ||
               Operation == EnumSpiOperationErase_64K_Byte ||
               Operation == EnumSpiOperationFullChipErase) {
        ///
        /// Erase Cycle
        ///
        FlashCycle = (UINT16) (V_PCH_SPI_HSFC_FCYCLE_ERASE << 1);
      } else {
        ///
        /// Unrecognized Operation
        ///
        ASSERT (FALSE);
        return EFI_ACCESS_DENIED;
      }
      ///
      /// Clear error flags
      ///
      MmioWrite16 (
        SpiBase + R_PCH_SPI_HSFS,
        (UINT16) (B_PCH_SPI_HSFS_AEL | B_PCH_SPI_HSFS_FCERR)
        );
      ///
      /// Send the command
      ///
      MmioWrite16 (
        SpiBase + R_PCH_SPI_HSFC,
        (UINT16) (((SpiDataCount - 1) << 8) & B_PCH_SPI_HSFC_FDBC_MASK) |
        FlashCycle | B_PCH_SPI_HSFC_FCYCLE_FGO
        );
      ///
      /// Read back for posted write to take effect
      ///
      MmioRead16 (SpiBase + R_PCH_SPI_HSFC);
    }
    ///
    /// end of command execution
    ///
    /// Wait the SPI cycle to complete.
    ///
    if (!WaitForSpiCycleComplete (This, UseSoftwareSequence, TRUE)) {
      if (BiosWriteProtect == FALSE) {
        ///
        /// Enable BIOS Write Protect
        ///
        EnableBiosWriteProtect (SpiBase);
      }
      return EFI_DEVICE_ERROR;
    }
    ///
    /// If shifts data in, get data from the SPI data buffer.
    ///
    if (!ShiftOut) {
      for (Index = 0; Index < SpiDataCount; Index++) {
        Buffer[Index] = MmioRead8 (SpiBase + R_PCH_SPI_FDATA00 + Index);
      }
    }

    HardwareSpiAddr += SpiDataCount;
    Buffer += SpiDataCount;
    DataByteCount -= SpiDataCount;
  } while (DataByteCount > 0);

  if (BiosWriteProtect == FALSE) {
    ///
    /// Enable BIOS Write Protect
    ///
    EnableBiosWriteProtect (SpiBase);
  }

  return EFI_SUCCESS;
}

/**
  Wait execution cycle to complete on the SPI interface. Check both Hardware
  and Software Sequencing status registers.

  @param[in] This                 The SPI protocol instance
  @param[in] UseSoftwareSequence  TRUE if Software Sequencing
  @param[in] ErrorCheck           TRUE if the SpiCycle needs to do the error check

  @retval TRUE                    SPI cycle completed on the interface.
  @retval FALSE                   Time out while waiting the SPI cycle to complete.
                                  It's not safe to program the next command on the SPI interface.
**/
BOOLEAN
WaitForSpiCycleComplete (
  IN     EFI_SPI_PROTOCOL   *This,
  IN     BOOLEAN            UseSoftwareSequence,
  IN     BOOLEAN            ErrorCheck
  )
{
  UINT64        WaitTicks;
  UINT64        WaitCount;
  UINT32        StatusRegAddr;
  UINT32        CycleInProgressBit;
  UINT16        AelBit;
  UINT16        FcErrBit;
  UINT16        FcycleDone;
  UINT16        Data16;
  SPI_INSTANCE  *SpiInstance;
  UINTN         SpiBase;

  SpiInstance = SPI_INSTANCE_FROM_SPIPROTOCOL (This);
  SpiBase     = SpiInstance->SpiBase;

  if (UseSoftwareSequence) {
    ///
    /// This is Software Sequencing
    ///
    StatusRegAddr       = R_PCH_SPI_SSFCS;
    CycleInProgressBit  = B_PCH_SPI_SSFCS_SCIP;
    AelBit              = B_PCH_SPI_SSFCS_AEL;
    FcErrBit            = B_PCH_SPI_SSFCS_FCERR;
    FcycleDone          = B_PCH_SPI_SSFCS_CDS;
  } else {
    ///
    /// This is Hardware Sequencing
    ///
    StatusRegAddr       = R_PCH_SPI_HSFS;
    CycleInProgressBit  = B_PCH_SPI_HSFS_SCIP;
    AelBit              = B_PCH_SPI_HSFS_AEL;
    FcErrBit            = B_PCH_SPI_HSFS_FCERR;
    FcycleDone          = B_PCH_SPI_HSFS_FDONE;
  }
  ///
  /// Convert the wait period allowed into to tick count
  ///
  WaitCount = WAIT_TIME / WAIT_PERIOD;

  ///
  /// Wait for the SPI cycle to complete.
  ///
  for (WaitTicks = 0; WaitTicks < WaitCount; WaitTicks++) {
    Data16 = MmioRead16 (SpiBase + StatusRegAddr);
    if ((Data16 & CycleInProgressBit) == 0) {
      if (UseSoftwareSequence){
        MmioWrite8 (SpiBase + StatusRegAddr, (UINT8)(AelBit | FcErrBit | FcycleDone));
      } else {
        MmioWrite16 (SpiBase + StatusRegAddr, (AelBit | FcErrBit | FcycleDone));
      }
      if (((Data16 & AelBit) || (Data16 & FcErrBit)) && (ErrorCheck == TRUE)) {
        return FALSE;
      } else {
        return TRUE;
      }
    }

    PchPmTimerStall (WAIT_PERIOD);
  }

  return FALSE;
}
