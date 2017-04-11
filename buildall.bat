set dateTime=%date:~6,4%%date:~3,2%%date:~0,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set testpath="EvolutionTest_%dateTime%"
git clone //SYNOLOGYNAS/Peter/Evolution.git %testpath%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
cd %testpath%
devenv Evolution.sln /build Debug 
devenv Evolution.sln /build Release
pause

