/*****************************************
 * 
 * NEO6M GPS Header File
 * 
 *****************************************/

#ifndef NEO6M_H
#define NEO6M_H
void initNEO6M();
bool encodeGPS();
float getLongitude();
float getLatitude();
void getGPSTimeString();
float getSats();
#endif // NEO6M_H
