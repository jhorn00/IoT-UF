import socket

def Main():
   
    host = '10.192.194.199' #Server ip
    port = 4000

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind((host, port))

    print("Server Started")
    while True:
        data, addr = s.recvfrom(1024)
        data = data.decode('utf-8')
        print("Message from: " + str(addr))
        # print("From connected user: " + data)
        # data = data.upper()
        # print("Sending: " + data)
        # s.sendto(data.encode('utf-8'), addr)
        file_object = open('cloud.txt', 'a')
        file_object.write(str(data))
        file_object.write("\n")
        file_object.close()
    c.close()

if __name__=='__main__':
    Main()