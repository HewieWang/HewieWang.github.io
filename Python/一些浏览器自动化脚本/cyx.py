from selenium import webdriver
from PIL import Image,ImageEnhance
import os,time
import sys, hashlib, os, random, urllib
from urllib import request,parse
import json
import requests
from hashlib import md5
import win32api
import win32con
import string

# # 读取图片
class RClient(object):

    def __init__(self, username, password, soft_id, soft_key):
        self.username = username
        self.password = md5(password.encode("utf-8")).hexdigest()
        self.soft_id = soft_id
        self.soft_key = soft_key
        self.base_params = {
            'username': 'xxxxxxxxx',
            'password': 'xxxxxxxxx',
            'softid': '1',
            'softkey': 'xxxxxxxxx',
        }
        self.headers = {
            'Connection': 'Keep-Alive',
            'Expect': '100-continue',
            'User-Agent': 'ben',
        }

    def rk_create(self, im, im_type, timeout=60):
        """
        im: 图片字节
        im_type: 题目类型
        """
        params = {
            'typeid': '1040',
            'timeout': '60',
        }
        params.update(self.base_params)
        files = {'image': ('bdbutton.png', im)}
        r = requests.post('http://api.ruokuai.com/create.json', data=params, files=files, headers=self.headers)
        return r.json()

    def rk_report_error(self, im_id):
        """
        im_id:报错题目的ID
        """
        params = {
            'id': im_id,
        }
        params.update(self.base_params)
        r = requests.post('http://api.ruokuai.com/reporterror.json', data=params, headers=self.headers)
        return r.json()

n=1
while n<2:
    browser = webdriver.Chrome()
    browser.get('http://caitianxia453.com/caiTianXiaLoginWeb/app/joinNow?ref=6396ce')
    time.sleep(3)
    browser.find_element_by_xpath('//*[@id="bottomNavClose"]').click()
    users = ''.join(random.sample(string.ascii_letters + string.digits, 8))
    browser.find_element_by_xpath('//*[@id="txtUserName"]').send_keys(users)
    browser.find_element_by_xpath('//*[@id="txtPassword"]').send_keys('woyebuzh1')
    browser.find_element_by_xpath('//*[@id="txtConfirmPassword"]').send_keys('woyebuzh1')
    browser.find_element_by_xpath('//*[@id="txtFirstName"]').send_keys('陈真')
    browser.save_screenshot('bdbutton.png')
    location = browser.find_element_by_id('checkLoginCodeImage2').location  
    size = browser.find_element_by_id('checkLoginCodeImage2').size 
    left = location['x']  
    top =  location['y']  
    right = location['x'] + size['width']  
    bottom = location['y'] + size['height']
    im = Image.open('bdbutton.png')
    im = im.crop((left, top, right, bottom))
    im.save('bdbutton.png')
    rc = RClient('username', 'password', 'soft_id', 'soft_key')
    im = open('bdbutton.png', 'rb').read()
    co=rc.rk_create(im, 3040)
    code=co['Result']
    browser.find_element_by_xpath('//*[@id="txtValidationCode2"]').send_keys(code)
    browser.find_element_by_xpath('//*[@id="signUpForm"]/ul/li[8]/a[1]').click()
    browser.quit()
