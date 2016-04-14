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
ASM_GLOBAL   ASM_PFX(gSmmInitStack)

    .data

NullSeg:    .quad      0
DataSeg32:
            .word -1                    # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x93
            .byte 0xcf                  # LimitHigh
            .byte 0                     # BaseHigh
CodeSeg32:
            .word -1                    # LimitLow
            .word 0                     # BaseLow
            .byte 0                     # BaseMid
            .byte 0x9b
            .byte 0xcf                  # LimitHigh
            .byte 0                     # BaseHigh
.equ  GDT_SIZE, . - NullSeg

    .text

GdtDesc:
            .word      GDT_SIZE
            .long      NullSeg

SmmStartup:
    .byte   0x66,0xb8
ASM_PFX(gSmmCr3):    .space     4
    movl    %eax, %cr3
    .byte   0x67,0x66
    lgdt    %cs:(GdtDesc - SmmStartup)(%ebp)
    .byte   0x66,0xb8
ASM_PFX(gSmmCr4):    .space     4
    movl    %eax, %cr4
    .byte   0x66,0xb8
ASM_PFX(gSmmCr0):    .space     4
    .byte   0xbf,8,0                      # mov di, 8
    movl    %eax, %cr0
    .byte   0x66,0xea                     # jmp far [ptr48]
ASM_PFX(gSmmJmpAddr): .long Start32bit
    .word   0x10
Start32bit:
    movl    %edi,%ds
    movl    %edi,%es
    movl    %edi,%fs
    movl    %edi,%gs
    movl    %edi,%ss
    .byte   0xbc                        # mov esp, imm32
ASM_PFX(gSmmInitStack):  .space  4
    call    ASM_PFX(SmmInitHandler)
    rsm

ASM_PFX(gcSmmInitTemplate):

_SmmInitTemplate:
    .byte 0x66
    movl    $SmmStartup, %ebp
    .byte 0x66, 0x81, 0xed, 0, 0, 3, 0  # sub ebp, 0x30000
    jmp     *%bp                        # jmp ebp actually

ASM_PFX(gcSmmInitSize):   .word  . - ASM_PFX(gcSmmInitTemplate)


ASM_PFX(SmmRelocationSemaphoreComplete):
    pushl   %eax
    movl    ASM_PFX(mRebasedFlag), %eax
    movb    $1, (%eax)
    popl    %eax
    jmp     *ASM_PFX(mSmmRelocationOriginalAddress)
