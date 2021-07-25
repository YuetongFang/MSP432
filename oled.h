/*
 * oled.h
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
 *      Author: FYT
 */

#ifndef OLED_H_
#define OLED_H_

#include "MSP432P4xx/driverlib.h"
#include "gpio_config.h"
#include "msp.h"

#define OLED_MODE   0
#define SIZE        8
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64

#define OLED_CMD  0
#define OLED_DATA 1

/*PIN_CONFIG_DEFINEs
 * P4.2 -- SCL
 * P4.1 -- SDA
 */

// Functions to control OLED are as follows
void OLED_WR_Byte       (unsigned dat,unsigned cmd);
void OLED_Display_On    (void);
void OLED_Display_Off   (void);
void OLED_Init  (void);
void OLED_Clear (void);
void OLED_DrawPoint     (uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill          (uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar      (uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowNum       (uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Char_Size);
void OLED_ShowVI        (uint8_t x,uint8_t y,uint32_t num,uint8_t Char_Size);
void OLED_ShowString    (uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);
void OLED_Set_Pos   (unsigned char x, unsigned char y);
void fill_picture   (unsigned char fill_Data);
void Picture    ();
void IIC_Start  ();
void IIC_Stop   ();
void Write_IIC_Command  (unsigned char IIC_Command);
void Write_IIC_Data     (unsigned char IIC_Data);
void Write_IIC_Byte     (unsigned char IIC_Byte);
void IIC_Wait_Ack();

#endif
