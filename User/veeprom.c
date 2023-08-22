#include "veeprom.h"

uint8_t userConfig[64] __attribute__ ((section(".user_data")));

SaveData deviceData;

uint32_t userDataStartAddress = (uint32_t)&_user_data_start;

void VEEPROM_Init()
{
    userDataStartAddress += 0x8000000;

    deviceData = *(__IO SaveData *)userDataStartAddress;

    printf("user data start addr: %x, end addr: %x\r\n", userDataStartAddress, &_user_data_end);
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

    FLASH_Status result = FLASH_ErasePage(userDataStartAddress); //Erase 4KB

    uint32_t Address = userDataStartAddress;

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
