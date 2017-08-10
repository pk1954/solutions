@echo off

echo *** preparing build environment
set VCVARS=\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat
if exist "C:\Program Files%VCVARS%"       call "C:\Program Files%VCVARS%"
if exist "C:\Program Files (x86)%VCVARS%" call "C:\Program Files (x86)%VCVARS%"
if ERRORLEVEL 1 (
	echo +++ error in calling vcvars32
	goto end
)

echo *** building DEBUG
devenv Evolution.sln /build Debug 
if ERRORLEVEL 1 (
	echo +++ error building Debug
	goto END
)

echo *** building RELEASE
devenv Evolution.sln /build Release 
if ERRORLEVEL 1 (
	echo +++ error building Release
	goto END
)

echo *** testing COMPARE
cd compare\test
call test_all
if ERRORLEVEL 1 (
	echo +++ error in COMPARE tests
	goto END
)
cd ..\..

echo *** buildall ok ***
:END
pause
