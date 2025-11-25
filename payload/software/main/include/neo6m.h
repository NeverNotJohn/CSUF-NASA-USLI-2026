/*****************************************
 * 
 * NEO6M GPS Header File
 * 
 *****************************************/

#ifndef NEO6M_H
#define NEO6M_H

/************** DEFINES **************/
#define NEO6M_SERIAL        Serial2
#define NEO6M_BAUD_RATE     9600

/************** EXTERNAL FUNCTIONS **************/
void initNEO6M();
bool encodeGPS();
float getLongitude();
float getLatitude();
void getGPSTimeString();
float getSats();

#endif // NEO6M_H
