#include <mpu6050.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <defines.h>
#include "i2c.h"
#include "MadgwickAHRS.h"

Adafruit_MPU6050 mpu;
Vector3D acceleration = {0, 0, 0};
Vector3D angularVelocity = {0, 0, 0};
Madgwick filter;

void initMPU6050()
{
    // Wire1.begin();
    // Try to initialize!
    openI2C();
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
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    filter.begin(512.0f);

    closeI2C();
    Serial.println("MPU6050 initialized successfully!");
    
}
Vector3D getAcceleration() {

    if (openI2C()) {
        sensors_event_t accel, gyro, temp;
        mpu.getEvent(&accel, &gyro, &temp);

        acceleration.x = accel.acceleration.x;
        acceleration.y = accel.acceleration.y;
        acceleration.z = accel.acceleration.z;

        closeI2C();
    }

    return acceleration;
}
Vector3D getAngularVelocity() {

    if (openI2C()) {
        sensors_event_t accel, gyro, temp;
        mpu.getEvent(&accel, &gyro, &temp);

        angularVelocity.x = gyro.gyro.x;
        angularVelocity.y = gyro.gyro.y;
        angularVelocity.z = gyro.gyro.z; // was gyro.acceleration.z

        closeI2C();
    }

    return angularVelocity;
}
float getRoll(){
    return filter.getRoll();
}
float getPitch(){
    return filter.getPitch();
}
float getYaw(){  
    return filter.getYaw();
}
void updateMPUFilter() {
    Vector3D gyro = getAngularVelocity();
    Vector3D accel = getAcceleration();
    filter.updateIMU(gyro.x,gyro.y,gyro.z,accel.x,accel.y,accel.z);
}