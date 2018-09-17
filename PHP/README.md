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
# PHP获取2个字符之间的字符串
###
```PHP
function get_between($input, $start, $end) {
    $substr = substr($input, strlen($start)+strpos($input, $start),(strlen($input) - strpos($input, $end))*(-1));
    return $substr;
}
```
# PHP判断是否有中文
###
```PHP
$str = 'php学习博客';
if(preg_match('/[\x7f-\xff]/', $str)){
  echo '字符串中有中文<br/>';
}else{
  echo '字符串中没有中文<br/>';
}

if(preg_match('/^[\x7f-\xff]+$/', $str)){
  echo '字符串全是中文';
}else{
  echo '字符串不全是中文';
}
```
