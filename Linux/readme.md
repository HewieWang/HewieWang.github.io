# 从Linux服务器下载文件或文件夹(Git)
###
```Bash
scp -r root@45.118.250.225:/www/wwwroot/www.tar.gz /c/Users/Administrator/Desktop/
```
# Linux打包文件夹
###
```Bash
tar -czvf www.tar.gz /www
```
# Centos解压tgz
###
```Bash
tar xvf 1234.tgz
```
# Linux解压压缩包
###
```Bash
tar -zxvf www.tar.gz
```
# 移动文件
###
```Bash
mv www.tar.gz /home/www/
```
# crontab每3个小时执行一次
###
```Bash
$ crontab -e
1 */3 * * * commands
```
# 神奇 指令 chattr
###
```Bash
chatter: 锁定文件，不能删除，不能更改
        +a:  只能给文件添加内容，但是删除不了，
              chattr +a  /etc/passwd
        -d:      不可删除
        加锁：chattr +i  /etc/passwd       文件不能删除，不能更改，不能移动
        查看加锁： lsattr /etc/passwd      文件加了一个参数 i 表示锁定
        解锁：chattr -i /home/omd/h.txt    - 表示解除
隐藏chattr命令：
		which chattr
		mv /usr/bin/chattr  /opt/ftl/
		cd /opt/ftl/
		mv chattr h    -->更改命令，使用别名h隐藏身份
		/opt/ftl/h +i /home/omd/h.txt   -->利用h 行驶chattr命令
		lsattr /home/omd/h.txt    -->查看加密信息
恢复隐藏命令:
		mv h /usr/bin/chattr
		chattr -i /home/omd/h.txt
		lsattr /home/omd/h.txt
```
# 删除文件
###
```Bash
rm www.tar.gz
```
# 重命名(将文件test.log重命名为test1.txt)
###
```Bash
mv test.log test1.txt
```
# 切换到root
###
```Bash
sudo su
```
# 修改root密码
###
```Bash
passwd
```
# Linux联网用vi编辑文件/etc/sysconfig/network-scripts/ifcfg-eth0
###
```Bash
BOOTPROTO=dhcp（没变）
ONBOOT=yes（修改之前是no）
NM_CONTROLLED=no（修改之前是yes）
```
# 上传下载命令：RZ，SZ
```Bash
yum -y install lrzsz 
```
