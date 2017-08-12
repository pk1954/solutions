@echo off

set TEST=TEST_1
..\..\x64\Release\HistoryTest.exe > %TEST%.RES
..\..\Release\compare.exe %TEST%.RES %TEST%.SPC /RTF > %TEST%.PRT
@if ERRORLEVEL 1 (
	echo +++ HistoryTest TEST_ALL: error in %TEST%
	goto ERROR_EXIT
)

echo *** HistoryTest TEST_ALL: all tests ok
pause
exit /B 0

:ERROR_EXIT
pause
exit /B 99
