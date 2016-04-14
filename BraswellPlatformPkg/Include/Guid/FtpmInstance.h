/** @file
  TPM instance guid, used for PcdTpmInstanceGuid.

  Copyright (c) 2013 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __FTPM_INSTANCE_GUID_H__
#define __FTPM_INSTANCE_GUID_H__

#define TPM_DEVICE_INTERFACE_TPM20_FTPM  \
  { 0x1dd8a521, 0x7de9, 0x47c2, { 0x8e, 0x6, 0x29, 0xf0, 0xd5, 0x70, 0x24, 0xc6 } }

extern EFI_GUID  gEfiTpmDeviceInstanceTpm20FtpmGuid;
#endif

