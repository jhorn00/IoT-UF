import socket
from gpiozero import LED
from time import sleep

UDP_IP = "10.189.114.212" # IP of the machine this is running on
UDP_PORT = 2598 # Pick any port, just needs to match

#socket setup
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

#LED setup
red = LED(12)
red.on()

while True:
	data, addr = sock.recvfrom(1024) # buffer size of 1024 bytes
	print("received message: ", data)
	# Turn off the "off" light
	if data == b"run":
		print("running script")
		red.off()
		sleep(3)
		red.on()