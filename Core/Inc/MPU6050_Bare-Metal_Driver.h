#ifndef MPU6050_BAREMETAL_H
#define MPU6050_BAREMETAL_H

#include "main.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_utils.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "I2C_Driver.h"

// --- MPU6050 ADDRESS CONSTANTS ---
#define MPU6050_I2C_ADDDRESS 0x68 // 7-bit address for the MPU Sensor

// ACCELERATION REGISTER ADDRESSES CONSTANTS
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0X3F
#define MPU6050_ACCEL_ZOUT_L 0X40

// GYROSCOPE REGISTER ADDRESS CONSTANTS
#define MPU6050_GYRO_XOUT_H 0X43
#define MPU6050_GYRO_XOUT_L 0X44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48

// Data struct to hold the many values the MPU6050 have
typedef struct {
    uint8_t I2C_Addr;

    int32_t Accel_X_Offset, Accel_Y_Offset, Accel_Z_Offset;
    int32_t Gyro_X_Offset, Gyro_Y_Offset, Gyro_Z_Offset;

    float Raw_Accel_X, Raw_Accel_Y, Raw_Accel_Z_G; // Raw Acceleration data
    float Accel_XG, Accel_YG, Accel_ZG; // Acceleration in g's
    float Accel_Xm, Accel_Ym, Accel_Zm; // Acceleration in m/s^2

    float Raw_Gyro_X, Raw_Gyro_Y, Raw_Gyro_Z; // Raw Gyro data
    float Roll; // Rotation in degrees (Orientation)
    float Pitch;
    float Yaw;
} MPU6050_t;

// DECLARE FUNCTIONS
void MPU6050_Init(MPU6050_t *mpu);
void MPU6050_Calibrate(MPU6050_t *mpu);
void Read_I2C_Acceleration(MPU6050_t *mpu);
void Read_I2C_Gyroscope(MPU6050_t *mpu);

#endif /* MPU6050_BAREMETAL_H */