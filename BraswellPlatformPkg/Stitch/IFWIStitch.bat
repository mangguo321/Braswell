@REM @file
@REM   Windows batch file to build BIOS ROM
@REM
@REM Copyright (c) 2006   - 2016, Intel Corporation. All rights reserved.<BR>
@REM 
@REM   This program and the accompanying materials are licensed and made available under
@REM   the terms and conditions of the BSD License that accompanies this distribution.
@REM   The full text of the license may be found at
@REM   http://opensource.org/licenses/bsd-license.php.
@REM 
@REM   THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
@REM   WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
@REM

@echo off
SetLocal EnableDelayedExpansion EnableExtensions

:: Set script defaults
set exitCode=0
set UpdateGop=1
set UpdateIsh=1
set UpdateMicro=1
set UpdateSEC=1
set BackupRom=1
set SpiLock=0
set Stitch_Config=Stitch_Config.txt
copy /y nul Stitching.log >nul

:: Set default Suffix as:  YYYY_MM_DD_HHMM
set hour=%time: =0%
reg copy "HKCU\Control Panel\International" "HKCU\Control Panel\International_Temp" /f >nul
reg add "HKCU\Control Panel\International" /v sShortDate /d "yyyy_MM_dd" /f >nul
for /f "tokens=1" %%i in ("%date%") do set today=%%i
reg copy "HKCU\Control Panel\International_Temp" "HKCU\Control Panel\International" /f >nul
reg delete "HKCU\Control Panel\International_Temp" /f >nul
set IFWI_Suffix=%today%_%hour:~0,2%%time:~3,2%


:: **********************************************************************
:: Process inputs
:: **********************************************************************
if "%~1"=="?"       goto Usage
if "%~1"=="/?"      goto Usage
if /i "%~1"=="Help" goto Usage

:OptLoop1
if /i "%~1"=="/nG" (
    set UpdateGop=0
    shift
    goto OptLoop1
)
if /i "%~1"=="/nI" (
    set UpdateIsh=0
    shift
    goto OptLoop1
)
if /i "%~1"=="/nM" (
    set UpdateMicro=0
    shift
    goto OptLoop1
)
if /i "%~1"=="/nS" (
    set UpdateSEC=0
    shift
    goto OptLoop1
)
if /i "%~1"=="/nB" (
    set BackupRom=0
    shift
    goto OptLoop1
)
if /i "%~1"=="/yL" (
    set SpiLock=1
    shift
    goto OptLoop1
)

::Require first parameter
if not exist %~1 echo BIOS not found. & goto Usage
set BIOS_Names=%~1
shift

:OptLoop2
if /i "%~1"=="/C" (
    set Stitch_Config=%~2
    shift & shift
    goto OptLoop2
)
if /i "%~1"=="/S" (
    if "%~2"=="" echo No Suffix found.  & goto Usage
    set IFWI_Suffix=%~2
    shift & shift
    goto OptLoop2
)

:: Parse the Stitch_Config File
if not exist %Stitch_Config% (
    echo Stitch Configuration File %Stitch_Config% not found.
    goto ScriptFail
)
for /f "delims== tokens=1,2" %%i in (%Stitch_Config%) do (
    if /i "%%i"=="SEC"        set SEC_Version=%%j
    if /i "%%i"=="GOP"        set GOP_Version=%%j
    if /i "%%i"=="ISH"        set ISH_Version=%%j
    if /i "%%i"=="Microcode"  set MC_Version=%%j
    if /i "%%i"=="Source" (
        if /i "%%j"=="ALPHA" set Source_Prefix=A_
        if /i "%%j"=="BF" set Source_Prefix=BF_
        if /i "%%j"=="BE" set Source_Prefix=BE_
        if /i "%%j"=="PV" set Source_Prefix=PV_
        if /i "%%j"=="SV" set Source_Prefix=SV_
        if /i "%%j"=="PPV" set Source_Prefix=PPV_
    )
)

if %UpdateGop% EQU 0  set GOP_Version=Unchanged
if %UpdateIsh% EQU 0  set ISH_Version=Unchanged
if %UpdateMicro% EQU 0  set MC_Version=Unchanged

