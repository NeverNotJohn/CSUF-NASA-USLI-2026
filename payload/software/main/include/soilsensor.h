/*****************************************
 * 
 * Hailsense 7-in-1 Soil Sensor Header File
 * 
 * Soil sensor used to read temperature, humidity, electircal conductivity, pH, and nitrogen, phosphorus, potassium
 *****************************************/

#ifndef SOIlSENSOR_H
#define SOIlSENSOR_H

/************** DEFINES **************/
#define SOIlSENSOR_SERIAL        Serial6
#define SOIlSENSOR_BAUD_RATE     4800

/************** EXTERNAL FUNCTIONS **************/
void initSoilSensor();
bool readSoilSensor();
float getSoilHumidity();
float getSoilTemperature_c();
float getSoilEC_us_cm();
float getSoilpH();
float getN_mg_kg();
float getP_mg_kg();
float getK_mg_kg();

#endif // SOIlSENSOR_H
