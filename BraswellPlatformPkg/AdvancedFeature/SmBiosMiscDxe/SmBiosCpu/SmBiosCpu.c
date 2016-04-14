/** @file
  This driver will determine memory configuration information from the chipset
  and memory and create SMBIOS memory structures appropriately.

  @par Revision Reference:
  SMBIOS Specification version 2.8.0 from DMTF: http://www.dmtf.org/standards/smbios
  Intel Framework Specifications, all available at: http://www.intel.com/technology/framework/spec.htm
   - Data Hub Specification
   - SMBUS Host Controller Protocol Specification
   - Human Interface Infrastructure Specification
  Unified Extensible Firmware Interface (UEFI) Specifications: http://www.uefi.org/specs/

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <protocol/MpService.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <CpuRegs.h>
#include <Protocol/Smbios.h>
#include <Library/HiiLib.h>
#include "CpuConfigLib.h"
#include "Cache.h"
#include "Processor.h"
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include "Cpu.h"
#include "SocketLga1156Lib.h"
#include <Library/SynchronizationLib.h>
#include <Library/TimerLib.h>
#include "MiscSubclassDriver.h"


#define PLATFORM_DESKTOP           0
#define PLATFORM_MOBILE            1
#define PLATFORM_SERVER            2

UINTN                          mCpuSocketStrNumber = 1;
UINTN                          mCpuAssetTagStrNumber = 4;
extern UINT8                   MiscSubclassStrings[];
UINT8                          mPlatformType = PLATFORM_MOBILE;
UINT32                         mPopulatedSocketCount;
CPU_CONFIG_CONTEXT_BUFFER      mCpuConfigConextBuffer;

EFI_STRING_ID
GetProcessorManufacturer (
  IN UINTN  ProcessorNumber
  );
  
BOOLEAN
IsIntelProcessor (
  IN UINTN  ProcessorNumber
  );

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from CpuConfig.c **********************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Get the location of specified processor.
  
  This function gets the location of specified processor, including
  package number, core number within package, thread number within core.

  @param  ProcessorNumber Handle number of specified logical processor.
  @param  PackageNumber   Pointer to the output package number.
  @param  CoreNumber      Pointer to the output core number.
  @param  ThreadNumber    Pointer to the output thread number.

**/
VOID
EFIAPI
GetProcessorLocation (
  IN    UINTN     ProcessorNumber,
  OUT   UINT32    *PackageNumber   OPTIONAL,
  OUT   UINT32    *CoreNumber      OPTIONAL,
  OUT   UINT32    *ThreadNumber    OPTIONAL
)
{
  CPU_COLLECTED_DATA          *CpuCollectedData;

  CpuCollectedData   = &mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber];

  //
  // If PackageNumber is not NULL, set the package number of the specified processor to it.
  //
  if (PackageNumber != NULL) {
    *PackageNumber = CpuCollectedData->ProcessorLocation.Package;
  }

  //
  // If CoreNumber is not NULL, set the core number within package to it.
  //
  if (CoreNumber != NULL) {
    *CoreNumber    = CpuCollectedData->ProcessorLocation.Core;
  }

  //
  // If ThreadNumber is not NULL, set the thread number within core to it.
  //
  if (ThreadNumber != NULL) {
    *ThreadNumber  = CpuCollectedData->ProcessorLocation.Thread;
  }
}

/**
  Get the pointer to specified CPUID leaf of Extended Topology Enumeration.
  
  This function gets the pointer to specified CPUID leaf of Extended Topology Enumeration.

  @param  ProcessorNumber   Handle number of specified logical processor.
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetExtendedTopologyEnumerationCpuidLeafs (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
  )
{
  UINTN                StartIndex;
  CPU_CPUID_DATA       *CpuidData;

  CpuidData = &(mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // Calculate the start index of Extended Topology Enumeration CPUID leafs.
  //
  StartIndex =  CpuidData->NumberOfBasicCpuidLeafs;
  StartIndex += CpuidData->NumberOfExtendedCpuidLeafs;
  StartIndex += CpuidData->NumberOfCacheAndTlbCpuidLeafs - 1;
  StartIndex += CpuidData->NumberOfDeterministicCacheParametersCpuidLeafs;

  return (&CpuidData->CpuIdLeaf[Index + StartIndex]);
}
/**
  Get the pointer to specified CPUID leaf of deterministic cache parameters.
  
  This function gets the pointer to specified CPUID leaf of deterministic cache parameters.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetDeterministicCacheParametersCpuidLeaf (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
)
{
  UINTN                StartIndex;
  CPU_CPUID_DATA       *CpuidData;

  CpuidData = &(mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // Calculate the start index of deterministic cache CPUID leafs.
  //
  StartIndex =  CpuidData->NumberOfBasicCpuidLeafs;
  StartIndex += CpuidData->NumberOfExtendedCpuidLeafs;
  StartIndex += CpuidData->NumberOfCacheAndTlbCpuidLeafs - 1;

  return (&CpuidData->CpuIdLeaf[Index + StartIndex]);
}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from ProcessorData.c ******************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Returns the procesor version string token installed in the system.

  @param    ProcessorNumber  Processor number of specified processor.
  @param    Version          Pointer to the output processor version.

**/
VOID
GetProcessorVersion (
  IN UINTN                                  ProcessorNumber,
  OUT CPU_PROCESSOR_VERSION_INFORMATION     *Version
  )
{
  CHAR16                BrandIdString[MAXIMUM_CPU_BRAND_STRING_LENGTH + 1];
  EFI_CPUID_REGISTER    *CpuBrandString;
  UINT8                 Index;

  //
  // Create the string using Brand ID String.
  //
  Version->StringValid = FALSE;

  if (IsIntelProcessor (ProcessorNumber)) {
    Version->StringRef = STRING_TOKEN (STR_INTEL_GENUINE_PROCESSOR);

    CpuBrandString = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_BRAND_STRING1);
    ASSERT (CpuBrandString != NULL);

    //
    // Check if Brand ID String is supported or filled up
    //
    if (CpuBrandString->RegEax != 0) {
      AsciiStrToUnicodeStr ((CHAR8 *) CpuBrandString, (CHAR16 *) &BrandIdString[0]);

      CpuBrandString = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_BRAND_STRING2);
      ASSERT (CpuBrandString != NULL);
      AsciiStrToUnicodeStr ((CHAR8 *) CpuBrandString, (CHAR16 *) &BrandIdString[16]);

      CpuBrandString = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_BRAND_STRING3);
      ASSERT (CpuBrandString != NULL);
      AsciiStrToUnicodeStr ((CHAR8 *) CpuBrandString, (CHAR16 *) &BrandIdString[32]);

      //
      // Remove preceeding spaces
      //
      Index = 0;
      while (((Index < MAXIMUM_CPU_BRAND_STRING_LENGTH) && (BrandIdString[Index] == 0x20)) != 0) {
        Index++;
      }

      ASSERT (Index <= MAXIMUM_CPU_BRAND_STRING_LENGTH);
      CopyMem (
        Version->BrandString,
        &BrandIdString[Index],
        (MAXIMUM_CPU_BRAND_STRING_LENGTH - Index) * sizeof (CHAR16)
        );
      Version->BrandString[MAXIMUM_CPU_BRAND_STRING_LENGTH - Index] = 0;
      Version->StringValid = TRUE;
    }
  } else {
    Version->StringRef = STRING_TOKEN (STR_UNKNOWN);
  }
}

