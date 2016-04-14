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

@echo off
setlocal EnableDelayedExpansion EnableExtensions
echo.
echo %date%  %time%
echo.

REM ---- Do NOT use :: for comments Inside of code blocks() ----

::**********************************************************************
:: Initial Setup
::**********************************************************************
set WORKSPACE=%CD%
if %WORKSPACE:~-1%==\ set WORKSPACE=%WORKSPACE:~0,-1%
set /a build_threads=1 
set "Build_Flags= "
set "SV_String=_"
set exitCode=0
set Arch=X64
set Compiler=VS2008
set "SpiLock= "

set VS2008_SP_REGKEY_PATH=HKLM\Software\Microsoft\DevDiv\VS\Servicing\9.0\
set VS2008_SP_REGKEY=SP

:: Clean up previous build files.
if exist %WORKSPACE%\edk2.log del %WORKSPACE%\edk2.log
if exist %WORKSPACE%\unitool.log del %WORKSPACE%\unitool.log
if exist %WORKSPACE%\Conf\target.txt del %WORKSPACE%\Conf\target.txt
if exist %WORKSPACE%\Conf\tools_def.txt del %WORKSPACE%\Conf\tools_def.txt
if exist %WORKSPACE%\Conf\build_rule.txt del %WORKSPACE%\Conf\build_rule.txt
if exist %WORKSPACE%\Conf\FrameworkDatabase.db del %WORKSPACE%\Conf\FrameworkDatabase.db
if exist conf\.cache rmdir /q/s conf\.cache

:: Setup EDK environment. Edksetup puts new copies of target.txt, tools_def.txt, build_rule.txt in WorkSpace\Conf
:: Also run edksetup as soon as possible to avoid it from changing environment variables we're overriding
set "VCINSTALLDIR="
set "EDK_TOOLS_PATH="
cd..
if /I "%1" == "DEBUG" (
  copy BaseTools\Conf\tools_def.template Conf\tools_def.txt /Y
) else (
  copy BaseTools\Conf\tools_def.template Conf\tools_def.txt /Y
)

call edksetup.bat > nul
cd %WORKSPACE%

@rem InternalOnlyBegin
@rem Workaround for not using InternalOnly tools from ClientCommonPkg
REM  XCOPY /I /Y /R %WORKSPACE%\%CLIENT_COMMON_PACKAGE%\InternalOnly\Tools\SignFv\SignFv.exe %WORKSPACE%\%CLIENT_COMMON_PACKAGE%\Tools\SignFv\
REM  XCOPY /I /Y /R %WORKSPACE%\%BP_COMMON_PACKAGE%\Restricted\Tools\Link16\link16.exe %WORKSPACE%\%CLIENT_COMMON_PACKAGE%\Tools\
@rem InternalOnlyEnd

@echo off

:: Description of each Platform/Board_ID
::   BRAS - Netbook/Desktop (PCCG)
::   CHTT - Tablet build (MCG)
::   CHTE - ERB build (tobe removed)
set eNB_RVP=BRAS
set Tblt_RVP=CHTT
set Tblt_ERB=CHTE

:: Initialize all the build flags to FALSE
:: Then based on the cmd line input, some will be set to TRUE prior to building.
:: Note: These variable names match the actual Build Macro names in DSC for clarity.
set SVP_PF_BUILD=FALSE
set ENBDT_PF_BUILD=FALSE
set TABLET_PF_BUILD=FALSE
set ERB_PF_BUILD=FALSE
set SV_BIOS_ENABLE=FALSE
set HYBRID_ENABLE=FALSE
set ECP_BUILD_ENABLE=FALSE
set PPV_ENABLE=FALSE
set MRC_EV_RMT_BUILD=FALSE
set FSP_WRAPPER_ENABLE=TRUE
set FSP_BUILD=FALSE
SET BUILDFSPB_PARAMETER=/d

