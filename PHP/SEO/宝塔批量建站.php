<?php
set_time_limit(0);
class bt_api {
  private $BT_KEY = "xxxxx";  //密钥
  private $BT_PANEL = "http://127.0.0.1:12345";     //地址

  public function __construct($bt_panel = null,$bt_key = null){
    if($bt_panel) $this->BT_PANEL = $bt_panel;
    if($bt_key) $this->BT_KEY = $bt_key;
  }
  public function addsite($txt){
    $urlreal = $this->BT_PANEL.'/site?action=AddSite';
    $p_data = $this->GetKeyData();
    $file = fopen($txt, "r");
    $s=file_get_contents($txt);
    $urls=array();
    $i=0;
    while(!feof($file)){$urls[$i]= fgets($file);$i++;}
    fclose($file);
    $url=array_filter($urls);
    foreach ($url as $k => $v) {
        $dmlist='"www.'.trim($v).'","*.'.trim($v).'"';
        $p_data['webname'] = '{"domain":"'.trim($v).'","domainlist":['.$dmlist.'],"count":3}';
        $p_data['path'] = '/www/wwwroot/'.trim($v);
        $p_data['type_id'] = '0';
        $p_data['type'] = 'PHP';
        $p_data['version'] = '54';
        $p_data['port'] = '80';
        $p_data['ps'] = trim($v);
        $p_data['ftp'] = 'false';
        $p_data['sql'] = 'false';
        $result = $this->HttpPostCookie($urlreal,$p_data);
        $data = json_decode($result,true);
        if($data['siteStatus']==true){
          $s2=str_replace(trim($v),'',$s);
          file_put_contents($txt,$s2);
        }
    }
    echo 'SUCCESS';
  }
  
    private function GetKeyData(){
      $now_time = time();
      $p_data = array(
      'request_token' =>  md5($now_time.''.md5($this->BT_KEY)),
      'request_time'  =>  $now_time
    );
      return $p_data;    
    }
    private function HttpPostCookie($url, $data,$timeout = 60)
    {
        $cookie_file='./'.md5($this->BT_PANEL).'.cookie';
        if(!file_exists($cookie_file)){
            $fp = fopen($cookie_file,'w+');
            fclose($fp);
        }
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
        curl_setopt($ch, CURLOPT_POST, 1);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
        curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file);
        curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
        $output = curl_exec($ch);
        curl_close($ch);
        return $output;
    }
}


$api = new bt_api();
$r_data = $api->addsite("url.txt");
echo json_encode($r_data);

?>
