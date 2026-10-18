#include "SPI_Driver.h"

// --- SPI FUNCTIONS ---

// This function reads a register from the BMP280 sensor
uint8_t SPI_Read(uint8_t register_addr) {
    // SPI read function for BMP280 sensor
    // OVERVIEW:
    // For every signal we send to the sensor SPI makes it where we need to recieve something back.
    // Send Address(Read indicator) -> Recieve Garbage
    // Send Garbage -> Recieve Data
  
    uint8_t data_in;
  
    while (LL_SPI_IsActiveFlag_BSY(SPI2)) {
        // Wait until SPI is not busy
    }

    LL_GPIO_ResetOutputPin(SPI_CS_GPIO_Port, SPI_CS_Pin); // Set CS low
    
    while (!LL_SPI_IsActiveFlag_TXE(SPI2)) {
        // Wait until transmit buffer is empty
    }
    LL_SPI_TransmitData8(SPI2, register_addr | 0x80); // Send register address with read bit set
    // | 0x80 sets the MSB to 1 so that the sensor can know that we want to read from the register

    while (!LL_SPI_IsActiveFlag_RXNE(SPI2)) {
        // Wait until receive buffer is not empty
    }
    LL_SPI_ReceiveData8(SPI2); // Read and discard
    
    while (!LL_SPI_IsActiveFlag_TXE(SPI2)) {
        // Wait until transmit buffer is empty
    }
    LL_SPI_TransmitData8(SPI2, register_addr & 0x7F); // Address with read bit cleared

    while (!LL_SPI_IsActiveFlag_RXNE(SPI2)) {
        // Wait until receive buffer is not empty
    }
    data_in = LL_SPI_ReceiveData8(SPI2); // Read the data from the sensor
  
    LL_GPIO_SetOutputPin(SPI_CS_GPIO_Port, SPI_CS_Pin); // Set CS high
    return data_in;
}

uint8_t SPI_Write(uint8_t register_addr, uint8_t data) {
    // SPI write function for BMP280 sensor
    // OVERVIEW:  
    // Transmit a register address with the MSB cleared for write operation, followed by the data byte to be written.
    // Send Address(Write indicator) -> Receive Garbage
    // Send Data -> Receive Garbage

    LL_GPIO_ResetOutputPin(SPI_CS_GPIO_Port, SPI_CS_Pin); // Set CS low

    while(!LL_SPI_IsActiveFlag_TXE(SPI2)) {
        // Wait until transmit buffer is empty
    }
    LL_SPI_TransmitData8(SPI2, register_addr & 0x7F); // Send register address with write bit cleared

    while(!LL_SPI_IsActiveFlag_RXNE(SPI2)) {
        // Wait until receive buffer is not empty
    }
    LL_SPI_ReceiveData8(SPI2); // Read and discard

    while(!LL_SPI_IsActiveFlag_TXE(SPI2)) {
        // Wait until transmit buffer is empty
    }
    LL_SPI_TransmitData8(SPI2, data);
    
    while(!LL_SPI_IsActiveFlag_RXNE(SPI2)) {
        // Wait until receive buffer is not empty
    }
    LL_SPI_ReceiveData8(SPI2); // Read and discard
    
    LL_GPIO_SetOutputPin(SPI_CS_GPIO_Port, SPI_CS_Pin); // Set CS high
    return 0; // Return 0 to indicate success
}

// Debugging function to test direct SPI trasnfer.
// Plug MISO directly into MOSI and send a byte. If the same byte is received, then SPI is working correctly.
// WILL NOT BE USED IN FINAL CODE. ONLY FOR DEBUGGING PURPOSES.
uint8_t SPI_TransferTest(uint8_t txByte) {
	uint8_t data = 0x00;

	LL_GPIO_ResetOutputPin(SPI_CS_GPIO_Port, SPI_CS_Pin);

	//while (!LL_SPI_IsActiveFlag_TXE(SPI2)) {
		LL_SPI_TransmitData8(SPI2, txByte);
	//}

	//while (!LL_SPI_IsActiveFlag_RXNE(SPI2)) {
		data = LL_SPI_ReceiveData8(SPI2);
	//}

	LL_GPIO_SetOutputPin(SPI_CS_GPIO_Port, SPI_CS_Pin);
  // Code works only in debugging. Need SPI_IsActiveFlag_TXE and SPI_IsActiveFlag_RXNE to work in real speed.
	return data;
}