/**
  Returns the procesor manufaturer string token installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Manufacturer string token.

**/
EFI_STRING_ID
GetProcessorManufacturer (
  IN UINTN  ProcessorNumber
  )
{
  if (IsIntelProcessor (ProcessorNumber)) {
    return STRING_TOKEN (STR_INTEL_CORPORATION);
  } else {
    return STRING_TOKEN (STR_UNKNOWN);
  }
}

/**
  Checks if processor is Intel or not.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   TRUE                Intel Processor.
  @return   FALSE               Not Intel Processor.

**/
BOOLEAN
IsIntelProcessor (
  IN UINTN  ProcessorNumber
  )
{
  EFI_CPUID_REGISTER  *Reg;

  Reg = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_SIGNATURE);
  ASSERT (Reg != NULL);

  //
  // After CPUID(0), check if EBX contians 'uneG', ECX contains 'letn', and EDX contains 'Ieni'
  //
  if ((Reg->RegEbx != 0x756e6547) || (Reg->RegEcx != 0x6c65746e) || (Reg->RegEdx != 0x49656e69)) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Returns the processor family of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Family

**/
PROCESSOR_FAMILY_DATA
GetProcessorFamily (
  IN UINTN  ProcessorNumber
  )
{
  UINT32  FamilyId;
  UINT32  ModelId;

  if (IsIntelProcessor (ProcessorNumber)) {

    GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);

    //
    // If Merom or Conroe processor family
    //
    if (FamilyId == 0x06 && (ModelId == 0x0F || ModelId == 0x16)) {
      switch (mPlatformType) {
      case PLATFORM_DESKTOP:
        return ProcessorFamilyIntelPentiumD;

      case PLATFORM_MOBILE:
        return ProcessorFamilyIntelPentiumM;

      case PLATFORM_SERVER:
        return ProcessorFamilyIntelXeon;

      default:
        return ProcessorFamilyUnknown;
      }
    }

    // If Quark SoC X1000 family (Family=0x05 Model=0x09)
    if (FamilyId == 0x05 && ModelId == 0x09) {
      return ProcessorFamilyOther;
    }

    return ProcessorFamilyPentium4;
  }

  return ProcessorFamilyUnknown;
}
/**
  Returns the processor voltage of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Voltage in mV

**/
UINT16
GetProcessorVoltage (
  IN UINTN  ProcessorNumber
  )
{
  UINT16             VoltageInmV;
  EFI_CPUID_REGISTER *Reg;

  Reg = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_VERSION_INFO);
  ASSERT (Reg != NULL);

  if ((Reg->RegEax >> 8 & 0x3F) == 0xF) {
    VoltageInmV = 3000;
  } else {
    VoltageInmV = 1600;
  }

  return VoltageInmV;
}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from CacheSubClass.c ******************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Get cache data from CPUID EAX = 2.
  
  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[out]   CacheData           Pointer to the cache data gotten from CPUID EAX = 2.

**/
VOID
GetCacheDataFromCpuid2 (
  IN UINTN              ProcessorNumber,
  OUT CPU_CACHE_DATA    *CacheData
  )
{
  UINT8                     CacheLevel;
  EFI_CPUID_REGISTER        *CacheInformation;
  UINTN                     CacheDescriptorNum;
  UINT32                    RegPointer[4];
  UINT8                     RegIndex;
  UINT32                    RegValue;
  UINT8                     ByteIndex;
  UINT8                     Descriptor;
  UINTN                     DescriptorIndex;

  DEBUG ((EFI_D_INFO, "Get cache data from CPUID EAX = 2\n"));

  CacheDescriptorNum = (UINTN) (sizeof (mCacheConverter) / sizeof (mCacheConverter[0]));
  CacheInformation = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_CACHE_INFO);
  ASSERT (CacheInformation != NULL);

  CopyMem (RegPointer, CacheInformation, sizeof (EFI_CPUID_REGISTER));
  RegPointer[0] &= 0xFFFFFF00;

  for (RegIndex = 0; RegIndex < 4; RegIndex++) {
    RegValue = RegPointer[RegIndex];
    //
    // The most significant bit (bit 31) of each register indicates whether the register
    // contains valid information (set to 0) or is reserved (set to 1).
    //
    if ((RegValue & BIT31) != 0) {
      continue;
    }

    for (ByteIndex = 0; ByteIndex < 4; ByteIndex++) {
      Descriptor = (UINT8) ((RegValue >> (ByteIndex * 8)) & 0xFF);
      for (DescriptorIndex = 0; DescriptorIndex < CacheDescriptorNum; DescriptorIndex++) {
        if (mCacheConverter[DescriptorIndex].CacheDescriptor == Descriptor) {
          CacheLevel = mCacheConverter[DescriptorIndex].CacheLevel; // 1 based
          ASSERT (CacheLevel >= 1 && CacheLevel <= CPU_CACHE_LMAX);
          CacheData[CacheLevel - 1].CacheSizeinKB = (UINT16) (CacheData[CacheLevel - 1].CacheSizeinKB + mCacheConverter[DescriptorIndex].CacheSizeinKB);
          CacheData[CacheLevel - 1].SystemCacheType = mCacheConverter[DescriptorIndex].SystemCacheType;
          CacheData[CacheLevel - 1].Associativity = mCacheConverter[DescriptorIndex].Associativity;
        }
      }   
    }
  }
}