:: Define build environment defaults
set EFI_SOURCE=%WORKSPACE%
set EDK_SOURCE=%WORKSPACE%

set PLATFORM_NAME=BraswellPlatformPkg
set PLATFORM_PACKAGE=BraswellPlatformPkg
set PLATFORM2_PACKAGE=BraswellPlatformPkg
set PLATFORM_RC_PACKAGE=ChvRefCodePkg
set "UniTool_Excludes=ChtSvPkg R8ChtDeviceSvRestrictedPkg"

::**********************************************************************
:: Parse command line arguments
::**********************************************************************

:: Optional arguments
:OptLoop
if /i "%~1"=="/?" goto Usage

if /i "%~1"=="fspb" (
    set FSP_BUILD=TRUE
    shift
    goto OptLoop
)

if /i "%~1"=="fspw" (
    set FSP_WRAPPER_ENABLE=TRUE
    shift
    goto OptLoop
)
if /i "%~1"=="/q" (
    set Build_Flags=%Build_Flags% --quiet
    shift
    goto OptLoop
)
if /i "%~1"=="/l" (
    set Build_Flags=%Build_Flags% -j EDK2.log
    shift
    goto OptLoop
)
if /i "%~1" == "/c" (
    echo Removing previous Build files...
    if exist build (
      del /f/s/q build > nul
      rmdir /s/q build
    )
   if exist conf\.cache (
     del /f/s/q conf\.cache > nul
     rmdir /s/q conf\.cache
   )
   echo.
    shift
    goto OptLoop
)
if /i "%~1"=="/ia32" (
    set Arch=IA32
    shift
    goto OptLoop
)

if /i "%~1"=="/ppv" (
    set PPV_ENABLE=TRUE
    set SV_String=_PPV_
    shift
    goto OptLoop
)
if /i "%~1"=="/rmt" (
    set MRC_EV_RMT_BUILD=TRUE
    shift
    goto OptLoop
)
if /i "%~1"=="/SV" (
    set SV_BIOS_ENABLE=TRUE
    set SVP_PF_BUILD=TRUE
    set SV_String=_SV_
    set UniTool_Excludes=BraswellPlatformPkg
    set PLATFORM_SV_PACKAGE=R8ChtDeviceSvRestrictedPkg
    shift
    goto OptLoop
)
if /i "%~1"=="/R" (
    set target=RELEASE
    set BUILDFSPB_PARAMETER=/r
    shift
    goto OptLoop
)
if /i "%~1"=="/D" (
    set target=DEBUG
    shift
    goto OptLoop
)
if /i "%~1"=="/vs10" (
    set Compiler=VS2010
    shift
    goto OptLoop
)
if /i "%~1"=="/vs08" (
    set Compiler=VS2008
    shift
    goto OptLoop
)
if /i "%~1"=="/vs13" (
    set Compiler=VS2013
    shift
    goto OptLoop
)
if /i "%~1"=="/yL" (
    set SpiLock=/yL
    shift
    goto OptLoop
)


:: Required argument(s)
if "%~1"=="" (
   echo. & echo Not Enough Arguments Provided, default is Braswell build.
   echo Please review the Help screen "/?" & echo.
   rem goto Exit
)

:: BOARD_ID needs to be exactly 7 characters (GenBiosId.exe limitation)
echo Setting  %1  platform configuration and BIOS ID...
if /i "%~1" == "%eNB_RVP%" (
   set BOARD_ID=BRASWEL
   set ENBDT_PF_BUILD=TRUE
   set PLATFORM_NAME=BraswellPlatformPkg

) else if /i "%~1" == "%Tblt_RVP%" (
   set BOARD_ID=CHTTRVP
   set TABLET_PF_BUILD=TRUE
   set PLATFORM_NAME=BraswellPlatformPkg

) else if /i "%~1" == "" (
   echo Error - No PlatformType specified, Taking Braswell as default.
    set BOARD_ID=BRASWEL
    set ENBDT_PF_BUILD=TRUE
    set PLATFORM_NAME=BraswellPlatformPkg
    set Platform_Type=BRAS
    goto SkipSetPlatformType

 ) else (
   echo Error - Unsupported PlatformType: %1 & echo.
   goto Usage

)
set Platform_Type=%~1
:SkipSetPlatformType
if "%SV_BIOS_ENABLE%"=="TRUE"  set PLATFORM_NAME=ChtSvPkg

