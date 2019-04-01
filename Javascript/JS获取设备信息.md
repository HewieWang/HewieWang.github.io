# 获取访问者IP及所在地

```Html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
 <head>
  <meta http-equiv="content-type" content="text/html;charset=utf-8">
  <title>javascript获取访问者IP及所在地</title>
  <script src="http://pv.sohu.com/cityjson?ie=utf-8"></script>
 </head>

 <body>
  <script type="text/javascript"> document.write("IP: " + returnCitySN['cip'] + "<br>地区代码: " + returnCitySN['cid'] + "<br>所在地: " + returnCitySN['cname']); </script>
 </body>
</html>
```
## 输出：
```Html
IP: 61.140.62.124
地区代码: 440100
所在地: 广东省广州市
```

# 获取访问者设备信息

```Html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
 <head>
  <meta http-equiv="content-type" content="text/html;charset=utf-8">
  <title>javascript获取访问者设备信息</title>
 </head>
 <body>
  <script type="text/javascript">
    document.write("userAgent: " + navigator.userAgent + "<br><br>");
    document.write("appName: " + navigator.appName + "<br><br>");
    document.write("appCodeName: " + navigator.appCodeName + "<br><br>");
    document.write("appVersion: " + navigator.appVersion + "<br><br>");
    document.write("appMinorVersion: " + navigator.appMinorVersion + "<br><br>");
    document.write("platform: " + navigator.platform + "<br><br>");
    document.write("cookieEnabled: " + navigator.cookieEnabled + "<br><br>");
    document.write("onLine: " + navigator.onLine + "<br><br>");
    document.write("userLanguage: " + navigator.language + "<br><br>");
    document.write("mimeTypes.description: " + navigator.mimeTypes[1].description + "<br><br>");
    document.write("mimeTypes.type: " + navigator.mimeTypes[1].type + "<br><br>");
    document.write("plugins.description: " + navigator.plugins[3].description + "<br><br>");
  </script>
 </body>
</html>
```