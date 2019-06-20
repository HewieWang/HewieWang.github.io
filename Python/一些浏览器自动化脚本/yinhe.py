from selenium import webdriver
from selenium.webdriver.support.ui import Select
import random,string,time
while 1<2:
	browser = webdriver.Chrome()
	browser.get('https://www.yh765.com/views/html/register.html')
	username= ''.join(random.sample(string.ascii_letters + string.digits, 8))
	browser.find_element_by_xpath('//*[@id="account"]').send_keys(username)
	browser.find_element_by_xpath('//*[@id="passwd"]').send_keys("sisiss597")
	browser.find_element_by_xpath('//*[@id="confirmPassword"]').send_keys("sisiss597")
	xing='赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许何吕施张孔曹严华金魏陶姜'
	ming='豫章故郡洪都新府星分翼轸地接衡庐襟三江而带五湖'
	X=random.choice(xing)
	M="".join(random.choice(ming) for i in range(2))
	browser.find_element_by_xpath('//*[@id="fullName"]').send_keys(X+M)
	selectEle = browser.find_element_by_id('pwd1')
	Select(selectEle).select_by_value('1')
	selectEle = browser.find_element_by_id('pwd2')
	Select(selectEle).select_by_value('1')
	selectEle = browser.find_element_by_id('pwd3')
	Select(selectEle).select_by_value('1')
	selectEle = browser.find_element_by_id('pwd4')
	Select(selectEle).select_by_value('1')
	browser.find_element_by_xpath('//*[@id="OK2"]').click()
	time.sleep(1)
	browser.find_element_by_xpath('//*[@id="layui-layer1"]/div[3]/a[1]').click()
	time.sleep(2)
	browser.get('https://www.yh765.com/page/user-center/payment/recharge.html')
	time.sleep(2)
	browser.find_element_by_xpath('//*[@id="amount"]').send_keys(random.randint(100,9999))
	browser.find_element_by_xpath('//*[@id="form"]/div[4]/div/button').click()
	browser.find_element_by_xpath('//*[@id="tab-content"]/button[1]').click()
	time.sleep(3)
	browser.quit()
	pass