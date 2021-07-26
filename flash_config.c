/*
 * flash_config.c
 *
 *  Created on: 2021Äê7ÔÂ25ÈÕ
 *      Author: FYT
 */
#include "flash_config.h"

uint8_t readData[8192];

uint32_t    Bank_x   = 0;
uint32_t    Sector_x = 0;

/* notes:
 * The Flash memory on the device consists of two independent equal sized memory banks:
 * Main Memory region: primary code memory and is intended for code/data for the user application
 *                     256 KB (mapped from 0h through 3_FFFFh)
 *                     Bank0: 0h - 1_FFFFh | Bank1: 2_0000h - 3_FFFFh
 * Information Memory: for TI or customer code/data
 *                     16 KB, (mapped from 20_0000h through 20_3FFFh)
 *                     Bank0: 20_0000h - 20_1FFFh | Bank1: 20_2000h - 20_3FFFh
 * Sector            : each sector-->4096B-->1000H
 */
void Address_Mapping(uint32_t dest){
    //for futher sector selecting
    uint8_t sector_flag=0;

    //if dest between 0h and 3_FFFFh
    if((dest >> 18) == 0){
        switch ((dest >> 17) & 0x01){
            case 0:
                Bank_x = FLASH_MAIN_MEMORY_SPACE_BANK0;
                sector_flag = 0x00;
                break;
            case 1:
                Bank_x = FLASH_MAIN_MEMORY_SPACE_BANK1;
                sector_flag = 0x20;
                break;

            default:
                break;
        }
    }
    //0010 0000 0011 1111 1111 1111
    //0010 0000 00
    //1000 0000
    else if((dest >> 14) == 0x80){
        switch ((dest >> 13) & 0x01){
            case 0:
                Bank_x = FLASH_INFO_MEMORY_SPACE_BANK0;
                sector_flag = 0x00;
                break;
            case 1:
                Bank_x = FLASH_INFO_MEMORY_SPACE_BANK1;
                sector_flag = 0x02;
                break;

            default:
                break;
        }
    }

    switch((dest >> 12) & 0xff - (sector_flag & 0xff)){
        case 0 : Sector_x =  FLASH_SECTOR0; break;
        case 1 : Sector_x =  FLASH_SECTOR1; break;
        case 2 : Sector_x =  FLASH_SECTOR2; break;
        case 3 : Sector_x =  FLASH_SECTOR3; break;

        case 4 : Sector_x =  FLASH_SECTOR4; break;
        case 5 : Sector_x =  FLASH_SECTOR5; break;
        case 6 : Sector_x =  FLASH_SECTOR6; break;
        case 7 : Sector_x =  FLASH_SECTOR7; break;

        case 8 : Sector_x =  FLASH_SECTOR8; break;
        case 9 : Sector_x =  FLASH_SECTOR9; break;
        case 10: Sector_x = FLASH_SECTOR10; break;
        case 11: Sector_x = FLASH_SECTOR11; break;

        case 12: Sector_x = FLASH_SECTOR12; break;
        case 13: Sector_x = FLASH_SECTOR13; break;
        case 14: Sector_x = FLASH_SECTOR14; break;
        case 15: Sector_x = FLASH_SECTOR15; break;

        case 16: Sector_x = FLASH_SECTOR16; break;
        case 17: Sector_x = FLASH_SECTOR17; break;
        case 18: Sector_x = FLASH_SECTOR18; break;
        case 19: Sector_x = FLASH_SECTOR19; break;

        case 20: Sector_x = FLASH_SECTOR20; break;
        case 21: Sector_x = FLASH_SECTOR21; break;
        case 22: Sector_x = FLASH_SECTOR22; break;
        case 23: Sector_x = FLASH_SECTOR23; break;

        case 24: Sector_x = FLASH_SECTOR24; break;
        case 25: Sector_x = FLASH_SECTOR25; break;
        case 26: Sector_x = FLASH_SECTOR26; break;
        case 27: Sector_x = FLASH_SECTOR27; break;

        case 28: Sector_x = FLASH_SECTOR28; break;
        case 29: Sector_x = FLASH_SECTOR29; break;
        case 30: Sector_x = FLASH_SECTOR30; break;
        case 31: Sector_x = FLASH_SECTOR31; break;

        default: break;
    }
}

/*function for flash programming
 * input:
 *   src   : Pointer to the data source to program into flash
 *   dest  : Pointer to the destination in flash to program
 *   length: Length in bytes to program
 *
 */
void flash_programming(uint32_t src, uint32_t dest, uint32_t length){
    Address_Mapping(dest);
    /* Unprotecting Info Bank_memorySpace, Sector_sectorMask  */
    MAP_FlashCtl_unprotectSector(Bank_x,Sector_x);

    /* Trying to program the memory. Within this function, the API will
     * automatically try to program the maximum number of tries. If it fails,
     * trap inside an infinite loop */
    if(!MAP_FlashCtl_programMemory((void*)src, (void*)dest, length))
        while(1);

    /* Setting the sector back to protected  */
    MAP_FlashCtl_protectSector(Bank_x,Sector_x);
}


/*notes:
 *  the minimum allowed size that can be erased is a flash sector (which is 4KB on the MSP432 family)
 *  If an address is provided to this function which is not on a 4KB boundary, the entire sector will still be erased.
 *
 */
//erasing the dest-directing block
void flash_erasing(uint_fast8_t dest){
    Address_Mapping(dest);
    /* Unprotecting Info Bank_memorySpace, Sector_sectorMask  */
    MAP_FlashCtl_unprotectSector(Bank_x,Sector_x);

    /* Trying a mass erase. This should erase the sector that is unprotected.
     *  If it fails, trap inside an infinite loop
     */
    if(!MAP_FlashCtl_performMassErase())
        while(1);

    /* Setting  sectorMask back to protected  */
    MAP_FlashCtl_protectSector(Bank_x,Sector_x);

}


uint8_t flash_read_word(uint32_t address){
    uint8_t data = *(uint8_t *) (address);
    return data;
}

void flash_reading(uint32_t dest, uint16_t length){
    uint8_t dataIndex;
    for(dataIndex = 0; dataIndex < length; dataIndex ++){
         readData[dataIndex] = flash_read_word(dest + dataIndex);
    }
}

