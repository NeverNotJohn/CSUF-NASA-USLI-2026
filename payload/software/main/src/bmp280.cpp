#include <bmp280.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
float baselinePressure;
void initBMP()
{
   //Call begin on Wire
    Wire.begin(); 
    Serial.println(F("BMP280 init"));

    unsigned status;
    //Test addresses 0x77 (default, SDO floating)
    status = bmp.begin(0x77); 

    if (!status) {
        // If 0x77 fails try 0x76 (SDO grounded)
        status = bmp.begin(0x76);
    }

    if (!status) {
        Serial.println(F("Could not find BMP280"));        
        while (1) { delay(100); } // How does delay work in freeRTOS
    }
    calibrateBMP(); // Calibrate the bmp, set the global variable to the average pressure
    Serial.print("BMP280 SensorID: 0x");
    Serial.println(bmp.sensorID(),16);
    Serial.println(F("BMP280 initialized successfully!"));
}

void calibrateBMP(){
    baselinePressure = calculateBaselinePressure(); // Sets the global variable "baselinePressure" to the returned value of "getBaselinePressure()"
}

float calculateBaselinePressure()
{
    float pressureSum = 0;
    // Take 10 readings, average it out, and return it
    for (int i = 0; i < 10; i++)
    {
        pressureSum += getPressure();
        // Delay for reading stability
        delay(500);
    }
    // Return the average in hPa
    return (float)(pressureSum/10)/100;
}
float getBaselinePressure()
{
    return baselinePressure;
}

float getTemperature()
{
    return bmp.readTemperature();
}
float getPressure()
{
    return bmp.readPressure();
}
// float getAltitude()
// {
//     return bmp.readAltitude();
// }
float getRelativeAltitude()
{
    return bmp.readAltitude(baselinePressure);
}


