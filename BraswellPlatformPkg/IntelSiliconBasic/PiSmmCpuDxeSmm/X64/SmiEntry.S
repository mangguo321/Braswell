## @file
#  Code template of the SMI handler for a particular processor
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


ASM_GLOBAL  ASM_PFX(gcSmiHandlerTemplate)
ASM_GLOBAL  ASM_PFX(gcSmiHandlerSize)
ASM_GLOBAL  ASM_PFX(gSmiCr3)
ASM_GLOBAL  ASM_PFX(gcSmiHandlerOffset)
ASM_GLOBAL  ASM_PFX(gSmiStack)
ASM_GLOBAL  ASM_PFX(gSmbase)
ASM_GLOBAL  ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug))

#
# Constants relating to PROCESSOR_SMM_DESCRIPTOR
#
.equ            DSC_OFFSET, 0xfb00
.equ            DSC_GDTPTR, 0x30
.equ            DSC_GDTSIZ, 0x38
.equ            DSC_CS, 14
.equ            DSC_DS, 16
.equ            DSC_SS, 18
.equ            DSC_OTHERSEG, 20
.equ            MSR_DR6,    0x0c05
.equ            MSR_DR7,    0x0c06

.equ            PROTECT_MODE_CS, 0x10
.equ            PROTECT_MODE_DS, 0x18
.equ            TSS_SEGMENT, 0x40
.equ            GDT_SIZE, 0x50

#
# Constants relating to CPU State Save Area
#
.equ            SSM_DR6,   0xffd0
.equ            SSM_DR7,   0xffc8


    .data

 ASM_PFX(gcSmiHandlerOffset):  .word      _SmiHandler - _SmiEntryPoint + 0x8000

    .text

ASM_PFX(gcSmiHandlerTemplate):

_SmiEntryPoint:
    #
    # The encoding of BX in 16-bit addressing mode is the same as of RDI in 64-
    # bit addressing mode. And that coincidence has been used in the following
    # "64-bit like" 16-bit code. Be aware that once RDI is referrenced as a
    # base address register, it is actually BX that is referrenced.
    #
    .byte 0xbb                          # mov bx, imm16
    .word _GdtDesc - _SmiEntryPoint + 0x8000
    #
    # fix GDT TSS table
    #
    .byte   0x66, 0x2e, 0xa1           # mov eax, cs:[offset16] ; eax = GDT base
    .word      DSC_OFFSET + DSC_GDTPTR
    movw    %ax, %dx
    movw    %ax, %bp                    # ebp = GDT base
    addw    $GDT_SIZE, %dx              # edx = TSS descriptor base
    movl    %edx, (TSS_SEGMENT+2)(%eax)
    shr     $16, %dx
    movb    %dl, (TSS_SEGMENT + 4)(%eax)
    movb    %dh, (TSS_SEGMENT + 7)(%eax)
    #
    # fix GDT descriptor
    #
    .byte 0x2e,0xa1                     # mov ax, cs:[offset16]
    .word      DSC_OFFSET + DSC_GDTSIZ
    .byte 0x48                          # dec ax
    .byte 0x2e
    movl    %eax, (%rdi)                # mov cs:[bx], ax
    .byte 0x66,0x2e,0xa1                # mov eax, cs:[offset16]
    .word      DSC_OFFSET + DSC_GDTPTR
    .byte 0x2e
    movw    %ax, 2(%rdi)
    .byte 0x66,0x2e
    lgdt    (%rdi)
    .byte 0x66,0xb8                     # mov eax, imm32
ASM_PFX(gSmiCr3):    .space  4
    movq    %rax, %cr3
    .byte 0x66
    movl    $0x668,%eax                 # as cr4.PGE is not set here, refresh cr3
    movq    %rax, %cr4                  # in PreModifyMtrrs() to flush TLB.
    #
    # Patch LongMode Segment
    #
    .byte 0x2e,0xa1                     # mov ax, cs:[offset16]
    .word      DSC_OFFSET + DSC_CS
    .byte 0x2e
    movl    %eax, (LongMode - ProtectedMode + 4)(%rdi)
    #
    # Patch ProtectedMode Segment
    #
    .byte 0xb8
    .word PROTECT_MODE_CS
    .byte 0x2e
    movl    %eax, -2(%rdi)
    #
    # Patch LongMode entry
    #
    .byte 0x66, 0xbf                    # mov edi, SMBASE
