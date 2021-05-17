
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Free_Fonts.h"

#include "RTClib.h"

#include "pressure_sensor_object.h"

#define GREEN_LED 26
#define RED_LED   27
#define POWER_ON  16
#define S4_PIN    39

#define DISPLAY_BACKLIGHT 5

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

RTC_DS3231 rtc;

char diagnostics[64];
char buf[128];
int xpos =  0;
int ypos = 40;
int counter = 0;

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;

void setup() 
{
    pinMode(POWER_ON, OUTPUT); 
    digitalWrite(POWER_ON, HIGH);

    Serial.begin(115200);
    delay(100); //Take some time to open up the Serial Monitor
    Serial.println("Setup");
    //Serial.end();

    pinMode(GREEN_LED, OUTPUT); 
    digitalWrite(GREEN_LED, HIGH);  // 
    pinMode(RED_LED, OUTPUT); 
    digitalWrite(RED_LED, LOW);  // 
    pinMode(S4_PIN, INPUT); 

    //*
  
    //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
    vspi = new SPIClass(VSPI);
    
    //clock miso mosi ss
  
    //initialise vspi with default pins
    //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
    vspi->begin();
    //alternatively route through GPIO pins of your choice
    //hspi->begin(0, 2, 4, 33); //SCLK, MISO, MOSI, SS
    
    //set up slave select pins as outputs as the Arduino API
    //doesn't handle automatically pulling SS low
    pinMode(21, OUTPUT); // sensor chs
    pinMode(22, OUTPUT); // display chs

    vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    pressure_sensor_object_init();
    vspi->endTransaction();

    //*/

    //Serial.println("Backlight turned on");
    //pinMode(DISPLAY_BACKLIGHT, OUTPUT);
    //digitalWrite(DISPLAY_BACKLIGHT, HIGH);

    tft.begin();
    tft.setRotation(1);
    tft.setTextFont(1);
    tft.fillScreen(TFT_BLACK);
    //tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    //snprintf(buf, sizeof(buf), "HELLO");
    //tft.drawString(buf, 5, 5, 7);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(xpos, ypos);    // Set cursor near top left corner of screen
  
    tft.setFreeFont(FSB9);   
    tft.println("HELLO"); 
    tft.setFreeFont(FSB12);
    tft.println("HELLO");    
    tft.setFreeFont(FSB18);
    tft.println("HELLO");   
    
    //*************************************** RTC **************************************
    
    if (! rtc.begin()) 
    {
	    Serial.println("Couldn't find RTC");
	    Serial.flush();
	    abort();
    }
    
    if (rtc.lostPower()) 
    {
	    Serial.println("RTC lost power, let's set the time!");
	    // When time needs to be set on a new device, or after a power loss, the
	    // following line sets the RTC to the date & time this sketch was compiled
	    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	    // This line sets the RTC with an explicit date & time, for example to set
	    // January 21, 2014 at 3am you would call:
	    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

	// When time needs to be re-set on a previously configured device, the
	// following line sets the RTC to the date & time this sketch was compiled
	//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	// This line sets the RTC with an explicit date & time, for example to set
	// January 21, 2014 at 3am you would call:
	// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  
    //*************************************** rtc **************************************

    Serial.println("Setup finished");
    ypos = 120;
    tft.setCursor(xpos, ypos);
    tft.setFreeFont(FSB12);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
    //*

    vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    pressure_sensor_measure_pressure_temperature();
    vspi->endTransaction();

    double pressure = pressure_sensor_get_pressure();
    double temp = pressure_sensor_get_temperature();

    delay(1000);

    //Serial.begin(115200);
    //delay(1000);
    
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    sprintf(diagnostics, "P = %f", pressure);
    sprintf(buf, "P = %f", pressure);
    Serial.println(diagnostics);
    sprintf(diagnostics, "T = %f", temp);
    Serial.println(diagnostics);
    //Serial.end();

    xpos = 7;  ypos = 160;
    tft.setCursor(xpos, ypos);
    tft.print(buf);

    //*/

    /*
    //vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    pressure_sensor_measure_pressure_temperature();
    double pressure = pressure_sensor_get_pressure();
    sprintf(buf, "P = %f", pressure);
    //vspi->endTransaction();

    delay(1000);

    //sprintf(buf, "C = %d     ", counter);
    xpos = 7;  ypos = 160;
    tft.setCursor(xpos, ypos);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(buf, xpos, ypos, GFXFF);
    counter++;
    //*/

    delay(3000);
}

void vspiCommand() 
{
    byte data = 0b01010101; // junk data to illustrate usage
  
    //use it as you would the regular arduino SPI API
    //vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(21, LOW); //pull SS slow to prep other end for transfer
    vspi->transfer(data);  
    digitalWrite(21, HIGH); //pull ss high to signify end of data transfer
    vspi->endTransaction();
}
