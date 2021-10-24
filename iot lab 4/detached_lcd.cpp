#include <thread>
#include <iostream>
#include "lcd_horn.h"
#include "temp_horn.h"

#define BTN_MODE 4//gpio 23
#define BTN_PWR	 5//gpio 24
#define LED_ACT  6//gpio 25

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
	std::string t = "";
	std::string antiFlicker = "                ";
	//added some history values to help with refresh flicker
	int h_hist[2] = {0, 0};
	int t_hist[3] = {0, 0, 0};
	//update indicator LED
	pinMode(LED_ACT, OUTPUT);
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
		//if shutdown initiated clear LCD and turn off backlight/LED
		if (shutdown)
		{
			lcd.ClrLcd(true);
			lcd.writeStringToLine("Shutting", LINE1);
			lcd.writeStringToLine("Down...", LINE2);
			delay(1500);
			lcd.ClrLcd(false);
			digitalWrite(LED_ACT, LOW);
			return;
		}
		//update strings if data has changed (any change would result in refresh, hence || check)
		if(h_hist[0] != data[0] || h_hist[1] != data[1])
		{
			//blink LED for updated info
			digitalWrite(LED_ACT, HIGH);
			h = "Humidity: " + std::to_string(data[0]) + "." + std::to_string(data[1]) + "%";
			h_hist[0] = data[0];
			h_hist[1] = data[1];
			lcd.writeStringToLine(antiFlicker, LINE1);
			lcd.writeStringToLine(h, LINE1);
			digitalWrite(LED_ACT, LOW);
		}
		if(standard)
		{
			if(t_hist[2] == (int)'C' || t_hist[0] != data[4] || t_hist[1] != data[5])
			{
				//blink LED for updated info
				digitalWrite(LED_ACT, HIGH);
				t = "Temp: " + std::to_string(data[4]) + "." + std::to_string(data[5]) + "F";
				t_hist[0] = data[4];
				t_hist[1] = data[5];
				t_hist[2] = (int)'F';
				lcd.writeStringToLine(antiFlicker, LINE2);
				lcd.writeStringToLine(t, LINE2);
				digitalWrite(LED_ACT, LOW);
			}
		}
		else
		{
			if(h_hist[2] == (int)'F' || t_hist[0] != data[2] || t_hist[1] != data[3])
			{
				//blink LED for updated info
				digitalWrite(LED_ACT, HIGH);
				t = "Temp: " + std::to_string(data[2]) + "." + std::to_string(data[3]) + "C";
				t_hist[0] = data[2];
				t_hist[1] = data[3];
				t_hist[2] = (int)'C';
				lcd.writeStringToLine(antiFlicker, LINE2);
				lcd.writeStringToLine(t, LINE2);
				digitalWrite(LED_ACT, LOW);
			}
		}
		//update at a rate of 120Hz
		delay(500);
	}
}

//thread to handle DHT-11
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
		//currently runs at rate of 120Hz
		delay(500);
	}
}

int main()
{
	//gpio setup
	wiringPiSetup();

	//handle lcd startup
	std::thread lcd_thread(lcd_loop);
	lcd_thread.detach();

	//handle DHT11 startup
	std::thread dht_thread(dht_loop);
	dht_thread.detach();

	//button setup
	pinMode(BTN_MODE, INPUT);
	pinMode(BTN_PWR, INPUT);

	//main loop
	std:: cout << "Red Button = Shutdown | Green Button = Temp Mode\n";
	while (true)
	{
		//if user hits red button initiate shutdown and wait 5 seconds to ensure shutdown of threads
		if (digitalRead(BTN_PWR) == HIGH)
		{
			std::cout << "Shutting down...\n";
			shutdown = true;
			delay(5000);
			break;
		}
		//if user hits green button change the temp mode
		else if (digitalRead(BTN_MODE) == HIGH)
		{
			std::cout << "Switching modes.\n";
			standard = !standard;
			delay(500);
		}
	}

	//no issues
	return 0;
}
