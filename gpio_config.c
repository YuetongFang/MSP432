/*
 * gpio_config.c
 *
 *  Created on: 2021Äê7ÔÂ24ÈÕ
 *      Author: FYT
 *      for all the pin definitions and configurations
 */

#include "gpio_config.h"


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
