@echo off

rem *** main program starts here ***

call :SET_BUILD_ENVIRONMENT vcvars32.bat
for %%C in ( "Debug|x86", "Release|x86" ) do call :BUILD_CONFIGURATION %%C

if %PROCESSOR_ARCHITECTURE% == AMD64 (
call :SET_BUILD_ENVIRONMENT vcvars64.bat
for %%C in ( Debug, Release ) do call :BUILD_CONFIGURATION %%C
)

echo *** BUILDALL ok ***
:ERROR_EXIT
pause
exit /b

rem *** subroutines ***

:SET_BUILD_ENVIRONMENT
set VCVARS=\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\%1
if exist "C:\Program Files%VCVARS%"       call "C:\Program Files%VCVARS%"
if exist "C:\Program Files (x86)%VCVARS%" call "C:\Program Files (x86)%VCVARS%"
if ERRORLEVEL 1 (
	echo +++ BUILDALL: error in calling %1
	goto ERROR_EXIT
)
exit /b

:BUILD_CONFIGURATION
echo *** Building %1 version
devenv Evolution.sln /build %1
if ERRORLEVEL 1 (
	echo +++ BUILDALL: error building %1
	goto ERROR_EXIT
)
exit /b

