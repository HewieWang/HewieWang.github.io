<?php
set_time_limit(0);
ini_set('memory_limit','2048M');
session_start();
$_SESSION['kk']=0;
class bt_api {
  private $BT_KEY = "xxxxx";  //密钥
  private $BT_PANEL = "http://127.0.0.1:12345";     //地址

  public function __construct($bt_panel = null,$bt_key = null){
    if($bt_panel) $this->BT_PANEL = $bt_panel;
    if($bt_key) $this->BT_KEY = $bt_key;
  }
  public function changeWord($txt1,$txt2){
    $urlreal = $this->BT_PANEL.'/files?action=GetFileBody';
    $p_data = $this->GetKeyData();
    $file = fopen($txt1, "r");
    $s=file_get_contents($txt1);
    $urls=array();
    $i=0;
    while(!feof($file)){$urls[$i]= fgets($file);$i++;}
    fclose($file);
    $url=array_filter($urls);
    foreach ($url as $k => $v) {
        $arr = explode("\n", file_get_contents($txt2));
        $kk=$_SESSION['kk'];
        $path="/www/wwwroot/".trim($v)."/index.html";
        $p_data['path'] = $path;
        $result = $this->HttpPostCookie($urlreal,$p_data);
        $data = json_decode($result,true);
        if($data['status']==true){
            $s=str_replace('关键词1',trim($arr[$kk]),$data['data']);
            $s=str_replace('关键词2',trim($arr[$kk+1]),$s);
            $s=str_replace('关键词3',trim($arr[$kk+2]),$s);
            $endurl=$this->BT_PANEL.'/files?action=SaveFileBody';
            $p_data2 = $this->GetKeyData();
            $p_data2['data'] = $s;
            $p_data2['path'] = $path;
            $p_data2['encoding'] = 'utf-8';
            $result2 = $this->HttpPostCookie($endurl,$p_data2);
            $res2 = json_decode($result2,true);
            if($res2['status']==true){
                $_SESSION['kk']+=3;
                echo '1';
            }
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
$r_data = $api->changeWord("ym.txt","gjc.txt");
echo json_encode($r_data);

?>