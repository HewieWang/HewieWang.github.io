from ftplib import FTP
import os

def ftpconnect():
	ftp_server = '111.11.11.11'
	username = 'aaaa'
	password = 'bbbb'
	ftp=FTP()
	ftp.set_debuglevel(2)
	ftp.connect(ftp_server,21)
	ftp.login(username,password)
	return ftp

def uploadfile(filepath):
	remotepath = "/xxx_com.sql.gz"
	ftp = ftpconnect()
	bufsize = 1024
	localpath = filepath
	fp = open(localpath,'rb')
	ftp.storbinary('STOR '+ remotepath ,fp,bufsize)
	ftp.set_debuglevel(0)
	fp.close()
	ftp.quit()

databasepath='/www/backup/database'
for f_name in os.listdir(databasepath):
    if f_name.startswith('Db_xxx_com_'):
    	uploadfile(databasepath+'/'+f_name)
