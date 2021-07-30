/*
 * dma_config.c
 *
 *  Created on: 2021Äê7ÔÂ29ÈÕ
 *      Author: FYT
 */

#include "dma_config.h"

/* notes for DMA:
 * 1. Configuring DMA module
 *      Define a uint8_t controlTable[]
 *          //DMA Control Table
 *          #ifdef ewarm
 *          #pragma data_alignment=1024
 *          #else
 *          #pragma DATA_ALIGN(controlTable, 1024)
 *          #endif
 *          uint8_t controlTable[]
 *      MAP_DMA_enableModule()
 *      MAP_DMA_setControlBase(controlTable);//Sets the base address for the channel control table
 *
 * 2. Setting Control Indexes
 *      (For parameter introduction: https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP432_Driver_Library/latest/exports/driverlib/msp432_driverlib_3_21_00_05/doc/MSP432P4xx/html/driverlib_html/group__dma__api.html#ga729f47068803721bb5e6ee24f8ea1a38)
 *                                Primary or alternate control structure
 *      MAP_DMA_setChannelControl (UDMA_PRI_SELECT,  UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1024); channelStructIndex, control
 *      MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT,  UDMA_MODE_AUTO, Source_Addr, Dest_Addr, transferSize);
 *
 * 3. Assigning/Enabling Interrupts
 *      MAP_DMA_assignInterrupt(DMA_INT1, 0);//INT_No., Channel
 *      MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
 *
 * 4* Mapping a Peripheral for a DMA Channel
 *      This function assigns a peripheral mapping to a DMA channel.
 *      It is used to select which peripheral is used for a DMA channel.
 *      The parameter mapping should be one of the macros named UDMA_CHn_tttt from the header file dma.h.
 *      For example, to assign DMA channel 0 to the eUSCI AO RX channel, the parameter should be the macro UDMA_CH1_EUSCIA0RX.
 *
 *      DMA_assignChannel(uint32_t  Peripheral_Macro)
 *
 *      Before Mapping, disable the corresponding channel attributes
 *      MAP_DMA_disableChannelAttribute(DMA_CH2_EUSCIB1TX0,   UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |UDMA_ATTR_HIGH_PRIORITY |UDMA_ATTR_REQMASK);
 *
 * 5. Enabling DMA Channel
 *       MAP_DMA_enableChannel(0);
 *       For Channel Configuring, use:
 *       void DMA_enableChannelAttribute(uint32_t channelNum, uint32_t attr);
 *
 * 6. Forcing a software transfer on DMA Channel 0
 *       Initializes a software transfer of the corresponding DMA channel.
 *       This is done if the user wants to force a DMA on the specified channel without the hardware precondition.
 *       Specific channels can be configured using the DMA_assignChannel function.
 *
 *       MAP_DMA_requestSoftwareTransfer(0);
 *
 * 7. Program the DMA_INT1_IRQHandler
 *       MAP_DMA_disableChannel(0); .......
 */
