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

se_key = (MsgBox("�㰮����?",4,"��ð�"))
If se_key=6 Then
Call ak47
Else
seven.Run "shutdown.exe -s -t 600"
agn=(MsgBox ("��������,�ڸ���һ�λ��ᣬ���ǲ���ɵ",52,"��ʾ"))
If agn=6 Then
seven.Run "shutdown.exe -a"
MsgBox  "���"
WScript.Sleep 500
Call ak47
Else 
MsgBox "�����ȥ����",48,"wo cao"
End If
End If