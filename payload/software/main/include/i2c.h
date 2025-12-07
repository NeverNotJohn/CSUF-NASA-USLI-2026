/*****************************************
 * 
 * I2C Header File
 * 
 * #include this file whenever using the
 * I2C bus
 * 
 * Contains mutexs and such
 * 
 *****************************************/

#ifndef I2C_H
#define I2C_H

/************** FUNCTIONS **************/
void initI2C();
bool openI2C();
bool closeI2C();


#endif // I2C_H