/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _DC_X_PMIC_REG_CC_H_
#define _DC_X_PMIC_REG_CC_H_

#define    BIT00    0x01
#define    BIT01    0x02
#define    BIT02    0x04
#define    BIT03    0x08
#define    BIT04    0x10
#define    BIT05    0x20
#define    BIT06    0x40
#define    BIT07    0x80

#define    DC_X_PMIC_REG_PWR_SRC_STS             0x00
#define      DC_X_MAST_BAT_CURR_DIR              BIT2  //Indication Battery current direction
#define      DC_X_MAST_VBUS_PRESENCE             BIT5  //VBUS presence indication

#define    DC_X_PMIC_REG_CHARGER_STS             0x01
#define      DC_X_MAST_BAT_PRESENCE              BIT5  //Battery presence indication

#define    DC_X_PMIC_REG_PUPD_REASON             0x02
#define      DC_X_MAST_PWRBTN_START              BIT0  //Power on key was the start up reason
#define      DC_X_MAST_CHR_INERT_START           BIT1  //Charger insertion was the start up reason
#define      DC_X_MAST_BAT_INERT_START           BIT2  //Battery insertion was the start up reason
#define      DC_X_MAST_SOC_GLOBRST               BIT3  //SOC initiated Global Reset was the start up reason
#define      DC_X_MAST_COLDRST                   BIT4  //Cold reset was the start up reaso
#define      DC_X_MAST_UVLO_SHDN                 BIT5  //PMIC UVLO threshold was the shutdown reason
#define      DC_X_MAST_SOC_SHDW                  BIT6  //SOC initiated cold off was the shutdown reason
#define      DC_X_MAST_PWRBTN_SHDW               BIT7  //Power on key override was the shutdown reason

#define    DC_X_PMIC_REG_IC_TYPC                 0x03
#define     PMIC_DEV_MASK_VERSION                (BIT05|BIT04)   
#define     PMIC_DEV_MASK_VENDER                 (BIT07|BIT06|BIT03|BIT02|BIT01|BIT00)  

#define    DC_X_PMIC_REG_DATA_BUFFER_04          0x04
#define    DC_X_PMIC_REG_DATA_BUFFER_05          0x05
#define    DC_X_PMIC_REG_DATA_BUFFER_06          0x06
#define    DC_X_PMIC_REG_DATA_BUFFER_07          0x07
#define    DC_X_PMIC_REG_DATA_BUFFER_08          0x08
#define    DC_X_PMIC_REG_DATA_BUFFER_09          0x09
#define    DC_X_PMIC_REG_DATA_BUFFER_0A          0x0A
#define    DC_X_PMIC_REG_DATA_BUFFER_0B          0x0B
#define    DC_X_PMIC_REG_DATA_BUFFER_0C          0x0C
#define    DC_X_PMIC_REG_DATA_BUFFER_0D          0x0D
#define    DC_X_PMIC_REG_DATA_BUFFER_0E          0x0E
#define    DC_X_PMIC_REG_DATA_BUFFER_0F          0x0F
#define    DC_X_PMIC_REG_DATA_BUFFER_F2          0xF2
#define    DC_X_PMIC_REG_DATA_BUFFER_F4          0xF4
#define    DC_X_PMIC_REG_DATA_BUFFER_FF          0xFF

#define    DC_X_PMIC_REG_VR_CTRL_1               0x10
#define    DC_X_PMIC_REG_VR_CTRL_2               0x12
#define     DC_X_MASK_DLDO4_CTRL                 BIT06  //DLDO4 on©\off control  0©\off; 1©\on
#define     DC_X_MASK_DLDO3_CTRL                 BIT05  //DLDO3 on©\off control
#define     DC_X_MASK_DLDO2_CTRL                 BIT04  //DLDO2 on©\off control
#define     DC_X_MASK_DLDO1_CTRL                 BIT03  //DLDO1 on©\off control
#define     DC_X_MASK_ELDO3_CTRL                 BIT02  //ELDO3 on©\off control
#define     DC_X_MASK_ELDO2_CTRL                 BIT01  //ELDO2 on©\off control
#define     DC_X_MASK_ELDO1_CTRL                 BIT00  //ELDO1 on©\off control

