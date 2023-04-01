@echo off

REM Execute InitProject.bat before building

REM cd to project home directory
cd ../
cd ./UnrealJSScripts

REM InitProject.bat will cd to project home dir. So building project there.
call yarn run build_shipping
pause


