/** @file
  This library contains power management configuration functions specific to 
  Cherryview processors.

  Acronyms:
    PPM   Processor Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology
    HT    Hyper-Threading Technology
    CMP   Core Multi-Processing

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SILVERMONT_PPM_DEFINES_H_
#define _SILVERMONT_PPM_DEFINES_H_

//
// Definititons for CPU stepping
//
#define CPU_A0_STEPPING  0

#define EFI_CPUID_XAPIC_PROC_TOPOLOGY  0xB

//
// Definitions for CPUID
//
#define CPUID_STEPPING    0x0000000F

#define CPUID_FUNCTION_4  0x00000004

#define CPUID_FUNCTION_6  0x00000006

//
// Extended Feature Flag definitions (CPUID EAX=1 ECX)
//
#define CPUXFF_MONITOR_MWAIT  (1 << 3)
#define CPUXFF_GV3            (1 << 7)

//
// Extended Feature Flag definitions (CPUID EAX=1 EDX)
//
#define CPUXFF_TM1            (1 << 29)
#define CPUXFF_TM2            (1 << 8)
//
// CPUID 5 definitions (ECX)
//
#define MONITOR_MWAIT_EXTENSIONS  (1 << 0)
//
// CPUID 5 definitions (EDX)
//
#define CSTATE_SUPPORTED          0x1
#define ENHANCED_CSTATE_SUPPORTED 0x2

//
// CPUID 6 definitions (EAX)
//
#define TURBO_MODE_SUPPORTED             (1 << 1)
#define ENERGY_EFFICIENT_P_STATE_CAPABLE (1 << 3)   // @NOTE: Not available in VLV
#define FINE_GRAINED_CLOCK_MODULATION    (1 << 5)   // @NOTE: Not available in VLV

#define CPUID_PROCESSOR_TOPOLOGY         0xB
//
// CPUID B definitions (EBX)
//
#define FACTORY_CONFIGURED_LOGICAL_PROCESSORS   0xFFFF

//
// MSR definitions
//
#define MSR_IA32_PLATFORM_ID                    0x17
#define PLATFORM_ID_BITS_MASK                 (0x7 << 18) // Bits 52:50
#define VID_MAX_PREFLX_BYTE_MASK              0x7F  // Bits 6:0
#define RATIO_MAX_PFLX_BYTE_MASK              0x3F  // Bits 13:8
#define RATIO_LOCKED                          (1 << 15)
#define FB_GV3_DISABLED                          (1 << 17)
#define FB_THERMAL_THROT_DISABLED                (1 << 19)
#define EFI_MSR_EBC_HARD_POWERON                0x2A
#define STOP_ENABLE                           (1 << 19)
#define MSR_FSB_CLOCK_VCC                       0xCE  
#define BUS_GVL_RATIO_BYTE_MASK               0x1F                  // Bits 12:8
#define MAX_RATIO_FUSE_MASK                   0x1F                  // Bits 44:40
#define VID_MIN_FUSE_MASK                     0x7F					// Bits 54:48	 
#define VID_MAX_FUSE_MASK                     0x7F					// Bits 62:56	
#define MSR_PLATFORM_INFO                       0xCE
#define MSR_IACORE_RATIOS                       0x66a
#define MSR_IACORE_VIDS                         0x66b
#define MSR_IACORE_TURBO_RATIOS                 0x66c
#define MAX_RATIO_4C_OFFSET	                  24
#define MAX_RATIO_3C_OFFSET	                  16
#define MAX_RATIO_2C_OFFSET	                  8
#define MAX_RATIO_1C_OFFSET	                  0
#define MAX_RATIO_1C_MASK                      0x000000ff
#define MAX_RATIO_2C_MASK                      0x0000ff00
#define MAX_RATIO_3C_MASK                      0x00ff0000
#define MAX_RATIO_4C_MASK                      0xff000000
#define MSR_IACORE_TURBO_VIDS                   0x66d
#define MAX_VID_4C_OFFSET                     24
#define MAX_VID_3C_OFFSET                     16
#define MAX_VID_2C_OFFSET                     8
#define MAX_VID_1C_OFFSET                     0
#define MAX_VID_1C_MASK                           0x000000ff
#define MAX_VID_2C_MASK                           0x0000ff00
#define MAX_VID_3C_MASK                           0x00ff0000
#define MAX_VID_4C_MASK                           0xff000000

#define MSR_PM_CFG_CTRL                         0xE2
#define C0_SUB_STATES_MASK                    0x0000000f
#define C1_SUB_STATES_MASK                    0x000000f0
#define C2_SUB_STATES_MASK                    0x00000f00
#define C3_SUB_STATES_MASK                    0x0000f000
#define C4_SUB_STATES_MASK                    0x000f0000
#define C5_SUB_STATES_MASK                    0x00f00000
#define C6_SUB_STATES_MASK                    0x0f000000
#define C7_SUB_STATES_MASK                    0xf0000000    
#define CSTATE_LIMIT_MASK                     0x7
#define CSTATE_LIMIT_NO_LIMIT                 0x0               
#define CSTATE_LIMIT_C1                       0x1
#define CSTATE_LIMIT_C2                       0x2
#define CSTATE_LIMIT_C3                       0x3
#define CSTATE_LIMIT_C4                       0x4
#define CSTATE_LIMIT_C6                       0x6
#define CSTATE_LIMIT_C7                       0x7

#define DYNAMIC_L2_ENABLE                     (1 << 3)
#define CSM_SMI_TRIG_MASK                     (7 << 4) 
#define CSM_SMI_TRIG_AUTO                     (7 << 4)
#define MWAIT_IO_REDIR                        (1 << 10)
#define HW_COORD_DIS                          (1 << 11)
#define SINGLE_PCTL                           (1 << 11)
#define LEVEL_5_READ_DISABLE                  (1 << 13)
#define CFG_LOCK                              (1 << 15)
#define C3_AUTO_DEMOTE_ENABLE                 (1 << 25)
#define C1_AUTO_DEMOTE_ENABLE                 (1 << 26)
#define C3_UNDEMOTION_ENABLE                  (1 << 27)
#define C1_UNDEMOTION_ENABLE                  (1 << 28)
#define L2_SHRINK_THRESHOLD_MASK              (0x3F << 16)
#define L2_SHRINK_THRESHOLD_DISABLE           (1 << 16)
#define L2_SHRINK_THRESHOLD_DEFAULT           (6 << 16)
#define L2_SHRINK_THRESHOLD_OFFSET            (16)

#define MSR_PMG_IO_CAPTURE_BASE                 0xE4
#define MSR_IO_CAPT_ADDR                        MSR_PMG_IO_CAPTURE_BASE
#define IO_CAPT_RANGE_MASK                    (0x7 << 16)
#define IO_CAPT_LVL2                          (0x0 << 16) // C4
#define IO_CAPT_LVL3                          (0x1 << 16) // C6
#define IO_CAPT_LVL4                          (0x2 << 16) // C6 or C7?
#define IO_CAPT_LVL5                          (0x3 << 16)
#define IO_CAPT_LVL6                          (0x4 << 16) 
#define IO_CAPT_LVL2_BASE_ADDR_MASK           (0xFFFF)
#define MSR_IO_CAPT_ADDR_MASK                 (0x0000FFFF)
#define MSR_IO_CAPT_RANGE_MASK                (0x007F0000)  // I/O capture range mask.

#define MSR_BBL_CR_CTL3                         0x11E
#define C6_RESIDENCY_ENABLE                   (1 << 19)
#define L2_WAY_CHUNK_SIZE_MASK                (0x7 << 25)
#define L2_WAY_CHUNK_SIZE_32                  (0x2 << 25)
#define L2_REDUCTION_MASK                     (0x3 << 30)
#define L2_REDUCTION_0_AUTO                   (1 << 30)
#define L2_WAY_RED_MIN_MASK	                  (3 << 30)
#define L2_WAY_RED_MIN_0_STAY                 (0 << 30)	//Reduced cache min. L2 ways = 0, stay.
#define L2_WAY_RED_MIN_0_AUTO                 (1 << 30)	//Reduced cache min. L2 ways = 0, autoexpand.
#define L2_WAY_RED_MIN_2                      (2 << 30)	//Reduced cache min. L2 ways = 2.
#define L2_WAY_RED_MIN_4                      (3 << 30)	//Reduced cache min. L2 ways = 4.
#define L2_WAY_CHUNK_SZ_MASK                  (7 << 25)	//L2 way chunk size.
#define L2_WAY_CHUNK_SZ_0                     (0 << 25)	//L2 chunk size = 0.
#define L2_WAY_CHUNK_SZ_2                     (1 << 25)	//L2 chunk size = 1/2.
#define L2_WAY_CHUNK_SZ_4                     (2 << 25)	//L2 chunk size = 1/4.
#define L2_WAY_CHUNK_SZ_8                     (3 << 25)	//L2 chunk size = 1/8.
#define L2_WAY_CHUNK_SZ_16                    (4 << 25)	//L2 chunk size = 1/16.
#define L2_WAY_CHUNK_SZ_32                    (5 << 25)	//L2 chunk size = 1/32.
#define L2_WAY_CHUNK_SZ_64                    (6 << 25)	//L2 chunk size = 1/64.
#define L2_WAY_CHUNK_SZ_128                   (7 << 25)	//L2 chunk size = 1/128.

#define MSR_POWER_MISC                          0x120
#define ENABLE_ULFM_AUTOCM_MASK               (1 << 2)
#define ENABLE_INDP_AUTOCM_MASK               (1 << 3)
#define ENABLE_IA_UNTRUSTED_MODE              (1 << 6)
#define MAX_POWER_LIMIT_1_TIME_IN_SECONDS       32767
#define AUTO                                    0
#define END_OF_TABLE                            0xFF

#define MSR_FLEX_RATIO                          0x194
#define VID_FLEX_BYTE_MASK                    0xFF                  // Bits 6:0  (7 --> Reserved)
#define RATIO_FLEX_BYTE_MASK                  0xFF                  // Bits 13:8 (15:14 --> Reserved)
#define RATIO_FLEX_N2                         (1 << 14)

#define FLEX_RATIO                            (0xFF << 8)
#define ENABLE_FLEX                           (1 << 16)
#define MAX_EXTRA_VOLTAGE                     0xFF
#define OVERCLOCKING_BINS                     (0x7 << 17)

#define MSR_IA32_PERF_STS                       0x198
#define BUS_RATIO_MIN_MASK			              0x1F			//Bits 28:24
#define MSR_IA32_PERF_CTL                     0x199
#define P_STATE_TARGET_OFFSET                 8
#define P_STATE_TARGET_MASK                   (0x3F7F)
#define RATIO_BYTE_MASK                       0x1F     
#define MSR_IA32_CLOCK_MODULATION               0x19A
#define MSR_IA32_THERM_INTERRUPT                0x19B
#define MSR_GV_THERM                            0x19D
#define VID_THROT_BYTE_MASK                   0x7F        // Bits 6:0
#define RATIO_THROT_BYTE_MASK                 0x3F        // Bits 13:8
#define MSR_IA32_MISC_ENABLES                   0x1A0
#define ACNT2_ENABLE_MASK                     (1 << 27)
#define MSR_TEMPERATURE_TARGET                  0x1A2
#define MSR_TURBO_RATIO_LIMIT                   0x1AD
#define MSR_IA32_ENERGY_PERFORMANCE_BIAS        0x1B0
#define ENERGY_POLICY_OFFSET                  0
#define ENERGY_POLICY_MASK                    0xF
#define MSR_POWER_CTL                           0x1FC
#define FAST_BRK_INTERRUPT_ENABLE             (1 << 4)
#define FAST_BRK_SNP_ENABLE                   (1 << 3)
#define MSR_PACKAGE_POWER_SKU_UNIT              0x606
#define MSR_PKGC3_IRTL                          0x60A
#define PKG_IRTL_VALID                        (1 << 15)
#define INTERRUPT_RESPONSE_TIME_LIMIT_MASK    0x1FF        // Bits 9:0
#define TIME_UNIT_MASK                        (0x7 << 10)  // Bits 12:10
#define TIME_UNIT_OFFSET                      10
#define TIME_UNIT_1024_NS                     0x2
#define TIME_UNIT_32768_NS                    0x3

//-----------------------------------------------------------------------------
// Thermal Management Registers
//-----------------------------------------------------------------------------
#define MSR_IA32_CR_THERM_INTERRUPT     0x19b
#define MSR_IA32_CR_THERM_STATUS        0x19c
#define MSR_CPU_THERM_TEMPERATURE       0x1a2
#define EFI_MSR_TURBO_POWER_LIMIT       0x610
#define MSR_PKG_TURBO_CFG1              0x670
#define TURBOMODE                           (0x0 << TURBOMODE_OFFSET)
#define TURBOMODE_MASK                      (0x7 << TURBOMODE_OFFSET)
#define TURBOMODE_OFFSET                    0
#define ICCMAX_CTRL                         (0x2 << ICCMAX_CTRL_OFFSET)
#define ICCMAX_CTRL_MASK                    (0x3 << ICCMAX_CTRL_OFFSET)
#define ICCMAX_CTRL_OFFSET                  3
#define ICCMAX_CTRL_EN                      (0x1 << ICCMAX_CTRL_EN_OFFSET)
#define ICCMAX_CTRL_EN_OFFSET               5
#define SOC_TDP_EN                          (0x1 << SOC_TDP_EN_OFFSET)
#define SOC_TDP_EN_OFFSET                   8
#define SOC_TDP_POLICY                      (0x3 << SOC_TDP_POLICY_OFFSET)
#define SOC_TDP_POLICY_MASK                 (0x7 << SOC_TDP_POLICY_OFFSET)
#define SOC_TDP_POLICY_OFFSET               9
#define MIN_ENERGY                          (0x1 << MIN_ENERGY_OFFSET)
#define MIN_ENERGY_MASK                     (0x1FFF << MIN_ENERGY_OFFSET)
#define MIN_ENERGY_OFFSET                   16
#define MSR_CPU_TURBO_WKLD_CFG1         0x671
#define C0STL_AFACTOR_THRESH                (0x0 << C0STL_AFACTOR_THRESH_OFFSET)
#define C0STL_AFACTOR_THRESH_MASK           (0x7 << C0STL_AFACTOR_THRESH_OFFSET)
#define C0STL_AFACTOR_THRESH_OFFSET         3
#define IPC_THRESH0                         (0x2 << IPC_THRESH0_OFFSET)
#define IPC_THRESH0_MASK                    (0xF << IPC_THRESH0_OFFSET)
#define IPC_THRESH0_OFFSET                  8
#define IPC_THRESH1                         (0x3 << IPC_THRESH1_OFFSET)
#define IPC_THRESH1_MASK                    (0xF << IPC_THRESH1_OFFSET)
#define IPC_THRESH1_OFFSET                  12 
#define IPC_THRESH2                         (0x6 << IPC_THRESH2_OFFSET)
#define IPC_THRESH2_MASK                    (0xF << IPC_THRESH2_OFFSET)
#define IPC_THRESH2_OFFSET                  16 
#define IPC_THRESH3                         (0xB << IPC_THRESH3_OFFSET)
#define IPC_THRESH3_MASK                    (0xF << IPC_THRESH3_OFFSET)
#define IPC_THRESH3_OFFSET                  20 
#define IPC_THRESH4                         (0xD << IPC_THRESH4_OFFSET)
#define IPC_THRESH4_MASK                    (0xF << IPC_THRESH4_OFFSET)
#define IPC_THRESH4_OFFSET                  24 
#define MSR_CPU_TURBO_WKLD_CFG2         0x672
#define AFACTOR0                            (0xB << AFACTOR0_OFFSET)
#define AFACTOR0_MASK                       (0xF << AFACTOR0_OFFSET)
#define AFACTOR0_OFFSET                     0
#define AFACTOR1                            (0x9 << AFACTOR1_OFFSET)
#define AFACTOR1_MASK                       (0xF << AFACTOR1_OFFSET)
#define AFACTOR1_OFFSET                     4
#define AFACTOR2                            (0x7 << AFACTOR2_OFFSET)
#define AFACTOR2_MASK                       (0xF << AFACTOR2_OFFSET)
#define AFACTOR2_OFFSET                     8
#define AFACTOR3                            (0x5 << AFACTOR3_OFFSET)
#define AFACTOR3_MASK                       (0xF << AFACTOR3_OFFSET)
#define AFACTOR3_OFFSET                     12
#define AFACTOR4                            (0x3 << AFACTOR4_OFFSET)
#define AFACTOR4_MASK                       (0xF << AFACTOR4_OFFSET)
#define AFACTOR4_OFFSET                     16

#define MSR_CPU_THERM_CFG1              0x673
#define MSR_CPU_THERM_CFG2              0x674
#define MSR_CPU_THERM_SENS_CFG          0x675

#define MSR_PKGC6_IRTL                          0x60B
#define MSR_PKGC7_IRTL                          0x60C
#define MSR_TURBO_POWER_LIMIT                   0x610
#define MSR_PACKAGE_POWER_SKU                   0x614
//#define MSR_PIC_SENS_CFG                        MSR_CPU_THERM_SENS_CFG
//#define TM1_AND_TM2_ENABLE                    (1 << 20)
//#define TM1_LOCK                              (1 << 21)
//#define IMVP_OPTIMIZATION_DIS                 (1 << 31)
#define MSR_EBC_HARD_POWERON                    0x2A
#define STOP_ENABLE                           (1 << 19)
#define MSR_PRIMARY_PLANE_CURRENT_CONFIG        0x601
#define CURRENT_LIMIT_MASK                    0x1FFF      // Bits 12:0

#define MSR_SECONDARY_PLANE_CURRENT_CONFIG      0x602

#define MSR_PRIMARY_PLANE_TURBO_POWER_LIMIT       0x638
#define PP_PWR_LIM_TIME                         (0 << PP_PWR_LIM_TIME_OFFSET)
#define PP_PWR_LIM_TIME_MASK                    (0x7F << PP_PWR_LIM_TIME_OFFSET)
#define PP_PWR_LIM_TIME_OFFSET                  17

#define MSR_SECONDARY_PLANE_TURBO_POWER_LIMIT     0x640
#define PLANE_POWER_LIMIT_MASK                (0x7FFF)
#define PLANE_POWER_LIMIT_ENABLE              (1 << 15)
#define PLANE_POWER_LIMIT_TIME_MASK           (0x7F)
#define PLANE_POWER_LIMIT_TIME_OFFSET         17
#define PLANE_POWER_LIMIT_LOCK                (1 << 31)

#define IA32_PERF_STS_VIP     (1 << 17)
#define PLATFORMID_LOCKED     (1 << 15)
#define VID_MASK                (0x3F << 0)
#define MIN_SLT_VID             (0x1D << 0)
#define BUS_RATIO_MASK          (0x1F << 8)
#define CLK_FLEX_MAX_MASK       (0x101F << 0)
#define CLK_FLEX_EN             (1 << 16)

#define ENERGY_EFFICIENT_P_STATE_FEATURE_ENABLE (1 << 18)

#define STOP_ENABLE                             (1 << 19)
#define CORE_COUNT_OFFSET                       (16)
#define CORE_COUNT_MASK                         (0xFFFF << 16)
#define THREAD_COUNT_MASK                       (0xFFFF)

#define PACKAGE_TDP_POWER_MASK                  (0x7FFF)      // Bits 14:0
#define PACKAGE_MIN_POWER_MASK                  (0x7FFF)      // Bits 14:0
#define PACKAGE_MAX_POWER_MASK                  (0x7FFF)      // Bits 46:32
#define PACKAGE_MAX_TIME                        (0x7F0000)    // Bits 54:48
#define PROG_TDP_LIMITS                         (1 << 29)
#define PROG_RATIO_LIMITS                       (1 << 28)
#define PROG_TCC_ACTIVATION_OFFSET              (1 << 30)

//
// Macro deinfition for Turbo related settings.
//
#define POWER_LIMIT_ENABLE          (1 << 15)   // Used as Bit 15 and Bit 47
#define POWER_LIMIT_MASK            (0x7FFF)    // Bits 14:0 and 46:32
#define POWER_LIMIT_1_TIME_MASK     (0xFE0000)  // Bits 23:17
#define POWER_LIMIT_LOCK            (1 << 31)   // Bit 63
#define CRITICAL_POWER_CLAMP_ENABLE (1 << 16)   // Bit 16 and Bit 48

#define TURBO_MODE_DISABLED         (1 << 11)
#define DISABLE_TURBO_MODE          (1 << (32 - 32))
#define TM_ENABLE                      (1 << 3)
#define GV3_ENABLE                  (1 << GV3_ENABLE_OFFSET)
#define GV3_ENABLE_OFFSET           16
#define FORCEPR_ENABLE_DC               (1 << 21)  //Only for MP
#define BIDIR_PHOT_ENABLE_SC            (1 << 17)
#define MONITOR_MWAIT_ENABLE            (1 << 18)
#define EIST_SELECT_LOCK                (1 << 20)
#define LIMIT_CPUID                     (1 << 22)
#define C1E_ENABLE                      (1 << 1)
#define C1E_ENABLE_MASK                 (1 << 1)
#define C2E_ENABLE                      (1 << 26)
#define C3E_ENABLE                      (1 << 31)
#define C4E_ENABLE                      (1 << (32-32))  //C4E enable bit.
#define HARD_C4E_ENABLE                 (1 << (33-32))  //Hard C4E enable bit.

#define MONITOR_MWAIT_ENABLE        (1 << 18)
#define LIMIT_CPUID                 (1 << 22)
#define DISABLE_MASTER_TURBO_MODE   (1 << (38 - 32))
#define LOCK_TM_INT                 (1 << 22)
#define MAX_RATIO_LIMIT_4C_OFFSET   24
#define MAX_RATIO_LIMIT_4C_MASK     (0x3F << MAX_RATIO_LIMIT_4C_OFFSET)
#define MAX_RATIO_LIMIT_3C_OFFSET   16
#define MAX_RATIO_LIMIT_3C_MASK     (0x3F << MAX_RATIO_LIMIT_3C_OFFSET)
#define MAX_RATIO_LIMIT_2C_OFFSET   8
#define MAX_RATIO_LIMIT_2C_MASK     (0x3F << MAX_RATIO_LIMIT_2C_OFFSET)
#define MAX_RATIO_LIMIT_1C_MASK     (0x3F)
#define MAX_TURBO_RATIO_LIMIT_MASK  (0x3F)

#define PHOT_ENABLE                 (1 << 0)
#define PACKAGE_POWER_UNIT_MASK     0xF
#define TCC_ACTIVATION_OFFSET_MASK  (0xF << 24)
#define PACKAGE_TIME_UNIT_MASK      0xF0000

#define CPU_CACHE_TYPE_MASK             0x1F
#define CPU_CACHE_LEVEL_MASK            0x07
#define CPU_CACHE_ASSOCIATIVITY_MASK    0x03FF
#define CPU_CACHE_PARTITION_MASK        0x03FF
#define CPU_CACHE_LINE_SIZE_MASK        0x0FFF

#define MAX_OVERCLOCKING_BINS        0x7

#define PROCESSOR_FLAVOR_MOBILE      0x0104
#define PROCESSOR_FLAVOR_DESKTOP     0x0100

//
// Power definitions (Based on EMTS V1.0 for standard voltage 2.4-2.6 GHz dual-core parts.)
//
#define FVID_MAX_POWER_SANDYBRIDGE      35000
#define FVID_TURBO_POWER_SANDYBRIDGE    35000
#define FVID_SUPERLFM_POWER_SANDYBRIDGE 12000
//
// Power definitions for LFM and Turbo mode TBD.
//
#define FVID_MIN_POWER_SANDYBRIDGE  15000

//
// Primary and Secondary Plane Current Limit Defaults
//
#define PRIMARY_PLANE_CURRENT_DEFAULT      0x380
#define SECONDARY_PLANE_CURRENT_DEFAULT    0x118

#define FVID_MAX_POWER_SANDYBRIDGE      35000
#define FVID_TURBO_POWER_SANDYBRIDGE    35000

#ifndef SW_SMI_S3_RESTORE_MSR
#define SW_SMI_S3_RESTORE_MSR       0x48
#endif

#ifndef SW_SMI_PPM_INTERNAL
#define SW_SMI_PPM_INTERNAL         0x47
#endif

#define PPM_ENABLE                  1
#define PPM_DISABLE                 0

#define EFI_MSR_IA32_PERF_CTL         MSR_IA32_PERF_CTL
#define EFI_MSR_IA32_CLOCK_MODULATION MSR_IA32_CLOCK_MODULATION
#define EFI_MSR_IA32_THERM_STATUS     0x19C
#define EFI_MSR_GV_THERM              0x19D

#define FVID_MAX_POWER_MIDVIEW 2000
#define FVID_MIN_POWER_MIDVIEW 600

#define FAMILYMODELSTEP_SLM  0x30670
#endif