/**
  Get cache data from CPUID EAX = 4.

  CPUID EAX = 4 is Deterministic Cache Parameters Leaf.
  
  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[out]   CacheData           Pointer to the cache data gotten from CPUID EAX = 4.

**/
VOID
GetCacheDataFromCpuid4 (
  IN UINTN              ProcessorNumber,
  OUT CPU_CACHE_DATA    *CacheData
  )
{
  EFI_CPUID_REGISTER            *CpuidRegisters;
  UINT8                         Index;
  UINT8                         NumberOfDeterministicCacheParameters;
  UINT32                        Ways;
  UINT32                        Partitions;
  UINT32                        LineSize;
  UINT32                        Sets;
  CACHE_TYPE_DATA               SystemCacheType;
  CACHE_ASSOCIATIVITY_DATA      Associativity;
  UINT8                         CacheLevel;
  UINT32                        FamilyId;
  UINT32                        ModelId;

  DEBUG ((EFI_D_INFO, "Get cache data from CPUID EAX = 4\n"));

  //
  // If Quark SoC X1000 chip, then force Cache record creation because CPUID Leaf 2 and 4 report 0.
  // Quark SoC X1000 contains a 4-way set associative 16KB cache with a 16 byte cache line and 256 lines per tag.
  // Quark SoC X1000 CPUID.(EAX=1):EAX = 0x00000590 (Family=0x05 Model = 0x09)
  //
  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);
  if (FamilyId == 0x05 && ModelId == 0x09) {
    CacheLevel = 1;
    ASSERT (CacheLevel >= 1 && CacheLevel <= CPU_CACHE_LMAX);
    CacheData[CacheLevel - 1].CacheSizeinKB = 16;
    CacheData[CacheLevel - 1].SystemCacheType = CacheTypeUnified;
    CacheData[CacheLevel - 1].Associativity = CacheAssociativity4Way;
    return;
  }

  NumberOfDeterministicCacheParameters = (UINT8) GetNumberOfCpuidLeafs (ProcessorNumber, DeterministicCacheParametersCpuidLeafs);

  for (Index = 0; Index < NumberOfDeterministicCacheParameters; Index++) {
    CpuidRegisters = GetDeterministicCacheParametersCpuidLeaf (ProcessorNumber, Index);

    if ((CpuidRegisters->RegEax & CPU_CACHE_TYPE_MASK) == 0) {
      //break;
      continue;
    }

    switch (CpuidRegisters->RegEax & CPU_CACHE_TYPE_MASK) {
      case 1:
        SystemCacheType = CacheTypeData;
        break;
      case 2:
        SystemCacheType = CacheTypeInstruction;
        break;
      case 3:
        SystemCacheType = CacheTypeUnified;
        break;
      default:
        SystemCacheType = CacheTypeUnknown;
    }

    Ways = ((CpuidRegisters->RegEbx & CPU_CACHE_WAYS_MASK) >> CPU_CACHE_WAYS_SHIFT) + 1;
    Partitions = ((CpuidRegisters->RegEbx & CPU_CACHE_PARTITIONS_MASK) >> CPU_CACHE_PARTITIONS_SHIFT) + 1;
    LineSize = (CpuidRegisters->RegEbx & CPU_CACHE_LINESIZE_MASK) + 1;
    Sets = CpuidRegisters->RegEcx + 1;

    switch (Ways) {
      case 2:
        Associativity = CacheAssociativity2Way;
        break;
      case 4:
        Associativity = CacheAssociativity4Way;
        break;
      case 8:
        Associativity = CacheAssociativity8Way;
        break;
      case 12:
        Associativity = CacheAssociativity12Way;
        break;
      case 16:
        Associativity = CacheAssociativity16Way;
        break;
      case 24:
        Associativity = CacheAssociativity24Way;
        break;
      case 32:
        Associativity = CacheAssociativity32Way;
        break;
      case 48:
        Associativity = CacheAssociativity48Way;
        break;
      case 64:
        Associativity = CacheAssociativity64Way;
        break;
      default:
        Associativity = CacheAssociativityFully;
        break;
    }

    CacheLevel = (UINT8) ((CpuidRegisters->RegEax & CPU_CACHE_LEVEL_MASK) >> CPU_CACHE_LEVEL_SHIFT); // 1 based
    ASSERT (CacheLevel >= 1 && CacheLevel <= CPU_CACHE_LMAX);
    CacheData[CacheLevel - 1].CacheSizeinKB = (UINT16) (CacheData[CacheLevel - 1].CacheSizeinKB + (Ways * Partitions * LineSize * Sets) / 1024);
    CacheData[CacheLevel - 1].SystemCacheType = SystemCacheType;
    CacheData[CacheLevel - 1].Associativity = Associativity;
  }

}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from CpuConfig.c **********************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Get the number of CPUID leafs of various types.
  
  This function get the number of CPUID leafs of various types.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  CpuidType         The type of the cpu id.

  @return Maximal index of CPUID instruction for basic leafs.

