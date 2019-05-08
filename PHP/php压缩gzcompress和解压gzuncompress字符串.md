```PHP
/**
 * 判断字符串是否base64编码
 */
function func_is_base64($str)
{  
    return $str == base64_encode(base64_decode($str)) ? true : false;  
} 
/**
 * 压缩内容
 */
function func_gzcompress($str, $level = 9)
{
    if (!func_is_base64($str)) {
        return base64_encode(gzcompress($str, $level));
    }
    return $str;
}
/**
 * 解压内容
 */
function func_gzuncompress($str)
{
    if (func_is_base64($str)) {
        return gzuncompress(base64_decode($str));
    }
    return $str;
}
```