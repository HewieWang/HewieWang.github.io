1. Windows Server 搭建VPN服务器
winserver-

打开服务器管理器，右击角色-添加角色

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-1.jpg)

点击下一步，选择网络策略和访问服务

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-2.jpg)

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-3.jpg)

下一步，选择网络策略服务器和路由和远程访问服务

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-4.jpg)

一路下一步完成安装

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-5.jpg)

右击路由和远程访问，选择配置并启用路由和远程访问

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-6.jpg)

下一步，选择自定义配置

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-7.jpg)

下一步，选择VPN访问、NAT、LAN路由

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-8.jpg)

下一步到最后，启动服务

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-9.jpg)

进入路由和远程访问-IPv4，右击NAT，选择新增接口

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-10.jpg)

选择本地连接

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-11.jpg)

设置属性，勾选公用接口连接到Internet，并选择在此接口上启用NAT

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-12.jpg)

接下来建立VPN访问用户，由于本机使用的是域环境，所以在Active Directory 域服务-Active Directory 用户和计算机-域名-User中创建用户，如果没有域环境在本机用户中创建即可

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-13.jpg)

在中间空白处右击创建用户

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-14.jpg)

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-15.jpg)

创建完用户后记得更改用户属性

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-16.jpg)

在属性中找到拨入选项卡，将网络访问权限改为允许访问

![avatar](https://cjjkkk.github.io/image/ser08vpn/winserver-17.jpg)

至此，Windows Server上的VPN服务器已经搭建完成，接下来去测试服务器是否能够正常连接