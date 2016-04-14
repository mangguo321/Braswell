/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_REG_H_
#define _PMIC_REG_H_

#define    BIT00	0x01
#define    BIT01	0x02
#define    BIT02	0x04
#define    BIT03	0x08
#define    BIT04	0x10
#define    BIT05	0x20
#define    BIT06	0x40
#define    BIT07	0x80

#define PMIC_REG_ID0                0x0 
#define PMIC_REG_REV                0x1 
#define PMIC_REG_IRQLVL1            0x2 
#define PMIC_REG_PWRSRCIRQ          0x3 
#define  B_PMIC_PWRSRCIRQ_BATDET        BIT02
#define  B_PMIC_PWRSRCIRQ_DCINDET       BIT01
#define  B_PMIC_PWRSRCIRQ_VBUSDET       BIT00
#define PMIC_REG_THRMIRQ0           0x4 
#define PMIC_REG_THRMIRQ1           0x5 
#define PMIC_REG_THRMIRQ2           0x6 
#define PMIC_REG_BCUIRQ             0x7 
#define PMIC_REG_ADCIRQ0            0x8 
#define PMIC_REG_ADCIRQ1            0x9 
#define PMIC_REG_CHGRIRQ            0xA 
#define PMIC_REG_GPIO0IRQ           0xB 
#define PMIC_REG_GPIO1IRQ           0xC 
#define PMIC_REG_VHDMIIRQ           0xD 
#define PMIC_REG_MIRQLVL1           0xE 
#define  PMIC_MASK_MVHMDI           BIT06
#define  PMIC_MASK_MGPIO            BIT05
#define  PMIC_MASK_MCHGRINT         BIT04
#define  PMIC_MASK_MADC             BIT03
#define  PMIC_MASK_MBCU             BIT02
#define  PMIC_MASK_MTHRM            BIT01
#define  PMIC_MASK_MPWRSRC          BIT00
#define  PMIC_MASK_ALL_IRQ          0x7F

#define PMIC_REG_MPWRSRCIRQS0       0xF 
#define  PMIC_MASK_BATDET           BIT02    //Battery Detection Mask in S0 State
#define  PMIC_MASK_DCINDET          BIT01    //AC/DC Adapter Detection Mask in S0 State
#define  PMIC_MASK_VBUSDET          BIT00    //VBUS Detection Mask in S0 State

#define PMIC_REG_MPWRSRCIRQSX       0x10
#define PMIC_REG_MTHRMIRQ0          0x11
#define PMIC_REG_MTHRMIRQ1          0x12
#define PMIC_REG_MTHRMIRQ2          0x13
#define PMIC_REG_MBCUIRQ            0x14
#define  PMIC_MASK_VCRIT            BIT02
#define  PMIC_MASK_VWARNA           BIT01 
#define  PMIC_MASK_VWARNB           BIT00
#define  PMIC_MASK_MBCU_ALL         (BIT02|BIT01|BIT00)

#define PMIC_REG_MADCIRQ0           0x15
#define PMIC_REG_MADCIRQ1           0x16
#define PMIC_REG_MCHGRIRQS0         0x17
#define  PMIC_MASK_MCHGR            BIT00
#define PMIC_REG_MCHGRIRQSX         0x18
#define PMIC_REG_MGPIO0IRQS0        0x19
#define PMIC_REG_MGPIO1IRQS0        0x1A
#define PMIC_REG_MGPIO0IRQSX        0x1B
#define PMIC_REG_MGPIO1IRQSX        0x1C
#define PMIC_REG_MVHDMIIRQ          0x1D
#define PMIC_REG_SPWRSRC            0x1E
#define  PMIC_MASK_BATDET           BIT02
#define  PMIC_MASK_DCINDET          BIT01
#define  PMIC_MASK_VBUSDET          BIT00

#define PMIC_REG_REGLOCK            0x1F
#define PMIC_REGLOCK_VREG_BIT       BIT0
#define PMIC_REGLOCK_VCRIT_CFG_BIT  BIT1
#define PMIC_REGLOCK_THERMAL_BIT    BIT2
#define PMIC_REGLOCK_LOWBATDET_BIT  BIT3

