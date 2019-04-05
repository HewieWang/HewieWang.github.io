# PHP获取文件名、文件路径、文件后缀名信息函数
```PHP
function fileinfo($path){
	$dir = dirname($path);
	$name = basename($path);
	$suffix = pathinfo($path,PATHINFO_EXTENSION);
	$info = array(
			'dir'=>$dir,
			'name'=>$name,
			'suffix'=>$suffix
		);
	return $info;
}
```
## 调用
```PHP
$file= fileinfo('var/www/www.02405.com/logo.jpg');
var_dump($file);
```
## 输出
```PHP
array(3) {
	["dir"]=>
	string(3) "var/www/www.02405.com"
	["name"]=>
	string(5) "logo.jpg"
	["suffix"]=>
	string(3) "jpg"
}
```