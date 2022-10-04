import socket
import random
import time
from socket import SOL_SOCKET, SO_RCVBUF

listenPort = 3310
socket.setdefaulttimeout(10)
localhost = '192.168.138.128'
robotIP='192.168.138.1'

s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s1.connect((robotIP,listenPort))
print("Connecting to robot")#connection part
data=b"1155127382"
s1.sendall(data)#sending SID part
print("Sending SID to robot")
port1=s1.recv(1024)#recv 5 ddddd part
print("Received message: %s\nNew connection will use port %s" % (port1.decode(),port1.decode()))


s_2= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s_2.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s_2.bind((localhost,int(port1.decode())))
s_2.listen(5)
print("s_2 listening robot's TCP connection")
#accepting the new TCP connection part
s2,address = s_2.accept()
print("Connection is received")
s_2.close()
###########################################################
def resent():# starting the experiment part

    testpercent=[1152, 2304, 4608, 9216, 18432, 36864,73728,147456]#set of buffer size
    i=0
    while i<8:
        
        
        s2.setsockopt(SOL_SOCKET,SO_RCVBUF,testpercent[i])
        i+=1
        Recv_buf=s2.getsockopt(SOL_SOCKET,SO_RCVBUF)
        print("s2 receive buffer is ",Recv_buf)
        message=("bs"+str(Recv_buf)).encode()#bs indicate the buffer
        s2.sendall(message)
        
        recvmessage=bytes()
        numbermessage=0

        total=0

        while True:
            recvmessage=s2.recv(1024)
            while recvmessage:
                
                recvmessage=s2.recv(1024)
                if recvmessage ==b"end":#end indicate the end of the exp
                    break
                numbermessage+=1
                total+=len(recvmessage)
                
            print( "All Data Received")
            break
        print("Number of received messages: %d, total received bytes: %d." %(numbermessage,total))
resent()


###########################################################
char12=s2.recv(1024)
char12=char12.decode()#receving 12 char
char12=char12.strip(".")
char12=char12.split(",")
print("New UDP port %s %s" % (char12[0],char12[1]))
robots3port=int(char12[0])

s3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s3.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s3.bind((localhost,int(char12[1])))
print("s3 is constructed")
num=random.randint(6,9)
num=str(num)
time.sleep(0.1)
s3.sendto(num.encode(),(robotIP,robots3port))#sending num
print("num is sent")

while True:
    num10,addr=s3.recvfrom(int(num)*10)
    print(num10)
    if num10:
        break
#only needs to receive any one of the strings

print("%s is received" % (num10))

for x in range(5):#Sendto num*10 char string 5 times
    
    s3.sendto(num10,(robotIP,robots3port))
    print("Sending num10 %d times" %(x+1))
    time.sleep(1)

print("The end of the world")


s1.close()
s2.close()
s3.close()
exit()




