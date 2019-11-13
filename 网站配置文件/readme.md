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
### web.config设置目录只读，不能执行PHP
```Bash
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <system.webServer>
        <handlers accessPolicy="Read" />
    </system.webServer>
</configuration>
```
### IIS配置文件位置
```Bash
C:\Windows\System32\inetsrv\config\applicationHost.config
如文件损坏备份位置: C:\inetpub\history
```