/*****************************************
 * 
 * BMP280 Altimeter Header File
 * 
 *****************************************/

#ifndef BMP280_H
#define BMP280_H

// Initilizes the BMP by finding it on I2C adresses 0x77 & 0x76
void initBMP();
// Updates the "baselinePressure" variable
void calibrateBMP();
// Averages 10 pressure readings and returns a baseline pressure in hPa
double calculateBaselinePressure();
// Reads the pressure data in Pa
double getPressure();
// Reads the temperature data in celcius
float getTemperature();
// Reads the Altitude data
// float getAltitude();
// Reads the altitude data using the baseline pressure
float getRelativeAltitude();
// Really for debugging
double getBaselinePressure();
#endif // BMP280_H
