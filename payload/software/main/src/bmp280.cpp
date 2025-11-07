/*****************************************
 * 
 * BMP280 Altimeter Source File
 * 
 *****************************************/

#include <bmp280.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <arduino_freertos.h>
#include <queue.h>

static Adafruit_BMP280 bmp;

/************** DATA VARIABLES **************/
QueueHandle_t altitudeQueue;
static double baselinePressure;

// FIXME try and catch
// Initilizes the BMP by finding it on I2C adresses 0x77 & 0x76
void initBMP()
{

    // I2C mutex stuff

    Wire.begin(); 
    Serial.println("BMP280 init");

    unsigned status;
    status = bmp.begin(0x77); 

    if (!status) {
        status = bmp.begin(0x76);
    }

    if (!status) {
        Serial.println("Could not find BMP280");        
    }

    calibrateBMP(); 

    Serial.print("BMP280 SensorID: 0x");
    Serial.println(bmp.sensorID(),16);
    Serial.println("BMP280 initialized successfully!");
}

// Guess
double calculateBaselinePressure()
{
    double pressureSum = 0;

    for (int i = 0; i < 10; i++)
    {
        pressureSum += (double)bmp.readPressure();
        delay(500);
    }

    // Return the average in hPa
    return (pressureSum/10.0)/100.0;
}

// Updates the "baselinePressure" variable
void calibrateBMP(){
    baselinePressure = calculateBaselinePressure(); // Sets the global variable "baselinePressure" to the returned value of "getBaselinePressure()"
}

double getBaselinePressure()
{
    return baselinePressure;
}

// Reads the temperature data in celcius
float getTemperature()
{
    return bmp.readTemperature();
}

// Reads the altitude data using the baseline pressure
float getAltitude()
{
    return bmp.readAltitude(baselinePressure);
}