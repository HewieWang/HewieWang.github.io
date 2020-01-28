1、 首先，要确保CentOS7安装了  openssh-server，在终端中输入  yum list installed | grep openssh-server



此处显示已经安装了  openssh-server，如果又没任何输出显示表示没有安装  openssh-server，通过输入  yum install openssh-server



来进行安装openssh-server

 

2、 找到了  /etc/ssh/  目录下的sshd服务配置文件 sshd_config，用Vim编辑器打开

将文件中，关于监听端口、监听地址前的 # 号去除

（备注：博主省去了一些操作方法，我作为菜鸟，还是补充下 vim 进入文本，按“i”开始编辑，编辑好之后按“esc”退到命令模式，按“:wq”保存 并退出）



然后开启允许远程登录



最后，开启使用用户名密码来作为连接验证



保存文件，退出

 

3、 开启  sshd  服务，输入 sudo service sshd start



检查  sshd  服务是否已经开启，输入ps -e | grep sshd