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