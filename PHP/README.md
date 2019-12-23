# PHP CURL去除响应头，响应主体的方法
###
```PHP
curl_setopt($ch, CURLOPT_HEADER, false);  //设置false表示只需要响应的正文
curl_setopt($ch, CURLOPT_NOBODY, FALSE);  //设置false表示只需要响应头部
```
# PHP7 生成指定位数随机字符串
###
```PHP
function str_rand($length = 32, $char = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ') {
    if(!is_int($length) || $length < 0) {
        return false;
    }
    $string = '';
    for($i = $length; $i > 0; $i--) {
        $string .= $char[mt_rand(0, strlen($char) - 1)];
    }
    return $string;
}
```
# 多个字符的排列组合(指定位数)
###
```PHP
function getCombinationToString($arr, $m) {
    if ($m ==1) {
       return $arr;
    }
    $result = array();
    $tmpArr = $arr;
    unset($tmpArr[0]);
    for($i=0;$i<count($arr);$i++) {
        $s = $arr[$i];
        $ret = getCombinationToString(array_values($tmpArr), ($m-1), $result);
        foreach($ret as $row) {
            $result[] = $s . $row;
        }
    }
    return $result;
}
$arr = array('a','b','c','d');
$result = array();
$t = getCombinationToString($arr, 4);
print_r($t);
```
# PHP 去掉字符串最后一个字符
###
```PHP
$newstr = substr($str,0,strlen($str)-1);
```
# PHP 截取某个字符之前的字符串
###
```PHP
$newa = substr($str,0,strrpos($str,'$'));
```
# PHP 替换所有a标签href
###
```PHP
$str=preg_replace('/href=(\"|\')(.*?)(\"|\')/i','href="#"', $str);
```
# PHP随机调用TXT文件中一行
###
```PHP
$f='1.txt';//文件名
$a=file($f);//把文件的所有内容获取到数组里面
$n=count($a);//获得总行数
$rnd=rand(0,$n);//产生随机行号
$rnd_line=$a[$rnd];//获得随机行
echo $rnd_line; //显示结果
```

# PHP随机调用TXT文件中某两行
###
```PHP
$arr = explode("\n", file_get_contents('1.txt'));
$key = array_rand($arr, 2);
echo $arr[$key[0]];
echo $arr[$key[1]];
```

# PHP随机调用中文字符串
###
```PHP
function mb_str_shuffle($str){
    $str_hz = preg_replace('/[^\x{4e00}-\x{9fa5}]/u', '', $str);
    $str_zm = preg_replace('/([\x80-\xff]*)/i','',$str);
    $re1 = chunk_split($str_hz,3,",");
    $re2 = chunk_split($str_zm,1,",");
    if($re1){
        $re1 = explode(",",$re1);
    }
    if($re2){
        $re2 = explode(",",$re2);
    }
    if($re1 && $re2){
        $re = array_merge($re1,$re2);
    }elseif($re1){
        $re = $re1;
    }elseif($re2){
        $re = $re2;
    }
    shuffle($re);
    $newstr = implode($re);
    $newstr=mb_substr($newstr,0,rand(0,count($re)-1),'utf-8');
    return $newstr;
}
```

# PHP将数组值转换为小写
###
```PHP
$yourArray = array_map('strtolower', $yourArray);
```

# PHP判断字符串是否在TXT中(按行)(黑名单)
###
```PHP
$file = fopen("1.txt", "r");
$user=array();
$i=0;
while(!feof($file)){$user[$i]= fgets($file);$i++;}
fclose($file);
$user=array_filter($user);
foreach ($user as $k => $v) {
	 if(trim($v)=="999"){
	 	echo '在列表中!';exit;
	 }
}
```

# PHP截取字符串指定部分(某符号前或后)
###
```PHP
/**
 * 按符号截取字符串的指定部分
 * @param string $str 需要截取的字符串
 * @param string $sign 需要截取的符号
 * @param int $number 如是正数以0为起点从左向右截  负数则从右向左截
 * @return string 返回截取的内容
 */
function cut_str($str,$sign,$number){
    $array=explode($sign, $str);
    $length=count($array);
    if($number<0){
        $new_array=array_reverse($array);
        $abs_number=abs($number);
        if($abs_number>$length){
            return 'error';
        }else{
            return $new_array[$abs_number-1];
        }
    }else{
        if($number>=$length){
            return 'error';
        }else{
            return $array[$number];
        }
    }
}
$str="127.0.0.1:5524";
echo cut_str($str,':',0);
```

# PHP读取TXT文件从N行到M行函数,返回字符串
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

# PHP高效率的读取TXT大文件行数
###
```PHP
function count_line($file){
  $fp=fopen($file, "r");
  $i=0;
  while(!feof($fp)) {
   //每次读取2M
   if($data=fread($fp,1024*1024*2)){
   //计算读取到的行数
   $num=substr_count($data,"\n");
   $i+=$num;
  }
}
fclose($fp);
return $i;
}
```

# PHP生成服务器信息函数
###
```PHP
function get_server_info(){
    $info = array(
        '运行环境'=>PHP_OS.':'.$_SERVER["SERVER_SOFTWARE"],
        'WEB服务端口'=>$_SERVER['SERVER_PORT'],
        '浏览器信息'=>substr($_SERVER['HTTP_USER_AGENT'], 0, 40),
        '通信协议'=>$_SERVER['SERVER_PROTOCOL'],
        '请求方法'=>$_SERVER['REQUEST_METHOD'],
        '上传附件限制'=>ini_get('upload_max_filesize'),
        '执行时间限制'=>ini_get('max_execution_time').'秒',
        '服务器时间'=>date("Y年n月j日 H:i:s"),
        '服务器域名/IP'=>$_SERVER['SERVER_NAME'].' [ '.gethostbyname($_SERVER['SERVER_NAME']).' ]',
        '用户IP地址'=>$_SERVER['REMOTE_ADDR'],
    );
    return $info;
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
# PHP全排列
###
```PHP
$results = [];
$arr = [];

function bfs($start) {
    global $arr;
    global $results;

    $queue = [];
    array_push($queue, $start);

    while( !empty($queue) ) {
        $cur = array_shift($queue);
        if(strlen($cur) === count($arr)) {
            array_push($results, $cur);
        }

        $arr_temp = $arr;
        for ($i=0; $i<strlen($cur); $i++) {
            unset($arr_temp[$cur[$i]]);
        }

        foreach ($arr_temp as $key => $value) {
            $node = $cur . $key;
            array_push($queue, $node);
        }
    }
}

function allPermutation($string) {
    $array = [];
    for($i=0; $i<strlen($string); $i++) {
        array_push($array, $string[$i]);
    }
    sort($array);

    foreach ($array as $item) {
        global $arr;
        $arr[$item] = 1;
    }

    foreach ($array as $item) {
        bfs($item);
    }
}

allPermutation('abcde');
var_dump($results);
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