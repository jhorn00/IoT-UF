#include <wiringPi.h>

//wiringPi pinouts for wiringPiSetup() [this would be different for wiringPiSetupGpio()]
#define SPEAKER 21
#define LED 6

//using main here for command line testing purposes - move it to a function to use elsewhere
int main() {
	//setup and set to off
	wiringPiSetup();
	digitalWrite(SPEAKER, LOW);
	digitalWrite(LED, LOW);
	for (int i = 0; i < 4; i++) {
		digitalWrite(SPEAKER, HIGH);
		digitalWrite(LED, HIGH);
		delay(100);
		digitalWrite(SPEAKER, LOW);
		digitalWrite(LED, LOW);
		delay(200);
	}

	//done
	return 0;
}
