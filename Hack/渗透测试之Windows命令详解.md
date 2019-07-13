### 0x01 快捷命令
```Bash
iexpress 系统自带文件(木马)捆绑工具

cleanmgr 磁盘整理

gpedit.msc 本地组策略

secpol.msc 本地安全设置
dcomcnfg 系统组件服务

eventvwr.msc 事件查看器
lusrmgr.msc 本地用户及用户组

regedit 注册表

taskmgr 任务管理器

explorer 资源管理器

devmgmt.msc 设备管理器

dcomcnfg 系统组件服务

control 控制面板

control admintools 管理工具

services.msc 服务

ncpa.cpl 本地网络

moconfig 启动项

sysdm.cpl 系统属性
```

### 0x02 进程命令
```Bash
msg username | server:servername "notice:you are hack by me" 系统弹窗

net start 查看开启的服务

net stop service_name 停止服务

tasklist 显示在本地或远程机器上当前运行的进程列表。

taskkill

/F 指定强制终止进程。
/PID processid 指定要终止的进程的 PID。使用 TaskList 取得 PID。
schtasks 查看计划任务

SCHTASKS /Create /SC DAILY /TN shutdown /TR "C:\WINDOWS\System32\shutdown.exe -t 0 -s" /ST 23：00
成功: 成功创建计划任务 "shutdown",每天定时关机
schtasks /Delete /TN shutdown 删除指定计划任务
还可以指定远程计算机，需要管理员权限，留待读者自查吧
net start service 启动服务

net pause service 暂停服务
net continue service 激活服务
net stop service 停止服务
文件命令
where shutdown.exe 在当前路径或path下查找程序或文件的位置

where /R D:\ user.rule 指定从D盘根目录递归搜索文件
for /r d:\ %i in (*.rule) do echo %i 指定D盘,搜索后缀为.rule的所有文件

find "user.rule" 比较慢

md [dir_name] 创建目录

rd [dir_name] 删除目录
type [file_name] 查看文件

start [file_name] 查看文件
del [file_name] 删除文件
copy [file_name] [target_path] 复制文件

move [file_name] [target_path] 移动文件
copy con 1.txt 创建文件 输入内容按ctrl+z
attrib 文件名(目录名) 查看某文件（目录）的属性

-A -R -S -H 去掉某文档的存档，只读。系统，隐藏属性
+A +R +S +H 添加某文档的存档，只读。系统，隐藏属性
```
### 0x03 网络命令
```Bash
netsh wlan set hostednetwork mode=allow ssid=[111222] key=123456.com.com 设置无线网络

netsh wlan start hostednetwork 启动承载网络
netsh wlan stop hostednetwork 停止承载网络
ping -l 65500 www.baidu.com 死亡之ping

tracert www.baidu.com 显示路由路径

route print 查看路由表

getmac 获取本地mac地址

arp -a 查看局域网内的主机

net view 显示当前域的计算机列表

net view \ip 查询某IP共享
net use 连接计算机或断开计算机与共享资源的连接

net use k: \[ip]\c$ 把目标主机的C盘映射为本地的k盘

net use k: \[ip]\c$ /del 断开连接
net share 查看本地开启的共享

net share ipc$ 开启ipc$共享
net share ipc$ /del 删除ipc$共享
net share c$ /del 删除c$共享
netstat -an 查看开启的端口和网络连接情况
```

### 0x04 账户命令
```Bash
runas /noprofile /user:mymachine\administrator cmd 以管理员权限运行cmd

net user 查看用户

net user test test /add 添加用户
net user test 查看test的信息
net user test new_pass 更改用户登录密码
net user test /del 删除test
net user guest /active:yes 激活guest用户(一般使用此用户避免让管理员察觉)

net user guest 12345 用guest用户登陆后将密码改为12345
net locaLGroup 查看用户组

net localgroup administrators test /add 将test添加到管理员组
net localgroup "remote desktop users" test /add 将test加入远程访问组
whoami 查看当前用户

query 查看当前在线的用户

域渗透：当我们拿到一台在域的机器时

net view /domain 查看域列表
net user /domain 查看域用户
net group /domain 查看域中的组
net user administrator /domain 查看域用户administrator属于哪个组
net localgroup administrators /domain 查询登录过本机的域管理员
net config workstation 当前登录域
```
### 0x05 管道命令
```Bash
|

将第一条命令的结果作为第二条命令的参数来使用
netstat -n -p tcp│find ":3389">>D:\IP.log
││

同时执行多条命令，当碰到执行正确的命令后将不执行后面的命令，如果没有出现正确的命令则一直执行完所有命令
dir 1.gif ││ del 1.gif
&

同时执行多条命令
ping www.baidu.com & whoami
&&
同时执行多条命令，当碰到执行出错的命令后将不执行后面的命令，如果一直没有出错则一直执行完所有命令
dir file &://192.168.0.1/database/backup.mdb && copy file&://192.168.0.1/database/backup.mdb
>>

追加到文件输出到文件或设备
ping www.baidu.com | find "来自" >> D:\IP.txt
>

输出到文件（清空原来内容）或设备
ping www.baidu.com | find "来自" > D:\IP.txt more
more 显示由 find 命令筛选的文本（在命令提示符窗口中每次显示一屏）
<

从文件中而不是从键盘中读入命令输入
```