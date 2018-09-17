# PHP给每一行文本加引号,逗号
###
```PHP
$str="'".implode("',\n'",explode("\n",str_replace("\r",'',$str)))."'";
echo $str;
```
# PHP去除所有空格
###
```PHP
function trimall($str)
{
    $qian=array(" ","　","\t","\n","\r");
    $hou=array("","","","","");
    return str_replace($qian,$hou,$str); 
}
echo trimall("a b c");
```
