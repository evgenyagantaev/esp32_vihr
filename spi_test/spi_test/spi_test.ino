
#include <SPI.h>

#include "pressure_sensor_object.h"

#define GREEN_LED 26
#define RED_LED   27
#define POWER_ON  16
#define S4_PIN    39

char diagnostics[64];

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

    pinMode(GREEN_LED, OUTPUT); 
    digitalWrite(GREEN_LED, HIGH);  // 
    pinMode(RED_LED, OUTPUT); 
    digitalWrite(RED_LED, LOW);  // 
    pinMode(S4_PIN, INPUT); 
  
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

}

// the loop function runs over and over again until power down or reset
void loop() 
{
    pressure_sensor_measure_pressure_temperature();

    double pressure = pressure_sensor_get_pressure();
    double temp = pressure_sensor_get_temperature();

    sprintf(diagnostics, "P = %f", pressure);
    Serial.println(diagnostics);
    sprintf(diagnostics, "T = %f", temp);
    Serial.println(diagnostics);

    delay(1000);
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
