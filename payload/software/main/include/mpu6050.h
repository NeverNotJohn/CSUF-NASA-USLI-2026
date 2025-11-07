/*****************************************
 * 
 * MPU6050 IMU Header File
 * 
 *****************************************/
#ifndef MPU6050_H
#define MPU6050_H
#include <defines.h>
// Init the MPU6050 sensor
void initMPU6050();
// Reads acceleration data in m/s^2 from accelerometer
Vector3D getAcceleration();
// Reads angular acceleration data in rad/s from gyroscope
Vector3D getAngularAcceleration();
// Calculates orientation from gyroscope
void getOrientation();

#endif // MPU6050_H