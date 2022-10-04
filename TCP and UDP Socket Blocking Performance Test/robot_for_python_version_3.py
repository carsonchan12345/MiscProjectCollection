# IERG3310 Project
# first version written by FENG, Shen Cody
# second version modified by YOU, Lizhao
# Third version modified by Jonathan Liang @ 2016.10.25

import socket
import random
import time

robotVersion = "3.0"
listenPort = 3310
socket.setdefaulttimeout(10)

localhost = '192.168.138.1'

print ("Robot version " + robotVersion + " started")
print ("You are reminded to check for the latest available version")

print ("")

# Create a TCP socket to listen connection
print ("Creating TCP socket...")
listenSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listenSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listenSocket.bind((localhost, listenPort))
listenSocket.listen(5)
print ("Done")

print ("\nTCP socket created, ready for listening and accepting connection...")
#print "Waiting for connection on port %(listenPort)s" % locals()
print ("Waiting for connection on port", listenPort)

# accept connections from outside, a new socket is constructed
s1, address = listenSocket.accept()
studentIP = address[0]
print ("\nClient from %s at port %d connected" %(studentIP,address[1]))
# Close the listen socket
# Usually you can use a loop to accept new connections
listenSocket.close()

data = s1.recv(10)
print ("Student ID received: " ),
print(data.decode())

iTCPPort2Connect = random.randint(0,9999) + 20000
print ("Requesting STUDENT to accept TCP <%d>..." %iTCPPort2Connect)

s1.send(str(iTCPPort2Connect).encode())
print ("Done")

time.sleep(1)
print ("\nConnecting to the STUDENT s1 <%d>..." %iTCPPort2Connect)
############################################################################# phase 1
# Connect to the server (student s2)
s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2.connect((studentIP,iTCPPort2Connect))

print("Done")
#############################################################
# experiment part
def resent():
    i=0
    while i<8:
        students2buf=s2.recv(1024)
        students2buf=students2buf.decode()
        students2buf=students2buf.strip("bs")
        print("Student buffer is ", students2buf)#receving buffer

        message=b"Testing"

        s2.setblocking(0)#set non-blocking mode
        time.sleep(2)
        timeout=time.time()+30#send for 30s
        print("Sending turn %d"% (i+1))# i set to have 8 different buffer size to try
        while True: 
            try:
                s2.sendall(message)
            except BlockingIOError:
                pass

            if time.time()>=timeout:
                break
        print("End this term")
        s2.setblocking(1)#set back to blocking mode to send the end message
        time.sleep(5)
        s2.sendall(b"end")
        i+=1
resent()

############################################################################# phase 2
# Send the ports required to STUDENT
iUDPPortRobot = random.randint(0,9999) + 20000
iUDPPortStudent = random.randint(0,9999) + 20000
print ("Sending the UDP information: to ROBOT: <%d>, to STUDENT: <%d>..." %(iUDPPortRobot,iUDPPortStudent))

s2.send((str(iUDPPortRobot)+","+str(iUDPPortStudent)+".").encode())
print ("Done")

############################################################################# phase 3
# Create a UDP socket to send and receive data
print ("Preparing to receive x...")
addr = (localhost, iUDPPortRobot)
s3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s3.bind(addr)

x, addr = s3.recvfrom(1)
print ("Get x = %d" % (int(x)))
############################################################################# phase 3

time.sleep(1)
print ("Sending UDP packets:")

messageToTransmit = ""
for i in range(0,int(x) * 2):
    messageToTransmit += str(random.randint(0,9999)).zfill(5)
print ("Message to transmit: " + messageToTransmit)

for i in range(0,5):
    s3.sendto(messageToTransmit.encode(),(studentIP,iUDPPortStudent))
    time.sleep(1)
    print ("UDP packet %d sent" %(i+1))
    
############################################################################# phase 4


print ("\nReceiving UDP packet:")
while True: # remove potentially duplicate msg
  data, addr = s3.recvfrom(int(x) * 10)
  print(data)
  if int(data) != int(x):
  	break

print ("Received: ", data)
print ("messageToTransmit: ", messageToTransmit)

if messageToTransmit == data.decode():
    print ("\nThe two strings are the same.")
else:
    print ("\nThe two strings are not the same.")

s1.close()
s2.close()
s3.close()
exit()
