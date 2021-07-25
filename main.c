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

    delay_init_us();

    OLED_Init();
    OLED_Clear();

    Key_Init();
    int KeyValue;//for the keyboard

    UART2_INIT();

    int num = 0;

    while(1){
        KeyValue=0;
        KeyValue=KEY_Scaning();
        if(KeyValue>0){
            char showing = (char)('0'+KeyValue);
            //delay_ms(100);
            OLED_Clear();
            OLED_ShowChar(0,0,showing,size);
        }
    }
}
