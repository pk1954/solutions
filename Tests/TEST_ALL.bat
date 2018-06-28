@echo off

if "%1"=="" (
	set APP_PATH=..\x64\Release
) else (
	set APP_PATH=%1
)

set APP_NAME=EvolutionConsole

set COMPARE=..\Release\compare.exe
set TESTEE=%APP_PATH%\%APP_NAME%.exe

call :TEST_CASE Test_1 /Text || goto ERROR_EXIT

echo *** EVOLUTION_CONSOLE tests ok
pause
exit /B 0

:ERROR_EXIT
echo +++ EVOLUTION_CONSOLE tests failed
pause
exit /B 99

rem *** subroutines ***

:TEST_CASE
del %1.RES
if not exist %1.IN (
	echo +++ could not find %1.IN
	exit /B 3
)
%TESTEE% %1.IN > %1.RES
if not exist %1.SPC (
	echo +++ could not find %1.SPC
	exit /B 3
)
%COMPARE% %1.RES %1.SPC
if ERRORLEVEL 1 (
	echo +++ error in Test %1
	goto ERROR_EXIT
)
exit /B 0

