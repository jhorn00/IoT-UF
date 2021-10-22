#include <chrono>
#include <thread>
#include <string>
#include <cpprest/http_msg.h>
#include <wiringPi.h>
#include "metronome.hpp"
#include "rest.hpp"

using namespace std;

// ** Remember to update these numbers to your personal setup. **
#define LED_RED   17
#define LED_GREEN 27
#define BTN_MODE  23
#define BTN_TAP   24

// Mark as volatile to ensure it works multi-threaded.
volatile bool btn_mode_pressed = false;
volatile bool btn_tap_pressed = false;
volatile int currentBPM = 0;
volatile int currentMAX = 0;
volatile int currentMIN = 0;
volatile bool on = false;
volatile bool first = true;

metronome m;


void getMAX(web::http::http_request msg)
{
	web::http::http_response response(200);
        response.headers().add("Access-Control-Allow-Origin", "*");
        response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
        response.headers().add("Access-Control-Allow-Headers", "Content-Type");
        response.set_body(web::json::value::number(currentMAX));
        msg.reply(response);
}


void getMIN(web::http::http_request msg)
{
	web::http::http_response response(200);
        response.headers().add("Access-Control-Allow-Origin", "*");
        response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
        response.headers().add("Access-Control-Allow-Headers", "Content-Type");
        response.set_body(web::json::value::number(currentMIN));
        msg.reply(response);
}

void getBPM(web::http::http_request msg)
{
	web::http::http_response response(200);
        response.headers().add("Access-Control-Allow-Origin", "*");
        response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
        response.headers().add("Access-Control-Allow-Headers", "Content-Type");
        response.set_body(web::json::value::number(currentBPM));
        msg.reply(response);
}

void setBPM(web::http::http_request msg)
{
	web::http::http_response response(200);
	response.headers().add("Access-Control-Allow-Origin", "*");
	response.headers().add("Access-Control-Allow-Origin", "GET, PUT, DELETE");
	response.headers().add("Access-Control-Allow-Origin", "Content-Type");
	string str = msg.extract_string().get();
	cout << str << endl;
	str.erase(remove( str.begin(), str.end(), '"' ),str.end());
	try
	{
		currentBPM = stoi(str);
		if(currentBPM == 0)
		{
			m.set_rate(0);
			on = false;
			digitalWrite(LED_GREEN, LOW);
		}
		else
		{
			m.set_rate(60000/currentBPM);
		}
		if(first)
		{
			currentMIN = currentBPM;
			currentMAX = currentBPM;
			first = false;
		}
		else
		{
			if(currentBPM < currentMIN)
				currentMIN = currentBPM;
			if(currentBPM > currentMAX)
				currentMAX = currentBPM;
		}
		on = true;
	}
	catch(...)
	{
		cout << "STOI FAILED" << endl;
	}
	msg.reply(response);
}

void deleteMAX(web::http::http_request msg)
{
	web::http::http_response response(200);
	response.headers().add("Access-Control-Allow-Origin", "*");
	response.headers().add("Access-Control-Allow-Origin", "GET, PUT, DELETE");
	response.headers().add("Access-Control-Allow-Origin", "Content-Type");
	currentMAX = 0;
	msg.reply(response);
	first = true;
}

void deleteMIN(web::http::http_request msg)
{
	web::http::http_response response(200);
	response.headers().add("Access-Control-Allow-Origin", "*");
	response.headers().add("Access-Control-Allow-Origin", "GET, PUT, DELETE");
	response.headers().add("Access-Control-Allow-Origin", "Content-Type");
	currentMIN = 0;
	msg.reply(response);
	first = true;
}


// Run an additional loop separate from the main one.
void blink() {
	while (true)
	{
		if(on && m.get_rate().count() > 0)
		{
			digitalWrite(LED_GREEN, HIGH);
			this_thread::sleep_for(m.get_rate());
			//write LED as high if on and low if off
			digitalWrite(LED_GREEN, LOW);
			this_thread::sleep_for(m.get_rate());
		}
		if(m.is_timing())
		{
			on = false;
			digitalWrite(LED_GREEN, LOW);
			this_thread::sleep_for(500ms);
		}
		else
		{
			on = true;
		}
	}
}

int main() {
	cout << "Listening to buttons..." << endl;
	bool learnMode = false;
	wiringPiSetupGpio();
	//set up I/O
	pinMode(LED_RED, OUTPUT);
	pinMode(BTN_MODE, INPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(BTN_TAP, INPUT);

	//blink on separate thread
	thread blink_thread(blink);
	blink_thread.detach();

	// Configure the rest services after setting up the pins,
	// but before we start using them.
	// ** You should replace these with the BPM endpoints. **
	auto getBPM_rest = rest::make_endpoint("/bpm");
	getBPM_rest.support(web::http::methods::GET, getBPM);
	getBPM_rest.support(web::http::methods::PUT, setBPM);

	auto getMAX_rest = rest::make_endpoint("/bpm/max");
	getMAX_rest.support(web::http::methods::GET, getMAX);
	getMAX_rest.support(web::http::methods::DEL, deleteMAX);

	auto getMIN_rest = rest::make_endpoint("/bpm/min");
	getMIN_rest.support(web::http::methods::GET, getMIN);
	getMIN_rest.support(web::http::methods::DEL, deleteMIN);

//	auto setBPM_rest = rest::make_endpoint("/bpm");
//	setBPM_rest.support(web::http::methods::PUT, setBPM);

	// Start the endpoints in sequence.
	getBPM_rest.open().wait();
	getMAX_rest.open().wait();
	getMIN_rest.open().wait();
//	setBPM_rest.open().wait();

	while (true)
	{
		//if mode button pressed
		if (digitalRead(BTN_MODE) == HIGH)
		{
			btn_mode_pressed = HIGH;
			btn_tap_pressed = LOW;
			//swap mode
			learnMode = !learnMode;
			if (learnMode)
			{
				//call learn mode
				m.start_timing();
			}
			else
			{
				//call play mode
				m.stop_timing();
				if(m.get_rate().count() != 0)
				{
					//convert the ms rate into bpm and update the variable
					currentBPM = (int)(((double) 1 / (double) m.get_rate().count()) * 60000);
				}
				if(first)
				{
					currentMIN = currentBPM;
					currentMAX = currentBPM;
					first = false;
				}
				else
				{
					if(currentBPM < currentMIN && currentBPM != 0)
						currentMIN = currentBPM;
					if(currentBPM > currentMAX)
						currentMAX = currentBPM;
				}
			}
			this_thread::sleep_for(200ms);
		}
		//if tap button pressed
		else if (digitalRead(BTN_TAP) == HIGH)
		{
			btn_mode_pressed = LOW;
			btn_tap_pressed = HIGH;
			//tap only acts in learn mode
			if (learnMode)
			{
				//taps metronome and flashes led
				m.tap();
				digitalWrite(LED_RED, HIGH);
				this_thread::sleep_for(100ms);
				digitalWrite(LED_RED, LOW);
			}
		}
		else
		{
			btn_mode_pressed = LOW;
			btn_tap_pressed = LOW;
		}
		this_thread::sleep_for(10ms);
	}

	return 0;
}
