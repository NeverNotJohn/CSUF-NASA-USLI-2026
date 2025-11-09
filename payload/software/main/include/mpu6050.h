/*****************************************
 * 
 * MPU6050 IMU Header File
 * 
 *****************************************/
#ifndef MPU6050_H
#define MPU6050_H
#include <vector>
//Define a reusable Vector that has 3 doubles
typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

// Init the MPU6050 sensor
void initMPU6050();
// Reads acceleration data in m/s^2 from accelerometer
Vector3D getAcceleration();
// Reads angular Velocity data in rad/s from gyroscope
Vector3D getAngularVelocity();

#endif // MPU6050_H