if "%FSP_WRAPPER_ENABLE%"=="TRUE" (
    SET FSP_PKG_NAME=BraswellFspPkg
    SET OUT_DIR=Build
    @if not defined VS_VERSION (
      call %~dp0\..\EdkSetupFsp.bat
    )
)
::**********************************************************************
:: Additional EDK Build Setup/Configuration
::**********************************************************************

::Setup DefineAtBuildMacros config file
set Build_Macros=.\%PLATFORM_NAME%\AutoPlatformCFG.txt
copy /y nul %Build_Macros% >nul

::output platform specific build macros to DefineAtBuildMacros.dsc
echo DEFINE SVP_PF_BUILD    = %SVP_PF_BUILD%      >> %Build_Macros%
echo DEFINE ENBDT_PF_BUILD  = %ENBDT_PF_BUILD%    >> %Build_Macros%
echo DEFINE TABLET_PF_BUILD = %TABLET_PF_BUILD%   >> %Build_Macros%
echo DEFINE SV_BIOS_ENABLE  = %SV_BIOS_ENABLE%    >> %Build_Macros%
echo DEFINE HYBRID_ENABLE   = %HYBRID_ENABLE%     >> %Build_Macros%
echo DEFINE ECP_BUILD_ENABLE = %ECP_BUILD_ENABLE% >> %Build_Macros%
echo DEFINE PPV_ENABLE       = %PPV_ENABLE%       >> %Build_Macros%
echo DEFINE MRC_EV_RMT_BUILD = %MRC_EV_RMT_BUILD% >> %Build_Macros%
echo DEFINE FSP_WRAPPER_ENABLE = %FSP_WRAPPER_ENABLE% >> %Build_Macros%

if "%Arch%"=="IA32" (
    echo DEFINE X64_CONFIG = FALSE  >> %Build_Macros%
) else if "%Arch%"=="X64" (
    echo DEFINE X64_CONFIG = TRUE  >> %Build_Macros%
)

::Stage of copy of BiosId.env in Conf/ with Platform_Type and Build_Target values removed
findstr /b /v "BOARD_ID  BUILD_TYPE OEM_ID" %PLATFORM_NAME%\BiosId.env > Conf\BiosId.env


if "%target%" == "RELEASE" (
    echo BUILD_TYPE = R >> Conf\BiosId.env
) else if "%target%" == "DEBUG" (
    echo BUILD_TYPE = D >> Conf\BiosId.env
) else (
    set target=DEBUG
    echo BUILD_TYPE = D >> Conf\BiosId.env
)

if "%Arch%"=="IA32" (
    echo OEM_ID = 86C >> Conf\BiosId.env
) else if "%Arch%"=="X64" (
    echo OEM_ID = X64 >> Conf\BiosId.env
)

:: Set the Build_Type, Version_Major, and Version_Minor environment variables
find /v "#" Conf\BiosId.env > ver_strings
for /f "tokens=1,3" %%i in (ver_strings) do set %%i=%%j
del /f/q ver_strings >nul
:: Do this after parsing "ver_strings"
echo BOARD_ID = %BOARD_ID% >> Conf\BiosId.env

