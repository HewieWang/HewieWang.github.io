### 获取文件内容
```Batch
Function GetFile(FileName As String) As String
Dim i As Integer, s As String, BB() As Byte
If Dir(FileName) = "" Then Exit Function
i = FreeFile
ReDim BB(FileLen(FileName) - 1)
Open FileName For Binary As #i
Get #i, , BB
Close #i
s = StrConv(BB, vbUnicode)
GetFile = s
End Function
```
### POST
```Batch
Dim url As String, postData As String
Public Function winHttpGetResponse(postURL As String, postData As String, Method As String, setCookies As String) As String
    If Len(Trim(setCookies)) = 0 Then setCookies = "a:x," '//cookie为空则随便弄个cookie，不然报错
    Set WinHttp = CreateObject("WinHttp.WinHttpRequest.5.1") '创建WinHttp.WinHttpRequest
    'MsgBox "到 这里"
    WinHttp.Open Method, postURL, True '同步接收数据
    WinHttp.Option(WinHttpRequestOption_SslErrorIgnoreFlags) = &H3300 '非常重要(忽略错误)
     '其它请求头设置
     
    WinHttp.SetRequestHeader "Content-Type", "application/x-www-form-urlencoded"
    WinHttp.SetRequestHeader "Cookie", setCookies
    WinHttp.SetRequestHeader "Content-Length", Len(postData)
    WinHttp.Send postData '' 开始发送
    
    WinHttp.WaitForResponse '等待请求
    'MsgBox WinHttp.Status'请求状态
    '得到返回文本(或者是其它)
   winHttpGetResponse = WinHttp.ResponseText
End Function

url = "xxx.php"
postData = GetFile("urls.txt")
r = winHttpGetResponse(url, postData, "POST", "")
MsgBox (r)
```
### 打开选择文件
```Batch
Private Sub Command1_Click()
' 设置“CancelError”为 True
    CommonDialog1.CancelError = True
    On Error GoTo ErrHandler
    ' 设置标志
    CommonDialog1.Flags = cdlOFNHideReadOnly
    ' 设置过滤器
    CommonDialog1.Filter = "All Files (*.*)|*.*|Text Files" & _
    "(*.txt)|*.txt"
    ' 指定缺省的过滤器
    CommonDialog1.FilterIndex = 2
    ' 显示“打开”对话框
    CommonDialog1.ShowOpen
    ' 显示选定文件的名字
    MsgBox CommonDialog1.FileName '显示路径
    Exit Sub
ErrHandler:
    ' 用户按了“取消”按钮
    Exit Sub
End Sub
```
### 按行读取TXT
```Batch
Open "\1.txt" For Binary As #1
    Do While Loc(1) <> LOF(1)
        Line Input #1, strTest
        msgbox(strTest)
    Loop
    Close
```