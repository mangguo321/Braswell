/** @file
  Battery and Energe Management Table.

  Copyright (c) 1996 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _BEMT_H
#define _BEMT_H

//
// Statements that include other files
//
#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi20.h>
#include <IndustryStandard/Acpi30.h>

#define EFI_ACPI_OEM0_REVISION         0x05
#define EFI_ACPI_OEM0_SIGNATURE        SIGNATURE_32 ('O', 'E', 'M', '0')

#define EFI_ACPI_OEM_ID                'I','N','T','E','L',' '   // OEMID 6 bytes long
#define EFI_ACPI_OEM0_TABLE_ID         SIGNATURE_64 ('B','A','T','T','E','R','Y',' ') // OEM table id 8 bytes long
#define CREATOR_ID                     SIGNATURE_32 ('I','N','T','L')  
#define EFI_FIX_BATTARY_ID             SIGNATURE_64 ('I','N','T','N','0','0','0','1')
#define ACPI_TEMP_RANGES_NUMBER        6

#define EFI_ACPI_BAT_CONF_SIGNATURE    SIGNATURE_32 ('B', 'C', 'F', 'G')
#define EFI_ACPI_BAT_CONF_REVISION     0x01
#define EFI_ACPI_BAT_CONF_TABLE_ID     SIGNATURE_64 ('B', 'A', 'T', 'T', 'C', 'O', 'N', 'F') // OEM table id 8 bytes long

#pragma pack(1)

//
// Oem0 ACPI table structure
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER  Header;
  UINT64                       BatteryId;
  UINT8                        TurboChargingSupport;
  UINT8                        BatteryType;
  UINT16                       Capacity;
  UINT16                       MaxVoltage;
  UINT16                       ChargeTermiationCurrent;
  UINT16                       LowBatteryThreshold;
  INT8                         SafeDischargeTemperatureUL;
  INT8                         SafeDischargeTemperatureLL;
  UINT16                       TempMonitoringRanges;
  struct {
    INT16                      TempUL;
    UINT16                     FullChargeVolt;
    UINT16                     FullChargeCurr;
    UINT16                     MaintChargeStartThreshVolt;
    UINT16                     MaintChargeStopThreshVolt;
    UINT16                     MaintChargeCurr;
  } TempMonitorRanges [ACPI_TEMP_RANGES_NUMBER];
  INT16                        TempLL;
} EFI_ACPI_OEM0_TABLE;

//
// Battery Config ACPI table structure
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT64                      FG_Name;
  UINT64                      BatteryId;
  UINT16                      Size;
  UINT8                       FG_ConfigOption;
  UINT16                      PrimaryChecksum;
  //
  // Use the maximum fuel gauge config data size.
  // MAXIM       - 200 Bytes
  // XPOWER      - 36  Bytes
  // $Cove SW FG - 256 Bytes
  // TI BQ       - Factory Programmed into the chip
  //
  UINT8                       ConfigData[256];
} EFI_ACPI_BAT_CONF_TABLE;

//
// Battery Config Variable structure
//
typedef struct {
  UINT64                      FG_Name;
  UINT16                      Size;
  UINT16                      Checksum;        // Checksum for variable config data
  UINT16                      PrimaryChecksum; // Primary config data checksum
  //
  // Use the maximum fuel gauge config data size.
  // MAXIM       - 200 Bytes
  // XPOWER      - 36  Bytes
  // $Cove SW FG - 256 Bytes
  // TI BQ       - Factory Programmed into the chip
  //
  UINT8                       ConfigData[256];
} FUEL_GAUGE_VARIABLE;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  struct{
	  UINT8 Averaging;
	  UINT8 Seq_edv;
	  UINT8 Overload_current[2];
	  UINT8 Term_voltage[2];
	  UINT8 Z_factor[2]; 
	  UINT8 Edv[9];
  }EDV_configuration;
  struct{
	  UINT8 Voltage_diff;
	  UINT8 Current_diff;
	  UINT8 Sleep_enter_current[2]; 
	  UINT8 Sleep_enter_samples;
	  UINT8 Sleep_exit_current[2];  
	  UINT8 Sleep_exit_samples;
	  UINT8 Long_sleep_current[2]; 
	  UINT8 Ocv_period[4]; 
	  UINT8 Relax_period[4]; 
	  UINT8 Flat_zone_low[2];
	  UINT8 Flat_zone_high[2];
	  UINT8 C_factor[2]; 
	  UINT8 Z_factor[2]; 
	  UINT8 Max_delta;
	  UINT8 Fcc_update_valid_min[2]; 
	  UINT8 Fcc_update_valid_max[2]; 
	  UINT8 Ocv_table[42];
  }OCV_configuration;
  struct{
	  UINT8 Max_rate[2]; 
	  UINT8 Min_rate[2]; 
	  UINT8 Chg_gain[2]; 
	  UINT8 Dsg_gain[2]; 
  }Rate_configuration;
  struct{
	  UINT8 Cc_voltage[2];
	  UINT8 Cc_current[2];
	  UINT8 Cc_q[4]; 
	  UINT8 Seq_cc;
	  UINT8 Cc_term_voltage[2];
      UINT8 Ocv_below_edv1; 
	  UINT8 Design_capacity[4]; 
	  UINT8 R_sense;
	  UINT8 Fcc_adjust[2]; 
	  UINT8 Max_impedance[2];
	  UINT8 Max_overcharge;
	  UINT8 Max_fcc_delta;
	  UINT8 Low_temp;
	  UINT8 Light_load;
	  UINT8 Near_full;
	  UINT8 Recharge;
	  UINT8 Mode_switch_capacity[4];  
	  }Cell_configuration;
	UINT8 Cal_volt[4]; 
	UINT8 Cal_cur[4];
	UINT8 Cal_temp[4];
	UINT8 ChgReg00;
	UINT8 ChgReg01;
	UINT8 ChgReg02;
	UINT8 ChgReg03;
	UINT8 ChgReg04;
	UINT8 ChgReg05;
	UINT8 ChgReg06;
	UINT8 ChgReg07;
	UINT8 SDP_ilim;
	UINT8 CDP_ilim;
	UINT8 DCP_ilim;
	UINT8 MaintainanceChg;

} EFI_ACPI_WIN_BAT_CONF_TABLE;

#pragma pack()

#endif
