#include "I2C_Driver.h"

// --- I2C FUNCTIONS ---
uint8_t I2C_Read(uint8_t sensor_addr, uint8_t register_addr) {
    // I2C read function for BMP280 sensor
    // OVERVIEW:
    // Configure the I2C peripheral to read a single byte from the given device address and register address.
    // Transmit and Receive data. Then lower the stop flag and reset the peripheral for the next set of instructions.
    // Configure I2C peripheral -> Activates start condition and sends out the desired device address and the read/write instruction.
    // Transmits the 
    
    // This function configures and starts an I2C transaction.
    // It tells the microcontroller, who to talk to, how much to send or receive, which direction we are sending in, how to start, and how to finish.
    LL_I2C_HandleTransfer(I2C1, sensor_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) {
        // Wait until transmit interrupt status is set
    }
    LL_I2C_TransmitData8(I2C1, register_addr); // Send register address

    LL_I2C_HandleTransfer(I2C1, sensor_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

    while(!LL_I2C_IsActiveFlag_RXNE(I2C1)) {
        // Wait until receive buffer is not empty
    }
    uint8_t data_in = LL_I2C_ReceiveData8(I2C1); // Read the data from the sensor

    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {
        // Wait until stop condition is detected
    }
    // Even though we are done with the I2C transaction, we need to clear the STOP flag before we can start another transaction.
    // Clearing the stop flag allows our I2C peripheral by clearing the flag and reseting the I2C peripheral to be ready for the next transaction.
    LL_I2C_ClearFlag_STOP(I2C1);

    return data_in;
}

uint8_t I2C_Write(uint8_t sensor_addr, uint8_t register_addr, uint8_t data) {
    // I2C write function for BMP280 sensor
    // OVERVIEW:
    
    // This function configures and starts an I2C transaction.
    // It tells the microcontroller, who to talk to, how much to send or receive, which direction we are sending in, how to start, and how to finish.
    // Note: TransferSize indicates how much data(bytes) we are going to transmit or read to the sensor.
    LL_I2C_HandleTransfer(I2C1, sensor_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

    while(!LL_I2C_IsActiveFlag_TXIS(I2C1)) { 
        // Wait until transmit interrupt status is set
    }
    LL_I2C_TransmitData8(I2C1, register_addr);

    while(!LL_I2C_IsActiveFlag_TXIS(I2C1)) {
        // Wait until transmit interrupt status is set
    }
    LL_I2C_TransmitData8(I2C1, data); 

    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {

    }
    LL_I2C_ClearFlag_STOP(I2C1);
    return 0;
}

void I2C_Burst_Read(uint8_t sensor_addr, uint8_t register_addr, uint8_t *buffer, uint8_t size) {

    // Phase 1: Set the I2C bus into write mode
    LL_I2C_HandleTransfer(I2C1, sensor_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    // Phase 2: Send out instruction / register address
    while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) {
        // Wait until transmit interrupt status is set
    }
    LL_I2C_TransmitData8(I2C1, register_addr); // Send register address

    // Phase 3: Set the I2C bus into read mode
    LL_I2C_HandleTransfer(I2C1, sensor_addr << 1, LL_I2C_ADDRSLAVE_7BIT, size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

    // Phase 4: Burst read the given size of data. The sensor will auto-increment the registers.
    for (uint8_t i = 0; i < size; i++) {
        while(!LL_I2C_IsActiveFlag_RXNE(I2C1)) {
            // Wait until receive buffer is not empty
        }
        buffer[i] = LL_I2C_ReceiveData8(I2C1);
    }

    // Phase 5: Stop condition
    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {
        // Wait until stop condition is detected
    }

    LL_I2C_ClearFlag_STOP(I2C1);
}