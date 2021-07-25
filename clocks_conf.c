/*
 * clocks_conf.c
 *
 *  Created on: 2021Äê7ÔÂ24ÈÕ
 *      Author: FYT
 */

#include "clocks_conf.h"

/*notes:
 * VIEW:     ./msp432p_series/MSP432P4xx%20Family%20Technical%20Reference%20Manual.pdf
 * P292 for a brief introduction of the clock sources
 * LFXTCLK   32.768k        |LPM3/LPM3.5
 * REFOCLK   32.768k/128k   |LPM3/LPM3.5 CS_setReferenceOscillatorFrequency CS_REFO_32KHZ | CS_REFO_128KHZ
 * VLOCLK    10k            |LPM3/LPM3.5
 * DCOCLK                   |LPM3/LPM4/LPM3.5/LPM4.5
 *           frequency selected by DCORSEL (view TABLE 5.4
 * MODCLK    24M            |LPM3/LPM4/LPM3.5/LPM4.5
 * HFXTCLK   1MHz-48MHz(max |LPM3/LPM4/LPM3.5/LPM4.5
 * SYSOSC    5MHz
 *
 * FOR USERs:
 * ACLK      maximum: 128kHz                        | LFXTCLK VLOCLK REFOCLK
 * MCLK      CPU & peripheral modules               | LFXTCLK VLOCLK REFOCLK DCOCLK MODCLK,HFXTCLK
 * HSMCLK    subsystem clock (individual PMs        | LFXTCLK VLOCLK REFOCLK DCOCLK MODCLK HFXTCLK
 * SMCLK     LOW-speed subsystem clock limited to 1/2 maximum HSMCLK (divided from HSMCLK) | Default:3M
 * BCLK      L-S backup domain clock (maximum 32k   | LFXTCLK REFOCLK
 *           for low power mode
 * externals: VLOCLK REFOCLK LFXTCLK MODCLK SYSCLK
 */

/*notes for the functions:
 *VIEW:     https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP432_Driver_Library/latest/exports/driverlib/msp432_driverlib_3_21_00_05/doc/MSP432P4xx/html/driverlib_html/group__cs__api.html
 *typically: to set LFXT and HFXT, use:
 *           void CS_setExternalClockSourceFrequency(uint32_t lfxt_XT_CLK_frequency,uint32_t hfxt_XT_CLK_frequency )
 */

void mclk_48M(){
/*configuring pins for peripheral/crystal usage for output
 *            |      PJ.3 HFXTIN |---------
 *            |                  |         |
 *            |                  |     < 48Mhz xTal >
 *            |                  |         |
 *            |     PJ.2 HFXTOUT |---------
 */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
/*
 *            |     PJ.0 LFXIN   |---------
 *            |                  |         |
 *            |                  |     < 32khz xTal >
 *            |                  |         |
 *            |     PJ.1 LFXOUT  |---------
 */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    CS_setExternalClockSourceFrequency(32000,48000000);//LFXT 32K, HFXT 48M

    /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);//config the flash(for programming etc.) to match the clk, must have, or it won't work
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    CS_startHFXT(false);//disable Bypass mode

    CS_startLFXT(false);//disable Bypass mode

    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);//MCLK 48M
}

void systick_1us(){
    //set the corresponding MCLK freq
    mclk_48M();

    //enable systick
    MAP_SysTick_enableModule();
    //SysTick counter and must be between 1 and 16,777,216, inclusive.
    SysTick_setPeriod(48);//counter value = t*clk
}

void systick_interrupt(){
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
}


void SysTick_Handler(){

}

//config the clocks for MCLK:48M; SMCLK:24M; ACLK:128k; HSMCLK: 48M
void clocks_init(){
    mclk_48M();//HFXTCLK:48M; LFXTCLK:32k;
    CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);//HSMCLK : 48M
    CS_initClockSignal(CS_SMCLK , CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_2);//SMCLK  : 24M

    CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);//REFOCLK:128k
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);//ACLK:128k
}
