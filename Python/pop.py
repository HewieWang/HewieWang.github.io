from selenium import webdriver
driver = webdriver.Chrome()
driver.maximize_window()
driver.get('http://www.w3school.com.cn/tiy/loadtext.asp?f=hdom_alert')
driver.find_element_by_xpath('/html/body/input').click()
a1 =driver.switch_to.alert
a1.accept()
