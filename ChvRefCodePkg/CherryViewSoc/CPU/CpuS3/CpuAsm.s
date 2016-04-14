## @file
#
#  This is the code that supports IA32 CPU architectural protocol.
#
#  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php.
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
#
##
.equ	VacantFlag, 0x00
.equ	NotVacantFlag, 0x0ff

.equ	LockLocation, 0x1000 - 0x0200
.equ	StackStartAddressLocation, 	LockLocation + 0x04
.equ	StackSizeLocation, 			LockLocation + 0x08
.equ	CProcedureLocation,			LockLocation + 0x0C
.equ	GdtrLocation,				LockLocation + 0x10
.equ	IdtrLocation,				LockLocation + 0x16
.equ	BufferStartLocation,		LockLocation + 0x1C
.equ	PmodeOffsetLocation,		LockLocation + 0x20
.equ	AcpiCpuDataAddressLocatio,	LockLocation + 0x24
.equ	MtrrValuesAddressLocation,	LockLocation + 0x28
.equ	FinishedCountAddressLocation,	LockLocation + 0x2C
.equ	SerializeLockAddressLocation,	LockLocation + 0x30
.equ	MicrocodeAddressLocation,	LockLocation + 0x34
.equ	BootScriptAddressLocation,	LockLocation + 0x38
.equ	StartStateLocation,			LockLocation + 0x3C


ASM_GLOBAL ASM_PFX(EnableMCE)
ASM_PFX(EnableMCE):
  movl     %cr4, %eax
  orl      $0x40, %eax
  movl     %eax, %cr4
  ret
#EnableMCE  endp

ASM_GLOBAL ASM_PFX(MpMtrrSynchUpEntry)
ASM_PFX(MpMtrrSynchUpEntry):
    #
    # Enter no fill cache mode, CD=1(Bit30), NW=0 (Bit29)
    #
    movl %cr0, %eax
    andl 0x0DFFFFFFF, %eax
    orl  0x040000000, %eax
    movl %eax, %cr0
    #
    # Flush cache
    #
    wbinvd
    #
    # Clear PGE flag Bit 7
    #
    movl %cr4, %eax
    movl %eax, %edx
    andl 0x0FFFFFF7F, %eax
    movl %eax, %cr4
    #
    # Flush all TLBs
    #
    movl %cr3, %eax
    movl %eax, %cr3
    
    movl %edx, %eax
    ret
#MpMtrrSynchUpEntry  ENDP

ASM_GLOBAL ASM_PFX(MpMtrrSynchUpExit)
ASM_PFX(MpMtrrSynchUpExit):
    push    %ebp             # C prolog
    movl    %esp, %ebp
    #
    # Flush all TLBs the second time
    #
    movl %cr3, %eax
    movl %eax, %cr3
    #
    # Enable Normal Mode caching CD=NW=0, CD(Bit30), NW(Bit29)
    #
    movl %cr0, %eax
    andl 0x09FFFFFFF, %eax
    movl %eax, %cr0
    #
    # Set PGE Flag in CR4 if set
    #
    movl 8(%ebp), %eax
    movl %eax, %cr4
    
    pop %ebp
    ret
#MpMtrrSynchUpExit  ENDP

#-------------------------------------------------------------------------------
#  AsmAcquireMPLock (&Lock)
#-------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(AsmAcquireMPLock)
ASM_PFX(AsmAcquireMPLock):
        pushal
        movl        %esp, %ebp

        movb        $NotVacantFlag, %al
        movl        0x24(%ebp), %ebx
TryGetLock:
        lock xchgb  (%ebx), %al
        cmpb        $VacantFlag, %al
        jz          LockObtained

	.byte 0xf3, 0x90	# PAUSE32
	
        jmp         TryGetLock       

LockObtained:
        popal
        ret
#AsmAcquireMPLock   ENDP

#-------------------------------------------------------------------------------
#  AsmReleaseMPLock (&Lock)
#-------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(AsmReleaseMPLock)
ASM_PFX(AsmReleaseMPLock):
        pushal
        movl        %esp, %ebp

        movb        $VacantFlag, %al
        movl        0x24(%ebp), %ebx
        lock xchgb  (%ebx), %al

        popal
        ret
#AsmReleaseMPLock   ENDP
