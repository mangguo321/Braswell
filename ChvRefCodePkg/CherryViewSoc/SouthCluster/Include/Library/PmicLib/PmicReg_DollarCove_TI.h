/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_REG_TIDC_H_
#define _PMIC_REG_TIDC_H_

#define    BIT00    0x01
#define    BIT01    0x02
#define    BIT02    0x04
#define    BIT03    0x08
#define    BIT04    0x10
#define    BIT05    0x20
#define    BIT06    0x40
#define    BIT07    0x80

#define TIDC_ID             0x00
#define  TIDC_MASK_VENDID   (BIT07|BIT06)
#define  TIDC_MASK_MAJREV   (BIT05|BIT04|BIT03)
#define  TIDC_MASK_MINREV   (BIT02|BIT01|BIT00)

#define TIDC_IRQ_R          0x01
#define  TIDC_MASK_CCEOCAL    BIT07  //CC calibration completes IRQ asserted
#define  TIDC_MASK_CCEOCONV   BIT06  //CC conversion completes IRQ asserted
#define  TIDC_MASK_VBUSDET    BIT05  //VBUS crosses detection threshold (rising or falling) IRQ asserted
#define  TIDC_MASK_VBATLOW    BIT04  //VBAT crosses low voltage detection threshold (rising or falling); VBATLOW IRQ asserted
#define  TIDC_MASK_ADCCOMPL   BIT02  //ADC conversion complete; ADCCOMPL IRQ asserted
#define  TIDC_MASK_DIETMPWARN BIT01  //Die temp comparator threshold crossed (rising and falling); DIETMPWARN IRQ asserted
#define  TIDC_MASK_PWRBTN     BIT00  //Power Button assertion debounce time met (press and release); PWRBTN IRQ asserted

#define TIDC_MIRQ           0x02
#define  TIDC_MASK_MCCEOCAL    BIT07  //CC calibration completes IRQ asserted
#define  TIDC_MASK_MCCEOCONV   BIT06  //CC conversion completes IRQ asserted
#define  TIDC_MASK_MVBUSDET    BIT05  //VBUS crosses detection threshold (rising or falling) IRQ asserted
#define  TIDC_MASK_MVBATLOW    BIT04  //VBAT crosses low voltage detection threshold (rising or falling); VBATLOW IRQ asserted
#define  TIDC_MASK_MADCCOMPL   BIT02  //ADC conversion complete; ADCCOMPL IRQ asserted
#define  TIDC_MASK_MDIETMPWARN BIT01  //Die temp comparator threshold crossed (rising and falling); DIETMPWARN IRQ asserted
#define  TIDC_MASK_MPWRBTN     BIT00  //Power Button assertion debounce time met (press and release); PWRBTN IRQ asserted

#define TIDC_SIRQ           0x03  
#define  TIDC_MASK_SCCEOCAL    BIT07  //One clock cycle pulse end of Calibration
#define  TIDC_MASK_SCCEOCONV   BIT06  //One clock cycle pulse end of Conversion
#define  TIDC_MASK_SVBUSDET    BIT05  //VBUS > VBUS_DET
#define  TIDC_MASK_SVBATLOW    BIT04  //VBAT < VBAT_LOW
#define  TIDC_MASK_SADCCOMPL   BIT02  //ADC is completing a conversion
#define  TIDC_MASK_SDIETMPWARN BIT01  //PMIC die temp monitoring threshold is triggered
#define  TIDC_MASK_SPWRBTN     BIT00  //power button release value after 30 ms debounce (non-pressed)

#define TIDC_PBUTTON        0x10
#define  TIDC_MASK_PBDWNTMR (BIT03|BIT02)
#define  TIDC_MASK_PBUPTMR  (BIT01|BIT00)

#define TIDC_CHIPCTRL       0x11
#define TIDC_RSTSRC_R       0x12
#define  TIDC_MASK_PBOFF    BIT07  //Power Button override was the shutdown reason
#define  TIDC_MASK_CLDOFF   BIT06  //SoC initiated cold off was the shutdown reason
#define  TIDC_MASK_UVLO     BIT05  //PMIC UVLO threshold was the shutdown reason
#define  TIDC_MASK_CLDRST   BIT04  //Cold reset was the start up reason
#define  TIDC_MASK_GLBRST   BIT03  //SoC initiated Global Reset was the start up reason
#define  TIDC_MASK_BATTIN   BIT02  //Battery insertion was the start up reason
#define  TIDC_MASK_CHGIN    BIT01  //Charger insertion was the start up reason
#define  TIDC_MASK_PBTN     BIT00  //Power button was the start up reason

#define TIDC_LOWBATTDET_W   0x17
#define TIDC_VBATTHR        0x18
#define TIDC_TMPSNSCFG      0x19  

#define TIDC_BUCK1CTRL      0x20
#define TIDC_BUCK2CTRL      0x21
#define TIDC_BUCK2SLPEXIT   0x22
#define TIDC_BUCK2SLEEP     0x23
#define TIDC_BUCK3CTRL      0x24
#define TIDC_BUCK3SLPEXIT   0x25
#define TIDC_BUCK3SLEEP     0x26
#define TIDC_BUCK4CTRL      0x27
#define TIDC_BUCK5CTRL      0x28
#define TIDC_BUCK5VSEL      0x29
#define TIDC_BUCK5SLPEXIT   0x2A
#define TIDC_BUCK5SLEEP     0x2B
#define TIDC_BUCK6CTRL      0x2C   

#define TIDC_LDO1CTRL       0x41
#define TIDC_LDO2CTRL       0x42
#define TIDC_LDO3CTRL       0x43
#define TIDC_LDO5CTRL       0x45
#define TIDC_LDO6CTRL       0x46
#define TIDC_LDO7CTRL       0x47
#define TIDC_LDO8CTRL       0x48
#define TIDC_LDO9CTRL       0x49
#define TIDC_LDO10CTRL      0x4A
#define TIDC_LDO11CTRL      0x4B
#define TIDC_LDO12CTRL      0x4C
#define TIDC_LDO13CTRL      0x4D
#define TIDC_LDO14CTRL      0x4E  

#define TIDC_ADCCTRL_W      0x50
#define TIDC_ADCDIETEMPZSE  0x51 
#define TIDC_ADCGPINTHERMGE 0x52
#define TIDC_ADCVBATZSEGE   0x53
#define TIDC_VBATHI         0x54
#define TIDC_VBATLO         0x55
#define  TIDC_MASK_DIETEMPH (BIT01|BIT00)

#define TIDC_DIETEMPHI      0x56
#define TIDC_DIETEMPLO      0x57
#define TIDC_BPTHERMHI      0x58
#define  TIDC_MASK_BPTHERMH (BIT01|BIT00)
#define TIDC_BPTHERMLO      0x59

#define TIDC_GPADCHI        0x5A
#define TIDC_GPADCLO        0x5B

#define TIDC_CC_CTRL        0x60
#define TIDC_CC_OFFSETHI    0x61
#define TIDC_CC_OFFSETLO    0x62
#define TIDC_CC_ACC_BYTE3   0x63
#define TIDC_CC_ACC_BYTE2   0x64
#define TIDC_CC_ACC_BYTE1   0x65
#define TIDC_CC_ACC_BYTE0   0x66
#define TIDC_CC_SMPL_BYTE2  0x67
#define TIDC_CC_SMPL_BYTE1  0x68
#define TIDC_CC_SMPL_BYTE0  0x69
#define TIDC_CC_INTG_BYTE1  0x6A
#define TIDC_CC_INTG_BYTE0  0x6B
#define TIDC_CC_SWOFFSETH   0x6C

#endif

