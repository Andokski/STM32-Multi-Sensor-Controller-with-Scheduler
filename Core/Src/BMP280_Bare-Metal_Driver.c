#include "BMP280_Bare-Metal_Driver.h"
#include "SPI_Driver.h"
#include "stm32l476xx.h"
#include <stdint.h>


void BMP280_SPI_Config(BMP280_t *bmp) {
    // Temperature Calibration
    bmp->dig_T1 = (unsigned short int)(SPI_Read(BMP280_CALIB00) | (SPI_Read(BMP280_CALIB01) << 8));
    bmp->dig_T2 = (short int)(SPI_Read(BMP280_CALIB02) | (SPI_Read(BMP280_CALIB03) << 8));
    bmp->dig_T3 = (short int)(SPI_Read(BMP280_CALIB04) | (SPI_Read(BMP280_CALIB05) << 8));

    bmp->dig_P1 = (unsigned short int)(SPI_Read(0x8E) | (SPI_Read(0x8F) << 8));
    bmp->dig_P2 = (short int)(SPI_Read(0x90) | (SPI_Read(0x91) << 8));
    bmp->dig_P3 = (short int)(SPI_Read(0x92) | (SPI_Read(0x93) << 8));
    bmp->dig_P4 = (short int)(SPI_Read(0x94) | (SPI_Read(0x95) << 8));
    bmp->dig_P5 = (short int)(SPI_Read(0x96) | (SPI_Read(0x97) << 8));
    bmp->dig_P6 = (short int)(SPI_Read(0x98) | (SPI_Read(0x99) << 8));
    bmp->dig_P7 = (short int)(SPI_Read(0x9A) | (SPI_Read(0x9B) << 8));
    bmp->dig_P8 = (short int)(SPI_Read(0x9C) | (SPI_Read(0x9D) << 8));
    bmp->dig_P9 = (short int)(SPI_Read(0x9E) | (SPI_Read(0x9F) << 8));
}
// Pressure Calibration
void BMP280_I2C_Config(uint8_t sensor_addr, BMP280_t *bmp) {
    // Temperature Calibration
    bmp->dig_T1 = (unsigned short int)(I2C_Read(sensor_addr, BMP280_CALIB00) | (I2C_Read(sensor_addr, BMP280_CALIB01) << 8));
    bmp->dig_T2 = (short int)(I2C_Read(sensor_addr, BMP280_CALIB02) | (I2C_Read(sensor_addr, BMP280_CALIB03) << 8));
    bmp->dig_T3 = (short int)(I2C_Read(sensor_addr, BMP280_CALIB04) | (I2C_Read(sensor_addr, BMP280_CALIB05) << 8));

    // Pressure Calibration
    bmp->dig_P1 = (unsigned short int)(I2C_Read(sensor_addr, 0x8E) | (I2C_Read(sensor_addr, 0x8F) << 8));
    bmp->dig_P2 = (short int)(I2C_Read(sensor_addr, 0x90) | (I2C_Read(sensor_addr, 0x91) << 8));
    bmp->dig_P3 = (short int)(I2C_Read(sensor_addr, 0x92) | (I2C_Read(sensor_addr, 0x93) << 8));
    bmp->dig_P4 = (short int)(I2C_Read(sensor_addr, 0x94) | (I2C_Read(sensor_addr, 0x95) << 8));
    bmp->dig_P5 = (short int)(I2C_Read(sensor_addr, 0x96) | (I2C_Read(sensor_addr, 0x97) << 8));
    bmp->dig_P6 = (short int)(I2C_Read(sensor_addr, 0x98) | (I2C_Read(sensor_addr, 0x99) << 8));
    bmp->dig_P7 = (short int)(I2C_Read(sensor_addr, 0x9A) | (I2C_Read(sensor_addr, 0x9B) << 8));
    bmp->dig_P8 = (short int)(I2C_Read(sensor_addr, 0x9C) | (I2C_Read(sensor_addr, 0x9D) << 8));
    bmp->dig_P9 = (short int)(I2C_Read(sensor_addr, 0x9E) | (I2C_Read(sensor_addr, 0x9F) << 8));
}

// --- BMP280 SENSOR CONFIGURATION FUNCTIONS ---
// Section 3.4 of BMP280 datasheet


// Indoor configuration:
// Mode: Normal - 11
// Oversampling: Ultra high resolution
// Osrs_p: 16x - 101
// Osrs_t: 2x - 010
// IR Filter Coeff: 16 - 100
// Standby Time: 0.5 ms - 000
// ODR: 26.3 Hz
// BW: 0.55 Hz
// Ctrl_meas register: 0b10101011 = 0xAB
// Config register: 0b00010000 = 0x10

