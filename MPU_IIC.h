/*
 * MPU_IIC.h
 *
 *  Created on: 2021Äê7ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef MPU_IIC_H_
#define MPU_IIC_H_

#include <stdint.h>
#include "MSP432P4xx/driverlib.h"
#include "delay.h"

/*****IIC******
 * P4.3 -- SCL
 * P4.4 -- SDA
 **************/
#define MPU_GPIO_CONFIG()  P4DIR |=  BIT3+BIT4
// P4.3--> SCL
#define SCL_OUT()   GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN3)

#define IIC_SCL_H() P4OUT |=  BIT3
#define IIC_SCL_L() P4OUT &=~ BIT3


// P4.4--> SDA
#define SDA_OUT()   GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN4)
#define SDA_IN()    GPIO_setAsInputPin(GPIO_PORT_P4,GPIO_PIN4)

#define IIC_SDA_H() P4OUT |=  BIT4
#define IIC_SDA_L() P4OUT &=~ BIT4

#define READ_SDA    GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4)

void    MPU_IIC_Init   (void);
void    MPU_IIC_Start  ();
void    MPU_IIC_Stop   ();
uint8_t MPU_IIC_Wait_Ack();
void    MPU_IIC_Ack     (void);
void    MPU_IIC_NAck    (void);
void    MPU_Write_IIC_Byte  (unsigned char IIC_Byte);
uint8_t MPU_Write_Byte      (uint8_t reg,uint8_t data);
uint8_t MPU_IIC_Read_Byte   (uint8_t ACK);
uint8_t MPU_Read_Len        (uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t MPU_Write_Len       (uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);


#endif /* MPU_IIC_H_ */
