#ifndef SPI_Driver_H
#define SPI_Driver_H

#include "main.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_gpio.h"
#include <stdint.h>

uint8_t SPI_Read(uint8_t register_addr);
uint8_t SPI_Write(uint8_t register_addr, uint8_t data);
uint8_t SPI_TransferTest(uint8_t txByte);

#endif