## @file
#  Component description file for the Braswell RC PEI drivers.
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
# NorthCluster
#

  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/NorthCluster/SmmAccess/Pei/SmmAccess.inf

#
# SouthCluster
#
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/Spi/Pei/PchSpiPeim.inf
  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/SouthCluster/SmmControl/Pei/SmmControl.inf

  $(PLATFORM_RC_PACKAGE)/CherryViewSoc/CPU/CpuS3/MpS3.inf

