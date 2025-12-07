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
 bool writeData(String filePath, const std::vector<DataPacket>& dataArray);
 
 #endif // SDFS_H