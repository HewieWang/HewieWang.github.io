import socketserver
import threading
 
ADDRESS = ('127.0.0.1', 8712)  # 绑定地址
 
g_conn_pool = []  # 连接池
 
class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
 
    def setup(self):
        self.request.sendall("连接服务器成功!".encode(encoding='utf8'))
        # 加入连接池
        g_conn_pool.append(self.request)
 
    def handle(self):
        while True:
            try:
                bytes = self.request.recv(1024)
                print("客户端消息：", bytes.decode(encoding="utf8"))
            except:  # 意外掉线
                self.remove()
                break
 
    def finish(self):
        print("清除了这个客户端。")
 
    def remove(self):
        print("有一个客户端掉线了。")
        g_conn_pool.remove(self.request)
 
 
class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass
 
 
if __name__ == '__main__':
    server = ThreadedTCPServer(ADDRESS, ThreadedTCPRequestHandler)
    # 新开一个线程运行服务端
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
 
    # 主线程逻辑
    while True:
        cmd = input("""--------------------------
输入1:查看当前在线人数
输入2:给指定客户端发送消息
输入3:关闭服务端
""")
        if cmd == '1':
            print("--------------------------")
            print("当前在线人数：", len(g_conn_pool))
        elif cmd == '2':
            print("--------------------------")
            index, msg = input("请输入“索引,消息”的形式：").split(",")
            g_conn_pool[int(index)].sendall(msg.encode(encoding='utf8'))
        elif cmd == '3':
            server.shutdown()
            server.server_close()
            exit()