**/
UINTN
EFIAPI
GetNumberOfCpuidLeafs (
  IN  UINTN               ProcessorNumber,
  IN  CPUID_TYPE          CpuidType
  )
{
  UINTN                NumberOfLeafs;
  CPU_CPUID_DATA       *CpuidData;

  CpuidData = &(mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber].CpuidData);

  switch (CpuidType) {
  case BasicCpuidLeaf:
    NumberOfLeafs = CpuidData->NumberOfBasicCpuidLeafs;
    break;
  case ExtendedCpuidLeaf:
    NumberOfLeafs = CpuidData->NumberOfExtendedCpuidLeafs;
    break;
  case CacheAndTlbCpuidLeafs:
    NumberOfLeafs = CpuidData->NumberOfCacheAndTlbCpuidLeafs;
    break;
  case DeterministicCacheParametersCpuidLeafs:
    NumberOfLeafs = CpuidData->NumberOfDeterministicCacheParametersCpuidLeafs;
    break;
  case ExtendedTopologyEnumerationCpuidLeafs:
    NumberOfLeafs = CpuidData->NumberOfExtendedTopologyEnumerationLeafs;
    break;
  default:
    NumberOfLeafs = 0;
    break;
  }

  return NumberOfLeafs;
}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from DataCollection.c *****************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Collects all CPUID leafs the processor.

  This function collects all CPUID leafs the processor.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectCpuidLeafs (
  UINTN    ProcessorNumber
  )
{
  CPU_COLLECTED_DATA   *CpuCollectedData;
  EFI_CPUID_REGISTER   *CpuidRegisters;
  UINT32               Index;

  CpuCollectedData = &mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber];
  //
  // Collect basic CPUID information.
  //
  CpuidRegisters = CpuCollectedData->CpuidData.CpuIdLeaf;
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, BasicCpuidLeaf); Index++) {
    AsmCpuid (
      Index,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect extended function CPUID information.
  //
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, ExtendedCpuidLeaf); Index++) {
    AsmCpuid (
      Index + 0x80000000,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect additional Cache & TLB information, if exists.
  //
  for (Index = 1; Index < GetNumberOfCpuidLeafs (ProcessorNumber, CacheAndTlbCpuidLeafs); Index++) {
    AsmCpuid (
      2,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect Deterministic Cache Parameters Leaf.
  //
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, DeterministicCacheParametersCpuidLeafs); Index++) {
    AsmCpuidEx (
      4,
      Index,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect Extended Topology Enumeration Leaf.
  //
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, ExtendedTopologyEnumerationCpuidLeafs); Index++) {
    AsmCpuidEx (
      EFI_CPUID_CORE_TOPOLOGY,
      Index,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }
}
/**
  Checks the number of CPUID leafs need by a processor.

  This function check the number of CPUID leafs need by a processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
CountNumberOfCpuidLeafs (
  IN UINTN  ProcessorNumber
  )
{
  UINT32               MaxCpuidIndex;
  UINT32               MaxExtendedCpuidIndex;
  UINT32               NumberOfCacheAndTlbRecords;
  UINT32               NumberOfDeterministicCacheParametersLeafs;
  UINT32               NumberOfExtendedTopologyEnumerationLeafs;
  UINT32               RegValue;
  CPU_COLLECTED_DATA   *CpuCollectedData;

  CpuCollectedData = &mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber];

  //
  // Get the max index of basic CPUID
  //
  AsmCpuid (0, &MaxCpuidIndex, NULL, NULL, NULL);
  //
  // Get the max index of extended CPUID
  //
  AsmCpuid (0x80000000, &MaxExtendedCpuidIndex, NULL, NULL, NULL);
  //
  // Get the number of cache and TLB CPUID leafs
  //
  AsmCpuid (2, &NumberOfCacheAndTlbRecords, NULL, NULL, NULL);
  NumberOfCacheAndTlbRecords = NumberOfCacheAndTlbRecords & 0xff;

  //
  // Get the number of deterministic cache parameter CPUID leafs
  //
  NumberOfDeterministicCacheParametersLeafs = 0;
  do {
    AsmCpuidEx (4, NumberOfDeterministicCacheParametersLeafs++, &RegValue, NULL, NULL, NULL);
  } while ((RegValue & 0x0f) != 0);

  //
  // Get the number of Extended Topology Enumeration CPUID leafs
  //
  NumberOfExtendedTopologyEnumerationLeafs = 0;
  if (MaxCpuidIndex >= EFI_CPUID_CORE_TOPOLOGY) {
    do {
      AsmCpuidEx (EFI_CPUID_CORE_TOPOLOGY, NumberOfExtendedTopologyEnumerationLeafs++, NULL, &RegValue, NULL, NULL);
    } while ((RegValue & 0x0FFFF) != 0);
  }

  //
  // Save collected data in Processor Configuration Context Buffer
  //
  CpuCollectedData->CpuidData.NumberOfBasicCpuidLeafs                        = MaxCpuidIndex + 1;
  CpuCollectedData->CpuidData.NumberOfExtendedCpuidLeafs                     = (MaxExtendedCpuidIndex - 0x80000000) + 1;
  CpuCollectedData->CpuidData.NumberOfCacheAndTlbCpuidLeafs                  = NumberOfCacheAndTlbRecords;
  CpuCollectedData->CpuidData.NumberOfDeterministicCacheParametersCpuidLeafs = NumberOfDeterministicCacheParametersLeafs;
  CpuCollectedData->CpuidData.NumberOfExtendedTopologyEnumerationLeafs       = NumberOfExtendedTopologyEnumerationLeafs;
}
/**
  Checks the number of CPUID leafs of all logical processors, and allocate memory for them.

  This function checks the number of CPUID leafs of all logical processors, and allocates memory for them.

**/
VOID
AllocateMemoryForCpuidLeafs (
  VOID
  )
{
  CPU_COLLECTED_DATA   *CpuCollectedData;
  UINTN                ProcessorNumber;
  UINTN                NumberOfLeafs;


  //
  // Allocate memory for CPUID leafs of all processors
  //
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigConextBuffer.NumberOfProcessors; ProcessorNumber++) {
    CpuCollectedData = &mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber];

    //
    // Get the number of basic CPUID leafs.
    //
    NumberOfLeafs = CpuCollectedData->CpuidData.NumberOfBasicCpuidLeafs;
    //
    // Get the number of extended CPUID leafs.
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfExtendedCpuidLeafs;
    //
    // Get the number of cache and TLB CPUID leafs.
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfCacheAndTlbCpuidLeafs - 1;
    //
    // Get the number of deterministic cache parameters CPUID leafs.
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfDeterministicCacheParametersCpuidLeafs;
    //
    // Get the number of Extended Topology Enumeration CPUID leafs
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfExtendedTopologyEnumerationLeafs;

    CpuCollectedData->CpuidData.CpuIdLeaf = AllocateZeroPool (sizeof (EFI_CPUID_REGISTER) * NumberOfLeafs);
  }
}

