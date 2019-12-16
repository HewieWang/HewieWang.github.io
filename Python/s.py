import sys,time
from selenium import webdriver

path = "chromedriver.exe"
options=webdriver.ChromeOptions()
prefs={
	'profile.default_content_setting_values': {
		'images': 2
	}
}
options.add_experimental_option('prefs',prefs)
b = webdriver.Chrome(executable_path=path,chrome_options=options)
b.set_window_size(250,480)
b.get('https://m.sogou.com/')
result=[]  
with open('word.txt','r') as f:  
    for line in f:
        b.find_element_by_xpath('//*[@id="keyword"]').send_keys(line)
        time.sleep(0.5)
        r=b.find_element_by_xpath('//*[@id="middleHint-1"]/div/div[1]/div').text
        with open('res.txt','a') as ra:
        	 ra.write(r)
        b.find_element_by_xpath('//*[@id="keyword"]').clear()
b.quit()