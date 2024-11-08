import time
import socket

def lamptime():
    ls=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    localtime=0
    ls.sendto(str(localtime).encode(),('localhost',12346))
    data,_=ls.recvfrom(1024)
    data=int(data.decode())
    print("lamptimr" ,data)
    
if __name__ == "__main__":
    lamptime()