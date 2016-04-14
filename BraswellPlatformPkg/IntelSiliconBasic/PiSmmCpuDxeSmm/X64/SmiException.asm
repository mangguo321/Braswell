;; @file
;  Exception handlers used in SM mode
;
;  Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>
;
;  This program and the accompanying materials
;  are licensed and made available under the terms and conditions of the BSD License
;  which accompanies this distribution.  The full text of the license may be found at
;  http://opensource.org/licenses/bsd-license.php.
;
;  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
;  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;;

EXTERNDEF   gSmiMtrrs:QWORD
EXTERNDEF   gcSmiIdtr:FWORD
EXTERNDEF   gcSmiGdtr:FWORD
EXTERNDEF   gcPsd:BYTE
EXTERNDEF   gPhyMask:QWORD
EXTERNDEF   FeaturePcdGet (PcdCpuSmmStackGuard):BYTE
EXTERNDEF   gSavedPageFaultIdtEntry:QWORD
EXTERNDEF   gSavedDebugExceptionIdtEntry:QWORD
EXTERNDEF   FeaturePcdGet (PcdCpuSmmProfileEnable):BYTE

CpuDeadLoop     PROTO

    .const

NullSeg     DQ      0                   ; reserved by architecture
            DQ      0                   ; reserved for future use
CodeSeg32   LABEL   QWORD
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      9bh
            DB      0cfh                ; LimitHigh
            DB      0                   ; BaseHigh
DataSeg32   LABEL   QWORD
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      93h
            DB      0cfh                ; LimitHigh
            DB      0                   ; BaseHigh
            DQ      0                   ; reserved for future use
CodeSeg16   LABEL   QWORD
            DW      -1
            DW      0
            DB      0
            DB      9bh
            DB      8fh
            DB      0
DataSeg16   LABEL   QWORD
            DW      -1
            DW      0
            DB      0
            DB      93h
            DB      8fh
            DB      0
CodeSeg64   LABEL   QWORD
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      9bh
            DB      0afh                ; LimitHigh
            DB      0                   ; BaseHigh
; TSS Segment for X64 specially
TssSeg      LABEL   QWORD
            DW      TSS_DESC_SIZE       ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      89h
            DB      080h                ; LimitHigh
            DB      0                   ; BaseHigh
            DD      0                   ; BaseUpper
            DD      0                   ; Reserved
GDT_SIZE = $ - offset NullSeg

; Create TSS Descriptor just after GDT
TssDescriptor LABEL BYTE
            DD      0                   ; Reserved
            DQ      0                   ; RSP0
            DQ      0                   ; RSP1
            DQ      0                   ; RSP2
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DQ      0                   ; IST1
            DQ      0                   ; IST2
            DQ      0                   ; IST3
            DQ      0                   ; IST4
            DQ      0                   ; IST5
            DQ      0                   ; IST6
            DQ      0                   ; IST7
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DW      0                   ; Reserved
            DW      0                   ; I/O Map Base Address
TSS_DESC_SIZE = $ - offset TssDescriptor

;
; This structure serves as a template for all processors.
;
gcPsd     LABEL   BYTE
            DB      'PSDSIG  '
            DW      PSD_SIZE
            DW      2
            DW      1 SHL 2
            DW      CODE_SEL
            DW      DATA_SEL
            DW      DATA_SEL
            DW      DATA_SEL
            DW      0
            DQ      0
            DQ      0
            DQ      0                   ; fixed in InitializeMpServiceData()
            DQ      offset NullSeg
            DD      GDT_SIZE
            DD      0
            DB      24 dup (0)
            DQ      offset gSmiMtrrs
PSD_SIZE  = $ - offset gcPsd

;
; CODE & DATA segments for SMM runtime
;
CODE_SEL    = offset CodeSeg64 - offset NullSeg
DATA_SEL    = offset DataSeg32 - offset NullSeg

gcSmiGdtr   LABEL   FWORD
    DW      GDT_SIZE - 1
    DQ      offset NullSeg

gcSmiIdtr   LABEL   FWORD
    DW      IDT_SIZE - 1
    DQ      offset _SmiIDT

    .data

;
; Here is the IDT. There are 32 (not 255) entries in it since only processor
; generated exceptions will be handled.
;
_SmiIDT:
REPEAT      32
    DW      0                           ; Offset 0:15
    DW      CODE_SEL                    ; Segment selector
    DB      0                           ; Unused
    DB      8eh                         ; Interrupt Gate, Present
    DW      0                           ; Offset 16:31
    DQ      0                           ; Offset 32:63
            ENDM
_SmiIDTEnd:

IDT_SIZE = (offset _SmiIDTEnd - offset _SmiIDT)

;
;  Saved IDT Entry for Page Fault
;
gSavedPageFaultIdtEntry    LABEL   QWORD
    DQ      0                        
    DQ      0 

;
;  Saved IDT Entry for INT 1
;
gSavedDebugExceptionIdtEntry    LABEL   QWORD
    DQ      0                        
    DQ      0 

