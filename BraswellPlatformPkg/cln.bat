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
echo.
echo Run build cleanall...
echo.
cd..
if not defined WORKSPACE set WORKSPACE=%cd%
if not defined PLATFORM_PACKAGE set PLATFORM_PACKAGE=ChvTbltDevicePkg
if not defined PLATFORM_SV_PACKAGE set PLATFORM_SV_PACKAGE=R8ChtDeviceSvRestrictedPkg

@if "%CD:~-1%" == "\" (set WORKSPACE=%CD:~0,-1%)
@echo WORKSPACE=%WORKSPACE%

echo.
echo Directories to clean...
echo.

if exist build rmdir /q /s build
if exist conf\.cache rmdir /q /s conf\.cache
if exist RomImages rmdir /q /s RomImages
if exist BiosImage rmdir /q /s BiosImage
echo.
echo Files to clean...
echo.

if exist %WORKSPACE%\edk2.log del %WORKSPACE%\edk2.log
if exist %WORKSPACE%\Unitool.log del %WORKSPACE%\Unitool.log
if exist %WORKSPACE%\UnitoolSv.log del %WORKSPACE%\UnitoolSv.log
if exist %WORKSPACE%\Conf\target.txt del %WORKSPACE%\Conf\target.txt
if exist %WORKSPACE%\Conf\BiosId.env del %WORKSPACE%\Conf\BiosId.env
if exist %WORKSPACE%\Conf\tools_def.txt del %WORKSPACE%\Conf\tools_def.txt
if exist %WORKSPACE%\Conf\build_rule.txt del %WORKSPACE%\Conf\build_rule.txt
if exist %WORKSPACE%\Conf\FrameworkDatabase.db del %WORKSPACE%\Conf\FrameworkDatabase.db
if exist %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.c del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.c
if exist %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.h del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.h
if exist %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.xml del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\AutoKnobs.xml
if exist %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\KnobsInfoArray.c del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\KnobsInfoArray.c
if exist %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\SETUP_DATA.h del %WORKSPACE%\%PLATFORM_SV_PACKAGE%\CpuSv\DXE\GBT_XML_2\SETUP_DATA.h
if exist %WORKSPACE%\string del string
if exist Stitch\*.ROM del Stitch\*.ROM
if exist Stitch\PDR\PDR_Signed.bin del Stitch\PDR\PDR_Signed.bin
if exist Stitch\BIOS_Signing\MCGBIOS_SB_config.xml del Stitch\BIOS_Signing\MCGBIOS_SB_config.xml
if exist Stitch\Fota\dfu_log.txt del Stitch\Fota\dfu_log.txt
if exist Stitch\Fota\SysFwUpdateCapsuleDxe.efi del Stitch\Fota\SysFwUpdateCapsuleDxe.efi
if exist Stitch\Fota\CapsuleGen rmdir /s /q Stitch\Fota\CapsuleGen
if exist Stitch\Fota\FotaCapsuleBinary rmdir /s /q Stitch\Fota\FotaCapsuleBinary
if exist Stitch\Fota\tmp rmdir /s /q Stitch\Fota\tmp
if exist Stitch\Int rmdir /s /q Stitch\Int
if exist Stitch\*.orig del Stitch\*.orig
if exist Stitch\Stitching.log del Stitch\Stitching.log
if exist Stitch\SEC\ftool.log del Stitch\SEC\ftool.log
if exist Stitch_2MB\*.ROM del Stitch_2MB\*.ROM
if exist Stitch_2MB\*.orig del Stitch_2MB\*.orig
if exist Stitch_2MB\*.bin del Stitch_2MB\*.bin
if exist Stitch_2MB\BIOS_Signing\MCGBIOS_SB_config.xml del Stitch_2MB\BIOS_Signing\MCGBIOS_SB_config.xml
if exist Stitch_2MB\Fota\CapsuleGen\tmp rmdir /s /q Stitch_2MB\Fota\CapsuleGen\tmp
if exist Stitch_2MB\Fota\CapsuleGen\dfu_log.txt del Stitch_2MB\Fota\CapsuleGen\dfu_log.txt
if exist Stitch_2MB\Fota\CapsuleGen\FotaFwUpdateCapsuleDxe.efi del Stitch_2MB\Fota\CapsuleGen\FotaFwUpdateCapsuleDxe.efi
if exist Stitch_2MB\Fota\*.fv del Stitch_2MB\Fota\*.fv
if exist Stitch_2MB\Fota\*.bin del Stitch_2MB\Fota\*.bin
if exist Stitch_2MB\SEC\Int rmdir /s /q Stitch_2MB\SEC\Int
if exist Stitch_2MB\SEC\CH2T\FITC\fitc.log del Stitch_2MB\SEC\CH2T\FITC\fitc.log
if exist Stitch_2MB\Stitching.log del Stitch_2MB\Stitching.log
if exist %WORKSPACE%\*.ROM del %WORKSPACE%\*.ROM
if exist ChvTbltDevicePkg\AutoPlatformCFG.txt del ChvTbltDevicePkg\AutoPlatformCFG.txt
if exist BraswellPlatformPkg\AutoPlatformCFG.txt del BraswellPlatformPkg\AutoPlatformCFG.txt
if exist ChtSvPkg\AutoPlatformCFG.txt del ChtSvPkg\AutoPlatformCFG.txt

set PLATFORM_SV_PACKAGE=

echo.
echo All done...
echo.
@echo on
cd BraswellPlatformPkg