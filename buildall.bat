set dateTime=%date:~6,4%%date:~3,2%%date:~0,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set testpath="EvolutionTest_%dateTime%"
git clone //SYNOLOGYNAS/Peter/Evolution.git %testpath%
if %errorlevel% equ 0 goto ENV
echo +++ error in git clone
goto END
:ENV
pause
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
if %errorlevel% equ 0 goto CD
echo +++ error in cd %testpath%
goto end
:CD
pause
cd %testpath%
if %errorlevel% equ 0 goto DEBUG
echo +++ error in cd %testpath%
goto END
:DEBUG
pause
devenv Evolution.sln /build Debug 
if %errorlevel% equ 0 goto RELEASE
echo +++ error building Debug
goto END
:RELEASE
pause
devenv Evolution.sln /build Release
if %errorlevel% equ 0 goto OK
echo +++ error building Release
goto END
:OK
echo *** ok ***
:END
pause
