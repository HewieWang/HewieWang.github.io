# 有用的Python代码

### 生成8位随机字符串

str = ''.join(random.sample(string.ascii_letters + string.digits, 8))

### 生成9位随机数

num = ''.join(str(i) for i in random.sample(range(0,9),9))

### selenium代理设置

chromeOptions = webdriver.ChromeOptions()<br>
chromeOptions.add_argument("--proxy-server=http://202.20.16.82:10152")
##### 一定要注意，=两边不能有空格，不能是这样--proxy-server = http://202.20.16.82:10152
browser = webdriver.Chrome(chrome_options = chromeOptions)