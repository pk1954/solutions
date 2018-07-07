@echo off

if "%1"=="" (
	set APP_PATH=..\..\x64\Release
) else (
	set APP_PATH=%1
)

echo *** APP_PATH = %APP_PATH%

set APP_NAME=EvolutionCoreTest

set TESTEE=%APP_PATH%\%APP_NAME%.exe
set COMPARE=..\..\Release\compare.exe

if not exist %TESTEE% (
	echo +++ Could not find %TESTEE%
	goto ERROR_EXIT
)

if not exist %COMPARE% (
	echo +++ Could not find %COMPARE%
	goto ERROR_EXIT
)

call :TEST_CASE TEST_1 || goto ERROR_EXIT

echo *** %APP_NAME% tests ok
exit /B 0

:ERROR_EXIT
echo +++ error in %APP_NAME% tests
pause
exit /B 99

rem *** subroutines ***

:TEST_CASE
echo *** %1
del %1.RES
@if not exist %1.IN (
	echo +++ could not find %1.IN
	exit /B 3
)
%TESTEE% %1.IN > %1.RES
@if ERRORLEVEL 1 (
	echo +++ error in %APP_NAME% %1.IN
	exit /B 1
)
%COMPARE% %1.RES %1.SPC /RTF > %TEST%.PRT
@if ERRORLEVEL 1 (
	echo +++ error in %COMPARE% %1.RES %1.SPC
	exit /B 2
)
exit /B 0
