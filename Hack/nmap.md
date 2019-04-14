# Nmap(端口扫描)

### 安装
```Bash
yum install nmap -y
```
### 使用
```Bash
nmap localhost    #查看主机当前开放的端口
nmap -p 1024-65535 localhost    #查看主机端口（1024-65535）中开放的端口
nmap -PS 192.168.21.163        #探测目标主机开放的端口
nmap -PS22,80,3306  192.168.21.163    #探测所列出的目标主机端口
nmap -O 192.168.21.163    #探测目标主机操作系统类型
nmap -A 192.168.21.163    #探测目标主机操作系统类型
nmap --help  #更多nmap参数请查询帮助信息
```
### 关闭或者打开主机端口
```Bash
nmap localhost    #查看主机当前开放端口
ntsysv            #打开系统服务器管理器（需要先安装yum install ntsysv），选择要关闭或者打开的服务
```