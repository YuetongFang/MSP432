/*
 * flash_config.c
 *
 *  Created on: 2021Äê7ÔÂ25ÈÕ
 *      Author: FYT
 */
#include "flash_config.h"

uint16_t readData[8192];

/*function for flash programming
 * input:
 *   src   : Pointer to the data source to program into flash
 *   dest  : Pointer to the destination in flash to program
 *   length: Length in bytes to program
 *
 */
void flash_programming(void* src, void* dest, uint32_t length, uint_fast8_t memorySpace, uint32_t sectorMask){
    /* Unprotecting Info Bank_memorySpace, Sector_sectorMask  */
    MAP_FlashCtl_unprotectSector(memorySpace,sectorMask);

    /* Trying to program the memory. Within this function, the API will
     * automatically try to program the maximum number of tries. If it fails,
     * trap inside an infinite loop */
    if(!MAP_FlashCtl_programMemory(src,dest, length))
        while(1);

    /* Setting the sector back to protected  */
    MAP_FlashCtl_protectSector(memorySpace,sectorMask);
}


/*notes:
 *  the minimum allowed size that can be erased is a flash sector (which is 4KB on the MSP432 family)
 *  If an address is provided to this function which is not on a 4KB boundary, the entire sector will still be erased.
 *
 */
void flash_erasing(uint_fast8_t memorySpace, uint32_t sectorMask){
    /* Unprotecting Info Bank_memorySpace, Sector_sectorMask  */
    MAP_FlashCtl_unprotectSector(memorySpace,sectorMask);

    /* Trying a mass erase. This should erase the sector that is unprotected.
     *  If it fails, trap inside an infinite loop
     */
    if(!MAP_FlashCtl_performMassErase())
        while(1);

    /* Setting  sectorMask back to protected  */
    MAP_FlashCtl_protectSector(memorySpace,sectorMask);

}

//Normal read mode
void flash_read_init(uint_fast8_t memorySpace, uint16_t length){
    FlashCtl_setReadMode(memorySpace, FLASH_NORMAL_READ_MODE);
}


uint16_t flash_read_word(uint32_t address){
    return *(__IO uint16_t*)address;
}

void flash_reading(uint_fast8_t memorySpace, uint_fast8_t start_sector, uint_fast8_t accessMethod, uint16_t length){
    //accessMethod: FLASH_DATA_READ, FLASH_INSTRUCTION_FETCH
    FlashCtl_enableReadBuffering(memorySpace, accessMethod);

    uint16_t dataIndex;
    for(dataIndex = 0; dataIndex < length; dataIndex ++){
         readData[dataIndex] = flash_read_word(start_sector + dataIndex*2);
    }

    FlashCtl_disableReadBuffering(memorySpace, accessMethod);
}
