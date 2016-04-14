## @file
#  This is the assembly code for HT (Hyper-threading) support
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

#-------------------------------------------------------------------------------------
#RendezvousFunnelProc  procedure follows. All APs execute their procedure. This
#procedure serializes all the AP processors through an Init sequence. It must be
#noted that APs arrive here very raw...ie: real mode, no stack.
#ALSO THIS PROCEDURE IS EXECUTED BY APs ONLY ON 16 BIT MODE. HENCE THIS PROC
#IS IN MACHINE CODE.
#-------------------------------------------------------------------------------------
#RendezvousFunnelProc (&WakeUpBuffer,MemAddress)

ASM_GLOBAL ASM_PFX(RendezvousFunnelProc)
ASM_PFX(RendezvousFunnelProc):
RendezvousFunnelProcStart:

#Step-1: Grab a lock. At this point CS = 0x(vv00) and ip= 0x0.
        .byte 0x8c,0xc8                                # mov        ax,cs
        .byte 0x8e,0xd8                                # mov        ds,ax
        .byte 0x8e,0xc0                                # mov        es,ax
        .byte 0x8e,0xd0                                # mov        ss,ax
        .byte 0x33,0xc0                                # xor        ax,ax
        .byte 0x8e,0xe0                                # mov        fs,ax
        .byte 0x8e,0xe8                                # mov        gs,ax

        .byte 0xBE, 0x1C, 0x0E                         # mov        si, BufferStartLocation
        .byte 0x66, 0x8B, 0x1C                         # mov        ebx,dword ptr [si]

        .byte 0xBF, 0x20, 0x0E                         # mov        di, PmodeOffsetLocation
        .byte 0x66, 0x8B, 0x05                         # mov        eax,dword ptr [di]
        .byte 0x8B, 0xF8                               # mov        di, ax
        .byte 0x83, 0xEF, 0x06                         # sub        di, 06h
        .byte 0x66, 0x03, 0xC3                         # add        eax, ebx
        .byte 0x66, 0x89, 0x05                         # mov        dword ptr [di],eax

        .byte 0xBE, 0x10, 0x0E                         # mov        si, GdtrProfile
        .byte 0x66                                     # db         66h
        .byte 0x2E, 0x0F, 0x01, 0x14                   # lgdt       fword ptr cs:[si]

        .byte 0x33, 0xC0                               # xor        ax,  ax
        .byte 0x8E, 0xD8                               # mov        ds,  ax

        .byte 0x0F, 0x20, 0xC0                         # mov        eax, cr0                        #Get control register 0
        .byte 0x66, 0x83, 0xC8, 0x03                   # or         eax, 000000003h                 #Set PE bit (bit #0) & MP
        .byte 0x0F, 0x22, 0xC0                         # mov        cr0, eax

        .byte  0x66, 0x67, 0xEA                        # far jump
        .word  0x0, 0x0                                # 32-bit offset
        .word  0x20                                    # 16-bit selector

NemInit:                                               # protected mode entry point

        .byte 0x66,  0xB8,  0x18,  0x00                # mov        ax, 18h
        .byte 0x66,  0x8E,  0xD8                       # mov        ds, ax
        .byte 0x66,  0x8E,  0xC0                       # mov        es, ax
        .byte 0x66,  0x8E,  0xE0                       # mov        fs, ax
        .byte 0x66,  0x8E,  0xE8                       # mov        gs, ax
        .byte 0x66,  0x8E,  0xD0                       # mov        ss, ax            # Flat mode setup.

        movl  %ebx, %esi

        movl  %esi, %edi
        addl  $StartStateLocation, %edi
        movl  $1, %eax
        movl  %eax, (%edi)

        movl  %esi, %edi
        addl  $LockLocation, %edi
        movl  $NotVacantFlag, %eax
TestLock:
        xchgl %eax, (%edi)
        cmpl  $NotVacantFlag, %eax
        jz   TestLock

ProgramStack:

        movl  %esi, %edi
        addl  $StackSizeLocation, %edi
        movl  (%edi), %eax
        movl  %esi, %edi
        addl  $StackStartAddressLocation, %edi
        addl  (%edi), %eax
        movl  %eax, %esp
        movl  %eax, (%edi)

Releaselock:

        movl  $VacantFlag, %eax
        movl  %esi, %edi
        addl  $LockLocation, %edi
        xchgl %eax, (%edi)

CProcedureInvoke:

        movl  %esi, %edi
        addl  $MtrrValuesAddressLocation, %edi
        movl  (%edi), %eax
        push  %eax

        movl  %esi, %eax
        addl  $LockLocation, %eax
        push  %eax

        movl  %esi, %edi
        addl  $CProcedureLocation, %edi
        movl  (%edi), %eax

        call  %eax
        addl  $8, %esp

        cli
        hlt
        jmp  .-2

#RendezvousFunnelProc   ENDP
RendezvousFunnelProcEnd:


#-------------------------------------------------------------------------------------
#  AsmGetAddressMap (&AddressMap)
#-------------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(AsmGetAddressMap)
ASM_PFX(AsmGetAddressMap):

        movl $RendezvousFunnelProcStart, %eax
        ret

#AsmGetAddressMap   ENDP

ASM_GLOBAL ASM_PFX(AsmGetPmodeOffset)
ASM_PFX(AsmGetPmodeOffset):


	movl $NemInit, %eax
	subl $RendezvousFunnelProcStart, %eax
        ret

#AsmGetPmodeOffset ENDP

ASM_GLOBAL ASM_PFX(AsmCliHltLoop)
ASM_PFX(AsmCliHltLoop):
        cli
        hlt
        jmp .-2
#AsmCliHltLoop ENDP
