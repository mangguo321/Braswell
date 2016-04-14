/** @file
  PCH SCC Tuning Protocol

  Copyright (c) 2014 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_SCC_TUNING_PROTOCOL_H_
#define _PCH_SCC_TUNING_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

#define PCH_SCC_TUNING_PROTOCOL_REVISION 1
//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gPchSccTuningProtocolGuid;

//
// Forward declaration for PCH_EMMC_TUNING_PROTOCOL
//
typedef struct _PCH_EMMC_TUNING_PROTOCOL PCH_EMMC_TUNING_PROTOCOL;
///
/// This structure describes the required Emmc info for HS400 tuning
///
typedef struct {
  EFI_HANDLE                PartitionHandle;    ///< eMMC partition handle for block read/write
  EFI_LBA                   Lba;                ///< Logical Block Address for HS400 Tuning block read/write
  UINT32                    RelativeDevAddress; ///< Device system address, dynamically assigned by the host during initialization.
  UINT8                     HS200BusWidth;      ///< The value to be programmed for BUS_WIDTH[183] byte
} EMMC_INFO;

///
/// This structure describes the return value after DLL tuning
///
typedef struct {
  UINT8       RxDllDataValid;     ///< Set if Rx DLL Tuning Data is valid
  UINT8       TxDllDataValid;     ///< Set if Tx DLL Tuning Data is valid
  UINT8       RxStrobeDllValue;   ///< Rx Strobe Delay Line Value
  UINT8       TxDataDllValue;     ///< Tx Data Delay Line value
} EMMC_TUNING_DATA;

///
/// EMMC HS200/HS400 TUNING INTERFACE
///

typedef EFI_STATUS (EFIAPI *EMMC_TUNE) (
  IN   PCH_EMMC_TUNING_PROTOCOL         *This,              ///< This pointer to PCH_EMMC_TUNING_PROTOCOL
  //
  //  Revision parameter is used to verify the layout of EMMC_INFO and TUNINGDATA.
  //  If the revision is not matched, means the revision of EMMC_INFO and TUNINGDATA is not matched.
  //  And function will return immediately.
  //
  IN   UINT8                            Revision,
  IN   EMMC_INFO                        *EmmcInfo,          ///< Pointer to EMMC_INFO
  OUT  EMMC_TUNING_DATA                 *EmmcTuningData     ///< Pointer to EMMC_TUNING_DATA
);

///
/// PCH EMMC TUNING PROTOCOL INTERFACE
///
struct _PCH_EMMC_TUNING_PROTOCOL {
  EMMC_TUNE  EmmcTune;  ///< Emmc Hs400 Tuning Interface
};

#endif
