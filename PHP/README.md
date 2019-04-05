# PHP CURL去除响应头，响应主体的方法
###
```PHP
curl_setopt($ch, CURLOPT_HEADER, false);  //设置false表示只需要响应的正文
curl_setopt($ch, CURLOPT_NOBODY, FALSE);  //设置false表示只需要响应头部
```

# PHP读取txt文件从N行到M行函数,返回字符串
###
```PHP
function get_lines($file_name,$start,$limit){
	$f = new SplFileObject( $file_name,'r');
	$f->seek($start);
	$ret = "";
	for( $i = 0 ; $i < $limit ; $i++ ){
	  $ret .= $f->current();
	  $f->next();
	}
	return $ret;
}
```

# PHP检查字符串的函数
###
```PHP
ctype_alnum($text); //检查是否是字母或数字或字母数字的组合
ctype_alpha($text); //检查字符串是否是字母
ctype_cntrl($text); //是否是控制字符如\n,\r,\t
ctype_digit($text); //是否是数字表示的字符
ctype_graph($text); //检查是否有任何可打印字符，除了空格（补）
ctype_lower($text); //检查是否是小写字母
ctype_upper($text); //检查是否是大写字母
ctype_space($text); //是否是空白字符
is_numeric($text);  //检查是否是数字
```

# PHP生成随机6位密码
###
```PHP
function GetRandStr($length){
    $str='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
    $len=strlen($str)-1;
    $randstr='';
    for($i=0;$i<$length;$i++){
        $num=mt_rand(0,$len);
        $randstr .= $str[$num];
    }
    return $randstr;
}
$number=GetRandStr(6);
echo $number;
```

# PHP保留两位小数(不四舍五入)
###
```PHP
function FA($a){
  $result = intval($a * pow(10, 2))/ pow(10, 2);
  return $result;
}
```

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

# XSS 和 SQL注入 过滤方法
## XSS 过滤：
###
```PHP
function XSS($val) {
    // remove all non-printable characters. CR(0a) and LF(0b) and TAB(9) are allowed
    $val = preg_replace('/([\\x00-\\x08][\\x0b-\\x0c][\\x0e-\\x20])/', '', $val);
    // straight replacements, the user should never need these since they're normal characters
    // this prevents like <IMG SRC=&#X40&#X61&#X76&#X61&#X73&#X63&#X72&#X69&#X70&#X74&#X3A&#X61&#X6C&#X65&#X72&#X74&#X28&#X27&#X58&#X53&#X53&#X27&#X29>
    $search = 'abcdefghijklmnopqrstuvwxyz';
    $search .= 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $search .= '1234567890!@#$%^&*()';
    $search .= '~`";:?+/={}[]-_|\'\\';
    for ($i = 0; $i < strlen($search); $i++) {
        // ;? matches the ;, which is optional
        // 0{0,7} matches any padded zeros, which are optional and go up to 8 chars
        // &#x0040 @ search for the hex values
        $val = preg_replace('/(&#[x|X]0{0,8}' . dechex(ord($search[$i])) . ';?)/i', $search[$i], $val);
        // with a ;
        // &#00064 @ 0{0,7} matches '0' zero to seven times
        $val = preg_replace('/(&#0{0,8}' . ord($search[$i]) . ';?)/', $search[$i], $val);
        // with a ;
    }
    // now the only remaining whitespace attacks are \t, \n, and \r
    $ra1 = array('javascript', 'vbscript', 'expression', 'applet', 'blink', 'script', 'iframe', 'frameset', 'ilayer', 'bgsound');
    $ra2 = array('onabort', 'onactivate', 'onafterprint', 'onafterupdate', 'onbeforeactivate', 'onbeforecopy', 'onbeforecut', 'onbeforedeactivate', 'onbeforeeditfocus', 'onbeforepaste', 'onbeforeprint', 'onbeforeunload', 'onbeforeupdate', 'onblur', 'onbounce', 'oncellchange', 'onchange', 'onclick', 'oncontextmenu', 'oncontrolselect', 'oncopy', 'oncut', 'ondataavailable', 'ondatasetchanged', 'ondatasetcomplete', 'ondblclick', 'ondeactivate', 'ondrag', 'ondragend', 'ondragenter', 'ondragleave', 'ondragover', 'ondragstart', 'ondrop', 'onerror', 'onerrorupdate', 'onfilterchange', 'onfinish', 'onfocus', 'onfocusin', 'onfocusout', 'onhelp', 'onkeydown', 'onkeypress', 'onkeyup', 'onlayoutcomplete', 'onload', 'onlosecapture', 'onmousedown', 'onmouseenter', 'onmouseleave', 'onmousemove', 'onmouseout', 'onmouseover', 'onmouseup', 'onmousewheel', 'onmove', 'onmoveend', 'onmovestart', 'onpaste', 'onpropertychange', 'onreadystatechange', 'onreset', 'onresize', 'onresizeend', 'onresizestart', 'onrowenter', 'onrowexit', 'onrowsdelete', 'onrowsinserted', 'onscroll', 'onselect', 'onselectionchange', 'onselectstart', 'onstart', 'onstop', 'onsubmit', 'onunload');
    $ra = array_merge($ra1, $ra2);
    $found = true;
    // keep replacing as long as the previous round replaced something
    while ($found == true) {
        $val_before = $val;
        for ($i = 0; $i < sizeof($ra); $i++) {
            $pattern = '/';
            for ($j = 0; $j < strlen($ra[$i]); $j++) {
                if ($j > 0) {
                    $pattern .= '(';
                    $pattern .= '(&#[x|X]0{0,8}([9][a][b]);?)?';
                    $pattern .= '|(&#0{0,8}([9][10][13]);?)?';
                    $pattern .= ')?';
                }
                $pattern .= $ra[$i][$j];
            }
            $pattern .= '/i';
            $replacement = substr($ra[$i], 0, 2) . '<x>' . substr($ra[$i], 2);
            // add in <> to nerf the tag
            $val = preg_replace($pattern, $replacement, $val);
            // filter out the hex tags
            if ($val_before == $val) {
                // no replacements were made, so exit the loop
                $found = false;
            }
        }
    }
    return $val;
}
```
## SQL注入过滤：
```PHP
function sqlInjection($val) {
    if (get_magic_quotes_gpc()) {
        $val = stripslashes($val);
    }
    if (version_compare(phpversion(), "4.3.0") == "-1") {
        return mysql_escape_string($val);
    } else {
        return mysql_real_escape_string($val);
    }
}
```