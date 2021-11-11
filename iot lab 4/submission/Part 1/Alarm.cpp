//Atlas C++ code by James Horn
//[Cannot be run outside atlas because there are no directives or functions.]
//constants for speaker and led pins
const int SPEAKER = 21;
const int LED = 6;

//gpio setup
wiringPiSetup();
pinMode(SPEAKER, OUTPUT);
pinMode(LED, OUTPUT);

//initialize to off (just in case)
digitalWrite(SPEAKER, LOW);
digitalWrite(LED, LOW);

//trigger alarm 4 times
for (int i = 0; i < 4; i++) {
	//turn led and speaker on at the same time
	digitalWrite(SPEAKER, HIGH);
	digitalWrite(LED, HIGH);

	//wait so they are noticeable to the user
	delay(100);

	//turn off speaker and led
	digitalWrite(SPEAKER, LOW);
	digitalWrite(LED, LOW);

	//delay before next cycle
	delay(200);
}