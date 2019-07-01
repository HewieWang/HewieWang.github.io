### PHP拆分url并提取域名、方法、参数
```PHP
$url = 'http://www.sina.com.cn/abc/de/fg.php?id=1';
$arr = parse_url($url);
print_r($arr);
```
#### 此时$arr就是一个一维数组
```PHP
Array
(
    [scheme] => http
    [host] => www.sina.com.cn
    [path] => /abc/de/fg.php
    [query] => id=1
)
```
```PHP
echo $host = $arr['host'];      //输出www.sina.com.cn
```