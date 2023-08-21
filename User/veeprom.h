#ifndef USER_VEEPROM_H_
#define USER_VEEPROM_H_

#include <debug.h>

#define SAVE_PAGE_ADDR  ((uint32_t)0x0800F000) /* Start from 60K */

typedef struct
{
    uint16_t serial;
    uint8_t dataByte;
    uint8_t ppmNumber;
}SaveData;

void VEEPROM_Init();

SaveData VEEPROM_GetSavedData();
void VEEPROM_SaveData(const SaveData* dataStruct);

#endif /* USER_VEEPROM_H_ */
