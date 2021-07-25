/*
 * timer_config.h
 *
 *  Created on: 2021Äê7ÔÂ25ÈÕ
 *      Author: FYT
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#include "MSP432P4xx/driverlib.h"

void PWMconfig(int TIMER_PERIOD, int DUTY_CYCLE);
void TimerCaptureconfig(void);


#endif /* TIMER_CONFIG_H_ */
