Dim wsh,ie 
Set wsh = CreateObject("wscript.shell") 
Set ie = WScript.CreateObject("InternetExplorer.Application") 
URL=" http://www.baidu.com"
code="�ű�֮��"
ie.visible = True
ie.navigate URL 
WScript.Sleep 1000 
wsh.AppActivate "�ٶ�һ�£����֪�� " ' �������������������ı��� 
wsh.Run "cmd.exe /c echo " & code & "| clip.exe", vbHide 
wsh.SendKeys "^v"
wsh.SendKeys "{ENTER}"