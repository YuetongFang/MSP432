/*
 * acd_config.h
 *
 *  Created on: 2021Äê7ÔÂ28ÈÕ
 *      Author: FYT
 */

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#include "MSP432P4xx/driverlib.h"
#include "stdio.h"
#include "uart_config.h"


void ADC_Temperature_Init();
void ADC14_IRQHandler(void);

#endif /* ADC_CONFIG_H_ */
