rem @echo off

..\..\Release\compare.exe F_1.RES F_1.SPC /Text > F_1_RES.RES
..\..\Release\compare.exe F_1_RES.RES F_1_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_1
	goto ERROR_EXIT
)

..\..\Release\compare.exe F_2.RES F_2.SPC /Text > F_2_RES.RES
..\..\Release\compare.exe F_2_RES.RES F_2_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_2
	goto ERROR_EXIT
)

..\..\Release\compare.exe F_3.RES F_3.SPC /Text > F_3_RES.RES
..\..\Release\compare.exe F_3_RES.RES F_3_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_3
	goto ERROR_EXIT
)

..\..\Release\compare.exe F_4.RES F_4.SPC /Text > F_4_RES.RES
..\..\Release\compare.exe F_4_RES.RES F_4_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_4
	goto ERROR_EXIT
)

..\..\Release\compare.exe F_5.RES F_5.SPC /Text > F_5_RES.RES
..\..\Release\compare.exe F_5_RES.RES F_5_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_5
	goto ERROR_EXIT
)

..\..\Release\compare.exe F_6.RES F_6.SPC /Text > F_6_RES.RES
..\..\Release\compare.exe F_6_RES.RES F_6_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_6
	goto ERROR_EXIT
)

..\..\Release\compare.exe F_7.RES F_7.SPC /RTF > F_7_RES.RES
..\..\Release\compare.exe F_7_RES.RES F_7_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test F_7
	goto ERROR_EXIT
)

..\..\Release\compare.exe > ERR_1_RES.RES
..\..\Release\compare.exe ERR_1_RES.RES ERR_1_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_1
	goto ERROR_EXIT
)

..\..\Release\compare.exe XYZ.RES > ERR_2_RES.RES
..\..\Release\compare.exe ERR_2_RES.RES ERR_2_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_2
	goto ERROR_EXIT
)

..\..\Release\compare.exe XYZ.RES XYZ.SPC /Text > ERR_3_RES.RES
..\..\Release\compare.exe ERR_3_RES.RES ERR_3_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_3
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_4.RES XYZ.SPC /Text > ERR_4_RES.RES
..\..\Release\compare.exe ERR_4_RES.RES ERR_4_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_4
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_5.RES ERR_5.SPC /Text > ERR_5_RES.RES
..\..\Release\compare.exe ERR_5_RES.RES ERR_5_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_5
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_6.RES ERR_6.SPC /Text > ERR_6_RES.RES
..\..\Release\compare.exe ERR_6_RES.RES ERR_6_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_6
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_7.RES ERR_7.SPC /Text > ERR_7_RES.RES
..\..\Release\compare.exe ERR_7_RES.RES ERR_7_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_7
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_8.RES ERR_8.SPC /Text > ERR_8_RES.RES
..\..\Release\compare.exe ERR_8_RES.RES ERR_8_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_8
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_9.RES ERR_9.SPC /Text > ERR_9_RES.RES
..\..\Release\compare.exe ERR_9_RES.RES ERR_9_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_9
	goto ERROR_EXIT
)

..\..\Release\compare.exe ERR_10.RES ERR_10.SPC /Text > ERR_10_RES.RES
..\..\Release\compare.exe ERR_10_RES.RES ERR_10_SPC.SPC
@if ERRORLEVEL 1 (
	echo +++ error in Test ERR_10
	goto ERROR_EXIT
)

echo *** COMPARE tests ok
pause
exit /B 0

:ERROR_EXIT
pause
exit /B 99
