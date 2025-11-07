/*****************************************
 * 
 * MPU6050 IMU Header File
 * 
 *****************************************/
#include <defines.h>
#ifndef MPU6050_H
#define MPU6050_H
// Init the MPU6050 sensor
void initMPU6050();
// Reads acceleration data in m/s^2 from accelerometer
long getAcceleration();
// Reads angular acceleration data in rad/s from gyroscope
long getAngularAcceleration();
// Calculates orientation from gyroscope
void getOrientation();

#endif // MPU6050_H