echo.
echo Setting the Build environment for Visual Studio...
if "%Compiler%"=="VS2008" (
  if defined VS90COMNTOOLS (
    call "%VS90COMNTOOLS%\vsvars32.bat"
    if /I "%VS90COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2008
         Set TOOL_CHAIN_VS2008=TRUE
    ) else  set TOOL_CHAIN_TAG=VS2008x86
       Set TOOL_CHAIN_VS2008 = TRUE
  ) else (
    echo  --ERROR: VS2008 not installed correctly. VS90COMNTOOLS not defined ^^!
    goto BldFail
  )
) else if "%Compiler%"=="VS2010" (
   if defined VS100COMNTOOLS (
      call "%VS100COMNTOOLS%\vsvars32.bat"
      if /I "%VS100COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 10.0\Common7\Tools\" (
         set TOOL_CHAIN_TAG=VS2010
      ) else  set TOOL_CHAIN_TAG=VS2010x86
   ) else (
      echo  --ERROR: VS2010 not installed correctly. VS100COMNTOOLS not defined ^^!
      goto BldFail
   )
) else if "%Compiler%"=="VS2012" (
   if defined VS110COMNTOOLS (
      call "%VS110COMNTOOLS%\vsvars32.bat"
      if /I "%VS110COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 11.0\Common7\Tools\" (
         set TOOL_CHAIN_TAG=VS2012
      ) else  set TOOL_CHAIN_TAG=VS2012x86
   ) else (
      echo  --ERROR: VS2012 not installed correctly. VS110COMNTOOLS not defined ^^!
      goto BldFail
   )
)else if "%Compiler%"=="VS2013" (
      if defined VS120COMNTOOLS (
	   call "%VS120COMNTOOLS%\vsvars32.bat"
		if /I "%VS120COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\" (
			set TOOL_CHAIN_TAG=VS2013
echo %TOOL_CHAIN_TAG%
			) else  set TOOL_CHAIN_TAG=VS2013x86
		) else (
      echo  --ERROR: VS2013 not installed correctly. VS120COMNTOOLS not defined ^^!
      goto BldFail
   )
)

    set VS_2008_SP1=FALSE
if "%TOOL_CHAIN_TAG%"=="VS2008x86" (
    echo DEFINE TOOL_CHAIN_VS2008 = TRUE  >> %Build_Macros%
    echo DEFINE TOOL_CHAIN_VS2010 = FALSE  >> %Build_Macros%
    echo DEFINE TOOL_CHAIN_VS2013 = FALSE  >> %Build_Macros%

    reg query %VS2008_SP_REGKEY_PATH% /v %VS2008_SP_REGKEY% 2>null

        for /f "tokens=2,*" %%a in ('reg query %VS2008_SP_REGKEY_PATH% /v %VS2008_SP_REGKEY% ^| findstr %VS2008_SP_REGKEY%') do (
        if 0x1==%%b (
        set VS_2008_SP1=TRUE
    echo DEFINE VS_2008_SP1 = TRUE >> %Build_Macros%
        )
  )



) else if "%TOOL_CHAIN_TAG%"=="VS2010x86" (
    echo DEFINE TOOL_CHAIN_VS2008 = FALSE  >> %Build_Macros%
    echo DEFINE TOOL_CHAIN_VS2010 = TRUE  >> %Build_Macros%
    echo DEFINE TOOL_CHAIN_VS2013 = FALSE  >> %Build_Macros%
) else if "%TOOL_CHAIN_TAG%"=="VS2013x86" (
    echo DEFINE TOOL_CHAIN_VS2008 = FALSE  >> %Build_Macros%
    echo DEFINE TOOL_CHAIN_VS2010 = FALSE  >> %Build_Macros%
    echo DEFINE TOOL_CHAIN_VS2013 = TRUE  >> %Build_Macros%
)
echo Modifing Conf files for this build...
:: Remove lines with these tags from target.txt
findstr /V "TARGET  TARGET_ARCH  TOOL_CHAIN_TAG  ACTIVE_PLATFORM  MAX_CONCURRENT_THREAD_NUMBER" Conf\target.txt > Conf\target.txt.tmp

