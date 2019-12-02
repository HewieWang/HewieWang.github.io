@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
set /A num=0
FOR /F "tokens=*" %%i in ('dir /A-D /B /OD /TC') do (
    IF NOT "%%i"=="%~n0%~x0" (
        set /A num+=1
        ren "%%i" "!num!%%~xi"
    )
)
ENDLOCAL
exit