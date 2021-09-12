from gpiozero import LED
from time import sleep

def flash():
	green = LED(12)

	green.on()
	sleep(3)
	green.off()

if __name__ == '__main__':
	flash()
