import itertools,requests
http="http://"
suffix=".com"
for i in itertools.product('0123456789qwertyuiopasdfghjklzxcvbnm',repeat=5):
	url=http+str(i).replace("'","").replace(",","").replace("(","").replace(")","")+suffix
	url=''.join(url.split())
	#print(url)
	try:
		r = requests.get(url,timeout=3)
		if r.status_code==200:
			with open(r'domain_status200.txt', 'a+') as f:
				f.write(url+'\n')
			pass
		print(url+"_____访问:"+str(r.status_code))
		pass
	except Exception as e:
		print(url+"_____访问:"+'timed out')
		pass
