@REM @file
@REM   Windows batch file to build BIOS ROM
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

rem @echo off
echo GenBIOS.bat
echo %WROKSPACE%
find /v "#" BraswellPlatformPkg\%BIOS_ID_FILE% > string
for /f "tokens=1,2,3" %%i in (string) do set %%i=%%k
set SPIROM=Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\ROM
copy /b /y %SPIROM%\bios.rom %SPIROM%\%BOARD_ID%_%1_%BUILD_TYPE%_%VERSION_MAJOR%_%VERSION_MINOR%.ROM
copy /b /y %SPIROM%\%BOARD_ID%_%1_%BUILD_TYPE%_%VERSION_MAJOR%_%VERSION_MINOR%.ROM Stitch
if exist %SPIROM%\bios.rom del %SPIROM%\bios.rom
if exist %WORKSPACE%\string del string