//
// Mask via Crystal Cove Plus spec 0.7
// BUGBUG: we added the TBD into PMIC definition, and need update the TBD when PMIC spec is >0.7
//
#define PMIC_REG_RESETSRC0          0x20
#define  PMIC_MASK_RBATRM             BIT06   
#define  PMIC_MASK_RVSYSOVP           BIT05
#define  PMIC_MASK_RVSYSUVP           BIT04   
#define  PMIC_MASK_RBATTEMP           BIT03
#define  PMIC_MASK_RSYSTEMP           BIT02   
#define  PMIC_MASK_RPMICTEMP          BIT01

#define PMIC_REG_RESETSRC1          0x21
#define  PMIC_MASK_RFCO               BIT02   
#define  PMIC_MASK_RIDBATRM           BIT01
#define  PMIC_MASK_RVCRIT             BIT00   

#define PMIC_REG_WAKESRC            0x22
#define  PMIC_MASK_WAKEADPT           BIT03   
#define  PMIC_MASK_WAKEUSB            BIT02   
#define  PMIC_MASK_WAKEBAT            BIT01   
#define  PMIC_MASK_WAKEPBTN           BIT00   
  
#define PMIC_REG_LOWBATDET0         0x23
#define  PMIC_MASK_DCBOOT           BIT07   //0--BATLOW_B status depending on battery voltage set by LOWBATDC[3:0]
#define  PMIC_MASK_LOWBATDC         (BIT06|BIT05|BIT04)  // Valid Battery Detection Threshold with AC/DC adapter
#define  PMIC_MASK_LOWBAT           (BIT03|BIT02|BIT01|BIT00)  //Valid Battery Detection Threshold with No Charge source

#define PMIC_REG_LOWBATDET1         0x24
#define  PMIC_MASK_LOWBATDCP        (BIT07|BIT06|BIT05|BIT04)  // Valid Battery Detection Threshold with DCP Charge Source
#define  PMIC_MASK_LOWBATSDP        (BIT03|BIT02|BIT01|BIT00)  //Valid Battery Detection Threshold with SDP Charge

#define PMIC_REG_PSDETCTRL          0x25
#define  PMIC_MASK_DBIEN            BIT05   //Digital Battery Communication enable
#define  PMIC_MASK_BATRMSRC         BIT04   //Battery removal Dectection Source 0-VBAT Comparator 1-BATID
#define  PMIC_MASK_BATRMPDEN        BIT03   //Battery Removal Power Down Enable
#define  PMIC_MASK_BATDBEN          BIT02   //Battery Detection Debounce timer
#define  PMIC_MASK_VDCINDBEN        BIT01   //Enables or disables VDCIN comparator debounce window.
#define  PMIC_MASK_VBUSDBEN         BIT00   //Enables or disables VBUS comparator debounce window.

#define PMIC_REG_PBCONFIG0          0x26 
#define  PMIC_MASK_PBDIS            (BIT07|BIT06)  //disable UI button func 
#define  PMIC_MASK_FCOT             (BIT03|BIT02|BIT01|BIT0) //Time that the power button has to be held down before a forced system shutdown is triggered.

#define PMIC_REG_PBSTATUS           0x27 
#define  PMIC_MASK_PBLVL            BIT04  //0 = Power button pressed 
#define  PMIC_MASK_PBHT             (BIT03|BIT02|BIT01|BIT00)  //Time that the power button has been held down

#define PMIC_REG_UIBSTATUS          0x28
#define  PMIC_MASK_UIBLVL           BIT04  //0 = UI button pressed 
#define  PMIC_MASK_UIBHT            (BIT03|BIT02|BIT01|BIT00)  //Time that the UI button has been held down

