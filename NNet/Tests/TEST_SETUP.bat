rem @echo off
REM output color: blue

if "%~1"=="" (
	set PROJECT_PATH=..\x64\Debug
) else (
	set PROJECT_PATH=%1
)

set CALL_TEST_CASE=call ..\Tests\TEST_CASE.bat
set MESSAGE=[1;37m[44m*** TEST_ALL.BAT %APP_NAME%

echo %MESSAGE%
