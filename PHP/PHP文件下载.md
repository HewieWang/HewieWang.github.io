```PHP
$file="./Upload/575e163e8938f.pptx";
header("Content-type:application/octet-stream");//二进制文件
$fileName = basename($file);//获得文件名
header("Content-Disposition:attachment;filename=<?php echo ($fileName); ?>");//下载窗口中显示的文件名
header("Accept-ranges:bytes");//文件尺寸单位
header("Accept-length:".filesize($file));//文件大小
readfile($file);//读出文件内容
```