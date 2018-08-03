'On Error Resume Next
Dim fso,ws,temp,file,op,words,wri,textfile,folder
Set fso=Wscript.CreateObject("Scripting.FileSystemObject")
Set ws=Wscript.CreateObject("Wscript.Shell")
'Create objects
temp=ws.ExpandEnvironmentStrings("%Temp%")
'Get system temp folder
file=temp & "\DirDriveAllFiles.dat"
inputfolderpath
Sub inputfolderpath()
folder=InputBox("请输入文件夹全路径：","输入路径","")
if folder="" Then
Wscript.Quit
End if
if fso.FolderExists(folder)<>True Then
MsgBox "文件夹不存在，请重新输入！",0+16,"错误"
inputfolderpath
End if
End Sub
'foler get
inputtextfile
Sub inputtextfile()
textfile=InputBox("请输入文本文档的创建位置：","输入路径","")
if textfile="" Then
inputfolderpath
End if
if fso.FileExists(textfile & ".txt") Then
MsgBox "文件已存在，请换个名称！",0+16,"错误"
inputtextfile
End if
End Sub
'Get folder
if fso.FileExists(file) Then
fso.DeleteFile(file)
End if
'Delete file
ws.run "cmd.exe /c dir /s /b "+Chr(34) & folder & Chr(34)+">>"+Chr(34) & file & Chr(34)+" & echo /////This is End///////>>"+Chr(34) & file & Chr(34)+" & attrib +h "+Chr(34) & file & Chr(34),0
Do Until fso.FileExists(file)
Loop
'Wait file write over
Set op=fso.OpenTextFile(file,1)
'Open this file
MsgBox textfile
Set wri=fso.CreateTextFile(textfile & ".txt",True)
Do Until words="/////This is End/////// "
words=op.ReadLine
if words<>"" And words<>"/////This is End/////// " Then
wri.WriteLine(Mid(words,InStrRev(words,"\",-1)+1,Len(words)))
End if
Loop
wri.Close
Wscript.Quit
