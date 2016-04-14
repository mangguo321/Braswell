/** @file
  Definitions of CPU registers

@brief

  Conventions:

  - Prefixes:
    Definitions beginning with "MSR_" are MSRs
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CPU_REGS_H_
#define _CPU_REGS_H_

//
// Local APIC defines
//
#define APIC_REGISTER_LOCAL_ID_OFFSET          0x00000020
#define APIC_REGISTER_APIC_VERSION_OFFSET      0x00000030
#define APIC_REGISTER_SPURIOUS_VECTOR_OFFSET   0x000000F0
#define APIC_REGISTER_ICR_LOW_OFFSET           0x00000300
#define APIC_REGISTER_ICR_HIGH_OFFSET          0x00000310
#define APIC_REGISTER_LINT0_VECTOR_OFFSET      0x00000350
#define APIC_REGISTER_LINT1_VECTOR_OFFSET      0x00000360

#define BROADCAST_MODE_SPECIFY_CPU             0x00
#define BROADCAST_MODE_ALL_INCLUDING_SELF      0x01
#define BROADCAST_MODE_ALL_EXCLUDING_SELF      0x02

#ifndef DELIVERY_MODE_FIXED
#define DELIVERY_MODE_FIXED           0x0
#endif
#ifndef DELIVERY_MODE_LOWEST_PRIORITY
#define DELIVERY_MODE_LOWEST_PRIORITY 0x1
#endif
#ifndef DELIVERY_MODE_SMI
#define DELIVERY_MODE_SMI             0x2
#endif
#ifndef DELIVERY_MODE_REMOTE_READ
#define DELIVERY_MODE_REMOTE_READ     0x3
#endif
#ifndef DELIVERY_MODE_NMI
#define DELIVERY_MODE_NMI             0x4
#endif
#ifndef DELIVERY_MODE_INIT
#define DELIVERY_MODE_INIT            0x5
#endif
#ifndef DELIVERY_MODE_SIPI
#define DELIVERY_MODE_SIPI            0x6
#endif
#ifndef DELIVERY_MODE_MAX
#define DELIVERY_MODE_MAX             0x7
#endif

#define EFI_CACHE_UNCACHEABLE                  0
#define EFI_CACHE_WRITECOMBINING               1
#define EFI_CACHE_WRITETHROUGH                 4
#define EFI_CACHE_WRITEPROTECTED               5
#define EFI_CACHE_WRITEBACK                    6

//
// CPUID defines
//
#define EFI_CPUID_SIGNATURE                    0x0

#define EFI_CPUID_VERSION_INFO                 0x1
#define   B_EFI_CPUID_VERSION_INFO_EAX_MASK                            0x0FFF0FFF
#define   B_EFI_CPUID_VERSION_INFO_EAX_FULL_FAMILY_MODEL_MASK          0x0FFF0FF0
#define   B_EFI_CPUID_VERSION_INFO_EAX_EXT_FAMILY_ID_MASK              0x0FF00000
#define   B_EFI_CPUID_VERSION_INFO_EAX_EXT_MODEL_ID_MASK               0x000F0000
#define   N_EFI_CPUID_VERSION_INFO_EAX_EXT_FAMILY_ID                   20
#define   N_EFI_CPUID_VERSION_INFO_EAX_EXT_MODEL_ID                    16
#define   N_EFI_CPUID_VERSION_INFO_EAX_TYPE                            12
#define   N_EFI_CPUID_VERSION_INFO_EAX_FAMILY_ID                       8
#define   N_EFI_CPUID_VERSION_INFO_EAX_MODEL                           4
#define   N_EFI_CPUID_VERSION_INFO_EAX_STEPPING_ID                     0
#define   B_EFI_CPUID_VERSION_INFO_EBX_DEFAULT_APIC_ID                 (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define   B_EFI_CPUID_VERSION_INFO_EBX_LOGICAL_CPU_PER_PACKAGE         (BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define   B_EFI_CPUID_VERSION_INFO_EBX_CLFLUSH_CHUNK_COUNT             (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define   B_EFI_CPUID_VERSION_INFO_ECX_AES                             BIT25
#define   B_EFI_CPUID_VERSION_INFO_ECX_XAPIC                           BIT21
#define   B_EFI_CPUID_VERSION_INFO_ECX_SSE4_2                          BIT20
#define   B_EFI_CPUID_VERSION_INFO_ECX_SSE4_1                          BIT19
#define   B_EFI_CPUID_VERSION_INFO_ECX_DCA                             BIT18
#define   B_EFI_CPUID_VERSION_INFO_ECX_XTPR_UPDATE                     BIT14
#define   B_EFI_CPUID_VERSION_INFO_ECX_CMPXCHG16B                      BIT13
#define   B_EFI_CPUID_VERSION_INFO_ECX_L1_CONTEXT_ID                   BIT10
#define   B_EFI_CPUID_VERSION_INFO_ECX_SUP_SSE3                        BIT9
#define   B_EFI_CPUID_VERSION_INFO_ECX_TM2                             BIT8
#define   B_EFI_CPUID_VERSION_INFO_ECX_EIST                            BIT7
#define   B_EFI_CPUID_VERSION_INFO_ECX_SME                             BIT6
#define   B_EFI_CPUID_VERSION_INFO_ECX_VME                             BIT5
#define   B_EFI_CPUID_VERSION_INFO_ECX_QPL                             BIT4
#define   B_EFI_CPUID_VERSION_INFO_ECX_MWAIT                           BIT3
#define   B_EFI_CPUID_VERSION_INFO_ECX_SSE3                            BIT0
#define   B_EFI_CPUID_VERSION_INFO_EDX_PBE                             BIT31
#define   B_EFI_CPUID_VERSION_INFO_EDX_THERMAL_CLOCK_CONTROL           BIT29
#define   B_EFI_CPUID_VERSION_INFO_EDX_HT                              BIT28
#define   B_EFI_CPUID_VERSION_INFO_EDX_SELF_SNOOP                      BIT27
#define   B_EFI_CPUID_VERSION_INFO_EDX_SSE2                            BIT26
#define   B_EFI_CPUID_VERSION_INFO_EDX_SSE                             BIT25
#define   B_EFI_CPUID_VERSION_INFO_EDX_FAST_SAVE_RESTORE               BIT24
#define   B_EFI_CPUID_VERSION_INFO_EDX_MMX                             BIT23
#define   B_EFI_CPUID_VERSION_INFO_EDX_ACPI_SUPPORT                    BIT22
#define   B_EFI_CPUID_VERSION_INFO_EDX_DEBUG_TRACE_STORE               BIT21
#define   B_EFI_CPUID_VERSION_INFO_EDX_CLFLUSH_INTR                    BIT19
#define   B_EFI_CPUID_VERSION_INFO_EDX_CPU_SERIAL_NUMBER               BIT18
#define   B_EFI_CPUID_VERSION_INFO_EDX_PSE                             BIT17
#define   B_EFI_CPUID_VERSION_INFO_EDX_PAT                             BIT16
#define   B_EFI_CPUID_VERSION_INFO_EDX_CON_MOVE_INTR                   BIT15
#define   B_EFI_CPUID_VERSION_INFO_EDX_MCA                             BIT14
#define   B_EFI_CPUID_VERSION_INFO_EDX_PGE                             BIT13
#define   B_EFI_CPUID_VERSION_INFO_EDX_MTRR                            BIT12
#define   B_EFI_CPUID_VERSION_INFO_EDX_SEP                             BIT11
#define   B_EFI_CPUID_VERSION_INFO_EDX_ON_CHIP_APIC                    BIT9
#define   B_EFI_CPUID_VERSION_INFO_EDX_CMPXCHG8                        BIT8
#define   B_EFI_CPUID_VERSION_INFO_EDX_MCE                             BIT7
#define   B_EFI_CPUID_VERSION_INFO_EDX_PAE                             BIT6
#define   B_EFI_CPUID_VERSION_INFO_EDX_MSR                             BIT5
#define   B_EFI_CPUID_VERSION_INFO_EDX_TIME_STAMP_COUNTER              BIT4
#define   B_EFI_CPUID_VERSION_INFO_EDX_PAGE_SIZE_EXT                   BIT3
#define   B_EFI_CPUID_VERSION_INFO_EDX_DEBUG_EXT                       BIT2
#define   B_EFI_CPUID_VERSION_INFO_EDX_VME_8086                        BIT1
#define   B_EFI_CPUID_VERSION_INFO_EDX_FP_386                          BIT0

#define EFI_CPUID_CACHE_INFO                   0x2
#define EFI_CPUID_SERIAL_NUMBER                0x3

#define EFI_CPUID_CACHE_PARAMS                 0x4
#define   B_EFI_CPUID_CACHE_PARAMS_EAX_MAX_CORES_IN_PACKAGE            (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26)
#define   B_EFI_CPUID_CACHE_PARAMS_EAX_TOTAL_THREADS_SHARE_CACHE       (BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14)
#define   B_EFI_CPUID_CACHE_PARAMS_EAX_FULLY_ASSOCIATIVE_CACHE         BIT9
#define   B_EFI_CPUID_CACHE_PARAMS_EAX_SELF_INITIALIZING               BIT8
#define   B_EFI_CPUID_CACHE_PARAMS_EAX_CACHE_LEVEL                     (BIT7 | BIT6 | BIT5)
#define   B_EFI_CPUID_CACHE_PARAMS_EAX_CACHE_TYPE                      (BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define   B_EFI_CPUID_CACHE_PARAMS_EBX_WAYS_OF_ASSOCIATIVITY           (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22)
#define   B_EFI_CPUID_CACHE_PARAMS_EBX_PHYSICAL_LINE_PARTITIONS        (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12)
#define   B_EFI_CPUID_CACHE_PARAMS_EBX_SYSTEM_COHERENCY_LINE_SIZE      (BIT11 | BIT10 | BIT9 | BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define   B_EFI_CPUID_CACHE_PARAMS_EDX_CACHE_INCLUSIVE_IN_LOWER_CACHE  BIT1
#define   B_EFI_CPUID_CACHE_PARAMS_EDX_WBINVD_INVD_ON_LOWER_CACHE      BIT0
#define   V_CPUID_CACHE_TYPE_MASK         0x1F
#define   B_CPUID_CACHE_TYPE_DATA         0x1
#define   B_CPUID_CACHE_TYPE_INSTRUCTION  0x2
#define   B_CPUID_CACHE_TYPE_UNIFIED      0x3
#define   V_CPUID_CACHE_LEVEL_MASK        0xE0
#define   B_CPUID_CACHE_LEVEL_SHIFT       5
#define   B_CPUID_CACHE_PARAMS_WAYS_SHIFT                               22
#define   B_CPUID_CACHE_PARAMS_PARTITIONS_SHIFT                         12

#define EFI_CPUID_MONITOR_MWAIT_PARAMS         0x5
#define   B_EFI_CPUID_MONITOR_MWAIT_ECX_INTERRUPTS_BREAK_MWAIT         BIT1
#define   B_EFI_CPUID_MONITOR_MWAIT_ECX_MWAIT_SUPPORT                  BIT0
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C7                      (BIT31 | BIT30 | BIT29 | BIT28)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C7                      28
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C6                      (BIT27 | BIT26 | BIT25 | BIT24)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C6                      24
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C5                      (BIT23 | BIT22 | BIT21 | BIT20)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C5                      20
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C4                      (BIT19 | BIT18 | BIT17 | BIT16)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C4                      16
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C3                      (BIT15 | BIT14 | BIT13 | BIT12)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C3                      12
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C2                      (BIT11 | BIT10 | BIT9 | BIT8)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C2                      8
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C1                      (BIT7 | BIT6 | BIT5 | BIT4)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C1                      4
#define   B_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C0                      (BIT3 | BIT2 | BIT1 | BIT0)
#define   N_EFI_CPUID_MONITOR_MWAIT_EDX_PARAMS_C0                      0

#define EFI_CPUID_POWER_MANAGEMENT_PARAMS      0x6
#define   EFI_CPUID_POWER_MANAGEMENT_EAX_DTS                           BIT0
#define   EFI_CPUID_POWER_MANAGEMENT_EBX_NUM_INT_THRESHOLDS            (BIT3 | BIT2 | BIT1 | BIT0)
#define   EFI_CPUID_POWER_MANAGEMENT_ECX_HW_COORDINATION_FEEDBACK      BIT0

#define EFI_CPUID_REV7                         0x7
#define EFI_CPUID_REV8                         0x8
#define EFI_CPUID_DCA_PARAMS                   0x9
#define EFI_CPUID_ARCH_PERF_MON                0xA
#define EFI_CPUID_CORE_TOPOLOGY                0xB

#define EFI_CPUID_EXTENDED_FUNCTION            0x80000000

#define EFI_CPUID_EXTENDED_FEATURE_BITS        0x80000001
#define EFI_CPUID_EXTENDED_FEATURE_BITS_ECX_LAHF_SAHF                  BIT0
#define EFI_CPUID_EXTENDED_FEATURE_BITS_EDX_XD                         BIT20
#define EFI_CPUID_EXTENDED_FEATURE_BITS_EDX_SYSCALL                    BIT11

//
// This constant defines the maximum length of the CPU brand string. According to the
// IA manual, the brand string is in EAX through EDX (thus 16 bytes) after executing
// the CPUID instructions with EAX as 80000002, 80000003, 80000004.
//
#define MAXIMUM_CPU_BRAND_STRING_LENGTH        48

#define EFI_CPUID_BRAND_STRING1                0x80000002
#define EFI_CPUID_BRAND_STRING2                0x80000003
#define EFI_CPUID_BRAND_STRING3                0x80000004

#define EFI_CPUID_ADVANCED_POWER_MANAGEMENT    0x80000007
#define   EFI_CPUID_ADVANCED_POWER_MANAGEMENT_EDX_TSC_INVARIANCE         BIT8

#define EFI_CPUID_VIRT_PHYS_ADDRESS_SIZE       0x80000008
#define   B_EFI_CPUID_VIRTUAL_ADDRESS_BITS                             (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define   B_EFI_CPUID_PHYSICAL_ADDRESS_BITS                            (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

//
// Common MSR
//
#define EFI_MSR_IA32_PLATFORM_ID               0x00000017
#define   N_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS                  50
#define   B_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS_MASK             (BIT52 | BIT51 | BIT50)
#define   N_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS_MASK_START       50
#define   N_EFI_MSR_IA32_PLATFORM_ID_PLATFORM_ID_BITS_MASK_END         52

#ifndef EFI_MSR_IA32_APIC_BASE
#define EFI_MSR_IA32_APIC_BASE                 0x0000001B

#define   B_EFI_MSR_IA32_APIC_BASE_APIC_BASE_ADDRESS                   0xFFFFFF000  //For Nehalem, base address can be up to 43 bits but not cover here yet
#define   B_EFI_MSR_IA32_APIC_BASE_APIC_GLOBAL_ENABLE                  BIT11
#define   B_EFI_MSR_IA32_APIC_BASE_M_XAPIC                             BIT10
#define   B_EFI_MSR_IA32_APIC_BASE_BSP                                 BIT8
#endif // EFI_MSR_IA32_APIC_BASE
//
// Local APIC defines, offset from APIC base address
//
#define APIC_REGISTER_LOCAL_ID_OFFSET          0x00000020
#define   N_APIC_REGISTER_LOCAL_ID_OFFSET_XAPIC_ID_MASK                24
#define   B_APIC_REGISTER_LOCAL_ID_OFFSET_XAPIC_ID_MASK                0xFF000000

#define APIC_REGISTER_APIC_VERSION_OFFSET      0x00000030
#define   B_APIC_REGISTER_APIC_VERSION_OFFSET_VERSION_MASK             0xFF

#define APIC_REGISTER_SPURIOUS_VECTOR_OFFSET   0x000000F0
#define APIC_REGISTER_ICR_LOW_OFFSET           0x00000300
#define APIC_REGISTER_ICR_HIGH_OFFSET          0x00000310
#define APIC_REGISTER_LINT0_VECTOR_OFFSET      0x00000350
#define APIC_REGISTER_LINT1_VECTOR_OFFSET      0x00000360

#define EFI_MSR_IA32_FEATURE_CONTROL           0x0000003A
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_SGE                           BIT15
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_SLFE                          (BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_SMRR                          BIT3
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_EVT                           BIT2
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_ELT                           BIT1
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_LOCK                          BIT0
#define   B_EFI_MSR_IA32_FEATURE_CONTROL_VT_SECURE                     0x0000FF02

#ifndef EFI_MSR_IA32_BIOS_UPDT_TRIG
#define EFI_MSR_IA32_BIOS_UPDT_TRIG            0x00000079
#endif
#ifndef EFI_MSR_IA32_BIOS_SIGN_ID
#define EFI_MSR_IA32_BIOS_SIGN_ID              0x0000008B
#endif

#define EFI_MSR_PMG_CST_CONFIG                 0x000000E2
#define   B_EFI_MSR_PMG_CST_CONFIG_CST_CONTROL_LOCK                    BIT15
#define   B_EFI_MSR_PMG_CST_CONFIG_IO_MWAIT_REDIRECTION_ENABLE         BIT10
#define   B_EFI_MSR_PMG_CST_CONFIG_PACKAGE_C_STATE_LIMIT               (BIT2 | BIT1 | BIT0)

#define EFI_MSR_PMG_IO_CAPTURE_ADDR            0x000000E4   //For Nehalem Spec: EFI_IA32_PMG_IO_CAPTURE_BASE
#define   N_EFI_MSR_PMG_IO_CAPTURE_ADDR_CST_RANGE                      16
#define   B_EFI_MSR_PMG_IO_CAPTURE_ADDR_LVL_2_BASE_ADDRESS_MASK        0xFFFF

#define EFI_MSR_IA32_MPERF                     0x000000E7
#define EFI_MSR_IA32_APERF                     0x000000E8

#define EFI_MSR_IA32_MTRR_CAP                  0x000000FE
#define   B_EFI_MSR_IA32_MTRR_CAP_EMRR_SUPPORT                         BIT12
#define   B_EFI_MSR_IA32_MTRR_CAP_SMRR_SUPPORT                         BIT11
#define   B_EFI_MSR_IA32_MTRR_CAP_WC_SUPPORT                           BIT10
#define   B_EFI_MSR_IA32_MTRR_CAP_FIXED_SUPPORT                        BIT8
#define   B_EFI_MSR_IA32_MTRR_CAP_VARIABLE_SUPPORT                     (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

#define EFI_MSR_IA32_MCG_CAP                   0x00000179
#define EFI_MSR_IA32_MCG_STATUS                0x0000017A

#define EFI_MSR_IA32_PERF_STS                  0x00000198
#define EFI_MSR_IA32_PERF_CTRL                 0x00000199

#ifndef EFI_MSR_IA32_THERM_INTERRUPT
#define EFI_MSR_IA32_THERM_INTERRUPT           0x0000019B
#endif

#define   B_EFI_MSR_IA32_THERM_INTERRUPT_VIE                           BIT4

#ifndef EFI_MSR_IA32_THERM_STATUS
#define EFI_MSR_IA32_THERM_STATUS              0x0000019C
#endif

#ifndef EFI_MSR_IA32_MISC_ENABLE
#define EFI_MSR_IA32_MISC_ENABLE               0x000001A0
#endif

#define   B_EFI_MSR_IA32_MISC_ENABLE_XD                                BIT34
#define   B_EFI_MSR_IA32_MISC_ENABLE_CPUID_MAX                         BIT22
#define   B_EFI_MSR_IA32_MISC_ENABLE_MONITOR                           BIT18
#define   B_EFI_MSR_IA32_MISC_ENABLE_TM1_EN                            BIT3

#define EFI_MSR_SMRR_PHYS_BASE                 0x000001F2
#define EFI_MSR_SMRR_PHYS_MASK                 0x000001F3

#define EFI_MSR_CACHE_VARIABLE_MTRR_BASE       0x00000200
#define EFI_MSR_CACHE_VARIABLE_MTRR_END        0x0000020F
#define   V_EFI_FIXED_MTRR_NUMBER                                      11

#define EFI_MSR_IA32_MTRR_FIX64K_00000         0x00000250
#define EFI_MSR_IA32_MTRR_FIX16K_80000         0x00000258
#define EFI_MSR_IA32_MTRR_FIX16K_A0000         0x00000259
#define EFI_MSR_IA32_MTRR_FIX4K_C0000          0x00000268
#define EFI_MSR_IA32_MTRR_FIX4K_C8000          0x00000269
#define EFI_MSR_IA32_MTRR_FIX4K_D0000          0x0000026A
#define EFI_MSR_IA32_MTRR_FIX4K_D8000          0x0000026B
#define EFI_MSR_IA32_MTRR_FIX4K_E0000          0x0000026C
#define EFI_MSR_IA32_MTRR_FIX4K_E8000          0x0000026D
#define EFI_MSR_IA32_MTRR_FIX4K_F0000          0x0000026E
#define EFI_MSR_IA32_MTRR_FIX4K_F8000          0x0000026F
#define EFI_MSR_CACHE_IA32_MTRR_DEF_TYPE       0x000002FF
#define   B_EFI_MSR_CACHE_MTRR_VALID                                   BIT11
#define   B_EFI_MSR_GLOBAL_MTRR_ENABLE                                 BIT11
#define   B_EFI_MSR_FIXED_MTRR_ENABLE                                  BIT10
#define   B_EFI_MSR_CACHE_MEMORY_TYPE                                  (BIT2 | BIT1 | BIT0)

#define EFI_MSR_VALID_MASK                     0xFFFFFFFFF
#define EFI_CACHE_VALID_ADDRESS                0xFFFFFF000
#define EFI_SMRR_CACHE_VALID_ADDRESS           0xFFFFF000
#define EFI_CACHE_VALID_EXTENDED_ADDRESS       0xFFFFFFFFFF000

// Leave one MTRR pairs for OS use
#define EFI_CACHE_NUM_VAR_MTRR_PAIRS_FOR_OS   1
#define EFI_CACHE_LAST_VARIABLE_MTRR_FOR_BIOS (EFI_MSR_CACHE_VARIABLE_MTRR_END) - \
        (EFI_CACHE_NUM_VAR_MTRR_PAIRS_FOR_OS * 2)

#define EFI_MSR_IA32_MC0_CTL                   0x00000400
#define EFI_MSR_IA32_MC0_STATUS                0x00000401
#define EFI_MSR_IA32_MC0_ADDR                  0x00000402
#define EFI_MSR_IA32_MC0_MISC                  0x00000403
#define EFI_MSR_IA32_MC8_CTL                   (EFI_IA32_MC0_CTL + (8*4))
#define EFI_MSR_IA32_MC8_STATUS                (EFI_IA32_MC0_STATUS + (8*4))

#define EFI_MSR_APIC_GLOBAL_ENABLE             0x00000800
#define EFI_MSR_EXT_XAPIC_LOGICAL_APIC_ID      0x00000802
#define EFI_MSR_EXT_XAPIC_VERSION              0x00000803
#define   B_EFI_MSR_EXT_XAPIC_VERSION_VERSION                          0xFF
#define EFI_MSR_EXT_XAPIC_SVR                  0x0000080F
#define EFI_MSR_EXT_XAPIC_ICR                  0x00000830
#define EFI_MSR_EXT_XAPIC_LVT_LINT0            0x00000835
#define EFI_MSR_EXT_XAPIC_LVT_LINT1            0x00000836

#define EFI_MSR_IA32_EFER                      0xC0000080
#define   B_EFI_MSR_IA32_EFER_NXE                                      BIT11
#define   B_EFI_MSR_IA32_EFER_LMA                                      BIT10
#define   B_EFI_MSR_IA32_EFER_LME                                      BIT8
#define   B_EFI_MSR_IA32_EFER_SCE                                      BIT0

#pragma pack(1)

typedef enum {
  EnumCpuUarchUnknown = 0,
  EnumNehalemUarch,
} EFI_CPU_UARCH;

typedef enum {
  EnumCpuPlatformUnknown = 0,
  EnumDesktop,
  EnumMobile,
  EnumServer,
  EnumNetTop
} EFI_CPU_PLATFORM;

typedef enum {
  EnumCpuTypeUnknown = 0,
  EnumAtom,
  EnumNehalemEx,
  EnumBloomfield,
  EnumGainestown,
  EnumHavendale,
  EnumLynnfield,
  EnumAuburndale,
  EnumClarksfield,
  EnumPineview,
  EnumCedarview,
  EnumValleyview,
  EnumClarkdale // Havendale 32nm
} EFI_CPU_TYPE;

typedef enum {
  EnumCpuFamilyUnknown = 0,
  EnumFamilyField,
  EnumFamilyDale
} EFI_CPU_FAMILY;

#pragma pack()

#endif
