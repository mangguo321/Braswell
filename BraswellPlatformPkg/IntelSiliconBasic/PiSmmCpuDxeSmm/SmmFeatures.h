/** @file
  The CPU specific programming for PiSmmCpuDxeSmm module.

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SMM_FEATURES_H__
#define __SMM_FEATURES_H__

////////
// Below definition is from IA32 SDM
////////
#define EFI_CPUID_VERSION_INFO                 0x1
#define EFI_CPUID_CORE_TOPOLOGY                0x0B
#define EFI_CPUID_EXTENDED_FUNCTION            0x80000000
#define EFI_CPUID_VIR_PHY_ADDRESS_SIZE         0x80000008

#define EFI_MSR_IA32_MTRR_CAP                  0xFE
#define  IA32_MTRR_SMRR_SUPPORT_BIT            BIT11

#define EFI_MSR_IA32_FEATURE_CONTROL           0x3A
#define  IA32_SMRR_ENABLE_BIT                  BIT3

#define  EFI_MSR_SMRR_PHYS_MASK_VALID          BIT11
#define  EFI_MSR_SMRR_MASK                     0xFFFFF000

#define EFI_MSR_SMRR_PHYS_BASE                 0x1F2
#define EFI_MSR_SMRR_PHYS_MASK                 0x1F3
#define CACHE_WRITE_PROTECT                    5
#define CACHE_WRITE_BACK                       6
#define SMM_DEFAULT_SMBASE                     0x30000

#define EFI_MSR_HASWELL_SMM_MCA_CAP            0x17D
#define  SMM_CODE_ACCESS_CHK_BIT               BIT58
#define  LONG_FLOW_INDICATION_BIT              BIT59

#define EFI_MSR_HASWELL_SMM_FEATURE_CONTROL    0x4E0
#define  SMM_FEATURE_CONTROL_LOCK_BIT          BIT0
#define  SMM_CODE_CHK_EN_BIT                   BIT2

#define SMM_HASWELL_CLIENT_LOG_PROC_EN_BIT_LENGTH     12

#define EFI_MSR_HASWELL_SMM_DELAYED            0x4E2

#define EFI_MSR_HASWELL_SMM_BLOCKED            0x4E3

////////
// Below section is definition for CPU SMM Feature context
////////

//
// Structure to describe CPU identification mapping
// if ((CPUID_EAX(1) & Mask) == (Signature & Mask)), it means matched.
//
typedef struct {
  UINT32  Signature;
  UINT32  Mask;
} CPUID_MAPPING;

//
// CPU SMM familiy
//
typedef enum {
  CpuCherryView,
  CpuSmmFamilyMax
} CPU_SMM_FAMILY;

//
// Structure to describe CPU SMM class
//
typedef struct {
  CPU_SMM_FAMILY    Family;
  UINT32            MappingCount;
  CPUID_MAPPING     *MappingTable;
} CPU_SMM_CLASS;

//
// Structure to describe CPU_SMM_FEATURE_CONTEXT
//
typedef struct {
  BOOLEAN          SmrrEnabled;
} CPU_SMM_FEATURE_CONTEXT;

//
// ATOM CPUID signatures
//
#define CPUID_SIGNATURE_CHERRYVIEW              0x000406C0

//
// CPUID masks
//
#define CPUID_MASK_NO_STEPPING                  0x0FFF0FF0
#define CPUID_MASK_NO_STEPPING_MODEL            0x0FFF0F00

//
// Enumerate registers which differ between client and server
//
typedef enum {
  SmmRegFeatureControl,
  SmmRegSmmDelayed,
  SmmRegSmmBlocked
} SMM_REG_NAME;

extern BOOLEAN          mSmmCodeAccessCheckEnable;
extern BOOLEAN          mSmmUseDelayIndication;
extern BOOLEAN          mSmmUseBlockIndication;
extern CPU_SMM_CLASS    *mThisCpu;

/**
  Return if it is needed to configure MTRR to set TSEG cacheability.

  @retval  TRUE  - we need configure MTRR
  @retval  FALSE - we do not need configure MTRR
**/
BOOLEAN
NeedConfigureMtrrs (
  VOID
  );

/**
  Processor specific hook point at each SMM entry.

  @param  CpuIndex    The index of the cpu which need to check.
**/
VOID
SmmRendezvousEntry (
  IN UINTN  CpuIndex
  );

/**
  Processor specific hook point at each SMM exit.

  @param  CpuIndex    The index of the cpu which need to check.
**/
VOID
SmmRendezvousExit (
  IN UINTN  CpuIndex
  );

/**
  Initialize SMRR context in SMM Init.
**/
VOID
InitializeSmmMtrrManager (
  VOID
  );

/**
  Initialize SMRR/SMBASE/SMM Sync features in SMM Relocate.

  @param  ProcessorNumber    The processor number
  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
  @param  SmBase             The SMBASE value.
  @param  IsBsp              If this processor treated as BSP.
**/
VOID
SmmInitiFeatures (
  IN UINTN   ProcessorNumber,
  IN UINT32  SmrrBase,
  IN UINT32  SmrrSize,
  IN UINT32  SmBase,
  IN BOOLEAN IsBsp
  );

/**
  Configure SMM Code Access Check feature for all processors.
  SMM Feature Control MSR will be locked after configuration.
**/
VOID
ConfigSmmCodeAccessCheck (
  VOID
  );

/**
  Read MSR or CSR based on the CPU type Register to read.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access 
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  
  @return 64-bit value read from register.
    
**/
UINT64
SmmReadReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName
  );

/**
  Write MSR or CSR based on the CPU type Register to write.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access 
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  @param[in]  RegValue  64-bit Register value.
    
**/
VOID
SmmWriteReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName,
  IN  UINT64          RegValue
  );
  
#endif
