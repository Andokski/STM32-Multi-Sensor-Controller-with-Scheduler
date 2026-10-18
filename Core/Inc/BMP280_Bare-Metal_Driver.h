#ifndef BMP280_BAREMETAL_H
#define BMP280_BAREMETAL_H

#include "main.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "SPI_Driver.h"
#include "I2C_Driver.h"


// --- I2C ADDRESS CONSTANTS ---
#define BMP280_I2C_ADDRESS 0x76 // 7-bit address for the BMP Sensor

// --- BMP280 ADDRESS CONSTANTS ---
#define BMP280_ID          0xD0
#define BMP280_RESET       0xE0
#define BMP280_CTRL_MEAS   0xF4
#define BMP280_CONFIG      0xF5
// BMP280 CALIBRATION REGISTER CONSTANTS
// R1 & R2 are both 8-bit registers that comebine to form a 16-bit register. R1 is the MSB and R2 is the LSB.
// When combined they become a 16-bit register that contains data that is used to calibrate the sensor.

#define BMP280_CALIB00     0x88 // 0x88 & 0x89 ==> dig_T1
#define BMP280_CALIB01     0x89
#define BMP280_CALIB02     0x8A // 0x8A & 0x8B ==> dig_T2
#define BMP280_CALIB03     0x8B
#define BMP280_CALIB04     0x8C // 0x8C & 0x8D ==> dig_T3
#define BMP280_CALIB05     0x8D
// The pattern continues for the dig_Px registers.
#define BMP280_CALIB25     0xA1 // Reserved
// TEMPERATURE REGISTER CONSTANTS
#define BMP280_TEMP_MSB    0xFA
#define BMP280_TEMP_LSB    0xFB
#define BMP280_TEMP_XLSB   0xFC
// PRESSURE REGISTER CONSTANTS
#define BMP280_PRESS_MSB   0xF7
#define BMP280_PRESS_LSB   0xF8
#define BMP280_PRESS_XLSB  0xF9

// BMP280 Environment Sensor Configuration Constants
// Section 3.4 of the BMP280 datasheet describes how to configure the sensor.
#define BMP280_INDOOR_CTRL 0xAB // 0b00100111
#define BMP280_INDOOR_CONFIG 0x10 // 0b00010000

typedef struct {
    uint16_t dig_T1;
    uint16_t dig_T2;
    uint16_t dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    int32_t raw_temperature;
    uint32_t raw_pressure;

    float temperature_C;
    float pressure_hPa;
} BMP280_t;


/* 4. FUNCTION PROTOTYPES (The Menu) */
// We copy the exact first line of our functions and end them with a semicolon.

void BMP280_SPI_Config(BMP280_t *bmp); // SPI
void BMP280_I2C_Config(uint8_t sensor_addr, BMP280_t *bmp);  // I2C

void Ctrl_SPI_Meas_Write(uint8_t ctrl_meas); // SPI
void Config_SPI_Write(uint8_t config); // SPI
void Ctrl_I2C_Meas_Write(uint8_t sensor_addr,uint8_t ctrl_meas); // I2C
void Config_I2C_Write(uint8_t sensor_addr, uint8_t config); // I2C

void Read_SPI_Temperature(BMP280_t *bmp);
void Read_I2C_Temperature(BMP280_t *bmp);
void Read_SPI_Pressure(BMP280_t *bmp);

int32_t bmp280_compensate_T_int32(int32_t adc_T, BMP280_t *bmp);
uint32_t bmp280_compensate_P_int32(int32_t adc_P, BMP280_t *bmp);

#endif /* BMP280_BAREMETAL_H */