#define    DC_X_PMIC_REG_VR_CTRL_3               0x13
#define     DC_X_MASK_ALDO3_CTRL                 BIT07  //ALDO3 on©\off control  0©\off; 1©\on
#define     DC_X_MASK_ALDO2_CTRL                 BIT06  //ALDO2 on©\off control 
#define     DC_X_MASK_ALDO1_CTRL                 BIT05  //ALDO1 on©\off control
#define     DC_X_MASK_FLDO3_CTRL                 BIT04  //FLDO2 on©\off control
#define     DC_X_MASK_FLDO2_CTRL                 BIT03  //FLDO1 on©\off control
#define     DC_X_MASK_FLDO1_CTRL                 BIT02  //FLDO3 on©\off control

#define    DC_X_PMIC_REG_VR_CTRL_SYNC            0x14
#define     DC_X_MASK_BUCK5_POLY_PHASE           BIT06  //BUCK5 poly©\phase control
#define     DC_X_MASK_BUCK3_$_POLY_PHASE         BIT05  //BUCK 3 & 4 change to poly©\phase Buck
#define     DC_X_MASK_BUCK235_VRUN_VSLP          BIT04  //Select the BUCK2/ 3 /5 Vrun register or Vsleep register
#define     DC_X_MASK_COLD_RESET_CONDITION       BIT03  //If SLP_S0IX_B go high and PLTRST_B status is low for 512ms, PMIC will do a cold reset or not (Reset: power on reset)
#define     DC_X_MASK_COLD_RESET_ENABLE          BIT02  //Cold reset Enable . All power rails power down and then power up,64ms delay
#define     DC_X_MASK_POWER_CONTROL_SEL          BIT01  //1©\select buffer register, output value of control register to buffer
#define     DC_X_MASK_OUTPUT_BUFFER              BIT00  //1©\outport to control register from buffer Bit[1:0], self clear to 0 after output

#define    DC_X_PMIC_REG_VR_CTRL_DLDO1           0x15
#define    DC_X_PMIC_REG_VR_CTRL_DLDO2           0x16
#define    DC_X_PMIC_REG_VR_CTRL_DLDO3           0x17
#define    DC_X_PMIC_REG_VR_CTRL_DLDO4           0x18
#define    DC_X_PMIC_REG_VR_CTRL_ELDO1           0x19
#define    DC_X_PMIC_REG_VR_CTRL_ELDO2           0x1A
#define    DC_X_PMIC_REG_VR_CTRL_ELDO3           0x1B 
#define     DC_X_MASK_ELDO_VOL_18                (BIT04|BIT02|BIT01) 

#define    DC_X_PMIC_REG_VR_CTRL_FLDO1           0x1C
#define    DC_X_PMIC_REG_VR_CTRL_FLDO2_3         0x1D

#define    DC_X_PMIC_REG_VR_CTRL_BUCK6           0x20
#define    DC_X_PMIC_REG_VR_CTRL_BUCK5           0x21
#define    DC_X_PMIC_REG_VR_CTRL_BUCK1           0x23
#define    DC_X_PMIC_REG_VR_CTRL_BUCK4           0x24
#define     DC_X_MASK_DVM_STATUS                 BIT07   //DVM finished or not status bit  1: finished
#define     DC_X_MASK_VOL_8BIT_6_0              (BIT00|BIT01|BIT02|BIT03|BIT04|BIT05|BIT06) //voltage setting Bit 6©\0  0.80©\1.12V£º10mV/step   1.14©\1.84V£º20mV/step

#define    DC_X_PMIC_REG_VR_CTRL_BUCK3           0x25
#define    DC_X_PMIC_REG_VR_CTRL_BUCK2           0x26
#define    DC_X_PMIC_REG_VR_BUCK_RAMP_CTRL       0x27
#define     DC_X_MASK_BUCK2_DVM_CTRL             BIT07  //BUCK2 DVM on©\off control
#define     DC_X_MASK_BUCK3_DVM_CTRL             BIT06  //BUCK3 DVM on©\off control
#define     DC_X_MASK_BUCK4_DVM_CTRL             BIT05  //BUCK4 DVM on©\off control
#define     DC_X_MASK_BUCK1_DVM_CTRL             BIT04  //BUCK1 DVM on©\off control
#define     DC_X_MASK_BUCK5_DVM_CTRL             BIT02  //BUCK5 DVM on©\off control
#define     DC_X_MASK_RSMRST_B_LOW               BIT01  //RSMRST_B drive low when ALDO3 less than 85% or not control
#define     DC_X_MASK_DRAMPWROK                  BIT00  //DRAMPWROK drive low when FLDO3 less than 85% or not control          

