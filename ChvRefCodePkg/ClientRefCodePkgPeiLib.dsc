## @file
#  Component description file for the Braswell RC PEI libraries.
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
# RC Pei Library
#

#
# Common
#
#SocSkuLib|$(PLATFORM_RC_PACKAGE)/Library/SocSkuLib/SocSkuPeiLib.inf

#
# SouthCluster
#
  I2CLibPei|$(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Library/I2CLibPei/I2CLibPei.inf

#
# Txe
#
#  SeCUmaLib|$(PLATFORM_RC_PACKAGE)/Txe/SeCUma/SeCUma.inf
!if $(FTPM_ENABLE)==TRUE
  fTPMInitLib|$(PLATFORM_RC_PACKAGE)/Txe/fTPM/Pei/fTPMInitPei.inf
!endif