/**
  Collects intended FSB frequency and core to bus ratio.

  This function collects intended FSB frequency and core to bus ratio.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectFrequencyData (
  UINTN    ProcessorNumber
  )
{
  CPU_MISC_DATA          *CpuMiscData;
  UINT64                 QWord;
  UINT32                 FamilyId;
  UINT32                 ModelId;
  UINT32                 SteppingId;

  CpuMiscData = &mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber].CpuMiscData;
  CpuMiscData->FrequencyLocked = FALSE;
  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, &SteppingId, NULL);

  switch (FamilyId) {
  case 0x06:
    switch (ModelId) {
    case 0x4C:
      QWord = AsmReadMsr64 (0x66A);
      CpuMiscData->MinCoreToBusRatio = (UINTN) BitFieldRead64 (QWord, 8, 15);
      CpuMiscData->MaxCoreToBusRatio = (UINTN) BitFieldRead64 (QWord, 16, 23);

      //
      // Collect VID
      //
      QWord = AsmReadMsr64 (0x66B);
      CpuMiscData->MinVid = (UINTN) BitFieldRead64 (QWord, 8, 15);
      CpuMiscData->MaxVid = (UINTN) BitFieldRead64 (QWord, 16, 23);
      QWord = AsmReadMsr64 (0x66C);
      CpuMiscData->MaxTurboRatio = (UINTN) BitFieldRead64 (QWord, 0, 7);
		
    default:
      break;
    }
    break;
  default:
    break;
  }

  //
  // Collect intended FSB frequency
  //
  CpuMiscData->IntendedFsbFrequency = 80;

  //
  // Default number of P-states is 1
  //
  CpuMiscData->NumberOfPStates = 1;
}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from CacheSubClass.c ******************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Add Type 7 SMBIOS Record for Cache Information.
  
  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[out]   L1CacheHandle       Pointer to the handle of the L1 Cache SMBIOS record.
  @param[out]   L2CacheHandle       Pointer to the handle of the L2 Cache SMBIOS record.
  @param[out]   L3CacheHandle       Pointer to the handle of the L3 Cache SMBIOS record.

**/
VOID
AddSmbiosCacheTypeTable (
  IN  EFI_SMBIOS_PROTOCOL  *Smbios,
  IN  UINTN              ProcessorNumber,
  OUT EFI_SMBIOS_HANDLE *L1CacheHandle,
  OUT EFI_SMBIOS_HANDLE *L2CacheHandle,
  OUT EFI_SMBIOS_HANDLE *L3CacheHandle
  )
{
  EFI_STATUS                    Status;
  SMBIOS_TABLE_TYPE7            *SmbiosRecord;
  EFI_SMBIOS_HANDLE             SmbiosHandle;
  UINT8                         CacheLevel;
  CPU_CACHE_DATA                CacheData[CPU_CACHE_LMAX];
  CHAR8                         *OptionalStrStart;
  UINTN                         StringBufferSize;
  UINTN                         CacheSocketStrLen;
  EFI_STRING                    CacheSocketStr;
  CACHE_SRAM_TYPE_DATA          CacheSramType;
  CPU_CACHE_CONFIGURATION_DATA  CacheConfig;

  ZeroMem (CacheData, CPU_CACHE_LMAX * sizeof (CPU_CACHE_DATA));
  //
  // Check whether the CPU supports CPUID EAX = 4, if yes, get cache data from CPUID EAX = 4,
  // or no, get cache data from CPUID EAX = 2 to be compatible with the earlier CPU.
  //
  if (GetNumberOfCpuidLeafs (ProcessorNumber, BasicCpuidLeaf) > 4 ) {
    GetCacheDataFromCpuid4 (ProcessorNumber, CacheData);
  } else {
    GetCacheDataFromCpuid2 (ProcessorNumber, CacheData);
  }

  //
  // Now cache data has been ready.
  //
  for (CacheLevel = 0; CacheLevel < CPU_CACHE_LMAX; CacheLevel++) {
    //
    // NO smbios record for zero-sized cache.
    //
    if (CacheData[CacheLevel].CacheSizeinKB == 0) {
      continue;
    }

    DEBUG ((
      EFI_D_INFO,
      "CacheData: CacheLevel = 0x%x CacheSizeinKB = 0x%xKB SystemCacheType = 0x%x Associativity = 0x%x\n",
      CacheLevel + 1,
      CacheData[CacheLevel].CacheSizeinKB,
      CacheData[CacheLevel].SystemCacheType,
      CacheData[CacheLevel].Associativity
      ));


    StringBufferSize = sizeof (CHAR16) * SMBIOS_STRING_MAX_LENGTH;
    CacheSocketStr = AllocateZeroPool (StringBufferSize);
    ASSERT (CacheSocketStr != NULL);
    CacheSocketStrLen = UnicodeSPrint (CacheSocketStr, StringBufferSize, L"L%x-Cache", CacheLevel + 1);
    ASSERT (CacheSocketStrLen <= SMBIOS_STRING_MAX_LENGTH);

    //
    // Report Cache Information to Type 7 SMBIOS Record.
    //

    SmbiosRecord = AllocatePool (sizeof (SMBIOS_TABLE_TYPE7) + CacheSocketStrLen + 1 + 1);
    ASSERT (SmbiosRecord != NULL);
    ZeroMem (SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE7) + CacheSocketStrLen + 1 + 1);

    SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_CACHE_INFORMATION;
    SmbiosRecord->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE7);
    //
    // Make handle chosen by smbios protocol.add automatically.
    //
    SmbiosRecord->Hdr.Handle = 0;
    //
    // Socket will be the 1st optional string following the formatted structure.
    //
    SmbiosRecord->SocketDesignation = 1;

    //
    // Cache Level - 1 through 8, e.g. an L1 cache would use value 000b and an L3 cache would use 010b.
    //
    CacheConfig.Level = CacheLevel;
    CacheConfig.Socketed = 0;           // Not Socketed
    CacheConfig.Reserved2 = 0;
    CacheConfig.Location = 0;           // Internal Cache
    CacheConfig.Enable = 1;             // Cache enabled
    CacheConfig.OperationalMode = 1;    // Write Back
    CacheConfig.Reserved1 = 0;
    CopyMem (&SmbiosRecord->CacheConfiguration, &CacheConfig, 2);
    //
    // Only 1K granularity assumed here.
    //
    SmbiosRecord->MaximumCacheSize = CacheData[CacheLevel].CacheSizeinKB;
    SmbiosRecord->InstalledSize = CacheData[CacheLevel].CacheSizeinKB;

    ZeroMem (&CacheSramType, sizeof (CACHE_SRAM_TYPE_DATA));
    CacheSramType.Synchronous = 1;
    CopyMem (&SmbiosRecord->SupportedSRAMType, &CacheSramType, 2);
    CopyMem (&SmbiosRecord->CurrentSRAMType, &CacheSramType, 2);

    SmbiosRecord->CacheSpeed = 0;
    SmbiosRecord->ErrorCorrectionType = CacheErrorSingleBit;
    SmbiosRecord->SystemCacheType = (UINT8) CacheData[CacheLevel].SystemCacheType;
    SmbiosRecord->Associativity = (UINT8) CacheData[CacheLevel].Associativity;

    OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);
    UnicodeStrToAsciiStr (CacheSocketStr, OptionalStrStart);

    // 
    // Now we have got the full smbios record, call smbios protocol to add this record.
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add ( Smbios, 
	                       NULL, 
						   &SmbiosHandle, 
						   (EFI_SMBIOS_TABLE_HEADER*) SmbiosRecord
						  );

    //
    // Record L1/L2/L3 Cache Smbios Handle, Type 4 SMBIOS Record needs it.
    //
    if (CacheLevel + 1 == CPU_CACHE_L1) {
      *L1CacheHandle = SmbiosHandle;
    } else if (CacheLevel + 1 == CPU_CACHE_L2) {
      *L2CacheHandle = SmbiosHandle;
    } else if (CacheLevel + 1 == CPU_CACHE_L3) {
      *L3CacheHandle  = SmbiosHandle;
    }
    FreePool (SmbiosRecord);
    FreePool (CacheSocketStr);
    ASSERT_EFI_ERROR (Status);
  } 
}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from CpuConfig.c **********************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Get the pointer to specified CPUID leaf.
  
  This function gets the pointer to specified CPUID leaf.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf

