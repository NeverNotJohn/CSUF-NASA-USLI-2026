/*****************************************
 * 
 * NEO6M LoRa Transceiver Header File
 * 
 *****************************************/
#ifndef NEO6M_H
#define NEO6M_H

/******** SETTINGS ********/
#define NEO6M_SERIAL        Serial2     

/******** FUNCTIONS ********/
void initNEO6M();
long getLat();
long getLong();

#endif // NEO6M_H