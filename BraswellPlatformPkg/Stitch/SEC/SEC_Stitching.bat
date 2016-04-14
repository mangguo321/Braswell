@REM @file
@REM
@REM Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
@REM This program and the accompanying materials
@REM are licensed and made available under the terms and conditions of the BSD License
@REM which accompanies this distribution.  The full text of the license may be found at
@REM http://opensource.org/licenses/bsd-license.php
@REM
@REM THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
@REM WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
@REM

@echo off
SetLocal EnableDelayedExpansion EnableExtensions

:: Require 2 input parameters
if "%~2"==""  goto Usage

:: Assign initial values
set exitCode=0
set BIOS_Rom=%~f1
set SEC_Version=%~2
set Platform_Type=%~3
set IFWI_Name=%~4

if "%~3"=="" (
  REM remove the leading path if present, then save only 1st 4 chars of filename
  set Platform_Type=%~n1
  set Platform_Type=!Platform_Type:~0,4!
)

if "%~4"=="" (
  set hour=%time: =0%
  reg copy "HKCU\Control Panel\International" "HKCU\Control Panel\International_Temp" /f >nul
  reg add "HKCU\Control Panel\International" /v sShortDate /d "yyyy-MM-dd" /f >nul
  for /f "tokens=1" %%i in ("%date%") do set today=%%i
  reg copy "HKCU\Control Panel\International_Temp" "HKCU\Control Panel\International" /f >nul
  reg delete "HKCU\Control Panel\International_Temp" /f >nul
  set today=%today:-=_%
  set IFWI_Name=%~n1_SEC_%SEC_Version%_%today%_%hour:~0,2%%time:~3,2%
)

echo =========================================================================
echo Start Stitching SEC ...
echo BIOS_Rom = %BIOS_Rom%;
echo Platform_Type = %Platform_Type%;  SEC_Version = %SEC_Version%;
echo IFWI_Name = %IFWI_Name%;
echo.

if not exist %BIOS_Rom% (
  echo Error: BIOS %Bios_Rom% not found.
  goto TxeFail
)

if x%IFWI_Name:_SV_=%==x%IFWI_Name% (
  if /i "!Platform_Type!"=="CHTT" (
    goto SkipFTool
  )
  if /i "!Platform_Type!"=="BSW" (
    if not exist %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc.xml (
      echo Error: %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc.xml not found.
      goto ContinueWithFtool
    )
    goto SkipFTool
  )
)

:ContinueWithFtool

::
:: execute below cmds from Stitch/SEC/ directory
::
REM FTOOL usage -- For PO and SV use only.
if /i "!Platform_Type!"=="CHTT" (
  if exist %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml (
    FTool\ftool.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml^
      /b /o ..\%IFWI_Name%_SecEnabled.bin^
      /me  %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin^
      /bios %BIOS_Rom%
  )
)

if /i "!Platform_Type!"=="BSW" (
  if exist %Platform_Type%\%SEC_Version%\Production_CHV_SEC_REGION.bin (
    if exist %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml (
      FTool\ftool.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml^
        /b /o ..\%IFWI_Name%_Production_SecEnabled.bin^
        /me  %Platform_Type%\%SEC_Version%\Production_CHV_SEC_REGION.bin^
        /bios %BIOS_Rom%
    )
  )
  if exist %Platform_Type%\%SEC_Version%\PreProduction_CHV_SEC_REGION.bin (
    if exist %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml (
      FTool\ftool.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml^
        /b /o ..\%IFWI_Name%_PreProduction_SecEnabled.bin^
        /me  %Platform_Type%\%SEC_Version%\PreProduction_CHV_SEC_REGION.bin^
        /bios %BIOS_Rom%
    )
  )
  if exist %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin (
    if exist %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml (
      FTool\ftool.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled.xml^
        /b /o ..\%IFWI_Name%_SecEnabled.bin^
        /me  %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin^
        /bios %BIOS_Rom%
    )
  )
)

goto SkipFITC

