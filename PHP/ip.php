<?php 

$allowIp = ['203.177.102.120'];

$ip=$_SERVER['REMOTE_ADDR'];

if (!in_array($ip, $allowIp)){
	echo '联系管理员加入白名单!';
    exit();
}

 ?>