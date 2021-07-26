/*
 * flash_config.h
 *
 *  Created on: 2021Äê7ÔÂ25ÈÕ
 *      Author: FYT
 */
#include <stdint.h>
#include "MSP432P4xx/driverlib.h"
#include "msp.h"

#ifndef FLASH_CONFIG_H_
#define FLASH_CONFIG_H_

void     Address_Mapping(uint32_t dest);

void     flash_programming(uint32_t src, uint32_t dest, uint32_t length);

//erasing the dest-directing block
void     flash_erasing(uint_fast8_t dest);

void     flash_read_init(uint_fast8_t memorySpace, uint16_t length);
uint8_t  flash_read_word(uint32_t address);
void     flash_reading(uint32_t dest, uint16_t length);

/* For flash reading:
 *      extern uint8_t readData[8192];
 * For flash writing
 *      uint8_t writeData[4096];
 * Define an address for flash operations
 *      #define Flash_start_adress 0x0003F000
 *
 * Examples:
 *      flash_programming((uint32_t) writeData, Flash_start_adress, length);
 *      flash_reading((uint32_t) Flash_start_adress, length);
 */

#endif /* FLASH_CONFIG_H_ */
