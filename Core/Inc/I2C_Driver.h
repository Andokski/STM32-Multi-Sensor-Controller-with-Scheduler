#ifndef I2C_Driver_H
#define I2C_Driver_H

#include "main.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_gpio.h"
#include <stdint.h>

uint8_t I2C_Read(uint8_t sensor_addr, uint8_t register_addr);
uint8_t I2C_Write(uint8_t sensor_addr, uint8_t register_addr, uint8_t data);
void I2C_Burst_Read(uint8_t sensor_addr, uint8_t register_addr, uint8_t *buffer, uint8_t size);

#endif /* I2C_Driver_H */