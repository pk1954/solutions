@echo off
set DATETIME=%date:~6,4%%date:~3,2%%date:~0,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set TESTPATH="EvolutionTest_%DATETIME%"
rem set MASTER=//SYNOLOGYNAS/Peter/SW-projects/Evolution
set MASTER=C:\Users\peter\SW-projects\Evolution
echo git clone %MASTER% %TESTPATH%
git clone %MASTER% %TESTPATH%
if ERRORLEVEL 1 (
	echo +++ error in git clone
	goto END
)

echo cd %TESTPATH%
cd %TESTPATH%
if ERRORLEVEL 1 (
	echo +++ error in cd %TESTPATH%
	goto END
)

echo *** ok ***
:END
pause
