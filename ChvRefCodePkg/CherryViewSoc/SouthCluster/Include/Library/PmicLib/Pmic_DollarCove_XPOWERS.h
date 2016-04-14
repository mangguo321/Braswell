/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_REG_DCX_P_H_
#define _PMIC_REG_DCX_P_H_

#define PMIC_DC_X_I2C_BUSNO        6     //I2C6 is used. index from 0
#define PMIC_DC_X_I2C_ADDR         0x34
#define PMIC_DEV_DC_XPOWERS        0x41  // bit 7-6 & bit 3-0:  010001: IC is AXP288

#define USB3750_ADDR              0x68
#define USB3750_CHRG_TYPE_REG     0x0

#define XPOWER_POWERSRC_AND_CHARGER_STATUS 0x02
#define XPOWER_REG_SOC	    	0xB9	//	Bit 7=soc is valid(1) or not valid(0)
									    //	BIT 0 - 6 = SOC in %
#define XPOWER_REG_MAXCAP1	    0xE0	//	Bit 7=Capacity is valid(1) or not valid(0)
									    //	BIT 0 - 6 = Batt max capacity bit[14:8]
#define XPOWER_REG_MAXCAP2	    0xE1	//	BIT 0 - 7 = Batt max capacity bit[7:0] multiply by 1.456mah
#define XPOWER_REG_VOLTAGE1     0x78	// BIT[11:4]
#define XPOWER_REG_VOLTAGE2     0x79	// BIT[3:0] = rsvd, BIT[7:4] = [3:0]
#define XPOWER_REG_CHG_CU1	    0x7A	//  BIT 0- 7 = Charging current [11:4]
#define XPOWER_REG_CHG_CU2	    0x7B	//  BIT 0- 3 =RSVD, 4-7 = Charging current [3:0] multiply by 1ma
#define XPOWER_REG_DISCHG_CU1	0x7C	//  BIT 0- 7 = Charging current [11:4]
#define XPOWER_REG_DISCHG_CU2	0x7D	//  BIT 0- 3 =RSVD, 4-7 = Charging current [3:0] multiply by 1ma
#define XPOWER_REG_CHR_TYP		0x2F	// BIT 0 - 4 = Reserved, 7-5 = chg src (01 = SDP, 02 = CPD, 03 = DCP)
#define XPOWER_REG_CHG_STS		0x01	// BIT6 = Charging Indication, BIT5 = Battery pres
#define XPOWER_BATT_CAPACITY	5100
#define XPOWER_BATT_CHG_CNTL	0x2C	//BIT 0 intiate charging
#define XPOWER_BATT_ILIM		0x30	//30[0:1] : ilim
#define XPOWER_BATT_ICHG		0x33	//33[3:0] : Ichg
#define XPOWER_VBUS_CLIM_SET    0x35    //35[7:4] : vbus current limit set

//DollarCove XPOWER FG Characterization data Provided by FG Mfg
//This Registers 0xC0-0xDF is OCV Percentage Table ( Refer AXP288 Datasheet for more detail )
//Characterization Start ----->
#define XPOWER_FG_OCV_C0	00
#define XPOWER_FG_OCV_C1	00
#define XPOWER_FG_OCV_C2	00
#define XPOWER_FG_OCV_C3	00
#define XPOWER_FG_OCV_C4	00
#define XPOWER_FG_OCV_C5	00

#define XPOWER_FG_OCV_C6	01
#define XPOWER_FG_OCV_C7	01
#define XPOWER_FG_OCV_C8	02
#define XPOWER_FG_OCV_C9	03
#define XPOWER_FG_OCV_CA	04

#define XPOWER_FG_OCV_CB	11
#define XPOWER_FG_OCV_CC	17
#define XPOWER_FG_OCV_CD	25
#define XPOWER_FG_OCV_CE	37
#define XPOWER_FG_OCV_CF	42
#define XPOWER_FG_OCV_D0	45

#define XPOWER_FG_OCV_D1	49
#define XPOWER_FG_OCV_D2	52
#define XPOWER_FG_OCV_D3	57
#define XPOWER_FG_OCV_D4	61
#define XPOWER_FG_OCV_D5	65
#define XPOWER_FG_OCV_D6	68

#define XPOWER_FG_OCV_D7	71
#define XPOWER_FG_OCV_D8	75
#define XPOWER_FG_OCV_D9	78

#define XPOWER_FG_OCV_DA	80
#define XPOWER_FG_OCV_DB	82
#define XPOWER_FG_OCV_DC	86
#define XPOWER_FG_OCV_DD	91
#define XPOWER_FG_OCV_DE	95
#define XPOWER_FG_OCV_DF  100

#define BATRDC  75    //mO
#define BATCAP  10164 //mAh

#define BATTERY_MAX_CAP_HI  (0x80|(((BATCAP*1000/1456)>>8)&0X7F))
#define BATTERY_MAX_CAP_LO  ((BATCAP*1000/1456)&0xFF)

#define	BATTERY_RDC1  (0xC0|((((BATRDC*10000+5371)/10742)>>8)&0x1F))
#define BATTERY_RDC0  (((BATRDC*10000+5371)/10742)&0xFF)

#define XPOWER_BATTERY_BPTH_VALID             0x0F00

//Characterization Table End <-----------

#define    DC_X_PMIC_REG_VR_CTRL_DLDO1           0x15
#define    DC_X_PMIC_REG_VR_CTRL_DLDO2           0x16
#define    DC_X_PMIC_REG_VR_CTRL_DLDO3           0x17
#define    DC_X_PMIC_REG_VR_CTRL_DLDO4           0x18
#define    DC_X_PMIC_REG_VR_CTRL_ELDO1           0x19
#define    DC_X_PMIC_REG_VR_CTRL_ELDO2           0x1A
#define    DC_X_PMIC_REG_VR_CTRL_ELDO3           0x1B 
#define     DC_X_MASK_ELDO_VOL_18                (BIT04|BIT02|BIT01) 

EFI_STATUS
EFIAPI
AcquireOwnership(void);

EFI_STATUS
EFIAPI
ReleaseOwnership(void);

EFI_STATUS
EFIAPI
DcXPmicDVFSInit(void);

#endif

