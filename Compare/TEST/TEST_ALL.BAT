@echo off

set COMPARE=..\..\Release\compare.exe

%COMPARE% > ERR_1_RES.RES
call :COMPARE_RESULTS ERR_1

%COMPARE% XYZ.RES > ERR_2_RES.RES
call :COMPARE_RESULTS ERR_2

%COMPARE% XYZ.RES XYZ.SPC /Text > ERR_3_RES.RES
call :COMPARE_RESULTS ERR_3

%COMPARE% ERR_4.RES XYZ.SPC /Text > ERR_4_RES.RES
call :COMPARE_RESULTS ERR_4

call :TEST_CASE ERR_5 /Text
call :TEST_CASE ERR_6 /Text
call :TEST_CASE ERR_7 /Text
call :TEST_CASE ERR_8 /Text
call :TEST_CASE ERR_9 /Text
call :TEST_CASE ERR_10 /Text

call :TEST_CASE F_1 /Text 
call :TEST_CASE F_2 /Text
call :TEST_CASE F_3 /Text
call :TEST_CASE F_4 /Text
call :TEST_CASE F_5 /Text
call :TEST_CASE F_6 /Text
call :TEST_CASE F_7 /RTF

echo *** COMPARE tests ok
exit /B 0

:ERROR_EXIT
pause
exit /B 99

rem *** subroutines ***

:TEST_CASE
%COMPARE% %1.RES %1.SPC %2 > %1_RES.RES
call :COMPARE_RESULTS %1
exit /B 0

:COMPARE_RESULTS
echo Test %1
%COMPARE% %1_RES.RES %1_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test %1
	goto ERROR_EXIT
)
exit /B 0

