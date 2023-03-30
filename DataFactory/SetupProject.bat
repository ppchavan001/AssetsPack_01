@echo off

cd ./UnrealJSScripts
call npm install
call uts init
call npx babel src --out-dir out --extensions ".ts,.tsx"


