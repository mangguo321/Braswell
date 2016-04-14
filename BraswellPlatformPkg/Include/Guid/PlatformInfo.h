/** @file
  GUID used for Platform Info Data entries in the HOB list.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PLATFORM_INFO_GUID_H_
#define _PLATFORM_INFO_GUID_H_
#include <PiPei.h>

#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/SmbusLib.h>
#ifndef FSP_FLAG
#include <IndustryStandard/Smbus.h>
#endif
#define PLATFORM_INFO_REVISION 1      // revision id for current platform information struct

#define BOARD_REVISION_DEFAULT  0xFF
#define UNKNOWN_FABID           0x0F
#define FAB_ID_MASK             0x0F

#define BSW_CR_PMIC_I2C_BUSNO        1
#define BSW_CR_PMIC_I2C_SLAVE_ADDR   0x5E
//
// Definitions required for panel initialization code for CherryTrail
//

#define MIPI_I2C_BUSNO                          5
#define FFD_MIPI_I2C_BUSNO                      1
#define MIPI_I2C_SLAVE_DEVICE_ADDR              0x2C
#define MIPI_I2C_BACKLIGHT_BOOST_REG_OFFSET     0x01
#define MIPI_I2C_UNUSEED_CHANNEL_DISABLE_OFFSET 0x16
#define MIPI_BACKLIGHT_BOOST_VALUE              0x01
#define MIPI_DISABLE_UNUSED_CHANNELS_VALUE      0x07
#define PANEL_CONFIG_MIPI                       0x00
#define PANEL_CONFIG_EDP                        0x01
#define PANEL_VENDOR_LG                         0x00
#define PANEL_VENDOR_PANASONIC                  0x01
#define PANEL_VENDOR_INNOLUX                    0x02
#define WIFI_PCI_BUS_NUMBER                     0x01
#define WIFI_PCI_DEVICE_NUMBER                  0x00
#define WIFI_PCI_FUNCTION_NUMBER                0x00
#define STP_WIFI_DEVICE_VENDOR_ID               0x095A8086

#define POST_CODE_I2C_BUS_NO     0x01
#define POST_CODE_I2C_SLAVE_ADDR 0x38

#define IO_EXPANDER_I2C_BUS_NO 6    // GPIO expander
#define IO_EXPANDER_SLAVE_ADDR 0x20

#define PSS_CHIP_I2C_BUS_NO 0       // PSS Chip
#define PSS_CHIP_SLAVE_ADDR 0x6E
#define PSS_CHIP_IPN_LENGTH 12
#define PSS_CHIP_IPN_OFFSET 160

#define I2C_EXPANDER_I2C_BUS_NO 6   // I2C expander
#define I2C_EXPANDER_SLAVE_ADDR  0x23

#define IO_EXP_REG0 0
#define IO_EXP_REG1 1
#define IO_EXP_REG2 2
#define IO_EXP_REG3 3

#define I2C_EXP_REG0 0
#define I2C_EXP_REG1 1
#define I2C_EXP_REG2 2
#define I2C_EXP_REG3 3
#define I2C_EXP_REG4 0x4
#define I2C_EXP_REGC 0xC

#define CHG_SMB349_I2C_CHANNEL_NUMBER 0x0
#define CHG_SMB349_I2C_SLAVE_ADDRESS 0x35
#define CHG_SMB349_STATUS_REG_B  0x3C
#define CHG_SMB349_STATUS_REG_D  0x3E

#define MAX_FAB_ID_RETRY_COUNT  100
#define MAX_FAB_ID_CHECK_COUNT  3

#define PLATFORM_INFO_HOB_REVISION      0x1

#define CR_USB_MUX_EN    0xfed9d448
#define COPOP_USB_MUX_EN 0xFED8D460

#define EFI_PLATFORM_INFO_GUID \
  { \
    0x1e2acc41, 0xe26a, 0x483d, 0xaf, 0xc7, 0xa0, 0x56, 0xc3, 0x4e, 0x8, 0x7b \
  }

extern EFI_GUID gEfiPlatformInfoGuid;

typedef enum {
  FlavorUnknown = 0,
  //
  // Mobile
  //
  FlavorMobile = 1,
  //
  // Desktop
  //
  FlavorDesktop = 2,
  //
  // Tablet
  //
  FlavorTablet = 3
} PLATFORM_FLAVOR;

typedef enum {
  BOARD_ID_AV_SVP         = 0x0,      // Alpine Valley Board
  BOARD_ID_CHT_CR         = 0x1,      // Cherry Trail CR
  BOARD_ID_CHT_COPOP      = 0x2,      // Cherry Trail COPOP
  BOARD_ID_CHV_T          = 0x3,      // Cherry Trail Tablet
  BOARD_ID_CHT_SONY       = 0x4,      // Cherry Trail Sony
  BOARD_ID_BL_RVP_DDR3L   = 0x5,      // BayLake Board (RVP DDR3L)
  BOARD_ID_BL_STHI        = 0x7,      // PPV- STHI Board
  BOARD_ID_CHT_FFD        = 0x8,      // Cherry Trail FFD
  BOARD_ID_CHT_HR15_RVP   = 0x9,      // Cherry Trail Holiday Refresh RVP
  BOARD_ID_CHT_HR15_FFD   = 0xA,      // Cherry Trail Holiday Refresh FFD
  BOARD_ID_DUMMY          = 0xf,      // For Usage incase of no Board identified
  BOARD_ID_BB_RVP         = 0x20,     // Bayley Bay Board
  BOARD_ID_BS_RVP         = 0x30,     // Bakersport Board
  BOARD_ID_BSW            = 0x44,     // Braswell ID
  BOARD_ID_BSW_CR         = 0x55,     // Braswell CR
  BOARD_ID_BSW_RVP_ECC    = 0x60,     // Braswell RVP ECC
  BOARD_ID_BSW_CH         = 0x80,     // Braswell Cherry Hill
  BOARD_ID_BSW_CI         = 0x82,     // Braswell Cherry Island
  BOARD_ID_BSW_WD         = 0X83,     // Braswell West Digital
  BOARD_ID_SVP            = 0xee,     // SV Board
  BOARD_ID_BL_FFRD        = 0xF1,     // BayLake FFRD
  BOARD_ID_BL_RVP         = 0xF2,     // BayLake RVP
  BOARD_ID_CL_ERB         = 0xF3,     // CherryTrail ERB
  BOARD_ID_CL_CRB         = 0xF4,     // CherryTrail CRB
  BOARD_ID_BL_FFRD8       = 0xF5      // Baytrail FFRD8
} BOARD_ID_LIST;

typedef enum {
  FAB1 = 0,
  FAB2 = 1,
  FAB3 = 2
} FAB_ID_LIST;

//
// Hardware_ID table for Product_Line_ID == INTEL_CHT_TABLET
//
typedef enum {
	CHT_TABLET_RVP1		= 0, /* Cherry Trail RVP Fab 1 */
	CHT_TABLET_RVP2		= 1, /* Cherry Trail RVP Fab 2 */
	CHT_TABLET_RVP3		= 2, /* Cherry Trail RVP Fab 3 */
	CHT_TABLET_FFD_PR0	= 3, /* Cherry Trail FFRD PR0 */
	CHT_TABLET_FFD_PR1	= 4, /* Cherry Trail FFRD PR1 */
	CHT_TABLET_FFD_PR2	= 5, /* Cherry Trail FFRD PR2 */
	CHT_TABLET_UNKNOWN	= 0xFF
}INTEL_CHT;