#define    DC_X_PMIC_REG_ALDO1_VOL_CTRL          0x28
#define     DC_X_MASK_ALDO_VOL_30                (BIT04|BIT02|BIT01|BIT00) 
#define     DC_X_MASK_ALDO_VOL_29                (BIT04|BIT02|BIT01) 
#define     DC_X_MASK_ALDO_VOL_28                (BIT04|BIT02|BIT00) 

#define    DC_X_PMIC_REG_ALDO2_VOL_CTRL          0x29
#define    DC_X_PMIC_REG_ALDO3_VOL_CTRL          0x2A
#define    DC_X_PMIC_REG_BC_GBL_DETECT           0x2C
#define    DC_X_PMIC_REG_BC_VBUS_CTRL_STS        0x2D
#define    DC_X_PMIC_REG_BC_USB_STATUS           0x2E
#define    DC_X_PMIC_REG_BC_DETECT_STS           0x2F

#define    DC_X_PMIC_REG_VBUS_PATH_CTRL          0x30
#define    DC_X_PMIC_REG_WAKE_UP_CTRL            0x31
#define    DC_X_PMIC_REG_PD_BAT_CHGLED           0x32
#define     DC_X_MASK_BAT_DETECT                 BIT06         //Battery detection function control: 0©\disable; 1©\enable
#define     DC_X_MASK_CHGLED_PIN                 (BIT05|BIT04) //CHGLED pin control
#define     DC_X_MASK_CHGLED_PIN_HIZ             0x0           //00: Hi-Z
#define     DC_X_MASK_CHGLED_PIN_05HZ            0x10          //01: 25% 0.5Hz toggle
#define     DC_X_MASK_CHGLED_PIN_2HZ             0x20          //10: 25% 2Hz toggle
#define     DC_X_MASK_CHGLED_PIN_LOW             0x30          //11: drive low
#define     DC_X_MASK_CHGLED_PIN_CTRL            BIT03         //0: controlled by REG 32H[5:4]
#define     DC_X_MASK_DELAY_TIME                 (BIT01|BIT00) //control bit for Delay time between PWROK signal and power good time
#define     DC_X_MASK_DELAY_TIME_8MS             0x0
#define     DC_X_MASK_DELAY_TIME_16MS            0x1
#define     DC_X_MASK_DELAY_TIME_32MS            0x2
#define     DC_X_MASK_DELAY_TIME_64MS            0x3

#define    DC_X_PMIC_REG_CHARGER_CTRL_1          0x33
#define    DC_X_PMIC_REG_CHARGER_CTRL_2          0x34
#define     DC_X_MASK_PRE_CHARGE_TIME            (BIT07|BIT06)  //Pre©\charge Timer length setting 1:0
#define     DC_X_MASK_PRE_CHARGE_TIME_40MIN      0x0            //40 minutes
#define     DC_X_MASK_PRE_CHARGE_TIME_50MIN      BIT06          //50 minutes
#define     DC_X_MASK_PRE_CHARGE_TIME_60MIN      BIT07          //60 minutes
#define     DC_X_MASK_PRE_CHARGE_TIME_70MIN      (BIT07|BIT06)  //70 minutes
#define     DC_X_MASK_CRG_ON_OFF                 BIT05          //Charger output turn off or not when charging is end & the PMIC is on state      1: do not turn off
#define     DC_X_MASK_CHGLED_SEL                 BIT04          //CHGLED Type select when REG 32_[3] is 0 1: Type B
#define     DC_X_MASK_FAST_CHARGE_TIME           (BIT01|BIT00)  //Fast charge maximum time setting1:0
#define     DC_X_MASK_FAST_CHARGE_TIME_6HRS      0x0            //6 hours
#define     DC_X_MASK_FAST_CHARGE_TIME_8HRS      0x1            //8 hours
#define     DC_X_MASK_FAST_CHARGE_TIME_10HRS     0x2            //10 hours
#define     DC_X_MASK_FAST_CHARGE_TIME_12HRS     0x3            //12 hours

