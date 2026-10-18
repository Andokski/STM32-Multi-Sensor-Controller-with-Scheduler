#include "scheduler.h"
#include "stm32l4xx.h"

Task_Control_Block_t TaskControlBlock_List[MAX_TASKS]; // Task control block keeps track of tasks and their status & addresses
uint32_t task_stacks[MAX_TASKS][STACK_SIZE]; // Reserve space in memory for our stack to exist when building the program. 
uint8_t current_task_id = 0; // Bookmarker
uint8_t task_count = 0;

uint8_t scheduler_running = 0;

// Stack Frame Initailization
void Scheduler_AddTask(void (*task_func)(void)) { 
    if (task_count >= MAX_TASKS) {
        return;
    }

    uint32_t *sp = &task_stacks[task_count][STACK_SIZE - 1]; // points to our memory address of our stack

    *(--sp) = (1 << 24);         // xPSR: Set Thumb bit (Bit 24) to 1 (Required for Cortex-M)
    *(--sp) = (uint32_t)task_func; // PC
    *(--sp) = 0xFFFFFFFE;        // LR

    // Scratch Registers
    *(--sp) = 0x00000000;        // R12
    *(--sp) = 0x00000000;        // R3
    *(--sp) = 0x00000000;        // R2
    *(--sp) = 0x00000000;        // R1
    *(--sp) = 0x00000000;        // R0

    *(--sp) = 0xFFFFFFFD;
    // Variable Registers
    for (int i = 0; i < 8; i++) {
        *(--sp) = 0x00000000;
    }

    TaskControlBlock_List[task_count].stack_ptr = sp;
    task_count++;
}

void Scheduler_Start(void) {
    // Set SysTick interrupt priority to lowest (so it doesn't interrupt critical I2C/SPI ops)
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);

    // Load the first task's stack pointer into the CPU's Process Stack Pointer (PSP)
    __set_PSP((uint32_t)TaskControlBlock_List[0].stack_ptr);

    // Switch CPU from Main Stack Pointer (MSP) to Process Stack Pointer (PSP)
    __set_CONTROL(0x02);
    __ISB(); // Instruction Synchronization Barrier

    //FPU->FPCCR &= ~((1UL  << 31) | (1UL << 30));

    // Start SysTick timer for a 5ms Time Quantum (Assuming 80MHz System Clock)
    SysTick_Config(SystemCoreClock / 200);
bn    
    scheduler_running = 1;

    // Execute the very first task directly
    void (*first_task)(void) = (void (*)(void))task_stacks[0][STACK_SIZE - 3];
    first_task();
}
