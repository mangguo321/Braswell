## @file
#  Exception handlers used in SM mode
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


ASM_GLOBAL  ASM_PFX(gSmiMtrrs)
ASM_GLOBAL  ASM_PFX(gcSmiIdtr)
ASM_GLOBAL  ASM_PFX(gcSmiGdtr)
ASM_GLOBAL  ASM_PFX(gcPsd)
ASM_GLOBAL  ASM_PFX(FeaturePcdGet (PcdCpuSmmStackGuard))
ASM_GLOBAL  ASM_PFX(gSavedPageFaultIdtEntry)
ASM_GLOBAL  ASM_PFX(gSavedDebugExceptionIdtEntry)
ASM_GLOBAL  ASM_PFX(FeaturePcdGet (PcdCpuSmmProfileEnable))
ASM_GLOBAL  ASM_PFX(InitializeSmmExternalVectorTablePtr)

    .data

NullSeg:    .quad 0
            .quad 0                     # reserved for future use
CodeSeg32:
            .word -1                    # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x9b
            .byte 0xcf                  # LimitHigh
            .byte 0                     # BaseHigh
DataSeg32:
            .word -1                    # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x93
            .byte 0xcf                  # LimitHigh
            .byte 0                     # BaseHigh
            .quad 0                     # reserved for future use
CodeSeg16:
            .word -1
            .word 0
            .byte 0
            .byte 0x9b
            .byte 0x8f
            .byte 0
DataSeg16:
            .word -1
            .word 0
            .byte 0
            .byte 0x93
            .byte 0x8f
            .byte 0
CodeSeg64:
            .word -1                    # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x9b
            .byte 0xaf                  # LimitHigh
            .byte 0                     # BaseHigh
# TSS Segment for X64 specially
TssSeg:
            .word TSS_DESC_SIZE         # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x89
            .byte 0xDB                  # LimitHigh
            .byte 0                     # BaseHigh
            .long 0                     # BaseUpper
            .long 0                     # Reserved
.equ  GDT_SIZE, .- NullSeg

TssDescriptor:
            .space 104, 0
.equ  TSS_DESC_SIZE, .- TssDescriptor

#
# This structure serves as a template for all processors.
#
ASM_PFX(gcPsd):
            .ascii  "PSDSIG  "
            .word      PSD_SIZE
            .word 2
            .word      1 << 2
            .word      CODE_SEL
            .word      DATA_SEL
            .word      DATA_SEL
            .word      DATA_SEL
            .word 0
            .quad 0
            .quad 0
            .quad 0                     # fixed in InitializeMpServiceData()
            .quad      NullSeg
            .long      GDT_SIZE
            .long 0
            .space 24, 0
            .quad      ASM_PFX(gSmiMtrrs)
.equ  PSD_SIZE,  . - ASM_PFX(gcPsd)

#
# CODE & DATA segments for SMM runtime
#
.equ  CODE_SEL,    CodeSeg64 - NullSeg
.equ  DATA_SEL,    DataSeg32 - NullSeg

ASM_PFX(gcSmiGdtr):
    .word      GDT_SIZE - 1
    .quad      NullSeg

ASM_PFX(gcSmiIdtr):
    .word      IDT_SIZE - 1
    .quad      _SmiIDT


#
# Here is the IDT. There are 32 (not 255) entries in it since only processor
# generated exceptions will be handled.
#
_SmiIDT:
# The following segment repeats 32 times:
# No. 1
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 2
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 3
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 4
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 5
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 6
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 7
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 8
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 9
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 10
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 11
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 12
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 13
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 14
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 15
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 16
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 17
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 18
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 19
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 20
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 21
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 22
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 23
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 24
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 25
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 26
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 27
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 28
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 29
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 30
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 31
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63
# No. 32
    .word 0                             # Offset 0:15
    .word CODE_SEL
    .byte 0                             # Unused
    .byte 0x8e                          # Interrupt Gate, Present
    .word 0                             # Offset 16:31
    .quad 0                             # Offset 32:63

_SmiIDTEnd:

.equ  IDT_SIZE, (_SmiIDTEnd - _SmiIDT)

#
# Saved IDT Entry for Page Fault
#
ASM_PFX(gSavedPageFaultIdtEntry):
    .quad      0                        
    .quad      0 

#
# Saved IDT Entry for INT 1
#
ASM_PFX(gSavedDebugExceptionIdtEntry):
    .quad      0                        
    .quad      0 
    
ExternalVectorTablePtr: .quad 0        # point to the external interrupt vector table

#
# Here are the global variables used by #PF exception handler.
#
_PFPML4:     .long      0
_PFPDP:      .long      0
_PFLOCK:     .byte      0

    .text

ASM_PFX(InitializeSmmExternalVectorTablePtr):
    movq    %rcx, ExternalVectorTablePtr(%rip)
    ret