#define    DC_X_PMIC_REG_CHARGER_CTRL_3          0x35
#define    DC_X_PMIC_REG_POK_SETTING             0x36
#define     DC_X_MASK_ONLEVEL                    (BIT06|BIT07)   //ONLEVEL setting
#define     DC_X_MASK_ONLEVEL_128MS              0               // 128ms
#define     DC_X_MASK_ONLEVEL_1S                 BIT06           // 1s
#define     DC_X_MASK_ONLEVEL_2S                 BIT07           // 2s
#define     DC_X_MASK_ONLEVEL_3S                 (BIT06|BIT07)   // 3s
#define     DC_X_MASK_IRQLEVEL                   (BIT04|BIT05)   //IRQLEVEL setting
#define     DC_X_MASK_IRQLEVEL_1S                0               //1s
#define     DC_X_MASK_IRQLEVEL_1_5S              BIT04           //1.5s
#define     DC_X_MASK_IRQLEVEL_2S                BIT05           //2s             
#define     DC_X_MASK_IRQLEVEL_2_5S              (BIT04|BIT05)   //2.5s
#define     DC_X_MASK_OFFEVEL                    (BIT00|BIT01)   //OFFLEVEL setting
#define     DC_X_MASK_OFFEVEL_4S                 0  
#define     DC_X_MASK_OFFEVEL_6S                 1  
#define     DC_X_MASK_OFFEVEL_8S                 2  
#define     DC_X_MASK_OFFEVEL_10S                3
#define     DC_X_MASK_AUTO_ON_OFFEVEL            BIT02
#define     DC_X_MASK_SHDW_OFFEVEL               BIT03   //Enable bit of the function which will shut down the PMIC when POK is larger than OFFLEVEL

#define    DC_X_PMIC_REG_POK_POFF_SETTING        0x37
#define     DC_X_MASK_POK_POFF                   (BIT00|BIT01|BIT02)   //Power off activity time setting
#define     DC_X_MASK_POK_POFF_0S                0
#define     DC_X_MASK_POK_POFF_10S               1            
#define    DC_X_PMIC_REG_VLTF_CHARGE_SETTING     0x38
#define    DC_X_PMIC_REG_VHTF_CHARGE_SETTING     0x39
#define    DC_X_PMIC_REG_BUCK_FREQ_SETTING       0x3B
#define    DC_X_PMIC_REG_VLTF_WORK_SETTING       0x3C
#define    DC_X_PMIC_REG_VHTF_WORK_SETTING       0x3D

#define    DC_X_PMIC_REG_IRQ_CTRL_1              0x40
#define     DC_X_MASK_VBUS_HI_TO_LOW_EN          BIT02   //VBUS from high go low IRQ enable
#define     DC_X_MASK_VBUS_LOW_TO_HI_EN          BIT03   //VBUS from low go high IRQ enable
#define     DC_X_MASK_VBUS_OV_VOL_EN             BIT04   //VBUS over voltage IRQ enable
#define     DC_X_MASK_VBUS_HI_TO_LOW_EN_1        BIT05   //VBUS from high go low IRQ enable
#define     DC_X_MASK_VBUS_LOW_TO_HI_EN_1        BIT06   //VBUS from low go high IRQ enable
#define     DC_X_MASK_VBUS_OV_VOL_EN_1           BIT07   //VBUS over voltage IRQ enable

#define    DC_X_PMIC_REG_IRQ_CTRL_2              0x41
#define    DC_X_PMIC_REG_IRQ_CTRL_3              0x42
#define    DC_X_PMIC_REG_IRQ_CTRL_4              0x43
#define     DC_X_MASK_PMIC_TEMP                  BIT07   //The PMIC temperature over the warning level 2 IRQ (OTIRQ) enable
#define     DC_X_MASK_GPADC                      BIT02   //GPADC(GPIO0) ADC convert finished IRQ enable
#define     DC_X_MASK_WARN_LEVEL1                BIT01   //Enable bit for IRQ which indicate battery capacity ratio being lower than warning level1
#define     DC_X_MASK_WARN_LEVEL2                BIT00   //Enable bit for IRQ which indicate battery capacity ratio being lower than warning level2

