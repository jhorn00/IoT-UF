import socket
import led

UDP_IP = "10.192.134.107" # IP of the machine this is running on
UDP_PORT = 2598

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
	data, addr = sock.recvfrom(1024) # buffer size of 1024 bytes
	print("received message: ", data)
	if data == b"run":
		print("running script")
		led.flash()
