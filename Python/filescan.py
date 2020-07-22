import os,time,requests
from urllib.parse import urlparse,unquote
from bs4 import BeautifulSoup

headers = {
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
    'Accept-Encoding': 'gzip, deflate, br',
    'Accept-Language': 'zh-CN,zh;q=0.9,en;q=0.8',
    'Cache-Control': 'no-cache',
    'Connection': 'keep-alive',
    'Host': 'officecdn-microsoft-com.akamaized.net',
    'Pragma': 'no-cache',
    'Sec-Fetch-Mode': 'navigate',
    'Sec-Fetch-Site': 'none',
    'Sec-Fetch-User': '?1',
    'Upgrade-Insecure-Requests': '1',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36'
}

def get_file_name(url, headers):
    filename = ''
    if 'Content-Disposition' in headers and headers['Content-Disposition']:
        disposition_split = headers['Content-Disposition'].split(';')
        if len(disposition_split) > 1:
            if disposition_split[1].strip().lower().startswith('filename='):
                file_name = disposition_split[1].split('=')
                if len(file_name) > 1:
                    filename = unquote(file_name[1])
    if not filename and os.path.basename(url):
        filename = os.path.basename(url).split("?")[0]
    if not filename:
        return time.time()
    return filename
	
def bytes(bytes):
    if bytes < 1024:  #比特
        bytes = str(round(bytes, 2)) + ' B' #字节
    elif bytes >= 1024 and bytes < 1024 * 1024:
        bytes = str(round(bytes / 1024, 2)) + ' KB' #千字节
    elif bytes >= 1024 * 1024 and bytes < 1024 * 1024 * 1024:
        bytes = str(round(bytes / 1024 / 1024, 2)) + ' MB' #兆字节
    elif bytes >= 1024 * 1024 * 1024 and bytes < 1024 * 1024 * 1024 * 1024:
        bytes = str(round(bytes / 1024 / 1024 / 1024, 2)) + ' GB' #千兆字节
    elif bytes >= 1024 * 1024 * 1024 * 1024 and bytes < 1024 * 1024 * 1024 * 1024 * 1024:
        bytes = str(round(bytes / 1024 / 1024 / 1024 / 1024, 2)) + ' TB' #太字节
    elif bytes >= 1024 * 1024 * 1024 * 1024 * 1024 and bytes < 1024 * 1024 * 1024 * 1024 * 1024 * 1024:
        bytes = str(round(bytes / 1024 / 1024 / 1024 / 1024 / 1024, 2)) + ' PB' #拍字节
    elif bytes >= 1024 * 1024 * 1024 * 1024 * 1024 * 1024 and bytes < 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024:
        bytes = str(round(bytes / 1024 / 1024 / 1024 / 1024 / 1024 /1024, 2)) + ' EB' #艾字节
    return bytes

def start(url, thread_num):
    get_file = requests.get(url=url, headers=headers, stream=True, allow_redirects=False, timeout=10)
    content_length = bytes(int(get_file.headers['Content-Length']))
    file_name = get_file_name(url,get_file.headers)
    if int(get_file.headers['Content-Length'])<1024:
        print(url,"文件大小：", content_length, "文件不存在")
        pass
    else:
        print(url,"文件大小：", content_length, "名称：" + file_name)
        with open(r'status200.txt', 'a+') as f:
            f.write(url+'\n')
        pass

def get_str(s):
	suffixs=['rar','zip','tar.gz','sql']
	for suffix in suffixs:
		if "www." in urlparse(s).netloc:
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/'+urlparse(s).netloc+'.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/'+urlparse(s).netloc.replace("www.","")+'.'+suffix)
			pass
		else:
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/'+urlparse(s).netloc+'.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/www.'+urlparse(s).netloc+'.'+suffix)
			pass
		CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/www.'+suffix)
		CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/wwwroot.'+suffix)
		CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/web.'+suffix)
		CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/data.'+suffix)
		CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/bak.'+suffix)
		CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/backup.'+suffix)
		pass

def CheckFile(url):
	# print(url)
	try:
		r = requests.get(url,timeout=3)
		if r.status_code==200:
			soup = BeautifulSoup(r.text,"lxml")
			#print(soup.title)
			if soup.title!='<title>ç½ç«éç«å¢</title>':
				start(url, 20)
				#print(url)
				# with open(r'status200.txt', 'a+') as f:
				# 	f.write(url+'\n')
			pass
		print(url+"_____访问:"+str(r.status_code))
		pass
	
	except Exception as e:
		print(url+"_____访问:"+'timed out')
		pass



f=input("URL or TXT:")
if f.endswith(".txt"):
	for line in open(f):
		get_str(line)
		pass
	pass
else:
	get_str(f)

