/*
 * delay.h
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
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
void setup_timer(void);
int  get_ms(unsigned long *count);
void SysTick_Handler(void);

#endif /* DELAY_H_ */
