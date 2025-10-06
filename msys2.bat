@echo off
set CHERE_INVOKING=1
set MSYSTEM=UCRT64
start "" "I:\msys64\usr\bin\bash.exe" --login -i -c "./make.bat"
pause