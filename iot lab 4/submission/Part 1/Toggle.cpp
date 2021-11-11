//Atlas C++ code by James Horn
//[Cannot be run outside atlas because there are no directives or functions.]
//button pin constant
const int TOGGLE = 4;

//gpio setup
wiringPiSetup();
pinMode(TOGGLE, INPUT);

//listen until button is pressed
while (true)
{
	//if button pressed
	if (digitalRead(TOGGLE) == HIGH)
	{
		//set status to the other mode
		//status is returned to python app by atlas
		if (current == 1) {
			status = 2;
			break;
		}
		else {
			status = 1;
			break;
		}
	}
}