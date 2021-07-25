/*
 * delay.c
 *
 *  Created on: 2021��7��24��
 *      Author: FYT
 */
#include "delay.h"

#define us_count 48
#define ms_count 48000

/* notes:
 * systick_couter_max: 0xffffff
 * so the max_us_cycles is 0xffffff/48 = 349525-->20bits
 *        max_ms_cycles is 0xffffff/48000 = 349-->3bits
 */

void delay_init_us(){
    MAP_SysTick_enableModule();
}


void delay_1us(){
    uint32_t tmp ;
    SysTick->LOAD = us_count-1;
    SysTick->VAL  = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;    // count down begin

    do{
        tmp = SysTick->CTRL;
    }while( !(tmp & (1<<16) && (tmp & 0x01) ) ); // counting completed: COUNTFLAG==1(CTRL->16) 1->completed ->break
                                                 // ENABLE==1(CTRL->0)(if the systick is open)  \0->closed ->break

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //close SysTick
    SysTick->VAL = 0X00;                         //clear SysTick
}

void delay_us(uint32_t nus){
    while(nus){
        nus--;
        delay_1us();
    }
}

void delay_1ms(){
    uint32_t tmp ;
    SysTick->LOAD = ms_count-1;
    SysTick->VAL  = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;    // count down begin

    do{
        tmp = SysTick->CTRL;
    }while( !(tmp & (1<<16) && (tmp & 0x01) ) ); // counting completed: COUNTFLAG==1(CTRL->16) 1->completed ->break
                                                 // ENABLE==1(CTRL->0)(if the systick is open)  \0->closed ->break

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //close SysTick
    SysTick->VAL = 0X00;
}


void delay_ms(uint32_t nms){
    while(nms){
        nms--;
        delay_1ms();
    }
}