typedef enum {
    PR0  = 0,   // FFRD PR0
    PR05 = 1,   // FFRD PR0.3 and PR 0.5
    PR1  = 2,   // FFRD PR1
    PR11 = 3    // FFRD PR1.1
} FFRD_ID_LIST;

typedef enum {
    FFD_8_PR0  = 1,  // FFRD8 PR0
    FFD_8_PR1  = 2,   // FFRD8 PR1
	FFD_8_PR2  = 3   // FFRD8 PR1
} FFD_ID_LIST;

//
// PMIC ID
//
typedef enum {
    PMIC_TYPE_CRC_PLUS   = 0,
    PMIC_TYPE_DC_TI      = 1,
    PMIC_TYPE_DC_XPOWERS = 2,
    PMIC_TYPE_WC         = 3,
    PMIC_TYPE_MTV_PLUS   = 4,
    PMIC_TYPE_DUMMY      = 0xFE,
    PMIC_TYPE_NONE       = 0xff,
}PMIC_TYPE;
#pragma pack(1)

typedef struct {
  UINT16  PciResourceIoBase;
  UINT16  PciResourceIoLimit;
  UINT32  PciResourceMem32Base;
  UINT32  PciResourceMem32Limit;
  UINT64  PciResourceMem64Base;
  UINT64  PciResourceMem64Limit;
  UINT64  PciExpressBase;
  UINT32  PciExpressSize;
  UINT8   PciHostAddressWidth;
  UINT8   PciResourceMinSecBus;
} EFI_PLATFORM_PCI_DATA;