// SPI FUNCTIONS

// Bits 7,6, 5 = Oversampling Temperature
// Bits 4,3, 2 = Oversampling Pressure
// Bits 1,0 = Power Mode
void Ctrl_SPI_Meas_Write(uint8_t ctrl_meas) {
    SPI_Write(BMP280_CTRL_MEAS, ctrl_meas);
}

// Bits 7,6,5 = Standby Time
// Bits 4,3,2 = Filter Coefficient
// Bits 0 = Enables 3-wire SPI when set to 1. See chapter 5.3.
void Config_SPI_Write(uint8_t config) {
    SPI_Write(BMP280_CONFIG, config);
}

// I2C FUNCTIONS
void Ctrl_I2C_Meas_Write(uint8_t sensor_addr,uint8_t ctrl_meas) {
    I2C_Write(sensor_addr, BMP280_CTRL_MEAS, ctrl_meas);
}

void Config_I2C_Write(uint8_t sensor_addr, uint8_t config) {
    I2C_Write(sensor_addr, BMP280_CONFIG, config);
}

// For BMP280 Sensor
void Read_SPI_Temperature(BMP280_t *bmp) {
    uint8_t temp_msb = SPI_Read(BMP280_TEMP_MSB);
    uint8_t temp_lsb = SPI_Read(BMP280_TEMP_LSB);
    uint8_t temp_xlsb = SPI_Read(BMP280_TEMP_XLSB);

    int32_t adc_T = (int32_t)(((uint32_t)temp_msb << 12) | ((uint32_t)temp_lsb << 4) | ((uint32_t)temp_xlsb >> 4));
    
    bmp->raw_temperature = bmp280_compensate_T_int32(adc_T, bmp);
}

void Read_I2C_Temperature(BMP280_t *bmp) {
    uint8_t temp_msb = I2C_Read(BMP280_I2C_ADDRESS, BMP280_TEMP_MSB);
    uint8_t temp_lsb = I2C_Read(BMP280_I2C_ADDRESS, BMP280_TEMP_LSB);
    uint8_t temp_xlsb = I2C_Read(BMP280_I2C_ADDRESS, BMP280_TEMP_XLSB);

    int32_t adc_T = (int32_t)(((uint32_t)temp_msb << 12) | ((uint32_t)temp_lsb << 4) | ((uint32_t)temp_xlsb >> 4));
    
    bmp->raw_temperature = bmp280_compensate_T_int32(adc_T, bmp);
}

void Read_SPI_Pressure(BMP280_t *bmp) {
    uint8_t press_msb = SPI_Read(BMP280_PRESS_MSB);
    uint8_t press_lsb = SPI_Read(BMP280_PRESS_LSB);
    uint8_t press_xlsb = SPI_Read(BMP280_PRESS_XLSB);

    int32_t adc_P = (int32_t)(((uint32_t)press_msb << 12) | ((uint32_t)press_lsb << 4) | ((uint32_t)press_xlsb >> 4));

    bmp->raw_pressure = bmp280_compensate_P_int32(adc_P, bmp);
}

// Functions from BMP280 datasheet
// BMP280_S32_t should be defined as a 32 bit signed integer variable type.

int32_t t_fine;
int32_t bmp280_compensate_T_int32(int32_t adc_T, BMP280_t *bmp) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)bmp->dig_T1 << 1))) * ((int32_t)bmp->dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)bmp->dig_T1)) * ((adc_T >> 4) - ((int32_t)bmp->dig_T1))) >> 12) * ((int32_t)bmp->dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t bmp280_compensate_P_int32(int32_t adc_P, BMP280_t *bmp) {
    int32_t var1, var2;
    uint32_t p;
    
    var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)bmp->dig_P6);
    var2 = var2 + ((var1 * ((int32_t)bmp->dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)bmp->dig_P4) << 16);
    var1 = (((bmp->dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)bmp->dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)bmp->dig_P1)) >> 15);
    
    if (var1 == 0) {
        return 0; // Avoid exception caused by division by zero
    }
    
    p = (((uint32_t)(((int32_t)1048576) - adc_P) - (var2 >> 12))) * 3125;
    
    if (p < 0x80000000) {
        p = (p << 1) / ((uint32_t)var1);
    } else {
        p = (p / (uint32_t)var1) * 2;
    }
    
    var1 = (((int32_t)bmp->dig_P9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(p >> 2)) * ((int32_t)bmp->dig_P8)) >> 13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + bmp->dig_P7) >> 4));
    
    return p;
}