@echo off&setlocal enabledelayedexpansion
for /f "delims=" %%i in (C:\qq.txt) do set "website=%%i"&call :s !website!
pause
:s
start tencent://Message/?Uin=%1
goto :eof