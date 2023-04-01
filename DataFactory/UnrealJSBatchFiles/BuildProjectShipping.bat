@echo off

REM initialise with InitProject.bat
call InitProject.bat

REM InitProject.bat will cd to project home dir. So building project there.
call npm run Build
pause


