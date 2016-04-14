/** @file
  Header file for PchPlatform Lib.

  Copyright (c) 2008 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_PLATFORM_LIB_H_
#define _PCH_PLATFORM_LIB_H_

//
// Timeout value used when Sending / Receiving messages.
// NOTE: this must cover the longest possible wait time
// between message being sent and response being available.
// e.g. Virtual function readiness might take some time.
//

/**
  Delay for at least the request number of microseconds.
  This function would be called by runtime driver, please do not use any MMIO marco here.

  @param[in] Microseconds         Number of microseconds to delay.

**/
VOID
EFIAPI
PchPmTimerStall (
  IN  UINTN   Microseconds
  );

/**
  Check whether SPI is in descriptor mode

  @param[in] SpiBase              The PCH Spi Base Address

  @retval TRUE                    SPI is in descriptor mode
  @retval FALSE                   SPI is not in descriptor mode
**/
BOOLEAN
EFIAPI
PchIsSpiDescriptorMode (
  IN  UINTN   SpiBase
  );

/**
  Return SoC stepping type

  @retval SOC_STEPPING            SoC stepping type
**/
SOC_STEPPING
EFIAPI
SocStepping (
  VOID
  );

/**
Return SoC package type

@retval SOC_PACKAGE           SoC package type
**/
SOC_PACKAGE
EFIAPI
GetSocPackageType (
  VOID
  );

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  );

/**
  This function can be called to enable/disable Alternate Access Mode

  @param[in] IlbBase              The PCH ILB Base Address
  @param[in] AmeCtrl              If TRUE, enable Alternate Access Mode.
                                  If FALSE, disable Alternate Access Mode.

**/
VOID
EFIAPI
PchAlternateAccessMode (
  IN  UINTN         IlbBase,
  IN  BOOLEAN       AmeCtrl
  );

/**
  Write DWord data to extended IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] Value                Value to be written
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

**/
VOID
PchMsgBusWriteEx32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          Value,
  IN     UINT8                           WriteOpCode,
  IN     UINT8                           Bar,          OPTIONAL
  IN     UINT8                           Device,       OPTIONAL
  IN     UINT8                           Function      OPTIONAL
  );

/**
  Write DWord data to IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] Value                Value to be written
  @param[in] WriteOpCode          Write Op Code

**/
VOID
PchMsgBusWrite32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT32                          Value,
  IN     UINT8                           WriteOpCode
  );

/**
  Read DWord data from extended IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] ReadOpCode           Read Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

  @retval Data32                  Value to be read.
**/
UINT32
PchMsgBusReadEx32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT8                           ReadOpCode,
  IN     UINT8                           Bar,          OPTIONAL
  IN     UINT8                           Device,       OPTIONAL
  IN     UINT8                           Function      OPTIONAL
  );

/**
  Read DWord data from IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] ReadOpCode           Read Op Code

  @retval Data32                  Value to be read.
**/
UINT32
PchMsgBusRead32 (
  IN     UINT8                           PortId,
  IN     UINT32                          Register,
  IN     UINT8                           ReadOpCode
  );

/**
  Read-modify-write DWord data from extended IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code
  @param[in] Bar                  BAR
  @param[in] Device               Device Number
  @param[in] Function             Function Number

  @retval EFI_SUCCESS             Successfully completed.
**/
EFI_STATUS
EFIAPI
PchMsgBusAndThenOrEx32 (
    IN     UINT8                           PortId,
    IN     UINT32                          Register,
    IN     UINT32                          AndData,
    IN     UINT32                          OrData,
    IN     UINT8                           ReadOpCode,
    IN     UINT8                           WriteOpCode,
    IN     UINT8                           Bar,          OPTIONAL
    IN     UINT8                           Device,       OPTIONAL
    IN     UINT8                           Function      OPTIONAL
  );

/**
  Read-modify-write DWord data from IOSF-SB register block

  @param[in] PortId               Port ID of the target IOSF-SB block
  @param[in] Register             Register of the target IOSF-SB block
  @param[in] AndData              Value to be ANDed
  @param[in] OrData               Value to be ORed
  @param[in] ReadOpCode           Read Op Code
  @param[in] WriteOpCode          Write Op Code

  @retval EFI_SUCCESS             Successfully completed.
**/
EFI_STATUS
EFIAPI
PchMsgBusAndThenOr32 (
    IN     UINT8                           PortId,
    IN     UINT32                          Register,
    IN     UINT32                          AndData,
    IN     UINT32                          OrData,
    IN     UINT8                           ReadOpCode,
    IN     UINT8                           WriteOpCode
  );

/**
  This function can be called to poll for certain value within a time given.

  @param[in] MmioAddress          The Mmio Address.
  @param[in] BitMask              Bits to be masked.
  @param[in] BitValue             Value to be polled.
  #param[in] DelayTime            Delay time in terms of 100 micro seconds.

  @retval EFI_SUCCESS             Successfully polled the value.
  @retval EFI_TIMEOUT             Timeout while polling the value.
**/
EFI_STATUS
EFIAPI
PchMmioPoll32 (
  IN  UINTN         MmioAddress,
  IN  UINT32        BitMask,
  IN  UINT32        BitValue,
  IN  UINT16        DelayTime
  );

#endif
