/** @file
  Interface definition details between DPTF and platform drivers during DXE phase.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _DPTF_PLATFORM_POLICY_PROTOCOL_H_
#define _DPTF_PLATFORM_POLICY_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gDxeDptfPlatformPolicyGuid;

//
// Protocol revision number
// Any backwards compatible changes to this protocol will result in an update in the revision number
// Major changes will require publication of a new protocol
//
#define DXE_DPTF_PLATFORM_POLICY_PROTOCOL_REVISION 0

//
// DPTF DXE Platform Policy ==================================================
//

typedef struct _DXE_DPTF_PLATFORM_POLICY_PROTOCOL {
  UINT8                   Revision;
  UINT8                   EnableDptf;                     // Option to enable/disable DPTF
  UINT8                   DptfProcessor;                  // Option to enable/disable Processor Participant
  UINT16                  ProcCriticalTemp;               // Processor critical temperature
  UINT16                  ProcPassiveTemp;                // Processor passive temperature
  UINT16                  GenericCriticalTemp0;           // Critical temperature value for generic sensor0 participant
  UINT16                  GenericPassiveTemp0;            // Passive temperature value for generic sensor0 participant
  UINT16                  GenericCriticalTemp1;           // Critical temperature value for generic sensor1 participant
  UINT16                  GenericPassiveTemp1;            // Passive temperature value for generic sensor1 participant
  UINT16                  GenericCriticalTemp2;           // Critical temperature value for generic sensor2 participant
  UINT16                  GenericPassiveTemp2;            // Passive temperature value for generic sensor2 participant
  UINT8                   BidirectionalProchotEnable;
  UINT8                   ThermalMonitoring;
  UINT8                   ThermalMonitoringHot;
  UINT8                   ThermalMonitoringSystherm0Hot;
  UINT8                   ThermalMonitoringSystherm1Hot;
  UINT8                   ThermalMonitoringSystherm2Hot;

  UINT16                  DptfProcActiveTemperatureSA;    // DPTF: Processor active temperature
  UINT16                  DptfProcCriticalTemperatureSA;  // DPTF: Processor critical temperature
  UINT16                  DptfProcCR3TemperatureSA;       // DPTF: Processor CR3 temperature
  UINT16                  DptfProcHotTemperatureSA;       // DPTF: Processor Hot temperature
  UINT16                  DptfProcPassiveTemperatureSA;   // DPTF: Processor passive temperature
  UINT16                  DptfGenericActiveTemperature0;  // DPTF: Active temperature value for generic sensor0 participant
  UINT16                  DptfGenericCR3Temperature0;     // DPTF: CR3 temperature value for generic sensor0 participant
  UINT16                  DptfGenericHotTemperature0;     // DPTF: Hot temperature value for generic sensor0 participant
  UINT16                  DptfGenericActiveTemperature1;  // DPTF: Active temperature value for generic sensor1 participant
  UINT16                  DptfGenericCR3Temperature1;     // DPTF: CR3 temperature value for generic sensor1 participant
  UINT16                  DptfGenericHotTemperature1;     // DPTF: Hot temperature value for generic sensor1 participant
  UINT16                  DptfGenericActiveTemperature2;  // DPTF: Active temperature value for generic sensor2 participant
  UINT16                  DptfGenericCR3Temperature2;     // DPTF: CR3 temperature value for generic sensor2 participant
  UINT16                  DptfGenericHotTemperature2;     // DPTF: Hot temperature value for generic sensor2 participant
  UINT8                   EnablePassivePolicy;            // DPTF: Passive Policy enable/disable
  UINT8                   EnableCriticalPolicy;           // DPTF: Critical Policy enable/disable
  UINT8                   EnableActivePolicy;             // DPTF: Active Policy enable/disable
} DXE_DPTF_PLATFORM_POLICY_PROTOCOL;

#endif