if "%Arch%"=="IA32" (        echo TARGET_ARCH = IA32        >> Conf\target.txt.tmp
) else if "%Arch%"=="X64" (  echo TARGET_ARCH = IA32 X64    >> Conf\target.txt.tmp
)
echo TARGET          = %TARGET%                                >> Conf\target.txt.tmp
echo TOOL_CHAIN_TAG  = %TOOL_CHAIN_TAG%                        >> Conf\target.txt.tmp
echo ACTIVE_PLATFORM = %PLATFORM_NAME%/PlatformPkg%Arch%.dsc  >> Conf\target.txt.tmp
echo MAX_CONCURRENT_THREAD_NUMBER = %build_threads%        >> Conf\target.txt.tmp

move /Y Conf\target.txt.tmp Conf\target.txt >nul
echo *_VS2008x86_*_ASL_PATH = C:\ASL\iasl.exe  >> Conf\tools_def.txt
echo *_VS2010x86_*_ASL_PATH = C:\ASL\iasl.exe  >> Conf\tools_def.txt
echo *_VS2012x86_*_ASL_PATH = C:\ASL\iasl.exe  >> Conf\tools_def.txt
echo *_VS2013x86_*_ASL_PATH = C:\ASL\iasl.exe  >> Conf\tools_def.txt

::
::  Place holder for SV Hook setup
::
:: if "%SV_BIOS_ENABLE%" == "TRUE"  call svhooks.bat
::
:: goto SkipSVPrep
::
::


::**********************************************************************
:: Build BIOS
::**********************************************************************
echo Ensuring correct build directory is present for GenBiosId...
set BUILD_PATH=Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%
if not exist %PLATFORM_PACKAGE%\IA32  mkdir %BUILD_PATH%\IA32

echo Creating BiosId...
:: Always generate 32bit for PEI modules.
@set BIOS_ID_OUTPUT_DIR=%BUILD_PATH%\IA32

@if not exist %BIOS_ID_OUTPUT_DIR% mkdir %BIOS_ID_OUTPUT_DIR%
@REM
@REM Reenable the GenBiosId if it is for external release
@REM
%PLATFORM2_PACKAGE%\Tools\GenBiosId\GenBiosId.exe -i Conf\BiosId.env -o %BIOS_ID_OUTPUT_DIR%\BiosId.bin -ob Conf\BiosId.bat
if %ERRORLEVEL% NEQ 0 goto BldFail
call Conf\BiosId.bat
echo BIOS_ID=%BIOS_ID%

@REM
@REM PLATFORM_SV_PACKAGE:RestrictedBegin
@REM
echo %PLATFORM_SV_PACKAGE%
@if not defined PLATFORM_SV_PACKAGE goto SkipSvBuild
set BIOS_ID_LOCATION=Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\IA32\%PLATFORM_SV_PACKAGE%\CpuSv\PEI\PeiHostInterface\PeiHostInterface\OUTPUT
@if not exist %BIOS_ID_LOCATION% (
  mkdir %BIOS_ID_LOCATION%
)
@if not exist Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\IA32\%PLATFORM_SV_PACKAGE%\Common\BiosId\BiosId\OUTPUT (
  mkdir Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\IA32\%PLATFORM_SV_PACKAGE%\Common\BiosId\BiosId\OUTPUT
)
@REM NOTE: For X64
@REM @copy /Y Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\X64\BiosId.bin %BIOS_ID_LOCATION%\BiosId.bin
@REM
@copy /Y Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\IA32\BiosId.bin %BIOS_ID_LOCATION%\BiosId.bin

  @echo.
  @echo --------------------------------------------------------------------------------
  @echo Create dummy SV Knob data files to pass setup module prebuild
  @echo --------------------------------------------------------------------------------
  @echo.
  @echo > %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.c
  @echo > %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.h
  @echo > %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.xml
  @echo > %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\KnobsInfoArray.c
  @echo > %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\SETUP_DATA.h
  @echo DONE!!
  @echo --------------------------------------------------------------------------------
  @echo.