#define    DC_X_PMIC_REG_IRQ_CTRL_5              0x44
#define    DC_X_PMIC_REG_IRQ_CTRL_6              0x45
#define    DC_X_PMIC_REG_IRQ_STS_1               0x48
#define     DC_X_MASK_VBUS_HI_TO_LOW             BIT02   //VBUS from high go low IRQ 
#define     DC_X_MASK_VBUS_LOW_TO_HI             BIT03   //VBUS from low go high IRQ 
#define     DC_X_MASK_VBUS_OV_VOL                BIT04   //VBUS over voltage IRQ 

#define    DC_X_PMIC_REG_IRQ_STS_2               0x49
#define     DC_X_MASK_BAT_APPEND                 BIT07   //Battery append IRQ
#define     DC_X_MASK_BAT_CHARGE_DONE            BIT02   //Battery charge done IRQ

#define    DC_X_PMIC_REG_IRQ_STS_3               0x4A
#define    DC_X_PMIC_REG_IRQ_STS_4               0x4B
#define    DC_X_PMIC_REG_IRQ_STS_5               0x4C
#define    DC_X_MASK_PMIC_EVENT_IRQ_STS          BIT07
#define    DC_X_MASK_PMIC_POKPIRQ_STS            BIT06
#define    DC_X_MASK_PMIC_POKNIRQ_STS            BIT05
#define    DC_X_MASK_PMIC_POKSIRQ_STS            BIT04
#define    DC_X_MASK_PMIC_POKLIRQ_STS            BIT03
#define    DC_X_MASK_PMIC_POKOIRQ_STS            BIT02
#define    DC_X_MASK_PMIC_GPIO1_IRQ_STS          BIT01
#define    DC_X_MASK_PMIC_GPIO0_IRQ_STS          BIT00

#define    DC_X_PMIC_REG_IRQ_STS_6               0x4D

#define    DC_X_PMIC_REG_TS_PIN_INPUT_HI         0x58
#define    DC_X_PMIC_REG_TS_PIN_INPUT_LO         0x59
#define    DC_X_PMIC_REG_GPADC_INPUT_HI          0x5A
#define    DC_X_PMIC_REG_GPADC_INPUT_LO          0x5B

#define    DC_X_PMIC_REG_BAT_VOL_HI              0x78
#define    DC_X_PMIC_REG_BAT_VOL_LO              0x79
#define    DC_X_PMIC_REG_BAT_CHG_CURRENT_HI      0x7A
#define    DC_X_PMIC_REG_BAT_CHG_CURRENT_LO      0x7B
#define    DC_X_PMIC_REG_BAT_DISCHG_CURRENT_HI   0x7C
#define    DC_X_PMIC_REG_BAT_DISCHG_CURRENT_LO   0x7D

#define    DC_X_PMIC_REG_BUCK_MODE_SEL           0x80
#define     DC_X_MASK_BUCK2_PFM_PWM_CTRL         BIT06   //BUCK2 PFM/PWM control: 0: auto switch 1: always PWM
#define     DC_X_MASK_BUCK3_PFM_PWM_CTRL         BIT05   //BUCK3 PFM/PWM control: 0: auto switch 1: always PWM
#define     DC_X_MASK_BUCK4_PFM_PWM_CTRL         BIT04   //BUCK4 PFM/PWM control: 0: auto switch 1: always PWM
#define     DC_X_MASK_BUCK1_PFM_PWM_CTRL         BIT03   //BUCK1 PFM/PWM control: 0: auto switch 1: PSM/PWM
#define     DC_X_MASK_BUCK5_PFM_PWM_CTRL         BIT01   //BUCK5 PFM/PWM control: 0: auto switch 1: PSM/PWM
#define     DC_X_MASK_BUCK6_PFM_PWM_CTRL         BIT00   //BUCK6 PFM/PWM control: 0: auto switch 1: always PWM

#define    DC_X_PMIC_REG_OUTPUT_MON_CTRL         0x81
#define    DC_X_PMIC_REG_ADC_ENABLE              0x82
#define     DC_X_MASK_GPIO0_ADC_ENABLE           BIT04   //GPIO0 ADC enable