#------------------------------------------------------------------------------
# _SmiExceptionEntryPoints is the collection of exception entrypoints followed
# by a common exception handler.
#
# Stack frame would be as follows as specified in IA32 manuals:
# +---------------------+ <-- 16-byte aligned ensured by processor
# +    Old SS           +
# +---------------------+
# +    Old RSP          +
# +---------------------+
# +    RFlags           +
# +---------------------+
# +    CS               +
# +---------------------+
# +    RIP              +
# +---------------------+
# +    Error Code       +
# +---------------------+
# +   Vector Number     +
# +---------------------+
# +    RBP              +
# +---------------------+ <-- RBP, 16-byte aligned
#
# RSP set to odd multiple of 8 at @CommonEntryPoint means ErrCode PRESENT
#------------------------------------------------------------------------------
_SmiExceptionEntryPoints:
.equ  IHDLRIDX,  0
# The following segment repeats 31 times:
# No. 1
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 2
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 3
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 4
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 5
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 6
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 7
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 8
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 9
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 10
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 11
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 12
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 13
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 14
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 15
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 16
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 17
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 18
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 19
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 20
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 21
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 22
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 23
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 24
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 25
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 26
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 27
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 28
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 29
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 30
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1
# No. 31
    pushq   $IHDLRIDX
    jmp     CommonEntryPoint
.equ  IHDLRIDX, IHDLRIDX + 1


    pushq   $IHDLRIDX
CommonEntryPoint:
    .byte   0x40, 0xf6, 0xc4, 0x08    #test    spl, 8
    jnz     L1
    pushq   (%rsp)
    movq    $0, 8(%rsp)
L1:
    pushq   %rbp
    movq    %rsp, %rbp

    #
    # Since here the stack pointer is 16-byte aligned, so
    # EFI_FX_SAVE_STATE_X64 of EFI_SYSTEM_CONTEXT_x64
    # is 16-byte aligned
    #       

## UINT64  Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
## UINT64  R8, R9, R10, R11, R12, R13, R14, R15;
    pushq   %r15
    pushq   %r14
    pushq   %r13
    pushq   %r12
    pushq   %r11
    pushq   %r10
    pushq   %r9
    pushq   %r8
    pushq   %rax
    pushq   %rcx
    pushq   %rdx
    pushq   %rbx
    pushq   48(%rbp)                   # RSP
    pushq   (%rbp)                     # RBP
    pushq   %rsi
    pushq   %rdi

## UINT64  Gs, Fs, Es, Ds, Cs, Ss;  insure high 16 bits of each is zero
    movzwq 56(%rbp), %rax
    pushq   %rax                       # for ss
    movzwq  32(%rbp), %rax
    pushq   %rax                       # for cs
    movq    %ds, %rax
    pushq   %rax
    movq    %es, %rax
    pushq   %rax
    movq    %fs, %rax
    pushq   %rax
    movq    %gs, %rax
    pushq   %rax

## UINT64  Rip;
    pushq   24(%rbp)

## UINT64  Gdtr[2], Idtr[2];
    subq    $16, %rsp
    sidt    (%rsp)
    subq    $16, %rsp
    sgdt    (%rsp)

## UINT64  Ldtr, Tr;
    xorq    %rax, %rax
    strw    %ax
    pushq   %rax
    sldtw   %ax
    pushq   %rax

## UINT64  RFlags;
    pushq   40(%rbp)

## UINT64  Cr0, Cr1, Cr2, Cr3, Cr4, Cr8;
    movq    %cr8, %rax
    pushq   %rax
    movq    %cr4, %rax
    orq     $0x208, %rax
    movq    %rax, %cr4
    pushq   %rax
    movq    %cr3, %rax
    pushq   %rax
    movq    %cr2, %rax
    pushq   %rax
    xorq    %rax, %rax
    pushq   %rax
    movq    %cr0, %rax
    pushq   %rax

## UINT64  Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
    movq    %dr7, %rax
    pushq   %rax
    movq    %dr6, %rax
    pushq   %rax
    movq    %dr3, %rax
    pushq   %rax
    movq    %dr2, %rax
    pushq   %rax
    movq    %dr1, %rax
    pushq   %rax
    movq    %dr0, %rax
    pushq   %rax

## FX_SAVE_STATE_X64 FxSaveState;

    subq    $512, %rsp
    movq    %rsp, %rdi
    .byte   0xf, 0xae, 0x7                  # fxsave [rdi]

# UEFI calling convention for x64 requires that Direction flag in EFLAGs is clear
    cld

## UINT32  ExceptionData;
    pushq   16(%rbp)

## call into exception handler
    movq    8(%rbp), %rcx
    movq    ExternalVectorTablePtr, %rax    # get the interrupt vectors base
    movq    (%rax, %rcx, 8),  %rax
    orq     %rax, %rax                      # NULL?

    je      nonNullValue;

## Prepare parameter and call
    movq    %rsp, %rdx
    #
    # Per X64 calling convention, allocate maximum parameter stack space
    # and make sure RSP is 16-byte aligned
    #
    subq    $4 * 8 + 8, %rsp
    call    *%rax
    addq    $4 * 8 + 8, %rsp
    jmp     L5

nonNullValue:
# CpuDeadLoop() is the default exception handler since it preserves the processor
# branch log.
    call    ASM_PFX(CpuDeadLoop)