@echo.
@echo ================================================================================
@echo  Prebuild:  Generate SetupStrDefs.h, as required by SV Knob data.
@echo.
echo %WROKSPACE%
build -n %NUMBER_OF_PROCESSORS% -m .\%PLATFORM_NAME%\PlatformSetupDxe\PlatformSetupDxe.inf -e %TEST_MENU_BUILD_OPTION%

@if %ERRORLEVEL% NEQ 0 (
  set SCRIPT_ERROR=1
  goto :BldFail
)


  @echo.
  @echo --------------------------------------------------------------------------------
  @echo  Prebuild:  Remove old SV Knob data.
  @echo --------------------------------------------------------------------------------
  @echo.
  @del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.c
  @del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.h
  @del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.xml
  @del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\KnobsInfoArray.c
  @del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\SETUP_DATA.h
  @echo DONE!!

  @echo.
  @echo --------------------------------------------------------------------------------
  @echo  Prebuild:  Create SV Knob data.
  @echo --------------------------------------------------------------------------------
  @echo.
  @echo   Step 1 - Call GenHeader.bat.
  @echo --------------------------------------------------------------------------------
  @echo.
REM @echo off
@call %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\GenHeader.bat %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2 %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobsFilter.txt
@echo on
  @echo.
  @echo   DONE!!
  @echo.
  @echo   Step 2 - Call GenKnobs.bat.
  @echo --------------------------------------------------------------------------------
  @echo.
REM @echo off
@call %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\GenKnobs.bat %WORKSPACE%\%PLATFORM_PACKAGE% %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2 %WORKSPACE%\%PLATFORM_PACKAGE%\PlatformSetupDxe\ %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobsFilter.txt
@echo.
@echo   DONE!!
@echo.
@echo ================================================================================
@echo  END OF PRE-BUILD:  Generate SetupStrDefs.h, as required by SV Knob data.
@echo ================================================================================
@echo on
@echo.
:SkipSvBuild
@REM
@REM PLATFORM_SV_PACKAGE:RestrictedEnd
@REM


echo Running UniTool...
REM :: Scan the main tree, excluding upto 2 packages from the scan
REM if "%SV_BIOS_ENABLE%"=="TRUE" (
REM  BpCommonPkg\Tools\UniTool\UniTool.exe -b -u -x . ChtSvPkg\PlatformSetupDxe\UqiList.uni %UniTool_Excludes% 1>>Unitool.log 2>&1
REM ) else (
REM  BpCommonPkg\Tools\UniTool\UniTool.exe -b -u -x . BraswellPlatformPkg\PlatformSetupDxe\UqiList.uni %UniTool_Excludes% 1>>Unitool.log 2>&1
REM )
REM if %ERRORLEVEL% NEQ 0 (
REM   echo  -- Error Parsing uni.  Please see %WORKSPACE%\unitool.log for more info.
REM   goto BldFail
REM )

if "%FSP_BUILD%"=="TRUE" (
    echo Invoking EDK2 BSW FSP build...
    call BuildFsp.cmd BSW %BUILDFSPB_PARAMETER%
	if exist %WORKSPACE%\BraswellFspWrapperPkg\FspBin\BSWFSP.fd (
      attrib -r %WORKSPACE%\BraswellFspWrapperPkg\FspBin\BSWFSP.fd
    )
	@if %ERRORLEVEL% NEQ 0 (
      @echo !!! ERROR:Build Fail !!!
    )
    if not exist %WORKSPACE%\BraswellFspPkg\FspBin mkdir BraswellFspPkg\FspBin
    copy /y /b %OUT_DIR%\%FSP_PKG_NAME%\%TARGET%_%TOOL_CHAIN_TAG%\FV\BSWFSP.fd %WORKSPACE%\BraswellFspWrapperPkg\FspBin\
    if exist %WORKSPACE%\BraswellFspWrapperPkg\Include\BraswellFspVpd.h (
      attrib -r %WORKSPACE%\BraswellFspWrapperPkg\Include\BraswellFspVpd.h
    )
    copy /y /b %OUT_DIR%\%FSP_PKG_NAME%\%TARGET%_%TOOL_CHAIN_TAG%\FV\fsp_vpd.h %WORKSPACE%\BraswellFspWrapperPkg\Include\BraswellFspVpd.h
    echo.
    @echo off
)
echo Invoking EDK2 BSW Normal build...
build %Build_Flags%
if %ERRORLEVEL% NEQ 0 goto BldFail

