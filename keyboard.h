/*
 * keyboard.h
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
 *      Author: FYT
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "MSP432P4xx/driverlib.h"
#include "delay.h"

/*****KEYBOARD******
 * ROW0-3: P8.5, P9.0, P8.4, P8.2
 * COW0-3: P8.6, P8.7, P9.1, P8.3
 **************/
//the structure for Key_defines
struct IO_port{
      uint8_t  GPIO_PORT_PX;
      uint16_t GPIO_PINX;};
//PIN DEFIN
//row0-3
static struct IO_port  Key_output[]={{GPIO_PORT_P8,GPIO_PIN5},
                                     {GPIO_PORT_P9,GPIO_PIN0},
                                     {GPIO_PORT_P8,GPIO_PIN4},
                                     {GPIO_PORT_P8,GPIO_PIN2}};
//col0-3
static struct IO_port  Key_input[]={ {GPIO_PORT_P8,GPIO_PIN6},
                                     {GPIO_PORT_P8,GPIO_PIN7},
                                     {GPIO_PORT_P9,GPIO_PIN1},
                                     {GPIO_PORT_P8,GPIO_PIN3}};
//PIN CONFIGs
void Key_Init();

uint16_t KEY_Scaning();

#endif /* KEYBOARD_H_ */