:SkipFtool
:: Always generate the SecEnabled IFWI
if /i "!Platform_Type!"=="BSW" (
  if exist %Platform_Type%\%SEC_Version%\PreProduction_CHV_SEC_REGION.bin (
    %Platform_Type%\%SEC_Version%\FITC\fitc.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc.xml^
    /b /o ..\%IFWI_Name%_PreProduction_SecEnabled.bin^
    /txe  %Platform_Type%\%SEC_Version%\PreProduction_CHV_SEC_REGION.bin^
    /bios %BIOS_Rom%^
    /fpf  %Platform_Type%\%SEC_Version%\FpfMirrorNvarValues.txt
  )
  if exist %Platform_Type%\%SEC_Version%\Production_CHV_SEC_REGION.bin (
    %Platform_Type%\%SEC_Version%\FITC\fitc.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc.xml^
    /b /o ..\%IFWI_Name%_Production_SecEnabled.bin^
    /txe  %Platform_Type%\%SEC_Version%\Production_CHV_SEC_REGION.bin^
    /bios %BIOS_Rom%^
    /fpf  %Platform_Type%\%SEC_Version%\FpfMirrorNvarValues.txt
  )
  if exist %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin (
    %Platform_Type%\%SEC_Version%\FITC\fitc.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc.xml^
    /b /o ..\%IFWI_Name%_SecEnabled.bin^
    /txe  %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin^
    /bios %BIOS_Rom%^
    /fpf  %Platform_Type%\%SEC_Version%\FpfMirrorNvarValues.txt
  )
)
if /i "!Platform_Type!"=="CHTT" (
  FITC\fitc.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc.xml^
    /b /o ..\%IFWI_Name%_SecEnabled.bin^
    /txe  %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin^
    /bios %BIOS_Rom%^
    /fpf  %Platform_Type%\%SEC_Version%\FpfMirrorNvarValues.txt
  
  FITC\fitc.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc_FFD.xml^
    /b /o ..\%IFWI_Name%_SecEnabled_FFD.bin^
    /txe  %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin^
    /bios %BIOS_Rom%^
    /fpf  %Platform_Type%\%SEC_Version%\FpfMirrorNvarValues.txt
  
  FITC\fitc.exe  %Platform_Type%\%SEC_Version%\%Platform_Type%_8MB_SPI_SecEnabled_fitc_CR.xml^
    /b /o ..\%IFWI_Name%_SecEnabled_CR.bin^
    /txe  %Platform_Type%\%SEC_Version%\CHV_SEC_REGION.bin^
    /bios %BIOS_Rom%^
    /fpf  %Platform_Type%\%SEC_Version%\FpfMirrorNvarValues.txt

)

:SkipFITC

:: Remove the intermediate map files.
del /f/q ..\*.map >nul 2>&1
del /f/q ..\*MAP.txt >nul 2>&1

echo.
echo SEC Stitching Finished!
echo =========================================================================
goto Exit

:Usage
echo Script to stitch SEC FW with BIOS.
echo.
echo Usage: SEC_Stitching.bat ^<BIOS.ROM^> ^<SEC_Version^> [Platform_Type] [IFWI_Name]
echo.
echo        BIOS.ROM:       The BIOS rom file to use for the IFWI (path needs to be relative to this Script.)
echo        SEC_Version:    Version of the SeC FW binary and FITC tool to use (eg. 1.0.0.1027)
echo        Platform_Type:  CHTS, CHTT, or CHTM  (default: first 4 Char of BIOS.rom filename)
echo        IFWI_Name:      The name to use for the IFWI created by FITC. (default: BIOS.ver_SEC.ver_Date_Time)
echo                        Output will be "IFWI_Name"_SecEnabled.bin and "IFWI_Name"_SecDisabled.bin
echo                        Note: Must define [Platform_Type] when using this options
echo                        Eg. "SEC_Stitching.bat BIOS.rom SecVersion IFWI_Name"  is Not valid.
echo.

:TxeFail
set exitCode=1

:Exit
exit /b %exitCode%
EndLocal