typedef struct {
  UINT8 CpuAddressWidth;
  UINT32 CpuFamilyStepping;
} EFI_PLATFORM_CPU_DATA;

typedef struct {
  UINT8 SysIoApicEnable;
  UINT8 SysSioExist;
} EFI_PLATFORM_SYS_DATA;

typedef struct {
  UINT32  MemTolm;
  UINT32  MemMaxTolm;
  UINT32  MemTsegSize;
  UINT32  MemTsegBase;
  UINT32  MemIedSize;
  UINT32  MemIgdSize;
  UINT32  MemIgdBase;
  UINT32  MemIgdGttSize;
  UINT32  MemIgdGttBase;
  UINT64  MemMir0;
  UINT64  MemMir1;
  UINT32  MemConfigSize;
  UINT16  MmioSize;
} EFI_PLATFORM_MEM_DATA;

typedef struct {
  UINT32 IgdOpRegionAddress;    // IGD OpRegion Starting Address
  UINT8  IgdBootType;           // IGD Boot Display Device
  UINT8  IgdPanelType;          // IGD Panel Type CMOs option
  UINT8  IgdTvFormat;           // IGD TV Format CMOS option
  UINT8  IgdTvMinor;            // IGD TV Minor Format CMOS option
  UINT8  IgdPanelScaling;       // IGD Panel Scaling
  UINT8  IgdBlcConfig;          // IGD BLC Configuration
  UINT8  IgdBiaConfig;          // IGD BIA Configuration
  UINT8  IgdSscConfig;          // IGD SSC Configuration
  UINT8  IgdDvmtMemSize;        // IGD DVMT Memory Size
  UINT8  IgdFunc1Enable;        // IGD Function 1 Enable
  UINT8  IgdHpllVco;            // HPLL VCO
  UINT8  IgdSciSmiMode;         // GMCH SMI/SCI mode (0=SCI)
  UINT8  IgdPAVP;               // IGD PAVP data
} EFI_PLATFORM_IGD_DATA;

/*
  ****************************************************************************
  ****************************************************************************
  ****************************************************************************
  ****************************************************************************
  ****************************************************************************
  *            CherryView GPIOs Configuration                                *
  ****************************************************************************
  ****************************************************************************
  ****************************************************************************
  ****************************************************************************
  ****************************************************************************
  */

#define NA                    0xFF 

//
// Security SAI for GPIO
//

#define IA_UNTRUUSTED      0x00000001
#define IA_UCODE           0x00000002
#define IA_SMM             0x00000004
#define UCODE_NPP          0x00000008
#define IA_BOOT            0x00000010
#define IA_XUCODE          0x00000080
#define PUNIT_TRUSTED      0x00000100
#define SEC_TRUSTED        0x00000200
#define DRM                0x00000400
#define FUSESTRAP_PULLER   0x00000800
#define FUSE_PROVIDER      0x00001000
#define STRAP_PROVIDER     0x00002000
#define DFX_UNTRUSTED      0x00004000
#define DFX_TRUSTED        0x00008000
#define PMC_TRUSTED        0x00010000
#define DRANG              0x00020000
#define ISH_T              0x00040000

typedef struct _EFI_PLATFORM_INFO_HOB {
  UINT16                      PlatformType; // Platform Type
  UINT8                       BoardId;             // Board ID
  PLATFORM_FLAVOR             PlatformFlavor;      // Platform Flavor
  UINT8                       ECSupport;
  UINT8                       FanSupport;
  UINT8                       BatterySupport;
  UINT8                       DDRDaughterCardCh0Id;// DDR daughter card channel 0 id
  UINT8                       DDRDaughterCardCh1Id;// DDR daughter card channel 1 id
  EFI_PLATFORM_PCI_DATA       PciData;
  EFI_PLATFORM_CPU_DATA       CpuData;
  EFI_PLATFORM_MEM_DATA       MemData;
  EFI_PLATFORM_SYS_DATA       SysData;
  EFI_PLATFORM_IGD_DATA       IgdData;
  UINT8                       RevisonId;           // Structure Revision ID
  UINT32                      SsidSvid;
  UINT8                       WarmResetOccured;
  UINT16                      MemCfgID;
  UINT16                      FABID;
  PMIC_TYPE                   PmicId;
  UINT8                       BoardSvidConfig;
} EFI_PLATFORM_INFO_HOB;

#pragma pack()

EFI_STATUS
InstallPlatformSysCtrlGPIONotify (
  IN CONST EFI_PEI_SERVICES           **PeiServices
  );

#endif
