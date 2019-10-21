# gzdeflate+str_to13+pack的一种加密解密代码方式
```PHP
$test=<<<'EVE'
echo 1; //需要加密的代码
EVE;
function String2Hex($string){
    $hex='';
    for ($i=0; $i < strlen($string); $i++){
        $hex .= dechex(ord($string[$i]));
    }
    return $hex;
}
function Hex2String($hex){
    $string='';
    for ($i=0; $i < strlen($hex)-1; $i+=2){
        $string .= chr(hexdec($hex[$i].$hex[$i+1]));
    }
    return $string;
}
$test1= gzdeflate($test);  #压缩
$test1=base64_encode($test1);  #base64编码
$adam=String2Hex(str_rot13($test1));
$decoded=gzinflate(base64_decode(str_rot13(pack("H*",$adam)))); #base64解码，再gzinflate解压还原,最后eval
```