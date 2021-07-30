/*
 * clocks_conf.h
 *
 *  Created on: 2021Äê7ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef CLOCKS_CONF_H_
#define CLOCKS_CONF_H_

#include "MSP432P4xx/driverlib.h"

/*****CLOCKs******
 * PJ.3 -- HFXTIN
 * PJ.2 -- HFXTOUT
 * PJ.0 -- LFXIN
 * PJ.1 -- LFXOUT
 **************/


void mclk_48M();
void clocks_init();
void systick_1us();
void systick_interrupt();
//void SysTick_Handler();

#endif /* CLOCKS_CONF_H_ */
