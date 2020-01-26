@echo off

set APP_NAME=HistoryTest

call ..\..\Tests\TEST_SETUP %1

%CALL_TEST_CASE% TEST_1 || goto ERROR_EXIT

echo %MESSAGE% tests ok
echo .
pause Press any key
exit /B 0

:ERROR_EXIT
echo %MESSAGE% tests failed
echo .
pause Press any key
exit 99
