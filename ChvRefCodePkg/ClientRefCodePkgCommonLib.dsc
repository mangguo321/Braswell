## @file
#  Component description file for the Braswell RC both Pei and Dxe libraries DSC file.
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
# RC Common Library
#

#
# Txe
#

#
# SouthCluster
#
  PchPlatformLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/PchPlatformLib/PchPlatformLib.inf
  PchSmbusLibDxe|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/PchSmbusLib/Dxe/PchSmbusLibDxe.inf
  PchSmbusLibPei|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/PchSmbusLib/Pei/PchSmbusLibPei.inf
  GpioLib|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/GpioLib/GpioLib.inf
