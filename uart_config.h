/*
 * uart_config.h
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
 *      Author: FYT
 *
 */

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

#include "MSP432P4xx/driverlib.h"
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

void UART2_INIT(void);
void putchar(uint32_t moduleInstance, uint8_t charac);
void putstr (uint32_t moduleInstance, unsigned char *send_buf);
void puth   (uint32_t moduleInstance, unsigned n);

void UART_RX(void);
void UART_TX(void);

void uart_printf(uint32_t moduleInstance, char *format, ...);

#endif /* UART_CONFIG_H_ */