#define PMIC_REG_MODEMCTRL          0x29
#define PMIC_REG_BBCHGRCFG          0x2A
#define PMIC_REG_GPIO0P0CTLO        0x2B
#define  PMIC_MASK_ALTFUNCEN        BIT06   //alternative function enable bit
#define  PMIC_MASK_DIR              BIT05   //GPIO pin direction 0-input 1-output 
#define  PMIC_MASK_DRV              BIT04   //GPIO output driver type 0-CMOS 1-Open Drain
#define  PMIC_MASK_REN              BIT03   //Internal Pullup/pulldown Enable Bit
#define  PMIC_MASK_RVAL             (BIT02|BIT01)  //Internal pullup resistor value 00-2kohm pulldown 01-2kohm pullup 10-50kohm pulldown 11-50kohm pullup
#define  PMIC_MASK_RVAL_2K_PD       0x0
#define  PMIC_MASK_RVAL_2K_PU       (0x1<<1)
#define  PMIC_MASK_RVAL_50K_PD      (0x2<<1)
#define  PMIC_MASK_RVAL_50K_PU      (0x3<<1)
#define  PMIC_MASK_DOUT             BIT00   //GPIO Pin outpin value    0--Electrical Low 1-Electrical High (CMOS) or High-Z (OD)
#define  PMIC_MASK_OUTPUT_ALL       0x7f

#define PMIC_REG_GPIO0P1CTLO        0x2C
#define PMIC_REG_GPIO0P2CTLO        0x2D
#define PMIC_REG_GPIO0P3CTLO        0x2E
#define PMIC_REG_GPIO0P4CTLO        0x2F
#define PMIC_REG_GPIO0P5CTLO        0x30
#define PMIC_REG_GPIO0P6CTLO        0x31
#define PMIC_REG_GPIO0P0CTLI        0x33
#define  PMIC_MASK_POL              BIT04  //Input polarity
#define  PMIC_MASK_GPIDBNC          BIT03  //GPIO input pin debounce enable
#define  PMIC_MASK_INTCNT           (BIT02|BIT01)  //GPIO input pin interrupt detected
#define  PMIC_MASK_INTCNT_DISABLE   0x0
#define  PMIC_MASK_INTCNT_NEGA      (0x1<<1)   //negative edge
#define  PMIC_MASK_INTCNT_POS       (0x2<<1)   //positive edge
#define  PMIC_MASK_INTCNT_BOTH      (0x3<<1)   //both edge
#define  PMIC_MASK_DIN              BIT00  //GPIO input pin status
#define  PMIC_MASK_INPUT_ALL        0x1f

#define PMIC_REG_GPIO0P1CTLI        0x34
#define PMIC_REG_GPIO0P2CTLI        0x35
#define PMIC_REG_GPIO0P3CTLI        0x36
#define PMIC_REG_GPIO0P4CTLI        0x37
#define PMIC_REG_GPIO0P5CTLI        0x38
#define PMIC_REG_GPIO0P6CTLI        0x39
#define PMIC_REG_GPIO1P0CTLO        0x3B
#define PMIC_REG_GPIO1P1CTLO        0x3C
#define PMIC_REG_GPIO1P2CTLO        0x3D
#define PMIC_REG_GPIO1P3CTLO        0x3E
#define PMIC_REG_GPIO1P4CTLO        0x3F
#define PMIC_REG_GPIO1P5CTLO        0x40
#define PMIC_REG_GPIO1P6CTLO        0x41
#define PMIC_REG_GPIO1P7CTLO        0x42
#define PMIC_REG_GPIO1P0CTLI        0x43
#define PMIC_REG_GPIO1P1CTLI        0x44
#define PMIC_REG_GPIO1P2CTLI        0x45
#define PMIC_REG_GPIO1P3CTLI        0x46
#define PMIC_REG_GPIO1P4CTLI        0x47
#define PMIC_REG_GPIO1P5CTLI        0x48
#define PMIC_REG_GPIO1P6CTLI        0x49
#define PMIC_REG_GPIO1P7CTLI        0x4A
#define PMIC_REG_PWM0CLKDIV         0x4B
#define PMIC_REG_PWM1CLKDIV         0x4C
#define PMIC_REG_PWM0DUTYCYCLE      0x4E
#define PMIC_REG_PWM1DUTYCYCLE      0x4F
#define PMIC_REG_BACKLIGHT_EN       0x51
#define PMIC_REG_PANEL_EN           0x52
#define PMIC_REG_COLD_RESET 		0x53 
#define PMIC_REG_VDDQVTTCNT         0x55
#define PMIC_REG_V1P8ACNT           0x56
#define PMIC_REG_V1P8SXCNT          0x57
#define PMIC_REG_VDDQCNT            0x58
#define PMIC_REG_V1P2ACNT           0x59
#define PMIC_REG_V1P2SXCNT          0x5A
#define PMIC_REG_VSYSACNT           0x5B
#define PMIC_REG_VSYSSXCNT          0x5C
#define PMIC_REG_V2P85SXCNT         0x5D
#define PMIC_REG_V3P3ACNT           0x5E
#define PMIC_REG_V3P3SXCNT          0x5F
#define PMIC_REG_V5P0ACNT           0x60
#define PMIC_REG_VBUSCNT            0x61
#define PMIC_MASK_VBUSSEL          BIT01   //0 = VBUS_EN is controlled by ULPI_VBUS_EN
#define PMIC_MASK_VBUSEN           BIT00

