# 有用的VBS代码

### 生成固定位随机字符串

Dim StrA, i, Sh<br>
StrA = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZqwertyuiopasdfghjklzxcvbnm"<br>
Set Sh = CreateObject("WScript.Shell")<br>
Function MakeStr (L)<br>
Dim i<br>
MakeStr = ""<br>
For i = 0 To L<br>
Randomize<br>
MakeStr = MakeStr & Mid(StrA, Int(Rnd() * Len(StrA)) + 1, 1)<br>
Next<br>
End Function<br>

使用：(生成7位随机字符串)<br>

MakeStr(7)