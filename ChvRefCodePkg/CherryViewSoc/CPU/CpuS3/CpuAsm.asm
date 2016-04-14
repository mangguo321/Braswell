;; @file
;  This is the code that supports IA32 CPU architectural protocol
;
;  Copyright (c) 2010 - 2015, Intel Corporation. All rights reserved.<BR>
;
;  This program and the accompanying materials
;  are licensed and made available under the terms and conditions of the BSD License
;  which accompanies this distribution.  The full text of the license may be found at
;  http://opensource.org/licenses/bsd-license.php.
;
;  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
;  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
;
;;

include Htequ.inc

.686p
.model  flat        
.code

PAUSE32   MACRO
            DB      0F3h
            DB      090h
            ENDM

EnableMCE  proc near C public

  mov     eax, cr4
  or      eax, 40h
  mov     cr4, eax

  ret

EnableMCE  endp

MpMtrrSynchUpEntry  PROC    NEAR C PUBLIC
    ;
    ; Enter no fill cache mode, CD=1(Bit30), NW=0 (Bit29)
    ;
    mov eax, cr0
    and eax, 0DFFFFFFFh
    or  eax, 040000000h
    mov cr0, eax
    ;
    ; Flush cache
    ;
    wbinvd
    ;
    ; Clear PGE flag Bit 7
    ;
    mov eax, cr4
    mov edx, eax
    and eax, 0FFFFFF7Fh
    mov cr4, eax
    ;
    ; Flush all TLBs
    ;
    mov eax, cr3
    mov cr3, eax
    
    mov eax, edx
    
    ret
    
MpMtrrSynchUpEntry  ENDP
    
MpMtrrSynchUpExit  PROC    NEAR C  PUBLIC

    push    ebp             ; C prolog
    mov     ebp, esp
    ;
    ; Flush all TLBs the second time
    ;
    mov eax, cr3
    mov cr3, eax
    ;
    ; Enable Normal Mode caching CD=NW=0, CD(Bit30), NW(Bit29)
    ;
    mov eax, cr0
    and eax, 09FFFFFFFh
    mov cr0, eax
    ;
    ; Set PGE Flag in CR4 if set
    ;
    mov eax, dword ptr [ebp + 8]
    mov cr4, eax
    
    pop ebp
    
    ret

MpMtrrSynchUpExit  ENDP

;-------------------------------------------------------------------------------
;  AsmAcquireMPLock (&Lock);
;-------------------------------------------------------------------------------
AsmAcquireMPLock   PROC  near C  PUBLIC

        pushad
        mov         ebp,esp

        mov         al, NotVacantFlag
        mov         ebx, dword ptr [ebp+24h]
TryGetLock:
        db 0f0h                       ; opcode for lock instruction
        xchg        al, byte ptr [ebx]
        cmp         al, VacantFlag
        jz          LockObtained

        PAUSE32
        jmp         TryGetLock       

LockObtained:
        popad
        ret
AsmAcquireMPLock   ENDP

;-------------------------------------------------------------------------------
;  AsmReleaseMPLock (&Lock);                                                    
;-------------------------------------------------------------------------------------
AsmReleaseMPLock   PROC  near C  PUBLIC

        pushad
        mov         ebp,esp

        mov         al, VacantFlag
        mov         ebx, dword ptr [ebp+24h]
        db 0f0h                       ; opcode for lock instruction
        xchg        al, byte ptr [ebx]
        
        popad
        ret
AsmReleaseMPLock   ENDP

CpuPause  PROC    NEAR  C  PUBLIC

    PAUSE32
    ret

CpuPause  ENDP

END