#define PMIC_REG_VHDMICNT           0x62
#define PMIC_VR_SEL_BIT             BIT01  
#define PMIC_VR_EN_BIT              BIT00
#define PMIC_VR_LPEN_BIT            BIT02 
#define PMIC_REG_PBCONFIG1			0x6A
#define PMIC_REG_PWROKCNTL			0x6B
#define PMIC_REG_DEBUG0				0x6C
#define PMIC_REG_FWREV				0x6D
#define PMIC_REG_BATID_WIAT         0x71
#define PMIC_REG_MANCONV0           0x72
#define  PMIC_MASK_BATID            BIT01 
#define  PMIC_MASK_VBAT             BIT00

#define PMIC_REG_MANCONV1           0x73

#define PMIC_REG_SYS0_THRM_RSLTH    0x74
#define PMIC_REG_SYS0_THRM_RSLTL    0x75
#define PMIC_REG_SYS1_THRM_RSLTH    0x76
#define PMIC_REG_SYS1_THRM_RSLTL    0x77
#define PMIC_REG_SYS2_THRM_RSLTH    0x78
#define PMIC_REG_SYS2_THRM_RSLTL    0x79
#define PMIC_REG_BAT0_THRM_RSLTH    0x7A
#define PMIC_REG_BAT0_THRM_RSLTL    0x7B
#define PMIC_REG_BAT1_THRM_RSLTH    0x7C
#define PMIC_REG_BAT1_THRM_RSLTL    0x7D
#define PMIC_REG_PMIC_THRM_RSLTH    0x7E
#define PMIC_REG_PMIC_THRM_RSLTL    0x7F
#define PMIC_REG_VBATRSLTH          0x80
#define PMIC_REG_VBATRSLTL          0x81
#define PMIC_REG_BATIDRSLTH         0x82
#define   PMISC_MASK_BATID_HIGH     (BIT01|BIT00)
#define PMIC_REG_BATIDRSLTL         0x83

#define PMIC_REG_PPLTMONCTL         0x84
#define PMIC_REG_PPLTRSLTH          0x85
#define PMIC_REG_PPLTRSLTL          0x86
#define PMIC_REG_VPPLTRSLTH         0x87
#define PMIC_REG_VPPLTRSLTL         0x88
#define PMIC_REG_IPPLTRSLTH         0x89
#define PMIC_REG_IPPLTRSLTL         0x8A
#define PMIC_REG_SRCWAKECFG         0x8B
#define  PMIC_MASK_ADPWAKEEN        BIT02
#define  PMIC_MASK_USBWAKEEN        BIT01
#define  PMIC_MASK_BATWAKEEN        BIT00
#define PMIC_REG_PWRSEQCFG          0x8C
#define PMIC_REG_THRMMONCTL0        0x8E
#define  PMIC_MASK_SYSFRQS          BIT06
#define  PMIC_MASK_BATFRQS          BIT05
#define  PMIC_MASK_SYSFRQA          (BIT04|BIT03)
#define  PMIC_MASK_BATFRQA          (BIT02|BIT01)
#define  PMIC_MASK_THERM_EN         BIT00

#define PMIC_REG_THRMMONCTL1        0x8F
#define  PMIC_MASK_PMICFRQS         BIT02
#define  PMIC_MASK_PMICFRQA          (BIT01|BIT00)