::**********************************************************************
:: Post Build processing and cleanup
::**********************************************************************

echo Running fce...
pushd %PLATFORM2_PACKAGE%\Tools\FCE
:: Extract Hii data from build and store in HiiDefaultData.txt
fce.exe read -i %WORKSPACE%\%BUILD_PATH%\FV\Cht.fd > %WORKSPACE%\%BUILD_PATH%\FV\HiiDefaultData.txt
:: copy the Setup variable to the SetupDefault variable and save changes to ChtXXX.fd
fce.exe update -i %WORKSPACE%\%BUILD_PATH%\FV\Cht.fd -s %WORKSPACE%\%BUILD_PATH%\FV\HiiDefaultData.txt -o %WORKSPACE%\%BUILD_PATH%\FV\Cht%Arch%.fd 
popd
if %ERRORLEVEL% NEQ 0 goto BldFail
echo  FD successfully updated with default Hii values.

::echo Running KeyEnroll...
::   call %PLATFORM2_PACKAGE%\Tools\KeyEnroll\BSW-KeyEnroll.bat %BUILD_PATH% %BUILD_PATH%\FV\Cht%Arch%.fd
::if %ERRORLEVEL% NEQ 0 goto BldFail

:: Set Bios filename and Copy BIOS.ROM to common location(s)
set BIOS_Name=%BOARD_ID%%SV_String%%Arch%_%BUILD_TYPE%_%VERSION_MAJOR%_%VERSION_MINOR%.ROM
if "%PSS-Rename%"=="TRUE"  set BIOS_Name=%BIOS_Name:~0,-3%PSS
if not exist %BUILD_PATH%\ROM  mkdir %BUILD_PATH%\ROM
copy /y/b %BUILD_PATH%\FV\Cht%Arch%.fd  %WORKSPACE%\BraswellPlatformPkg\Stitch\%BIOS_Name% >nul
copy /y/b %BUILD_PATH%\FV\Cht%Arch%.fd  %BUILD_PATH%\ROM\%BIOS_Name% >nul

echo
echo Make backup on ROM file (unsigned release)
echo ================================
copy /y/b %WORKSPACE%\BraswellPlatformPkg\Stitch\%BIOS_Name% %BUILD_PATH%\ROM\%BIOS_Name%.rel

echo.
echo Calling IFWIStitch...
pushd %PLATFORM2_PACKAGE%\Stitch
if  /i "%Platform_Type%" == "%Tblt_RVP%" (
   if "%SV_BIOS_ENABLE%" == "TRUE" (
     call IFWIStitch.bat %BIOS_Name% /C CHTT_SV_Stitch_Config.txt /S %VERSION_MAJOR%_%VERSION_MINOR%
   ) else (
     call IFWIStitch.bat %BIOS_Name% /C CHTT_Stitch_Config.txt /S %VERSION_MAJOR%_%VERSION_MINOR%
   )
) else if /i "%Platform_Type%" == "%eNB_RVP%" (
   if "%SV_BIOS_ENABLE%" == "TRUE" (
     call IFWIStitch.bat /nG /nM /nI %SpiLock% %BIOS_Name% /C BSW_SV_Stitch_Config.txt /S %VERSION_MAJOR%_%VERSION_MINOR%
   ) else if "%PPV_ENABLE%" == "TRUE" (
     call IFWIStitch.bat /nG /nM /nI %SpiLock% %BIOS_Name% /C BSW_PPV_Stitch_Config.txt /S %VERSION_MAJOR%_%VERSION_MINOR%
   ) else (
     call IFWIStitch.bat /nG /nM /nI %SpiLock% %BIOS_Name% /C BSW_Stitch_Config.txt /S %VERSION_MAJOR%_%VERSION_MINOR%
   )
)
@echo off
popd

