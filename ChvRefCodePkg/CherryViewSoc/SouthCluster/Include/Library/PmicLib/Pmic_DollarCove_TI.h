/** @file

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PMIC_REG_TIDC_P_H_
#define _PMIC_REG_TIDC_P_H_

#define  PMIC_DEV_TI         (0x3<<6)    //b'11
#define  PMIC_TI_MAJOR_PG10  (0x0<<3)
#define  PMIC_TI_MAJOR_PG20  (0x1<<3)

#define PMIC_TI_I2C_SLAVE_ADDR_1   0x5C //7bits address
#define PMIC_TI_I2C_SLAVE_ADDR_2   0x5D
#define PMIC_TI_I2C_SLAVE_ADDR_3   0x5E  //default from SPEC
#define PMIC_TI_I2C_SLAVE_ADDR_4   0x5F

#define PMIC_TI_I2C_1_SLAVE_ADDR_0  0x6B
#define PMIC_TI_I2C_1_SLAVE_ADDR_1  0x68

#define PMIC_TI_I2C_BUS_NO         0x2

#define TI_POWERSRC_AND_CHARGER_STATUS 0x12

#define USB3750_ADDR              0x68
#define USB3750_CHRG_TYPE_REG     0x0

#define TI_BATTERY_BPTH_VALID     0x380

//Charger
#define CHG_TI_BQ24296_REG0			0x00
#define CHG_TI_BQ24296_REG1			0x01
#define CHG_TI_BQ24296_REG2			0x02
#define CHG_TI_BQ24296_REG3			0x03
#define CHG_TI_BQ24296_REG4			0x04
#define CHG_TI_BQ24296_REG5			0x05
#define CHG_TI_BQ24296_REG6			0x06
#define CHG_TI_BQ24296_REG7			0x07
#define CHG_TI_BQ24296_REG8			0x08
#define CHG_TI_BQ24296_REG9			0x09
#define CHG_TI_BQ24296_REG10		0x0A
//FuelGauge
#define TI_BATT_CAPACITY			5100
#define REG_TI_MIRQ_ADDR			0x02		//MIRQ Register
#define REG_TI_ADCCTRL_ADDR			0x50		//ADC Control Register
#define REG_TI_VBATLO_ADDR			0x55		//VBATLO 
#define REG_TI_VBATHI_ADDR			0x54		//VBATHigh
#define REG_TI_DITEMPHI_ADDR		0x56		//Die Temp High
#define REG_TI_BPTEMPHI_ADDR		0x58		//BP Temp High
#define REG_TI_GPTEMPHI_ADDR		0x5A		//GP ADC High
#define REG_TI_IRQ_ADDR				0x01		//IRQ Register
#define	REG_TI_BPTHREMHI			0x58		//BPTHERM High
#define REG_TI_BPTHERMLO			0x59		//BPTHERM Low
#define REG_TI_CC_CTRL				0x60		//CC Control

#define REG_TI_CC_ACC_BYT0			0x66		//CC accumulator Reg.
#define REG_TI_CC_ACC_BYT1			0x65		//CC accumulator Reg.
#define REG_TI_CC_ACC_BYT2			0x64		//CC accumulator Reg.
#define REG_TI_CC_ACC_BYT3			0x63		//CC accumulator Reg.

#define REG_TI_CC_SMPL_BYT0			0x69		//CC Sample counter BYTE
#define REG_TI_CC_SMPL_BYT1			0x68		//CC Sample counter BYTE
#define REG_TI_CC_SMPL_BYT2			0x67		//CC Sample counter BYTE

#define REG_TI_CC_INTG1_BYT1		0x6A		//CC integrator Register MSB (0:5,6-7 = rsvd)
#define REG_TI_CC_INTG1_BYT0		0x6B		//CC integrator Register LSB

#define EEPROM_ACCESS_CTRL			0x88
#define EEPROM_REG20				0xF3		// 4:7 : CC offset 
#define EEPROM_GAIN_REG				0xF4		// 4:7 : CC gain
#define OFFSET_REG_TRIM_REV_3		0xFD
#define TRIM_REV_3_OFFSET_STEP		1
#define DEFAULT_CC_OFFSET_STEP		2
#define DEFAULT_CC_OFFSET_SHIFT		0 
#define TRIM_REV_3_OFFSET_SHIFT		1

#define EEPROM_CTRL					0xFE
#define EEPROM_CTRL_EEPSEL			0x3
#define EEPROM_CTRL_EEPSEL_MASK		0x03
#define EEPROM_BANK0_SEL			0x01
#define EEPROM_BANK1_SEL			0x02

#define REG_TI_ADCVBATZSEGE			0x53		//Data Register for ADC Calibration
#define REG_TI_ADCVBATZSEGE_SHIFT	4

#define SMPL_INTVL					0x03
#define CURRENT_GAIN				366			//TBD from hw team, Rsens = 10 gain = 0.366, Rsens = 20 gain = 0.1831
												//Rsens = 30, gain = 0.122066667
#define MAX_CC_SCALE				3662		/*CC Accumulator Bit unit 3.662uV/10mohm */
#define CC_SMPL_CTR_MAX_VAL			0xFFFFFF
#define CC_CNTL_CC_CTR_EN			BIT0
#define CC_CNTL_CC_CLR_EN			BIT1
#define CC_CNTL_CC_CAL_EN			BIT2
#define CC_CNTL_CC_OFFSET_EN		BIT3												
#define CC_CNTL_SMPL_INTVL			(BIT4 | BIT5)	//TODO: Check with hw for sample interval
#define CC_GAIN_STEP				25
#define DEFAULT_CC_OFFSET_STEP		2
#define TRIM_REV_3_OFFSET_STEP		1

