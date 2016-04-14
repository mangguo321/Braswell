/** @file
  CPU policy PPI produced by a platform driver specifying various
  expected CPU settings. This PPI is consumed by CPU PEI modules.

  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef CPU_PLATFORM_POLICY_PPI_H_
#define CPU_PLATFORM_POLICY_PPI_H_

extern EFI_GUID gPeiCpuPlatformPolicyPpiGuid;

typedef struct _PEI_CPU_PLATFORM_POLICY_PPI PEI_CPU_PLATFORM_POLICY_PPI;

#define CPU_FEATURE_ENABLE  1
#define CPU_FEATURE_DISABLE 0

//
// PEI_CPU_PLATFORM_POLICY_PPI revisions
//
#define PEI_CPU_PLATFORM_POLICY_PPI_REVISION_1  1
#define PEI_CPU_PLATFORM_POLICY_PPI_REVISION_2  2

///
/// Platform Policies for CPU features configration Platform code can enable/disable/configure features through this structure.
///
typedef struct {
  //
  // Byte 0, bit definition Generic CPU feature configuration
  //
  UINT8     ActiveCoreCount       : 1; ///< Enable or Disable BIST on Reset; <b>0: Disable</b>; 1 Enable.
  UINT8     Rsvdbyte              : 7; ///< Place holder for additonal functionalites expected in future platforms.
} CPU_CONFIG_PPI;

typedef struct {
  /**
  Boot performance mode.
   - <b>0: Maximum non-turbo performance</b>
   - 1:Maximum battery performance.
  **/
  UINT8  BootInLfm;
} POWER_MGMT_CONFIG_PPI;

/**
  The PPI allows the platform code to publish a set of configuration information that the
  CPU drivers will use to configure the processor in the PEI phase.
  This Platform Policy PPI needs to be initialized for CPU configuration.
  @note The PPI has to be published before processor PEIMs are dispatched.
**/
struct _PEI_CPU_PLATFORM_POLICY_PPI {
  /**
  This member specifies the revision of the CPU Policy PPI. This field is used to indicate backward
  compatible changes to the PPI. Any such changes to this PPI will result in an update in the revision number.

  <b>Revision 1</b>:
   - Initial version.
yy
  **/
  UINT8                   Revision;
  CPU_CONFIG_PPI          *CpuConfig;          ///< Processor Features Configuration.
  POWER_MGMT_CONFIG_PPI   *PowerMgmtConfig;    ///< Processor Power ManagementFeatures Configuration.
};

#endif
