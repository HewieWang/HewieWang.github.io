@echo off
for /f "tokens=1,2* delims=_" %%a in ('dir /a-d/b') do if exist "%%~a_%%~b" ren "%%~a_%%~b" "%%~a%%~xb"
pause