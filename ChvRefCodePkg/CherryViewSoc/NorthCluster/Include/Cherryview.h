/** @file
  This header file provides common definitions just for Cherryview-SOC using to avoid including extra module's file.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MC_H_INCLUDED_
#define _MC_H_INCLUDED_
//
// Extended Configuration Base Address
//
#define EC_BASE        0xE0000000

//
// DEVICE 0 (Memory Controller Hub)
//
#define MC_BUS      0x00
#define MC_DEV      0x00
#define MC_DEV2      0x02
#define MC_FUN      0x00
#define MC_VID      0x8086
#define MC_DID_OFFSET    0x2      //Device Identification
#define MC_GGC_OFFSET    0x50    //GMCH Graphics Control Register

// Message Bus Register Definitions
#define CHV_MBR_READ_CMD  0x10000000
#define CHV_MBR_WRITE_CMD  0x11000000

//Common for Gunit/DISPIO/DFXLAKSEMORE/DISPCONT units
#define CHV_MBR_GDISPIOREAD_CMD    0x00000000
#define CHV_MBR_GDISPIOWRITE_CMD  0x01000000

//Common for Smbus units
#define CHV_SMB_REGREAD_CMD  0x04000000
#define CHV_SMB_REGWRITE_CMD  0x05000000

//Common for Punit/DFX/GPIONC/DFXSOC/DFXNC/DFXVISA/CCU units
#define CHV_MBR_PDFXGPIODDRIOREAD_CMD  0x06000000
#define CHV_MBR_PDFXGPIODDRIOWRITE_CMD  0x07000000

//Msg Bus Registers
#define MC_MCR      0x000000D0    //Cunit Message Control Register
#define MC_MDR      0x000000D4    //Cunit Message Data Register
#define MC_MCRX      0x000000D8    //Cunit Message Control Register Extension
#define MC_MCRXX    0x000000DC   //cunit Message Controller Register Extension 2

#define MC_DEVEN_OFFSET    0x54    //Device Enable
#define B_DEVEN_D2F0EN      BIT3      // Internal Graphics Engine F0 Enable

//smBiosMemory.c use this
//PunitDriver.c
#define MC_TSEGMB_OFFSET  0xAC    //TSEG Memory Base


//
// Device 2 Register Equates
//
#define IGD_BUS             0x00
#define IGD_DEV             0x02
#define IGD_FUN_0           0x00
#define IGD_FUN_1           0x01
#define IGD_DEV_FUN         (IGD_DEV << 3)
#define IGD_BUS_DEV_FUN     (MC_BUS << 8) + IGD_DEV_FUN
#define IGD_VID             0x8086
#define IGD_DID             0x22B0
#define IGD_DID_C0          0x22B1
#define IGD_MGGC_OFFSET     0x0050      //GMCH Graphics Control Register 0x50
#define IGD_BSM_OFFSET      0x005C      //Base of Stolen Memory
#define IGD_SWSCI_OFFSET    0x00E0      //Software SCI 0xE0 2
#define IGD_ASLE_OFFSET     0x00E4      //System Display Event Register 0xE4 4
#define IGD_ASLS_OFFSET     0x00FC      // ASL Storage
#define IGD_DID_QS          0x0BE2      //RCOverride -a: Fix the DID error


#endif
