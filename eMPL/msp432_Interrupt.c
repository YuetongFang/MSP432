/*
 * msp432_Interrupt.c
 *
 *  Created on: 24 jun 2017
 *      Author: Madara
 */

#include "msp432_Interrupt.h"


static void (*interruptRoutine)(void);

int pin_int_cb(void (*cb)(void), unsigned short pin, unsigned char lp_exit){

    setup();
    interruptRoutine = cb;
    return 0;
}



