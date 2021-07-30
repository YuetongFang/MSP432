/*
 * MPU_IIC.c
 *
 *  Created on: 2021Äê7ÔÂ26ÈÕ
 *      Author: Administrator
 */
#include "MPU_IIC.h"
#include "mpu6050.h"

void MPU_IIC_Init(void){
    //MPU_GPIO_CONFIG();
    SCL_OUT();
    SDA_OUT();

    IIC_SCL_H();
    IIC_SDA_H();
}

/**********************************************
//IIC Start
**********************************************/
void MPU_IIC_Start(){
    SDA_OUT();
    IIC_SDA_H();    delay_us(1);
    IIC_SCL_H();    delay_us(1);
    IIC_SDA_L();    delay_us(1);
    IIC_SCL_L();    delay_us(1);
}

/**********************************************
//IIC Stop
**********************************************/
void MPU_IIC_Stop(){
    SDA_OUT();
    IIC_SCL_L();    delay_us(1);
    IIC_SDA_L();    delay_us(1);
    IIC_SCL_H();    delay_us(1);
    IIC_SDA_H();    delay_us(1);
}

/**********************************************
//IIC Wait Ack
**********************************************/
uint8_t MPU_IIC_Wait_Ack(){
    uint8_t ucErrTime=0;
    IIC_SDA_H();    delay_us(1);
    SDA_IN();
    IIC_SCL_H();    delay_us(1);

    while(READ_SDA){
        ucErrTime++;
        if(ucErrTime>250){
            MPU_IIC_Stop();
            return 1;
        }
    }

    IIC_SCL_L();
    return 0;
}

/**********************************************
//IIC Ack
**********************************************/
void MPU_IIC_Ack(void){
    IIC_SCL_L();

    SDA_OUT();
    IIC_SDA_L();    delay_us(1);
    IIC_SCL_H();    delay_us(1);

    IIC_SCL_L();

    SDA_IN();
}

/**********************************************
//IIC NAck
**********************************************/
void MPU_IIC_NAck(void){
    IIC_SCL_L();

    SDA_OUT();
    IIC_SDA_H();    delay_us(1);
    IIC_SCL_H();    delay_us(1);

    IIC_SCL_L();

    SDA_IN();
}

uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data){
    MPU_IIC_Start();

    MPU_Write_IIC_Byte(MPU_WRITE);//send slave address and write command
    MPU_IIC_Wait_Ack();


    MPU_Write_IIC_Byte(reg); //send towards reg address
    MPU_IIC_Wait_Ack();     //wait ack

    MPU_Write_IIC_Byte(data);//send data
    if(MPU_IIC_Wait_Ack())  //wait ack
    {
        MPU_IIC_Stop();
        return 1;
    }

    MPU_IIC_Stop();
    return 0;
}

/**********************************************
// IIC Write byte
**********************************************/
void MPU_Write_IIC_Byte(unsigned char IIC_Byte){
    uint8_t t, bBit_W;

    SDA_OUT();
    IIC_SCL_L();

    for(t=0;t<8;t++){

        //IIC_SDA=(IIC_Byte & 0x80) >> 7;
        bBit_W = IIC_Byte & 0x80;
        if(bBit_W)  IIC_SDA_H();
        else IIC_SDA_L();

        IIC_Byte<<=1;
        delay_us(1);
        IIC_SCL_H();    delay_us(1);
        IIC_SCL_L();    delay_us(1);
    }
}

/**********************************************
// IIC Read byte
**********************************************/
uint8_t MPU_IIC_Read_Byte(uint8_t ACK){
    uint8_t i,receive=0;

    Interrupt_disableMaster();
    SDA_IN();
    SCL_OUT();
    for(i=0;i<8;i++){
        IIC_SCL_L();
        delay_us(1);
        IIC_SCL_H();

        receive <<= 1;
        if(READ_SDA)    //receive++;
             receive |= 0x01;
        else receive &= 0xfe;
        delay_us(1);
    }
    if (!ACK)   //Nack
        MPU_IIC_NAck();
    else        //Ack
        MPU_IIC_Ack();

    Interrupt_enableMaster();
    return receive;
}

uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
    MPU_IIC_Start();

    MPU_Write_IIC_Byte((addr<<1)|0);//send slave address + write command
    if(MPU_IIC_Wait_Ack())         //wait for ack
    {
        MPU_IIC_Stop();
        return 1;
    }

    MPU_Write_IIC_Byte(reg); //write to reg address
    MPU_IIC_Wait_Ack();      //wait for ack

    MPU_IIC_Start();
    MPU_Write_IIC_Byte((addr<<1)|1);//send slave address + write command
    MPU_IIC_Wait_Ack();             //wait for ack

    while(len){

        if(len==1)*buf=MPU_IIC_Read_Byte(0);//read data,send NACK
        else      *buf=MPU_IIC_Read_Byte(1);//read data,send ACK
        len--;
        buf++;
    }

    MPU_IIC_Stop(); //Stop
    return 0;
}


uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
    uint8_t i;
    MPU_IIC_Start();
    MPU_Write_IIC_Byte((addr<<1)|0);//send slave address + write command
    if(MPU_IIC_Wait_Ack())              //wait ack
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_Write_IIC_Byte(reg); //write slave address
    MPU_IIC_Wait_Ack();     //wait ack
    for(i=0;i<len;i++)
    {
        MPU_Write_IIC_Byte(buf[i]);  //pending for data
        if(MPU_IIC_Wait_Ack())      //wait ack
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Stop();
    return 0;
}
