#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "main.h"
#include "BMP280_Bare-Metal_Driver.h"
#include "MPU6050_Bare-Metal_Driver.h"
#include "SPI_Driver.h"
#include "I2C_Driver.h"
#include "stm32l4xx.h"

#define MAX_TASKS 2
#define STACK_SIZE 256 // Hold 256 words or 1,024 bytes or 1 KB per task

extern uint8_t scheduler_running;

typedef struct {
    uint32_t *stack_ptr;
} Task_Control_Block_t;

void Scheduler_AddTask(void (*task_func)(void));
void Scheduler_Start(void);


#endif