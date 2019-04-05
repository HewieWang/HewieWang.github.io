# PHP随机生成n个汉字字符串函数
```PHP
/**
* 随机生成N个汉字
* @param  string $giveStr  必须含有的字
* @param  int $num 需要生成多少个汉字
* @return string 返回生成的字符串
*/ 
function getRndWords($giveStr="", $num=18){ 
    $str = "这个函数可以随机生成一个汉字字符串，可用于猜成语类程序生成答案使用";# 字库 
    $newStr  = "";       # 随机生成的包含答案的字符串 
    $anLo    = array();  # 设定的答案所在的位置。 
    $bit     = 3;        # 位数，在本系统中是utf-8编码，一个中文长度为3 
    $anLenth = floor(strlen($giveStr)/$bit); # 答案长度,在UTF编码中， 
     
    # 这些汉字在18个汉字中的位置 
    $i = 0; 
    while ( $i<$anLenth ) { 
        $rd = rand( 0, $num-1 ); 
        if(in_array($rd,$anLo)) continue; # 保证了不重复。 
        $anLo[] = $rd; 
        $i++; 
    } 
     
    for( $j=0; $j<$num;$j++ ){ 
        if(in_array($j,$anLo)){ 
            $k = array_search($j,$anLo); 
            $newStr .= mb_substr($giveStr,$k*$bit,$bit); #echo $newStr."<br>"; 
             
        } else { 
            $rd  = rand(0,(strlen($str)-1)/$bit); 
            $wd  = mb_substr($str,$rd*$bit,$bit); 
            $str = str_replace($wd, '', $str); 
            $newStr .= $wd; 
        } 
    } 
    return $newStr; 
}
```