//
//	0x01- SDP
//	0x02- CDP
//	0x03- DCP
//  0x04- ACA
//  0x05- Unknow or Other Charger Type
//
#define	CHG_SDP		0x01
#define CHG_CDP		0x02
#define CHG_DCP		0x03
#define CHG_ACA		0x04
#define CHG_UNKNW	0x05
//ADC Channel
#define CH_VBAT		0
#define CH_DIETEMP	1
#define CH_BPTHERM	2
#define CH_GPADC	3

//Following two macros return TRUE(1) if bit is set/cleared
#define ISBITSET(var,bitpos)	((var & (1<<bitpos))>>bitpos)
#define ISBITCLR(var,bitpos)	(~(var | (~(1<<bitpos)))>>bitpos)

#define BITSET(Data,BitPos)		Data |= (1<<BitPos)
#define BITCLR(Data,BitPos)		Data &= ~(1<<BitPos)
#define BITINV(Data,BitPos)		Data ^= (1<<BitPos)

//Get structure offset - Pass sturcuture name and element name as parameter
#define OFFSETOF_STURCT(STRUCTURE,ELEMENT)	((int) (&(((STRUCTURE*)(0))->ELEMENT)))
#define CONVERT8to16BIT(Lsb,Msb,Result)	Result = (UINT16) (Lsb | ((UINT16)(Msb << 8)))
#define CONVERT8to32BIT(BYT0,BYT1,BYT2,BYT3,Result) Result = (UINT32)(BYT0 | ((UINT32)(BYT1 << 8)) | ((UINT32)(BYT2 << 16)) | ((UINT32)(BYT3 << 24)))
#define CONVERT8to24BIT(BYT0,BYT1,BYT2,Result) Result = (UINT32)(BYT0 | ((UINT32)(BYT1 << 8)) | ((UINT32)(BYT2 << 16)))

static void ResetSemaphore(void);

void SendDoorBellToPunit(void);

EFI_STATUS
EFIAPI 
AcquireOwnership(void);

EFI_STATUS
EFIAPI
ReleaseOwnership(void);

EFI_STATUS
EFIAPI
TiPmicDVFSInit(void);

EFI_STATUS
EFIAPI
TiReadEeprom(void);

EFI_STATUS
EFIAPI
TiReadOffsetGainCompensation(void);

UINT16
EFIAPI
TiPmicGetCorrectedVoltage(void);

#define DIV_ROUND_CLOSEST(x, divisor)                  \
        (x > 0) ? ((x + divisor / 2) / divisor) : ((x - divisor / 2) / divisor)        

#endif

