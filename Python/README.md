# Python

### Python打包 nuitka 更优秀的打包库
```Python
python -m pip install nuitka    ###加速  pip install -i https://pypi.tuna.tsinghua.edu.cn/simple nuitka
python -m nuitka hello.py       ###完结，就这么牛逼，实测Pyinstaller打包出错，这个一下就OK，简直无敌
```
### 生成8位随机字符串
```Python
str = ''.join(random.sample(string.ascii_letters + string.digits, 8))
```
### 生成9位随机数
```Python
num = ''.join(str(i) for i in random.sample(range(0,9),9))
```
### 读取指定行
```Python
import linecache
file_path = r'D:\work\python\test.txt'
line_number = 5
def get_line_context(file_path, line_number):
	return linecache.getline(file_path, line_number).strip()
```
### WriteCSV
```Python
import csv
def write_csv(title,keyword):
    path  = "baidu.csv"
    with open(path,'a+') as f:
        csv_write = csv.writer(f)
        data_row = [title,keyword]
        csv_write.writerow(data_row)
```
### 现在的时间
```Python
time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))
```
### Tkinter
```Python
user = filedialog.askopenfilename()    ###打开文件,user返回文件路径
self.Text1.insert(INSERT,user)         ###向编辑器插入内容
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
### 用 cx_Freeze 将 Python 打包exe
```Python
import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
build_exe_options = {'packages': [], 'excludes': []}

setup(  name = '<程序名>',
        version = '<程序版本>',
        description = '<程序描述>',
        options = {'build_exe': build_exe_options},
        executables = [Executable('main.py')])
```
```Python
python setup.py build
```
### selenium指定drive,屏蔽图片
```Python
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
```
