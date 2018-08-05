from selenium import webdriver
import os,time
import sys,random
import string

n=1
while n<2:
    b = webdriver.Chrome()
    b.get('http://www-kgcp3.com/kuaiGouCPLoginWeb/app/joinNow?ref=da71b0')
    users = ''.join(random.sample(string.ascii_letters + string.digits, 8))
    b.find_element_by_xpath('//*[@id="txtUserName"]').send_keys(users)
    b.find_element_by_xpath('//*[@id="txtPassword"]').send_keys('woyebuzh1')
    b.find_element_by_xpath('//*[@id="txtConfirmPassword"]').send_keys('woyebuzh1')
    b.find_element_by_xpath('//*[@id="txtFirstName"]').send_keys('你好')
    j = 4
    qq = []
    qq = ''.join(str(i) for i in random.sample(range(0,9),j))
    b.find_element_by_xpath('//*[@id="qq"]').send_keys(qq)
    b.find_element_by_xpath('//*[@id="txtPhoneNum"]').send_keys('13222222548')
    b.find_element_by_xpath('//*[@id="signUpForm"]/ul/li[1]/a').click()
    time.sleep(3)
    b.quit()