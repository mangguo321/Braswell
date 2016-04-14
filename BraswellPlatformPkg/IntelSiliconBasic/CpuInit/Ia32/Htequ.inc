;; @file
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

VacantFlag             Equ   00h
NotVacantFlag          Equ   0ffh
StartupApSignal        Equ   6E750000h
MonitorFilterSize      Equ   10h
ApCounterInit          Equ   0
ApInHltLoop            Equ   1
ApInMwaitLoop          Equ   2
ApInRunLoop            Equ   3

LockLocation        equ        1000h - 0400h
StackStart          equ        LockLocation + 4h
StackSize           equ        LockLocation + 8h
RendezvousProc      equ        LockLocation + 0Ch
GdtrProfile         equ        LockLocation + 10h
IdtrProfile         equ        LockLocation + 16h
BufferStart         equ        LockLocation + 1Ch
Cr3Location         equ        LockLocation + 20h
InitFlag            equ        LockLocation + 24h 
WakeUpApManner      equ        LockLocation + 28h
BistBuffer          equ        LockLocation + 2Ch
PAUSE32   MACRO
            DB      0F3h
            DB      090h
            ENDM

;-------------------------------------------------------------------------------
