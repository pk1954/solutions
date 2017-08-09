REM @echo off
%1 > %2.RES
if exist %2.SPC goto COMP
goto END
:COMP
COMPARE %2.RES %2.SPC %3 > %2.PRT
:END