#define    DC_X_PMIC_REG_ADC_CTRL                0x84
#define     DC_X_MASK_GPIO0_PIN_CTRL            (BIT07|BIT06)   //Current source from GPIO0 pin control
#define     DC_X_MASK_GPIO0_PIN_CTRL_20UA       (0x0)           //20uA
#define     DC_X_MASK_GPIO0_PIN_CTRL_40UA       (BIT06)         //40uA
#define     DC_X_MASK_GPIO0_PIN_CTRL_60UA       (BIT07)         //60uA
#define     DC_X_MASK_GPIO0_PIN_CTRL_80UA       (BIT07|BIT06)   //80uA

#define    DC_X_PMIC_REG_ADC_SPEED_SETTING       0x85
#define     DC_X_MASK_GPIO0_ADC_WORK_MODE       (BIT02)   //GPIO0 ADC work mode 1-outout current

#define    DC_X_PMIC_REG_TIMER_CTRL              0x8A
#define    DC_X_PMIC_REG_BUCK_OUTPUT_MON         0x8E
#define    DC_X_PMIC_REG_IRQ_HOT_OVSHDN          0x8F
#define    DC_X_MASK_POK16S_RESET_EN             BIT03
#define    DC_X_MASK_OVTEMP_SHWD_EN              BIT02
#define    DC_X_MASK_VOL_RECOVERY_EN             BIT01

#define    DC_X_PMIC_REG_GPIO0_CTRL              0x90
#define     DC_X_MASK_GPIO0_PIN_FUNC_CTRL       (BIT02|BIT01|BIT00)   //ADC input mode

#define    DC_X_PMIC_REG_GPIO0_VOLTAGE           0x91
#define    DC_X_PMIC_REG_GPIO1_CTRL              0x92
#define    DC_X_PMIC_REG_GPIO1_VOLTAGE           0x93
#define    DC_X_PMIC_REG_GPIO_STATUS             0x94
#define    DC_X_PMIC_REG_GPIO_CTRL               0x97
#define    DC_X_PMIC_REG_WAKEUP_SEQUENCE1        0x9A

#define    DC_X_PMIC_REG_WAKEUP_SEQUENCE2        0x9B
#define    DC_X_PMIC_REG_SLEEP_SEQUENCE1         0x9C
#define    DC_X_PMIC_REG_SLEEP_SEQUENCE2         0x9D
#define    DC_X_PMIC_REG_VR_SLEEP_STATE          0x9E
#define     DC_X_MASK_BUCK2_SLP_S0IX_B           BIT04   //When BUCK2 controlled by SLP_S0IX_B, power state in sleep mode: 1:Vsleep
#define     DC_X_MASK_BUCK3_SLP_S0IX_B           BIT03   //When BUCK3 controlled by SLP_S0IX_B, power state in sleep mode: 1:Vsleep
#define     DC_X_MASK_BUCK1_SLP_S0IX_B           BIT02   //When BUCK1 controlled by SLP_S0IX_B, power state in sleep mode: 1:Vsleep
#define     DC_X_MASK_BUCK5_SLP_S0IX_B           BIT01   //When BUCK5 controlled by SLP_S0IX_B, power state in sleep mode: 1:Vsleep
#define     DC_X_MASK_FLDO1_SLP_S0IX_B           BIT00   //When FLDO1 controlled by SLP_S0IX_B, power state in sleep mode: 1:Vsleep

#define    DC_X_PMIC_REG_RT_BAT_VOL_HI           0xA0
#define    DC_X_PMIC_REG_RT_BAT_VOL_LO           0xA1

