/*
 * i2c.h
 *
 *  Created on: Oct 24, 2018
 *      Author: krad2
 */

#ifndef I2C_H_
#define I2C_H_

#include <ti/drivers/I2C.h>
#include "Board.h"

#include "c_imports.h"

// Hardware I2C module on the board
I2C_Handle i2c_master;
I2C_Transaction i2c_transaction;
#define LCD_ADDR         0x78>>1
#define LCD_CTRL_REG     0x00  /* Die Temp Result Register */
#define LCD_DATA_REG     0x40  /* Die Temp Result Register */

#ifndef Board_LCD_ADDR
#define Board_LCD_ADDR       LCD_ADDR
#endif

// Class private constants and definition
// ----------------------------------------------------------------------------
#define     CMD_DELAY            1  // Command delay in miliseconds
#define     CHAR_DELAY           0  // Delay between characters in miliseconds
#define     PIXEL_ROWS_PER_CHAR  8  // Number of pixel rows in the LCD character
#define     MAX_USER_CHARS       16 // Maximun number of user defined characters

// LCD Command set
#define DISP_CMD        0x0  // Command for the display
#define RAM_WRITE_CMD   0x40 // Write to display RAM
#define CLEAR_DISP_CMD  0x01 // Clear display command
#define HOME_CMD        0x02 // Set cursos at home (0,0)
#define DISP_ON_CMD     0x0C // Display on command
#define DISP_OFF_CMD    0x08 // Display off Command
#define SET_DDRAM_CMD   0x80 // Set DDRAM address command
#define CONTRAST_CMD    0x70 // Set contrast LCD command
#define FUNC_SET_TBL0   0x38 // Function set - 8 bit, 2 line display 5x8, inst table 0
#define FUNC_SET_TBL1   0x39 // Function set - 8 bit, 2 line display 5x8, inst table 1

// LCD bitmap definition
#define CURSOR_ON_BIT   ( 1 << 1 )// Cursor selection bit in Display on cmd.
#define BLINK_ON_BIT    ( 1 << 0 )// Blink selection bit on Display on cmd.


// Sets up hardware I2C
void init_i2c();
void initLCD();
void command(uint8_t value);
void setCursor(uint8_t line_num, uint8_t x);
void writeToLCD( unsigned char *text);
void clear();

#endif
