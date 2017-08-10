@echo off
echo *** Preparing build environment
set VCVARS=\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat
if exist "C:\Program Files%VCVARS%"       call "C:\Program Files%VCVARS%"
if exist "C:\Program Files (x86)%VCVARS%" call "C:\Program Files (x86)%VCVARS%"
if ERRORLEVEL 1 (
	echo +++ error in calling vcvars32
	goto ERROR_EXIT
)

echo *** Building DEBUG version
devenv Evolution.sln /build Debug 
if ERRORLEVEL 1 (
	echo +++ error building DEBUG
	goto ERROR_EXIT
)

echo *** Building RELEASE version
devenv Evolution.sln /build Release 
if ERRORLEVEL 1 (
	echo +++ error building RELEASE
	goto ERROR_EXIT
)

echo *** Perform COMPARE tests
cd Compare\TEST
call TEST_ALL
if ERRORLEVEL 1 (
	echo +++ error in COMPARE tests
	goto ERROR_EXIT
)

echo *** BUILDALL ok ***
:ERROR_EXIT
pause
