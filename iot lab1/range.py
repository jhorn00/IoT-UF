import RPi.GPIO as GPIO
import time
import socket
GPIO.setmode(GPIO.BCM)
UDP_IP = "10.192.134.107"
UDP_PORT = 2598
MESSAGE = ""
TRIG = 23
ECHO = 24

print("Distance Measurement In Progress")

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

GPIO.output(TRIG, False)
print("Waiting For Sensor To Settle")
time.sleep(2)
while True:
	GPIO.output(TRIG, True)
	time.sleep(0.00001)
	GPIO.output(TRIG, False)

	while GPIO.input(ECHO)==0:
		pulse_start = time.time()

	while GPIO.input(ECHO)==1:
		pulse_end = time.time()

	pulse_duration = pulse_end - pulse_start
	distance = pulse_duration * 17150

	distance = round(distance, 2)

	print("Distance: ", distance, " cm")
	if distance < 30:
		MESSAGE = b"run"
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
		print("sent")
	time.sleep(1)

GPIO.cleanup()