ExternalVectorTablePtr QWORD 0 ; point to the external interrupt vector table

;
; Here are the global variables used by #PF exception handler.
;
_PFPML4     DD      0
_PFPDP      DD      0
_PFLOCK     DB      0

    .code

InitializeSmmExternalVectorTablePtr PROC 
    mov     ExternalVectorTablePtr, rcx
    ret
InitializeSmmExternalVectorTablePtr ENDP

;------------------------------------------------------------------------------
; _SmiExceptionEntryPoints is the collection of exception entrypoints followed
; by a common exception handler.
;
; Stack frame would be as follows as specified in IA32 manuals:
;
; +---------------------+ <-- 16-byte aligned ensured by processor
; +    Old SS           +
; +---------------------+
; +    Old RSP          +
; +---------------------+
; +    RFlags           +
; +---------------------+
; +    CS               +
; +---------------------+
; +    RIP              +
; +---------------------+
; +    Error Code       +
; +---------------------+
; +   Vector Number     +
; +---------------------+
; +    RBP              +
; +---------------------+ <-- RBP, 16-byte aligned
;
; RSP set to odd multiple of 8 at @CommonEntryPoint means ErrCode PRESENT
;------------------------------------------------------------------------------
_SmiExceptionEntryPoints    PROC
IHDLRIDX    = 0
REPEAT      31                          ; handler for INT0~INT30
    push    IHDLRIDX
    jmp     @CommonEntryPoint
IHDLRIDX    = IHDLRIDX + 1
            ENDM
    push    IHDLRIDX                    ; handler for INT31
@CommonEntryPoint:
    test    spl, 8                      ; odd multiple of 8 => ErrCode present
    jnz     @F
    push    [rsp]                       ; duplicate INT# if no ErrCode
    mov     qword ptr [rsp + 8], 0
@@:
    push    rbp
    mov     rbp, rsp

    ;
    ; Since here the stack pointer is 16-byte aligned, so
    ; EFI_FX_SAVE_STATE_X64 of EFI_SYSTEM_CONTEXT_x64
    ; is 16-byte aligned
    ;       

;; UINT64  Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
;; UINT64  R8, R9, R10, R11, R12, R13, R14, R15;
    push    r15
    push    r14
    push    r13
    push    r12
    push    r11
    push    r10
    push    r9
    push    r8
    push    rax
    push    rcx
    push    rdx
    push    rbx
    push    qword ptr [rbp + 48]  ; RSP
    push    qword ptr [rbp]       ; RBP
    push    rsi
    push    rdi

;; UINT64  Gs, Fs, Es, Ds, Cs, Ss;  insure high 16 bits of each is zero
    movzx   rax, word ptr [rbp + 56]
    push    rax                      ; for ss
    movzx   rax, word ptr [rbp + 32]
    push    rax                      ; for cs
    mov     rax, ds
    push    rax
    mov     rax, es
    push    rax
    mov     rax, fs
    push    rax
    mov     rax, gs
    push    rax

;; UINT64  Rip;
    push    qword ptr [rbp + 24]

;; UINT64  Gdtr[2], Idtr[2];
    sub     rsp, 16
    sidt    fword ptr [rsp]
    sub     rsp, 16
    sgdt    fword ptr [rsp]

;; UINT64  Ldtr, Tr;
    xor     rax, rax
    str     ax
    push    rax
    sldt    ax
    push    rax

;; UINT64  RFlags;
    push    qword ptr [rbp + 40]

;; UINT64  Cr0, Cr1, Cr2, Cr3, Cr4, Cr8;
    mov     rax, cr8
    push    rax
    mov     rax, cr4
    or      rax, 208h
    mov     cr4, rax
    push    rax
    mov     rax, cr3
    push    rax
    mov     rax, cr2
    push    rax
    xor     rax, rax
    push    rax
    mov     rax, cr0
    push    rax

;; UINT64  Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
    mov     rax, dr7
    push    rax
    mov     rax, dr6
    push    rax
    mov     rax, dr3
    push    rax
    mov     rax, dr2
    push    rax
    mov     rax, dr1
    push    rax
    mov     rax, dr0
    push    rax

;; FX_SAVE_STATE_X64 FxSaveState;

    sub rsp, 512
    mov rdi, rsp
    db 0fh, 0aeh, 00000111y ;fxsave [rdi]

; UEFI calling convention for x64 requires that Direction flag in EFLAGs is clear
    cld

;; UINT32  ExceptionData;
    push    qword ptr [rbp + 16]

;; call into exception handler
    mov     rcx, [rbp + 8]
    mov     rax, ExternalVectorTablePtr  ; get the interrupt vectors base
    mov     rax, [rax + rcx * 8]       
    or      rax, rax                        ; NULL?

    je    nonNullValue;

;; Prepare parameter and call
    mov     rdx, rsp
    ;
    ; Per X64 calling convention, allocate maximum parameter stack space
    ; and make sure RSP is 16-byte aligned
    ;
    sub     rsp, 4 * 8 + 8
    call    rax
    add     rsp, 4 * 8 + 8
    jmp     @F

