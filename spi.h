/*
 * spi.h
 *
 *  Created on: Nov 16, 2018
 *      Author: krad2
 */

#ifndef SPI_H_
#define SPI_H_

#include <ti/drivers/SPI.h>
#include "Board.h"
#include "c_imports.h"
#include "fixed_point.h"
#include "gpio.h"

// Addresses register by index
#define REG(x) ((x) << 3)

// Read command flags
#define READ_MODE 0x40
#define CONTINUOUS_READ 0x04

// Commands to read / write to a register
#define WRITE_CMD(x) REG(x)
#define READ_CMD(x) (READ_MODE | REG(x))

// Flags for extended resolution and conversion mode
#define EXT_RES 0x80
#define MODE(x) ((x) << 5)

// Conversion modes
#define CONTINUOUS MODE(0b00)
#define ONESHOT MODE(0b01)
#define SPS MODE(0b10)
#define SHUTDOWN MODE(0b11)

// Read temperature commands
#define READ_TEMP READ_CMD(0b010)
#define READ_TEMP_CONT (READ_TEMP | CONTINUOUS)

// NULL values for the sensor
#define RESET 0xFF
#define DUMMY RESET

// Register indices
#define STATUS_REGISTER 0
#define CONFIG_REGISTER 1
#define TEMP_REGISTER 2
#define ID_REGISTER 3
#define T_CRIT_REGISTER 4
#define T_HYST_REGISTER 5
#define T_HIGH_REGISTER 6
#define T_LOW_REGISTER 7

#define BYTE_SIZE 8
#define BUS_FREQ 100000

#define RESET_COOLDOWN 500
#define CONVERSION_WAIT 250000

SPI_Handle sensor;

void transfer(size_t frame_count, void *read_buf, void *write_buf);
int32_t read_spi();

void init_spi();
void reset_spi();

#endif /* SPI_H_ */
