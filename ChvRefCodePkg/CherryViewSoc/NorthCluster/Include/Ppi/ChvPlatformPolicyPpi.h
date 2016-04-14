/** @file
  Interface definition details between CherryView MRC and platform drivers during PEI phase.

  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CHV_PLATFORM_POLICY_PPI_H_
#define _CHV_PLATFORM_POLICY_PPI_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gChvPolicyPpiGuid;

//
// PPI revision number
// Any backwards compatible changes to this PPI will result in an update in the revision number
// Major changes will require publication of a new PPI
//
#define MRC_PLATFORM_POLICY_PPI_REVISION  1

#ifndef MAX_SOCKETS
#define MAX_SOCKETS 4
#endif

#ifndef MAX_CHANNELS
#define MAX_CHANNELS 2
#endif

#ifndef MAX_DIMMS
#define MAX_DIMMS 2
#endif

#define MAX_OEM_SPD_ADDR  125
#define SPD_TABLE_SIZE  MAX_OEM_SPD_ADDR + 1

#define S3_TIMING_DATA_LEN          9
#define S3_READ_TRAINING_DATA_LEN   16
#define S3_WRITE_TRAINING_DATA_LEN  12

#ifndef S3_RESTORE_DATA_LEN
#define S3_RESTORE_DATA_LEN (S3_TIMING_DATA_LEN + S3_READ_TRAINING_DATA_LEN + S3_WRITE_TRAINING_DATA_LEN)
#endif // S3_RESTORE_DATA_LEN

#define BSW_SVID_CONFIG0    0
#define BSW_SVID_CONFIG1    1
#define BSW_SVID_CONFIG3    3
#define BSW_I2C_PMIC_CONFIG 8
#pragma pack(1)
//
// MRC Platform Data Structure
//
typedef struct {
  UINT8   SpdAddressTable[MAX_SOCKETS];
  UINT8   TSonDimmSmbusAddress[MAX_SOCKETS];

  UINT16  SmbusBar;
  UINT32  IchRcba;
  UINT32  WdbBaseAddress; // Write Data Buffer area (WC caching mode)
  UINT32  WdbRegionSize;
  UINT32  SmBusAddress;
  UINT8   UserBd;
  UINT8   PlatformType;
  UINT8   FastBoot;
  UINT8   DynSR;
  UINT8   HgMode;                            ///< HgMode (0=Disabled, 1=SG Muxed, 2=SG Muxless, 3=PEG)
  UINT16  HgSubSystemId;                     ///< Hybrid Graphics Subsystem ID
  UINT16  HgDelayAfterPwrEn;                 ///< Dgpu Delay after Power enable using Setup option
  UINT16  HgDelayAfterHoldReset;             ///< Dgpu Delay after Hold Reset using Setup option
} CHV_PLATFORM_DATA;

typedef struct {
  UINT16  MmioSize;
  UINT16  GttSize;
  UINT8   IgdDvmt50PreAlloc;
  UINT8   PrimaryDisplay;
  UINT8   ApertureSize;
  UINT8   InternalGraphics;
  UINT8   IgdTurboEn;
  UINT64  GttMmAdr;
  UINT32  GMAdr;
} GT_CONFIGURATION;

enum {
  MRC_MOTHERBOARDDOWN = 0, 
  MRC_SODIMM = 1, 
  MRC_COPOP = 2
};

typedef enum {
  MRC_DDR3L = 0, 
  MRC_LPDDR3 = 1,
} CHV_POLICY_DDR_TYPE;

typedef enum {
  MRC_DDR800         = 0,
  MRC_DDR1066        = 1,
  MRC_DDR1600        = 2,
  MRC_DDR800_SKU333  = 3,
  MRC_DDR1000_SKU333 = 4,
  MRC_DDR1333_SKU333 = 5,
  MRC_DDR900_SKU360  = 6,
  MRC_DDR1800_SKU360 = 7,
  MRC_DDR933_SKU373  = 8,
  MRC_DDR1866_SKU373 = 9,
  MRC_DDR_MAX =10,
} CHV_POLICY_DDR_FREQUENCY;

typedef enum {
  MRC_Width_x8 = 0,
  MRC_Width_x16 = 1,
  MRC_Width_x32 = 2,
} CHV_POLICY_DRAM_WIDTH;

typedef enum {
  MRC_Density_1Gb = 0,
  MRC_Density_2Gb = 1,
  MRC_Density_4Gb = 2,
  MRC_Density_8Gb = 3,
  MRC_Density_16Gb = 4,
  MRC_Density_6b = 5,
} CHV_POLICY_DRAM_DENSITY;

typedef enum {
  DimmInstalled = 0,    // Check Physical Spd Data thru the Smbus
  SolderDownMemory,     // Use OemSpdDataMemorySolderDown SPD data pointer
  DimmDisabled          // Dimm Slot Disabled
} OEM_MEMORY_DIMM_TYPE;

typedef UINT8 OEM_MEMORY_DIMM_CONFIGURATION;

typedef struct {
  UINT8   Enabled;
  UINT8   CurrentConfiguration;
} CHANNEL_CONFIGURATION;

typedef struct {
  UINT8                         EccSupport;
  UINT16                        DdrFreqLimit;
  UINT8                         MaxTolud;
  CHV_POLICY_DDR_TYPE           DdrType;
  UINT8                         DvfsEnable;
  CHV_POLICY_DDR_FREQUENCY      DvfsFreqA;
  CHV_POLICY_DDR_FREQUENCY      DvfsFreqB;
  UINT8                         RmtEvMode;
  UINT8                         RmtCommandMode;
  UINT8                         DualRankDram;
  UINT8                         MrcDynamicSr;
  UINT8                         MrcChannelSel_3_0;
  UINT8                         MrcChannelSel_4;
  UINT8                         AutoDetectDram;
  UINT8                         MrcDebugMsgLevel;
  UINT8                         MrcPm5Enable;
  UINT8                         MrcBankAddressHashingEnable;
  UINT8                         MrcRankSelInterleave;
  CHANNEL_CONFIGURATION         Channel[MAX_CHANNELS];
  UINT8                         MrcConfigChanged;
  UINT8                         UseDimmSpd;
  CHV_POLICY_DRAM_WIDTH         DramWidth;
  CHV_POLICY_DRAM_DENSITY       DramDensity;
  UINT8                         MrcDdr2nMode;
  UINT8                         CaMirrorEn;
  UINT8                         TSEGSizeSel;
  UINT8                         (*OemSpdDataMemorySolderDown)[SPD_TABLE_SIZE];
  OEM_MEMORY_DIMM_CONFIGURATION OemMemoryDimmConfiguration[MAX_CHANNELS][MAX_DIMMS];
  UINT8                         MrcRxPwrTrainingDisable;
  UINT8                         MrcTxPwrTrainingDisable;
  UINT8                         MrcScramblerDisable;
  UINT8                         MrcSpeedGrade;
  UINT8                         DrpLockDisable;
  UINT8                         ReutLockDisable;
  UINT8                         UseBiosProvidedDramTimings;
  UINT8                         RhPrevention;
  UINT8                         RemapEnable;
  UINT8                         Ddr3AutoSelfRefreshEnable;
  UINT8                         MrcSpdFrequencyOverrideEnable;
} MEMORY_CONFIGURATION;

///
/// HG GPIO Data Structure
///
typedef struct {
  UINT32   CommunityOffset;  ///< GPIO Community
  UINT32   PinOffset;        ///< GPIO Pin
  BOOLEAN  Active;           ///< 0=Active Low; 1=Active High
} HG_GPIO_INFO;

///
/// Defines the Hybrid Graphics configuration parameters.
///
typedef struct {
  BOOLEAN       GpioSupport;        ///< 1=Supported; 0=Not Supported
  HG_GPIO_INFO  HgDgpuHoldRst;     ///< This field contain dGPU HLD RESET GPIO value and level information
  HG_GPIO_INFO  HgDgpuPwrEnable;   ///< This field contain dGPU_PWR Enable GPIO value and level information
} HG_GPIO_DATA;
//
// CHV Platform Policiy PPI
//
typedef struct _CHV_POLICY_PPI {
  UINT8                 Revision;
  CHV_PLATFORM_DATA     PlatformData;
  GT_CONFIGURATION      GtConfig;
  MEMORY_CONFIGURATION  MemConfig;
  HG_GPIO_DATA          HgGpioData;
  UINT8                 PMIC_I2CBus;       // This field is NA when ChvSvidConfig is not BSW_I2C_PMIC_CONFIG
  VOID                  *S3DataPtr;       // was called MRC_PARAMS_SAVE_RESTORE
  UINT8                 ISPEn;            //ISP (IUNIT) Device Enabled
  UINT8                 ISPPciDevConfig;  //ISP (IUNIT) Device Config: 0->B0/D2/F0 for Window OS, 1->B0D3/F0 for Linux OS
  BOOLEAN               IEDEnabled;
  UINT32                IEDRamSize;
  UINT8                 ChvSvidConfig;
  UINT8                 PunitPwrConfigDisable;
#ifdef PRAM_SUPPORT
  UINT32                PramSize;
#endif
  UINT8                 PkgPwrLim1; // Power Limit Value (to be set in PUINT PKG_PWR_LIM1)
} CHV_POLICY_PPI;

#pragma pack()

#endif // _CHV_POLICY_PPI_H_
