/*****************************************
 * 
 * BMP280 Altimeter Source File
 * 
 *****************************************/

#include "bmp280.h"
#include <Adafruit_BMP280.h>
#include "i2c.h"
#include "beep.h"

static Adafruit_BMP280 bmp;

/************** STATIC VARIABLES **************/
static double baselinePressure;
static bool statusFlag;

// FIXME try and catch
// Initilizes the BMP by finding it on I2C adresses 0x77 & 0x76
void initBMP()
{
    openI2C();

    // Setup
    statusFlag = bmp.begin(0x77); 
    if (!statusFlag) statusFlag = bmp.begin(0x76);
    if (!statusFlag) Serial.println("Could not find BMP280");      


    // Set Settings
    bmp.setSampling(
        Adafruit_BMP280::MODE_NORMAL,
        Adafruit_BMP280::SAMPLING_X2,                              // Temp          
        Adafruit_BMP280::SAMPLING_X16,                             // Pressure
        Adafruit_BMP280::FILTER_OFF,
        Adafruit_BMP280::STANDBY_MS_1
    );

    closeI2C();

    // Calibrate
    calibrateBMP(); 

    Serial.print("BMP280 SensorID: 0x");
    Serial.println(bmp.sensorID(),16);
    Serial.println("BMP280 initialized successfully!");
}

// Guess
double calculateBaselinePressure_hPa()
{
    double pressureSum = 0;

    openI2C();

    for (int i = 0; i < 10; i++)
    {
        pressureSum += (double)bmp.readPressure();
        delay(500);
    }

    closeI2C();

    // Return the average in hPa
    return (pressureSum/10.0)/100.0;
}

// Updates the "baselinePressure" variable
void calibrateBMP(){
    baselinePressure = calculateBaselinePressure_hPa(); // Sets the global variable "baselinePressure" to the returned value of "getBaselinePressure()"
}

double getBaselinePressure()
{
    return baselinePressure;
}

// Reads the temperature data in celcius
float getTemperature_C()
{
    float val = -676767.0;
    if (openI2C())
    {
        val = bmp.readTemperature();
        closeI2C();
    }

    return val;
    return bmp.readTemperature();
}

// Reads the altitude data using the baseline pressure
float getAltitude_ft()
{
    float val = -676767.0;
    if (openI2C())
    {
        val = bmp.readAltitude(baselinePressure) * 3.28084;
        closeI2C();
    }

    return val;
}