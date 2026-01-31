/*****************************************
 * 
 * Hailsense 7-in-1 Soil Sensor Source File 
 * 
 * Modbus RTU magic to read soil sensor data
 *****************************************/

#include "soilsensor.h"
#include "defines.h"
#include "arduino_freertos.h"
#include <array>
/******** STATIC VARS ********/
static const byte SENSOR_ADDRESS = 0x01;
const int RS485_DE_RE = 2; // Pin to control RS485 board DE/RE
static const byte READ_COMMAND[] = {
    SENSOR_ADDRESS,
    0x03, // Function Code: Read Holding Registers
    0x00, // Start Address (Hi)
    0x00, // Start Adress (Lo)
    0x00, // Number of Points/Registers (Hi)
    0x07, // Number of Points/Registers (Lo)
    0x04, // Error Check (CRC) (Lo)
    0x08  // Error Check (CRC) (Hi)
};
static float soilHumidity;
static float soilTemperature_c;
static float soilEC_us_cm;
static float soilpH;
static float nitrogen_mg_kg;
static float phosphorus_mg_kg;
static float potassium_mg_kg;

/******** INTERNAL FUNCTIONS ********/

uint16_t calculateCRC(const byte buf[], int len) {
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];    // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;             // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else {                     // Else just shift right
                crc >>= 1;
            }
        }
    }
    return crc;
}

/******** EXTERNAL FUNCTIONS ********/
void initSoilSensor()
{
    SOIlSENSOR_SERIAL.begin(SOIlSENSOR_BAUD_RATE);
    pinMode(RS485_DE_RE, 1); // Set as output aka OUTPUT
    digitalWrite(RS485_DE_RE, 0); // Set to receive mode aka LOW

    Serial.println("Soil Sensor Initialized");
}

bool readSoilSensor(){
    digitalWrite(RS485_DE_RE, 1); // Set to transmit mode aka HIGH
    uint16_t crc = calculateCRC(READ_COMMAND, 6); // Cyclic Redundancy Check
    byte command[8];
    for(int i = 0; i < 6; i++) {
        command[i] = READ_COMMAND[i];
    }
    command[6] = crc & 0xFF; // CRC low byte
    command[7] = (crc >> 8) & 0xFF; // CRC high byte
    SOIlSENSOR_SERIAL.write(command, sizeof(command));
    SOIlSENSOR_SERIAL.flush();
    digitalWrite(RS485_DE_RE, 0); // Set to receive mode aka LOW

    unsigned long startTime = millis();
    while (SOIlSENSOR_SERIAL.available() < 19) { // Waiting for 19 byte response
        if (millis() - startTime > 500) return false; // Timeout after 500ms
    }

    byte response[19];
    SOIlSENSOR_SERIAL.readBytes(response, 19);

    uint16_t receivedCRC = (response[18] << 8) | response[17];
    if (calculateCRC(response, 17) != receivedCRC) {
        Serial.println("CRC Error");
        return false;
    }

    // Parse response (registers: hum, temp, ec, ph, n, p, k)
    uint16_t rawHumidity = (response[3] << 8) | response[4];
    uint16_t rawTemperature = (response[5] << 8) | response[6];
    uint16_t rawEC = (response[7] << 8) | response[8];
    uint16_t rawPH = (response[9] << 8) | response[10];
    uint16_t rawNitrogen = (response[11] << 8) | response[12];
    uint16_t rawPhosphorus = (response[13] << 8) | response[14];
    uint16_t rawPotassium = (response[15] << 8) | response[16];
    soilHumidity = rawHumidity / 10.0f;
    soilTemperature_c = rawTemperature / 10.0f;
    soilEC_us_cm = float(rawEC);
    soilpH = rawPH / 10.0f;
    nitrogen_mg_kg = float(rawNitrogen);
    phosphorus_mg_kg = float(rawPhosphorus);
    potassium_mg_kg = float(rawPotassium);
    return true;
    
}

float getSoilHumidity() {
    return soilHumidity;
}

float getSoilTemperature_c() {
    return soilTemperature_c;
}

float getSoilEC_us_cm() {
    return soilEC_us_cm;
}
float getSoilpH() {
    return soilpH;
}
float getN_mg_kg() {
    return nitrogen_mg_kg;
}

float getP_mg_kg() {
    return phosphorus_mg_kg;
}

float getK_mg_kg() {
    return potassium_mg_kg;
}

