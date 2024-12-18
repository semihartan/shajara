@echo off

set __SHJ_SHARED_LIB=
set __SHJ_STATIC_LIB=
set __SHJ_PARSE_ERROR=
set __SHJ_TARGET_ARCH=x64
set __SHJ_HOST_ARCH=x64
set __SHJ_CLEAN=
set __SHJ_BUILD=1
set __SHJ_REBUILD=
set __SHJ_CONFIG=Release
set __SHJ_HELP=
set __SHJ_ALL=
set __SHJ_BUILD_CONFIG=
set __SHJ_BUILD_CONFIG_ARCH=
set __SHJ_BUILD_CONFIG_OUT=
set __SHJ_BUILD_CONFIG_TYPE=

set "__SHJ_ALL_BUILD_CONFIGS=x86 exe Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x86 exe Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x86 dll Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x86 dll Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x86 lib Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x86 lib Release"

set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x64 exe Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x64 exe Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x64 dll Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x64 dll Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x64 lib Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; x64 lib Release"

set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm exe Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm exe Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm dll Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm dll Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm lib Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm lib Release"

set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm64 exe Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm64 exe Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm64 dll Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm64 dll Release"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm64 lib Debug"
set "__SHJ_ALL_BUILD_CONFIGS=%__SHJ_ALL_BUILD_CONFIGS%; arm64 lib Release"

:check_installation
set __SHJ_VSInstallationDir=
set "__SHJ_VSWhere=C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

echo %__SHJ_VSWhere%

