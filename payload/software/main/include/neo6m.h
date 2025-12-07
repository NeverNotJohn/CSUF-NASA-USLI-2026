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
float getLongitude();
float getLatitude();
void getGPSTimeString();
float getSats();
bool encodeGPS();
#endif // NEO6M_H
