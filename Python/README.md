# Python

### 生成8位随机字符串
```Python
str = ''.join(random.sample(string.ascii_letters + string.digits, 8))
```
### 生成9位随机数
```Python
num = ''.join(str(i) for i in random.sample(range(0,9),9))
```
### 现在的时间
```Python
time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))
```
### 生成随机姓名
```Python
import random
xing='赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许何吕施张孔曹严华金魏陶姜'
ming='豫章故郡洪都新府星分翼轸地接衡庐襟三江而带五湖'
X=random.choice(xing)
M="".join(random.choice(ming) for i in range(2))
print(X+M)
```
### Pyinstaller打包为EXE
```Python
pyinstaller -F -w demo.py
```
### selenium代理设置
```Python
chromeOptions = webdriver.ChromeOptions()<br>
chromeOptions.add_argument("--proxy-server=http://202.20.16.82:10152")
### 一定要注意，=两边不能有空格，不能是这样--proxy-server = http://202.20.16.82:10152
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