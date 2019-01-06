/*
 * Rotary Encoder.c
 *
 *  Created on: Nov 28, 2018
 *      Author: krad2
 */
unsigned int idx = 3;
int count = 0;
int setPoint = 25; //25*4
#include <Rotary Encoder.h>
#include "controller-main.h"
void encoder_transition(uint_least8_t index) {
    //printf("%d\n",setPoint);

    //Read the current values of the pins
    uint8_t a = GPIO_read(ENCODER_A);
    uint8_t b = GPIO_read(ENCODER_B);

    // Compute the state of the pins currently
    uint8_t state = (a << 1) | b;

    // Use the new and old states to traverse the table
    idx <<= 2;
    idx |= state;

    // Extract the direction value for the state
    // Update the counter
    count += encoder_lut[idx & 15];

    // Perform the output function on a successful tick

    if (count > 3) {
        setPoint++;
        set_sp(&ctls[0], setPoint);
        count = 0;
    }
    if (count < -3) {
        setPoint--;
        set_sp(&ctls[0], setPoint);
        count = 0;
    }
    if(setPoint<0){
        setPoint=0;
    }
    if(setPoint>150){
            setPoint=150;
        }
}



// Count = 0
// 0000 -> 0010 = +1
// 0010 -> 1011 = +2
// 1011 -> 1101 = +3
// 1101 -> 0100 = +4
