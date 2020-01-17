import sys,time,requests
from selenium import webdriver

path = "chromedriver.exe"
options=webdriver.ChromeOptions()
prefs={
	'profile.default_content_setting_values': {
		'images': 2
	}
}
options.add_experimental_option('prefs',prefs)
options.add_argument('--headless')
b = webdriver.Chrome(executable_path=path,options=options)

def getsogou(word):
    #b.set_window_size(200,300)
    b.get('https://wap.sogou.com/web/searchList.jsp?keyword='+word+'&prs=8&rfh=2')
    time.sleep(0.5)
    return b.page_source

with open('word.txt','r') as f:  
    for line in f:
        getsogou(line)
        r=b.find_element_by_xpath('//*[@id="middleHint-1"]/div/div[1]/div').text
        with open('res.txt','a') as ra:
            ra.write(r+'\n')
        pass
b.quit()