Set Seven = WScript.CreateObject("WScript.Shell")
strDesktop = Seven.SpecialFolders("AllUsersDesktop") 
set oShellLink = Seven.CreateShortcut(strDesktop & "\Seven.url")
oShellLink.Save
Sub ak47
Set oShellLink=Nothing
seven.Run "notepad",3
WScript.Sleep 500
seven.SendKeys "       I  "
WScript.Sleep 500
seven.SendKeys "L"
WScript.Sleep 500
seven.SendKeys "O"
WScript.Sleep 500
seven.SendKeys "V"
WScript.Sleep 500
seven.SendKeys "E   "
WScript.Sleep 500
seven.SendKeys "Y"
WScript.Sleep 500
seven.SendKeys "O"
WScript.Sleep 500
seven.SendKeys "U  TOO"
WScript.Sleep 500
seven.SendKeys "{ }"
End Sub

se_key = (MsgBox("你爱我吗?",4,"你好啊"))
If se_key=6 Then
Call ak47
Else
seven.Run "shutdown.exe -s -t 600"
agn=(MsgBox ("你死定了,在给你一次机会，你是不是傻",52,"提示"))
If agn=6 Then
seven.Run "shutdown.exe -a"
MsgBox  "真乖"
WScript.Sleep 500
Call ak47
Else 
MsgBox "你可以去死了",48,"wo cao"
End If
End If