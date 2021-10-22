#include <thread>
#include <iostream>
#include "lcd_horn.h"
#include "temp_horn.h"

volatile int data[6] = {0, 0, 0, 0, 0, 0};
volatile bool shutdown = false;
volatile bool standard = false;

void lcd_loop()
{
	//create lcd object
	LCD_Thing lcd;
	//display startup message, notifying user something is happening
	lcd.writeStringToLine("Starting...", LINE1);
	//initialize strings
	std::string h = "";
	std::string t_c = "";
	std::string t_f = "";
	//give the dht some time to grab good temp data
	bool ready = false;
	while(!ready)
	{
		for(int i = 0; i < 6; i++)
		{
			//if any of the data points != 0 we know the DHT found usable data
			if(data[i] != 0)
				ready = true;
		}
	}
	while (true)
	{
		lcd.ClrLcd(true);
		//if shutdown initiated clear LCD and turn off backlight
		if (shutdown)
		{
			lcd.writeStringToLine("Shutting", LINE1);
			lcd.writeStringToLine("Down...", LINE2);
			delay(1500);
			lcd.ClrLcd(false);
			return;
		}
		//update strings after each display cycle
		h = "Humidity: " + std::to_string(data[0]) + "." + std::to_string(data[1]) + "%";
		t_c = "Temp: " + std::to_string(data[2]) + "." + std::to_string(data[3]) + "C";
		t_f = "Temp: " + std::to_string(data[4]) + "." + std::to_string(data[5]) + "F";
		lcd.writeStringToLine(h, LINE1);
		if(standard)
			lcd.writeStringToLine(t_f, LINE2);
		else
			lcd.writeStringToLine(t_c, LINE2);
		//update at a rate of 30Hz
		delay(500);
	}
}

void dht_loop()
{
	std::vector<int> v = {0, 0, 0, 0, 0, 0};
	while (true)
	{
		//if shutdown initiated return
		if (shutdown)
		{
			return;
		}
		if (read_dht11_dat(v))
		{
			for (int i = 0; i < 6; i++)
			{
				data[i] = v[i];
			}
		}
		delay(500);
	}
}

int main()
{
	//handle lcd startup
	std::thread lcd_thread(lcd_loop);
	lcd_thread.detach();

	//handle DHT11 startup
	wiringPiSetup();
	std::thread dht_thread(dht_loop);
	dht_thread.detach();

	//main loop
	char c;
	while (true)
	{
		std::cout << "Enter q to quit:\n";
		std::cin >> c;
		//if user enters q initiate shutdown and wait 5 seconds to ensure shutdown of threads
		if (c == 'q')
		{
			shutdown = true;
			delay(5000);
			break;
		}
		else if (c == 's')
		{
			standard = !standard;
		}
	}

	//no issues
	return 0;
}
