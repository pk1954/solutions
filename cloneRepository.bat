set DATETIME=%date:~6,4%%date:~3,2%%date:~0,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set TESTPATH="EvolutionTest_%DATETIME%"
git clone //SYNOLOGYNAS/Peter/Evolution %TESTPATH%
if ERRORLEVEL 1 (
	echo +++ error in git clone
	goto END
)

cd %TESTPATH%
if ERRORLEVEL 1 (
	echo +++ error in cd %TESTPATH%
	goto END
)

echo *** ok ***
:END
pause
