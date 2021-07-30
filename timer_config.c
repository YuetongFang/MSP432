/*
 * timer_config.c
 *
 *  Created on: 2021Äê7ÔÂ25ÈÕ
 *      Author: FYT
 */
#include "timer_config.h"
#include "uart_config.h"
#include "stdio.h"
#include "interrupt_config.h"

//Timer counter--> Max: 65535

#define Clock_freq 24000000

//global variables for further calculation
long    Edge            = 0;
long    PreviousEdge    = 0;
int     int_time        = 0;
double  Time            = 0;
double  Freq            = 0;
double  Duty            = 0;
long    Count_Val       = 0;

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfig ={

};

/* Timer_A Continuous Mode Configuration Parameter */
Timer_A_ContinuousModeConfig continuousModeConfig ={

};

/* Timer_A Capture Mode Configuration Parameter */
Timer_A_CaptureModeConfig captureModeConfig ={

};

/* Tx.y; x:TIMER_Ax; y:capturecompare_register_y */

void Timer_32_conf(){
    /* Timer: 0, 1 | PreScaler: 1, 16,256 |Resolution :16, 32 | Mode: FREE_RUN_MODE, PERIODIC_MODE
     * FREE_RUN_MODE: The value of the timer is reset to UINT16_MAX (for 16-bit mode)
     *                or UINT32_MAX (for 16-bit mode) when the timer reaches zero
     * PERIODIC_MODE: The timer is reset to the value set by the Timer32_setCount function
     */
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,TIMER32_PERIODIC_MODE);
    /* Timer, Count */
    MAP_Timer32_setCount(TIMER32_0_BASE,128000);
    /* Timer32_setCountInBackground(timer,count);
     * Sets the count of the timer without resetting the current value.
     * When the current value of the timer reaches zero,
     * the value passed into this function will be set as the new count value.
     */

    //TIMER32_0_INTERRUPT| TIMER32_1_INTERRUPT| TIMER32_COMBINED_INTERRUPT
    MAP_Timer32_enableInterrupt(TIMER32_0_BASE); //T32_INT1_IRQHandler |T32_INT2_IRQHandler | T32_INTC_IRQHandler
    MAP_Timer32_startTimer(TIMER32_0_BASE, true);

    //MAP_Timer32_haltTimer(TIMER32_0_BASE);
}

void TimerA_PWM_Init(){
    /* Configuring GPIO2.4 as peripheral output for PWM | P2.4--> TA0.1 */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
            GPIO_PRIMARY_MODULE_FUNCTION);

    pwmConfig.clockSource       =TIMER_A_CLOCKSOURCE_SMCLK;
    pwmConfig.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_1;
    pwmConfig.compareOutputMode =TIMER_A_OUTPUTMODE_RESET_SET;
    pwmConfig.compareRegister   =TIMER_A_CAPTURECOMPARE_REGISTER_1;
    pwmConfig.dutyCycle         =0;
    pwmConfig.timerPeriod       =0;

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

void TimerA_PWM_settings(float frequency, float duty_ratio){
    pwmConfig.timerPeriod =Clock_freq/frequency;
    pwmConfig.dutyCycle   =duty_ratio*Clock_freq/frequency;

    //P2.4--> TA0.1
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

void TimerA_capture_init(){
    /* **************Counter config ***************** */
    /* Configuring Timer A2 as Continuous Mode */
    continuousModeConfig.clockSource              =TIMER_A_CLOCKSOURCE_SMCLK;
    continuousModeConfig.clockSourceDivider       =TIMER_A_CLOCKSOURCE_DIVIDER_1;
    continuousModeConfig.timerClear               =TIMER_A_SKIP_CLEAR;
    continuousModeConfig.timerInterruptEnable_TAIE=TIMER_A_TAIE_INTERRUPT_ENABLE;
    MAP_Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);

    /* ************Capture Mode config************** */
    /* Clear TA2 capture interrupt */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4);
    /* Configure a pull up resistor for the capture GPIO*/
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6,GPIO_PIN7);
    /* Configuring GPIO6.7 as peripheral output for PWM | P6.7-->TA2.4 */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN7,
            GPIO_PRIMARY_MODULE_FUNCTION);
    captureModeConfig.captureInputSelect          =TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    captureModeConfig.captureInterruptEnable      =TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    captureModeConfig.captureMode                 =TIMER_A_CAPTUREMODE_RISING_EDGE;
    captureModeConfig.captureOutputMode           =TIMER_A_OUTPUTMODE_OUTBITVALUE;
    captureModeConfig.captureRegister             =TIMER_A_CAPTURECOMPARE_REGISTER_4;
    captureModeConfig.synchronizeCaptureSource    =TIMER_A_CAPTURE_SYNCHRONOUS;
    /* Configuring Capture Mode */
    MAP_Timer_A_initCapture(TIMER_A2_BASE, &captureModeConfig);

    /* Start TA2 counting */
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);
}

/* TAxIV: Timer_A interrupt vector value
 * 00H --> No interrupt pending         | 02H  Capture/compare1: TAxCCR1 CCIFG | 04H  Capture/compare2: TAxCCR2 CCIFG
 * ..............
 * 0CH  Capture/compare2: TAxCCR6 CCIFG | 0EH   Timer overflow;Interrupt Flag: TAxCTL TAIFG (Lowest)
 * To find out which Interrupt is pending:
 * Use: TAxIV & xx(vector No.)
 */
void TA2_N_IRQHandler(void){
    static int overflow = 0;
    //Judge if TA2.4 capture interrupt is pending
    uint8_t Interrupt_Source = __even_in_range(TA2IV,0x1E);
    switch(Interrupt_Source){
          case  0x0: break;                     // Vector  0:  No interrupt
          case  0x2: break;                     // Vector  2:  TACCR0 CCIFG
          case  0x4: break;                     // Vector  4:  TACCR2 CCIFG
          case  0x6: break;                     // Vector  6:  TACCR3 CCIFG
          case  0x8:                            // Vector  8:  TACCR4 CCIFG
              Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4);
              //Get the current counter value
              Edge = Timer_A_getCaptureCompareCount(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4);

              //Get the retenstion time(considering the overflows)
              Count_Val = (long)(65536*overflow - PreviousEdge + Edge);
              if(Count_Val < 0)
                  Count_Val = Count_Val+65536;
              //Retention time calculation
              //Time = (Count_Val + 1) / Clock_freq;
              Freq = Clock_freq / Count_Val;
              PreviousEdge = Edge;
              overflow=0;
            break;
          case 0xA: break;                      // Vector 10:  TACCR5 CCIFG
          case 0xC: break;                      // Vector 12:  TACCR5 CCIFG
          //for handling the overflow and interrupt
          case 0xE:
              Timer_A_clearTimerInterrupt(TIMER_A2_BASE);
              overflow++;
              break;
          default:  break;
      }

}
