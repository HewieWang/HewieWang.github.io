import random,requests,re
from lxml import etree
from pyquery import PyQuery as pq
"""
定义头信息
"""
headers = {
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
    'Accept-Encoding': 'gzip, deflate, br',
    'Accept-Language': 'zh-CN,zh;q=0.9',
    'Cache-Control': 'no-cache',
    'Connection': 'keep-alive',
    'Host': 'www.sogou.com',
    'Pragma': 'no-cache',
    'Upgrade-Insecure-Requests': '1',
    'User-Agent': 'User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36',
}
orderList = []          # 关键词排名结果
domain = 'hug-code.cn'  # 设置自己的域名

"""
获取cookie信息
"""
def getOneCookie():
    cookies = {}
    from selenium import webdriver
    from selenium.webdriver.chrome.options import Options
    chrome_options = Options()
    chrome_options.add_argument('--headless')
    driver = webdriver.Chrome(options=chrome_options)
    #wait = ui.WebDriverWait(driver, 2)
    driver.maximize_window()
    url = "https://www.sogou.com/web?query=%s&page=1" % random.choice('abcdefghijklmnopqrstuvwxyz')
    driver.get(url)
    cookie = driver.get_cookies()
    for i in cookie:
        cookies[i['name']] = i['value']
    return cookies

"""
请求链接获取页面内容
url 请求链接
"""
def getHtml(url):
    global cookies
    requests.packages.urllib3.disable_warnings()
    resp = requests.get(url, headers=headers, cookies=cookies, allow_redirects=False, timeout=10)
    print(resp.status_code,resp.url)
    path = etree.HTML(resp.content)
    html = etree.tostring(path)
    return pq(html)

"""
正则匹配字符串
matching -> 正则匹配规则
str -> 匹配字符串
"""
def matchingString(matching, str):
    pattern = re.compile(matching, re.S)
    result = re.findall(pattern, str)
    return result and result[0] or ''

"""
获取关键词在搜狗的排名
word 关键词
"""
def getSouGouPcRanking(word):
    global getWordNumber,cookies
    try:
        if (getWordNumber - 1) % 50 == 0:
            cookies = getOneCookie()
        html = getHtml('https://www.sogou.com/web?query=' + str(word) + '&page=1')
        all_list = html('.results h3 a').items()
        order = 1
        for i in all_list:  # 添加线程
            href = i.attr('href')
            result = requests.get('https://www.sogou.com' + str(href), headers=headers, cookies=cookies, allow_redirects=False, timeout=10)
            trueUrl = matchingString('replace\(\"(.*)?\"\)', str(result.text))
            if(trueUrl.find(domain) > 0):  # 判断是不是需要得链接
                orderList.append({word:{'order': order, 'url': trueUrl}})
            order = order + 1
    except Exception as e:
        print(e)

wordList = ['sql', 'shell', 'word3']
getWordNumber = 1
cookies = getOneCookie()
for word in wordList:
    getSouGouPcRanking(word)
    getWordNumber = getWordNumber + 1

print(orderList)