**/
EFI_CPUID_REGISTER*
EFIAPI
GetProcessorCpuid (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
)
{
  CPU_CPUID_DATA       *CpuidData;

  CpuidData = &(mCpuConfigConextBuffer.CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // If Index specifies basic CPUID leafs, then locate basic leaf to return.
  //
  if (Index < CpuidData->NumberOfBasicCpuidLeafs) {
    return (&CpuidData->CpuIdLeaf[Index]);
  }

  //
  // If Index specifies extended CPUID leafs, then locate extended leaf to return.
  //
  if (Index >= 0x80000000 && Index < 0x80000000 + CpuidData->NumberOfExtendedCpuidLeafs) {
    return (&CpuidData->CpuIdLeaf[Index - 0x80000000 + CpuidData->NumberOfBasicCpuidLeafs]);
  }

  //
  // If Index specifies invalid CPUID index, then return NULL.
  //
  return NULL;
}

/**
  Get the version information of specified logical processor.
  
  This function gets the version information of specified logical processor,
  including family ID, model ID, stepping ID and processor type.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  DisplayedFamily   Pointer to family ID for output
  @param  DisplayedModel    Pointer to model ID for output
  @param  SteppingId        Pointer to stepping ID for output
  @param  ProcessorType     Pointer to processor type for output

**/
VOID
EFIAPI
GetProcessorVersionInfo (
  IN  UINTN               ProcessorNumber,
  OUT UINT32              *DisplayedFamily OPTIONAL,
  OUT UINT32              *DisplayedModel  OPTIONAL,       
  OUT UINT32              *SteppingId      OPTIONAL,
  OUT UINT32              *ProcessorType   OPTIONAL
  )
{
  EFI_CPUID_REGISTER      *VersionInfo;
  UINT32                  RegEax;
  UINT32                  FamilyId;
  UINT32                  ExtendedFamilyId;
  UINT32                  ExtendedModelId;

  //
  // Get CPUID(1).EAX
  //
  VersionInfo = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_VERSION_INFO);
  ASSERT (VersionInfo != NULL);
  RegEax      = VersionInfo->RegEax;

  //
  // Processor Type is CPUID(1).EAX[12:13]
  //
  if (ProcessorType != NULL) {
    *ProcessorType = BitFieldRead32 (RegEax, 12, 13);
  }

  //
  // Stepping ID is CPUID(1).EAX[0:3]
  //
  if (SteppingId != NULL) {
    *SteppingId = BitFieldRead32 (RegEax, 0, 3);
  }

  //
  // The Extended Family ID needs to be examined only when the Family ID is 0FH.
  // If Family ID is 0FH, Displayed Family ID = Family ID + Extended Family ID.
  // Otherwise, Displayed Family ID is Family ID
  //
  FamilyId = BitFieldRead32 (RegEax, 8, 11);
  if (DisplayedFamily != NULL) {
    *DisplayedFamily = FamilyId;
    if (FamilyId == 0x0f) {
      ExtendedFamilyId = BitFieldRead32 (RegEax, 20, 27);
      *DisplayedFamily += ExtendedFamilyId;
    }
  }

  //
  // The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
  // If Family ID is 06H or 0FH, Displayed Model ID = Model ID + (Extended Model ID << 4).
  // Otherwise, Displayed Model ID is Model ID.
  //
  if (DisplayedModel != NULL) {
    *DisplayedModel = BitFieldRead32 (RegEax, 4, 7);
    if (FamilyId == 0x06 || FamilyId == 0x0f) {
      ExtendedModelId = BitFieldRead32 (RegEax, 16, 19);
      *DisplayedModel += (ExtendedModelId << 4);
    }
  }
}

/***********************************************************************************************************************
 ***********************************************************************************************************************
 *************************************** The following functions are from ProcessorSubClass.c  *************************
 ***********************************************************************************************************************
 ***********************************************************************************************************************/
