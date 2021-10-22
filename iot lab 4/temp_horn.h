//Edited by James Horn - Anything I didn't comment on I did not write.
#pragma once
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#define MAXTIMINGS	85
#define DHTPIN		7

bool read_dht11_dat(std::vector<int>& dht11_dat) //add outvector param
{
	if (dht11_dat.size() != 6) { //check container size
		printf("DHT-11 Error: Invalid container size\n");
		//failure
		return false;
	}

	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float	f;

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = dht11_dat[5] = 0; //initialize vector values to 0

	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHTPIN, INPUT);

	for (i = 0; i < MAXTIMINGS; i++)
	{
		counter = 0;
		while (digitalRead(DHTPIN) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
			{
				break;
			}
		}
		laststate = digitalRead(DHTPIN);

		if (counter == 255)
			break;

		if ((i >= 4) && (i % 2 == 0))
		{
			dht11_dat[j / 8] <<= 1;
			if (counter > 50) //changed from 16 to 50 as advised by a forum user
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}

	if ((j >= 40) &&
		(dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)))
	{
		f = dht11_dat[2] * 9. / 5. + 32;
		//store fahrenheit data
		dht11_dat[4] = f;
		dht11_dat[5] = (f - dht11_dat[4]) * 10;
		//success
		return true;
	}
	else {
		//set to zero to avoid misleading temp values
		dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = dht11_dat[5] = 0;
		//failure
		return false;
	}
}
