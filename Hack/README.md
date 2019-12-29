### cmd_powershell_create_user
```Bash
Syntax of creating new users and grant them admin right (Powershell & CMD)

Bash

cmd.exe < run Administrator
net user /add [Username] [Password]
new localgroup Administrators [Username] /add

Powershell

NET USER [Username] [Password]/add /y /expires:never
NET LOCALGROUP Administrators [Username]/add
WMIC USERACCOUNT WHERE "Name='[Username]'" SET PasswordExpires=FALSE
```
### 查询终端端口 CMD Find 3389 PORT 得到16进制端口号
```Bash
REG query HKLM\SYSTEM\CurrentControlSet\Control\Terminal" "Server\WinStations\RDP-Tcp /v PortNumber
```
### cmd命令读取IIS7网站信息
```Bash
C:\Windows\System32\inetsrv\appcmd list VDIR
```