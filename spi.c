/*
 * spi.c
 *
 *  Created on: Nov 16, 2018
 *      Author: krad2
 */
#include "spi.h"

// Performs an SPI transfer into the read and write buffers for the number of frames
void transfer(size_t frame_count, void *read_buf, void *write_buf) {
    SPI_Transaction transaction;

    // Sets the passed in values
    transaction.count = frame_count;
    transaction.txBuf = (void *) write_buf;
    transaction.rxBuf = (void *) read_buf;

    // Performs SPI transfer
    SPI_transfer(sensor, &transaction);
}

// Performs a read on the SPI sensor
int32_t read_spi() {
    // Sets up a list of commands for the device
    uint8_t num_commands = 3;
    uint8_t command_buffer[num_commands];

    // Address the device with the first command
    // Put the device in 16-bit precision and oneshot mode with the next
    // Ask to read the temperature with the third
    command_buffer[0] = WRITE_CMD(CONFIG_REGISTER);
    command_buffer[1] = EXT_RES | ONESHOT;
    command_buffer[2] = READ_TEMP;

    // Drive CS high before performing the transfer
    GPIO_toggle(CHIP_ENABLE);

    // Issue all 3 commands
    transfer(num_commands, NULL, command_buffer);

    // Set up a buffer to hold temperature values
    uint8_t num_reads = 2;
    uint8_t temperature_buffer[num_reads];
    temperature_buffer[0] = 0;
    temperature_buffer[1] = 0;
    // Set up a dummy values array so the transmit buffer is ignored for the frames
    uint8_t dummy_transmit[num_reads];

    // Transmit buffer needs to contain dummy values for this part of the transaction
    dummy_transmit[0] = DUMMY;
    dummy_transmit[1] = DUMMY;

    // Wait for the chip to get ready
    usleep(CONVERSION_WAIT);

    // Read the temperature samples into the buffer
    transfer(num_reads, temperature_buffer, dummy_transmit);

    // Complete the transaction
    GPIO_toggle(CHIP_ENABLE);

    // Extract the words of the temperature reading
    // Combine to generate the ADC code of the reading
    volatile uint8_t low_word = temperature_buffer[1];
    volatile uint8_t high_word = temperature_buffer[0];
    uint16_t raw_num = (high_word << 8) | low_word;

    // Reinterpret the number as a fixed point number
    int16_t fp_temp = *((int16_t *) &raw_num);

    return (fp32_t) fp_temp;
}

// Resets the SPI interface
void reset_spi() {
    uint8_t reset_count = 4;

    // Array of RESET signals for ADT7310 reset
    uint8_t reset_buffer[reset_count];

    // Fill array
    memset(reset_buffer, RESET, reset_count);

    // Drive CS high before performing a transfer
    GPIO_toggle(CHIP_ENABLE);

    // Transmit 8 reset signals
    transfer(reset_count, NULL, reset_buffer);

    // Complete the transfer here
    GPIO_toggle(CHIP_ENABLE);

    // Wait for the reset cooldown
    usleep(RESET_COOLDOWN);
}

// Initializes SPI objects
void init_spi() {
    SPI_init();

    SPI_Params spiParams;
    SPI_Params_init(&spiParams);                                // Initialize SPI parameters

    spiParams.dataSize = BYTE_SIZE;                             // 8-bit data size
    spiParams.bitRate = BUS_FREQ;                               // 100 kHz bus frequency
    spiParams.frameFormat = SPI_POL0_PHA0;                      // SPI Mode 0

    // Opens board SPI and tests for success
    SPI_Handle handle = SPI_open(Board_SPI0, &spiParams);
    if (handle == NULL) {
        while (1);  // SPI_open() failed
    }

    // Assign the opened handle and reset the interface
    sensor = handle;
    //reset_spi();
}
