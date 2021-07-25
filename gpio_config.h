/*
 * gpio.h
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
 *      Author: FYT
 *      for all the pin definitions and configurations
 */

#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_

#include "MSP432P4xx/driverlib.h"
#include "msp.h"

/*notes for GPIO usage:
 *
 *
 *
 */


/*****OLED******
 * P4.2 -- SCL
 * P4.1 -- SDA
 **************/
#define OLED_GPIO_CONFIG()  P4DIR |=  BIT1+BIT2
#define OLED_SCLK_Clr()     P4OUT &=~ BIT2  //SCL
#define OLED_SCLK_Set()     P4OUT |=  BIT2

#define OLED_SDIN_Clr()     P4OUT &=~ BIT1  //SDA
#define OLED_SDIN_Set()     P4OUT |=  BIT1

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



#endif /* GPIO_CONFIG_H_ */
