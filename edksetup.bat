@REM @file
@REM   Windows batch file to setup a WORKSPACE environment
@REM
@REM Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
@REM This program and the accompanying materials
@REM are licensed and made available under the terms and conditions of the BSD License
@REM which accompanies this distribution.  The full text of the license may be found at
@REM http://opensource.org/licenses/bsd-license.php
@REM
@REM THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
@REM WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
@REM

@REM usage:
@REM   edksetup.bat [--help | -h] [-v | --version] [Rebuild] [Reconfig]

@REM ##############################################################
@REM # You should not have to modify anything below this line
@REM #

@echo off
@if not defined ORIGINAL_PATH set "ORIGINAL_PATH=%PATH%"
@set INCLUDE_VS=FALSE
@REM
@REM Set the WORKSPACE to the current working directory
@REM
@pushd .
@cd %~dp0

@set TOOL_ERROR=0
@if /I "%1"=="-n" (
  @set WORKSPACE=
  @shift
)
@if /I "%1"=="--new" (
  @set WORKSPACE=
  @shift
)

if not defined WORKSPACE (
  @goto SetWorkSpace
)

if %WORKSPACE% == %CD% (
  @REM Workspace is not changed.
  @goto ParseArgs
)

:SetWorkSpace
@REM set new workspace
@REM clear EFI_SOURCE and EDK_SOURCE for the new workspace
@set WORKSPACE=%CD%
@set EFI_SOURCE=
@set EDK_SOURCE=
@set INCLUDE_VS=

:ParseArgs
@if /I "%1"=="-h" goto Usage
@if /I "%1"=="--help" goto Usage
@rem
@rem Handle Windows Only Users who are not aware of other methods for specifying flags
@if /I "%1"=="/h" goto Usage
@if /I "%1"=="/?" goto Usage
@if /I "%1"=="/help" goto Usage
@if /I "%1"=="--version" (
  @set VER_ONLY=TRUE
  @goto Usage
)
@if /I "%1"=="-v" (
  @set VER_ONLY=TRUE
  @goto Usage
)

@if not defined ORIGINAL_PYTHONPATH set ORIGINAL_PYTHONPATH=%PYTHONPATH%
@if not defined ORIGINAL_BASETOOLS set ORIGINAL_BASETOOLS=%WORKSPACE%\BaseTools
@if not defined ORIGINAL_SDB_TOOLS set ORIGINAL_SDB_TOOLS=%WORKSPACE%\SdbTools
@if not defined TOOL_PATH set TOOL_PATH=%WORKSPACE%\MakeBinPkg\Win32;%WORKSPACE%\BuildServerScripts;%WORKSPACE%\BuildServerScripts\Win32
@if exist %WORKSPACE%\MakeBinPkg\Source @set PYTHONPATH=%WORKSPACE%\MakeBinPkg\Source
@if exist %WORKSPACE%\BuildServerScripts\Source @set PYTHONPATH=%WORKSPACE%\BuildServerScripts\Source;%PYTHONPATH%
@if not defined EDK_TOOLS_PATH set EDK_TOOLS_PATH=%WORKSPACE%\BaseTools
if not defined SDB_TOOLS_PATH set SDB_TOOLS_PATH=%ORIGINAL_SDB_TOOLS%

@set "PATH=%TOOL_PATH%;%SDB_TOOLS_PATH%\Bin\Win32;%SDB_TOOLS_PATH%\Bin\Win32\Viewer;%EDK_TOOLS_PATH%\Bin\Win32;%ORIGINAL_PATH%"

@if /I "%1"=="--vs" (
  @echo Including Microsoft Visual Studio Path
  @shift
	@if not defined VCINSTALLDIR (
	  if defined VS100COMNTOOLS (
		  call "%VS100COMNTOOLS%\vsvars32.bat"
	  ) else (
		  if defined VS90COMNTOOLS (
		    call "%VS90COMNTOOLS%\vsvars32.bat"
		  ) else (
		    if defined VS80COMNTOOLS (
			    call "%VS80COMNTOOLS%\vsvars32.bat"
		    ) else (
		      if defined VS71COMNTOOLS (
			      call "%VS71COMNTOOLS%\vsvars32.bat"
		      ) else (
			      echo.
			      echo !!! WARNING !!! Cannot find Visual Studio !!!
			      echo.
			    )
		    )
		  )
	  )
	)
)


@set PATH_MODIFIED=TRUE

@if not exist %WORKSPACE%\Conf @mkdir %WORKSPACE%\Conf
@if /I "%1"=="Reconfig" (
  @copy /Y /A %EDK_TOOLS_PATH%\Conf\build_rule.template %WORKSPACE%\Conf\build_rule.txt /A
  @copy /Y /A %EDK_TOOLS_PATH%\Conf\tools_def.template %WORKSPACE%\Conf\tools_def.txt /A
  @copy /Y /A %EDK_TOOLS_PATH%\Conf\target.template %WORKSPACE%\Conf\target.txt /A
) else (
  @if not exist %WORKSPACE%\Conf\build_rule.txt @copy /Y /A %EDK_TOOLS_PATH%\Conf\build_rule.template %WORKSPACE%\Conf\build_rule.txt /A
  @if not exist %WORKSPACE%\Conf\tools_def.txt @copy /Y /A %EDK_TOOLS_PATH%\Conf\tools_def.template %WORKSPACE%\Conf\tools_def.txt /A
  @if not exist %WORKSPACE%\Conf\target.txt @copy /Y /A %EDK_TOOLS_PATH%\Conf\target.template %WORKSPACE%\Conf\target.txt /A
)


@IF NOT EXIST "%EDK_TOOLS_PATH%\Bin\Win32\build.exe" goto BadBaseTools

@goto end

:BadBaseTools
@echo.
  @REM
  @REM Need BaseTools binaries in order to build
  @REM
  @echo !!! ERROR !!! The BaseTools binaries were not found !!!
  @echo.
  @echo.
  @goto enosuccess

:Usage
@echo %0, Version 1.1
  @echo Copyright(c) 2012 - 2015, Intel Corporation  All rights reserved.
  @if defined VER_ONLY goto enosuccess
  @echo.
  @echo   Usage: "%0 [-n | --new] [-h | --help ] [-v | --version] [Rebuild] [Reconfig]"
  @echo.
  @echo Where,
  @echo    -v, --version  Display version information and exit
  @echo    -n, --new      Force setting the WORKSPACE to the currect directory
  @echo    --vs           Include Microsoft Visual Studio in the PATH
  @echo    Rebuild        Force rebuilding of the SdbTools and BaseTools.
  @echo    Reconfig       Force overwriting the Conf files, target.txt,
  @echo                     tools_def.txt and build_rule.txt
  @echo    -h, --help     Display this information and exit
  @goto enosuccess

:end
@cd %WORKSPACE%
  @echo.
  @echo #############################################################################
  @echo    WORKSPACE           = %WORKSPACE%
  @echo    EDK_TOOLS_PATH      = %EDK_TOOLS_PATH%
  @if defined BASE_TOOLS_PATH (
    @echo    BASE_TOOLS_PATH     = %BASE_TOOLS_PATH%
    @echo    PYTHONHOME          = %PYTHONHOME%
  )
  @echo    PATH                = "%PATH%"
  @echo.
  @echo #############################################################################
  @echo.
  @echo on

:enosuccess
@cd %WORKSPACE%
  @set VER_ONLY=
  @echo on
  @exit /B %TOOL_ERROR%
