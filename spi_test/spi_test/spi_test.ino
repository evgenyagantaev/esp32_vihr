
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Free_Fonts.h"

#include "FS.h"
#include "SD.h"

#include "RTClib.h"

#include "pressure_sensor_object.h"

#define GREEN_LED 26
#define RED_LED   27
#define POWER_ON  16
#define S1_PIN    34
#define S2_PIN    35
#define S3_PIN    36
#define S4_PIN    39
#define SENSOR_CS  21
#define DISPLAY_CS 22

#define HSPI_MISO   2
#define HSPI_MOSI   15
#define HSPI_SCLK   14
#define HSPI_SS     13

#define VSPI_MISO           19
#define VSPI_MOSI           23
#define VSPI_SCLK           18
#define VSPI_DISPLAY_CS     22
#define VSPI_SENSOR_CS      21


#define DISPLAY_BACKLIGHT 5

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

RTC_DS3231 rtc;
DateTime now;

char diagnostics[64];
char buf[256];
char buf1[128];
char debug_buf[128];
char timestamp_buf[128];
char log_name_buf[128];
int xpos =  2;
int ypos = 12;
int counter = 0;

int current_year;
int current_month;
int current_day;
int current_hour;
int current_minute;
int current_seconds;

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
extern SPIClass spi;
SPIClass * vspi = &spi;

