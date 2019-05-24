### Apache定时重启
```Bash
@ECHO OFF
net stop apache2
net start apache2
net stop mysql
net start mysql
```
### 定时任务CMD
```Bash
at 04:00 c:\apacheautostart.bat
```