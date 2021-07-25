/*
 * delay.h
 *
 *  Created on: 2021��7��11��
 *      Author: FYT
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

#include "MSP432P4xx/driverlib.h"
#include "msp.h"
#include "clocks_conf.h"

void delay_init_us();
void delay_1us();
void delay_us(uint32_t nus);
void delay_1ms();
void delay_ms(uint32_t nms);


#endif /* DELAY_H_ */
