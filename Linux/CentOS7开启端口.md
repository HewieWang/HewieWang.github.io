### 方法一：使用firewall
### 1、运行命令：
####  firewall-cmd --get-active-zones
####  运行完成之后，可以看到zone名称，如下：
#### 2、执行如下命令命令：

#### firewall-cmd --zone=public --add-port=6379/tcp --permanent

#### 3、重启防火墙，运行命令：
#### firewall-cmd --reload

#### 4、查看端口号是否开启，运行命令：
#### firewall-cmd --query-port=6379/tcp

### 方法二：使用iptables
#### 1、先运行如下命令：

#### /sbin/iptables -I INPUT -p tcp --dport xxx -j ACCEPT

#### 2、然后运行：

#### /etc/rc.d/init.d/iptables save 

#### 或者这个命令：

#### 1、-A INPUT -m state –state NEW -m tcp -p tcp –dport xxx -j ACCEPT 

#### 2、在运行这个命令：

#### -A INPUT -j REJECT –reject-with icmp-host-prohibited

#### 注：xxx ——表示你要开启的端口号，如：6379

#### 测试：在windows下按下win+R键，输入cmd，运行命令(需开启telnet)，如果变成空界面表示成功：

#### telnet  192.168.xx.xx 6379