/**
  Add Processor Information to Type 4 SMBIOS Record for Socket Populated.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[in]    L1CacheHandle       The handle of the L1 Cache SMBIOS record.
  @param[in]    L2CacheHandle       The handle of the L2 Cache SMBIOS record.
  @param[in]    L3CacheHandle       The handle of the L3 Cache SMBIOS record.

**/
VOID
AddSmbiosProcessorTypeTable (
  IN EFI_SMBIOS_PROTOCOL  *Smbios,
  IN UINTN              ProcessorNumber,
  IN EFI_SMBIOS_HANDLE  L1CacheHandle,
  IN EFI_SMBIOS_HANDLE  L2CacheHandle,
  IN EFI_SMBIOS_HANDLE  L3CacheHandle
  )
{
  EFI_STATUS                    Status;
  EFI_SMBIOS_HANDLE             SmbiosHandle;
  UINTN                         TotalSize;
  EFI_STRING_ID                 Token;
  CHAR8                         *OptionalStrStart;
  EFI_STRING                    CpuManuStr;
  EFI_STRING                    CpuVerStr;
  EFI_STRING                    CpuSocketStr;
  EFI_STRING                    CpuAssetTagStr;
  UINTN                         CpuManuStrLen;
  UINTN                         CpuVerStrLen;
  UINTN                         CpuSocketStrLen;
  UINTN                         CpuAssetTagStrLen;
  SMBIOS_TABLE_TYPE4            *SmbiosRecord;
  EFI_CPUID_REGISTER            *CpuidRegister;
  UINT16                        ProcessorVoltage;
  CPU_PROCESSOR_VERSION_INFORMATION     Version;
  CPU_PROCESSOR_STATUS_DATA     ProcessorStatus;
  CPU_PROCESSOR_CHARACTERISTICS_DATA    ProcessorCharacteristics;
  UINT16                        PackageThreadCount;
  UINT16                        CoreThreadCount;
  UINT16                        CoreCount;
  UINT32                        FamilyId;
  UINT32                        ModelId;

  CoreCount = 0;
  PackageThreadCount = 0;

  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);

  //
  // Get CPU Socket string, it will be updated when PcdPlatformCpuSocketNames is set.
  //
  Token = STRING_TOKEN (STR_UNKNOWN);
  CpuSocketStr = HiiGetPackageString (&gEfiCallerIdGuid, Token ,NULL);
  ASSERT (CpuSocketStr != NULL);
  CpuSocketStrLen = StrLen (CpuSocketStr);
  ASSERT (CpuSocketStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU Manufacturer string.
  //
  Token = GetProcessorManufacturer (ProcessorNumber);
  CpuManuStr = HiiGetPackageString (&gEfiCallerIdGuid, Token, NULL);
  ASSERT (CpuManuStr != NULL);
  CpuManuStrLen = StrLen (CpuManuStr);
  ASSERT (CpuManuStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU Version string.
  //
  GetProcessorVersion (ProcessorNumber, &Version);
  if (Version.StringValid) {
    Token = HiiSetString (mHiiHandle, 0, Version.BrandString, NULL);
    if (Token == 0) {
      Token = Version.StringRef;
    }
  } else {
      Token = Version.StringRef;
  }
  CpuVerStr = HiiGetPackageString (&gEfiCallerIdGuid, Token, NULL);
  ASSERT (CpuVerStr != NULL);
  CpuVerStrLen = StrLen (CpuVerStr);
  ASSERT (CpuVerStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU Asset Tag string, it will be updated when PcdPlatformCpuAssetTags is set.
  //
  Token = STRING_TOKEN (STR_UNKNOWN);
  CpuAssetTagStr = HiiGetPackageString (&gEfiCallerIdGuid, Token ,NULL);
  ASSERT (CpuAssetTagStr != NULL);
  CpuAssetTagStrLen = StrLen (CpuAssetTagStr);
  ASSERT (CpuAssetTagStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU core count.
  //
  if (GetNumberOfCpuidLeafs (ProcessorNumber, BasicCpuidLeaf) > EFI_CPUID_CORE_TOPOLOGY) {
    CpuidRegister = GetExtendedTopologyEnumerationCpuidLeafs (ProcessorNumber, 1);
    PackageThreadCount = (UINT16) (CpuidRegister->RegEbx);
    CpuidRegister = GetExtendedTopologyEnumerationCpuidLeafs (ProcessorNumber, 0);
    CoreThreadCount = (UINT16) (CpuidRegister->RegEbx);
    CoreCount = PackageThreadCount / CoreThreadCount;
  } else {
    //
    // Quark SoC X1000 CPUID.(EAX=1):EAX = 0x00000590 (Family=0x05 Model = 0x09)
    //
    if (FamilyId == 0x05 && ModelId == 0x09) {
      PackageThreadCount = 1;
      CoreThreadCount = 1;
      CoreCount = PackageThreadCount / CoreThreadCount;
    }
  }

  //
  // Report Processor Information to Type 4 SMBIOS Record.
  //

  TotalSize = sizeof (SMBIOS_TABLE_TYPE4) + CpuSocketStrLen + 1 + CpuManuStrLen + 1 + CpuVerStrLen + 1 + CpuAssetTagStrLen + 1 + 1;
  SmbiosRecord = AllocatePool (TotalSize);
  ASSERT (SmbiosRecord != NULL);
  ZeroMem (SmbiosRecord, TotalSize);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  SmbiosRecord->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE4);
  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;
  //
  // Socket will be the 1st optional string following the formatted structure.
  //
  SmbiosRecord->Socket = (UINT8) mCpuSocketStrNumber;
  SmbiosRecord->ProcessorType = CentralProcessor;
  SmbiosRecord->ProcessorFamily = (UINT8) GetProcessorFamily (ProcessorNumber); 
  //
  // Manu will be the 2nd optional string following the formatted structure.
  //
  SmbiosRecord->ProcessorManufacture = 2;   

  CpuidRegister = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_VERSION_INFO);
  ASSERT (CpuidRegister != NULL);
  *(UINT32 *) &SmbiosRecord->ProcessorId.Signature = CpuidRegister->RegEax;
  *(UINT32 *) &SmbiosRecord->ProcessorId.FeatureFlags = CpuidRegister->RegEdx;

  //
  // Version will be the 3rd optional string following the formatted structure.
  // 
  SmbiosRecord->ProcessorVersion = 3;   

  ProcessorVoltage = GetProcessorVoltage (ProcessorNumber); // mV unit
  ProcessorVoltage = (UINT16) ((ProcessorVoltage * 10) / 1000);
  *(UINT8 *) &SmbiosRecord->Voltage = (UINT8) ProcessorVoltage;
  SmbiosRecord->Voltage.ProcessorVoltageIndicateLegacy = 1;

  SmbiosRecord->ExternalClock = (UINT16) (GET_CPU_MISC_DATA (ProcessorNumber, IntendedFsbFrequency));
  SmbiosRecord->CurrentSpeed = (UINT16) (GET_CPU_MISC_DATA (ProcessorNumber, IntendedFsbFrequency) * GET_CPU_MISC_DATA (ProcessorNumber, MaxCoreToBusRatio));

  ProcessorStatus.CpuStatus = 1;        // CPU Enabled
  ProcessorStatus.Reserved1 = 0;
  ProcessorStatus.SocketPopulated = 1;  // CPU Socket Populated
  ProcessorStatus.Reserved2 = 0;
  CopyMem (&SmbiosRecord->Status, &ProcessorStatus, 1);

  //
  // Quark SoC X1000 CPUID.(EAX=1):EAX = 0x00000590 (Family=0x05 Model = 0x09)
  //
  if (FamilyId == 0x05 && ModelId == 0x09) {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeNone;
  } else {
  SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA775;
  }

  SmbiosRecord->L1CacheHandle = L1CacheHandle;
  SmbiosRecord->L2CacheHandle = L2CacheHandle;
  SmbiosRecord->L3CacheHandle = L3CacheHandle;

  //
  // AssetTag will be the 4th optional string following the formatted structure.
  //
  SmbiosRecord->AssetTag = (UINT8) mCpuAssetTagStrNumber;

  CoreCount = (CoreCount > 255) ? 0 : CoreCount;
  PackageThreadCount = (PackageThreadCount > 255) ? 0 : PackageThreadCount;
  SmbiosRecord->CoreCount = (UINT8) CoreCount;
  SmbiosRecord->EnabledCoreCount = (UINT8) CoreCount;
  SmbiosRecord->ThreadCount = (UINT8) PackageThreadCount; 

  ProcessorCharacteristics.Reserved = 0;
  ProcessorCharacteristics.Unknown = 0;
  ProcessorCharacteristics.Reserved2 = 0;
  //
  // Quark SoC X1000 CPUID.(EAX=1):EAX = 0x00000590 (Family=0x05 Model = 0x09)
  //
  if (FamilyId == 0x05 && ModelId == 0x09) {
    ProcessorCharacteristics.Capable64Bit = 0;
  } else {
    ProcessorCharacteristics.Capable64Bit = 1;
  }
  CopyMem (&SmbiosRecord->ProcessorCharacteristics, &ProcessorCharacteristics, 2);

  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);
  UnicodeStrToAsciiStr (CpuSocketStr, OptionalStrStart);
  UnicodeStrToAsciiStr (CpuManuStr, OptionalStrStart + CpuSocketStrLen + 1);
  UnicodeStrToAsciiStr (CpuVerStr, OptionalStrStart + CpuSocketStrLen + 1 + CpuManuStrLen + 1);
  UnicodeStrToAsciiStr (CpuAssetTagStr, OptionalStrStart + CpuSocketStrLen + 1 + CpuManuStrLen + 1 + CpuVerStrLen + 1);
  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = Smbios->Add ( Smbios, 
	                     NULL, 
						 &SmbiosHandle, 
						 (EFI_SMBIOS_TABLE_HEADER*) SmbiosRecord
						);
  FreePool (SmbiosRecord);
  FreePool (CpuSocketStr);
  FreePool (CpuManuStr);
  FreePool (CpuVerStr);
  FreePool (CpuAssetTagStr);
  ASSERT_EFI_ERROR (Status);
}

