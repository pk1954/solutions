set DATETIME=%date:~6,4%%date:~3,2%%date:~0,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set TESTPATH="EvolutionTest_%DATETIME%"
git clone //SYNOLOGYNAS/Peter/Evolution.git %TESTPATH%
if ERRORLEVEL 1 (
	echo +++ error in git clone
	goto END
)

set VCVARS=\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat
if exist "C:\Program Files%VCVARS%"       call "C:\Program Files%VCVARS%"
if exist "C:\Program Files (x86)%VCVARS%" call "C:\Program Files (x86)%VCVARS%"
if ERRORLEVEL 1 (
	echo +++ error in calling vcvars32
	goto end
)

cd %TESTPATH%
if ERRORLEVEL 1 (
	echo +++ error in cd %TESTPATH%
	goto END
)

devenv Evolution.sln /build Debug 
if ERRORLEVEL 1 (
	echo +++ error building Debug
	goto END
)

devenv Evolution.sln /build Release 
if ERRORLEVEL 1 (
	echo +++ error building Release
	goto END
)

echo *** ok ***
:END
pause
