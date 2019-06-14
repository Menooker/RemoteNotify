import socket
import sys
import subprocess
import time
import datetime
last_msg_time = 0
def sendmessage(title, message):
    global last_msg_time
    now = time.time()
    if now - last_msg_time > 10 :
        subprocess.call(['notify-send',"{}".format(title),"{}".format(message)])
    last_msg_time = now
    return

HOST = ''	# Symbolic name, meaning all available interfaces
PORT = 8888	# Arbitrary non-privileged port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

#Bind socket to local host and port
try:
	s.bind((HOST, PORT))
except socket.error as msg:
	print('Bind failed. Error Code : ' + str(msg))
	sys.exit()
	
print('Socket bind complete')

#Start listening on socket
s.listen(10)
print('Socket now listening')

#now keep talking with the client
while True:
    conn, addr = s.accept()
    print(datetime.datetime.today(), 'Connected with ' + addr[0] + ':' + str(addr[1]))
    buf = conn.recv(1)
    sendmessage("Remote nofity", "Got a message")
    conn.close()
s.close()


