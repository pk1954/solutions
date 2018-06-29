@echo off

echo [30m[47m **** TEST_ALL.BAT

if "%1"=="" (
	set APP_PATH=..\..\x64\Release
) else (
	set APP_PATH=%1
)

set APP_NAME=EvolutionConsole

set COMPARE=%APP_PATH%\compare.exe
set TESTEE=%APP_PATH%\%APP_NAME%.exe

call :TEST_CASE Test_1 /Text || goto ERROR_EXIT

echo *** EVOLUTION_CONSOLE tests ok
echo .
pause Press any key
exit /B 0

:ERROR_EXIT
echo +++ EVOLUTION_CONSOLE tests failed
echo .
pause Press any key
exit /B 99

rem *** subroutines ***

:TEST_CASE
echo [30m[47m **** Start test case %1
del %1.RES
if not exist %1.in (
	echo +++ could not find %1.in
	exit /B 1
)
%TESTEE% %1.in > %1.RES
if not exist %1.SPC (
	echo +++ could not find %1.SPC
	exit /B 2
)
echo [30m[47m **** Start COMPARE
%COMPARE% %1.RES %1.SPC  /Text > %1.PRT
echo [30m[47m
if ERRORLEVEL 1 (
	echo +++ error in test case "%1"
	type %1.PRT
	echo [30m[47m **** Test case %1 failed
	exit /B 3
)
exit /B 0

