/*****************************************
 * 
 * SD File System Header File
 * 
 * Reads and write data to the SD card in a CSV format
 *****************************************/

#ifndef SDFS_H
#define SDFS_H
#include "defines.h"
/************** EXTERNAL FUNCTIONS **************/

void createFile(String filePath);
void createSoilFile(String filePath);
bool writeData(String filePath, DataPacketLog& dataArray);
bool writeSoilData(String filePath,  SoilPacketLog& dataArray);

#endif // SDFS_H