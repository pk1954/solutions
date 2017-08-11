rem build_configuration.bat

echo *** Building %1 version
devenv Evolution.sln /build %1
if ERRORLEVEL 1 (
	echo +++ error building %1
)
