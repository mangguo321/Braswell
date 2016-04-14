/** @file
  This library contains power management configuration functions specific to 
  CherryView processors.

  Acronyms:
    PPM   Platform Power Management
    GV    Geyserville
    TM    Thermal Monitor
    IST   Intel(R) Speedstep technology
    HT    Hyper-Threading Technology
    CMP   Core Multi-Processing

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CHERRYVIEW_PPM_LIB_H_
#define _CHERRYVIEW_PPM_LIB_H_

//
// Statements that include other files
//
#include "PiDxe.h"
//
// MSR definitions
//
#define BSEL_CR_OVERCLOCK_CONTROL	0xCD
#define	FUSE_BSEL_MASK		        0x07

#endif
