#include <Arduino.h>

static bool sendCommand(String cmd)
{
    String cmdTemp = cmd + "\r\n";

    Serial1.print(cmdTemp);
    delay(500);                                                // Wait for +OK

    return 1;
}

bool txRYLR896(const char* data)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "AT+SEND=0,%i,%s", strlen(data), data);

    // Debug
    return sendCommand(buf);
}

/******** EXTERNAL FUNCTIONS ********/

void initRYLR896()
{
    Serial1.begin(115200);

    sendCommand("AT+ADDRESS=1");
    sendCommand("AT+NETWORKID=4");
    sendCommand("AT+BAND=901000000");
    sendCommand("AT+PARAMETER=12,4,1,7");

    delay(1000);                                               // RYLR896 Needs time to setup
}       

void setup() {
    initRYLR896();
    Serial.begin(9600);

    delay(5000);
    Serial.print("Oh Yea! \n");
}

void loop() {

    while(Serial1.available())
    {
        Serial.write(Serial1.read());
    }

    while(Serial.available())
    {
        String line = Serial.readStringUntil('\n');
        line.trim();
        
        if (line == "C")
        {
            //Serial.println("Sending Calibrate CMD");
            txRYLR896("C");
        }
        if (line == "A")
        {
            //Serial.println("Sending Arm CMD");
            txRYLR896("A");
        }
    }

    delay(100);
}
