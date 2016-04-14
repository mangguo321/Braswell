/** @file
  This file defines the PCH Info Protocol.

  Copyright (c) 2008 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_INFO_H_
#define _PCH_INFO_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                       gEfiPchInfoProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_PCH_INFO_PROTOCOL EFI_PCH_INFO_PROTOCOL;

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///
/// Revision 1:  Original version
///
#define PCH_INFO_PROTOCOL_REVISION_1  1

///
/// RCVersion[7:0] is the release number.
/// For example:
/// PchFramework 0.6.0-01 should be 00 06 00 01 (0x00060001)
/// PchFramework 0.6.2    should be 00 06 02 00 (0x00060200)
///
#define PCH_RC_VERSION                0x00090000

///
/// Protocol definition
///
/// This protocol is used to provide the information of PCH controller.
///
struct _EFI_PCH_INFO_PROTOCOL {
  ///
  /// This member specifies the revision of the PCH Info protocol. This field is used
  /// to indicate backwards compatible changes to the protocol. Platform code that
  /// consumes this protocol must read the correct revision value to correctly interpret
  /// the content of the protocol fields.
  ///
  UINT8   Revision;
  ///
  /// The actual bus number of the PCH devices.
  ///
  UINT8   BusNumber;
  ///
  /// The reference code package release number
  ///
  UINT32  RCVersion;
};

#endif
