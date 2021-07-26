/*
 * keyboard.c
 *
 *  Created on: 2021Äê7ÔÂ11ÈÕ
 *      Author: FYT
 */

#include "keyboard.h"

//value mapping, can be altered
uint16_t key[4][4] = {1 , 2 , 3 ,  4,
                      5 , 6 , 7 ,  8,
                      9 , 10, 11, 12,
                      13, 14, 15, 16};

void Key_Init(){
    MAP_GPIO_setAsOutputPin(Key_output[0].GPIO_PORT_PX,Key_output[0].GPIO_PINX);
    MAP_GPIO_setAsOutputPin(Key_output[1].GPIO_PORT_PX,Key_output[1].GPIO_PINX);
    MAP_GPIO_setAsOutputPin(Key_output[2].GPIO_PORT_PX,Key_output[2].GPIO_PINX);
    MAP_GPIO_setAsOutputPin(Key_output[3].GPIO_PORT_PX,Key_output[3].GPIO_PINX);

    MAP_GPIO_setDriveStrengthLow(Key_output[0].GPIO_PORT_PX,Key_output[0].GPIO_PINX);
    MAP_GPIO_setDriveStrengthLow(Key_output[1].GPIO_PORT_PX,Key_output[1].GPIO_PINX);
    MAP_GPIO_setDriveStrengthLow(Key_output[2].GPIO_PORT_PX,Key_output[2].GPIO_PINX);
    MAP_GPIO_setDriveStrengthLow(Key_output[3].GPIO_PORT_PX,Key_output[3].GPIO_PINX);

    MAP_GPIO_setAsInputPinWithPullUpResistor(Key_input[0].GPIO_PORT_PX,Key_input[0].GPIO_PINX);
    MAP_GPIO_setAsInputPinWithPullUpResistor(Key_input[1].GPIO_PORT_PX,Key_input[1].GPIO_PINX);
    MAP_GPIO_setAsInputPinWithPullUpResistor(Key_input[2].GPIO_PORT_PX,Key_input[2].GPIO_PINX);
    MAP_GPIO_setAsInputPinWithPullUpResistor(Key_input[3].GPIO_PORT_PX,Key_input[3].GPIO_PINX);
}

uint16_t KEY_Scaning(){
    uint16_t i,j;
    uint16_t KeyVal = 0;

    //pull up all the outputs
    MAP_GPIO_setOutputHighOnPin(Key_output[0].GPIO_PORT_PX,Key_output[0].GPIO_PINX);
    MAP_GPIO_setOutputHighOnPin(Key_output[1].GPIO_PORT_PX,Key_output[1].GPIO_PINX);
    MAP_GPIO_setOutputHighOnPin(Key_output[2].GPIO_PORT_PX,Key_output[2].GPIO_PINX);
    MAP_GPIO_setOutputHighOnPin(Key_output[3].GPIO_PORT_PX,Key_output[3].GPIO_PINX);

    //scan the rows
    for(i=0;i<4;i++){
        MAP_GPIO_setOutputLowOnPin(Key_output[i].GPIO_PORT_PX,Key_output[i].GPIO_PINX);
    //scan the cols
         for(j=0;j<4;j++){
    //if Key detected (Low)
             if(MAP_GPIO_getInputPinValue(Key_input[j].GPIO_PORT_PX,Key_input[j].GPIO_PINX)==GPIO_INPUT_PIN_LOW){
                 delay_ms(5);
    //find the specific key
                if(MAP_GPIO_getInputPinValue(Key_input[j].GPIO_PORT_PX,Key_input[j].GPIO_PINX)==GPIO_INPUT_PIN_LOW){
                    KeyVal=key[i][j];
    //stop until the key release
                    while(MAP_GPIO_getInputPinValue(Key_input[j].GPIO_PORT_PX,Key_input[j].GPIO_PINX)==GPIO_INPUT_PIN_LOW);
                }
                else
                    KeyVal=0;
             }
         }
    //reset
         MAP_GPIO_setOutputHighOnPin(Key_output[i].GPIO_PORT_PX,Key_output[i].GPIO_PINX);
    }

    return KeyVal;
}

