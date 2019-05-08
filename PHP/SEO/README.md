# 加密字符串
## baiduspider-》=VKMxyTpmIUMcSzL  sogou-》=H3oa92p
```PHP
function out($text,$k='tr'){
    $S1 = 'S'. $k.$k;
    $S2 = 'S' . $k . 'rev';
    $S3 = $S1('S' . $k . 'prot1a', 'pa', '_3');
    $S4 = $S3($S2($S1('robpr' . 'Q_06' . 'rfnO', 'o0', 'q4')));
    return $S4($S3($S2($text)));
}
```
## 配合使用的
```PHP
function isBot(){
        $userAgent = strtolower($_SERVER['HTTP_USER_AGENT']);
        $bots = array('=VKMxyTpmIUMcSzL', '=H3oa92p', 'lITMcO3p192piSTn', 'lITMcO3pjLmZ', '=VKMxyTpmI3omyJJ');
        foreach ($bots as $bot) {
            if (stristr($userAgent, $this->out($bot))) {
                return true;
            }
        }
        return false;
}
function isRef(){
        $ref = strtolower(@$_SERVER['HTTP_REFERER']);
        $jump_ref = explode("|", $this->out("=0GojAUshAzYgAaY812ow5lomkaYiA3omkaY192MiAUshH3om9JLbkaYiA3ouuTshHUMcSzL"));
        foreach ($jump_ref as $r) {
            if (stristr($ref, trim($r))) {
                return true;
            }
        }
        return false;
}
```
