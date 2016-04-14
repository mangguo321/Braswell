;; @file
;
;  This is the equates file for HT (Hyper-threading) support
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

LockLocation                  equ        1000h - 0200h
StackStartAddressLocation     equ        LockLocation + 04h
StackSizeLocation             equ        LockLocation + 08h
CProcedureLocation            equ        LockLocation + 0Ch
GdtrLocation                  equ        LockLocation + 10h
IdtrLocation                  equ        LockLocation + 16h
BufferStartLocation           equ        LockLocation + 1Ch
PmodeOffsetLocation           equ        LockLocation + 20h
AcpiCpuDataAddressLocation    equ        LockLocation + 24h
MtrrValuesAddressLocation     equ        LockLocation + 28h
FinishedCountAddressLocation  equ        LockLocation + 2Ch
SerializeLockAddressLocation  equ        LockLocation + 30h
MicrocodeAddressLocation      equ        LockLocation + 34h
BootScriptAddressLocation     equ        LockLocation + 38h
StartStateLocation            equ        LockLocation + 3Ch

PAUSE32   MACRO
            DB      0F3h
            DB      090h
            ENDM

;-------------------------------------------------------------------------------