#define PMIC_REG_TS_ENABLE          0x90
#define  PMIC_MASK_PMIC_EN          BIT05
#define  PMIC_MASK_BAT1_EN          BIT04
#define  PMIC_MASK_BAT0_EN          BIT03
#define  PMIC_MASK_SYS2_EN          BIT02
#define  PMIC_MASK_SYS1_EN          BIT01
#define  PMIC_MASK_SYS0_EN          BIT00
#define  PMIC_MASK_ALL_EN           (BIT05|BIT04|BIT03|BIT02|BIT01|BIT00)

#define PMIC_REG_TS__CRIT_ENABLE    0x91
#define  PMIC_MASK_PMIC_CRIT_EN     BIT05
#define  PMIC_MASK_BAT1_CRIT_EN     BIT04
#define  PMIC_MASK_BAT0_CRIT_EN     BIT03
#define  PMIC_MASK_SYS2_CRIT_EN     BIT02
#define  PMIC_MASK_SYS1_CRIT_EN     BIT01
#define  PMIC_MASK_SYS0_CRIT_EN     BIT00
#define  PMIC_MASK_ALL_EN           (BIT05|BIT04|BIT03|BIT02|BIT01|BIT00)

#define PMIC_REG_TS_A0_STATUS       0x92
#define PMIC_REG_TS_A1_STATUS       0x93
#define PMIC_REG_SYS0_THRMALRT0_H   0x94
#define PMIC_REG_SYS0_THRMALRT0_L   0x95
#define PMIC_REG_SYS0_THRMALRT1_H   0x96
#define PMIC_REG_SYS0_THRMALRT1_L   0x97
#define PMIC_REG_SYS0_THRMALRT2_H   0xC1
#define PMIC_REG_SYS0_THRMALRT2_L   0xC2
#define PMIC_REG_SYS0_THRMALRT3_H   0xC3
#define PMIC_REG_SYS0_THRMALRT3_L   0xC4
#define PMIC_REG_SYS0_THRMCRIT      0x98
#define PMIC_REG_SYS1_THRMALRT0_H   0x99
#define PMIC_REG_SYS1_THRMALRT0_L   0x9A
#define PMIC_REG_SYS1_THRMALRT1_H   0x9B
#define PMIC_REG_SYS1_THRMALRT1_L   0x9C
#define PMIC_REG_SYS1_THRMCRIT      0x9D
#define PMIC_REG_SYS1_THRMALRT2_H   0xC5
#define PMIC_REG_SYS1_THRMALRT2_L   0xC6
#define PMIC_REG_SYS1_THRMALRT3_H   0xC7
#define PMIC_REG_SYS1_THRMALRT3_L   0xC8
#define PMIC_REG_SYS2_THRMALRT0_H   0x9E
#define PMIC_REG_SYS2_THRMALRT0_L   0x9F
#define PMIC_REG_SYS2_THRMALRT1_H   0xA0
#define PMIC_REG_SYS2_THRMALRT1_L   0xA1
#define PMIC_REG_SYS2_THRMCRIT      0xA2
#define PMIC_REG_SYS2_THRMALRT2_H   0xC9
#define PMIC_REG_SYS2_THRMALRT2_L   0xCA
#define PMIC_REG_SYS2_THRMALRT3_H   0xCB
#define PMIC_REG_SYS2_THRMALRT3_L   0xCC
#define PMIC_REG_BAT0_THRMALRT0_H   0xA3
#define PMIC_REG_BAT0_THRMALRT0_L   0xA4
#define PMIC_REG_BAT0_THRMALRT1_H   0xA5
#define PMIC_REG_BAT0_THRMALRT1_L   0xA6
#define PMIC_REG_BAT0_THRMCRIT_H    0xA7
#define PMIC_REG_BAT0_THRMCRIT_L    0xA8
#define PMIC_REG_BAT0_THRMALRT2_H   0xCD
#define PMIC_REG_BAT0_THRMALRT2_L   0xCE
#define PMIC_REG_BAT0_THRMALRT3_H   0xCF
#define PMIC_REG_BAT0_THRMALRT3_L   0xD0
#define PMIC_REG_BAT1_THRMALRT0_H   0xA9
#define PMIC_REG_BAT1_THRMALRT0_L   0xAA
#define PMIC_REG_BAT1_THRMALRT1_H   0xAB
#define PMIC_REG_BAT1_THRMALRT1_L   0xAC
#define PMIC_REG_BAT1_THRMCRIT_H    0xAD
#define PMIC_REG_BAT1_THRMCRIT_L    0xAE
#define PMIC_REG_BAT1_THRMALRT2_H   0xD1
#define PMIC_REG_BAT1_THRMALRT2_L   0xD2
#define PMIC_REG_BAT1_THRMALRT3_H   0xD3
#define PMIC_REG_BAT1_THRMALRT3_L   0xD4
#define PMIC_REG_PMIC_THRMALRT0_H   0xAF
#define PMIC_REG_PMIC_THRMALRT0_L   0xB0
#define PMIC_REG_PMIC_THRMALRT1_H   0xB1
#define PMIC_REG_PMIC_THRMALRT1_L   0xB2
#define PMIC_REG_PMIC_THRMCRIT      0xB3
#define PMIC_REG_PMIC_THRMALRT2_H   0xD5
#define PMIC_REG_PMIC_THRMALRT2_L   0xD6
#define PMIC_REG_PMIC_THRMALRT3_H   0xD7
#define PMIC_REG_PMIC_THRMALRT3_L   0xD8
#define PMIC_REG_VWARNA_CFG         0xB4
#define  PMIC_MASK_VWARNA_EN        BIT03
#define PMIC_REG_VWARNB_CFG         0xB5
#define  PMIC_MASK_VWARNB_EN        BIT03
#define PMIC_REG_VCRIT_CFG          0xB6
#define  PMIC_MASK_VCRITSDWNEN      BIT04
#define  PMIC_MASK_VCRIT_EN         BIT03
#define PMIC_REG_BCUDISA_BEH        0xB7
#define  PMIC_MASK_STICKY           BIT02   //0--output signal assertion is not sticky
#define  PMIC_MASK_PIN_POL          BIT01   //0--active low
#define  PMIC_MASK_EN               BIT00   // 0-disable behavivor

