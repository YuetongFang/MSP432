/*
 * interrupt_config.c
 *
 *  Created on: 2021Äê7ÔÂ27ÈÕ
 *      Author: FYT
 */

// Interrupt Source resolution
unsigned int   __even_in_range(unsigned int val, unsigned int range){
    if (val & 0x01 ==1)
        return 0;
    else if(val <= range)
        return val;
    else return 0;
}

/* Configuring interrupt priorities
 * Vector No., Priority level (the smaller, the higher)
 * MAP_Interrupt_setPriority(INT_EUSCIB0, 0x20);
 */

/* Setting the priority mask, masking any interrupts below the setting Interrupt priority level
 * Priority level
 * MAP_Interrupt_setPriorityMask(0x40);
 *
 * To cancel:
 * MAP_Interrupt_setPriorityMask(0);
 */

/* Setting the priority grouping of the interrupt controller (preemptable priority levels)
 * 3 bits are available , values of 3-7 have the same effect.(maximum: 7 levels for preemptable priority)
 * The high priority preemption priority CAN INTERRUPT the ongoing low priority preemption interrupt
 * For interrupts with the same preemptive priority, high response priority CANNOT INTERRUPT interrupts with low response priority
 * For interrupts with the same preemptive priority, when two interrupts occur at the same time, which response priority is higher and which is executed first
 *
 * 1. Groupping: NVIC_SetPriority(PriorityGroup);
 * 2. Get the Priority code: NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
 * 3. NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority) (priority -->2)
 * 4. NVIC_EnableIRQ(IRQn_Type IRQn)
 *
 * 5. NVIC_DisableIRQ(IRQn_Type IRQn)
 */

/* Global Interrupt enable
 * Interrupt_enableMaster (void)
 */

/* Add User Interrupt.
 * In most cases, this is done through means of the user setting the ISR function pointer in the startup file
 * Interrupt_registerInterrupt (uint32_t interruptNumber, void(*intHandler)(void))
 */

/* Enable a specific interrupt
 * Priority level
 * Interrupt_enableInterrupt(Priority level)
 */

