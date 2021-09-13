import socket
from datetime import datetime, date

def Main():
   
    host = '10.192.213.244' #Server ip
    port = 2598             # Any port as long as they match
    
    UDP_IP1 = "10.192.146.236" # Pi IP we send to
    UDP_IP2 = "10.192.148.250" # Pi IP we send to 
    UDP_IP3 = "10.192.131.56" # Cloud Device IP
    UDP_PORT = 2598

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind((host, port))

    print("Server Started")
    while True:
        data, addr = s.recvfrom(1024)
        data = data.decode('utf-8')
        print("From connected user: " + data)
        if float(data) < 30: 
            MESSAGE = b"run" 
            timestamp = "Activated: " + date.today().strftime("%m/%d/%y") + "   " + str(datetime.now().strftime("%H:%M:%S"))
            sock1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
            sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock1.sendto(MESSAGE, (UDP_IP1, UDP_PORT))
            sock2.sendto(MESSAGE, (UDP_IP2, UDP_PORT))
            sock3.sendto(timestamp.encode("utf-8"), (UDP_IP3, 4000))
        
    s.close()

if __name__=='__main__':
    Main()