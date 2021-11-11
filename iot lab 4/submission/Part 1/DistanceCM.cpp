//Atlas C++ code by James Horn
//[Cannot be run outside atlas because there are no directives or functions.]
//constants for distance sensor pins
const int trigger = 23;
const int echo = 24;

//setup variables and pins
unsigned int start = 0, end = 0, ttime = 0;
wiringPiSetup();
pinMode(trigger, OUTPUT);
pinMode(echo, INPUT);
digitalWrite(trigger, LOW);
delay(500);

//get distance
//send out the wave
digitalWrite(trigger, HIGH);
delayMicroseconds(10);
digitalWrite(trigger, LOW);

//listen
while (digitalRead(echo) == LOW) { start = micros(); }
while (digitalRead(echo) == HIGH) { end = micros(); }

//calculate distance
ttime = end - start;
double distanceCM = 100.0 * (((double)ttime / 1000000.0) * 340.29) / 2.0;

//return CM
DistanceInCM = (int)distanceCM;