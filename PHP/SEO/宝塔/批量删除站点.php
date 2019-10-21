<?php
class bt_api {
  private $BT_KEY = "xxxxx";  //密钥
  private $BT_PANEL = "http://127.0.0.1:12345";     //地址

  public function __construct($bt_panel = null,$bt_key = null){
    if($bt_panel) $this->BT_PANEL = $bt_panel;
    if($bt_key) $this->BT_KEY = $bt_key;
  }

  public function DelSite($txt){
    $urlreal = $this->BT_PANEL.'/data?action=getData';
    $p_data = $this->GetKeyData();
    $file = fopen($txt, "r");
    $urls=array();
    $i=0;
    while(!feof($file)){$urls[$i]= fgets($file);$i++;}
    fclose($file);
    $url=array_filter($urls);
    foreach ($url as $k => $v) {
        $p_data['tojs'] = 'site.get_list';
        $p_data['table'] = 'sites';
        $p_data['limit'] = '15';
        $p_data['p'] = '1';
        $p_data['search'] = trim($v);
        $p_data['order'] = 'id desc';
        $p_data['type'] = '-1';
        $result = $this->HttpPostCookie($urlreal,$p_data);
        $data = json_decode($result,true);
        if($data){
            $id=str_replace("id=","",$data['where']);
            $delurl = $this->BT_PANEL.'/site?action=DeleteSite';
            $d_data = $this->GetKeyData();
            $d_data['id']=$id;
            $d_data['webname']=trim($v);
            $res = $this->HttpPostCookie($delurl,$d_data);
            $data2 = json_decode($res,true);
            //return $data;
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
$r_data = $api->DelSite("url.txt");
echo json_encode($r_data);

?>