ASM_PFX(gSmbase): .space 4
    lea     ((LongMode - _SmiEntryPoint) + 0x8000)(%edi), %ax
    .byte 0x2e
    movw    %ax, (LongMode - ProtectedMode)(%rdi)
    #
    # Patch ProtectedMode entry
    #
    lea     ((ProtectedMode - _SmiEntryPoint) + 0x8000)(%edi), %ax
    .byte 0x2e
    movw    %ax, -6(%rdi)
    #
    # Switch into ProtectedMode
    #
    .byte 0x66
    movl    $0xc0000080,%ecx

    movq    %cr0, %rbx
    .byte 0x66
    andl    $0x9ffafff3, %ebx
    .byte 0x66
    orl     $0x00000023, %ebx

    movq    %rbx, %cr0
    .byte 0x66, 0xea
    .space 6

_GdtDesc:    .space  6

ProtectedMode:
    movw    $PROTECT_MODE_DS, %ax
    movl    %eax, %ds
    movl    %eax, %es
    movl    %eax, %ss

    #
    # Load TSS
    #
    movl    %ebp, %eax                   # ebp = GDT base
    movl    %eax, %edx
    movb    $0x89, %dl
    movb    %dl, (TSS_SEGMENT + 5)(%rax) # clear busy flag
    movl    $TSS_SEGMENT, %eax
    ltr     %ax

    #
    # Switch to LongMode
    #
    rdmsr
    orb     $1,%ah
    wrmsr
    btsl    $31, %ebx
    movq    %rbx, %cr0
    .byte 0x67, 0xea
    .space  6

LongMode:                               # long mode (64-bit code) starts here
    lea     (DSC_OFFSET)(%rdi), %ebx
    movw    DSC_DS(%rbx), %ax
    movl    %eax,%ds
    movw    DSC_OTHERSEG(%rbx), %ax
    movl    %eax,%es
    movl    %eax,%fs
    movl    %eax,%gs
    movw    DSC_SS(%rbx), %ax
    movl    %eax,%ss
#   jmp     _SmiHandler                 ; instruction is not needed

_SmiHandler:
    .byte 0x48,0xbc                     # mov rsp, imm64
ASM_PFX(gSmiStack):   .space  8
    movabsq $ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug)), %rax
    cmpb    $0, (%rax)
    jz      L1

    jz      L3

L3:
    .byte   0x48, 0x8b, 0x0d            # mov rcx, [rip + disp32]
    .long   SSM_DR6 - (. + 4 - _SmiEntryPoint + 0x8000)
    .byte   0x48, 0x8b, 0x15            # mov rdx, [rip + disp32]
    .long   SSM_DR7 - (. + 4 - _SmiEntryPoint + 0x8000)
L4:
    movq    %rcx, %dr6
    movq    %rdx, %dr7
L1:

    movabsq $ASM_PFX(SmiRendezvous), %rax
    movq    (%rsp), %rcx
    # Save FP registers

    subq    $0x208, %rsp
    .byte   0x48                        # FXSAVE64
    fxsave  (%rsp)

    addq    $-0x20, %rsp
    call    *%rax
    addq    $0x20, %rsp

    #
    # Restore FP registers
    #
    .byte   0x48                        # FXRSTOR64
    fxrstor (%rsp)

    movabsq $ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug)), %rax
    cmpb    $0, (%rax)
    jz      L2

    movq    %dr7, %rdx
    movq    %dr6, %rcx
    .byte   0x48, 0x89, 0x15            # mov [rip + disp32], rdx
    .long   SSM_DR7 - (. + 4 - _SmiEntryPoint + 0x8000)
    .byte   0x48, 0x89, 0x0d            # mov [rip + disp32], rcx
    .long   SSM_DR6 - (. + 4 - _SmiEntryPoint + 0x8000)
L2:

    rsm

ASM_PFX(gcSmiHandlerSize):    .word      . - _SmiEntryPoint