if exist "%__SHJ_VSWhere%" (
    for /f "usebackq tokens=*" %%a in (`"%__SHJ_VSWhere%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do ( 
		set "__SHJ_VSInstallationDir=%%a"
	)
)

echo %__SHJ_VSInstallationDir%

if "%__SHJ_VSInstallationDir%"=="" (
	if exist "C:\Program Files\Microsoft Visual Studio\2022\Community" (
		set "__SHJ_VSInstallationDir=C:\Program Files\Microsoft Visual Studio\2022\Community"
	) else (
		if exist "C:\Program Files\Microsoft Visual Studio\2019\Community" (
			set "__SHJ_VSInstallationDir=C:\Program Files\Microsoft Visual Studio\2019\Community"
		) else (
			goto :missing
		)
	)
)
echo %__SHJ_VSInstallationDir%

set "__SHJ_VSDEVCMD=%__SHJ_VSInstallationDir%\Common7\Tools\vsdevcmd.bat"

echo %__SHJ_VSDEVCMD%

if "%__SHJ_VSDEVCMD%"=="" (
	goto :missing
)

:parse_args
set __SHJ_PARSE_ERROR=0
set "__SHJ_ARGS_LIST=%*"
call :parse_loop
set "__SHJ_ARGS_LIST="

:check_error
if "%SHJ_DEBUG%" GEQ "1" (
	echo "__SHJ_PARSE_ERROR=%__SHJ_PARSE_ERROR%"
	echo "__SHJ_HELP=%__SHJ_HELP%"
)
if "%__SHJ_PARSE_ERROR%" NEQ "0" goto :usage_error
if "%__SHJ_HELP%" NEQ "" goto :usage

if "%__SHJ_ALL%"=="" (
	goto :assemble_vscmdargs
)

:build_all

echo Building all...
echo:

set __SHJ_BUILD=1
set __SHJ_REBUILD=0
set __SHJ_CLEAN=0

setlocal enabledelayedexpansion

:parse_configs_loop
for /f "tokens=1,* delims=;" %%A in ("!__SHJ_ALL_BUILD_CONFIGS!") do (
	echo In parse_configs_loop 
	set __SHJ_BUILD_CONFIG=%%A
	echo __SHJ_BUILD_CONFIG=!__SHJ_BUILD_CONFIG!
	
	call :parse_config_loop
	
	set __SHJ_TARGET_ARCH=!__SHJ_BUILD_CONFIG_ARCH!
	set __SHJ_CONFIG=!__SHJ_BUILD_CONFIG_TYPE!
	if /I "!__SHJ_BUILD_CONFIG_OUT!"=="lib" (
		set __SHJ_STATIC_LIB=1
		set __SHJ_SHARED_LIB=
		echo Building static library...
	) else (
		if /I "!__SHJ_BUILD_CONFIG_OUT!"=="dll" (
			set __SHJ_STATIC_LIB=
			set __SHJ_SHARED_LIB=1
			echo Building shared library...
		) else (
			set __SHJ_STATIC_LIB=
			set __SHJ_SHARED_LIB=
			echo Building executable...
		)
	)
	echo Target Architecture=!__SHJ_TARGET_ARCH!
	echo Config=!__SHJ_CONFIG!
	echo:
	call :assemble_vscmdargs
	
	set __SHJ_ALL_BUILD_CONFIGS=%%B
	goto :parse_configs_loop
)

goto :parse_configs_end

:parse_config_loop
for /f "tokens=1,*" %%C in ("!__SHJ_BUILD_CONFIG!") do (
	if /I "%%C"=="x86"     (set __SHJ_BUILD_CONFIG_ARCH=%%C)
	if /I "%%C"=="x64"     (set __SHJ_BUILD_CONFIG_ARCH=%%C)
	if /I "%%C"=="arm"     (set __SHJ_BUILD_CONFIG_ARCH=%%C)
	if /I "%%C"=="arm64"   (set __SHJ_BUILD_CONFIG_ARCH=%%C)

	if /I "%%C"=="exe"     (set __SHJ_BUILD_CONFIG_OUT=%%C)
	if /I "%%C"=="dll"     (set __SHJ_BUILD_CONFIG_OUT=%%C)
	if /I "%%C"=="lib"     (set __SHJ_BUILD_CONFIG_OUT=%%C)

	if /I "%%C"=="debug"   (set __SHJ_BUILD_CONFIG_TYPE=%%C)
	if /I "%%C"=="release" (set __SHJ_BUILD_CONFIG_TYPE=%%C)

	set __SHJ_BUILD_CONFIG=%%D
	goto :parse_config_loop
)

exit /B 0

:parse_configs_end
exit /B 0

endlocal

:convert_args_case
if /I "%__SHJ_TARGET_ARCH%"=="x86" (
	set __SHJ_TARGET_ARCH=x86
)
if /I "%__SHJ_TARGET_ARCH%"=="x86" (
	set __SHJ_TARGET_ARCH=win32
)
if /I "%__SHJ_TARGET_ARCH%"=="x64" (
	set __SHJ_TARGET_ARCH=amd64
)
if /I "%__SHJ_TARGET_ARCH%"=="amd64" (
	set __SHJ_TARGET_ARCH=amd64
)
if /I "%__SHJ_TARGET_ARCH%"=="arm" (
	set __SHJ_TARGET_ARCH=arm
)
if /I "%__SHJ_TARGET_ARCH%"=="arm64" (
	set __SHJ_TARGET_ARCH=arm64
)
if /I "%__SHJ_HOST_ARCH%"=="x86" (
	set __SHJ_HOST_ARCH=x86
)
if /I "%__SHJ_HOST_ARCH%"=="win32" (
	set __SHJ_HOST_ARCH=x86
)
if /I "%__SHJ_HOST_ARCH%"=="x64" (
	set __SHJ_HOST_ARCH=amd64
)
if /I "%__SHJ_HOST_ARCH%"=="amd64" (
	set __SHJ_HOST_ARCH=amd64
)
if /I "%__SHJ_HOST_ARCH%"=="arm" (
	set __SHJ_HOST_ARCH=arm
)
if /I "%__SHJ_HOST_ARCH%"=="arm64" (
	set __SHJ_HOST_ARCH=arm64
)

:assemble_vscmdargs
set "__SHJ_VSDEVCMD_ARGS=-arch=%__SHJ_TARGET_ARCH% -host_arch=%__SHJ_HOST_ARCH% -no_logo"

if /I "%__SHJ_TARGET_ARCH%"=="amd64" (
	set __SHJ_TARGET_ARCH=x64
)

if defined VSCMD_ARG_TGT_ARCH (
	if /I "%VSCMD_ARG_TGT_ARCH%" NEQ "%__SHJ_TARGET_ARCH%" (
		call "%__SHJ_VSDEVCMD%" -no_logo /clean_env
		echo VsDevCmd is cleaned and now being initialized for target=%__SHJ_TARGET_ARCH%, host=%__SHJ_HOST_ARCH%
		call "%__SHJ_VSDEVCMD%" %__SHJ_VSDEVCMD_ARGS%
	)
) else (
	echo VsDevCmd is being initialized for target=%__SHJ_TARGET_ARCH%, host=%__SHJ_HOST_ARCH%
    call "%__SHJ_VSDEVCMD%" %__SHJ_VSDEVCMD_ARGS%
)

:call_nmake
if "%SHJ_DEBUG%" GEQ "1" (
	echo __SHJ_SHARED_LIB=%__SHJ_SHARED_LIB%
	echo __SHJ_STATIC_LIB=%__SHJ_STATIC_LIB%
	echo __SHJ_PARSE_ERROR=%__SHJ_PARSE_ERROR%
	echo __SHJ_TARGET_ARCH=%__SHJ_TARGET_ARCH%
	echo __SHJ_HOST_ARCH=%__SHJ_HOST_ARCH%
	echo __SHJ_CLEAN=%__SHJ_CLEAN%
	echo __SHJ_BUILD=%__SHJ_BUILD%
	echo __SHJ_REBUILD=%__SHJ_REBUILD%
	echo __SHJ_CONFIG=%__SHJ_CONFIG%
)
set SHJ_STATIC_LIB=%__SHJ_STATIC_LIB%
set SHJ_SHARED_LIB=%__SHJ_SHARED_LIB%
set SHJ_TARGET_ARCH=%__SHJ_TARGET_ARCH%
set SHJ_CONFIG=%__SHJ_CONFIG%

if /I "%__SHJ_REBUILD%"=="1" (
	nmake clean&& nmake
) else (
	if /I "%__SHJ_CLEAN%"=="1" (
		nmake clean
	) else (
		if /I "%__SHJ_BUILD%"=="1" (
			nmake
		)
	)
)
REM if !ERRORLEVEL! NEQ 0 (
REM	echo Build failed.
REM	echo Exiting...
REM exit 1
REM )

exit /B 0

:parse_loop
for /F "tokens=1,* delims= " %%a in ("%__SHJ_ARGS_LIST%") do (
    call :parse_argument %%a
@REM Reduce __SHJ_ARGS_LIST	
    set "__SHJ_ARGS_LIST=%%b"
    goto :parse_loop
)
:parse_end
exit /B 0

:parse_argument
@REM called by :parse_loop and expects the arguments to either be:
@REM 1. a single argument in %1
@REM 2. an argument pair from the command line specified as '%1=%2'

set __local_ARG_FOUND=0
@REM Architecture

if /I "%1"=="all" (
	set __SHJ_ALL=1
	set __local_ARG_FOUND=1
	goto :parse_end
)
if /I "%1"=="-arch" (
	goto :parse_arch_opt
)
if /I "%1"=="--arch" (
	goto :parse_arch_opt
)
if /I "%1"=="/arch" (
	goto :parse_arch_opt
)
if /I "%1"=="-lib" (
	goto :parse_lib_opt
)
if /I "%1"=="--lib" (
	goto :parse_lib_opt
)
if /I "%1"=="/lib" (
	goto :parse_lib_opt
)
if /I "%1"=="-config" (
	goto :parse_config_opt
)
if /I "%1"=="--config" (
	goto :parse_config_opt
)
if /I "%1"=="/config" (
	goto :parse_config_opt
)
if /I "%1"=="help" (
    set __SHJ_HELP=1
    set __local_ARG_FOUND=1
)
if /I "%1"=="/help" (
    set __SHJ_HELP=1
    set __local_ARG_FOUND=1
)
if /I "%1"=="-help" (
    set __SHJ_HELP=1
    set __local_ARG_FOUND=1
)
if /I "%1"=="--help" (
    set __SHJ_HELP=1
    set __local_ARG_FOUND=1
)
if /I "%1"=="clean" (
    set __SHJ_CLEAN=1
    set __SHJ_BUILD=0
    set __SHJ_REBUILD=0
    set __local_ARG_FOUND=1
)
if /I "%1"=="build" (
    set __SHJ_CLEAN=0
    set __SHJ_BUILD=1
    set __SHJ_REBUILD=0
    set __local_ARG_FOUND=1
)
if /I "%1"=="rebuild" (
    set __SHJ_CLEAN=0
    set __SHJ_BUILD=0
    set __SHJ_REBUILD=1
    set __local_ARG_FOUND=1
)
if /I "%1"=="/?" (
    set __SHJ_HELP=1
    set __local_ARG_FOUND=1
)
if /I "%1"=="-?" (
    set __SHJ_HELP=1
    set __local_ARG_FOUND=1
)

:parse_args_end
if "%SHJ_DEBUG%" GEQ "1" (
	echo "__local_ARG_FOUND=%__local_ARG_FOUND%"
)
if "%__local_ARG_FOUND%" NEQ "1" (
	if "%SHJ_DEBUG%" GEQ "1" (
		echo "No Argument found."
	)
    set /A __SHJ_PARSE_ERROR=__SHJ_PARSE_ERROR+1
)

set __local_ARG_FOUND=

exit /B 0

:parse_lib_opt
if /I "%2"=="static" (
	set __SHJ_STATIC_LIB=1
	set __local_ARG_FOUND=1
) else (
	if /I "%2"=="shared" (
		set __SHJ_SHARED_LIB=1
		set __local_ARG_FOUND=1
	) else (
		set __local_ARG_FOUND=0
		echo "Invalid Argument"
	)
)
goto :parse_args_end


:parse_config_opt
if /I "%2"=="debug" (
	set __SHJ_CONFIG=Debug
	set __local_ARG_FOUND=1
) else (
	if /I "%2"=="release" (
		set __SHJ_CONFIG=Release
		set __local_ARG_FOUND=1
	) else (
		set __local_ARG_FOUND=0
	)
)

goto :parse_args_end


:parse_arch_opt
if /I "%2"=="x86" (
	set __SHJ_TARGET_ARCH=x86
	set __local_ARG_FOUND=1
) else (
	if /I "%2"=="x64" (
		set __SHJ_TARGET_ARCH=x64
		set __local_ARG_FOUND=1
	) else (
		if /I "%2"=="arm" (
			set __SHJ_TARGET_ARCH=arm
			set __local_ARG_FOUND=1
		) else (
			if /I "%2"=="arm64" (
				set __SHJ_TARGET_ARCH=arm64
				set __local_ARG_FOUND=1
			) else (
					set __local_ARG_FOUND=0
				)
			)
		)
)
goto :parse_args_end	
	

:usage_error
echo [ERROR:%~nx0] Error in script usage. The correct usage is:
goto :usage

:usage
echo Syntax:
echo     %~nx0 [build_type] [--arch=arch_type] [--lib=lib_type] [--config=config_type]
echo where :
echo     [build_type]: clean ^| build ^| rebuild
echo     [arch_type]: x86 ^| amd64 ^| arm ^| arm64
echo     [lib_type]: static ^| shared
echo     [config_type] : debug ^| release
echo:
echo:
echo For example:
echo     %~nx0 
echo     %~nx0 --arch=x64 --config=Debug
echo     %~nx0 clean --arch=x64 --config=Debug
echo     %~nx0 build --arch=x86 --config=Release
echo     %~nx0 rebuild --arch=arm --config=Debug --lib=static
echo:
goto :missing
	
:missing
echo Please make sure either Visual Studio or C++ Build SKU is installed.	
	
:end
set SHJ_STATIC_LIB=
set SHJ_SHARED_LIB=
set SHJ_TARGET_ARCH=
set SHJ_CONFIG=
set __SHJ_SHARED_LIB=
set __SHJ_STATIC_LIB=
set __SHJ_PARSE_ERROR=
set __SHJ_TARGET_ARCH=
set __SHJ_HOST_ARCH=
set __SHJ_CLEAN=
set __SHJ_BUILD=
set __SHJ_REBUILD=