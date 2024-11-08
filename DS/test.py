import time
import socket
import threading
lamporttime=0
def lamportclock():
    global lamporttime
    ls=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    ls.bind(("localhost",12346))
    print("server is running")
    
    while True:
        data,addr=ls.recvfrom(1024)
        if data:
            data=int(data.decode())
            data=max(lamporttime,data)+1
            ls.sendto(str(data).encode(),addr)

def ntpser():
    ns=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    ns.bind(("localhost",12347))
    print("NTP server is running ")
    
    while True:
        data,addr=ns.recvfrom(1024)
        if data:
            nt=int(time.time())+2208988800
            response=b'\x1b'+b'\0'*39 + nt.to_bytes(4,'big') + b'\0'*4
            ns.sendto(response,addr)
            
def runserver():
    t1=threading.Thread(target=lamportclock)
    t2=threading.Thread(target=ntpser)
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    
if __name__ == "__main__":
    runserver()
            