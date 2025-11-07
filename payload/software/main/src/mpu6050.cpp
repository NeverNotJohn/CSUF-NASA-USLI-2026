#include <mpu6050.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <defines.h>
#include <TimeLib.h>
#include <vector>
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
        Serial.println("Could not find MPU6050");
        while (1)
        {
            delay(100);
        } // How does delay work in freeRTOS
    }
    Serial.println("MPU6050 initialized successfully!");
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}
Vector3D getAcceleration(){
    Vector3D acceleration;
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    acceleration.x = accel.acceleration.x;
    acceleration.y = accel.acceleration.y;
    acceleration.z = accel.acceleration.z;
    return acceleration;
}

Vector3D getAngularAcceleration(){
    Vector3D angularAcceleration;
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    angularAcceleration.x = gyro.orientation.x;
    angularAcceleration.y = gyro.orientation.y;
    angularAcceleration.z = gyro.acceleration.z;
    return angularAcceleration;
}


Vector3D getOrientation(){
    
}
