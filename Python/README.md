# Python

### 生成8位随机字符串
```Python
str = ''.join(random.sample(string.ascii_letters + string.digits, 8))
```
### 生成9位随机数
```Python
num = ''.join(str(i) for i in random.sample(range(0,9),9))
```
### Pyinstaller打包为EXE
```Python
pyinstaller -F -w demo.py
```
### selenium代理设置
```Python
chromeOptions = webdriver.ChromeOptions()<br>
chromeOptions.add_argument("--proxy-server=http://202.20.16.82:10152")
##### 一定要注意，=两边不能有空格，不能是这样--proxy-server = http://202.20.16.82:10152
browser = webdriver.Chrome(chrome_options = chromeOptions)
```
### centos 7 安装 pip
```Python
yum -y install epel-release
yum install python-pip
pip install --upgrade pip
```
### ChromeDirver下载地址
```Python
http://chromedriver.storage.googleapis.com/index.html
对应关系:https://sites.google.com/a/chromium.org/chromedriver/downloads
```