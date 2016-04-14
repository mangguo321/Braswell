/** @file
  IST/PPM reference code definitions.

  Acronyms:
    PPM   Platform Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PPM_COMMON_H_
#define _PPM_COMMON_H_

#include <PiDxe.h>

//
// Forward Declarations
//
typedef union _FVID_TABLE FVID_TABLE;

//
// Limit the number of P-states to 16. Up to Windows 7, the OS allocates 1KB buffer for the PSS package.
// So the maximum number of P-state OS can handle is 19. This is not an OS issue. Having too many P-states 
// is not good for the system performance.
//
#define FVID_MAX_STATES     32 //  ACPI Max P-States = 16

#define FVID_MIN_STEP_SIZE  1

//
// Structure declarations
//

typedef struct _FVID_HEADER {
  UINT32  Stepping;     // Matches value returned by CPUID function 1
  UINT16  MaxVid;       // Matches VID_MAX field in PERF_STS_MSR
  UINT16  MaxBusRatio;  // Matches BUS_RATIO_MAX field in PERF_STS_MSR
  UINT16  Gv3States;    // Number of states of FVID (N)
} FVID_HEADER;

typedef struct _FVID_STATE {
  UINT32  State;        // State Number (0 - N-1)
  UINT16  Vid;          // VID_SEL value to be written to PERF_CTL
  UINT16  BusRatio;     // BUS_RATIO_SEL value to be written to PERF_CTL
  UINT16  Power;        // Typical power consumed by CPU in this state
} FVID_STATE;

typedef union _FVID_TABLE {
  FVID_HEADER FvidHeader;
  FVID_STATE  FvidState;
  UINT64      FvidData;
} FVID_TABLE;

//
// Processor Power Management Flags Bit Definitions:
//
#define PPM_GV3                           (1 << 0)  // (EIST) - Enhanced Intel SpeedStep Technology.a.k.a Geyserville 3.
#define PPM_C1                            (1 << 1)  // C1 enabled, supported.
#define PPM_C1E                           (1 << 2)  // C1E enabled.
#define PPM_C2                            (1 << 3)  // C2 enabled, supported.
#define PPM_C2E                           (1 << 4)  // C2E enabled.
#define PPM_C3                            (1 << 5)  // C3 enabled, supported.
#define PPM_C3E                           (1 << 6)  // C3E enabled.
#define PPM_C4                            (1 << 7)  // C4 enabled, supported.
#define PPM_C4E                           (1 << 8)  // C4E enabled, supported.
#define PPM_DEEP_C4                       (1 << 9)  // Deep C4 enabled, supported.
#define PPM_HARD_C4E                      (1 << 10) // Hard C4E enabled.
#define PPM_C6                            (1 << 11) // C6 enabled, supported.
#define PPM_C6S                           (1 << 12) // C6S enabled, supported.
#define PPM_C7                            (1 << 13) // C6S enabled, supported.

#define PPM_TM                            (1 << 16) // Thermal Monitor 1 & Thermal Monitor 2.
#define PPM_TM1                           (1 << 16) // Thermal Monitor 1.
#define PPM_TM2                           (1 << 17) // Thermal Monitor 2.

#define PPM_PROC_HOT                      (1 << 19) // Bi-directional ProcHot.
#define PPM_TSC                           (1 << 20) // TSC 64-bit writeable.
#define PPM_MWAIT_EXT                     (1 << 21) // MONITIOR/MWAIT Extensions supported.
#define PPM_QUAD                          (1 << 23) // Quad-core processor.

#define PPM_CMP                           (1 << 24) // CMP.
#define PPM_TURBO                         (1 << 25) // Turbo Mode
#define PPM_EMTTM                         (1 << 26) // Enhanced Multi Threaded Thermal Monitoring
#define PPM_DYNAMIC_FSB                   (1 << 27) // Dynamic FSB Switching

#define PPM_TSTATES                       (1 << 28) // CPU throttling states
#define PPM_BOOT_P_ST_HFM                 (1 << 29) // Set boot P-state to HFM when EIST is enabled
#define PPM_S0ix                          (1 << 30) // S0ix is enabled

// @NOTE: Not available in VLV
#define PPM_FAST_BRK_SNOOP                0x200000 // BTI 21: Fast break Snoop Enable
#define PPM_FAST_BRK_INTERRUPT            0x400000 // BIT 22: Fast break Interrupt Enable

#define PPM_C_STATES                      (PPM_C1 + PPM_C4 + PPM_C6 + PPM_C6S + PPM_C7)
#define PPM_ENHANCED_C_STATES             (PPM_C1E)

#define CST_UPDT_TSC                      (1 << 1)
#define CST_DATA_TBASE                    3579545

#define PPM_ENABLE                        1
#define PPM_DISABLE                       0

#endif
