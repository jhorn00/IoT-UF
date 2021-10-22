//Class function definitions for LCD driver class - James Horn
#include "lcd_horn.h"

//default (and only constructor)
LCD_Thing::LCD_Thing()
{
    fd = wiringPiI2CSetup(I2C_ADDR);
    lcd_init(); // setup LCD
}
// float to string
void LCD_Thing::typeFloat(float myFloat)
{
    char buffer[20];
    sprintf(buffer, "%4.2f", myFloat);
    typeln(buffer);
}

// int to string
void LCD_Thing::typeInt(int i)
{
    char array1[20];
    sprintf(array1, "%d", i);
    typeln(array1);
}

// clr lcd go home loc 0x80
void LCD_Thing::ClrLcd(bool backlight) //Modified by James Horn
{
    lcd_byte(0x01, LCD_CMD, backlight);
    lcd_byte(0x02, LCD_CMD, backlight);
}

// go to location on LCD
void LCD_Thing::lcdLoc(int line)
{
    lcd_byte(line, LCD_CMD, true);
}

// out char to LCD at current position
void LCD_Thing::typeChar(char val)
{

    lcd_byte(val, LCD_CHR, true);
}

// this allows use of any size string
void LCD_Thing::typeln(const char *s)
{

    while (*s)
        lcd_byte(*(s++), LCD_CHR, true);
}

void LCD_Thing::lcd_byte(int bits, int mode, bool backlight) //Modified by James Horn
{

    //Send byte to data pins
    // bits = the data
    // mode = 1 for data, 0 for command
    int bits_high;
    int bits_low;
    // uses the two half byte writes to LCD
    // backlight will be on or off based on backlight parameter
    if (backlight)
    {
        // uses the two half byte writes to LCD
        bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
        bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;
    }
    else
    {
        bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT_OFF;
        bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT_OFF;
    }
    // High bits
    wiringPiI2CReadReg8(fd, bits_high);
    lcd_toggle_enable(bits_high);

    // Low bits
    wiringPiI2CReadReg8(fd, bits_low);
    lcd_toggle_enable(bits_low);
}

void LCD_Thing::lcd_toggle_enable(int bits)
{
    // Toggle enable pin on LCD display
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
    delayMicroseconds(500);
}

void LCD_Thing::lcd_init()
{
    // Initialise display
    lcd_byte(0x33, LCD_CMD, true); // Initialise
    lcd_byte(0x32, LCD_CMD, true); // Initialise
    lcd_byte(0x06, LCD_CMD, true); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD, true); // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD, true); // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD, true); // Clear display
    delayMicroseconds(500);
}

//print strings to the desired line - just makes things more convenient - Written by James Horn
void LCD_Thing::writeStringToLine(std::string str, int line)
{
    if (line != LINE1 && line != LINE2)
    {
        std::cout << "Invalid Line Number\n";
        return;
    }
    lcdLoc(line);
    typeln(str.c_str());
}