echo BIOS_ID=%BIOS_ID%
copy /y/b %PLATFORM2_PACKAGE%\Stitch\%BIOS_ID%.bin %BUILD_PATH%\ROM\
copy /y/b %PLATFORM2_PACKAGE%\Stitch\%BIOS_Name% %BUILD_PATH%\ROM\
::if "%Arch%"=="X64" (
::  @call BraswellPlatformPkg\BuildCapsulePkg.bat %TARGET% %Arch%
::
::  @REM Create Capsule Image again with the updated FD image
::  @if exist %WORKSPACE%\Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\FV\Capsule_Reset.cap (
::    copy /y /b %WORKSPACE%\Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\FV\Capsule_Reset.cap %WORKSPACE%\%PLATFORM2_PACKAGE%\CapsuleUpdateConfig\Capsule_Reset.cap
::  )

::  @REM Copy the CapsuleApp.efi capsule Efi tool
::  @if exist %WORKSPACE%\Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\X64\CapsuleApp.efi (
::    copy /y /b %WORKSPACE%\Build\%PLATFORM_PACKAGE%\%TARGET%_%TOOL_CHAIN_TAG%\X64\CapsuleApp.efi %WORKSPACE%\%PLATFORM2_PACKAGE%\CapsuleUpdateConfig\CapsuleApp.efi
::  )
::)
::echo.
::echo Build location:     %BUILD_PATH%
::echo BIOS ROM Created:   %BIOS_Name%
echo.
echo -------------------- The build has successfully completed. --------------------
echo.
goto Exit

:Usage
echo.
echo ***************************************************************************
echo Build BIOS Rom for CHT platforms.
echo.
echo Usage: BuildBsw.bat [options] ^<PlatformType^> ^<BuildTarget^>
echo.
echo.   /?     Display this help text
echo    /q     Quiet mode. Only display Fatal Errors (slightly faster)
echo    /l     Log a copy of the console output to EDK2.log
echo    /c     CleanAll before building
echo    /sv    Build Platform Type using SVBIOS config
echo    /ppv   Build Platform Type using PPV config
echo    /r     Build release BIOS
echo    /d     Build debug BIOS

echo    /ia32  Set Arch to IA32  (default: X64)
echo    /vs13  Set compiler to VisualStudio 2013  (default: 2012)
echo    /vs10  Set compiler to VisualStudio 2010
echo    /vs08  Set compiler to VisualStudio 2008
echo    /yL    Enable SPI lock
echo.
echo    Platform Types:  %eNB_RVP% %Tblt_RVP% %Embd_RVP%
echo    Build Targets:   Debug, Release
echo.
echo Examples:
echo    BuildBsw.bat /d %eNB_RVP%          : X64 Debug build for M/D
echo    BuildBsw.bat /c /q /d BRAS         : clean X64 Debug quiet build for Braswell
echo    BuildBsw.bat /r /ia32 %eNB_RVP%    : IA32 Release build for M/D
echo    BuildBsw.bat /r /sv %Tblt_RVP% : X64 SV Release build for Tblt
echo.
echo ***************************************************************************
set exitCode=1
goto Exit

:BldFail
set exitCode=1
echo  -- Error:  Build has failed^^!
echo See EDK2.log for more details

:Exit
echo %date%  %time%
exit /b %exitCode%

EndLocal
