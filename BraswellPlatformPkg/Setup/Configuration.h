/** @file
  Driver configuration include file.

  Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

//
// System Setup Page. Do not have to be sequential but have to be unique
//
#define ROOT_FORM_ID                        1
#define ROOT_MAIN_FORM_ID                   2
#define CPU_CONFIGURATION_FORM_ID           3
#define CPU_PWR_CONFIGURATION_FORM_ID       4
#define BOOT_CONFIGURATION_FORM_ID          5
#define IGD_FORM_ID                         6
#define SECURITY_CONFIGURATION_FORM_ID      7
#define SOUTH_CLUSTER_FORM_ID               8
#define DPTF_FORM_ID                        9
#define PLATFORM_INFORMATION_FORM_ID        10
#define SENSOR_CONFIGURATION_ID             11
#define LPSS_OPTIONS_FORM_ID                12
#define UNCORE_FORM_ID                      13
#define TPM_FORM_ID                         14
#define THERMAL_FORM_ID                     15
#define PASSWORD_SETTING_ID                 16
#define PCH_PCIEX_S0IX_OPTIONS_FORM_ID      17
#define AZALIA_OPTIONS_FORM_ID              18
#define MISC_OPTIONS_FORM_ID                19
#define USB_OPTIONS_FORM_ID                 20
#define PCIE_DEVICE_OPTIONS_FORM_ID         21
#define SYSTEM_COMPONENT_FORM_ID            22
#define DEBUG_CONFIGURATION_FORM_ID         23
#define SCC_OPTIONS_FORM_ID                 24
#define PCH_PCIERP1_OPTIONS_FORM_ID         25
#define PCH_PCIERP2_OPTIONS_FORM_ID         26
#define PCH_PCIERP3_OPTIONS_FORM_ID         27
#define PCH_PCIERP4_OPTIONS_FORM_ID         28
#define ISH_OPTIONS_FORM_ID                 29
#define SECURITY_OPTIONS_FORM_ID            30
#define ISCT_CONFIGURATION_ID               31
#define ANDROID_OPTIONS_FORM_ID             32
#define SATA_OPTIONS_FORM_ID                33
#define SATA_SFMS_OPTIONS_FORM_ID           34
#define THERMAL_DPTF_FORM_ID                35
#define THERMAL_FAILSAFE_FORM_ID            36
#define PSS_CHIP_OPTION_FORM_ID             37
#define PSS_CHIP_FACTORY_FORM_ID            38
#define PSS_CHIP_ENGINEERING_FORM_ID        39
#define PSS_CHIP_LAB_FORM_ID                40
#define MEMORY_CONFIGURATION_FORM_ID        41
#define CHARGING_OPTIONS_FORM_ID            42
#define OFFSET_0                            0
#define OFFSET_1                            1
#define OFFSET_2                            2
#define OFFSET_3                            3
#define OFFSET_4                            4
#define OFFSET_5                            5
#define OFFSET_6                            6
#define OFFSET_7                            7
#define SATA_MODE_IDE                       0
#define SATA_MODE_AHCI                      1
#define SATA_MODE_RAID                      2
#endif // #ifndef _CONFIGURATION_H
