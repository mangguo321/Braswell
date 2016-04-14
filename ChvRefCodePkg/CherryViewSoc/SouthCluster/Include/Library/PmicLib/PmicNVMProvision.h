/** @file

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef PMICNVMPROVISION_H_
#define PMICNVMPROVISION_H_

#define PMIC_PROVISIONING_ASK_USER 0
#define PMIC_PROVISIONING_PUTS_DEBUG 1
#define PMIC_PROVISIONING_NEUTERED_FOR_DEBUG 0
#define PMIC_PROVISIONING_ASK_USER_TIMEOUT 60

// setting below to '1' will print the current nvm content and exit.
#define NVM_READBACK_ONLY 0

#define NVM_OTP_MAX 2048
#define NVM_WR_ERR 1
#define MAGIC_NVM 0xDEADBEEF

// Flags
#define OTP_WR_ONGOING  0x1 //NVMSTAT0_REG[0]
#define OTP_PD_ACT      0x2 //NVMSTAT0_REG[1]
#define OTP_WR_MODE_EN  0x1 //NVMCTRL1_REG[0]
#define OTP_WR_TRIGGER  0x2 //NVMCTRL1_REG[1]
#define WRITE_FINISHED_INFO 0x1
#define WRITE_ONE_CLEAR 0x1
#define IA_IRQ_ASSERTED  1
#define NVM_CONTROLLER  0x1

// Register definitions
#define PMIC_NVMDBUF0      0x1C6
#define PMIC_NVMDBUF1      0x1C7
#define PMIC_NVMDBUF2      0x1C8
#define PMIC_NVMDBUF3      0x1C9
#define PMIC_NVMDBUF4      0x1CA
#define PMIC_NVMDBUF5      0x1CB
#define PMIC_NVMDBUF6      0x1CC
#define PMIC_NVMDBUF7      0x1CD
#define PMIC_NVMDBUF8      0x1CE
#define PMIC_NVMDBUF9      0x1CF
#define PMIC_NVMDBUF10     0x1D0
#define PMIC_NVMDBUF11     0x1D1
#define PMIC_NVMDBUF12     0x1D2
#define PMIC_NVMDBUF13     0x1D3
#define PMIC_NVMDBUF14     0x1D4
#define PMIC_NVMDBUF15     0x1D5
#define PMIC_NVMDBUF16     0x1D6
#define PMIC_NVMDBUF17     0x1D7
#define PMIC_NVMDBUF18     0x1D8
#define PMIC_NVMDBUF19     0x1D9
#define PMIC_NVMDBUF20     0x1DA
#define PMIC_NVMDBUF21     0x1DB
#define PMIC_NVMDBUF22     0x1DC
#define PMIC_NVMDBUF23     0x1DD
#define PMIC_NVMDBUF24     0x1DE
#define PMIC_NVMDBUF25     0x1DF
#define PMIC_NVMDBUF26     0x1E0
#define PMIC_NVMDBUF27     0x1E1
#define PMIC_NVMDBUF28     0x1E2
#define PMIC_NVMDBUF29     0x1E3
#define PMIC_NVMDBUF30     0x1E4
#define PMIC_NVMDBUF31     0x1E5
#define PMIC_NVMDBUF32     0x1E6
#define PMIC_NVMDBUF33     0x1E7
#define PMIC_NVMCTL0       0x1E8
#define PMIC_NVMCTL1       0x1E9
#define PMIC_OTPMRB1       0x1EA
#define PMIC_OTPMRB0_RD1   0x1ED
#define PMIC_OTPMRB0_RD2   0x1EE
#define PMIC_NVMSTAT0      0x1F0
#define PMIC_NVMUSAGESTATH 0x1F3
#define PMIC_NVMUSAGESTATL 0x1F4

#define PMIC_ID_REG        0x000 // PMIC_ID_REG: 5E offset 0x00
#define PMICSPARE00_REG    0x108 // OTP_VERSION: 6E offset 0x08
#define PMICSPARE01_REG    0x109

#define PMIC_VENDIRQLVL1   0x200
#define PMIC_NVMVIRQ_REG   0x202 //4E

#define PMIC_NVM_MB_ADDRH_REG  0x1C3 // for read-back
#define PMIC_NVM_MB_ADDRL_REG  0x1C4 // for read-back
#define PMIC_NVM_MB_DATA_REG   0x1C5 // for read-back

#define MAX_SECTION_SIZE 34
#define SECTION_HEADER_SIZE 2
#define PMIC_PROVISIONING_PUTS_DEBUG 1
#define PMIC_NVM_FILE_NAME L"PMIC_NVM_payload.bin"

typedef struct
{
    UINT16 rstid_msb:2;
    UINT16 owner:5;
    UINT16 valid:1;
    UINT16 sectionLength:5;
    UINT16 type:1;
    UINT16 rstid_lsb:2;
} NVM_SectionHeader_t;

typedef struct
{
    UINT32 Magic;
    UINT16 xlsVersion;
    UINT16 glueVersion;
    UINT16 lengthOfPayloadStructure;
    UINT8 checksum;
    char nvmDate;
} NVM_PayLoadHeader_t;

#define MAX_OWNER_TYPE 20
#define NUM_VALID_RESET_IDS 7
#define RAW_DATA (1 << 2)
#define ADDRESS_AUTO_INCREMENT (1 << 1)
#define ARBITRARY_ADDRESS (1 << 0)

	
// WHC:
typedef enum
{
    Production = 1,
    TLP1 = 0x2,
    TLP2 = 0x3,
	CSSM_Transl_Table = 0x4,
	SoC_Device_ID2 = 0x6,
    SoC_Device_ID3 = 0x7,
    Soc_Device_ID4 = 0x8,
	SCRATCH = 0x9, //also called "SoC_Device_ID_6"
	VNN_SVID = 0xA,
    VNN_DCDC = 0xF,
    //NVM.sectionOwner.
	VCC_PHS1_DCDC = 0x18,
    //NVM.sectionOwner.
	VCC_PHS2_DCDC = 0x19,
    //NVM.sectionOwner.
	VCC_PHS3_DCDC = 0x1A,
    //NVM.sectionOwner.
	VCC_PHS4_DCDC = 0x1B,
    //NVM.sectionOwner.
	VCC_PHS5_DCDC = 0x1C,
    //NVM.sectionOwner.
	VMEM_PHS1_DCDC = 0x1D,
    //NVM.sectionOwner.
	VDD2_PHS1_DCDC = 0x1E,
    //-----
    SoC_Device_ID_5 = 0x5,
    VCC0_SVID = 0xB,
    VCC1_SVID = 0xC,
    VGG_SVID =  0xD,
    V_PLT_SVID = 0xE,
    VCC0_DCDC = 0x10,
    VCC1_DCDC = 0x11,
    VGG_DCDC = 0x12,
    V1P8A_DCDC = 0xB,
    V1P05A_DCDC = 0x14,
    V1P15_DCDC = 0x15,
    VDDQ_DCDC = 0x16,
    V3P3A_DCDC = 0x17,
} owner_type_e;

typedef enum
{
    Production_ResetID = 0,
    PMIC_OFF = 0x1,
    PMIC_OFF_or_COLD_RESET = 0x3,
    PMIC_OFF_or_COLD_WARM_RESET = 0x7,
    VRTC_RESET = 0x8,
    PMIC_OFF_or_TLP1_RESET = 0xC,
    PMIC_OFF_or_TLP2_RESET = 0xA,
} rstid_e;

extern UINT32 sendToHostRT(char *, UINT32);

UINT8 Calculate8BitChecksum_16(UINT8 *, UINT16);
int validatePayload(UINT8, UINT16);
void disable_OTP_WR_MODE_EN(void);
UINT8 NVMRead(UINT8, UINT8, UINT16, UINT8 *);
UINT8 Write_Section(NVM_SectionHeader_t *, UINT8 *);
UINT32 NVM_ReadBack_and_exit(UINT16);
UINT32 PmicNVMProvision(UINT8* , UINTN);
void write_OTP_trigger_flag(void);

#endif /* PMICNVMPROVISION_H_ */

