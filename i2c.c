/*
 * i2c.c
 *
 *  Created on: Oct 24, 2018
 *      Author: krad2
 */

#include "i2c.h"
// Driver DDRAM addressing
const uint8_t dram_dispAddr [][3] =
{
   { 0x00, 0x00, 0x00 },  // One line display address
   { 0x00, 0x40, 0x00 },  // Two line display address
   { 0x00, 0x10, 0x20 }   // Three line display address
};

void init_i2c(){
    I2C_init();
    I2C_Params      i2cParams;
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.transferCallbackFxn = NULL;
    i2c_master = I2C_open(Board_I2C0, &i2cParams);
    if (i2c_master == NULL) {
        //Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1);
    }
    initLCD ();
}

void initLCD(){
    uint8_t       txBuffer[12];
    txBuffer[0] = LCD_CTRL_REG;
    txBuffer[1] = 0x38;
    txBuffer[2] = 0x38;
    txBuffer[3] = 0x39;
    txBuffer[4] = 0x39;
    txBuffer[5] = 0x14;
    txBuffer[6] = 0x78;
    txBuffer[7] = 0x5E;
    txBuffer[8] = 0x6D;
    txBuffer[9] = 0x0C;
    txBuffer[10] = 0x01;
    txBuffer[11] = 0x06;
    i2c_transaction.slaveAddress = LCD_ADDR;
    i2c_transaction.writeBuf = txBuffer;
    i2c_transaction.writeCount = 12;
    i2c_transaction.readCount = 0;
    i2c_transaction.readBuf = NULL;
    if(!I2C_transfer(i2c_master, &i2c_transaction)){
        printf("error\n");
    }
}

void command(uint8_t value)
{
      uint8_t       txBuffer[2];
      txBuffer[0] = ( DISP_CMD );
      txBuffer[1] = ( value );
      i2c_transaction.slaveAddress = LCD_ADDR;
      i2c_transaction.writeBuf = txBuffer;
      i2c_transaction.writeCount = 2;
      i2c_transaction.readCount = 0;
      i2c_transaction.readBuf = NULL;
      if(!I2C_transfer(i2c_master, &i2c_transaction)){
              printf("error\n");
      }
}


void setCursor(uint8_t line_num, uint8_t x)
{
   uint8_t base = 0x00;

   // If the LCD has been initialised correctly, write to it
   // ------------------------------------------------------
      // set the baseline address with respect to the number of lines of
      // the display
      base = dram_dispAddr[1][line_num];
      base = SET_DDRAM_CMD + base + x;
      command (base);
}

void writeToLCD( unsigned char *text){
    uint8_t       txBuffer[strlen(text)+1];
    txBuffer[0] = 0x40;
    int n = 0;
    for(n=1;n<strlen(text)+1;n++){
        txBuffer[n]=text[n-1];
    }

    i2c_transaction.slaveAddress = LCD_ADDR;
    i2c_transaction.writeBuf = txBuffer;
    i2c_transaction.writeCount = strlen(text)+1;
    i2c_transaction.readCount = 0;
    i2c_transaction.readBuf = NULL;
    if(!I2C_transfer(i2c_master, &i2c_transaction)){
        printf("error\n");
    }
}

void clear()
{
   command (CLEAR_DISP_CMD);
}