EFI_STATUS
GetSmbiosCpuInformation ( EFI_SMBIOS_PROTOCOL  *Smbios
  )
/*++

  Routine Description:

    This function will get the CPU information to fill SMBIOS type4/type7.


  Arguments:

    None

  Returns:

    EFI_SUCCESS if the data is successfully reported
    EFI_NOT_FOUND if the HOB list could not be located.

--*/
{
  EFI_STATUS			  Status = EFI_SUCCESS;
  EFI_MP_SERVICES_PROTOCOL      *MpService;
  UINTN                         MaximumNumberOfCPUs;
  UINTN                         NumberOfEnabledCPUs;
  UINT32                        PreviousPackageNumber;
  UINTN                         ProcessorIndex;
  EFI_SMBIOS_HANDLE     L1CacheHandle;
  EFI_SMBIOS_HANDLE     L2CacheHandle;
  EFI_SMBIOS_HANDLE     L3CacheHandle;
  EFI_PROCESSOR_INFORMATION                                   ProcessorInfoBuffer;
  UINTN                 *SocketProcessorNumberTable;
  UINT32                SocketIndex;
  UINTN                 Index;
  L1CacheHandle = 0xFFFF;
  L2CacheHandle = 0xFFFF;
  L3CacheHandle = 0xFFFF;

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &MpService
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
               MpService,
               &MaximumNumberOfCPUs,
               &NumberOfEnabledCPUs
               );

  SocketProcessorNumberTable = AllocateZeroPool (MaximumNumberOfCPUs * sizeof (UINTN));
  SocketProcessorNumberTable[0] = 0;
  ASSERT (SocketProcessorNumberTable != NULL);

  //
  // Initialize data for CPU configuration context buffer
  //
  mCpuConfigConextBuffer.BspNumber = 0;
  mCpuConfigConextBuffer.NumberOfProcessors = NumberOfEnabledCPUs;
  mCpuConfigConextBuffer.CollectedDataBuffer  = AllocateZeroPool (sizeof (CPU_COLLECTED_DATA) * NumberOfEnabledCPUs);

  mCpuConfigConextBuffer.SettingSequence = (UINTN *) (((UINTN) mCpuConfigConextBuffer.RegisterTable) + (sizeof (CPU_REGISTER_TABLE) * NumberOfEnabledCPUs));
  
  for (Index = 0; Index < NumberOfEnabledCPUs; Index++) {
    mCpuConfigConextBuffer.SettingSequence[Index] = Index;
    CountNumberOfCpuidLeafs (Index);
  }

  //
  // Check the number of CPUID leafs of all logical processors, and allocate memory for them.
  //
  AllocateMemoryForCpuidLeafs ();
  for (Index = 0; Index < NumberOfEnabledCPUs; Index++) {
    CollectCpuidLeafs (Index);
    CollectFrequencyData(Index);
  }
  //
  // System has 1 populated socket at least, initialize mPopulatedSocketCount to 1 and record ProcessorNumber 0 for it.
  //
  mPopulatedSocketCount = 1;
  GetProcessorLocation (0, &PreviousPackageNumber, NULL, NULL);
  //
  // Scan and compare the processors' PackageNumber to find the populated sockets.
  //
  for (ProcessorIndex = 1; ProcessorIndex < NumberOfEnabledCPUs; ProcessorIndex++) {
    Status = MpService->GetProcessorInfo (
                    MpService,
                    ProcessorIndex,
                    &ProcessorInfoBuffer
                    );
    if (ProcessorInfoBuffer.Location.Package != PreviousPackageNumber) {
      //
      // Found a new populated socket.
      //
      PreviousPackageNumber = ProcessorInfoBuffer.Location.Package;
      mPopulatedSocketCount++;
      SocketProcessorNumberTable[mPopulatedSocketCount - 1] = ProcessorIndex;
    }
  }

  //
  // Add SMBIOS tables for populated sockets.
  //
  for (SocketIndex = 0; SocketIndex < mPopulatedSocketCount; SocketIndex++) {
    AddSmbiosCacheTypeTable (Smbios, SocketProcessorNumberTable[SocketIndex], &L1CacheHandle, &L2CacheHandle, &L3CacheHandle);
    AddSmbiosProcessorTypeTable (Smbios, SocketProcessorNumberTable[SocketIndex], L1CacheHandle, L2CacheHandle, L3CacheHandle);
  }
  FreePool (SocketProcessorNumberTable);  
  FreePool (mCpuConfigConextBuffer.CollectedDataBuffer);

  return Status;
}
