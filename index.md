### 文件上传
	    <?php
	      if ((($_FILES["file"]["type"] == "image/gif")
	      || ($_FILES["file"]["type"] == "image/jpeg")
	      || ($_FILES["file"]["type"] == "image/pjpeg"))
	      && ($_FILES["file"]["size"] < 20000)){
		  if ($_FILES["file"]["error"] > 0){
		    echo "Return Code: " . $_FILES["file"]["error"] . "<br />";
		  }else{
		    if (file_exists("upload/" . $_FILES["file"]["name"])){
			echo $_FILES["file"]["name"] . " already exists. ";
		    }else{
			move_uploaded_file($_FILES["file"]["tmp_name"],
			"upload/" . $_FILES["file"]["name"]);
			echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
		    }
		  }
		}else{
		  echo "Invalid file";
		}
	    ?>


### 文件下载
	  <?php 
	      $file="./Upload/575e163e8938f.pptx";
	      header("Content-type:application/octet-stream");//二进制文件
	      $fileName = basename($file);//获得文件名
	      header("Content-Disposition:attachment;filename=<?php echo ($fileName); ?>");//下载窗口中显示的文件名
	      header("Accept-ranges:bytes");//文件尺寸单位  
	      header("Accept-length:".filesize($file));//文件大小  
	      readfile($file);//读出文件内容
	    ?>

### 正则
    <?php
      $preg= "/xuebingsi/";
      $c = preg_match($preg,"学并思网址xuebingsi.com");
      echo $c;
      ?>

### 判断电脑手机自动跳转
	<script type="text/javascript">try {var urlhash = window.location.hash;
	if (!urlhash.match("fromapp")){if ((navigator.userAgent.match(/(iPhone|iPod|Android|ios|iPad)/i)))
	{window.location="http://m.xx.com";}}}catch(err){}</script>
        

### PHP生成随机6位密码
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

### JS生成随机 N 位数字
	function randomNum(n){
		 var t=''; 
		 for(var i=0;i<n;i++){ 
		 t+=Math.floor(Math.random()*10); 
		 } 
		 return t; 
	}