#define PMIC_REG_BCUDISB_BEH        0xB8
#define PMIC_REG_BCUDISCRIT_BEH     0xB9
#define PMIC_REG_BCUPROCHOT_B_BEH   0xBA
#define PMIC_REG_SBCUIRQ            0xBB
#define PMIC_REG_SBCUCTRL           0xBC
#define PMIC_REG_TS_CRIT_ST         0xBD

#define PMIC_REG_TS_A2_STATUS       0xBF
#define PMIC_REG_TS_A3_STATUS       0xC0

#define PMIC_REG_ALERT0LOCK         0xC5
#define  PMIC_MASK_A0LOCK           BIT00
#define PMIC_REG_VREFDQ1CNT         0xC6

#define PMIC_REG_THRMIRQ3           0xD9
#define PMIC_REG_MTHRMIRQ3          0xDA
#define PMIC_REG_UIBCONFIG          0xDB
#define PMIC_REG_PSOCMONCTL         0xDC
#define PMIC_REG_PSOCRSLTH          0xDD
#define PMIC_REG_PSOCRSLTL          0xDE
#define PMIC_REG_VSOCRSLTH          0xDF
#define PMIC_REG_VSOCRSLTL          0xE0
#define PMIC_REG_ISOCRSLTH          0xE1
#define PMIC_REG_ISOCRSLTL          0xE2
#define PMIC_REG_SOC_PALERTH        0xE3
#define PMIC_REG_SOC_PALERTL        0xE4

//Changes specific to Dialog PMIC
#define PMIC_REG_PWRSEQCFG_DIALOG_B0   0xE0
#define PMIC_REG_SRCWAKECFG_DIALOG_B0  0xDF
#define  PMIC_MASK_ILIM500          BIT04              // choose the current limit source for the external charger. 1 = 500mA USB charger available 0 = 100mA ..
#define  PMIC_MASK_VCCPWRCFG        BIT03              // de-assert VCCAPWROK when PMIC enters SOC S0IX state.
#define  PMIC_MASK_SUSPWRDNNACK     BIT02              // when SLP_S3_B=0 it decides if the power sequencer should go to SOC_G3 if SUSPWRDNNACK is asserted
#define  PMIC_MASK_DTPWROK          (BIT01|BIT00)      // The delay time for Core rails valid to VCCAPWEROK and COREPWROK assertion 00-1ms 01-10ms 10-100ms 11-120ms

#endif
