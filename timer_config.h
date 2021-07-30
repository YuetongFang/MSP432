/*
 * timer_config.h
 *
 *  Created on: 2021Äê7ÔÂ25ÈÕ
 *      Author: FYT
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#include "MSP432P4xx/driverlib.h"
#include "stdint.h"

unsigned int   __even_in_range(unsigned int val, unsigned int range);

void Timer_32_conf();
//P2.4--> TA0.1
void TimerA_PWM_Init();
//freq, duty_ratio
void TimerA_PWM_settings(float frequency, float duty_ratio);
//P6.6--> TA2.3
void TimerA_capture_init();
void TA0_N_IRQHandler(void);

#endif /* TIMER_CONFIG_H_ */
