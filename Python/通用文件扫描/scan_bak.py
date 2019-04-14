#coding:utf-8
__author__ = 'mOon'
import requests

confFiles=['.php~','.php.txt','.php.old','.php_old','.php-old','.php.save','.php.swp','.php.swo','.php_bak','.php-bak','.php.original','.php.old','.php.orig','.php.bak','.save','.old','.bak','.orig','.original','.txt']

dc = ['.rar','.zip','.tar','.gz','.tar.gz','.bz2','.tar.bz2','.Z']


def file_line(filename):
    with open(filename) as f:
        line = f.readlines()
        f.close()
    return line



def scan_bak(site,line):
    rar=[]
    for i in line:
        url = i.split(".")[0].rstrip("\n")
        for j in confFiles:
            urls = site+url+j
            x = requests.get(urls)
            code =str(x.status_code)
            print urls +"    ----    "+code
            if "<?" in x:
                rar.append(urls)
                print urls +"    ----    "+code


    return rar



def cread_list(site,line):
    name_rz=[]
    for d in line:
        dd = d.rstrip("\n")
        path = dd.split("/")
        if len(path)==3:
            for s in dc:
                name_rz.append(dd+path[1]+str(s))
        elif len(path)==4:
            for s in dc:
                 name_rz.append(dd+path[2]+str(s))
            
          
       
    return name_rz

def scan_list(site,line):  
    urldata=[]      
    for x in  cread_list(site,line):
        urls = site+x
        
        req = requests.get(urls)
        code = str(req.status_code)
        print urls +"    ----    "+ code
        if req.status_code == 200:
            print urls +"    ----    "+ code
    return urldata



site="http://www.sagano-kanko.co.jp"

re_data =scan_bak(site,file_line("file.txt"))
re_list =scan_list(site,file_line("filepath.txt"))
moon_bak =re_data.extend(re_list)
if moon_bak:
    print u"存在文件备份文件"
    print moon_bak
else:
    print u"不存在备份文件"
