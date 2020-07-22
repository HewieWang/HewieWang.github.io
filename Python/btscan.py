import struct
import socket,requests,re

def findIPs(start, end):
    ipstruct = struct.Struct('>I')
    start, = ipstruct.unpack(socket.inet_aton(start))
    end, = ipstruct.unpack(socket.inet_aton(end))
    return [socket.inet_ntoa(ipstruct.pack(i)) for i in range(start, end+1)]

def get_title(html):
    title = ''
    p = '<title>.*?</title>'
    target = re.findall(p, html)
    if target:
        title = target[0]
        return title

for ip in findIPs('103.86.65.1', '103.86.65.255'):
    url='http://'+ip+':8888/'
    try:
        r = requests.get(url,timeout=3,verify=False)
        if r.status_code==200:
            if get_title(r.text)!='<title>安全入口校验失败</title>':
                print(url+'_'+get_title(r.text))
        pass
    except requests.exceptions.RequestException as e:
        pass
    