if %SpiLock% EQU 1 (
  set IFWI_HEADER_FILE=..\IFWIHeader\IFWI_HEADER_SPILOCK.bin
) else (
  set IFWI_HEADER_FILE=..\IFWIHeader\IFWI_HEADER.bin
)

:: **********************************************************************
:: Stitching Init
:: **********************************************************************
echo %date%  %time% >>Stitching.log 2>&1
echo %date%  %time%
echo.

for %%i in (%BIOS_Names%) do (
    REM  ----- Do NOT use :: for comments Inside of code blocks() -------
    set BIOS_Rom=%%i
    set BIOS_Name=%%~ni
)
set BIOS_Version=!BIOS_Name:~-7,7!
set Platform_Type=!BIOS_Name:~0,4!
    
REM Capitalize and validate the Platform_Type
if /i "!Platform_Type!"=="CHTS" (
  set Platform_Type=CHTS
) else if /i "!Platform_Type!"=="CHTT" (
  set Platform_Type=CHTT
) else if /i "!Platform_Type!"=="CHTI" (
  set Platform_Type=CHTI
) else if /i "!Platform_Type!"=="BRAS" (
        set Platform_Type=BSW
) else (
  echo Error - Unsupported PlatformType: !Platform_Type!
  goto Usage
)
REM search BIOS_Name for Arch substring:  either IA32 or X64
if not "!BIOS_Name!"=="!BIOS_Name:_IA32_=!" (
  set Arch=IA32
) else if not "!BIOS_Name!"=="!BIOS_Name:_X64_=!" (
  set Arch=X64
) else (
  echo Error:  Could not determine Architecture for !BIOS_Rom!
  goto Usage
)
set IFWI_Prefix=!Platform_Type!_IFWI_%Source_Prefix%!Arch!

REM search BIOS_Name for Build_Target substring: either R or D
if not "!BIOS_Name!"=="!BIOS_Name:_R_=!" (
  set Build_Target=Release
  set IFWI_Prefix=!IFWI_Prefix!_R
) else if not "!BIOS_Name!"=="!BIOS_Name:_D_=!" (
  set Build_Target=Debug
  set IFWI_Prefix=!IFWI_Prefix!_D
) else (
  echo Error:  Could not determine Build Target for !BIOS_Rom!
  goto Usage
)

REM Create a BIOS backup before Stitching
if %BackupRom% EQU 1 (
  echo Creating backup of original BIOS rom.
  copy /y !BIOS_Rom! !BIOS_Rom!.orig >nul
)

:: **********************************************************************
:: Stitch IFWI
:: **********************************************************************

echo.  >>Stitching.log
echo ********** Stitching !BIOS_Rom! **********  >>Stitching.log
echo.  >>Stitching.log
echo.
echo Stitching IFWI for !BIOS_Rom! ...
echo ---------------------------------------------------------------------------
echo IFWI Header: !IFWI_HEADER_FILE!  SEC Version: %SEC_Version%,   BIOS Version: !BIOS_Version!
echo GOP Version: %GOP_Version%,       ISH Version: %ISH_Version%,   Microcode Version: %MC_Version%  
echo Platform Type: !Platform_Type!,   IFWI Prefix: !IFWI_Prefix!,   IFWI Suffix: %IFWI_Suffix%
echo ---------------------------------------------------------------------------
echo.
echo ------------------------------
echo.
:: Deleted PDR related lines

