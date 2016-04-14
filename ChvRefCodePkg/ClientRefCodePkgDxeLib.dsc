## @file
#  Component description file for the Braswell RC DXE libraries.
#
#  Copyright (c) 2012 - 2015, Intel Corporation. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php.
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
##

#
# RC Dxe Library
#

#
# Common
#
 AslUpdateLib|$(PLATFORM_RC_PACKAGE)/Library/AslUpdate/Dxe/AslUpdateLib.inf
# SocSkuLib|$(PLATFORM_RC_PACKAGE)/Library/SocSkuLib/SocSkuLib.inf

#
# Txe
#


#
# SouthCluster
#
  PchDxeRuntimePciLibPciExpress|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/DxeRuntimePciLibPciExpress/DxeRuntimePciLibPciExpress.inf
  I2cLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/I2CLib/I2CLib.inf