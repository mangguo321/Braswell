;; @file
;  Functions for relocating SMBASE's for all processors
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

    .686p
    .xmm
    .model  flat,C

SmmInitHandler  PROTO   C

EXTERNDEF   C   gSmmCr0:DWORD
EXTERNDEF   C   gSmmCr3:DWORD
EXTERNDEF   C   gSmmCr4:DWORD
EXTERNDEF   C   gcSmmInitTemplate:BYTE
EXTERNDEF   C   gcSmmInitSize:WORD
EXTERNDEF   C   gSmmJmpAddr:QWORD
EXTERNDEF   C   mRebasedFlag:PTR BYTE
EXTERNDEF   C   mSmmRelocationOriginalAddress:DWORD
EXTERNDEF   C   gSmmInitStack:DWORD

    .data

NullSeg     DQ      0                   ; reserved by architecture
DataSeg32   LABEL   QWORD
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      93h
            DB      0cfh                ; LimitHigh
            DB      0                   ; BaseHigh
CodeSeg32   LABEL   QWORD
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      9bh
            DB      0cfh                ; LimitHigh
            DB      0                   ; BaseHigh
GDT_SIZE = $ - offset NullSeg

    .code

GdtDesc     LABEL   FWORD
            DW      GDT_SIZE
            DD      offset NullSeg

SmmStartup  PROC
    DB      66h, 0b8h
gSmmCr3     DD      ?
    mov     cr3, eax
    DB      67h, 66h
    lgdt    fword ptr cs:[ebp + (offset GdtDesc - SmmStartup)]
    DB      66h, 0b8h
gSmmCr4     DD      ?
    mov     cr4, eax
    DB      66h, 0b8h
gSmmCr0     DD      ?
    DB      0bfh, 8, 0                  ; mov di, 8
    mov     cr0, eax
    DB      66h, 0eah                   ; jmp far [ptr48]
gSmmJmpAddr LABEL   QWORD
    DD      @32bit
    DW      10h
@32bit:
    mov     ds, edi
    mov     es, edi
    mov     fs, edi
    mov     gs, edi
    mov     ss, edi
    DB      0bch                        ; mov esp, imm32
gSmmInitStack  DD ?
    call    SmmInitHandler
    rsm
SmmStartup  ENDP

gcSmmInitTemplate   LABEL   BYTE

_SmmInitTemplate    PROC
    DB      66h
    mov     ebp, SmmStartup
    DB      66h, 81h, 0edh, 00h, 00h, 03h, 00  ; sub ebp, 30000h
    jmp     bp                          ; jmp ebp actually
_SmmInitTemplate    ENDP

gcSmmInitSize   DW  $ - gcSmmInitTemplate

SmmRelocationSemaphoreComplete PROC
    push    eax
    mov     eax, mRebasedFlag
    mov     byte ptr [eax], 1
    pop     eax
    jmp     [mSmmRelocationOriginalAddress]
SmmRelocationSemaphoreComplete ENDP
    END
