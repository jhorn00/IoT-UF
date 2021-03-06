import socket
from gpiozero import LED
from time import sleep

UDP_IP = "10.189.114.212" # IP of the machine this is running on
UDP_PORT = 2598 # Any port as long as they match

# Socket setup
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# LED setup
green = LED(12)

while True:
	data, addr = sock.recvfrom(1024) # buffer size of 1024 bytes
	print("received message: ", data)
	if data == b"run":
		print("running script")
		green.on()
		sleep(3)
		green.off()
