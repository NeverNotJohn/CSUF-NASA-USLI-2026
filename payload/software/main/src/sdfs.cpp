/*****************************************
 * 
 * SD File Systen Source File
 * 
 *****************************************/

#include "sdfs.h"
#include "defines.h"
#include <SD.h>
#include <vector>

/************** STATIC VARS **************/

/************** FUNCTIONS **************/


void createFile(String filePath) {
    // Initialize SD card
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("No card");
        return;
    }
    Serial.println("SD Card initialized.");
    // Check if .csv file exists already
    if (SD.exists(filePath.c_str())) {
        Serial.println("File already exists.");
        return;
    }
    // Create .csv file
    File dataFile = SD.open(filePath.c_str(), FILE_WRITE);
    if (dataFile) {
        // Write CSV header
        dataFile.println("n,hour,min,sec,altitude_ft,long,lat,roll,yaw,pitch");
        dataFile.close();
        Serial.println("File created and header written.");
    } else {
        Serial.println("Error creating file.");
    }
}

void createSoilFile(String filePath) {
    // Initialize SD card
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("No card");
        return;
    }
    Serial.println("SD Card initialized.");
    // Check if .csv file exists already
    if (SD.exists(filePath.c_str())) {
        Serial.println("File already exists.");
        return;
    }
    // Create .csv file
    File dataFile = SD.open(filePath.c_str(), FILE_WRITE);
    if (dataFile) {
        // Write CSV header
        dataFile.println("n,hour,min,sec,pH,EC");
        dataFile.close();
        Serial.println("File created and header written.");
    } else {
        Serial.println("Error creating file.");
    }
}

char* parsePacket(DataPacket packet){
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "%lu,%u,%u,%u,%f,%f,%f,%f,%f,%f",
             packet.n, packet.hour, packet.min, packet.sec,
             packet.altitude_ft, packet.lng, packet.lat,
             packet.roll_deg, packet.pitch_deg, packet.yaw_deg);
    return buffer;
}

char* parseSoilPacket(SoilPacket packet){
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "%u,%u,%u,%u,%f,%f",
             packet.n, packet.hour, packet.min, packet.sec, packet.pH, packet.EC);
    return buffer;
}

// Using a vector for the datapacket array to allow dynamic sizing, can be changed to array if needed
bool writeData(String filePath,  DataPacketLog& dataArray) {
    File dataFile = SD.open(filePath.c_str(), FILE_WRITE);
    if (dataFile) {
        for (int i = 0; i < dataArray.size; i++) {
            const char* packetData = parsePacket(dataArray.data[i]);
            dataFile.println(packetData);
        }
        dataFile.close();
        Serial.println("Data written to file successfully.");
        return true;
    } else {
        Serial.println("Error opening file for writing.");
        return false;
    }
}

bool writeSoilData(String filePath,  SoilPacketLog& dataArray) {
    File dataFile = SD.open(filePath.c_str(), FILE_WRITE);
    if (dataFile) {
        for (int i = 0; i < dataArray.size; i++) {
            const char* packetData = parseSoilPacket(dataArray.data[i]);
            dataFile.println(packetData);
        }
        dataFile.close();
        Serial.println("Data written to file successfully.");
        return true;
    } else {
        Serial.println("Error opening file for writing.");
        return false;
    }
}
