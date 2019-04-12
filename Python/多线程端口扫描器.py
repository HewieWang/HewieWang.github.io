#coding: utf-8
# author: charlotte
import socket,sys,re,time
import threading
import argparse

# 定义变量
lock = threading.Lock()
num = 0
host = ''
begin_port = 1
end_port = 65535
mutex = threading.Lock()
thread_num = 0

def scan(host,port):
    global num
    try:

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(10)
        s.connect((host,port))
        print ' [+] port %5s open' %(port)
        s.close()
        num += 1

    except:
        pass

class threads(threading.Thread):
    def __init__(self,ip):
        threading.Thread.__init__(self)
        self.ip = ip

    def run(self):
        global begin_port,end_port,mutex
        while True:
            mutex.acquire()
            begin_port += 1
            if begin_port > end_port:
                mutex.release()
                break
            mutex.release()
            scan(self.ip,begin_port)

#判断是ip还是domain
def ip_match(h):
    global host
    pattern = '^([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])$'
    m = re.search(pattern,h)
    if m:
        host = h
        print ' [+] ip is %s ' %(host)
    else:
        host = socket.gethostbyname(h)
        print ' [+] ip is %s ' %(host)

# def usage():
#     usage = 'Usage: python scan.py ip/domain [ -t thread_num ]'
#     parser = argparse.ArgumentParser()
#     parser.add_argument("-t",help = "set the num of thread",type = int)
#     parser.add_argument("--ip",help = "ip")
#     args = parser.parse_args()


def main():
    thread_list = []
    #begin_host = raw_input("input ip : ")
    begin_host = sys.argv[1]
    ip_match(begin_host)
    print ' [+] begin  scan  ...'
    t = time.time()

    for i in range(200):
        thread = threads(host)
        thread.start()
        thread_list.append(thread)

    for thread in thread_list:
        thread.join()
    print ' [+] scan   end...'
    print 'cost %s s for scan' % (time.time() - t)




if __name__ == '__main__':
    try:
        main()
    except:
        print 'error...'
        exit()