#define    DC_X_PMIC_REG_FUEL_GARGE_CTRL         0xB8
#define    DC_X_PMIC_REG_BAT_CAP_PERCENT         0xB9
#define    DC_X_PMIC_REG_RDC1                    0xBA
#define    DC_X_PMIC_REG_RDC0                    0xBB
#define    DC_X_PMIC_REG_OCV1                    0xBC
#define    DC_X_PMIC_REG_OCV0                    0xBD
#define    DC_X_PMIC_REG_BAT_MAX_CAP_HI          0xE0
#define    DC_X_PMIC_REG_BAT_MAX_CAP_LO          0xE1
#define    DC_X_PMIC_REG_CLB_M_COUNTER_HI        0xE2
#define    DC_X_PMIC_REG_CLB_M_COUNTER_LO        0xE3
#define    DC_X_PMIC_REG_OCV_PERCENT_BAT_CAP     0xE4
#define    DC_X_PMIC_REG_CLB_M_PERCENT_BAT_CAP   0xE5
#define    DC_X_PMIC_REG_BAT_CAP_PERCENT_W       0xE6
#define    DC_X_PMIC_REG_FUEL_GARGE_T_CTRL_0     0xE8
#define    DC_X_PMIC_REG_FUEL_GARGE_T_CTRL_1     0xE9
#define    DC_X_PMIC_REG_FUEL_GARGE_T_CTRL_2     0xEA
#define    DC_X_PMIC_REG_FUEL_GARGE_T_CTRL_3     0xEB
#define    DC_X_PMIC_REG_FUEL_GARGE_T_CTRL_4     0xEC
#define    DC_X_PMIC_REG_FUEL_GARGE_T_CTRL_5     0xED

#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C0		 0xC0
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C1		 0xC1
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C2		 0xC2
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C3		 0xC3
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C4		 0xC4
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C5		 0xC5
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C6		 0xC6
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C7		 0xC7
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C8		 0xC8
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_C9		 0xC9
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_CA		 0xCA
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_CB		 0xCB
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_CC		 0xCC
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_CD		 0xCD
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_CE		 0xCE
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_CF		 0xCF
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D0		 0xD0
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D1		 0xD1
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D2		 0xD2
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D3		 0xD3
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D4		 0xD4
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D5		 0xD5
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D6		 0xD6
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D7		 0xD7
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D8		 0xD8
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_D9		 0xD9
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_DA		 0xDA
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_DB		 0xDB
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_DC		 0xDC
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_DD		 0XDD
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_DE		 0xDE
#define	   DC_X_PMIC_REG_FUEL_GAUGE_CHAR_DF		 0xDF

#if 0
//DollarCove XPOWER FG Characterization data Provided by FG Mfg
//This Registers 0xC0-0xDF is OCV Percentage Table ( Refer AXP288 Datasheet for more detail )
//Charcterizaiton Start ----->
#define XPOWER_FG_OCV_C0	0x00
#define XPOWER_FG_OCV_C1	0x00
#define XPOWER_FG_OCV_C2	0x00
#define XPOWER_FG_OCV_C3	0x00
#define XPOWER_FG_OCV_C4	0x01
#define XPOWER_FG_OCV_C5	0x01
#define XPOWER_FG_OCV_C6	0x02
#define XPOWER_FG_OCV_C7	0x02
#define XPOWER_FG_OCV_C8	0x02
#define XPOWER_FG_OCV_C9	0x03
#define XPOWER_FG_OCV_CA	0x03
#define XPOWER_FG_OCV_CB	0x04
#define XPOWER_FG_OCV_CC	0x0C
#define XPOWER_FG_OCV_CD	0x10
#define XPOWER_FG_OCV_CE	0x16
#define XPOWER_FG_OCV_CF	0x1C
#define XPOWER_FG_OCV_D0	0x27
#define XPOWER_FG_OCV_D1	0x2C
#define XPOWER_FG_OCV_D2	0x30
#define XPOWER_FG_OCV_D3	0x35
#define XPOWER_FG_OCV_D4	0x3A
#define XPOWER_FG_OCV_D5	0x3F
#define XPOWER_FG_OCV_D6	0x43
#define XPOWER_FG_OCV_D7	0x47
#define XPOWER_FG_OCV_D8	0x4B
#define XPOWER_FG_OCV_D9	0x4E
#define XPOWER_FG_OCV_DA	0x50
#define XPOWER_FG_OCV_DB	0x51
#define XPOWER_FG_OCV_DC	0x54
#define XPOWER_FG_OCV_DD	0x57
#define XPOWER_FG_OCV_DE	0x5B
#define XPOWER_FG_OCV_DF	0x5E

#define BATTERY_MAX_CAP_HI	0x8D
#define BATTERY_MAX_CAP_LO	0xA3
#define	BATTERY_RDC1		0xC0
#define BATTERY_RDC0		0x97

//Charcterizaiton Table End <-----------
#endif

#endif

