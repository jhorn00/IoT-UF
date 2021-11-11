//Atlas C++ code by James Horn
//[Cannot be run outside atlas because there are no directives or functions.]
//constant for led pin
const int LED = 6;

//setup
wiringPiSetup();
pinMode(LED, OUTPUT);
//set to off (just in case)
digitalWrite(LED, LOW);

//trigger alarm 4 times
for (int i = 0; i < 4; i++) {
	//turn on led
	digitalWrite(LED, HIGH);

	//delay so it is noticeable to user
	delay(100);

	//turn off led
	digitalWrite(LED, LOW);

	//delay before next cycle
	delay(200);
}