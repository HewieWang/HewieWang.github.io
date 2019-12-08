### Nginx安装配置
1、安装pcre软件包（使nginx支持http rewrite模块）
```Bash
yum install -y pcre
yum install -y pcre-devel
```
2、安装openssl-devel（使nginx支持ssl）
```Bash
yum install -y openssl-devel
```
3.下载nginx 安装包 http://nginx.org/download/
```Bash
wget http://nginx.org/download/nginx-1.13.6.tar.gz
tar xvf nginx-1.13.6.tar.gz
./configure  --prefix=/usr/local/nginx --conf-path=/usr/local/nginx/conf/nginx.conf --error-log-path=/var/log/nginx/error.log  --http-log-path=/var/log/nginx/access.log  --pid-path=/var/run/nginx/nginx.pid --lock-path=/var/lock/nginx.lock --with-http_ssl_module --with-http_stub_status_module --with-http_gzip_static_module --with-pcre
make
make install
```
ps.nginx完成安装以后，有四个主要的目录：
```Bash
conf：保存nginx所有的配置文件，其中nginx.conf是nginx服务器的最核心最主要的配置文件，其他的.conf则是用来配置nginx相关的功能的，例如fastcgi功能使用的是fastcgi.conf和fastcgi_params两个文件，配置文件一般都有个样板配置文件，是文件名.default结尾，使用的使用将其复制为并将default去掉即可。

html目录中保存了nginx服务器的web文件，但是可以更改为其他目录保存web文件,另外还有一个50x的web文件是默认的错误页面提示页面。

logs：用来保存nginx服务器的访问日志错误日志等日志，logs目录可以放在其他路径，比如/var/logs/nginx里面。

sbin：保存nginx二进制启动脚本，可以接受不同的参数以实现不同的功能。
```
加入环境变量
```Bash
vim /etc/profile
export PATH=$PATH:/usr/local/nginx/sbin
source /etc/profile
```
命令|作用
--|:--:
nginx|启动服务
nginx -s reload|不停止服务重读配置文件
nginx -s stop|停止服务
查看Nginx状态
```Bash
ps -ef | grep nginx
```