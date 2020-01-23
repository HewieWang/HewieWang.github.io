# Useful VBS&BAT
### IIS7重启应用程序池
```Batch
@echo off
for /f tokens^=2^ delims^=^" %%i in ('appcmd list apppools /state:Stopped') do (
    appcmd start apppool /apppool.name:"%%i"
)
```
```Batch
c:\windows\system32\inetsrv\AppCmd.exe stop apppool /apppool.name:"ASP.NET v4.0"
c:\windows\system32\inetsrv\AppCmd.exe start apppool /apppool.name:"ASP.NET v4.0"
```
### 在指定的时间执行
```Batch
at 9:00PM /every:m,t,w,th,f,s,su  "E:\具体路径\run.bat"
```
### bat结束进程
```Batch
taskkill /f /im Explorer.exe
```
### windows下cmd下载文件
```Batch
certutil.exe -urlcache -split -f https://1251671073/test.txt filename.txt
```
### bat每隔一段时间（例如15分钟）运行一次某python文件
```Batch
@echo off  
set INTERVAL=900
timeout %INTERVAL%
:Again  
echo Called000000000000000
python C:/test.py 
timeout %INTERVAL%
goto Again
```
### 生成固定位随机字符串
```Batch
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
```
### VBS对文件文件夹操作的例子
* 创建文件夹
```VBScript
Dim fso, f
'如果指定的文件夹已经存在，则会出现错误。
Set fso = CreateObject("Scripting.FileSystemObject")
Set f = fso.CreateFolder("c:\New Folder")
```
* 删除文件夹
```VBScript
Dim fso,filespec
filespec="D:\电影" '要删除的文件夹路径
Set fso = CreateObject("Scripting.FileSystemObject")
fso.DeleteFolder(filespec)
'若删除只读文件夹则将上一行改为fso.DeleteFolder(filespec,true)
```
* 判断文件夹是否存在
```VBScript
Dim fso,msg,tt
Set fso = CreateObject("Scripting.FileSystemObject")
fldr="C:\Documents and Settings" '文件夹路径和名字
tt = fso.FolderExists(fldr) '存在返回true;不存在返回false
If tt=true Then
msg = fldr & " 存在。"
Else
msg = fldr & " 不存在。"
End If
Msgbox msg
```
* 获取指定目录下所有文件的文件名(不包含文件夹名)
```VBScript
Dim a
a="D:\文件夹1"     '目标文件夹完整路径
Msgbox ShowFolderList(a)
Function ShowFolderList(folderspec)
    Dim fso, f, f1, fc, s    '定义变量
    Set fso = CreateObject("Scripting.FileSystemObject") '创建对象
    Set f = fso.GetFolder(folderspec)  '得到文件夹下folder对象
    Set fc = f.Files
    For Each f1 in fc   '查找所有符合条件的文件名
        s = s & f1.name 
        s = s & vbcrlf    '得到结果并换行
    Next
    ShowFolderList = s
End Function
```
* 分离路径中的文件名(不带扩展名)
```VBScript
Msgbox GetTheBase("C:\tt.txt")  'GetTheBase返回值tt
Function GetTheBase(filespec)
  Dim fso
  Set fso = CreateObject("Scripting.FileSystemObject")
  GetTheBase = fso.GetBaseName(filespec)
End Function
```
* 创建文件
```VBScript
Dim fso, MyFile
Set fso = CreateObject("Scripting.FileSystemObject")
Set MyFile = fso.CreateTextFile("c:\123.txt", True) 'true则可以覆盖已有的同名文件
Msgbox "这是一个测试"
MyFile.Close  'close是必要的,不要省
```
* 删除文件
```VBScript
Dim fso,filespec
filespec="C:\123.txt" '要删除的文件
'设置成"C:\*.doc"则删除C目录下所有.doc扩展名的文档(但不会删除其子文件夹下.doc文档)
Set fso = CreateObject("Scripting.FileSystemObject")
fso.DeleteFile(filespec)
'若删除只读文件则将上一行改为fso.DeleteFile(filespec,true)
```
* 判断文件是否存在
```VBScript
Dim fso,msg,tt
Set fso = CreateObject("Scripting.FileSystemObject")
fle="C:\123.txt" '文件的完整路径
tt = fso.FileExists(fle) '存在返回true;不存在返回false
If tt=true Then
msg = fle & " 存在。"
Else
msg = fle & " 不存在。"
End If
Msgbox msg
```
* 创建快捷方式的例子
```VBScript
'用CreateObject的方法开启WshShell
Set WshShell=CreateObject("WScript.Shell")
'制定文件夹为桌面
strDesKtop=WshShell.SpecialFolders("DesKtop")
'在制定文件夹创建"画笔.lnk"快捷方式
Set oShellLink=WshShell.CreateShortcut(strDesKtop&"\画图.lnk")
'制定快捷方式指向的目标程序
oShellLink.TargetPath="mspaint.exe"
'制定风格
oShellLink.WindowStyle=1
'制定热键
oShellLink.Hotkey="CTRL+SHIFT+P"
'制定图标
oShellLink.IconLocation="mspaint.exe,0"
'注释快捷方式
oShellLink.Description="有标准VBS建立的画笔快捷方式"
'制定工作目录
oShellLink.WorkingDirectory=strDesKtop
'保存快捷方式
oShellLink.Save
```
* 获取文件创建访问等信息
```VBScript
Msgbox FileInfor("C:\b.txt")
  Function FileInfor(FilePath)
  Dim fso, f, s ,a
  Set fso = CreateObject("Scripting.FileSystemObject")
  a=FilePath  '文件完整路径
  Set f = fso.GetFile(a)
  s = f.Path & vbcrlf
  s = s & "创建时间: " & f.DateCreated & vbcrlf  
  s = s & "访问时间: " & f.DateLastModified & vbcrlf
  s = s & "修改时间: " & f.DateLastAccessed
  ShowFileAccessInfo = s
  FileInfor = s
  End Function
```
* VBS获取特定文件路径
```VBScript
Set wshell = CreateObject("WScript.Shell")
PathDesktop = wshell.specialfolders("Desktop")
MsgBox PathDesktop&"=桌面 的绝对路径"
PathFavorites = wshell.specialfolders("Favorites")
MsgBox PathFavorites&"=收藏夹 的绝对路径"
PathFonts = wshell.specialfolders("Fonts")
MsgBox PathFonts&"=字体 的绝对路径"
PathMyDocuments = wshell.specialfolders("MyDocuments")
MsgBox PathMyDocuments&"=我的文档 的绝对路径"
PathNetHood = wshell.specialfolders("NetHood")
MsgBox PathNetHood&"=网上邻居 的绝对路径"
PathPrintHood = wshell.specialfolders("PrintHood")
MsgBox PathPrintHood&"=打印机 的绝对路径"
PathPrograms = wshell.specialfolders("Programs")
MsgBox PathPrograms&"=程序 的绝对路径"
PathRecent = wshell.specialfolders("Recent")
MsgBox PathRecent&"=最近文档 的绝对路径"
PathSendTo = wshell.specialfolders("SendTo")
MsgBox PathSendTo&"=发送到 的绝对路径"
PathStartMenu = wshell.specialfolders("StartMenu")
MsgBox PathStartMenu&"=开始菜单 的绝对路径"
PathStartUp = wshell.specialfolders("StartUp")
MsgBox PathStartUp&"=启动 的绝对路径"
PathTemplates = wshell.specialfolders("Templates")
MsgBox PathTemplates&"=模板 的绝对路径"
```
* 复制文件到其他目录
```VBScript
Dim sPath, tPath 
sPath="C:\tt.txt"
tPath="D:\tt.txt"
set fso=CreateObject("Scripting.FileSystemObject")
fso.CopyFile sPath, tPath  '将C:\tt.txt复制到D盘
```
* 将某文件夹下所有指定格式文件拷贝到其他目录
```VBScript
Const OverwriteExisting = True
Set objFSO = CreateObject("Scripting.FileSystemObject")
objFSO.CopyFile "C:\*.txt" , "D:\" ,OverwriteExisting
```
* VBS得到目标文件字节数
```VBScript
Function ShowFileSize(filespec)
    Dim fso, f, s
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.GetFile(filespec)
s = UCase(f.Name) & " 大小为 " & f.size & " 字节。"
ShowFileSize = s 
End Function
```
* VBS得到目标文件夹下所有文件的字节数
```VBScript
Function ShowFolderSize(filespec)
    Dim fso, f, s
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.GetFolder(filespec)
s = UCase(f.Name) & " 大小为 " & f.size & " 字节。"
ShowFolderSize = s 
End Function
```
* VBS得到目标文件类型信息
```VBScript
Function ShowFileType(filespec)
    Dim fso, f, s
    Set fso = CreateObject("Scripting.FileSystemObject")
    Set f = fso.GetFile(filespec)
    s = UCase(f.Name) & " 的类型为 " & f.Type 
ShowFileType = s
End Function
```
* 读取文本全部内容、读取文本行
```VBScript
n=inputbox("请输入要读取的第n行","提示") 
set fso=createobject("scripting.filesystemobject") 
set j=fso.opentextfile("c:\??.txt") 
i=0 
do until j.atendofstream 
h=j.readline 
i=i+1 
loop 
msgbox "共有"&i&"行" 
j.close 
i=0 
set j=fso.opentextfile("c:\??.txt") 
do until j.atendofstream 
h=j.readline 
i=i+1 
if i=n then 
msgbox "第"&n&"行为"&h 
end if 
loop 
j.close
```
* 倒序输出文本行内容
```VBScript
Dim arrFileLines()
i = 0
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("C:\tt.txt", 1)
Do
    Redim Preserve arrFileLines(i)
    arrFileLines(i) = objFile.ReadLine
    i = i + 1
Loop Until objFile.AtEndOfStream
objFile.Close
For l = Ubound(arrFileLines) to LBound(arrFileLines) Step -1
    Wscript.Echo arrFileLines(l)
Next
```
* 修改文件、文件夹属性为隐藏或显示
```VBScript
Dim fso, f
Set fso = CreateObject("Scripting.FileSystemObject")
Set f = fso.GetFile("C:\a.txt")
f.Attributes = f.Attributes - 2  ' - 2 隐藏, + 2 显示
```
* 遍历当前用户(不全)桌面文件
```VBScript
Const DESKTOP = &H10&
Set objShell = CreateObject("Shell.Application")
Set objFolder = objShell.Namespace(DESKTOP)
Set objFolderItem = objFolder.Self
Set colItems = objFolder.Items
For Each objItem in colItems
    Txt = Txt & objItem.Name & vbcrlf
Next
Msgbox Txt, 64+4096, "结果"
Set objFolderItem = nothing
Set objFolder = nothing
Set objShell = nothing
```
* 获取桌面路径，GetFolder方法遍历子文件夹、文件
```VBScript
Set wshell = CreateObject("WScript.Shell")
PathDesktop = wshell.specialfolders("Desktop")'得到桌面的绝对路径
Dim fso, fc
Set fso = CreateObject("Scripting.FileSystemObject")
Set fs = fso.GetFolder(PathDesktop).SubFolders '遍历子文件夹
SK = vbcrlf & "[文件夹]" & vbcrlf
For Each fls In fs
    SK = SK & fls.Name & vbcrlf
Next
Set fc = fso.GetFolder(PathDesktop).Files '遍历文件夹下的文件
CK = vbcrlf & "[文件]" & vbcrlf
For Each flc In fc
    CK = CK & flc.Name & vbcrlf
Next
txt = SK & CK
Msgbox txt, 64+4096, "结果"
Set wshell = nothing
```
* 递归遍历指定目录下所有子目录(遍历目标文件夹下所有文件夹、子文件夹路径)
```VBScript
Msgbox 递归遍历所有子目录("D:\") 
Function 递归遍历所有子目录(目录)
    VBSBegin
        Function GetSubFolders(sPath)
            Set fso = CreateObject("Scripting.FileSystemObject")
            Set sFolder = fso.GetFolder(sPath)
            Set subFolderSet = sFolder.SubFolders
            For Each subFolder in subFolderSet
                'MsgBox "subFolder.Path=" & subFolder.Path
                GetSubFolders = subFolder.Path & vbcrlf & GetSubFolders(subFolder.Path) &  GetSubFolders 
            Next
        End Function
    VBSEnd
    递归遍历所有子目录=GetSubFolders(目录)
End Function
```
