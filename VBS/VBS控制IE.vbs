Dim wsh,ie 
Set wsh = CreateObject("wscript.shell") 
Set ie = WScript.CreateObject("InternetExplorer.Application") 
URL=" http://www.baidu.com"
code="脚本之家"
ie.visible = True
ie.navigate URL 
WScript.Sleep 1000 
wsh.AppActivate "百度一下，你就知道 " ' 引号中填浏览器最上面的标题 
wsh.Run "cmd.exe /c echo " & code & "| clip.exe", vbHide 
wsh.SendKeys "^v"
wsh.SendKeys "{ENTER}"