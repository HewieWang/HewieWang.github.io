```Bash
curl https://www.example.com
```
POST
```Bash
curl -d'login=emma＆password=123'-X POST https://google.com/login
```
文件
```Bash
curl -F 'file=@photo.png' https://google.com/profile
```
-k参数指定跳过 SSL 检测
```Bash
curl -k https://www.example.com
```
-o参数将服务器的回应保存成文件，等同于wget命令
```Bash
curl -o example.html https://www.example.com
```