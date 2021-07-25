/*
 * uart.c
 *
 *  Created on: 2021Äê7ÔÂ12ÈÕ
 *      Author: FYT
 */
#include "uart_config.h"

//definitions for data_recv
uint8_t receivedData = 0x00;

static const unsigned long dv[] = {
//  4294967295      // 32 bit unsigned max
        1000000000, // +0
        100000000, // +1
        10000000, // +2
//  8388607         // 24 bit unsigned max
        1000000, // +3
        100000, // +4
//  65535           // 16 bit unsigned max
        10000, // +5
        1000, // +6
//  255             // 8 bit unsigned max
        100, // +7
        10, // +8
        1, // +9
};

/*uses SMCLK:24MHz for Baud:115200*/
void UART2_INIT(void){
    /* Selecting P3.2(RX) and P3.3(TX) in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
             GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    UCA2CTLW0 |= UCSWRST;//software reset enable, put the state machine in reset
    UCA2CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;//0,1,2:UCLK, ACLK, SMCLK

/* setting the Baudrate of UART; N= division factor= UCAxBR1|UCAxBR0(int)+UCAxCTLW0(decimal)
 * decimal part should be converted into binary system
 * decimal*8-->UCAxMCTLW0
 * ref:https://blog.csdn.net/grublyboy/article/details/9664009?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-0&spm=1001.2101.3001.4242
 */
    UCA2BR1   = 0x00;    //High
    UCA2BR0   = 0xD0;    //Low
    UCA2MCTLW = 0x04;    //for controlling the baudrate//number of "1" in the decimal part(binary)

    UCA2CTLW0 &= ~UCSWRST; //initialize USCI state machine

    //RX interrupt Enable; for TX, register is UCTXIE
    UCA2IE |= UCRXIE;
}


void uart_char(uint32_t moduleInstance, uint8_t charac){
    MAP_UART_transmitData(moduleInstance, charac);
    while (!MAP_UART_getInterruptStatus(moduleInstance,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG));
}

void uart_str(uint32_t moduleInstance, unsigned char *send_buf){
    while(*send_buf){
        MAP_UART_transmitData(moduleInstance, (*send_buf));
        while (!MAP_UART_getInterruptStatus(moduleInstance,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG));
        send_buf++;
    }
    MAP_UART_transmitData(moduleInstance, '\n');
    MAP_UART_transmitData(moduleInstance, '\r');
}

void uart_hex(uint32_t moduleInstance, unsigned n) {
    static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
            '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uart_char(moduleInstance, hex[n & 15]);
}

void uart_printf(uint32_t moduleInstance, char *fmt, ...){
  char      uart0_pString[101];
  va_list   uart0_ap;            //Define a va_list-type variable, which is a pointer to a parameter
  va_start (uart0_ap, fmt);      //Initialize the variables
  vsnprintf(uart0_pString, 100, fmt, uart0_ap);
  uart_str (moduleInstance, uart0_pString);
  va_end   (uart0_ap);              //END
}

//******************************************************************************
//This is the EUSCIA0 interrupt vector service routine.
//******************************************************************************
void EUSCIA2_IRQHandler(void){
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG){
        UART_TX();
    }

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        UART_TX();
    }
}

void UART_RX(void){
    receivedData = MAP_UART_receiveData(EUSCI_A2_BASE);
    uart_str(EUSCI_A2_BASE, "received");
}
void UART_TX(void){

}


