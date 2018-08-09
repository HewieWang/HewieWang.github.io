<?php 

/**
 * PHP导出CSV
 * @author   Hannibal
 */

ini_set('memory_limit','1024M');
set_time_limit(0);

$mysqli = new mysqli('localhost', 'root', 'root', 'xxx');

if (mysqli_connect_errno()){
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}


$sql="select * from user";

$res=$mysqli->query($sql);

header('Content-Type: application/vnd.ms-excel;charset=gbk');
header('Content-Disposition: attachment;filename="CVS文件导出.csv"');
header('Cache-Control: max-age=0');
$fp = fopen('php://output', 'a');

$head = array(
	0 => '用户名',
	1 => '手机号'
);

foreach ($head as $i => $v){
	$head[$i] = iconv('utf-8', 'gbk', $v);
}
fputcsv($fp, $head);

if($res){
	while($row=mysqli_fetch_array($res,MYSQLI_ASSOC)){
		// 导出CSV会丢失前导0,所以添加#
	 	$row['u_user']='#'.$row['u_user'];
	    fputcsv($fp, $row);
	  }
	}else{
	  die("fetch data failed!");
	}

//释放游标内存
mysqli_free_result($res); 
//关闭数据库连接
mysqli_close($mysqli);

?>