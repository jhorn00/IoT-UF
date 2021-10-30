#include <wiringPi.h>

//wiringPi pinouts for wiringPiSetup() [this would be different for wiringPiSetupGpio()]
#define TOGGLE 4 //green button toggle

//using main here for command line testing purposes - move it to a function to use elsewhere
int main() {
	wiringPiSetup();
	pinMode(TOGGLE, INPUT);
	while(true)
	{
		if(digitalRead(TOGGLE) == HIGH)
		{
			//tell python to change the mode
			//when it changes the mode it should also call this service again
			break;
		}
	}
	//done
	return 0;
}

