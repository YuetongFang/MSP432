/*
 * adc_config.c
 *
 *  Created on: 2021Äê7ÔÂ28ÈÕ
 *      Author: FYT
 */

#include "adc_config.h"

/* ADC Clock Source: MODCLK, SYSCLK, ACLK, MCLK, SMCLK, HSMCLK
 *          Divider: 1, 4, 32, 64
 *           VeREF-: recommended to be connected to Board Ground
 */

/* Get temperature from the internal temperature sensor
 * Set ADC14TCMAP to 1 to enable the temperature sensor input channel
 * Set REFON bit to 1(in REF module) for using the temperature sensor
 * Select the analog input channel ADC14INCHx=MAX-1(MAX == maximum number of external ADC input channels(start at 0)
 * Temperature sensor is in the REF Module
 * Sample period must be greater than 5us
 */

volatile float tempC;
volatile float tempF;

/* ADC configuration
 * 1.  Set REF voltage
 *          MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V); (1_2V 1_45V 2_5V
 *
 * 2.  Initialize ADC module
 *          MAP_ADC14_enableModule();
 *          MAP_ADC14_initModule(clockSource, clockPredivider, clockDivider,internalChannelMask);
 *          Final divided clock: PreDiv*ClkDiv; Channel: configures the pin mapping(which channel?)
 *
 * 3.  Configuring GPIOs for Analog In
 *          MAP_GPIO_setAsPeripheralModuleFunctionInputPin()
 *
 * 4.  Configuring ADC Memory
 *        **Config the Memory for further usage
 *          MAP_ADC_configure xxx Mode(memoryStart, memoryEnd, repeatMode)
 *        **Then config the distribution
 *          MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A0, false);
 *                                              MemorySelect, VoltageReferenceSelect, ChannelSelect, DifferentialModeSelect
 *          Since the results will be stored continuously in the selected memory, it would be better to distribute an independent memory for each channel respectively
 *
 * 5*.  Set the comparator window to trigger interrupts when input voltage is not within the configured window
 *           MAP_ADC14_setComparatorWindowValue(ADC_COMP_WINDOW0, low, high);
 *           MAP_ADC14_enableComparatorWindow(ADC_MEM0, ADC_COMP_WINDOW0);
 *           MAP_ADC14_enableInterrupt(ADC_HI_INT | ADC_LO_INT);
 *           MAP_ADC14_clearInterruptFlag(ADC_HI_INT | ADC_LO_INT);
 *
 * 6*.  Enabling the FPU with stacking enabled (for use within ISR)
 *          FPU_enableModule();
 *          FPU_enableLazyStacking();
 *
 * 7*. Set the internal timer as sample trigger source
 *     ADC would be setup to continuously sample/convert from timer when the trigger starts, and store the results in resultsBuffer (it is setup to be a circular buffer where resPos overflows to 0).
 *          MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false); //The wanted trigger source, Bool invertSignal
 *
 * 8*. Enabling the interrupt (happens every time a conversion has completed)
 *          MAP_ADC14_enableInterrupt(ADC_INT7);     (Enable the channel to pend ADC14 interrupt)
 *          MAP_Interrupt_enableInterrupt(INT_ADC14);(Enable ADC14 Interrupt)
 *
 * 8.  Triggering the start of the sample
 *          MAP_ADC14_enableConversion();
 *          MAP_ADC14_toggleConversionTrigger();
 */
void ADC_Temperature_Init(){
    /* Enabling the FPU with stacking enabled (for use within ISR) */
    FPU_enableModule();
    FPU_enableLazyStacking();

    /* Initializing ADC (MCLK/1/1) with temperature sensor routed */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,ADC_TEMPSENSEMAP);

    /* Configuring ADC Memory (ADC_MEM0 A22 (Temperature Sensor) in repeatmode).*/
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A22, false);

    /* Configuring the sample/hold time for TBD */
    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192,ADC_PULSE_WIDTH_192);

    /* Enabling sample timer in auto iteration mode and interrupts*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableInterrupt(ADC_INT0);

    /* Setting reference voltage to 2.5 and enabling temperature sensor */
    REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    REF_A_enableReferenceVoltage();
    REF_A_enableTempSensor();

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

/* This interrupt happens every time a conversion has completed. Since the FPU
 * is enabled in stacking mode, we are able to use the FPU safely to perform
 * efficient floating point arithmetic.*/
void ADC14_IRQHandler(void)
{
    uint64_t status;
    uint32_t cal30, cal85;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT0)
    {
        cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF,
                SYSCTL_30_DEGREES_C);
        cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF,
                SYSCTL_85_DEGREES_C);

        tempC = (float) (((uint32_t) ADC14_getResult(ADC_MEM0) - cal30) * (85 - 30))
                / (cal85 - cal30) + 30.0f;
        tempF = tempC * 9.0f / 5.0f + 32.0f;
    }
    uart_printf(EUSCI_A2_BASE,"Tem: %f",tempC);

}


