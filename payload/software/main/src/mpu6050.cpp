#include <mpu6050.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void initMPU6050()
{
    // Wire1.begin();
    // Try to initialize!
    unsigned status;
    // Test addresses 0x68 (default, AD0 floating)
    status = mpu.begin(0x68);
    if (!status)
    {
        // If 0x68 fails try 0x69 (AD0 pulled high)
        status = mpu.begin(0x69);
    }
    if (!status)
    {
        Serial.println(F("Could not find MPU6050"));
        while (1)
        {
            delay(100);
        } // How does delay work in freeRTOS
    }
    Serial.println("MPU6050 initialized successfully!");
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }
}