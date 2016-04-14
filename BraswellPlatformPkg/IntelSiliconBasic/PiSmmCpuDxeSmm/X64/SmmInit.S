## @file
#  Functions for relocating SMBASE's for all processors
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


ASM_GLOBAL   ASM_PFX(gSmmCr0)
ASM_GLOBAL   ASM_PFX(gSmmCr3)
ASM_GLOBAL   ASM_PFX(gSmmCr4)
ASM_GLOBAL   ASM_PFX(gcSmmInitTemplate)
ASM_GLOBAL   ASM_PFX(gcSmmInitSize)
ASM_GLOBAL   ASM_PFX(gSmmJmpAddr)
ASM_GLOBAL   ASM_PFX(SmmRelocationSemaphoreComplete)
ASM_GLOBAL   ASM_PFX(SmmRelocationSemaphoreComplete32)
ASM_GLOBAL   ASM_PFX(mRebasedFlagAddr32)
ASM_GLOBAL   ASM_PFX(mSmmRelocationOriginalAddressPtr32)
ASM_GLOBAL   ASM_PFX(gSmmInitStack)

    .data

NullSeg:    .quad      0
CodeSeg64:
            .word -1                    # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x9b
            .byte 0xaf                  # LimitHigh
            .byte 0                     # BaseHigh
.equ  GDT_SIZE, . - NullSeg

    .text

GdtDesc:
            .word      GDT_SIZE
            .quad      NullSeg

SmmStartup:
    .byte 0x66,0xb8                     # mov eax, imm32
ASM_PFX(gSmmCr3):    .space     4
    movq    %rax, %cr3
    .byte 0x66,0x2e
    lgdt    (GdtDesc - SmmStartup)(%ebp)
    .byte 0x66,0xb8                     # mov eax, imm32
ASM_PFX(gSmmCr4):    .space     4
    movq    %rax, %cr4
    .byte 0x66
    movl    $0xc0000080,%ecx            # IA32_EFER MSR
    rdmsr
    orb     $1,%ah                      # set LME bit
    wrmsr
    .byte 0x66,0xb8                     # mov eax, imm32
ASM_PFX(gSmmCr0):    .space     4
    movq    %rax, %cr0
    .byte 0x66,0xea                     # far jmp to long mode
ASM_PFX(gSmmJmpAddr): .quad      LongMode
LongMode:                               # long-mode starts here
    .byte 0x48,0xbc                     # mov rsp, imm64
ASM_PFX(gSmmInitStack):  .space  8
    andw  $0xfff0, %sp                  # make sure RSP is 16-byte aligned
    addq  $-0x20, %rsp
    call  ASM_PFX(SmmInitHandler)
    addq  $0x20, %rsp
    rsm

ASM_PFX(gcSmmInitTemplate):

_SmmInitTemplate:
    .byte 0x66,0x2e,0x8b,0x2e           # mov ebp, cs:[@F]
    .word L1 - _SmmInitTemplate + 0x8000
    .byte 0x66, 0x81, 0xed, 0, 0, 3, 0  # sub ebp, 0x30000
    jmp     *%bp                        # jmp ebp actually
L1:
    .quad      SmmStartup

ASM_PFX(gcSmmInitSize):   .word  . - ASM_PFX(gcSmmInitTemplate)

ASM_PFX(SmmRelocationSemaphoreComplete):
    # Create a simple stack frame to store RAX and the original RSM location
    pushq   %rax  # Used to store return address
    pushq   %rax

    # Load the original RSM location onto stack
    movabsq $ASM_PFX(mSmmRelocationOriginalAddress), %rax
    movq    (%rax), %rax
    movq    %rax, 0x08(%rsp)

    # Update rebase flag
    movabsq $ASM_PFX(mRebasedFlag), %rax
    movq    (%rax), %rax
    movb    $1, (%rax)

    #restore RAX and return to original RSM location
    popq    %rax
    retq

#
# Semaphore code running in 32-bit mode
#
ASM_PFX(SmmRelocationSemaphoreComplete32):
    #
    # movb $1, ()
    #
    .byte   0xc6, 0x05
ASM_PFX(mRebasedFlagAddr32):
    .long   0
    .byte   1
    #
    # jmpd ()
    #
    .byte   0xff, 0x25
ASM_PFX(mSmmRelocationOriginalAddressPtr32):
    .long   0

