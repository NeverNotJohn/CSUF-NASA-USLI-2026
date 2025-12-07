/*****************************************
 * 
 * BMP280 Altimeter Header File
 * 
 * Pressure sensor used to read altitude
 *****************************************/

#ifndef BMP280_H
#define BMP280_H

/************** EXTERNAL FUNCTIONS **************/
void initBMP();
void calibrateBMP();
float getTemperature_C();
float getAltitude_ft();

#endif // BMP280_H
