@echo off

rem *** main program starts here ***

for %%C in ( Debug, Release ) do (
    call :PERFORM_TESTS ..\..\%%C || goto ERROR_EXIT
)

if %PROCESSOR_ARCHITECTURE% == AMD64 (
    for %%C in ( Debug, Release ) do (
        call :PERFORM_TESTS ..\..\x64\%%C || goto ERROR_EXIT
    )
)

echo ***
echo *** TESTALL: all tests ok ***
:ERROR_EXIT
pause
exit /b 99

rem *** subroutines ***

:PERFORM_TESTS
echo ***
echo *** PERFORM_TESTS for %1
echo ***
for /D %%T in ( * ) do if exist %%T\TEST\TEST_ALL.BAT (
	echo ***
	echo *** Start %%T tests ***
    cd %%T\TEST
    call TEST_ALL %1 || goto ERROR_EXIT_FROM_PERFORM_TESTS
    cd ..\..
)

exit /b

:ERROR_EXIT_FROM_PERFORM_TESTS
cd ..\..
exit /B 1



