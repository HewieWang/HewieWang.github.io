Dim wsh,ie,btn,con
Set ie = WScript.CreateObject("InternetExplorer.Application")
ie.Visible = true
ie.navigate "http://www-kgcp3.com/kuaiGouCPLoginWeb/app/joinNow?ref=da71b0"
 
Do
Wscript.Sleep 200
Loop Until ie.readyState = 4 

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

Dim StrA2, i2, Sh2
StrA2 = "1234567890"
Set Sh2 = CreateObject("WScript.Shell")
Function MakeQQ (L)
Dim i2
MakeQQ = ""
For i = 0 To L
Randomize
MakeQQ = MakeQQ & Mid(StrA2, Int(Rnd() * Len(StrA2)) + 1, 1)
Next
End Function

Set con=ie.Document.getElementById("txtUserName")
con.value=MakeStr(7)

Set con2=ie.Document.getElementById("txtPassword")
con2.value="hacker007"

Set con3=ie.Document.getElementById("txtConfirmPassword")
con3.value="hacker007"

Set con4=ie.Document.getElementById("txtFirstName")
con4.value="成龙"

Set con5=ie.Document.getElementById("qq")
con5.value=MakeQQ(8)

Set con4=ie.Document.getElementById("txtPhoneNum")
con4.value="13555556666"

ie.Document.getElementsByTagName("a")(63).click