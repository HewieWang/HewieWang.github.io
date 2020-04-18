@echo off
set a=0
setlocal EnableDelayedExpansion
for %%n in (*.txt) do (
set /A a+=1
ren "%%n" "jz!a!.txt"
)