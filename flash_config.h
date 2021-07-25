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

void flash_programming(void* src, void* dest, uint32_t length, uint_fast8_t memorySpace, uint32_t sectorMask);
void flash_erasing(uint_fast8_t memorySpace, uint32_t sectorMask);
void flash_read_init(uint_fast8_t memorySpace, uint16_t length);
uint16_t flash_read_word(uint32_t address);
void flash_reading(uint_fast8_t memorySpace, uint_fast8_t start_sector, uint_fast8_t accessMethod, uint16_t length);

#endif /* FLASH_CONFIG_H_ */
