@echo off
cd.>B.txt
for /f "delims=" %%i in (A.txt) do (
find /i "%%i" B.txt||echo %%i>>B.txt
)