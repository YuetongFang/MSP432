#include "usr.h"
#include "msp.h"

/**
 * notes:
 * functions with "MAP_" would reduce the power consumption
 * differences between MSP430 and MSP432 ware mostly exist in the interrupt functions
 */

/* £¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡
 * the configs for clocks is in clocks_config.c
 * there should be a clocks_init before all the following processings
 */
void main(void){
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /***MUST HAVE!!!***/
    clocks_init();

    //delay_init_us(); //only for delay
    setup_timer(); //for delay and get_ms

    OLED_Init();
    OLED_Clear();

    Key_Init();
    int KeyValue;//for the keyboard

    UART2_INIT();

    int num = 0;

    while(1){
            OLED_ShowNum(0,0,num,1,size);
            num++;
            delay_ms(1000);
    }
}
