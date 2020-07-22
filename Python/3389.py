import socket,struct

def findIPs(start, end):
    ipstruct = struct.Struct('>I')
    start, = ipstruct.unpack(socket.inet_aton(start))
    end, = ipstruct.unpack(socket.inet_aton(end))
    return [socket.inet_ntoa(ipstruct.pack(i)) for i in range(start, end+1)]

def detect_port(ip,port):
    """检测ip上的端口是否开放
    """
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    try:
        s.connect((ip,int(port)))
        s.shutdown(2)
        print('{0}:{1} is open'.format(ip,port))
        return True
    except:
        print('{0}:{1} is close'.format(ip,port))
        #return False
        pass

if __name__ == '__main__':
    for ip in findIPs('103.100.104.1', '103.100.104.255'):
        port=3389
        detect_port(ip,port)
