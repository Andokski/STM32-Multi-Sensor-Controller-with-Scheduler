#include "MPU6050_Bare-Metal_Driver.h"
#include "I2C_Driver.h"
#include "main.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_utils.h"

void MPU6050_Init(MPU6050_t *mpu) {
    // Wake up the sensor AND set clock source to X-axis Gyro for stability (Write 0000 0001)
    I2C_Write(mpu->I2C_Addr, 0x6B, 0x01);
    
    // Set Gyro to +- 500 degrees/sec (Write 0000 1000)
    I2C_Write(mpu->I2C_Addr, 0x1B, 0x08);
    
    // Set Accel to +- 8g (Write 0001 0000)
    I2C_Write(mpu->I2C_Addr, 0x1C, 0x10);
    
    // Turn on the Low Pass Filter to level 6 for vibration smoothing
    I2C_Write(mpu->I2C_Addr, 0x1A, 0x06);
    
    // Divide the sample rate to 100Hz so we don't overwhelm the STM32
    I2C_Write(mpu->I2C_Addr, 0x19, 0x09);
}

// Probably want to calibrate 
void MPU6050_Calibrate(MPU6050_t *mpu) {
    int32_t sum_accel_x = 0, sum_accel_y = 0, sum_accel_z = 0;
    int32_t sum_gyro_x = 0, sum_gyro_y = 0, sum_gyro_z = 0;

    LL_GPIO_SetOutputPin(Green_LED_GPIO_Port, Green_LED_Pin);
    
    int num_samples = 1000;
    uint8_t buffer[14];

    // Disregard the first 100 readings for the sensor to settle.
    for (int i = 0; i < 100; i++) {
        I2C_Burst_Read(mpu->I2C_Addr, MPU6050_ACCEL_XOUT_H, buffer, 14);
        LL_mDelay(10);
    }
    // Take in 1000 readings for calibration
    for (int i = 0; i < num_samples; i++) {
        I2C_Burst_Read(mpu->I2C_Addr, MPU6050_ACCEL_XOUT_H, buffer, 14);

        sum_accel_x += (int16_t)((buffer[0] << 8) | buffer[1]);
        sum_accel_y += (int16_t)((buffer[2] << 8) | buffer[3]);
        sum_accel_z += (int16_t)((buffer[4] << 8) | buffer[5]);

        sum_gyro_x += (int16_t)((buffer[8] << 8) | buffer[9]);
        sum_gyro_y += (int16_t)((buffer[10] << 8) | buffer[11]);
        sum_gyro_z += (int16_t)((buffer[12] << 8) | buffer[13]);

        LL_mDelay(10); // Delay pulling so that we don't oversample more than the sensor can read.
    }

    mpu->Accel_X_Offset = sum_accel_x / num_samples;
    mpu->Accel_Y_Offset = sum_accel_y / num_samples;
    // 1g = 4096. We want to offset the error and keep gravity.
    // We subtract 4096 so that we don't average out gravity in our offset.
    mpu->Accel_Z_Offset = (sum_accel_z / num_samples) - 4096;

    mpu->Gyro_X_Offset = sum_gyro_x / num_samples;
    mpu->Gyro_Y_Offset = sum_gyro_y / num_samples;
    mpu->Gyro_Z_Offset = sum_gyro_z / num_samples;

    LL_GPIO_ResetOutputPin(Green_LED_GPIO_Port, Green_LED_Pin);
}

void Read_I2C_Acceleration(MPU6050_t *mpu) {

    // MPU6050_ACCEL_XOUT_H = 0x3B
    // MPU6050_ACCEL_XOUT_L = 0x3C
    // MPU6050_ACCEL_YOUT_H = 0x3D
    // MPU6050_ACCEL_YOUT_L = 0x3E
    // MPU6050_ACCEL_ZOUT_H = 0x3F
    // MPU6050_ACCEL_ZOUT_L = 0x40

    uint8_t buffer[6];

    I2C_Burst_Read(mpu->I2C_Addr, MPU6050_ACCEL_XOUT_H, buffer, 6);

    int16_t Raw_Accel_X = (int16_t)((buffer[0] << 8) | buffer[1]) - mpu->Accel_X_Offset;

    int16_t Raw_Accel_Y = (int16_t)((buffer[2] << 8) | buffer[3]) - mpu->Accel_Y_Offset;

    int16_t Raw_Accel_Z = (int16_t)((buffer[4] << 8) | buffer[5]) - mpu-> Accel_Z_Offset;

    // Using 4096.0f because the sensor is configured for +/- 8g
    mpu->Accel_XG = (float)Raw_Accel_X / 4096.0f;
    mpu->Accel_YG = (float)Raw_Accel_Y / 4096.0f;
    mpu->Accel_ZG = (float)Raw_Accel_Z / 4096.0f;
}

void Read_I2C_Gyroscope(MPU6050_t *mpu) {
    uint8_t buffer[6];
    I2C_Burst_Read(mpu->I2C_Addr, MPU6050_GYRO_XOUT_H, buffer, 6);

    int16_t Raw_Gyro_X = (int16_t)((buffer[0] << 8) | buffer[1]) - mpu->Gyro_X_Offset;
    int16_t Raw_Gyro_Y = (int16_t)((buffer[2] << 8) | buffer[3]) - mpu->Gyro_Y_Offset;
    int16_t Raw_Gyro_Z = (int16_t)((buffer[4] << 8) | buffer[5]) - mpu->Gyro_Z_Offset;

    mpu->Roll = (float)Raw_Gyro_X / 65.5f;
    mpu->Pitch = (float)Raw_Gyro_Y / 65.5f;
    mpu->Yaw = (float)Raw_Gyro_Z / 65.5f;

}