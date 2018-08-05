# 有用的VBS代码

### 生成固定位随机字符串

Dim StrA, i, Sh
StrA = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZqwertyuiopasdfghjklzxcvbnm"
Set Sh = CreateObject("WScript.Shell")
Function MakeStr (L)
Dim i
MakeStr = ""
For i = 0 To L
Randomize
MakeStr = MakeStr & Mid(StrA, Int(Rnd() * Len(StrA)) + 1, 1)
Next
End Function

使用：(生成7位随机字符串)

MakeStr(7)