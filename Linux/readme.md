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
# 移动文件
###
```Bash
mv www.tar.gz /home/www/
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