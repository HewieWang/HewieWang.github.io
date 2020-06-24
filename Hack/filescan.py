from urllib.parse import urlparse
import requests

def get_str(s):
	suffixs=['rar','zip','sql','tar.gz']
	for suffix in suffixs:
		if "www." in urlparse(s).netloc:
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/'+urlparse(s).netloc+'.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/'+urlparse(s).netloc.replace("www.","")+'.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/www.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/wwwroot.'+suffix)
			pass
		else:
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/'+urlparse(s).netloc+'.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/www.'+urlparse(s).netloc+'.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/www.'+suffix)
			CheckFile(urlparse(s).scheme+'://'+urlparse(s).netloc+'/wwwroot.'+suffix)
			pass
		pass

def CheckFile(url):
	# print(url)
	try:
		r = requests.get(url,timeout=3)
		if r.status_code==200:
			with open(r'status200.txt', 'a+') as f:
				f.write(url+'\n')
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
