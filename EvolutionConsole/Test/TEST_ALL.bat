@echo off
REM output color: blue

set APP_NAME=EvolutionConsole

call ..\..\Tests\TEST_SETUP %1

%CALL_TEST_CASE% Test_1 || goto ERROR_EXIT
%CALL_TEST_CASE% Test_2 || goto ERROR_EXIT
%CALL_TEST_CASE% Test_3 || goto ERROR_EXIT

echo %MESSAGE% tests ok
echo .
pause Press any key
exit /B 0

:ERROR_EXIT
echo %MESSAGE% tests failed
echo .
pause Press any key
exit 99
