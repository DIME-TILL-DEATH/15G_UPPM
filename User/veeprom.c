#include "veeprom.h"

SaveData deviceData;

void VEEPROM_Init()
{
    deviceData = *(__IO SaveData *)SAVE_PAGE_ADDR;
}

SaveData VEEPROM_GetSavedData()
{
     return deviceData;
}

void VEEPROM_SaveData(const SaveData* dataStruct)
{
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV2;
    __disable_irq();
    USART_Printf_Init(115200);

    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR);

    FLASH_Status result = FLASH_ErasePage(SAVE_PAGE_ADDR); //Erase 4KB

    uint32_t Address = SAVE_PAGE_ADDR;

    int16_t dataSize = sizeof(*dataStruct);

    uint16_t* dataStruct_ptr = (uint16_t*)dataStruct;

    while((dataSize > 0) && (result == FLASH_COMPLETE))
    {
        uint16_t save2bytes = *dataStruct_ptr;

        result = FLASH_ProgramHalfWord(Address, save2bytes);
        Address = Address + 2;
        dataSize = dataSize - 2;
        dataStruct_ptr = dataStruct_ptr + 1;
    }

    FLASH_Lock();

    RCC->CFGR0 &= ~(uint32_t)RCC_HPRE_DIV2;
    __enable_irq();
    USART_Printf_Init(115200);
}