nonNullValue:
; CpuDeadLoop() is the default exception handler since it preserves the processor
; branch log.
    call    CpuDeadLoop

@@:
;; UINT64  ExceptionData;
    add     rsp, 8

;; FX_SAVE_STATE_X64 FxSaveState;

    mov rsi, rsp
    db 0fh, 0aeh, 00001110y ; fxrstor [rsi]
    add rsp, 512

;; UINT64  Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
;; Skip restoration of DRx registers to support in-circuit emualators
;; or debuggers set breakpoint in interrupt/exception context
  add     rsp, 8 * 6

;; UINT64  Cr0, Cr1, Cr2, Cr3, Cr4, Cr8;
    pop     rax
    mov     cr0, rax
    add     rsp, 8   ; not for Cr1
    pop     rax
    mov     cr2, rax
    pop     rax
    mov     cr3, rax
    pop     rax
    mov     cr4, rax
    pop     rax
    mov     cr8, rax

;; UINT64  RFlags;
    pop     qword ptr [rbp + 40]

;; UINT64  Ldtr, Tr;
;; UINT64  Gdtr[2], Idtr[2];
;; Best not let anyone mess with these particular registers...
    add     rsp, 48

;; UINT64  Rip;
    pop     qword ptr [rbp + 24]

;; UINT64  Gs, Fs, Es, Ds, Cs, Ss;
    pop     rax
    ; mov     gs, rax ; not for gs
    pop     rax
    ; mov     fs, rax ; not for fs
    ; (X64 will not use fs and gs, so we do not restore it)
    pop     rax
    mov     es, rax
    pop     rax
    mov     ds, rax
    pop     qword ptr [rbp + 32]  ; for cs
    pop     qword ptr [rbp + 56]  ; for ss

;; UINT64  Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
;; UINT64  R8, R9, R10, R11, R12, R13, R14, R15;
    pop     rdi
    pop     rsi
    add     rsp, 8               ; not for rbp
    pop     qword ptr [rbp + 48] ; for rsp
    pop     rbx
    pop     rdx
    pop     rcx
    pop     rax
    pop     r8
    pop     r9
    pop     r10
    pop     r11
    pop     r12
    pop     r13
    pop     r14
    pop     r15

    mov     rsp, rbp

; Set single step DB# if SMM profile is enabled and page fault exception happens
    mov     rbp, offset FeaturePcdGet (PcdCpuSmmProfileEnable)
    cmp     byte ptr [rbp], 0
    jz      @Done
; Check if this is page fault exception
    cmp     qword ptr [rsp + 8], 0eh
    jnz     @F
; Enable TF bit after page fault handler runs
    bts     dword ptr [rsp + 40], 8  ;RFLAGS
    jmp     @Done
@@:
; Check if this is INT 1 exception
    cmp     qword ptr [rsp + 8], 1
    jnz     @Done
; Clear TF bit after INT1 handler runs
    btc     dword ptr [rsp + 40], 8  ;RFLAGS    

@Done:

    pop     rbp
    add     rsp, 16           ; skip INT# & ErrCode
    iretq
_SmiExceptionEntryPoints    ENDP

InitializeIDT   PROC
    mov     ecx, (_SmiIDTEnd - _SmiIDT) SHR 2
    lea     rdx, _SmiIDT - 16
    lea     r10, _SmiExceptionEntryPoints - 4
@@:
    lea     rax, [r10 + rcx]            ; rax <- handler address
    mov     [rdx + rcx*4], ax
    shr     rax, 16
    mov     [rdx + rcx*4 + 6], ax
    shr     rax, 16
    mov     [rdx + rcx*4 + 8], eax
    add     ecx, -4
    jnz     @B

; Get absolute address. Avoid RIP relative addressing
    mov     rax, offset FeaturePcdGet (PcdCpuSmmStackGuard)
    cmp     byte ptr [rax], 0
    jz      @F

;
; If SMM Stack Guard feature is enabled, set the IST field of
; the interrupe gate for Page Fault Exception to be 1
;
    lea     rax, _SmiIDT + 14 * 16
    mov     byte ptr [rax + 4], 1
@@:
;
; Save Page Fault IDT entry in gPageFaultIdtEntry
;
    lea     rcx, _SmiIDT + 14 * 16
    lea     rdx, gSavedPageFaultIdtEntry
    mov     rax, [rcx]
    mov     [rdx], rax
    mov     rax, [rcx + 8]
    mov     [rdx + 8], rax

    mov     rax, offset FeaturePcdGet (PcdCpuSmmProfileEnable)
    cmp     byte ptr [rax], 0
    jz      @F

;
; Save INT 1 IDT entry in gSavedDebugExceptionIdtEntry
;
    lea     rcx, _SmiIDT + 1 * 16
    lea     rdx, gSavedDebugExceptionIdtEntry
    mov     rax, [rcx]
    mov     [rdx], rax
    mov     rax, [rcx + 8]
    mov     [rdx + 8], rax
@@:    
    ret
InitializeIDT   ENDP

    END
