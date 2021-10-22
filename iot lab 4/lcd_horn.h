//LCD Driver class created by James Horn based on existing LCD I2C code.
#pragma once
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

// Define some device parameters
#define I2C_ADDR 0x27 // I2C device address
//*May be different for other devices

// Define some device constants
#define LCD_CHR 1              // Mode - Sending data
#define LCD_CMD 0              // Mode - Sending command
#define LINE1 0x80             // 1st line
#define LINE2 0xC0             // 2nd line
#define LCD_BACKLIGHT 0x08     // On
#define LCD_BACKLIGHT_OFF 0x00 // Off
#define ENABLE 0b00000100      // Enable bit

class LCD_Thing
{
    int fd;
    void lcd_init();
    void lcd_byte(int bits, int mode, bool backlight); //backlight toggle added by James Horn
    void lcd_toggle_enable(int bits);

public:
    LCD_Thing();
    void typeInt(int i);
    void typeFloat(float myFloat);
    void lcdLoc(int line);       //move cursor
    void ClrLcd(bool backlight); //clr LCD return home - backlight toggle added by James Horn
    void typeln(const char *s);
    void typeChar(char val);
    void writeStringToLine(std::string str, int line); //added by James Horn
};
