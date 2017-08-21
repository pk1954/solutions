@echo off

set EVOLUTION_CONSOLE=..\x64\Release\EvolutionConsole.exe

call :TEST_CASE Test_1 /Text

echo *** EVOLUTION_CONSOLE tests ok
pause
exit /B 0

:ERROR_EXIT
pause
exit /B 99

rem *** subroutines ***

:TEST_CASE
%EVOLUTION_CONSOLE% %1.IN 
rem > %1.RES
rem call :COMPARE_RESULTS %1
exit /B 0

:COMPARE_RESULTS
echo Test %1
%COMPARE% %1.RES %1.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test %1
	goto ERROR_EXIT
)
exit /B 0