L5:
## UINT64  ExceptionData;
    addq    $8, %rsp

## FX_SAVE_STATE_X64 FxSaveState;

    movq    %rsp, %rsi
    .byte   0xf, 0xae, 0xe                 # fxrstor [rsi]
    addq    $512, %rsp

## UINT64  Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
## Skip restoration of DRx registers to support in-circuit emualators
## or debuggers set breakpoint in interrupt/exception context
    addq    $8 * 6, %rsp 

## UINT64  Cr0, Cr1, Cr2, Cr3, Cr4, Cr8;
    popq    %rax
    movq    %rax, %cr0
    addq    $8, %rsp                      # not for Cr1
    popq    %rax
    movq    %rax, %cr2
    popq    %rax
    movq    %rax, %cr3
    popq    %rax
    movq    %rax, %cr4
    popq    %rax
    movq    %rax, %cr8

## UINT64  RFlags;
    popq    40(%rbp)

## UINT64  Ldtr, Tr;
## UINT64  Gdtr[2], Idtr[2];
## Best not let anyone mess with these particular registers...
    addq    $48, %rsp

## UINT64  Rip;
    popq    24(%rbp)

## UINT64  Gs, Fs, Es, Ds, Cs, Ss;
    popq    %rax
    # mov   gs, rax ; not for gs
    popq    %rax
    # mov   fs, rax ; not for fs
    # (X64 will not use fs and gs, so we do not restore it)
    popq    %rax
    movq    %rax, %es
    popq    %rax
    movq    %rax, %ds
    popq    32(%rbp)                      # for cs
    popq    56(%rbp)                      # for ss

## UINT64  Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
## UINT64  R8, R9, R10, R11, R12, R13, R14, R15;
    popq    %rdi
    popq    %rsi
    addq    $8, %rsp                      # not for rbp
    popq    48(%rbp)                      # for rsp
    popq    %rbx
    popq    %rdx
    popq    %rcx
    popq    %rax
    popq    %r8
    popq    %r9
    popq    %r10
    popq    %r11
    popq    %r12
    popq    %r13
    popq    %r14
    popq    %r15

    movq    %rbp, %rsp

# Set single step DB# if SMM profile is enabled and page fault exception happens
    movabsq $ASM_PFX(FeaturePcdGet (PcdCpuSmmProfileEnable)), %rbp
    cmpb    $0, (%rbp)
    jz      Done
# Check if this is page fault exception
    cmpq    $0xe, 8(%rsp)
    jnz     L6
# Enable TF bit after page fault handler runs
    btsl    $8, 40(%rsp)                 #RFLAGS
    jmp     Done
L6:
# Check if this is INT 1 exception
    cmpq    $1, 8(%rsp)
    jnz     Done
# Clear TF bit after INT1 handler runs
    btcl    $8, 40(%rsp)                 #RFLAGS    

Done:

    popq    %rbp
    addq    $16, %rsp                    # skip INT# & ErrCode
    iretq

ASM_GLOBAL ASM_PFX(InitializeIDT)
ASM_PFX(InitializeIDT):
    movl     $((_SmiIDTEnd - _SmiIDT) >> 2), %ecx
    movabsq  $_SmiIDT - 16, %rdx
    movabsq  $_SmiExceptionEntryPoints - 4, %r10
L2:
    lea      (%r10, %rcx), %rax
    movw     %ax, (%rdx, %rcx, 4)
    shr      $16, %rax
    movw     %ax, 6(%rdx, %rcx, 4)
    shr      $16, %rax
    movl     %eax, 8(%rdx, %rcx, 4)
    addl     $-4, %ecx
    jnz      L2


# Get absolute address.
    movabsq  $ASM_PFX(FeaturePcdGet (PcdCpuSmmStackGuard)), %rax
    cmpb     $0, (%rax)
    jz       L3

#
# If SMM Stack Guard feature is enabled, set the IST field of
# the interrupe gate for Page Fault Exception to be 1
#
    movabsq  $_SmiIDT + 14 * 16, %rax
    movb     $1, 4(%rax)
L3:
#
# Save Page Fault IDT entry in gPageFaultIdtEntry
#
    movabsq  $_SmiIDT + 14 * 16, %rcx
    movabsq  $ASM_PFX(gSavedPageFaultIdtEntry), %rdx
    movq    (%rcx), %rax
    movq    %rax, (%rdx)
    movq    8(%rcx), %rax
    movq    %rax, 8(%rdx)

    movabsq  $ASM_PFX(FeaturePcdGet (PcdCpuSmmProfileEnable)), %rax
    cmpb     $0, (%rax)
    jz       L4

#
# Save INT 1 IDT entry in gSavedDebugExceptionIdtEntry
#
    movabsq  $_SmiIDT + 1 * 16, %rcx
    movabsq  $ASM_PFX(gSavedDebugExceptionIdtEntry), %rdx
    movq    (%rcx), %rax
    movq    %rax, (%rdx)
    movq    8(%rcx), %rax
    movq    %rax, 8(%rdx)    
    
L4:
    ret
