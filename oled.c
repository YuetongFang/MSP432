/*
 * oled.c
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
 *      Author: FYT
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "oled.h"
#include "oled_font.h"
#include "delay.h"

/*notes:
 * OLED_CLear should be called after OLED_Init
 */

//OLED storing format
//[0--7]0 1 2 3 ... 127

/**********************************************
//IIC Start
**********************************************/
void IIC_Start(){
    OLED_SCLK_Set();
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(){
    OLED_SCLK_Set() ;
    //OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

void IIC_Wait_Ack(){
    OLED_SCLK_Set() ;
    OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m,da;
    da = IIC_Byte;
    OLED_SCLK_Clr();
    for(i=0;i<8;i++){
        m = da;
        //  OLED_SCLK_Clr();
        m = m & 0x80;
        if(m == 0x80){
            OLED_SDIN_Set();
        }
        else OLED_SDIN_Clr();
        da = da<<1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
        }
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command){
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
   IIC_Wait_Ack();
   Write_IIC_Byte(0x00);            //write command
   IIC_Wait_Ack();
   Write_IIC_Byte(IIC_Command);
   IIC_Wait_Ack();
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data){
   IIC_Start();
   Write_IIC_Byte(0x78);            //D/C#=0; R/W#=0
   IIC_Wait_Ack();
   Write_IIC_Byte(0x40);            //write data
   IIC_Wait_Ack();
   Write_IIC_Byte(IIC_Data);
   IIC_Wait_Ack();
   IIC_Stop();
}

void OLED_WR_Byte(unsigned dat,unsigned cmd){
    if(cmd){
        Write_IIC_Data(dat);
    }
    else {
        Write_IIC_Command(dat);
    }
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data){
    unsigned char m,n;
    for(m=0;m<8;m++){
        OLED_WR_Byte(0xb0+m,0);     //page0-page1
        OLED_WR_Byte(0x00,0);       //low column start address
        OLED_WR_Byte(0x10,0);       //high column start address
        for(n=0;n<128;n++){
                OLED_WR_Byte(fill_Data,1);
        }
    }
}

void OLED_Set_Pos(unsigned char x, unsigned char y){
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

void OLED_Display_On(void){
    OLED_WR_Byte(0X8D,OLED_CMD);
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

void OLED_Display_Off(void){
    OLED_WR_Byte(0X8D,OLED_CMD);
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

void OLED_Clear(void){
    uint8_t i,n;
    for(i=0;i<8;i++){
        OLED_WR_Byte (0xb0+i,OLED_CMD);
        OLED_WR_Byte (0x00,OLED_CMD);
        OLED_WR_Byte (0x10,OLED_CMD);
        for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
    } //refresh --> black
}
void OLED_On(void){
    uint8_t i,n;
    for(i=0;i<8;i++){
        OLED_WR_Byte (0xb0+i,OLED_CMD);
        OLED_WR_Byte (0x00,OLED_CMD);
        OLED_WR_Byte (0x10,OLED_CMD);
        for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
    }
}

//x:0~127; y:0~63; size:16/12
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size){
    unsigned char c=0,i=0;
    c=chr-' ';
    if(x>Max_Column-1){
        x=0;y=y+2;
    }
    if(Char_Size ==16){
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
        OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
        OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
        }
        else {
            OLED_Set_Pos(x,y);
            for(i=0;i<6;i++)
            OLED_WR_Byte(F6x8[c][i],OLED_DATA);
        }
}

//m^n
unsigned long oled_pow(unsigned char m,unsigned char n){
  unsigned long result=1;
  while(n--)result*=m;
  return result;
}

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Char_Size){
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++){
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1)){
            if(temp==0){
                OLED_ShowChar(x+(Char_Size/2)*t,y,' ',Char_Size);
                continue;
            }else enshow=1;
        }
        OLED_ShowChar(x+(Char_Size/2)*t,y,temp+'0',Char_Size);
    }
}

void OLED_ShowVI(uint8_t x,uint8_t y,uint32_t num,uint8_t Char_Size){
    OLED_ShowNum(x+Char_Size*4-1,y,num%10,1,Char_Size);
    OLED_ShowNum(x+Char_Size*3-1,y,num/10%10,1,Char_Size);
    OLED_ShowNum(x+Char_Size*2-1,y,num/100%10,1,Char_Size);
    OLED_ShowString(x+Char_Size*1-1,y,".",Char_Size);
    OLED_ShowNum(x-1,y,num/1000,1,Char_Size);
}

void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size){
    unsigned char j=0;
    while (chr[j]!='\0'){
        OLED_ShowChar(x,y,chr[j],Char_Size);
        x += 8;
        if(x>120){
            x=0;y+=2;
        }
        j++;
    }
}


void OLED_Init(void){
    OLED_GPIO_CONFIG();
    delay_ms(200);
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
    OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
    OLED_WR_Byte(0x81,OLED_CMD); // contract control
    OLED_WR_Byte(0xFF,OLED_CMD);//--128
    OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
    OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
    OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
    OLED_WR_Byte(0x00,OLED_CMD);//

    OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
    OLED_WR_Byte(0x80,OLED_CMD);//

    OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05,OLED_CMD);//

    OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    OLED_WR_Byte(0xF1,OLED_CMD);//

    OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
    OLED_WR_Byte(0x12,OLED_CMD);//

    OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
    OLED_WR_Byte(0x30,OLED_CMD);//

    OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
    OLED_WR_Byte(0x14,OLED_CMD);//

    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}