if %UpdateIsh% EQU 1 (
  echo.
  echo ------------------------------
  echo.
  echo   Merging Ish Fw...
  pushd ISH
  call ISH_Merging.bat ..\!BIOS_Rom!  %ISH_Version% !Platform_Type! >>..\Stitching.log 
  echo Ish Fw Merge done
  popd
  if !ERRORLEVEL! NEQ 0 (
    echo. & echo  -- Error Updating ISH FW ^^!
    goto ScriptFail
  )
)
if %UpdateGop% EQU 1 (
  echo.
  echo ------------------------------
  echo.
  echo   Merging GoP Driver...
  pushd GOP
  call GOP_Merging.bat ..\!BIOS_Rom!  %GOP_Version%  !Arch!  !Platform_Type! >>..\Stitching.log
  popd
  if !ERRORLEVEL! NEQ 0 (
    echo. & echo  -- Error Updating GOP driver ^^!
    goto ScriptFail
  )
)
if %UpdateMicro% EQU 1 (
  echo.
  echo ------------------------------
  echo.
  echo   Patching Unified Microcode patch...
  pushd Microcode
  call Microcode_Patching.bat ..\!BIOS_Rom! "%MC_Version%" !Platform_Type!  >>..\Stitching.log 2>&1
  popd
  if !ERRORLEVEL! NEQ 0 (
    echo. & echo  -- Error Updating Microcode ^^!
    goto ScriptFail
  )
)


if %UpdateSEC% EQU 1 (
  echo.
  echo ------------------------------
  echo.
  echo   Stitching with SEC Firmware...
  pushd SEC  
  copy /b/y !IFWI_HEADER_FILE!+.\BSW\%SEC_Version%\SEC_Region.bin+.\BSW\%SEC_Version%\Vacant.bin+..\!BIOS_Rom! ..\%BIOS_ID%.bin
  popd
  if !ERRORLEVEL! NEQ 0 (
    echo. & echo  -- Error during Sec_Stitching ^^!
    goto ScriptFail
  )
) else echo. & echo /nS detected - Skipping SeC Stitching.


echo.
echo ------------------------------
echo.

echo.
echo -------------------------------------------------
echo.
echo IFWI Successfully Stitched and Checking PASSED ^^!
echo.
echo -------------------------------------------------
echo.
echo ===========================================================================
goto Exit

:Usage
echo.
echo **************************************************************************************************
echo This Script is used to Stitch together BIOS, GOP Driver,Ish Fw, Microcode Patch and TXE FW
echo into a single Integrated Firmware Image (IFWI).
echo.
echo Usage: IFWIStitch.bat [flags] BIOS.ROM [/C Stitch_Config] [/S IFWI_Suffix]
echo.
echo    This script has NO Required arguments, so that the user can just double click from the GUI.
echo    However, this requires that the BIOS.ROM file name is formatted correctly.
echo    It must begin with (CHTS||CHTT||CHTM) and contain (_IA32_||_X64_) and (_R_||_D_).
echo    Eg. CHTTxxxx_IA32_xxxx_R_xxxx.ROM,  or  CHTTxxxx_D_xxxx_X64_xxxx.ROM
echo.
echo    /nG             Do NOT update the GOP driver.  (applies to all ROM files for this run)
echo    /nI             Do NOT update the ISH Fw.  (applies to all ROM files for this run)
echo    /nM             Do NOT update the Microcode.   (applies to all ROM files for this run)
echo    /nB             Do NOT backup BIOS.ROMs. (Default will backup to BIOS.ROM.Orig)
echo    /nS             Do NOT stitch with SeC.  (No .bin file will be generated)
echo.
echo    BIOS.ROM:       A single BIOS ROM file to use for stitching (Required)
echo.
echo    Stitch_Config:  Text file containing version info of each FW component
echo                    (DEFAULT: Stitch_Config.txt)
echo    IFWI_Suffix:    Suffix to append to the end of the IFWI filename
echo                    (DEFAULT: YYYY_MM_DD_HHMM)
echo.
echo Examples:
echo    IFIWStitch.bat C:/MyRoms/testBIOS.rom            : Stitch ROM with defaults
echo    IFIWStitch.bat ../testBIOS.rom /S test123        : Stitch ROM and add custom suffix
echo    IFIWStitch.bat /nM /nB testBIOS.rom /S test456   : Stitch ROM, keep uCode from .rom, 
echo                                                       don't create backup, and add custom suffix.
echo ****************************************************************************************************
pause
exit /b 1

:ScriptFail
set exitCode=1 

:Exit
echo  -- See Stitching.log for more info. --
echo.
echo %date%  %time% 
echo.
REM pause
exit /b %exitCode%
EndLocal
