/*
 * timer_config.h
 *
 *  Created on: 2021��7��25��
 *      Author: FYT
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#include "MSP432P4xx/driverlib.h"

void PWMconfig(int TIMER_PERIOD, int DUTY_CYCLE);
void TimerCaptureconfig(void);


#endif /* TIMER_CONFIG_H_ */
