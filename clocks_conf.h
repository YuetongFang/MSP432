/*
 * clocks_conf.h
 *
 *  Created on: 2021��7��24��
 *      Author: Administrator
 */

#ifndef CLOCKS_CONF_H_
#define CLOCKS_CONF_H_

#include "MSP432P4xx/driverlib.h"
#include "msp.h"


void mclk_48M();
void clocks_init();
void systick_1us();
void systick_interrupt();
void SysTick_Handler();

#endif /* CLOCKS_CONF_H_ */
