# PHP给每一行文本加引号,逗号
###
```PHP
$str="'".implode("',\n'",explode("\n",str_replace("\r",'',$str)))."'";
echo $str;
```