void setup() 
{
    pinMode(POWER_ON, OUTPUT); 
    digitalWrite(POWER_ON, HIGH);

    Serial.begin(115200);
    delay(100); //Take some time to open up the Serial Monitor
    Serial.println("Setup");
 
    pinMode(GREEN_LED, OUTPUT); 
    digitalWrite(GREEN_LED, LOW);  // 
    pinMode(RED_LED, OUTPUT); 
    digitalWrite(RED_LED, HIGH);  // 
    pinMode(S1_PIN, INPUT); 
    pinMode(S2_PIN, INPUT); 
    pinMode(S3_PIN, INPUT); 
    pinMode(S4_PIN, INPUT); 

    //*
  
    pinMode(SENSOR_CS, OUTPUT); // sensor chs
    pinMode(DISPLAY_CS, OUTPUT); // display chs

    //*/

    

    tft.begin();

    Serial.println("Backlight turned on");
    pinMode(DISPLAY_BACKLIGHT, OUTPUT);
    digitalWrite(DISPLAY_BACKLIGHT, HIGH);
    
    tft.setRotation(3);
    tft.setTextSize(2);
    //tft.setTextFont(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  
    //tft.setFreeFont(FSB9);   
    //tft.setFreeFont(FSB12);
    //tft.setFreeFont(FSB18);
    tft.drawString("SETUP", xpos, ypos, GFXFF);


    digitalWrite(DISPLAY_CS, HIGH);
    digitalWrite(SENSOR_CS, LOW);  // cs low
    vspi->setHwCs(false);
    vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    pressure_sensor_object_init();
    vspi->endTransaction();
    digitalWrite(SENSOR_CS, HIGH);  // cs high
    
    
    //*************************************** RTC **************************************
    
    if (! rtc.begin()) 
    {
	    Serial.println("Couldn't find RTC");
	    Serial.flush();
    }
    
    if (rtc.lostPower()) 
    {
	    Serial.println("RTC lost power, let's set the time!");
	    // When time needs to be set on a new device, or after a power loss, the
	    // following line sets the RTC to the date & time this sketch was compiled
	    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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

    //*************************************** SD ***************************************

    //*
    pinMode(HSPI_SS, OUTPUT); //HSPI SS
    SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS

    spiSSDisable(SPI.bus());
    spiDetachSS(SPI.bus(), DISPLAY_BACKLIGHT);

    
    if(!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC)
    {
        Serial.println("MMC");
    } else if(cardType == CARD_SD)
    {
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    } else 
    {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    listDir(SD, "/", 0);
    //createDir(SD, "/mydir");
    //listDir(SD, "/", 0);
    //removeDir(SD, "/mydir");
    //listDir(SD, "/", 2);
    //writeFile(SD, "/hello.txt", "Hello ");
    //appendFile(SD, "/hello.txt", "World!\n");
    //readFile(SD, "/hello.txt");
    //deleteFile(SD, "/foo.txt");
    //deleteFile(SD, "/hello.txt");
    //deleteFile(SD, "/test.txt");
    //renameFile(SD, "/hello.txt", "/foo.txt");
    //readFile(SD, "/foo.txt");
    //testFileIO(SD, "/test.txt");
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    // form timestamp
    now = rtc.now();

    current_year = now.year();
    current_month = now.month();
    current_day = now.day();
    current_hour = now.hour();
    current_minute = now.minute();
    current_seconds = now.second();
    sprintf(timestamp_buf, "%4d.%02d.%02d %02d:%02d:%02d\n", current_year, current_month, current_day, current_hour, current_minute, current_seconds);
    sprintf(log_name_buf, "/log_%4d%02d%02d_%02d%02d.txt", current_year, current_month, current_day, current_hour, current_minute);
    appendFile(SD, log_name_buf, "Power ON\n");
    appendFile(SD, log_name_buf, timestamp_buf); 
    appendFile(SD, log_name_buf, "***\n\n");   

    //*/
    //*************************************** sd ***************************************

    //tft.drawString("Setup finished    ", xpos, ypos, GFXFF);
    //tft.drawString("Pressure              ", xpos, ypos, GFXFF);
    //tft.drawString("registration          ", xpos, ypos+43, GFXFF);
    tft.drawString("                         ", xpos, ypos, GFXFF);
    tft.drawString("                         ", xpos, ypos+30, GFXFF);
    tft.drawString("                         ", xpos, ypos+60, GFXFF);
    tft.drawString("                         ", xpos, ypos+90, GFXFF);

    pinMode(DISPLAY_BACKLIGHT, OUTPUT);
    digitalWrite(DISPLAY_BACKLIGHT, HIGH);

    //tft.setFreeFont(FSB12);
    
    Serial.println("Setup finished");
    
}


int power_off_counter = 0;
// the loop function runs over and over again until power down or reset
void loop() 
{
    
    //*
    digitalWrite(DISPLAY_CS, HIGH);
    digitalWrite(SENSOR_CS, LOW);  // cs low
    vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    pressure_sensor_measure_pressure_temperature();
    vspi->endTransaction();
    digitalWrite(SENSOR_CS, HIGH);  // cs high

    xpos = 2;  ypos = 12;

    if(digitalRead(S1_PIN) == LOW)
      tft.drawString("button 1                 ", xpos, ypos, GFXFF);
    else
      tft.drawString("                         ", xpos, ypos, GFXFF);
    if(digitalRead(S2_PIN) == LOW)
      tft.drawString("button 2                 ", xpos, ypos+30, GFXFF);
    else
      tft.drawString("                         ", xpos, ypos+30, GFXFF);
    if(digitalRead(S3_PIN) == LOW)
      tft.drawString("button 3                 ", xpos, ypos+60, GFXFF);
    else
      tft.drawString("                         ", xpos, ypos+60, GFXFF);
    if(digitalRead(S4_PIN) == HIGH)
      tft.drawString("button 4                 ", xpos, ypos+90, GFXFF);
    else
      tft.drawString("                         ", xpos, ypos+90, GFXFF);

    if(digitalRead(S4_PIN) == HIGH)
    {
        power_off_counter++;
        
        if(power_off_counter > 13)
        {
            tft.fillScreen(TFT_BLACK);
            xpos = 2;  ypos = 12;
            tft.drawString("OFF", xpos, ypos, 4);
            digitalWrite(POWER_ON, LOW);

            while(1)
            {
                delay(1000);
            }
        }
    }
    else
    {
        power_off_counter = 0;
    }
    

    double pressure = pressure_sensor_get_pressure();
    double temp = pressure_sensor_get_temperature();

    

    now = rtc.now();

    current_year = now.year();
    current_month = now.month();
    current_day = now.day();
    current_hour = now.hour();
    current_minute = now.minute();
    current_seconds = now.second();
    sprintf(timestamp_buf, "%4d.%02d.%02d %02d:%02d:%02d", current_year, current_month, current_day, current_hour, current_minute, current_seconds);


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
    
    sprintf(diagnostics, "P = %d", (int)pressure);
    sprintf(buf, "P = %d     ", (int)pressure);
    Serial.println(diagnostics);
    sprintf(diagnostics, "T = %d     ", (int)temp);
    sprintf(buf1, "T = %d     ", (int)temp);
    Serial.println(diagnostics);
    //Serial.end();


    //*
    xpos = 2;  ypos = 135;
    tft.drawString(timestamp_buf, xpos, ypos, GFXFF);
    xpos = 2;  ypos = 160;
     tft.drawString(buf, xpos, ypos, GFXFF);
    xpos = 2;  ypos = 185;
    tft.drawString(buf1, xpos, ypos, GFXFF);
    sprintf(debug_buf, "%d     ", counter);
    xpos = 2;  ypos = 210;
    tft.drawString(debug_buf, xpos, ypos, GFXFF);
    counter++;
    //*/

    // write in sd-card
    sprintf(buf, "%s P= %04dhPa T= %02dC\n", timestamp_buf, (int)(round(pressure/100)), (int)(round(temp/100)));
    appendFile(SD, log_name_buf, buf);

    //delay(1000);
}

void vspiCommand() 
{
    byte data = 0b01010101; // junk data to illustrate usage
  
    //use it as you would the regular arduino SPI API
    //vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(21, LOW); //pull SS slow to prep other end for transfer
    //vspi->transfer(data);  
    digitalWrite(21, HIGH); //pull ss high to signify end of data transfer
    //vspi->endTransaction();
}
