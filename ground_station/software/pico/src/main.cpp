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

    // Change freq
    sendCommand("AT+BAND=915000000");
    delay(500);

    // Send
    snprintf(buf, sizeof(buf), "AT+SEND=13,%i,%s", strlen(data), data);
    sendCommand(buf);

    // Change freq
    sendCommand("AT+BAND=901000000");
    delay(500);

    // Debug
    return 1;
}

/******** EXTERNAL FUNCTIONS ********/

void initRYLR896()
{
    Serial1.begin(115200);

    sendCommand("AT+ADDRESS=1");
    sendCommand("AT+NETWORKID=4");
    sendCommand("AT+BAND=901000000");
    sendCommand("AT+PARAMETER=10,7,1,7");

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
        if (line == "DT")
        {
            //Serial.println("Sending Arm CMD");
            txRYLR896("D");
        }
        if (line == "67")
        {
            //Serial.println("Sending Arm CMD");
            txRYLR896("6");
        }
        if (line == "LT")
        {
            //Serial.println("Sending Arm CMD");
            txRYLR896("L");
        }
        if (line == "F")
        {
            sendCommand("AT+BAND=901000000");

            // Debug
            sendCommand("AT+BAND?");
        }
    }

    delay(100);
}
