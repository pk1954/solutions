rem @echo off
REM Parameters:
REM 1: Project path 
REM 2: Application name
REM 3: Test case 
REM output color: yellow

set TEST_CASE=%~1
set COMPARE=%PROJECT_PATH%\compare.exe
set TESTEE=%PROJECT_PATH%\%APP_NAME%.exe
set COLOR=[1;37m[43m  

echo %COLOR%*** Start test case "%TEST_CASE%"

del %TEST_CASE%.RES

if not exist %TEST_CASE%.in (
	echo +++ could not find %TEST_CASE%.in
	exit /B 1
)
%TESTEE% %TEST_CASE%.in > %TEST_CASE%.RES

if not exist %TEST_CASE%.SPC (
	echo +++ could not find %TEST_CASE%.SPC
	exit /B 2
)
%COMPARE% %TEST_CASE%.RES %TEST_CASE%.SPC /Text > %TEST_CASE%.PRT

if ERRORLEVEL 1 (
	echo +++ error in test case "%TEST_CASE%"
	type %TEST_CASE%.PRT
	echo %COLOR%+++ Test case "%TEST_CASE%" failed
	exit /B 3
)

echo %COLOR%*** "%TEST_CASE%" ok

exit /B 0

