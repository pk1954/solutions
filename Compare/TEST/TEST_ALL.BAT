rem @echo off

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_1_RES.RES F_1_SPC.SPC /Text > F_1.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_1.RES F_1.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_1

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_2_RES.RES F_2_SPC.SPC /Text > F_2.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_2.RES F_2.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_2

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_3_RES.RES F_3_SPC.SPC /Text > F_3.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_3.RES F_3.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_3

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_4_RES.RES F_4_SPC.SPC /Text > F_4.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_4.RES F_4.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_4

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_5_RES.RES F_5_SPC.SPC /Text > F_5.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_5.RES F_5.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_5

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_6_RES.RES F_6_SPC.SPC /Text > F_6.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_6.RES F_6.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_6

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_7_RES.RES F_7_SPC.SPC /RTF > F_7.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe F_7.RES F_7.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test F_7

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe > ERR_1.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_1.RES ERR_1.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_1

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe XYZ.RES > ERR_2.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_2.RES ERR_2.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_2

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe XYZ.RES XYZ.SPC /Text > ERR_3.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_3.RES ERR_3.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_3

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_4_.RES XYZ.SPC /Text > ERR_4.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_4.RES ERR_4.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_4

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_5_.RES ERR_5_.SPC /Text > ERR_5.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_5.RES ERR_5.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_5

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_6_.RES ERR_6_.SPC /Text > ERR_6.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_6.RES ERR_6.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_6

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_7_.RES ERR_7_.SPC /Text > ERR_7.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_7.RES ERR_7.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_7

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_8_.RES ERR_8_.SPC /Text > ERR_8.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_8.RES ERR_8.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_8

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_9_.RES ERR_9_.SPC /Text > ERR_9.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_9.RES ERR_9.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_9

C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_10_.RES ERR_10_.SPC /Text > ERR_10.RES
C:\Users\peter\SW-projects\Evolution\x64\Release\compare.exe ERR_10.RES ERR_10.SPC
@IF ERRORLEVEL 1 ECHO Fehler bei Test ERR_10

