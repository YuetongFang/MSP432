#include "usr.h"
#include "msp.h"
#include "stdio.h"

/**************MPU6050*******************/
uint16_t tmp;           //temperature
short aacx,aacy,aacz;   //acc_row
short gyrox,gyroy,gyroz;//gyro_row

float pitch,roll,yaw;   //Eura

void OLED_MPUInit();
void OLED_ShowMPU();
/****************************************/

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

    delay_init_us(); //only for delay
    //setup_timer(); //for delay and get_ms

    OLED_Init();
    OLED_Clear();

    Key_Init();
    int KeyValue;//for the keyboard

    UART2_INIT();

    TimerA_PWM_Init();
    TimerA_PWM_settings(1000, 0.5);
    TimerA_capture_init();

    //ADC_Temperature_Init();

    P1DIR |=  BIT0;//LED1
    P1OUT &=~ BIT0;

    P2DIR |=  BIT1;//LED2
    P2OUT &=~ BIT1;

    MPU_Init();
    while(mpu_dmp_init()!=0){
        P2OUT |= BIT1;
    }
    int num = 0;

    P2DIR |=  BIT1;

    /* ******INTERRUPT CONFIGs ******** */
    MAP_Interrupt_enableInterrupt(INT_TA2_N);// TimerA2_Interrupts
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);  // UART2
    //MAP_Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();


    while(1){
            mpu_dmp_get_data (&pitch, &roll, &yaw);
    }
}

void OLED_MPUInit(){
    /* ****** MPU6050 OLED Initialized ****** */
    OLED_ShowString(0,0,"pitch:", size);
    OLED_ShowString(0,1,"yaw  :", size);
    OLED_ShowString(0,2,"roll :", size);

    OLED_ShowString(70,0,".", size);
    OLED_ShowString(70,1,".", size);
    OLED_ShowString(70,2,".", size);
}

void OLED_ShowMPU(){
    uint16_t int_pitch=(uint16_t) pitch;
    uint16_t int_yaw  =(uint16_t) yaw;
    uint16_t int_roll =(uint16_t) roll;

    /********* Integer part*******/
    OLED_ShowNum(50,0,(uint16_t) (pitch+180),3,size);
    OLED_ShowNum(50,1,(uint16_t) (yaw+180)  ,3,size);
    OLED_ShowNum(50,2,(uint16_t) (roll+180) ,3,size);
    /********* Decimal part*******/
    OLED_ShowNum(75,0,(uint16_t) ((pitch - int_pitch)*1000),3,size);
    OLED_ShowNum(75,1,(uint16_t) ((yaw - int_yaw)*1000    ),3,size);
    OLED_ShowNum(75,2,(uint16_t) ((roll - int_roll)*1000  ),3,size);
}
