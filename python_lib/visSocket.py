import os
import time
import socket
APP_SOCKET_PATH = "/tmp/visApp.sock"
ALGO_SOCKET_PATH = "/tmp/visAlgo.sock"

class VisSocketAlgo:
    def __init__(self, app_path, algo_path):
        self.app_sock_path = app_path
        self.algo_sock_path = algo_path
        self.algo_sock = None
        self.app_sock = None

    def init(self):
        try:
            self.algo_sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        except socket.error as msg:
            print(f"algo socket error: {msg}")
            return -1
        
        try:
            os.unlink(self.algo_sock_path)
        except OSError:
            if os.path.exists(self.algo_sock_path):
                raise
        
        try:
            self.algo_sock.bind(self.algo_sock_path)
            print("algo socket bound success")
        except socket.error as msg:
            print(f"algo socket bind error: {msg}")
            return -1
        
        try:
            self.algo_sock.listen(20)
        except socket.error as msg:
            print(f"listen error: {msg}")
            return -1
        
        try:
            self.app_sock, _ = self.algo_sock.accept()
            print("accept app socket successfully")
        except socket.error as msg:
            print(f"accept error: {msg}")
            return -1
        
        return 0

    def deliver(self, content:str):
        try:
            self.app_sock.sendall(content.encode())
        except socket.error as msg:
            print(f"send error: {msg}")
            return -1
        return 0


    def receive(self, length):
        try:
            data = self.app_sock.recv(length)
            if not data:
                print("EOF")
                return 1
            return data
        except socket.error as msg:
            print(f"receive content error: {msg}")
            return -1
    
    def close(self):
        self.app_sock.close()
        self.algo_sock.close()



if __name__ == "__main__":
    test_sock = VisSocketAlgo(APP_SOCKET_PATH, ALGO_SOCKET_PATH)
    test_sock.init()
    count = 0
    while 1:
        test_sock.deliver(str(f"test text {count}"))
        count += 1
        time.sleep(1)
    test_sock.close()