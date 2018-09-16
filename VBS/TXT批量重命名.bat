@echo off
setlocal enabledelayedexpansion
for %%i in (y*) do (
set "str=%%i"
ren "%%i" "!str:y=TX!")